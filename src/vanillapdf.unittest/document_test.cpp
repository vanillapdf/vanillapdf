#include "unittest.h"

#include <gtest/gtest.h>

static const unsigned char SIGNING_CERTIFICATE[] = {
    0x30, 0x82, 0x09, 0xc6, 0x02, 0x01, 0x03, 0x30, 0x82, 0x09, 0x90, 0x06, 0x09, 0x2a, 0x86, 0x48,
    0x86, 0xf7, 0x0d, 0x01, 0x07, 0x01, 0xa0, 0x82, 0x09, 0x81, 0x04, 0x82, 0x09, 0x7d, 0x30, 0x82,
    0x09, 0x79, 0x30, 0x82, 0x03, 0xff, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07,
    0x06, 0xa0, 0x82, 0x03, 0xf0, 0x30, 0x82, 0x03, 0xec, 0x02, 0x01, 0x00, 0x30, 0x82, 0x03, 0xe5,
    0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07, 0x01, 0x30, 0x1c, 0x06, 0x0a, 0x2a,
    0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x0c, 0x01, 0x03, 0x30, 0x0e, 0x04, 0x08, 0x7c, 0x42, 0x40,
    0x1e, 0xe7, 0x87, 0x29, 0x52, 0x02, 0x02, 0x08, 0x00, 0x80, 0x82, 0x03, 0xb8, 0x29, 0xf4, 0x8b,
    0xd9, 0x7b, 0x69, 0x4a, 0x9e, 0x96, 0x07, 0x12, 0xee, 0x80, 0xe3, 0xa2, 0xe9, 0x1b, 0x12, 0x6d,
    0x0c, 0x8b, 0xdc, 0x13, 0xe5, 0xbd, 0x9c, 0x57, 0xf3, 0x29, 0x75, 0xc3, 0xd8, 0x39, 0x5a, 0x73,
    0x57, 0xd9, 0xb7, 0xc6, 0xde, 0x2c, 0x5c, 0x34, 0xfe, 0x57, 0xfc, 0x22, 0x19, 0x0e, 0x7c, 0xd3,
    0x6f, 0x26, 0x29, 0x4c, 0x90, 0x86, 0x2b, 0xc6, 0x93, 0x49, 0xdb, 0xc5, 0xe3, 0x6e, 0x83, 0x3c,
    0x05, 0x2c, 0xac, 0xe7, 0x1f, 0x36, 0xb8, 0x77, 0xe1, 0xd2, 0x99, 0x17, 0x5a, 0xba, 0x70, 0x66,
    0x79, 0xb6, 0xbc, 0xad, 0xf5, 0xb0, 0x0b, 0x64, 0x36, 0xe4, 0xa6, 0xed, 0x27, 0xc9, 0x68, 0x4f,
    0xef, 0x0d, 0x7a, 0x02, 0x77, 0xdc, 0x16, 0x8e, 0xbc, 0x8a, 0x11, 0x54, 0x7f, 0x69, 0x5f, 0x55,
    0x4e, 0x38, 0xb7, 0x40, 0xa0, 0x6e, 0xf0, 0xa8, 0x3e, 0x10, 0x4d, 0x39, 0x79, 0x28, 0x64, 0x86,
    0x11, 0x52, 0xea, 0xc4, 0xab, 0x65, 0x0e, 0x63, 0xec, 0x02, 0x0c, 0x20, 0xa3, 0x51, 0x2c, 0x63,
    0x35, 0x73, 0xc0, 0x1d, 0xb0, 0xbe, 0xf4, 0xeb, 0x81, 0x7d, 0xae, 0x45, 0x01, 0x4d, 0x80, 0x79,
    0xa1, 0xbf, 0x77, 0x28, 0xf1, 0x4a, 0xfe, 0x59, 0x1d, 0x40, 0xb0, 0x1f, 0xbc, 0x36, 0xb6, 0xe2,
    0x4e, 0x47, 0xe1, 0x5d, 0xb7, 0xb6, 0x9d, 0x74, 0x75, 0x6f, 0xcb, 0xc6, 0xb8, 0x5c, 0x55, 0xf0,
    0x1c, 0xee, 0x9f, 0x90, 0xb0, 0xf7, 0xa9, 0xeb, 0x21, 0x15, 0xda, 0xa8, 0x65, 0x37, 0x40, 0x55,
    0xfc, 0x59, 0x2f, 0xf4, 0x0a, 0x67, 0x85, 0xec, 0xbf, 0x21, 0x23, 0xc1, 0xe0, 0x0d, 0xea, 0xce,
    0xb9, 0x9d, 0xc1, 0xd9, 0xf2, 0x11, 0xfb, 0x87, 0x33, 0xf7, 0x79, 0xa6, 0x3d, 0x4a, 0x90, 0x40,
    0x18, 0x61, 0x33, 0x0a, 0x52, 0x63, 0xdc, 0x2d, 0xfc, 0x4f, 0x4e, 0x44, 0x9d, 0x9a, 0x92, 0x58,
    0x65, 0x27, 0xab, 0x70, 0x79, 0xc3, 0x2a, 0x0f, 0x11, 0x48, 0xfe, 0xab, 0x9a, 0xa7, 0xe5, 0x9e,
    0xfd, 0xf9, 0x9a, 0xc8, 0x58, 0xa5, 0x03, 0x9e, 0xec, 0x18, 0x40, 0x88, 0x6d, 0x45, 0x45, 0x25,
    0x31, 0x89, 0x8d, 0xa9, 0xbd, 0xdd, 0x46, 0x38, 0x6b, 0xc5, 0x4f, 0x62, 0xf4, 0xd7, 0x27, 0x1a,
    0x11, 0x56, 0x15, 0xad, 0xc6, 0x48, 0xea, 0xf4, 0xc9, 0x7a, 0x3c, 0x4b, 0x1d, 0xef, 0xba, 0x2e,
    0x84, 0x66, 0x1c, 0x68, 0x37, 0x06, 0x56, 0xbd, 0x33, 0x5f, 0xde, 0x9b, 0x1d, 0xf6, 0x34, 0x36,
    0xc4, 0xc4, 0xc9, 0x3b, 0x29, 0x31, 0xfb, 0xcc, 0xe2, 0xb4, 0x7d, 0xf9, 0xb2, 0xbd, 0x5e, 0x44,
    0x57, 0xb9, 0x39, 0xd0, 0xae, 0x86, 0xe3, 0xf9, 0x0f, 0xde, 0x1c, 0x06, 0xe1, 0x98, 0xb7, 0x9e,
    0x11, 0x38, 0x50, 0x1c, 0x9e, 0x18, 0x55, 0xc8, 0xd2, 0x3d, 0x2c, 0x1d, 0xeb, 0xb7, 0xb9, 0x85,
    0x58, 0x75, 0xee, 0x84, 0xe8, 0xb9, 0xf2, 0xbb, 0xca, 0xc4, 0x21, 0xc8, 0x15, 0x60, 0x8e, 0x9a,
    0xab, 0x97, 0x68, 0xba, 0x7c, 0xdf, 0x10, 0xa4, 0xec, 0xb1, 0x2b, 0xa6, 0x57, 0xec, 0xd5, 0x04,
    0x14, 0x72, 0x57, 0xb3, 0x93, 0xfc, 0x96, 0xdd, 0x6f, 0xec, 0xc7, 0x24, 0x6b, 0x49, 0x8b, 0x9f,
    0xcd, 0x0b, 0x2a, 0x4a, 0xfa, 0xbf, 0x12, 0x3b, 0x6c, 0x9c, 0xc7, 0x5f, 0x89, 0x2f, 0xbd, 0xd5,
    0xe2, 0x21, 0x8d, 0xbd, 0xc0, 0xca, 0x9f, 0x05, 0x5f, 0x16, 0xa9, 0x13, 0xb6, 0xea, 0x7c, 0x4f,
    0xcf, 0x7d, 0x21, 0xad, 0x86, 0xa3, 0x87, 0xcc, 0xf1, 0xe2, 0x09, 0x5e, 0x61, 0x07, 0x24, 0x28,
    0xa7, 0x04, 0x7f, 0x0d, 0xda, 0x10, 0x80, 0x92, 0x35, 0x54, 0x21, 0xa4, 0xba, 0xb5, 0x9b, 0xf5,
    0xd5, 0x82, 0x5e, 0x0d, 0x23, 0xee, 0x77, 0xe8, 0x85, 0xea, 0x64, 0xbf, 0x8f, 0xe0, 0xfa, 0x9c,
    0xc2, 0x11, 0xca, 0xa8, 0x83, 0xac, 0x75, 0x9d, 0x06, 0xe3, 0x9c, 0x0c, 0xd3, 0x3d, 0x2f, 0x16,
    0xc7, 0x37, 0x66, 0x5a, 0xe5, 0x47, 0x7e, 0xa0, 0xb8, 0x4b, 0x48, 0xa7, 0x25, 0x31, 0x44, 0x5a,
    0xc3, 0x97, 0xf2, 0xfc, 0xe3, 0xa6, 0x69, 0x11, 0x74, 0x28, 0xf6, 0x74, 0xa4, 0x98, 0xf7, 0x30,
    0x65, 0x35, 0x42, 0xad, 0x08, 0x49, 0x92, 0xbb, 0x65, 0x57, 0xf8, 0x4a, 0xe4, 0x43, 0x78, 0xd8,
    0x90, 0x4d, 0x9a, 0xb1, 0xae, 0x21, 0x2b, 0x1d, 0x44, 0x12, 0x74, 0x10, 0x5f, 0x90, 0x0c, 0x35,
    0xdc, 0x78, 0x97, 0xe5, 0x83, 0xd9, 0x72, 0x09, 0x52, 0x60, 0xb5, 0x49, 0xed, 0xff, 0x43, 0x66,
    0x96, 0x49, 0x9a, 0xa4, 0x66, 0x10, 0x24, 0x0d, 0xa5, 0xaa, 0xa7, 0x17, 0xe6, 0x95, 0x6c, 0x66,
    0x76, 0x2b, 0xc0, 0x66, 0x19, 0x2a, 0xda, 0x8b, 0x78, 0x87, 0x4f, 0x1a, 0x00, 0xc3, 0x7f, 0x0e,
    0x13, 0x60, 0x30, 0xb1, 0x78, 0x54, 0x5b, 0xbf, 0x50, 0x25, 0xcd, 0x89, 0xe5, 0xb7, 0x98, 0xbd,
    0xb9, 0x2c, 0xfc, 0xc6, 0xda, 0x6c, 0x6a, 0xc7, 0x68, 0xc4, 0x1e, 0x5a, 0x0d, 0xa7, 0xf3, 0x89,
    0x48, 0x48, 0x74, 0xf7, 0xe4, 0xfa, 0xb4, 0x13, 0x7b, 0xa5, 0x2f, 0x28, 0x1f, 0x31, 0xb3, 0x6c,
    0x61, 0x72, 0x03, 0x36, 0x90, 0x79, 0x46, 0xc7, 0x31, 0xbf, 0xf7, 0xba, 0x41, 0x67, 0x41, 0x8e,
    0xb6, 0xbc, 0x98, 0xc4, 0xbf, 0xa6, 0x28, 0xde, 0x1c, 0x18, 0x8e, 0xd1, 0xb4, 0x0d, 0xf5, 0x99,
    0xe5, 0x3e, 0x45, 0x2d, 0x67, 0x28, 0xab, 0xbd, 0x93, 0x56, 0x21, 0x43, 0xd1, 0x98, 0xdf, 0xe9,
    0x56, 0x45, 0xbb, 0x58, 0x6c, 0x80, 0x07, 0xd7, 0x90, 0x8c, 0x76, 0x60, 0x93, 0x2e, 0x5e, 0x74,
    0xe5, 0x09, 0x1d, 0x78, 0xc1, 0x80, 0x4e, 0xf5, 0x91, 0x75, 0x4a, 0x01, 0xef, 0x9a, 0xa1, 0x89,
    0x33, 0xa0, 0x2b, 0xbe, 0xc1, 0x44, 0x8a, 0x10, 0x55, 0xe3, 0x7d, 0x71, 0xe1, 0xf5, 0x8b, 0x83,
    0x61, 0x10, 0x78, 0x6d, 0x58, 0x54, 0x68, 0x18, 0xd9, 0xf4, 0x21, 0xb0, 0x16, 0x98, 0x1b, 0xb2,
    0xa4, 0x81, 0x17, 0x35, 0x2a, 0x55, 0x98, 0xcf, 0xdb, 0x54, 0x92, 0x8e, 0x58, 0xf0, 0xd9, 0x6f,
    0x66, 0xf6, 0xe7, 0x14, 0x06, 0xcd, 0x30, 0x2f, 0x87, 0x7b, 0x2d, 0x22, 0x40, 0x84, 0xb9, 0xfb,
    0x42, 0xed, 0x5e, 0x71, 0x85, 0x2c, 0x3d, 0xfa, 0xbe, 0x97, 0x1b, 0xf8, 0xbc, 0x34, 0x4f, 0xa3,
    0xd6, 0x42, 0x35, 0x18, 0x89, 0x0c, 0x6f, 0xfe, 0x98, 0xed, 0x02, 0x5c, 0xba, 0x23, 0xbf, 0x98,
    0x4f, 0xc0, 0x74, 0xc2, 0xd5, 0xb6, 0x92, 0x48, 0xe7, 0xd5, 0xad, 0xc0, 0x12, 0x09, 0x28, 0x90,
    0x04, 0xc4, 0x77, 0xde, 0x71, 0x9d, 0x6f, 0x1e, 0x4d, 0x24, 0xba, 0x89, 0x7e, 0x23, 0x65, 0x99,
    0x96, 0x46, 0x86, 0x5d, 0x9a, 0xfd, 0xdd, 0xcb, 0xd3, 0xc1, 0xb1, 0xf3, 0xa3, 0x44, 0xf6, 0x22,
    0xd1, 0xe1, 0x11, 0x7f, 0x4a, 0x98, 0xd2, 0xdc, 0xdf, 0xaa, 0x6c, 0xec, 0xce, 0x80, 0xeb, 0x44,
    0x13, 0x5d, 0x31, 0x84, 0x7a, 0xff, 0xb0, 0x40, 0xc7, 0xf7, 0xfd, 0x3d, 0x8f, 0x80, 0x4e, 0xe5,
    0xac, 0x55, 0x43, 0xfb, 0x3c, 0x30, 0x82, 0x05, 0x72, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7,
    0x0d, 0x01, 0x07, 0x01, 0xa0, 0x82, 0x05, 0x63, 0x04, 0x82, 0x05, 0x5f, 0x30, 0x82, 0x05, 0x5b,
    0x30, 0x82, 0x05, 0x57, 0x06, 0x0b, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x0c, 0x0a, 0x01,
    0x02, 0xa0, 0x82, 0x04, 0xee, 0x30, 0x82, 0x04, 0xea, 0x30, 0x1c, 0x06, 0x0a, 0x2a, 0x86, 0x48,
    0x86, 0xf7, 0x0d, 0x01, 0x0c, 0x01, 0x03, 0x30, 0x0e, 0x04, 0x08, 0x01, 0x56, 0x02, 0x71, 0x1b,
    0x2e, 0x67, 0x97, 0x02, 0x02, 0x08, 0x00, 0x04, 0x82, 0x04, 0xc8, 0x1a, 0x2f, 0x5e, 0x97, 0x16,
    0x2b, 0x90, 0xaf, 0x09, 0xfe, 0x80, 0xc7, 0x28, 0x5f, 0x84, 0xb8, 0x43, 0xf5, 0x7d, 0xa1, 0x2f,
    0x7e, 0xd1, 0x3f, 0x8e, 0x97, 0x7f, 0xe6, 0xf8, 0xa1, 0x10, 0x51, 0x68, 0x43, 0xe9, 0x38, 0x1a,
    0x2b, 0xd8, 0xb4, 0x50, 0x52, 0x1d, 0x19, 0x30, 0x1d, 0x09, 0x79, 0x9d, 0x8e, 0x31, 0xbe, 0x97,
    0x9d, 0xb7, 0x63, 0xc2, 0xf4, 0x55, 0xaa, 0xa6, 0x79, 0x5c, 0xc1, 0x77, 0x61, 0x05, 0x59, 0x48,
    0x75, 0xb4, 0xf8, 0x0d, 0xfd, 0x19, 0xb0, 0x64, 0xf5, 0xe6, 0x19, 0x48, 0xe8, 0x41, 0x73, 0xe0,
    0x7b, 0x5c, 0xc5, 0x72, 0xeb, 0xd2, 0xcc, 0x20, 0x5a, 0xab, 0xf4, 0x89, 0x73, 0x64, 0xac, 0xc3,
    0xd2, 0xe8, 0x76, 0x75, 0xd9, 0x2f, 0x89, 0xf0, 0x12, 0xd3, 0xce, 0xea, 0x39, 0x2e, 0xe9, 0xbd,
    0x66, 0x7c, 0xa1, 0xfe, 0x6d, 0xcb, 0x56, 0x5d, 0xf8, 0x42, 0x46, 0x4b, 0x2e, 0x7b, 0x29, 0x48,
    0x6e, 0xb9, 0xd8, 0xac, 0x23, 0xa9, 0xe0, 0x2f, 0x45, 0xe0, 0x0f, 0x7f, 0x5b, 0xea, 0x2a, 0xc5,
    0xdb, 0x09, 0x5b, 0x71, 0xc2, 0x6b, 0x48, 0x36, 0x3a, 0x4b, 0xaa, 0x02, 0xd1, 0x24, 0x4c, 0x63,
    0x63, 0xa9, 0x5b, 0x23, 0x07, 0xaf, 0x69, 0x7a, 0x3d, 0x46, 0xc7, 0xad, 0xae, 0x74, 0x96, 0x67,
    0x2b, 0xcc, 0x3c, 0x4d, 0x34, 0x52, 0xea, 0xce, 0xf2, 0x9e, 0x68, 0x82, 0x6b, 0x6a, 0x7b, 0xa5,
    0x76, 0x74, 0x93, 0x3d, 0x69, 0x28, 0xe9, 0xe6, 0x0f, 0x04, 0x31, 0xcf, 0x6a, 0xcb, 0x51, 0xfb,
    0xba, 0x00, 0x1e, 0x69, 0x36, 0x1c, 0xf2, 0xc1, 0x71, 0x07, 0x15, 0x0c, 0xae, 0x01, 0x95, 0x7b,
    0xac, 0x7a, 0xd5, 0xa7, 0x26, 0xb3, 0x8e, 0x94, 0x16, 0x60, 0x34, 0x79, 0x72, 0x87, 0x3c, 0x19,
    0xd7, 0x73, 0xe4, 0xe6, 0x7e, 0xe0, 0x3d, 0x9c, 0x6a, 0x0c, 0x11, 0x3d, 0x41, 0xab, 0x21, 0xda,
    0x99, 0x6e, 0xd1, 0xf9, 0x4b, 0xde, 0x3c, 0x73, 0x4b, 0xa1, 0x13, 0xb0, 0xec, 0x1e, 0xf9, 0xfd,
    0x92, 0xa9, 0x6a, 0x35, 0x19, 0x70, 0x0c, 0x96, 0xd1, 0xad, 0xa9, 0xab, 0x20, 0xda, 0x27, 0x0f,
    0x6c, 0x72, 0x3c, 0xa3, 0x0a, 0xe1, 0xb7, 0x90, 0xf6, 0x99, 0x42, 0x8b, 0xd2, 0x3b, 0xff, 0xb0,
    0xc4, 0x46, 0x3a, 0x07, 0x7a, 0x47, 0x68, 0x41, 0x4b, 0xd2, 0x49, 0x4c, 0x9b, 0xfb, 0x50, 0x20,
    0x0a, 0xe6, 0x0a, 0x53, 0x23, 0xa1, 0x00, 0xbe, 0x95, 0x76, 0xb4, 0xde, 0xfe, 0xba, 0x66, 0xef,
    0x0b, 0xde, 0x71, 0x68, 0x9e, 0x1a, 0x7c, 0xf0, 0x24, 0x8b, 0x8a, 0xd8, 0x23, 0xa9, 0x2c, 0xaf,
    0xae, 0xe3, 0x17, 0x60, 0x78, 0x5c, 0xa1, 0xfd, 0xa3, 0x83, 0x03, 0x6e, 0x28, 0x50, 0xe2, 0x5e,
    0xdd, 0x11, 0xf2, 0x71, 0x96, 0xae, 0x90, 0xf7, 0x09, 0x94, 0x28, 0x68, 0x22, 0xde, 0x9c, 0x15,
    0x83, 0x0b, 0x45, 0xf5, 0x8f, 0x76, 0x93, 0x72, 0x5d, 0x16, 0xd6, 0x44, 0xeb, 0x88, 0xfd, 0x30,
    0xf9, 0x5a, 0x78, 0x65, 0x77, 0x16, 0x9e, 0xe8, 0x20, 0x27, 0xc3, 0xf1, 0x66, 0x0d, 0x61, 0x07,
    0xc8, 0x37, 0xb8, 0xd2, 0x39, 0x8f, 0xaa, 0xa6, 0xc3, 0xb1, 0xeb, 0x96, 0xd7, 0x21, 0xd9, 0x16,
    0xb9, 0xec, 0x11, 0xd8, 0x48, 0x3c, 0x46, 0x19, 0xe9, 0xff, 0x3d, 0x0f, 0x7e, 0xf0, 0x73, 0x6c,
    0x22, 0x41, 0x6c, 0x2f, 0xa0, 0x3a, 0x29, 0x6e, 0x11, 0xdc, 0x3a, 0x80, 0x57, 0xc1, 0x38, 0xd4,
    0x46, 0x31, 0x21, 0x22, 0xa5, 0xf6, 0x9c, 0x77, 0xf7, 0xb7, 0xa2, 0x81, 0xc2, 0x61, 0x13, 0x44,
    0x98, 0x32, 0x53, 0xdb, 0x5e, 0xcc, 0x83, 0x21, 0xaf, 0x07, 0x99, 0x30, 0xb4, 0x84, 0xa3, 0x90,
    0x32, 0x78, 0xaa, 0xa1, 0x9e, 0xb9, 0xbc, 0x79, 0xae, 0x03, 0x56, 0x57, 0x53, 0x26, 0x58, 0x42,
    0x25, 0xcb, 0x23, 0x98, 0x72, 0xb9, 0xc7, 0x82, 0xe8, 0x77, 0x11, 0xb1, 0xa8, 0xa4, 0x36, 0x06,
    0x12, 0x19, 0x3e, 0x58, 0x18, 0x01, 0x09, 0xfa, 0x39, 0xe5, 0x43, 0x0e, 0x60, 0x9e, 0xb8, 0x11,
    0x39, 0x6a, 0x54, 0x5c, 0xf7, 0x50, 0x9b, 0xb3, 0x90, 0x50, 0x59, 0x2b, 0xdd, 0x15, 0xfc, 0xe8,
    0xe2, 0xaf, 0x1a, 0xf2, 0x16, 0xe5, 0x43, 0x4d, 0xd3, 0x59, 0xee, 0x9d, 0x71, 0x98, 0xee, 0x78,
    0xbc, 0xe0, 0x6b, 0x9a, 0x13, 0x2c, 0x05, 0x9a, 0x5d, 0x07, 0x31, 0xfe, 0x77, 0xef, 0x1b, 0x92,
    0xd3, 0xec, 0x21, 0x7c, 0xf6, 0x7e, 0x3b, 0x57, 0x96, 0xcc, 0x32, 0x78, 0x00, 0x9f, 0x05, 0x09,
    0x16, 0xd9, 0x42, 0xc8, 0xdc, 0xa9, 0x16, 0x04, 0xb0, 0x7b, 0xf5, 0x87, 0x94, 0x0d, 0xb8, 0xe3,
    0x16, 0xb2, 0xc7, 0x6c, 0xd5, 0xe7, 0xce, 0x71, 0xbf, 0xb7, 0x15, 0x81, 0xb2, 0x5a, 0x74, 0x3d,
    0xc8, 0xcb, 0xf7, 0x1b, 0x3f, 0x5f, 0xdd, 0x65, 0x89, 0xfa, 0x60, 0xb6, 0x06, 0x0a, 0x53, 0xbd,
    0xb9, 0x17, 0xb3, 0x3a, 0x58, 0x15, 0x01, 0x66, 0xbd, 0x36, 0x18, 0x38, 0x39, 0x37, 0xb0, 0x83,
    0x79, 0xe6, 0x8a, 0x09, 0x78, 0x9b, 0x5e, 0x8b, 0x94, 0xb7, 0x7c, 0x11, 0x53, 0x7f, 0x13, 0xf3,
    0x47, 0x34, 0xdd, 0xa4, 0x7b, 0xab, 0x2f, 0xa4, 0xe9, 0x7e, 0x38, 0x7a, 0x72, 0x8d, 0xe3, 0xea,
    0x85, 0x4a, 0xcc, 0x09, 0xbe, 0xc9, 0x87, 0x66, 0xdb, 0xfe, 0xc8, 0x02, 0x1d, 0x0b, 0x48, 0xbf,
    0x6c, 0x95, 0xe0, 0x53, 0x44, 0x92, 0xfa, 0xe7, 0x58, 0xf5, 0xf3, 0x9e, 0xd3, 0x3b, 0xda, 0x3b,
    0x75, 0x18, 0xa9, 0xdc, 0xc8, 0x9b, 0xac, 0xd0, 0x17, 0xc9, 0x19, 0xf6, 0x62, 0xdb, 0xee, 0xa4,
    0x70, 0xa7, 0x1f, 0x80, 0x00, 0x51, 0xfe, 0x7f, 0x1b, 0xd2, 0x7d, 0x67, 0x62, 0xed, 0xd3, 0x71,
    0xfd, 0xcb, 0x59, 0x67, 0x5f, 0x16, 0x28, 0x90, 0x4e, 0xb8, 0x7a, 0x1e, 0x6d, 0x0f, 0x39, 0x9c,
    0xce, 0xf3, 0x7d, 0x41, 0xdc, 0xfd, 0x22, 0xf6, 0xb1, 0x38, 0xbe, 0xa8, 0x1a, 0x2c, 0x8b, 0x0e,
    0x35, 0x79, 0xc6, 0xcd, 0x61, 0x7e, 0xc4, 0x76, 0x3f, 0x29, 0x18, 0xa3, 0x5c, 0x23, 0x2f, 0x5c,
    0x64, 0xc4, 0x71, 0x17, 0x78, 0x63, 0xca, 0x04, 0x29, 0x03, 0x53, 0xab, 0xb3, 0x48, 0x29, 0xe5,
    0x8f, 0x4b, 0xcc, 0xd9, 0xc7, 0xc2, 0x8a, 0x04, 0x4b, 0x1e, 0xd9, 0xee, 0xc3, 0x16, 0x69, 0xb9,
    0x82, 0xe2, 0x35, 0xc7, 0x40, 0x27, 0xce, 0xbd, 0xea, 0x37, 0x34, 0xaf, 0x30, 0x60, 0x38, 0x25,
    0x75, 0xf4, 0x74, 0xb4, 0x71, 0x6e, 0x2f, 0x57, 0x7e, 0xa6, 0x37, 0x62, 0x7a, 0x31, 0xf5, 0x9f,
    0xcf, 0xd9, 0xe1, 0x20, 0x29, 0xfb, 0x6c, 0x74, 0x49, 0x55, 0x2d, 0xe9, 0x3f, 0xa6, 0xc3, 0xb4,
    0xf7, 0x82, 0x9b, 0xa0, 0x4e, 0x4e, 0x20, 0x32, 0xd3, 0x49, 0x9b, 0xde, 0x50, 0x4e, 0x12, 0x80,
    0xd3, 0x76, 0xa5, 0x9e, 0x13, 0x3c, 0x2a, 0x59, 0x5f, 0xcb, 0x68, 0xf8, 0x27, 0x64, 0x09, 0x35,
    0xfc, 0x52, 0x4a, 0x7f, 0x8e, 0x50, 0x80, 0xc6, 0x57, 0x79, 0xa5, 0x6e, 0xca, 0x2a, 0x01, 0x17,
    0x63, 0x61, 0x74, 0x3f, 0x33, 0x69, 0x23, 0x8a, 0x59, 0x32, 0x9c, 0x9f, 0x93, 0xcc, 0x1d, 0x65,
    0x84, 0xce, 0x7b, 0x6f, 0x60, 0x6b, 0xac, 0x62, 0x00, 0xad, 0xc3, 0xba, 0xa0, 0x1d, 0x72, 0xf6,
    0xc3, 0xff, 0xca, 0xb4, 0xa2, 0x99, 0xfd, 0x3d, 0x99, 0x45, 0xa5, 0xd7, 0x88, 0xed, 0xa8, 0xb9,
    0xb7, 0x15, 0x6c, 0x02, 0x69, 0xfa, 0x91, 0x69, 0xfc, 0x67, 0xc1, 0xa0, 0x69, 0xcb, 0xc9, 0xca,
    0x6e, 0xf2, 0x63, 0xbd, 0x5c, 0x07, 0x67, 0x3a, 0xc3, 0xcf, 0x2d, 0x08, 0x68, 0xbb, 0x60, 0x88,
    0x13, 0xec, 0x2d, 0x64, 0xbf, 0x98, 0xcf, 0x4b, 0x2d, 0x83, 0x28, 0x85, 0x6e, 0x64, 0xcf, 0x04,
    0x4a, 0x28, 0x4a, 0x2e, 0xc9, 0xf8, 0x19, 0x69, 0xe1, 0xa5, 0x25, 0x77, 0x4f, 0x70, 0xfa, 0x2b,
    0x62, 0x06, 0x35, 0x42, 0xc5, 0x6c, 0x0b, 0xca, 0xbe, 0x94, 0xec, 0x8b, 0x8c, 0xf9, 0x14, 0x4d,
    0x2e, 0x76, 0x9e, 0x5c, 0x24, 0x07, 0x1f, 0x58, 0x1e, 0x74, 0x85, 0x30, 0x70, 0x55, 0x0b, 0xb8,
    0x2a, 0x8e, 0xe9, 0x5b, 0x37, 0xb3, 0x3a, 0x57, 0xc0, 0x5f, 0x23, 0xad, 0x92, 0x25, 0x52, 0x82,
    0xfe, 0xea, 0xb5, 0x28, 0x15, 0xc4, 0xfe, 0x83, 0xa6, 0xfe, 0x25, 0x9b, 0xd2, 0xc8, 0xca, 0xe1,
    0xe0, 0xf1, 0xf4, 0xce, 0x0b, 0x42, 0xea, 0xe4, 0x44, 0xf9, 0xae, 0x9d, 0x5d, 0x66, 0x57, 0x4a,
    0x32, 0x2f, 0x80, 0xef, 0xe1, 0xc4, 0x34, 0x10, 0x0b, 0xab, 0x1d, 0xdd, 0x36, 0xf2, 0xd0, 0x78,
    0xaf, 0x48, 0xf1, 0x8d, 0x65, 0x36, 0x3d, 0x65, 0x91, 0xf6, 0xe5, 0xdb, 0xe0, 0x77, 0xfb, 0x26,
    0xd2, 0x67, 0xe9, 0x87, 0x6c, 0x70, 0x9f, 0x8f, 0x45, 0xf8, 0xd0, 0x1f, 0xac, 0xca, 0xfb, 0xd8,
    0xd3, 0x3e, 0x1d, 0xb3, 0x03, 0x28, 0x70, 0x96, 0xed, 0x0c, 0xb9, 0xdb, 0x6f, 0x98, 0xc0, 0x8d,
    0xec, 0x6b, 0x1f, 0x1b, 0x20, 0x88, 0x91, 0x1f, 0x45, 0xee, 0xba, 0xd4, 0xd5, 0xe4, 0xf4, 0xd8,
    0x3b, 0x68, 0xe9, 0x31, 0x56, 0x30, 0x23, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01,
    0x09, 0x15, 0x31, 0x16, 0x04, 0x14, 0x9f, 0xa2, 0x58, 0x17, 0x89, 0x9f, 0x12, 0xb3, 0x4b, 0x40,
    0x9f, 0xea, 0xdf, 0x99, 0xb2, 0x5b, 0xc9, 0x50, 0xf6, 0xaa, 0x30, 0x2f, 0x06, 0x09, 0x2a, 0x86,
    0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x14, 0x31, 0x22, 0x1e, 0x20, 0x00, 0x55, 0x00, 0x6e, 0x00,
    0x69, 0x00, 0x74, 0x00, 0x20, 0x00, 0x74, 0x00, 0x65, 0x00, 0x73, 0x00, 0x74, 0x00, 0x20, 0x00,
    0x73, 0x00, 0x69, 0x00, 0x67, 0x00, 0x6e, 0x00, 0x65, 0x00, 0x72, 0x30, 0x2d, 0x30, 0x21, 0x30,
    0x09, 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14, 0x71, 0xab, 0x0a, 0xe6,
    0x44, 0x16, 0xee, 0x25, 0xb5, 0xe0, 0x64, 0x0a, 0xf3, 0x69, 0x34, 0x82, 0x3d, 0x35, 0x97, 0x4a,
    0x04, 0x08, 0x7e, 0x6c, 0xfd, 0x27, 0x63, 0xdc, 0x87, 0x2d
};

namespace documents {

    TEST(DocumentEncryptionSettings, CreateRelease) {
        DocumentEncryptionSettingsHandle* encryption_settings = nullptr;

        ASSERT_EQ(DocumentEncryptionSettings_Create(&encryption_settings), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(encryption_settings, nullptr);

        ASSERT_EQ(DocumentEncryptionSettings_Release(encryption_settings), VANILLAPDF_ERROR_SUCCESS);
    }

    TEST(DocumentEncryptionSettings, PermissionsFlagMaxValue) {
        DocumentEncryptionSettingsHandle* encryption_settings = nullptr;

        UserAccessPermissionFlags permissions_flags = static_cast<UserAccessPermissionFlags>(-1);
        UserAccessPermissionFlags permissions_flags_check = UserAccessPermissionFlag_None;

        ASSERT_EQ(DocumentEncryptionSettings_Create(&encryption_settings), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(encryption_settings, nullptr);

        ASSERT_EQ(DocumentEncryptionSettings_SetUserAccessPermissions(encryption_settings, permissions_flags), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DocumentEncryptionSettings_GetUserAccessPermissions(encryption_settings, &permissions_flags_check), VANILLAPDF_ERROR_SUCCESS);

        EXPECT_EQ(permissions_flags, permissions_flags_check);

        ASSERT_EQ(DocumentEncryptionSettings_Release(encryption_settings), VANILLAPDF_ERROR_SUCCESS);
    }

    void EncryptDocument(
        std::string owner_password,
        std::string user_password,
        EncryptionAlgorithmType encryption_algorithm,
        integer_type encryption_key_length,
        UserAccessPermissionFlags user_permissions) {

        FileHandle* memory_file = nullptr;
        DocumentHandle* memory_document = nullptr;
        InputOutputStreamHandle* io_stream = nullptr;
        DocumentEncryptionSettingsHandle* encryption_settings = nullptr;

        FileHandle* destination_save_file = nullptr;
        FileHandle* destination_load_file = nullptr;
        InputOutputStreamHandle* destination_io_stream = nullptr;

        boolean_type destination_is_encrypted = VANILLAPDF_RV_FALSE;

        ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(io_stream, nullptr);

        ASSERT_EQ(File_CreateStream(io_stream, "temp", &memory_file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(memory_file, nullptr);

        ASSERT_EQ(Document_CreateFile(memory_file, &memory_document), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(memory_document, nullptr);

        ASSERT_EQ(DocumentEncryptionSettings_Create(&encryption_settings), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(encryption_settings, nullptr);

        ASSERT_EQ(DocumentEncryptionSettings_SetAlgorithm(encryption_settings, encryption_algorithm), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DocumentEncryptionSettings_SetKeyLength(encryption_settings, encryption_key_length), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DocumentEncryptionSettings_SetUserAccessPermissions(encryption_settings, user_permissions), VANILLAPDF_ERROR_SUCCESS);

        BufferHandle* owner_password_buffer = nullptr;
        BufferHandle* user_password_buffer = nullptr;

        ASSERT_EQ(Buffer_CreateFromData(owner_password.data(), owner_password.length(), &owner_password_buffer), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Buffer_CreateFromData(user_password.data(), user_password.length(), &user_password_buffer), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_NE(owner_password_buffer, nullptr);
        ASSERT_NE(user_password_buffer, nullptr);

        ASSERT_EQ(DocumentEncryptionSettings_SetOwnerPassword(encryption_settings, owner_password_buffer), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DocumentEncryptionSettings_SetUserPassword(encryption_settings, user_password_buffer), VANILLAPDF_ERROR_SUCCESS);

        // Add encryption dictionary and initialize keys
        ASSERT_EQ(Document_AddEncryption(memory_document, encryption_settings), VANILLAPDF_ERROR_SUCCESS);

        // Create the destination stream file for output
        ASSERT_EQ(InputOutputStream_CreateFromMemory(&destination_io_stream), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(destination_io_stream, nullptr);

        ASSERT_EQ(File_CreateStream(destination_io_stream, "temp_destination", &destination_save_file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(destination_save_file, nullptr);

        // Save the file into destination memory stream
        ASSERT_EQ(Document_SaveFile(memory_document, destination_save_file), VANILLAPDF_ERROR_SUCCESS);

        // Check the destination file for consistency
        ASSERT_EQ(File_OpenStream(destination_io_stream, "temp_destination", &destination_load_file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(destination_load_file, nullptr);

        ASSERT_EQ(File_Initialize(destination_load_file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_IsEncrypted(destination_load_file, &destination_is_encrypted), VANILLAPDF_ERROR_SUCCESS);

        EXPECT_EQ(destination_is_encrypted, VANILLAPDF_RV_TRUE);

        ASSERT_EQ(File_SetEncryptionPassword(destination_load_file, owner_password.data()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_SetEncryptionPassword(destination_load_file, user_password.data()), VANILLAPDF_ERROR_SUCCESS);

        // Cleanup

        ASSERT_EQ(File_Release(destination_load_file), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_EQ(File_Release(destination_save_file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(InputOutputStream_Release(destination_io_stream), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_EQ(Buffer_Release(owner_password_buffer), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Buffer_Release(user_password_buffer), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_EQ(DocumentEncryptionSettings_Release(encryption_settings), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Document_Release(memory_document), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_Release(memory_file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
    }

    TEST(Document, Encrypt_RC4_40) {
        EncryptDocument("owner", "user", EncryptionAlgorithmType_RC4, 40, UserAccessPermissionFlag_None);
    }

    TEST(Document, Encrypt_RC4_128) {
        EncryptDocument("owner", "user", EncryptionAlgorithmType_RC4, 128, UserAccessPermissionFlag_None);
    }

    TEST(Document, Encrypt_AES_40) {
        EncryptDocument("owner", "user", EncryptionAlgorithmType_AES, 40, UserAccessPermissionFlag_None);
    }

    TEST(Document, Encrypt_AES_128) {
        EncryptDocument("owner", "user", EncryptionAlgorithmType_AES, 128, UserAccessPermissionFlag_None);
    }

    // TODO: FIX AES-256
    //TEST(Document, Encrypt_AES_256) {
    //	EncryptDocument("owner", "user", EncryptionAlgorithmType_AES, 256, UserAccessPermissionFlag_None);
    //}

    TEST(Document, Sign) {

        FileHandle* source_memory_file = nullptr;
        DocumentHandle* source_memory_document = nullptr;
        InputOutputStreamHandle* io_stream = nullptr;

        DocumentSignatureSettingsHandle* signature_settings = nullptr;
        DateHandle* signing_time = nullptr;
        SigningKeyHandle* signing_key = nullptr;
        PKCS12KeyHandle* signature_pkcs12_key = nullptr;
        BufferHandle* signing_key_data = nullptr;

        FileHandle* destination_file = nullptr;
        InputOutputStreamHandle* destination_io_stream = nullptr;

        ASSERT_EQ(InputOutputStream_CreateFromMemory(&io_stream), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(io_stream, nullptr);

        ASSERT_EQ(File_CreateStream(io_stream, "temp", &source_memory_file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(source_memory_file, nullptr);

        ASSERT_EQ(Document_CreateFile(source_memory_file, &source_memory_document), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(source_memory_document, nullptr);

        // Create the destination stream file for output
        ASSERT_EQ(InputOutputStream_CreateFromMemory(&destination_io_stream), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(destination_io_stream, nullptr);

        ASSERT_EQ(File_CreateStream(destination_io_stream, "temp_destination", &destination_file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(destination_file, nullptr);

        // Configure the signature settings
        ASSERT_EQ(DocumentSignatureSettings_Create(&signature_settings), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(signature_settings, nullptr);

        ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE), sizeof(SIGNING_CERTIFICATE), &signing_key_data), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(signing_key_data, nullptr);

        ASSERT_EQ(PKCS12Key_CreateFromBuffer(signing_key_data, nullptr, &signature_pkcs12_key), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(signature_pkcs12_key, nullptr);

        ASSERT_EQ(PKCS12Key_ToSigningKey(signature_pkcs12_key, &signing_key), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(signing_key, nullptr);

        ASSERT_EQ(Date_CreateCurrent(&signing_time), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(signing_time, nullptr);

        ASSERT_EQ(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(DocumentSignatureSettings_SetSigningTime(signature_settings, signing_time), VANILLAPDF_ERROR_SUCCESS);

        // Sign the document and save it to the destination file
        ASSERT_EQ(Document_Sign(source_memory_document, destination_file, signature_settings), VANILLAPDF_ERROR_SUCCESS);

        // Cleanup

        ASSERT_EQ(Date_Release(signing_time), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_EQ(File_Release(destination_file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(InputOutputStream_Release(destination_io_stream), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_EQ(DocumentSignatureSettings_Release(signature_settings), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Document_Release(source_memory_document), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_Release(source_memory_file), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(InputOutputStream_Release(io_stream), VANILLAPDF_ERROR_SUCCESS);
    }
}
