#pragma once
#include "esphome/components/spi/spi.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/display/display_color_utils.h"
#include "jd9613_defines.h"
#include "jd9613_init.h"

namespace esphome {
namespace jd9613 {

static const char *const TAG = "jd9613";
const size_t JD9613_TRANSFER_BUFFER_SIZE = 126;  // ensure this is divisible by 6

enum JD9613ColorMode {
  BITS_8 = 0x08,
  BITS_8_INDEXED = 0x09,
  BITS_16 = 0x10,
};

enum PixelMode {
  PIXEL_MODE_UNSPECIFIED,
  PIXEL_MODE_16,
  PIXEL_MODE_18,
};

class JD9613Display : public display::DisplayBuffer,
                      public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_LOW,
                                            spi::CLOCK_PHASE_LEADING, spi::DATA_RATE_80MHZ> {
 protected:
  JD9613Display(int16_t width, int16_t height);

 public:
  JD9613Display();

  bool is_composite() const {
    return left_panel_ != nullptr;
  }

  void add_init_sequence(const std::vector<uint8_t> &sequence) { this->extra_init_sequence_ = sequence; }
  void set_dc_pin(GPIOPin *dc_pin) { dc_pin_ = dc_pin; }
  void set_cs0_pin(GPIOPin *dc_pin) { cs0_pin_ = dc_pin; }
  void set_cs1_pin(GPIOPin *dc_pin) { cs1_pin_ = dc_pin; }
  float get_setup_priority() const override;
  void set_reset_pin(GPIOPin *reset) { this->reset_pin_ = reset; }
  void set_palette(const uint8_t *palette) { this->palette_ = palette; }
  void set_buffer_color_mode(JD9613ColorMode color_mode) { this->buffer_color_mode_ = color_mode; }
  void set_dimensions(int16_t width, int16_t height) {
    // this->height_ = height;
    // this->width_ = width;
  }
  void set_offsets(int16_t offset_x, int16_t offset_y) {
    this->offset_x_ = offset_x;
    this->offset_y_ = offset_y;
  }
  void invert_colors(bool invert);
  virtual void command(uint8_t value);
  virtual void data(uint8_t value);
  void send_command(uint8_t command_byte, const uint8_t *data_bytes, uint8_t num_data_bytes);
  void set_color_order(display::ColorOrder color_order) { this->color_order_ = color_order; }
  void set_swap_xy(bool swap_xy) { this->swap_xy_ = swap_xy; }
  void set_mirror_x(bool mirror_x) { this->mirror_x_ = mirror_x; }
  void set_mirror_y(bool mirror_y) { this->mirror_y_ = mirror_y; }
  void set_pixel_mode(PixelMode mode) { this->pixel_mode_ = mode; }

  void update() override;

  void fill(Color color) override;

  void dump_config() override;
  void _dump_config(const char *name);
  void setup() override;
  void on_shutdown() override { this->command(JD9613_SLPIN); }

  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_COLOR; }

 protected:
  inline bool check_buffer_() {
    if (this->buffer_ == nullptr) {
      this->alloc_buffer_();
      return !this->is_failed();
    }
    return true;
  }

  void draw_absolute_pixel_internal(int x, int y, Color color) override;
  void setup_pins_();

  virtual void set_madctl();
  void display_(GPIOPin *cs_pin);
  void init_lcd_(const uint8_t *addr);
  void reset_();
  void set_addr_window_(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2);

  uint8_t const *init_sequence_{nullptr};
  std::vector<uint8_t> extra_init_sequence_;
  int16_t width_{0};   ///< Display width as modified by current rotation
  int16_t height_{0};  ///< Display height as modified by current rotation
  int16_t offset_x_{0};
  int16_t offset_y_{0};
  uint16_t x_low_{0};
  uint16_t y_low_{0};
  uint16_t x_high_{0};
  uint16_t y_high_{0};
  const uint8_t *palette_{};

  JD9613ColorMode buffer_color_mode_{BITS_16};

  uint32_t get_buffer_length_();
  int get_width_internal() override;
  int get_height_internal() override;

  void start_command_();
  void end_command_();
  void start_data_();
  void end_data_();
  void alloc_buffer_();

  GPIOPin *reset_pin_{nullptr};
  GPIOPin *dc_pin_{nullptr};
  GPIOPin *cs0_pin_{nullptr};
  GPIOPin *cs1_pin_{nullptr};

  bool prossing_update_ = false;
  bool need_update_ = false;
  bool is_18bitdisplay_ = false;
  PixelMode pixel_mode_{};
  bool pre_invertcolors_{};
  display::ColorOrder color_order_{display::COLOR_ORDER_RGB};
  bool swap_xy_{};
  bool mirror_x_{};
  bool mirror_y_{};

  JD9613Display *left_panel_{nullptr};
  JD9613Display *right_panel_{nullptr};
};

}  // namespace jd9613
}  // namespace esphome

// vim: noai:ts=2:sw=2
