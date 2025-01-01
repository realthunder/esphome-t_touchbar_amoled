#pragma once
#include "esphome/core/helpers.h"

#include <cinttypes>

namespace esphome {
namespace jd9613 {

// clang-format off
static const uint8_t INITCMD_JD9613[] = {
    0xfe,  0x01,  0x01,
    0xf7,  0x03,  0x96, 0x13, 0xa9,
    0x90,  0x01,  0x01,
    0x2c,  0x0e,  0x19, 0x0b, 0x24, 0x1b, 0x1b, 0x1b, 0xaa, 0x50, 0x01, 0x16, 0x04, 0x04, 0x04, 0xd7,
    0x2d,  0x03,  0x66, 0x56, 0x55,
    0x2e,  0x09,  0x24, 0x04, 0x3f, 0x30, 0x30, 0xa8, 0xb8, 0xb8, 0x07,
    0x33,  0x0c,  0x03, 0x03, 0x03, 0x19, 0x19, 0x19, 0x13, 0x13, 0x13, 0x1a, 0x1a, 0x1a,
    0x10,  0x0d,  0x0b, 0x08, 0x64, 0xae, 0x0b, 0x08, 0x64, 0xae, 0x00, 0x80, 0x00, 0x00, 0x01,
    0x11,  0x05,  0x01, 0x1e, 0x01, 0x1e, 0x00,
    0x03,  0x05,  0x93, 0x1c, 0x00, 0x01, 0x7e,
    0x19,  0x01,  0x00,
    0x31,  0x06,  0x1b, 0x00, 0x06, 0x05, 0x05, 0x05,
    0x35,  0x04,  0x00, 0x80, 0x80, 0x00,
    0x12,  0x01,  0x1b,
    0x1a,  0x08,  0x01, 0x20, 0x00, 0x08, 0x01, 0x06, 0x06, 0x06,
    0x74,  0x07,  0xbd, 0x00, 0x01, 0x08, 0x01, 0xbb, 0x98,
    0x6c,  0x09,  0xdc, 0x08, 0x02, 0x01, 0x08, 0x01, 0x30, 0x08, 0x00,
    0x6d,  0x09,  0xdc, 0x08, 0x02, 0x01, 0x08, 0x02, 0x30, 0x08, 0x00,
    0x76,  0x09,  0xda, 0x00, 0x02, 0x20, 0x39, 0x80, 0x80, 0x50, 0x05,
    0x6e,  0x09,  0xdc, 0x00, 0x02, 0x01, 0x00, 0x02, 0x4f, 0x02, 0x00,
    0x6f,  0x09,  0xdc, 0x00, 0x02, 0x01, 0x00, 0x01, 0x4f, 0x02, 0x00,
    0x80,  0x07,  0xbd, 0x00, 0x01, 0x08, 0x01, 0xbb, 0x98,
    0x78,  0x09,  0xdc, 0x08, 0x02, 0x01, 0x08, 0x01, 0x30, 0x08, 0x00,
    0x79,  0x09,  0xdc, 0x08, 0x02, 0x01, 0x08, 0x02, 0x30, 0x08, 0x00,
    0x82,  0x09,  0xda, 0x40, 0x02, 0x20, 0x39, 0x00, 0x80, 0x50, 0x05,
    0x7a,  0x09,  0xdc, 0x00, 0x02, 0x01, 0x00, 0x02, 0x4f, 0x02, 0x00,
    0x7b,  0x09,  0xdc, 0x00, 0x02, 0x01, 0x00, 0x01, 0x4f, 0x02, 0x00,
    0x84,  0x0a,  0x01, 0x00, 0x09, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19,
    0x85,  0x0a,  0x19, 0x19, 0x19, 0x03, 0x02, 0x08, 0x19, 0x19, 0x19, 0x19,
    0x20,  0x0c,  0x20, 0x00, 0x08, 0x00, 0x02, 0x00, 0x40, 0x00, 0x10, 0x00, 0x04, 0x00,
    0x1e,  0x0c,  0x40, 0x00, 0x10, 0x00, 0x04, 0x00, 0x20, 0x00, 0x08, 0x00, 0x02, 0x00,
    0x24,  0x0c,  0x20, 0x00, 0x08, 0x00, 0x02, 0x00, 0x40, 0x00, 0x10, 0x00, 0x04, 0x00,
    0x22,  0x0c,  0x40, 0x00, 0x10, 0x00, 0x04, 0x00, 0x20, 0x00, 0x08, 0x00, 0x02, 0x00,
    0x13,  0x03,  0x63, 0x52, 0x41,
    0x14,  0x03,  0x36, 0x25, 0x14,
    0x15,  0x03,  0x63, 0x52, 0x41,
    0x16,  0x03,  0x36, 0x25, 0x14,
    0x1d,  0x03,  0x10, 0x00, 0x00,
    0x2a,  0x02,  0x0d, 0x07,
    0x27,  0x06,  0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x28,  0x06,  0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x26,  0x02,  0x01, 0x01,
    0x86,  0x02,  0x01, 0x01,
    0xfe,  0x01,  0x02,
    0x16,  0x05,  0x81, 0x43, 0x23, 0x1e, 0x03,
    0xfe,  0x01,  0x03,
    0x60,  0x01,  0x01,
    0x61,  0x0f,  0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x0d, 0x26, 0x5a, 0x80, 0x80, 0x95, 0xf8, 0x3b, 0x75,
    0x62,  0x0f,  0x21, 0x22, 0x32, 0x43, 0x44, 0xd7, 0x0a, 0x59, 0xa1, 0xe1, 0x52, 0xb7, 0x11, 0x64, 0xb1,
    0x63,  0x0b,  0x54, 0x55, 0x66, 0x06, 0xfb, 0x3f, 0x81, 0xc6, 0x06, 0x45, 0x83,
    0x64,  0x0f,  0x00, 0x00, 0x11, 0x11, 0x21, 0x00, 0x23, 0x6a, 0xf8, 0x63, 0x67, 0x70, 0xa5, 0xdc, 0x02,
    0x65,  0x0f,  0x22, 0x22, 0x32, 0x43, 0x44, 0x24, 0x44, 0x82, 0xc1, 0xf8, 0x61, 0xbf, 0x13, 0x62, 0xad,
    0x66,  0x0b,  0x54, 0x55, 0x65, 0x06, 0xf5, 0x37, 0x76, 0xb8, 0xf5, 0x31, 0x6c,
    0x67,  0x0f,  0x00, 0x10, 0x22, 0x22, 0x22, 0x00, 0x37, 0xa4, 0x7e, 0x22, 0x25, 0x2c, 0x4c, 0x72, 0x9a,
    0x68,  0x0f,  0x22, 0x33, 0x43, 0x44, 0x55, 0xc1, 0xe5, 0x2d, 0x6f, 0xaf, 0x23, 0x8f, 0xf3, 0x50, 0xa6,
    0x69,  0x0b,  0x65, 0x66, 0x77, 0x07, 0xfd, 0x4e, 0x9c, 0xed, 0x39, 0x86, 0xd3,
    0xfe,  0x01,  0x05,
    0x61,  0x0f,  0x00, 0x31, 0x44, 0x54, 0x55, 0x00, 0x92, 0xb5, 0x88, 0x19, 0x90, 0xe8, 0x3e, 0x71, 0xa5,
    0x62,  0x0f,  0x55, 0x66, 0x76, 0x77, 0x88, 0xce, 0xf2, 0x32, 0x6e, 0xc4, 0x34, 0x8b, 0xd9, 0x2a, 0x7d,
    0x63,  0x0b,  0x98, 0x99, 0xaa, 0x0a, 0xdc, 0x2e, 0x7d, 0xc3, 0x0d, 0x5b, 0x9e,
    0x64,  0x0f,  0x00, 0x31, 0x44, 0x54, 0x55, 0x00, 0xa2, 0xe5, 0xcd, 0x5c, 0x94, 0xcf, 0x09, 0x4a, 0x72,
    0x65,  0x0f,  0x55, 0x65, 0x66, 0x77, 0x87, 0x9c, 0xc2, 0xff, 0x36, 0x6a, 0xec, 0x45, 0x91, 0xd8, 0x20,
    0x66,  0x0b,  0x88, 0x98, 0x99, 0x0a, 0x68, 0xb0, 0xfb, 0x43, 0x8c, 0xd5, 0x0e,
    0x67,  0x0f,  0x00, 0x42, 0x55, 0x55, 0x55, 0x00, 0xcb, 0x62, 0xc5, 0x09, 0x44, 0x72, 0xa9, 0xd6, 0xfd,
    0x68,  0x0f,  0x66, 0x66, 0x77, 0x87, 0x98, 0x21, 0x45, 0x96, 0xed, 0x29, 0x90, 0xee, 0x4b, 0xb1, 0x13,
    0x69,  0x0b,  0x99, 0xaa, 0xba, 0x0b, 0x6a, 0xb8, 0x0d, 0x62, 0xb8, 0x0e, 0x54,
    0xfe,  0x01,  0x07,
    0x3e,  0x01,  0x00,
    0x42,  0x02,  0x03, 0x10,
    0x4a,  0x01,  0x31,
    0x5c,  0x01,  0x01,
    0x3c,  0x06,  0x07, 0x00, 0x24, 0x04, 0x3f, 0xe2,
    0x44,  0x04,  0x03, 0x40, 0x3f, 0x02,
    0x12,  0x0a,  0xaa, 0xaa, 0xc0, 0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8,
    0x11,  0x0f,  0xaa, 0xaa, 0xaa, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8,
    0x10,  0x0f,  0xaa, 0xaa, 0xaa, 0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58,
    0x14,  0x10,  0x03, 0x1f, 0x3f, 0x5f, 0x7f, 0x9f, 0xbf, 0xdf, 0x03, 0x1f, 0x3f, 0x5f, 0x7f, 0x9f, 0xbf, 0xdf,
    0x18,  0x0c,  0x70, 0x1a, 0x22, 0xbb, 0xaa, 0xff, 0x24, 0x71, 0x0f, 0x01, 0x00, 0x03,
    0xfe,  0x01,  0x00,
    0x3a,  0x01,  0x55,
    0xc4,  0x01,  0x80,
    0x2a,  0x04,  0x00, 0x00, 0x00, 0x7d,
    0x2b,  0x04,  0x00, 0x00, 0x01, 0x25,
    0x35,  0x01,  0x00,
    0x53,  0x01,  0x28,
    0x51,  0x01,  0xff,
    JD9613_SLPOUT , 0x81, 0x00,               // Exit Sleep
    // 0x11,  0x81,  0x00,
    JD9613_DISPON , 0x81, 0x00,               // Display on
    // 0x29,  0x81,  0x00,
    0x00                                   // End of list
};

// clang-format on
}  // namespace jd9613
}  // namespace esphome