#pragma once

namespace esphome {
namespace jd9613 {

// Color definitions
// clang-format off
static const uint8_t MADCTL_MY    = 0x80;   ///< Bit 7 Bottom to top
static const uint8_t MADCTL_MX    = 0x40;   ///< Bit 6 Right to left
static const uint8_t MADCTL_MV    = 0x20;   ///< Bit 5 Reverse Mode
static const uint8_t MADCTL_ML    = 0x10;   ///< Bit 4 LCD refresh Bottom to top
static const uint8_t MADCTL_RGB   = 0x00;  ///< Bit 3 Red-Green-Blue pixel order
static const uint8_t MADCTL_BGR   = 0x08;  ///< Bit 3 Blue-Green-Red pixel order
static const uint8_t MADCTL_MH    = 0x04;   ///< Bit 2 LCD refresh right to left
// clang-format on

// All JD9613 specific commands some are used by init()
static const uint8_t JD9613_NOP = 0x00;
static const uint8_t JD9613_SWRESET = 0x01;
static const uint8_t JD9613_RDDID = 0x04;
static const uint8_t JD9613_RDDST = 0x09;

static const uint8_t JD9613_SLPIN = 0x10;
static const uint8_t JD9613_SLPOUT = 0x11;
static const uint8_t JD9613_PTLON = 0x12;
static const uint8_t JD9613_NORON = 0x13;

static const uint8_t JD9613_RDMODE = 0x0A;
static const uint8_t JD9613_RDMADCTL = 0x0B;
static const uint8_t JD9613_RDPIXFMT = 0x0C;
static const uint8_t JD9613_RDIMGFMT = 0x0D;
static const uint8_t JD9613_RDSELFDIAG = 0x0F;

static const uint8_t JD9613_INVOFF = 0x20;
static const uint8_t JD9613_INVON = 0x21;
static const uint8_t JD9613_GAMMASET = 0x26;
static const uint8_t JD9613_DISPOFF = 0x28;
static const uint8_t JD9613_DISPON = 0x29;

static const uint8_t JD9613_CASET = 0x2A;
static const uint8_t JD9613_PASET = 0x2B;
static const uint8_t JD9613_RAMWR = 0x2C;
static const uint8_t JD9613_RAMRD = 0x2E;

static const uint8_t JD9613_PTLAR = 0x30;
static const uint8_t JD9613_VSCRDEF = 0x33;
static const uint8_t JD9613_MADCTL = 0x36;
static const uint8_t JD9613_VSCRSADD = 0x37;
static const uint8_t JD9613_IDMOFF = 0x38;
static const uint8_t JD9613_IDMON = 0x39;
static const uint8_t JD9613_PIXFMT = 0x3A;
static const uint8_t JD9613_COLMOD = 0x3A;

static const uint8_t JD9613_GETSCANLINE = 0x45;

static const uint8_t JD9613_WRDISBV = 0x51;
static const uint8_t JD9613_RDDISBV = 0x52;
static const uint8_t JD9613_WRCTRLD = 0x53;

static const uint8_t JD9613_IFMODE = 0xB0;
static const uint8_t JD9613_FRMCTR1 = 0xB1;
static const uint8_t JD9613_FRMCTR2 = 0xB2;
static const uint8_t JD9613_FRMCTR3 = 0xB3;
static const uint8_t JD9613_INVCTR = 0xB4;
static const uint8_t JD9613_DFUNCTR = 0xB6;
static const uint8_t JD9613_ETMOD = 0xB7;

static const uint8_t JD9613_PWCTR1 = 0xC0;
static const uint8_t JD9613_PWCTR2 = 0xC1;
static const uint8_t JD9613_PWCTR3 = 0xC2;
static const uint8_t JD9613_PWCTR4 = 0xC3;
static const uint8_t JD9613_PWCTR5 = 0xC4;
static const uint8_t JD9613_PWCTR6 = 0xF6;
static const uint8_t JD9613_VMCTR1 = 0xC5;
static const uint8_t JD9613_IFCTR = 0xC6;
static const uint8_t JD9613_VMCTR2 = 0xC7;
static const uint8_t JD9613_GMCTR = 0xC8;
static const uint8_t JD9613_SETEXTC = 0xC8;

static const uint8_t JD9613_PWSET = 0xD0;
static const uint8_t JD9613_VMCTR = 0xD1;
static const uint8_t JD9613_PWSETN = 0xD2;
static const uint8_t JD9613_RDID4 = 0xD3;
static const uint8_t JD9613_RDINDEX = 0xD9;
static const uint8_t JD9613_RDID1 = 0xDA;
static const uint8_t JD9613_RDID2 = 0xDB;
static const uint8_t JD9613_RDID3 = 0xDC;
static const uint8_t JD9613_RDIDX = 0xDD;  // TBC

static const uint8_t JD9613_GMCTRP1 = 0xE0;
static const uint8_t JD9613_GMCTRN1 = 0xE1;

static const uint8_t JD9613_CSCON = 0xF0;
static const uint8_t JD9613_ADJCTL3 = 0xF7;
static const uint8_t JD9613_DELAY_FLAG = 0xFF;
// special marker for delay - command byte reprents ms, length byte is an impossible value
#define JD9613_DELAY(ms) ((uint8_t) ((ms) | 0x80)), JD9613_DELAY_FLAG

}  // namespace jd9613
}  // namespace esphome
