#include "jd9613_display.h"
#include "esphome/core/application.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <cstdio>

namespace esphome {
namespace jd9613 {

static const uint16_t SPI_SETUP_US = 100;         // estimated fixed overhead in microseconds for an SPI write
static const uint16_t SPI_MAX_BLOCK_SIZE = 4092;  // Max size of continuous SPI transfer

// store a 16 bit value in a buffer, big endian.
static inline void put16_be(uint8_t *buf, uint16_t value) {
  buf[0] = value >> 8;
  buf[1] = value;
}

JD9613Display::JD9613Display()
  :init_sequence_{INITCMD_JD9613}
  ,width_(588)
  ,height_(126)
{
  uint8_t cmd, num_args, bits;
  const uint8_t *addr = init_sequence_;
  while ((cmd = *addr++) != 0) {
    num_args = *addr++;
    if (num_args == JD9613_DELAY_FLAG)
      continue;
    bits = *addr;
    switch (cmd) {
      case JD9613_MADCTL: {
        this->swap_xy_ = (bits & MADCTL_MV) != 0;
        this->mirror_x_ = (bits & MADCTL_MX) != 0;
        this->mirror_y_ = (bits & MADCTL_MY) != 0;
        this->color_order_ = (bits & MADCTL_BGR) ? display::COLOR_ORDER_BGR : display::COLOR_ORDER_RGB;
        break;
      }

      case JD9613_PIXFMT: {
        if ((bits & 0xF) == 6)
          this->is_18bitdisplay_ = true;
        break;
      }

      default:
        break;
    }
    addr += (num_args & 0x7F);
  }

  // Orientation according to Lilygo T-Touchbar-AMOLED, which has two JD9613
  // screen of size 126x294 combined to form one virtual screen of size 588x126.
  //
  // Screen 0 (select by CS0) is rotated (from potrait to landscape) 90 degree
  // clockwise to form the right half of the virtual screen.
  //
  // Screen 1 (select by CS1) is rotated 90 degree counter-clockwise to form
  // the left half of the virtual screen.
  //
  left_panel_ = new JD9613Display(126, 294);
  left_panel_->set_rotation(display::DISPLAY_ROTATION_270_DEGREES);
  right_panel_ = new JD9613Display(126, 294);
  right_panel_->set_rotation(display::DISPLAY_ROTATION_90_DEGREES);
}


JD9613Display::JD9613Display(int16_t width, int16_t height)
  : width_{width}, height_{height}
{
}

void JD9613Display::set_madctl() {
  // custom x/y transform and color order
  uint8_t mad = this->color_order_ == display::COLOR_ORDER_BGR ? MADCTL_BGR : MADCTL_RGB;
  if (this->swap_xy_)
    mad |= MADCTL_MV;
  if (this->mirror_x_)
    mad |= MADCTL_MX;
  if (this->mirror_y_)
    mad |= MADCTL_MY;
  this->command(JD9613_MADCTL);
  this->data(mad);
  esph_log_d(TAG, "Wrote MADCTL 0x%02X", mad);
}

void JD9613Display::setup() {
  this->x_low_ = this->width_;
  this->y_low_ = this->height_;
  this->x_high_ = 0;
  this->y_high_ = 0;

  if (!is_composite())
    return;

  ESP_LOGD(TAG, "Setting up jd9613");

  this->setup_pins_();

  cs0_pin_->digital_write(false);
  cs1_pin_->digital_write(false);

  this->init_lcd_(this->init_sequence_);
  this->init_lcd_(this->extra_init_sequence_.data());

  switch (this->pixel_mode_) {
    case PIXEL_MODE_16:
      if (this->is_18bitdisplay_) {
        this->command(JD9613_PIXFMT);
        this->data(0x55);
        this->is_18bitdisplay_ = false;
      }
      break;
    case PIXEL_MODE_18:
      if (!this->is_18bitdisplay_) {
        this->command(JD9613_PIXFMT);
        this->data(0x66);
        this->is_18bitdisplay_ = true;
      }
      break;
    default:
      break;
  }

  this->set_madctl();
  this->command(this->pre_invertcolors_ ? JD9613_INVON : JD9613_INVOFF);
  cs0_pin_->digital_write(true);
  cs1_pin_->digital_write(true);

  left_panel_->pixel_mode_ =
  right_panel_->pixel_mode_ = pixel_mode_;
  left_panel_->buffer_color_mode_ =
  right_panel_->buffer_color_mode_ = buffer_color_mode_;
  left_panel_->color_order_ =
  right_panel_->color_order_ = color_order_;
  left_panel_->is_18bitdisplay_ =
  right_panel_->is_18bitdisplay_ = is_18bitdisplay_;
  left_panel_->palette_ =
  right_panel_->palette_ = palette_;

  left_panel_->setup();
  right_panel_->setup();
}

void JD9613Display::alloc_buffer_() {
  if (is_composite())
    return;

  if (this->buffer_color_mode_ == BITS_16) {
    this->init_internal_(this->get_buffer_length_() * 2);
    if (this->buffer_ != nullptr) {
      return;
    }
    this->buffer_color_mode_ = BITS_8;
  }
  this->init_internal_(this->get_buffer_length_());
  if (this->buffer_ == nullptr) {
    this->mark_failed();
  }
}

void JD9613Display::setup_pins_() {
  if (!is_composite())
    return;

  this->dc_pin_->setup();  // OUTPUT
  this->dc_pin_->digital_write(false);
  left_panel_->dc_pin_ =
  right_panel_->dc_pin_ = this->dc_pin_;

  this->cs0_pin_->setup();  // OUTPUT
  this->cs0_pin_->digital_write(false);

  this->cs1_pin_->setup();  // OUTPUT
  this->cs1_pin_->digital_write(false);

  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->setup();  // OUTPUT
    this->reset_pin_->digital_write(true);
  }

  this->spi_setup();

  left_panel_->set_spi_parent(this->parent_);
  left_panel_->set_data_rate(this->data_rate_);
  left_panel_->spi_setup();
  right_panel_->set_spi_parent(this->parent_);
  right_panel_->set_data_rate(this->data_rate_);
  right_panel_->spi_setup();

  this->reset_();
}

void JD9613Display::dump_config() {
  if (is_composite()) {
    _dump_config("jd9613");
    left_panel_->_dump_config("left panel");
    right_panel_->_dump_config("right panel");
  }
}

void JD9613Display::_dump_config(const char *name)
{
  ESP_LOGCONFIG(TAG, "%s", name);
  ESP_LOGCONFIG(TAG, "  Rotations: %d °", rotation_);
  ESP_LOGCONFIG(TAG, "  Dimensions: %dpx x %dpx", get_width(), get_height());

  ESP_LOGCONFIG(TAG, "  Width Offset: %u", this->offset_x_);
  ESP_LOGCONFIG(TAG, "  Height Offset: %u", this->offset_y_);
  ESP_LOGCONFIG(TAG, "  Data rate: %dMHz", (unsigned) (this->data_rate_ / 1000000));

  LOG_PIN("  Reset Pin: ", this->reset_pin_);
  LOG_PIN("  DC Pin: ", this->dc_pin_);
  LOG_PIN("  CS0 Pin: ", this->cs0_pin_);
  LOG_PIN("  CS1 Pin: ", this->cs1_pin_);
  ESP_LOGCONFIG(TAG, "  Color order: %s", this->color_order_ == display::COLOR_ORDER_BGR ? "BGR" : "RGB");
  ESP_LOGCONFIG(TAG, "  Swap_xy: %s", YESNO(this->swap_xy_));
  ESP_LOGCONFIG(TAG, "  Mirror_x: %s", YESNO(this->mirror_x_));
  ESP_LOGCONFIG(TAG, "  Mirror_y: %s", YESNO(this->mirror_y_));
  ESP_LOGCONFIG(TAG, "  Invert colors: %s", YESNO(this->pre_invertcolors_));

  if (this->is_failed()) {
    ESP_LOGCONFIG(TAG, "  => Failed to init Memory: YES!");
  }
  LOG_UPDATE_INTERVAL(this);
}

float JD9613Display::get_setup_priority() const { return setup_priority::HARDWARE; }

void JD9613Display::fill(Color color) {
  if (is_composite()) {
    left_panel_->fill(color);
    right_panel_->fill(color);
    return;
  }

  if (!this->check_buffer_())
    return;
  uint16_t new_color = 0;
  this->x_low_ = 0;
  this->y_low_ = 0;
  this->x_high_ = this->get_width_internal() - 1;
  this->y_high_ = this->get_height_internal() - 1;
  switch (this->buffer_color_mode_) {
    case BITS_8_INDEXED:
      new_color = display::ColorUtil::color_to_index8_palette888(color, this->palette_);
      break;
    case BITS_16:
      new_color = display::ColorUtil::color_to_565(color);
      {
        const uint32_t buffer_length_16_bits = this->get_buffer_length_() * 2;
        if (((uint8_t) (new_color >> 8)) == ((uint8_t) new_color)) {
          // Upper and lower is equal can use quicker memset operation. Takes ~20ms.
          memset(this->buffer_, (uint8_t) new_color, buffer_length_16_bits);
        } else {
          for (uint32_t i = 0; i < buffer_length_16_bits; i = i + 2) {
            this->buffer_[i] = (uint8_t) (new_color >> 8);
            this->buffer_[i + 1] = (uint8_t) new_color;
          }
        }
      }
      return;
    default:
      new_color = display::ColorUtil::color_to_332(color, display::ColorOrder::COLOR_ORDER_RGB);
      break;
  }
  memset(this->buffer_, (uint8_t) new_color, this->get_buffer_length_());
}

void HOT JD9613Display::draw_absolute_pixel_internal(int x, int y, Color color) {
  if (x >= this->get_width_internal() || x < 0 || y >= this->get_height_internal() || y < 0) {
    return;
  }

  if (is_composite()) {
    const int16_t half = width_ >> 1;
    if (x >= half) {
      // ESP_LOGV(TAG, "Draw pixel %d, %d, %lx", x - half, y, color.raw_32);
      right_panel_->draw_pixel_at(x - half, y, color);
    }
    else {
      // ESP_LOGV(TAG, "Draw pixel %d, %d, %lx", x, y, color.raw_32);
      left_panel_->draw_pixel_at(x, y, color);
    }
    return;
  }

  if (!this->check_buffer_())
    return;
  // ESP_LOGV(TAG, "pixel at %d, %d", x, y);
  uint32_t pos = (y * width_) + x;
  uint16_t new_color;
  bool updated = false;
  switch (this->buffer_color_mode_) {
    case BITS_8_INDEXED:
      new_color = display::ColorUtil::color_to_index8_palette888(color, this->palette_);
      break;
    case BITS_16:
      pos = pos * 2;
      new_color = display::ColorUtil::color_to_565(color, display::ColorOrder::COLOR_ORDER_RGB);
      if (this->buffer_[pos] != (uint8_t) (new_color >> 8)) {
        this->buffer_[pos] = (uint8_t) (new_color >> 8);
        updated = true;
      }
      pos = pos + 1;
      new_color = new_color & 0xFF;
      break;
    default:
      new_color = display::ColorUtil::color_to_332(color, display::ColorOrder::COLOR_ORDER_RGB);
      break;
  }

  if (this->buffer_[pos] != new_color) {
    this->buffer_[pos] = new_color;
    updated = true;
  }
  if (updated) {
    // low and high watermark may speed up drawing from buffer
    if (x < this->x_low_)
      this->x_low_ = x;
    if (y < this->y_low_)
      this->y_low_ = y;
    if (x > this->x_high_)
      this->x_high_ = x;
    if (y > this->y_high_)
      this->y_high_ = y;
  }
}

void JD9613Display::update() {
  if (this->prossing_update_) {
    this->need_update_ = true;
    return;
  }
  this->prossing_update_ = true;
  do {
    this->need_update_ = false;
    this->do_update_();
  } while (this->need_update_);
  this->prossing_update_ = false;

  if (is_composite()) {
    left_panel_->display_(cs1_pin_);
    right_panel_->display_(cs0_pin_);
  }
}

void JD9613Display::display_(GPIOPin *cs_pin) {
  // check if something was displayed
  if ((this->x_high_ < this->x_low_) || (this->y_high_ < this->y_low_)) {
    return;
  }

  cs_pin->digital_write(false);

  // Start addresses and widths/heights must be divisible by 2 (CASET/RASET restriction in datasheet)
  if (this->x_low_ % 2 == 1) {
    this->x_low_--;
  }
  if (this->x_high_ % 2 == 0) {
    this->x_high_++;
  }
  if (this->y_low_ % 2 == 1) {
    this->y_low_--;
  }
  if (this->y_high_ % 2 == 0) {
    this->y_high_++;
  }

  // we will only update the changed rows to the display
  size_t const w = this->x_high_ - this->x_low_ + 1;
  size_t const h = this->y_high_ - this->y_low_ + 1;

  size_t mhz = this->data_rate_ / 1000000;
  // estimate time for a single write
  size_t sw_time = this->width_ * h * 16 / mhz + this->width_ * h * 2 / SPI_MAX_BLOCK_SIZE * SPI_SETUP_US * 2;
  // estimate time for multiple writes
  size_t mw_time = (w * h * 16) / mhz + w * h * 2 / JD9613_TRANSFER_BUFFER_SIZE * SPI_SETUP_US;
  ESP_LOGV(TAG,
           "Start display(xlow:%d, ylow:%d, xhigh:%d, yhigh:%d, width:%d, "
           "height:%zu, mode=%d, 18bit=%d, sw_time=%zuus, mw_time=%zuus)",
           this->x_low_, this->y_low_, this->x_high_, this->y_high_, w, h, this->buffer_color_mode_,
           this->is_18bitdisplay_, sw_time, mw_time);
  auto now = millis();
  if (this->buffer_color_mode_ == BITS_16 && !this->is_18bitdisplay_ && sw_time < mw_time) {
    // 16 bit mode maps directly to display format
    ESP_LOGV(TAG, "Doing single write of %zu bytes", this->width_ * h * 2);
    set_addr_window_(0, this->y_low_, this->width_ - 1, this->y_high_);
    this->write_array(this->buffer_ + this->y_low_ * this->width_ * 2, h * this->width_ * 2);
  } else {
    ESP_LOGV(TAG, "Doing multiple write");
    uint8_t transfer_buffer[JD9613_TRANSFER_BUFFER_SIZE];
    size_t rem = h * w;  // remaining number of pixels to write
    set_addr_window_(this->x_low_, this->y_low_, this->x_high_, this->y_high_);
    size_t idx = 0;    // index into transfer_buffer
    size_t pixel = 0;  // pixel number offset
    size_t pos = this->y_low_ * this->width_ + this->x_low_;
    while (rem-- != 0) {
      uint16_t color_val;
      switch (this->buffer_color_mode_) {
        case BITS_8:
          color_val = display::ColorUtil::color_to_565(display::ColorUtil::rgb332_to_color(this->buffer_[pos++]));
          break;
        case BITS_8_INDEXED:
          color_val = display::ColorUtil::color_to_565(
              display::ColorUtil::index8_to_color_palette888(this->buffer_[pos++], this->palette_));
          break;
        default:  // case BITS_16:
          color_val = (this->buffer_[pos * 2] << 8) + this->buffer_[pos * 2 + 1];
          pos++;
          break;
      }
      if (this->is_18bitdisplay_) {
        transfer_buffer[idx++] = (uint8_t) ((color_val & 0xF800) >> 8);  // Blue
        transfer_buffer[idx++] = (uint8_t) ((color_val & 0x7E0) >> 3);   // Green
        transfer_buffer[idx++] = (uint8_t) (color_val << 3);             // Red
      } else {
        put16_be(transfer_buffer + idx, color_val);
        idx += 2;
      }
      if (idx == sizeof(transfer_buffer)) {
        this->write_array(transfer_buffer, idx);
        idx = 0;
        App.feed_wdt();
      }
      // end of line? Skip to the next.
      if (++pixel == w) {
        pixel = 0;
        pos += this->width_ - w;
      }
    }
    // flush any balance.
    if (idx != 0) {
      this->write_array(transfer_buffer, idx);
    }
  }
  this->end_data_();
  cs_pin->digital_write(true);
  ESP_LOGV(TAG, "Data write took %dms", (unsigned) (millis() - now));
  // invalidate watermarks
  this->x_low_ = this->width_;
  this->y_low_ = this->height_;
  this->x_high_ = 0;
  this->y_high_ = 0;
}

// should return the total size: return this->get_width_internal() * this->get_height_internal() * 2 // 16bit color
// values per bit is huge
uint32_t JD9613Display::get_buffer_length_() { return this->get_width_internal() * this->get_height_internal(); }

void JD9613Display::command(uint8_t value) {
  this->start_command_();
  this->write_byte(value);
  this->end_command_();
}

void JD9613Display::data(uint8_t value) {
  this->start_data_();
  this->write_byte(value);
  this->end_data_();
}

void JD9613Display::send_command(uint8_t command_byte, const uint8_t *data_bytes, uint8_t num_data_bytes) {
  this->command(command_byte);  // Send the command byte
  this->start_data_();
  this->write_array(data_bytes, num_data_bytes);
  this->end_data_();
}

void JD9613Display::start_command_() {
  this->dc_pin_->digital_write(false);
  this->enable();
}
void JD9613Display::start_data_() {
  this->dc_pin_->digital_write(true);
  this->enable();
}

void JD9613Display::end_command_() { this->disable(); }
void JD9613Display::end_data_() { this->disable(); }

void JD9613Display::reset_() {
  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->digital_write(false);
    delay(150);
    this->reset_pin_->digital_write(true);
    delay(150);
  }
}

void JD9613Display::init_lcd_(const uint8_t *addr) {
  if (addr == nullptr)
    return;
  uint8_t cmd, x, num_args;
  while ((cmd = *addr++) != 0) {
    x = *addr++;
    if (x == JD9613_DELAY_FLAG) {
      cmd &= 0x7F;
      ESP_LOGV(TAG, "Delay %dms", cmd);
      delay(cmd);
    } else {
      num_args = x & 0x7F;
      ESP_LOGV(TAG, "Command %02X, length %d, bits %02X", cmd, num_args, *addr);
      this->send_command(cmd, addr, num_args);
      addr += num_args;
      if (x & 0x80) {
        ESP_LOGV(TAG, "Delay 120ms");
        delay(120);  // NOLINT
      }
    }
  }
}

// Tell the display controller where we want to draw pixels.
void JD9613Display::set_addr_window_(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  x1 += this->offset_x_;
  x2 += this->offset_x_;
  y1 += this->offset_y_;
  y2 += this->offset_y_;
  this->command(JD9613_CASET);
  this->data(x1 >> 8);
  this->data(x1 & 0xFF);
  this->data(x2 >> 8);
  this->data(x2 & 0xFF);
  this->command(JD9613_PASET);  // Page address set
  this->data(y1 >> 8);
  this->data(y1 & 0xFF);
  this->data(y2 >> 8);
  this->data(y2 & 0xFF);
  this->command(JD9613_RAMWR);  // Write to RAM
  this->start_data_();
}

void JD9613Display::invert_colors(bool invert) {
  this->pre_invertcolors_ = invert;
  if (is_ready()) {
    this->command(invert ? JD9613_INVON : JD9613_INVOFF);
  }
}

int JD9613Display::get_width_internal()
{
  return this->width_;
}

int JD9613Display::get_height_internal()
{
  return this->height_;
}

}  // namespace jd9613
}  // namespace esphome

// vim: noai:ts=2:sw=2
