// Copyright (c) 2025 Inan Evin

#ifndef VEKT_HPP
#define VEKT_HPP

#include <memory>
#include <assert.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <utility> // For std::swap, std::move

#ifndef VEKT_STRING
#include <string>
#endif

#ifndef VEKT_VARIANT
#include <variant>
#endif

#define VEKT_INLINE inline
#define VEKT_API	extern

namespace vekt
{
	////////////////////////////////////////////////////////////////////////////////
	// :: FONTS
	////////////////////////////////////////////////////////////////////////////////

	static constexpr unsigned char __icons_ttf[] = {
		0x00, 0x01, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x80, 0x00, 0x03, 0x00, 0x50, 0x46, 0x46, 0x54, 0x4d, 0xa8, 0xa5, 0xc0, 0x59, 0x00, 0x00, 0x07, 0x94, 0x00, 0x00, 0x00, 0x1c, 0x47, 0x44, 0x45, 0x46, 0x00, 0x27, 0x00, 0x0e, 0x00, 0x00, 0x07, 0x74, 0x00, 0x00,
		0x00, 0x1e, 0x4f, 0x53, 0x2f, 0x32, 0x55, 0xe5, 0x62, 0x2c, 0x00, 0x00, 0x01, 0x58, 0x00, 0x00, 0x00, 0x56, 0x63, 0x6d, 0x61, 0x70, 0x05, 0x28, 0x0d, 0xfa, 0x00, 0x00, 0x01, 0xd0, 0x00, 0x00, 0x01, 0x52, 0x67, 0x61, 0x73, 0x70, 0xff, 0xff, 0x00, 0x03,
		0x00, 0x00, 0x07, 0x6c, 0x00, 0x00, 0x00, 0x08, 0x67, 0x6c, 0x79, 0x66, 0x2f, 0x77, 0x44, 0x4a, 0x00, 0x00, 0x03, 0x38, 0x00, 0x00, 0x01, 0xc8, 0x68, 0x65, 0x61, 0x64, 0x2c, 0x29, 0x39, 0xf4, 0x00, 0x00, 0x00, 0xdc, 0x00, 0x00, 0x00, 0x36, 0x68, 0x68,
		0x65, 0x61, 0x07, 0xf9, 0x04, 0xcd, 0x00, 0x00, 0x01, 0x14, 0x00, 0x00, 0x00, 0x24, 0x68, 0x6d, 0x74, 0x78, 0x1e, 0x08, 0x03, 0x10, 0x00, 0x00, 0x01, 0xb0, 0x00, 0x00, 0x00, 0x20, 0x6c, 0x6f, 0x63, 0x61, 0x01, 0xac, 0x01, 0x36, 0x00, 0x00, 0x03, 0x24,
		0x00, 0x00, 0x00, 0x12, 0x6d, 0x61, 0x78, 0x70, 0x00, 0x4c, 0x00, 0x33, 0x00, 0x00, 0x01, 0x38, 0x00, 0x00, 0x00, 0x20, 0x6e, 0x61, 0x6d, 0x65, 0xc8, 0xc7, 0x9a, 0x50, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x02, 0x2e, 0x70, 0x6f, 0x73, 0x74, 0x9e, 0xd1,
		0x38, 0xb1, 0x00, 0x00, 0x07, 0x30, 0x00, 0x00, 0x00, 0x3a, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xd2, 0x92, 0x52, 0x08, 0x5f, 0x0f, 0x3c, 0xf5, 0x00, 0x0b, 0x03, 0xe8, 0x00, 0x00, 0x00, 0x00, 0xe4, 0x51, 0x11, 0xc1, 0x00, 0x00, 0x00, 0x00,
		0xe4, 0x7d, 0xe3, 0x66, 0x00, 0x3f, 0x00, 0x47, 0x03, 0xfd, 0x03, 0xa1, 0x00, 0x00, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x03, 0xa1, 0x00, 0x00, 0x00, 0x5a, 0x04, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfd,
		0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x30, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00,
		0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0xb4, 0x01, 0x90, 0x00, 0x05, 0x00, 0x00, 0x02, 0x8a, 0x02, 0xbc, 0x00, 0x00, 0x00, 0x8c, 0x02, 0x8a, 0x02, 0xbc, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x31, 0x01, 0x02, 0x00, 0x00, 0x02, 0x00,
		0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x66, 0x45, 0x64, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x24, 0x03, 0xe8, 0x00, 0x00, 0x00, 0x5a, 0x03, 0xa1,
		0xff, 0xb9, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe8, 0x00, 0x00, 0x03, 0xe8, 0x00, 0x00, 0x00, 0xfa, 0x00, 0x00, 0x03, 0xe8, 0x00, 0x47, 0x03, 0xe8, 0x00, 0x49, 0x04, 0xc4, 0x00, 0xc7, 0x04, 0xc2, 0x01, 0x7a, 0x03, 0xe8,
		0x00, 0x3f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4c, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x04, 0x00, 0x30, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x02,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x24, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x20, 0xff, 0xff, 0x00, 0x01, 0xff, 0xf5, 0xff, 0xe3, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x03, 0x04, 0x05, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x7a, 0x00, 0x9a, 0x00, 0xbc, 0x00, 0xe4, 0x00, 0x00, 0x00, 0x01, 0x00, 0x47, 0x00, 0x47, 0x03, 0xa1, 0x03, 0xa1, 0x00, 0x1c, 0x00, 0x00, 0x01, 0x14,
		0x07, 0x06, 0x07, 0x06, 0x23, 0x22, 0x27, 0x26, 0x27, 0x26, 0x27, 0x3c, 0x01, 0x26, 0x35, 0x34, 0x37, 0x3e, 0x01, 0x33, 0x32, 0x17, 0x16, 0x17, 0x16, 0x15, 0x14, 0x03, 0xa0, 0x39, 0x39, 0x63, 0x62, 0x75, 0x74, 0x64, 0x63, 0x38, 0x36, 0x03, 0x01, 0x3b,
		0x3c, 0xbc, 0x7a, 0x7b, 0x5c, 0x5e, 0x3b, 0x3d, 0x01, 0xf4, 0x76, 0x61, 0x61, 0x3b, 0x3a, 0x3a, 0x3a, 0x62, 0x5f, 0x40, 0x12, 0x1f, 0x11, 0x01, 0x6e, 0x61, 0x5f, 0x74, 0x3a, 0x3b, 0x5c, 0x5e, 0x6a, 0x0d, 0x00, 0x02, 0x00, 0x49, 0x00, 0x47, 0x03, 0x9f,
		0x03, 0xa1, 0x00, 0x17, 0x00, 0x2f, 0x00, 0x00, 0x01, 0x22, 0x07, 0x06, 0x07, 0x06, 0x15, 0x14, 0x17, 0x16, 0x17, 0x16, 0x33, 0x32, 0x37, 0x36, 0x37, 0x36, 0x35, 0x34, 0x27, 0x26, 0x27, 0x26, 0x01, 0x14, 0x07, 0x06, 0x07, 0x06, 0x23, 0x22, 0x27, 0x26,
		0x27, 0x26, 0x35, 0x34, 0x37, 0x36, 0x37, 0x36, 0x33, 0x32, 0x17, 0x16, 0x17, 0x16, 0x01, 0xf4, 0x52, 0x47, 0x45, 0x29, 0x28, 0x29, 0x28, 0x45, 0x44, 0x55, 0x54, 0x44, 0x42, 0x2c, 0x2b, 0x29, 0x28, 0x48, 0x47, 0x01, 0x5a, 0x39, 0x37, 0x64, 0x62, 0x75,
		0x74, 0x64, 0x62, 0x3a, 0x37, 0x3b, 0x39, 0x5f, 0x5e, 0x7a, 0x7b, 0x5c, 0x5d, 0x3c, 0x3b, 0x03, 0x24, 0x29, 0x28, 0x47, 0x46, 0x52, 0x51, 0x47, 0x46, 0x29, 0x29, 0x29, 0x28, 0x47, 0x46, 0x52, 0x51, 0x47, 0x46, 0x29, 0x29, 0xfe, 0xce, 0x74, 0x61, 0x61,
		0x3b, 0x3a, 0x3a, 0x39, 0x63, 0x63, 0x74, 0x77, 0x60, 0x61, 0x3b, 0x3a, 0x3a, 0x3b, 0x61, 0x62, 0x00, 0x00, 0x00, 0x01, 0x00, 0xc7, 0x00, 0xfa, 0x03, 0xfd, 0x02, 0xca, 0x00, 0x0f, 0x00, 0x00, 0x01, 0x14, 0x07, 0x01, 0x06, 0x22, 0x27, 0x01, 0x26, 0x34,
		0x36, 0x33, 0x21, 0x32, 0x16, 0x15, 0x03, 0xfd, 0x0e, 0xfe, 0x98, 0x10, 0x28, 0x10, 0xfe, 0x98, 0x10, 0x20, 0x14, 0x02, 0xd0, 0x14, 0x1e, 0x02, 0x96, 0x13, 0x11, 0xfe, 0x98, 0x10, 0x10, 0x01, 0x68, 0x10, 0x28, 0x20, 0x1e, 0x13, 0x00, 0x00, 0x00, 0x01,
		0x01, 0x7a, 0x00, 0x58, 0x03, 0x48, 0x03, 0x90, 0x00, 0x11, 0x00, 0x00, 0x01, 0x16, 0x15, 0x14, 0x07, 0x01, 0x06, 0x22, 0x26, 0x35, 0x11, 0x34, 0x36, 0x37, 0x36, 0x33, 0x32, 0x17, 0x03, 0x3a, 0x0e, 0x0e, 0xfe, 0x98, 0x10, 0x28, 0x20, 0x20, 0x0a, 0x08,
		0x05, 0x12, 0x0f, 0x02, 0x18, 0x0e, 0x16, 0x13, 0x11, 0xfe, 0x98, 0x10, 0x20, 0x14, 0x02, 0xd0, 0x17, 0x1a, 0x02, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x3f, 0x00, 0xbb, 0x03, 0xaa, 0x03, 0x2d, 0x00, 0x15, 0x00, 0x00, 0x01, 0x16, 0x14, 0x07,
		0x01, 0x06, 0x23, 0x22, 0x2f, 0x01, 0x26, 0x34, 0x36, 0x33, 0x37, 0x32, 0x1f, 0x01, 0x01, 0x36, 0x33, 0x32, 0x03, 0x98, 0x12, 0x12, 0xfe, 0x0c, 0x13, 0x19, 0x1a, 0x13, 0xfa, 0x12, 0x24, 0x0d, 0x0e, 0x19, 0x13, 0xce, 0x01, 0xc7, 0x13, 0x1a, 0x19, 0x03,
		0x1a, 0x12, 0x34, 0x12, 0xfe, 0x0c, 0x13, 0x13, 0xfa, 0x12, 0x34, 0x24, 0x01, 0x13, 0xcd, 0x01, 0xc7, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x96, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0f, 0x00, 0x20, 0x00, 0x01, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x02, 0x00, 0x07, 0x00, 0x40, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x33, 0x00, 0xb0, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x17, 0x01, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x0b,
		0x01, 0x44, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x17, 0x01, 0x80, 0x00, 0x03, 0x00, 0x01, 0x04, 0x09, 0x00, 0x01, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x04, 0x09, 0x00, 0x02, 0x00, 0x0e, 0x00, 0x30, 0x00, 0x03, 0x00, 0x01,
		0x04, 0x09, 0x00, 0x03, 0x00, 0x66, 0x00, 0x48, 0x00, 0x03, 0x00, 0x01, 0x04, 0x09, 0x00, 0x04, 0x00, 0x2e, 0x00, 0xe4, 0x00, 0x03, 0x00, 0x01, 0x04, 0x09, 0x00, 0x05, 0x00, 0x16, 0x01, 0x2c, 0x00, 0x03, 0x00, 0x01, 0x04, 0x09, 0x00, 0x06, 0x00, 0x2e,
		0x01, 0x50, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x61, 0x00, 0x45, 0x00, 0x64, 0x00, 0x69, 0x00, 0x74, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x49, 0x00, 0x63, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x73, 0x00, 0x00, 0x4c, 0x69, 0x6e, 0x61, 0x45, 0x64, 0x69, 0x74,
		0x6f, 0x72, 0x49, 0x63, 0x6f, 0x6e, 0x73, 0x00, 0x00, 0x52, 0x00, 0x65, 0x00, 0x67, 0x00, 0x75, 0x00, 0x6c, 0x00, 0x61, 0x00, 0x72, 0x00, 0x00, 0x52, 0x65, 0x67, 0x75, 0x6c, 0x61, 0x72, 0x00, 0x00, 0x46, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x74, 0x00, 0x46,
		0x00, 0x6f, 0x00, 0x72, 0x00, 0x67, 0x00, 0x65, 0x00, 0x20, 0x00, 0x32, 0x00, 0x2e, 0x00, 0x30, 0x00, 0x20, 0x00, 0x3a, 0x00, 0x20, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x61, 0x00, 0x45, 0x00, 0x64, 0x00, 0x69, 0x00, 0x74, 0x00, 0x6f, 0x00, 0x72,
		0x00, 0x49, 0x00, 0x63, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x73, 0x00, 0x20, 0x00, 0x52, 0x00, 0x65, 0x00, 0x67, 0x00, 0x75, 0x00, 0x6c, 0x00, 0x61, 0x00, 0x72, 0x00, 0x20, 0x00, 0x3a, 0x00, 0x20, 0x00, 0x32, 0x00, 0x32, 0x00, 0x2d, 0x00, 0x36, 0x00, 0x2d,
		0x00, 0x32, 0x00, 0x30, 0x00, 0x32, 0x00, 0x35, 0x00, 0x00, 0x46, 0x6f, 0x6e, 0x74, 0x46, 0x6f, 0x72, 0x67, 0x65, 0x20, 0x32, 0x2e, 0x30, 0x20, 0x3a, 0x20, 0x4c, 0x69, 0x6e, 0x61, 0x45, 0x64, 0x69, 0x74, 0x6f, 0x72, 0x49, 0x63, 0x6f, 0x6e, 0x73, 0x20,
		0x52, 0x65, 0x67, 0x75, 0x6c, 0x61, 0x72, 0x20, 0x3a, 0x20, 0x32, 0x32, 0x2d, 0x36, 0x2d, 0x32, 0x30, 0x32, 0x35, 0x00, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x61, 0x00, 0x45, 0x00, 0x64, 0x00, 0x69, 0x00, 0x74, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x49,
		0x00, 0x63, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x73, 0x00, 0x20, 0x00, 0x52, 0x00, 0x65, 0x00, 0x67, 0x00, 0x75, 0x00, 0x6c, 0x00, 0x61, 0x00, 0x72, 0x00, 0x00, 0x4c, 0x69, 0x6e, 0x61, 0x45, 0x64, 0x69, 0x74, 0x6f, 0x72, 0x49, 0x63, 0x6f, 0x6e, 0x73, 0x20,
		0x52, 0x65, 0x67, 0x75, 0x6c, 0x61, 0x72, 0x00, 0x00, 0x56, 0x00, 0x65, 0x00, 0x72, 0x00, 0x73, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x20, 0x00, 0x31, 0x00, 0x2e, 0x00, 0x30, 0x00, 0x00, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x31, 0x2e,
		0x30, 0x00, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x61, 0x00, 0x45, 0x00, 0x64, 0x00, 0x69, 0x00, 0x74, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x49, 0x00, 0x63, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x73, 0x00, 0x2d, 0x00, 0x52, 0x00, 0x65, 0x00, 0x67, 0x00, 0x75,
		0x00, 0x6c, 0x00, 0x61, 0x00, 0x72, 0x00, 0x00, 0x4c, 0x69, 0x6e, 0x61, 0x45, 0x64, 0x69, 0x74, 0x6f, 0x72, 0x49, 0x63, 0x6f, 0x6e, 0x73, 0x2d, 0x52, 0x65, 0x67, 0x75, 0x6c, 0x61, 0x72, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07,
		0x07, 0x75, 0x6e, 0x69, 0x30, 0x30, 0x30, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x01, 0x00, 0x07, 0x00, 0x01, 0x00, 0x04,
		0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xdf, 0xd6, 0xcb, 0x31, 0x00, 0x00, 0x00, 0x00, 0xe4, 0x51, 0x11, 0xc1, 0x00, 0x00, 0x00, 0x00, 0xe4, 0x7d, 0xe3, 0x66};
	static constexpr unsigned int __icons_ttf_len = 1968;

	////////////////////////////////////////////////////////////////////////////////
	// :: COMMON DEFINES
	////////////////////////////////////////////////////////////////////////////////

#define M_PI	  3.14159265358979f
#define DEG_2_RAD 0.0174533f
#ifndef VEKT_USER_DATA_SIZE
#define VEKT_USER_DATA_SIZE 1024
#endif

#if defined _MSC_VER && !__INTEL_COMPILER
#define ALIGNED_MALLOC(SZ, ALIGN) _aligned_malloc(SZ, ALIGN)
#define ALIGNED_FREE(PTR)		  _aligned_free(PTR);
#else
#include <stdlib.h>
#define ALIGNED_MALLOC(SZ, ALIGN) std::aligned_alloc(ALIGN, SZ)
#define ALIGNED_FREE(PTR)		  std::free(PTR)
#endif

#define MEMMOVE(...) memmove(__VA_ARGS__)
#define REALLOC(...) realloc(__VA_ARGS__)
#define MEMCPY(...)	 memcpy(__VA_ARGS__)
#define ASSERT(...)	 assert(__VA_ARGS__)

#ifndef VEKT_STRING
#define VEKT_STRING std::string
#endif

#ifndef VEKT_VARIANT
#define VEKT_VARIANT std::variant
#endif

	////////////////////////////////////////////////////////////////////////////////
	// :: LOGS & CONFIGS
	////////////////////////////////////////////////////////////////////////////////

	enum class log_verbosity
	{
		info,
		warning,
		error
	};

	typedef void (*log_callback)(log_verbosity, const char*, ...);
	struct config_data
	{
		log_callback on_log		  = nullptr;
		unsigned int atlas_width  = 1024;
		unsigned int atlas_height = 1024;
	};

	extern config_data config;

#define V_LOG(...)                                                                                                                                                                                                                                                 \
	if (vekt::config.on_log) vekt::config.on_log(vekt::log_verbosity::info, __VA_ARGS__)
#define V_ERR(...)                                                                                                                                                                                                                                                 \
	if (vekt::config.on_log) vekt::config.on_log(vekt::log_verbosity::error, __VA_ARGS__)
#define V_WARN(...)                                                                                                                                                                                                                                                \
	if (vekt::config.on_log) vekt::config.on_log(vekt::log_verbosity::warning, __VA_ARGS__)

	static inline void* mmm(size_t sz)
	{
		if (sz < 0) { int a = 5; }
		V_LOG("Malloced %d", sz);
		return malloc(sz);
	}

	static inline void fff(void* ptr)
	{
		// V_LOG("freed");
		free(ptr);
	}
#define MALLOC(SZ) mmm(SZ)
#define FREE(X)	   fff(X)

	////////////////////////////////////////////////////////////////////////////////
	// :: COMMON CONTAINERS
	////////////////////////////////////////////////////////////////////////////////

	typedef unsigned int id;

	template <typename T>
	class pod_vector
	{
	public:
		using iterator								   = T*;
		using const_iterator						   = const T*;
		static constexpr unsigned int initial_capacity = 4;

		pod_vector() {};
		pod_vector(const pod_vector<T>& other)
		{
			if (other.empty()) { return; }

			_capacity = other._capacity;
			_elements = reinterpret_cast<T*>(MALLOC(_capacity * sizeof(T)));
			if (!_elements) throw std::bad_alloc();

			for (unsigned int i = 0; i < other._count; ++i)
			{
				new (&_elements[i]) T(other._elements[i]);
			}
			_count = other._count;
		}

		pod_vector<T>& operator=(const pod_vector<T>& other)
		{
			if (this == &other) { return *this; }

			clear();

			if (other.empty()) { return *this; }

			_capacity = other._capacity;
			_elements = reinterpret_cast<T*>(MALLOC(_capacity * sizeof(T)));
			if (!_elements) { throw std::bad_alloc(); }

			_count = other._count;
			MEMCPY(_elements, other._elements, _count * sizeof(T));

			return *this;
		}

		~pod_vector() { clear(); }

		inline void push_back(const T& elem)
		{
			check_grow();
			new (&_elements[_count]) T(elem);
			_count++;
		}

		inline void push_back(T&& elem)
		{
			check_grow();
			new (&_elements[_count]) T(std::move(elem));
			_count++;
		}

		inline void increment_back()
		{
			const bool req_place = _count >= _capacity;
			check_grow();
			if (req_place) new (&_elements[_count]) T();
			_count++;
		}

		inline T& get_back() { return _elements[_count - 1]; }

		inline void remove(unsigned int index)
		{
			if (index >= _count) { return; }
			_elements[index].~T();

			if (index < _count - 1)
			{
				for (unsigned int i = index; i < _count - 1; ++i)
					_elements[i] = std::move(_elements[i + 1]);
			}
			_count--;
		}

		inline void remove(T& elem)
		{
			for (unsigned int i = 0; i < _count; ++i)
			{
				if (_elements[i] == elem)
				{
					remove(i);
					return;
				}
			}
		}

		inline void remove(iterator it) { remove(*it); }

		inline void clear()
		{
			for (unsigned int i = 0; i < _count; ++i)
				_elements[i].~T();

			if (_elements) { FREE(_elements); }
			_elements = nullptr;
			_count	  = 0;
			_capacity = 0;
		}

		inline void reserve(unsigned int new_capacity)
		{
			if (new_capacity <= _capacity) { return; }

			T* new_elements = (T*)MALLOC(new_capacity * sizeof(T));
			if (!new_elements) { throw std::bad_alloc(); }

			for (unsigned int i = 0; i < _count; ++i)
			{
				new (&new_elements[i]) T(std::move(_elements[i]));
				_elements[i].~T();
			}

			for (unsigned int i = _capacity; i < new_capacity; i++)
			{
				new (&new_elements[i]) T();
			}

			if (_elements) { FREE(_elements); }

			_elements = new_elements;
			_capacity = new_capacity;
		}

		inline void resize(unsigned int sz, bool call_dest = true)
		{
			// Handle destructors if shrinking.
			if (call_dest && sz < _count)
			{
				for (unsigned int i = sz; i < _count; ++i)
					_elements[i].~T();
			}

			if (sz > _capacity)
			{
				T* new_elements = (T*)MALLOC(sz * sizeof(T));
				if (!new_elements) throw std::bad_alloc();

				// Move existing elements
				for (unsigned int i = 0; i < _count; ++i)
				{
					new (&new_elements[i]) T(std::move(_elements[i]));
					_elements[i].~T();
				}

				FREE(_elements);
				_elements = new_elements;
				_capacity = sz;
			}

			// If growing, the new elements are default-constructed
			if (sz > _count)
			{
				for (unsigned int i = _count; i < sz; ++i)
					new (&_elements[i]) T();
			}
			_count = sz;
		}

		inline iterator find(T& t)
		{
			for (unsigned int i = 0; i < _count; i++)
			{
				if (t == _elements[i]) return _elements + i;
			}

			return end();
		};

		inline T* data() const { return _elements; }

		T&					operator[](unsigned int index) { return _elements[index]; }
		const T&			operator[](unsigned int index) const { return _elements[index]; }
		inline bool			empty() const { return _count == 0; }
		inline unsigned int size() const { return _count; }

		inline iterator		  begin() { return _elements; }
		inline const_iterator begin() const { return _elements; }
		inline const_iterator cbegin() const { return _elements; }
		inline iterator		  end() { return _elements + _count; }
		inline const_iterator end() const { return _elements + _count; }
		inline const_iterator cend() const { return _elements + _count; }

	private:
		inline void check_grow()
		{
			if (_count >= _capacity)
			{
				unsigned int new_capacity = (_capacity == 0) ? initial_capacity : _capacity * 2;
				reserve(new_capacity);
			}
		}

	private:
		T*			 _elements = nullptr;
		unsigned int _count	   = 0;
		unsigned int _capacity = 0;
	};

	struct handle
	{
		id	 value = 0;
		bool alive = false;
	};

	template <typename T>
	class pool
	{
	public:
		~pool() { clear(); }

		inline void init(unsigned int size)
		{
			_elements = reinterpret_cast<T*>(ALIGNED_MALLOC(sizeof(T) * size, alignof(T)));
			_size	  = size;
			_head	  = 0;

			if (_elements)
			{
				for (unsigned int i = 0; i < size; i++)
				{
					new (&_elements[i]) T();
					T& elem					= _elements[i];
					elem._pool_handle.value = i;
					elem._pool_handle.alive = false;
				}
			}
		}

		inline void clear()
		{
			if (_elements) ALIGNED_FREE(_elements);
			_size	  = 0;
			_head	  = 0;
			_elements = nullptr;
		}

		template <typename... Args>
		inline T* allocate(Args&&... args)
		{
			unsigned int index = 0;

			if (!_freelist.empty())
			{
				index = _freelist[_freelist.size() - 1];
				_freelist.remove(_freelist.size() - 1);
			}
			else
			{
				ASSERT(_head < _size);
				index = _head;
				_head++;
			}

			id pool_handle			 = _elements[index]._pool_handle.value;
			T* elem					 = new (&_elements[index]) T(args...);
			elem->_pool_handle.alive = true;
			elem->_pool_handle.value = pool_handle;
			return elem;
		}

		inline void deallocate(T* ptr)
		{
			ASSERT(ptr);
			ASSERT(ptr->_pool_handle.alive);
			ptr->_pool_handle.alive = false;
			_freelist.push_back(ptr->_pool_handle.value);
			ptr->~T();
		}

	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type		= T;
			using diff_type			= std::ptrdiff_t;
			using pointer			= T*;
			using reference			= T&;

			iterator(pool<T>* p, unsigned int current_index) : _pool(p), _current_index(current_index) {};

			T*	 operator*() const { return &_pool->_elements[_current_index]; }
			T*	 operator->() const { return &_pool->_elements[_current_index]; }
			bool operator==(const iterator& other) const { return _current_index == other._current_index; }
			bool operator!=(const iterator& other) const { return _current_index != other._current_index; }

			iterator& operator++()
			{
				do
				{
					_current_index++;
				} while (_current_index < _pool->_size && !_pool->_elements[_current_index]._pool_handle.alive);
				return *this;
			}

			iterator& operator++(int)
			{
				iterator temp = *this;
				operator++();
				return temp;
			}

		private:
			pool<T>*	 _pool			= nullptr;
			unsigned int _current_index = 0;
		};

		iterator begin() { return iterator(this, 0); }
		iterator end() { return iterator(this, _size); }

	private:
		T*						 _elements = nullptr;
		unsigned int			 _size	   = 0;
		unsigned int			 _head	   = 0;
		pod_vector<unsigned int> _freelist = {};
	};

	////////////////////////////////////////////////////////////////////////////////
	// :: VECTORS & MATH
	////////////////////////////////////////////////////////////////////////////////

	struct vec2
	{
		float x = 0.0f;
		float y = 0.0f;

		vec2 operator+(const vec2& other) const
		{
			vec2 v = {};
			v.x	   = x + other.x;
			v.y	   = y + other.y;
			return v;
		}

		vec2 operator*(float f) const
		{
			vec2 v = *this;
			v.x *= f;
			v.y *= f;
			return v;
		}

		vec2 operator-(const vec2& other) const
		{
			vec2 v = *this;
			v.x -= other.x;
			v.y -= other.y;
			return v;
		}

		vec2 operator/=(const vec2& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		vec2 operator/=(float f)
		{
			x /= f;
			y /= f;
			return *this;
		}

		vec2 operator*=(float f)
		{
			x *= f;
			y *= f;
			return *this;
		}

		vec2 operator+=(const vec2& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		inline void normalize()
		{
			const float s = mag();
			x /= s;
			y /= s;
		}

		inline vec2 normalized()
		{
			vec2 v = *this;
			v.normalize();
			return v;
		}

		inline float mag() { return sqrt(x * x + y * y); }
		inline float mag2() { return x * x + y * y; }
	};

	class math
	{
	public:
		template <typename T>
		static inline T max(T a, T b)
		{
			return a > b ? a : b;
		}
		template <typename T>
		static inline T min(T a, T b)
		{
			return a < b ? a : b;
		}
		static inline float equals(float a, float b, float eps = 0.0001f) { return a > b - eps && a < b + eps; }
		static inline float cos(float x) { return std::cos(x); }
		static inline float sin(float x) { return std::sin(x); }
		static inline float lerp(float a, float b, float t) { return a + (b - a) * t; }
		static inline float ceilf(float f) { return std::ceilf(f); }
		static inline float remap(float val, float from_low, float from_high, float to_low, float to_high) { return to_low + (val - from_low) * (to_high - to_low) / (from_high - from_low); }
	};

	struct vec4
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

		bool equals(const vec4& other) const { return math::equals(x, other.x, 0.1f) && math::equals(y, other.y, 0.1f) && math::equals(z, other.z, 0.1f) && math::equals(w, other.w, 0.1f); }
		bool is_point_inside(const vec2& point) const { return point.x >= x && point.x <= x + z && point.y >= y && point.y <= y + w; }

		static inline vec4 lerp(const vec4& a, const vec4& b, float t) { return vec4(math::lerp(a.x, b.x, t), math::lerp(a.y, b.y, t), math::lerp(a.z, b.z, t), math::lerp(a.w, b.w, t)); }

		vec4 operator+(const vec4& other) const
		{
			vec4 v = {};
			v.x	   = x + other.x;
			v.y	   = y + other.y;
			v.z	   = z + other.z;
			v.w	   = w + other.w;
			return v;
		}

		vec4 operator*(float f) const
		{
			vec4 v = *this;
			v.x *= f;
			v.y *= f;
			v.z *= f;
			v.w *= f;
			return v;
		}
	};

	////////////////////////////////////////////////////////////////////////////////
	// :: INPUT
	////////////////////////////////////////////////////////////////////////////////

	enum class input_event_type
	{
		pressed,
		released,
		repeated,
	};

	enum class input_event_result
	{
		handled,
		not_handled,
	};

	struct mouse_event
	{
		input_event_type type	= input_event_type::pressed;
		int				 button = 0;
	};

	struct mouse_wheel_event
	{
		float amount = 0.0f;
	};

	struct key_event
	{
		input_event_type type	   = input_event_type::pressed;
		int				 key	   = 0;
		int				 scan_code = 0;
	};

	class widget;
	typedef std::function<input_event_result(widget* w, const mouse_event& ev, widget*& last_widget)>		custom_mouse_event;
	typedef std::function<input_event_result(widget* w, const key_event& ev, widget*& last_widget)>			custom_key_event;
	typedef std::function<input_event_result(widget* w, const mouse_wheel_event& ev, widget*& last_widget)> custom_mouse_wheel_event;
	typedef std::function<void(widget* w, const mouse_event& ev)>											custom_click_event;
	typedef std::function<void(widget* w, const vec2& mouse, const vec2& mouse_delta)>						custom_mouse_drag_event;

	////////////////////////////////////////////////////////////////////////////////
	// :: WIDGET UTILS
	////////////////////////////////////////////////////////////////////////////////

	enum widget_flags
	{
		wf_pos_x_relative		 = 1 << 0,
		wf_pos_y_relative		 = 1 << 1,
		wf_pos_x_absolute		 = 1 << 2,
		wf_pos_y_absolute		 = 1 << 3,
		wf_size_x_relative		 = 1 << 4,
		wf_size_y_relative		 = 1 << 5,
		wf_size_x_absolute		 = 1 << 6,
		wf_size_y_absolute		 = 1 << 7,
		wf_size_x_copy_y		 = 1 << 8,
		wf_size_y_copy_x		 = 1 << 9,
		wf_size_x_total_children = 1 << 10,
		wf_size_x_max_children	 = 1 << 11,
		wf_size_y_total_children = 1 << 12,
		wf_size_y_max_children	 = 1 << 13,
		wf_size_x_fill			 = 1 << 14,
		wf_size_y_fill			 = 1 << 15,
		wf_invisible			 = 1 << 16,
		wf_pos_anchor_x_center	 = 1 << 17,
		wf_pos_anchor_x_end		 = 1 << 18,
		wf_pos_anchor_y_center	 = 1 << 19,
		wf_pos_anchor_y_end		 = 1 << 20,
	};

	enum class helper_pos_type
	{
		absolute,
		relative,
	};

	enum class child_positioning
	{
		none,
		row,
		column,
	};

	enum class helper_size_type
	{
		absolute,
		relative,
		fill,
		max_children,
		total_children,
		copy_other,
	};

	enum class helper_anchor_type
	{
		start,
		center,
		end
	};

	enum class direction
	{
		horizontal,
		vertical
	};

	struct margins
	{
		float top	 = 0.0f;
		float bottom = 0.0f;
		float left	 = 0.0f;
		float right	 = 0.0f;
	};

	////////////////////////////////////////////////////////////////////////////////
	// :: WIDGET GFX
	////////////////////////////////////////////////////////////////////////////////

	enum class gfx_type
	{
		none,
		filled_rect,
		stroke_rect,
		text,
	};

	struct gfx_filled_rect
	{
		vec4		 color_start	   = vec4(1, 1, 1, 1);
		vec4		 color_end		   = vec4(1, 1, 1, 1);
		vec4		 hovered_color	   = vec4(1, 1, 1, 0);
		vec4		 pressed_color	   = vec4(1, 1, 1, 0);
		float		 rounding		   = 0.0f;
		unsigned int segments		   = 0;
		unsigned int outline_thickness = 0;
		vec4		 outline_color	   = {};
		unsigned int aa_thickness	   = 0;
		direction	 color_direction   = direction::horizontal;
		bool		 clip_children	   = false;
	};

	struct gfx_stroke_rect
	{
		vec4		 color_start	 = vec4(1, 1, 1, 1);
		vec4		 color_end		 = vec4(1, 1, 1, 1);
		vec4		 hovered_color	 = vec4(1, 1, 1, 0);
		vec4		 pressed_color	 = vec4(1, 1, 1, 0);
		float		 rounding		 = 0.0f;
		unsigned int segments		 = 0;
		unsigned int thickness		 = 10;
		unsigned int aa_thickness	 = 0;
		direction	 color_direction = direction::horizontal;
		bool		 clip_children	 = false;
	};

	struct font;
	struct gfx_text
	{
		VEKT_STRING	 text			 = "";
		font*		 target_font	 = nullptr;
		unsigned int spacing		 = 0;
		vec4		 color_start	 = vec4(1, 1, 1, 1);
		vec4		 color_end		 = vec4(1, 1, 1, 1);
		vec4		 hovered_color	 = vec4(1, 1, 1, 0);
		vec4		 pressed_color	 = vec4(1, 1, 1, 0);
		direction	 color_direction = direction::horizontal;
		bool		 _dirty			 = true;

		inline void set_text(const VEKT_STRING& txt)
		{
			text   = txt;
			_dirty = true;
		}

		inline void set_font(font* fnt)
		{
			target_font = fnt;
			_dirty		= true;
		}
	};

	struct widget_gfx
	{

		VEKT_VARIANT<gfx_text, gfx_filled_rect, gfx_stroke_rect> gfx;

		void*		 user_data	= nullptr;
		gfx_type	 type		= gfx_type::none;
		unsigned int draw_order = 0;

		template <typename T>
		T& get_data()
		{
			return reinterpret_cast<T&>(gfx);
		}
	};

	////////////////////////////////////////////////////////////////////////////////
	// :: WIDGET
	////////////////////////////////////////////////////////////////////////////////

	typedef std::function<void(widget*)> custom_func;

	struct widget_data
	{
		VEKT_STRING				 debug_name		   = "";
		widget*					 parent			   = nullptr;
		custom_func				 on_hover_begin	   = nullptr;
		custom_func				 on_hover_end	   = nullptr;
		custom_mouse_event		 on_mouse		   = nullptr;
		custom_mouse_wheel_event on_mouse_wheel	   = nullptr;
		custom_click_event		 on_mouse_clicked  = nullptr;
		custom_mouse_drag_event	 on_mouse_dragged  = nullptr;
		custom_key_event		 on_key			   = nullptr;
		custom_func				 custom_pos_pass   = nullptr;
		custom_func				 custom_size_pass  = nullptr;
		pod_vector<widget*>		 children		   = {};
		child_positioning		 child_positioning = child_positioning::none;
		margins					 margins		   = {};
		vec2					 pos			   = {};
		vec2					 size			   = {};
		vec2					 final_pos		   = {};
		vec2					 final_size		   = {};
		vec2					 scroll_offset	   = {};
		unsigned int			 flags			   = 0;
		float					 spacing		   = 0.0f;
		bool					 receive_input	   = false;
	};

	class widget
	{
	public:
		widget() = default;
		widget(const widget_data& data) : _widget_data(data) {};

		void add_child(widget* w);
		void remove_child(widget* w);
		void set_visible(bool is_visible, bool recursive);

		inline void set_pos_x(float x, helper_pos_type type = helper_pos_type::relative, helper_anchor_type anchor = helper_anchor_type::start)
		{
			_widget_data.pos.x = x;
			_widget_data.flags &= ~(widget_flags::wf_pos_x_relative | widget_flags::wf_pos_x_absolute | widget_flags::wf_pos_anchor_x_center | widget_flags::wf_pos_anchor_x_end);

			switch (type)
			{
			case helper_pos_type::relative:
				_widget_data.flags |= widget_flags::wf_pos_x_relative;
				break;
			case helper_pos_type::absolute:
				_widget_data.flags |= widget_flags::wf_pos_x_absolute;
				break;
			default:
				break;
			}

			switch (anchor)
			{
			case helper_anchor_type::center:
				_widget_data.flags |= widget_flags::wf_pos_anchor_x_center;
				break;
			case helper_anchor_type::end:
				_widget_data.flags |= widget_flags::wf_pos_anchor_x_end;
				break;
			default:
				break;
			}
		}

		inline void set_pos_y(float y, helper_pos_type type = helper_pos_type::relative, helper_anchor_type anchor = helper_anchor_type::start)
		{
			_widget_data.pos.y = y;
			_widget_data.flags &= ~(widget_flags::wf_pos_y_relative | widget_flags::wf_pos_y_absolute | widget_flags::wf_pos_anchor_y_center | widget_flags::wf_pos_anchor_y_end);

			switch (type)
			{
			case helper_pos_type::relative:
				_widget_data.flags |= widget_flags::wf_pos_y_relative;
				break;
			case helper_pos_type::absolute:
				_widget_data.flags |= widget_flags::wf_pos_y_absolute;
				break;
			default:
				break;
			}

			switch (anchor)
			{
			case helper_anchor_type::center:
				_widget_data.flags |= widget_flags::wf_pos_anchor_y_center;
				break;
			case helper_anchor_type::end:
				_widget_data.flags |= widget_flags::wf_pos_anchor_y_end;
				break;
			default:
				break;
			}
		}

		inline void set_width(float width, helper_size_type type = helper_size_type::relative)
		{
			_widget_data.size.x = width;
			_widget_data.flags &= ~(widget_flags::wf_size_x_absolute | widget_flags::wf_size_x_relative | widget_flags::wf_size_x_fill | widget_flags::wf_size_x_copy_y | widget_flags::wf_size_x_max_children | widget_flags::wf_size_x_total_children);

			switch (type)
			{
			case helper_size_type::absolute:
				_widget_data.flags |= widget_flags::wf_size_x_absolute;
				break;
			case helper_size_type::relative:
				_widget_data.flags |= widget_flags::wf_size_x_relative;
				break;
			case helper_size_type::fill:
				_widget_data.flags |= widget_flags::wf_size_x_fill;
				break;
			case helper_size_type::max_children:
				_widget_data.flags |= widget_flags::wf_size_x_max_children;
				break;
			case helper_size_type::total_children:
				_widget_data.flags |= widget_flags::wf_size_x_total_children;
				break;
			case helper_size_type::copy_other:
				_widget_data.flags |= widget_flags::wf_size_x_copy_y;
				break;
			default:
				break;
			}
		}

		inline void set_height(float height, helper_size_type type = helper_size_type::relative)
		{
			_widget_data.size.y = height;
			_widget_data.flags &= ~(widget_flags::wf_size_y_absolute | widget_flags::wf_size_y_relative | widget_flags::wf_size_y_fill | widget_flags::wf_size_y_copy_x | widget_flags::wf_size_y_max_children | widget_flags::wf_size_y_total_children);

			switch (type)
			{
			case helper_size_type::absolute:
				_widget_data.flags |= widget_flags::wf_size_y_absolute;
				break;
			case helper_size_type::relative:
				_widget_data.flags |= widget_flags::wf_size_y_relative;
				break;
			case helper_size_type::fill:
				_widget_data.flags |= widget_flags::wf_size_y_fill;
				break;
			case helper_size_type::max_children:
				_widget_data.flags |= widget_flags::wf_size_y_max_children;
				break;
			case helper_size_type::total_children:
				_widget_data.flags |= widget_flags::wf_size_y_total_children;
				break;
			case helper_size_type::copy_other:
				_widget_data.flags |= widget_flags::wf_size_y_copy_x;
				break;
			default:
				break;
			}
		}

		inline void				set_draw_order(bool draw_order) { _widget_gfx.draw_order = draw_order; }
		inline bool				get_is_visible() const { return !(_widget_data.flags & widget_flags::wf_invisible); }
		inline widget_data&		get_data_widget() { return _widget_data; }
		inline widget_gfx&		get_gfx_data() { return _widget_gfx; }
		inline gfx_text&		get_gfx_text() { return set_gfx_type_text(); }
		inline gfx_filled_rect& get_gfx_filled_rect() { return set_gfx_type_filled_rect(); }
		inline gfx_stroke_rect& get_gfx_stroke_rect() { return set_gfx_type_stroke_rect(); }
		inline bool				get_is_hovered() const { return _is_hovered; };
		inline bool				get_is_pressed() const { return _press_states[0]; }

		inline void set_gfx_type_none() { _widget_gfx.type = gfx_type::none; }

		inline gfx_text& set_gfx_type_text()
		{
			if (_widget_gfx.type == gfx_type::text) return std::get<gfx_text>(_widget_gfx.gfx);
			_widget_gfx.gfx	 = gfx_text();
			_widget_gfx.type = gfx_type::text;
			return std::get<gfx_text>(_widget_gfx.gfx);
		}

		inline gfx_filled_rect& set_gfx_type_filled_rect()
		{
			if (_widget_gfx.type == gfx_type::filled_rect) return std::get<gfx_filled_rect>(_widget_gfx.gfx);
			_widget_gfx.gfx	 = gfx_filled_rect();
			_widget_gfx.type = gfx_type::filled_rect;
			return std::get<gfx_filled_rect>(_widget_gfx.gfx);
		}

		inline gfx_stroke_rect& set_gfx_type_stroke_rect()
		{
			if (_widget_gfx.type == gfx_type::stroke_rect) return std::get<gfx_stroke_rect>(_widget_gfx.gfx);
			_widget_gfx.gfx	 = gfx_stroke_rect();
			_widget_gfx.type = gfx_type::stroke_rect;
			return std::get<gfx_stroke_rect>(_widget_gfx.gfx);
		}

		inline bool is_point_in_bounds(unsigned int x, unsigned int y)
		{
			return x >= _widget_data.final_pos.x && x <= _widget_data.final_pos.x + _widget_data.final_size.x && y >= _widget_data.final_pos.y && y <= _widget_data.final_pos.y + _widget_data.final_size.y;
		}

		template <typename T>
		inline T* get_data_user() const
		{
			static_assert(sizeof(T) < VEKT_USER_DATA_SIZE);
			return static_cast<T*>(&_user_data);
		}

		inline vec4 get_clip_rect() const { return {_widget_data.final_pos.x, _widget_data.final_pos.y, _widget_data.final_size.x, _widget_data.final_size.y}; }

	private:
		void size_pass();
		void size_pass_children(pod_vector<widget*>& fill_x, pod_vector<widget*>& fill_y);
		void size_pass_post(pod_vector<widget*>& fill_x, pod_vector<widget*>& fill_y);
		void size_copy_check();
		void pos_pass();
		void pos_pass_children();
		void pos_pass_post();
		bool draw_pass_clip_check(class builder& builder);
		void draw_pass_clip_check_end(class builder& builder);
		void draw_pass(class builder& builder);
		void draw_pass_children(class builder& builder);

		template <typename EventType>
		input_event_result on_event_internal(const EventType& ev, widget*& out_last_widget)
		{
			if constexpr (std::is_same_v<EventType, mouse_event>)
			{
				if (_widget_data.receive_input && _widget_data.on_mouse)
				{
					const input_event_result res = _widget_data.on_mouse(this, ev, out_last_widget);
					if (res == input_event_result::handled) return res;
				}
			}
			else if constexpr (std::is_same_v<EventType, mouse_wheel_event>)
			{
				if (_widget_data.receive_input && _widget_data.on_key)
				{
					const input_event_result res = _widget_data.on_mouse_wheel(this, ev, out_last_widget);
					if (res == input_event_result::handled) return res;
				}
			}
			else if constexpr (std::is_same_v<EventType, key_event>)
			{
				if (_widget_data.receive_input && _widget_data.on_mouse_wheel)
				{
					const input_event_result res = _widget_data.on_key(this, ev, out_last_widget);
					if (res == input_event_result::handled) return res;
				}
			}
			else
				static_assert(false);

			out_last_widget = this;

			for (widget* w : _widget_data.children)
			{
				const input_event_result res = w->on_event_internal(ev, out_last_widget);
				if (res == input_event_result::handled) return res;
			}

			// Bubbling code if needed.
			/*
			{
				if (_widget_data.parent)
				{
					out_last_widget = this;

					widget* sibling = nullptr;
					for (widget* w : _widget_data.parent->_widget_data.children)
					{
						if (w == this)
						{
							if (sibling) { return sibling->find_deepest_child()->on_event_internal(ev, phase, out_last_widget); }
						}
						sibling = w;
					}

					if (sibling == nullptr) return _widget_data.parent->on_event_internal(ev, phase, out_last_widget);
				}
			}
			*/

			return input_event_result::not_handled;
		}

	private:
		friend class pool<widget>;
		friend class builder;

		handle		  _pool_handle = {};
		widget_data	  _widget_data = {};
		widget_gfx	  _widget_gfx  = {};
		unsigned char _user_data[VEKT_USER_DATA_SIZE];
		bool		  _is_hovered	   = false;
		bool		  _press_states[3] = {false};
	};

	////////////////////////////////////////////////////////////////////////////////
	// :: VERTICES
	////////////////////////////////////////////////////////////////////////////////

	struct vertex
	{
		vec2 pos;
		vec2 uv;
		vec4 color;
	};

	typedef unsigned short index;

	////////////////////////////////////////////////////////////////////////////////
	// :: BUILDER
	////////////////////////////////////////////////////////////////////////////////

	struct draw_buffer
	{
		void*		 user_data	   = nullptr;
		font*		 used_font	   = nullptr;
		vec4		 clip		   = vec4();
		vertex*		 vertex_start  = nullptr;
		index*		 index_start   = nullptr;
		unsigned int draw_order	   = 0;
		unsigned int vertex_count  = 0;
		unsigned int index_count   = 0;
		unsigned int _max_vertices = 0;
		unsigned int _max_indices  = 0;

		inline void add_vertex(const vertex& vtx)
		{
			ASSERT(vertex_count < _max_vertices);
			vertex_start[vertex_count] = vtx;
			vertex_count++;
		}

		inline vertex& add_get_vertex()
		{
			ASSERT(vertex_count < _max_vertices);
			const unsigned int idx = vertex_count;
			vertex_count++;
			return vertex_start[idx];
		}

		inline void add_index(index idx)
		{
			ASSERT(index_count < _max_indices);
			index_start[index_count] = idx;
			index_count++;
		}
	};

	typedef std::function<void(const draw_buffer& db)> draw_callback;

	class builder
	{
	public:
		struct input_layer
		{
			unsigned int priority = 0;
			widget*		 root	  = nullptr;
			bool		 operator==(const input_layer& other) const { return priority == other.priority && root == other.root; }
		};

		struct init_config
		{
			size_t widget_buffer_sz = 1024 * 1024;
			size_t vertex_buffer_sz = 1024 * 1024;
			size_t index_buffer_sz	= 1024 * 1024;
			size_t buffer_count		= 10;
		};

		builder()					  = default;
		builder(const builder& other) = delete;
		~builder() {}

		void			   init(const init_config& conf);
		void			   uninit();
		void			   build(const vec2& screen_size);
		void			   flush();
		void			   on_mouse_move(const vec2& mouse);
		input_event_result on_mouse_event(const mouse_event& ev);
		input_event_result on_mouse_wheel_event(const mouse_wheel_event& ev);
		input_event_result on_key_event(const key_event& ev);
		void			   add_input_layer(unsigned int priority, widget* root);
		void			   remove_input_layer(unsigned int priority);
		void			   add_filled_rect(const gfx_filled_rect& rect, const vec2& min, const vec2& max, unsigned int draw_order, void* user_data, bool use_hovered, bool use_pressed);
		void			   add_stroke_rect(const gfx_stroke_rect& rect, const vec2& min, const vec2& max, unsigned int draw_order, void* user_data, bool use_hovered, bool use_pressed);
		void			   add_text(const gfx_text& text, const vec2& position, const vec2& size, unsigned int draw_order, void* user_data, bool use_hovered, bool use_pressed);
		static vec2		   get_text_size(const gfx_text& text);
		draw_buffer*	   get_draw_buffer(unsigned int draw_order, void* user_data, font* fnt = nullptr);
		bool			   push_to_clip_stack(const vec4& rect);
		bool			   push_to_clip_stack_if_intersects(const vec4& rect);
		void			   pop_clip_stack();
		vec4			   calculate_intersection(const vec4& clip0, const vec4& clip1) const;

		// Widgets
		widget* widget_horizontal_divider(float height, const vec4& color = vec4(1, 1, 1, 1));
		widget* widget_vertical_divider(float width, const vec4& color = vec4(1, 1, 1, 1));
		widget* widget_button(float height, font* fnt, const VEKT_STRING& text, const vec4& color = vec4(0, 0, 0, 1), const vec4& text_color = vec4(1, 1, 1, 1));
		widget* widget_checkbox(float height, void* sdf_material, const vec4& color = vec4(0, 0, 0, 1), const vec4& check_color = vec4(1, 1, 1, 1));

		inline vec4 get_current_clip() const { return _clip_stack.empty() ? vec4() : _clip_stack[_clip_stack.size() - 1]; }
		inline void set_root(widget* root) { _root = root; }
		inline void set_on_draw(draw_callback cb) { _on_draw = cb; }

		template <typename... Args>
		widget* allocate(Args&&... args)
		{
			return _widget_pool.allocate(args...);
		}

		inline void deallocate(widget* w)
		{
			pod_vector<widget*>::iterator it = _press_state_history.find(w);
			if (it != _press_state_history.end()) { _press_state_history.remove(it); }

			for (widget* c : w->_widget_data.children)
				deallocate(c);

			_widget_pool.deallocate(w);
		}

	private:
		void	generate_rounded_rect(pod_vector<vec2>& out_path, const vec2& min, const vec2& max, float rounding, int segments);
		void	generate_sharp_rect(pod_vector<vec2>& out_path, const vec2& min, const vec2& max);
		void	generate_offset_rect(pod_vector<vec2>& out_path, const pod_vector<vec2>& base_path, float amount);
		void	generate_offset_rounded_rect(pod_vector<vec2>& out_path, const pod_vector<vec2>& base_path, float amount);
		void	add_strip(draw_buffer* db, unsigned int outer_start, unsigned int inner_start, unsigned int size, bool add_ccw);
		void	add_filled_rect(draw_buffer* db, unsigned int start, unsigned int size);
		void	add_filled_rect_central(draw_buffer* db, unsigned int start, unsigned int central_start, unsigned int size);
		void	add_vertices(draw_buffer* db, const pod_vector<vec2>& path, const vec4& color_start, const vec4& color_end, direction direction, const vec2& min, const vec2& max);
		void	add_central_vertex(draw_buffer* db, const vec4& color_start, const vec4& color_end, const vec2& min, const vec2& max);
		void	add_vertices_aa(draw_buffer* db, const pod_vector<vec2>& path, unsigned int original_vertices_idx, float alpha, const vec2& min, const vec2& max);
		widget* find_widget_at(widget* current_widget, const vec2& mouse);
		void	pass_hover_state(widget* w, const vec2& mouse);

	private:
		pool<widget>			_widget_pool;
		pod_vector<vec4>		_clip_stack;
		pod_vector<input_layer> _input_layers;
		pod_vector<draw_buffer> _draw_buffers;
		widget*					_root				 = nullptr;
		draw_callback			_on_draw			 = nullptr;
		pod_vector<widget*>		_press_state_history = {};
		vec2					_mouse_position		 = {};

		pod_vector<vec2>	_reuse_outer_path;
		pod_vector<vec2>	_reuse_inner_path;
		pod_vector<vec2>	_reuse_outline_path;
		pod_vector<vec2>	_reuse_aa_outer_path;
		pod_vector<vec2>	_reuse_aa_inner_path;
		pod_vector<widget*> _reuse_fill_x;
		pod_vector<widget*> _reuse_fill_y;

		vertex*		 _vertex_buffer			  = nullptr;
		index*		 _index_buffer			  = nullptr;
		unsigned int _vertex_count_per_buffer = 0;
		unsigned int _index_count_per_buffer  = 0;
		unsigned int _buffer_count			  = 0;
		unsigned int _buffer_counter		  = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// :: FONT & ATLAS & GLYPH
	////////////////////////////////////////////////////////////////////////////////

	class atlas;

	struct glyph
	{
		unsigned char* sdf_data			 = nullptr;
		int			   kern_advance[128] = {0};
		int			   width			 = 0;
		int			   height			 = 0;
		int			   advance_x		 = 0;
		int			   left_bearing		 = 0;
		float		   x_offset			 = 0.0f;
		float		   y_offset			 = 0.0f;
		int			   atlas_x			 = 0;
		int			   atlas_y			 = 0;
		float		   uv_x				 = 0.0f;
		float		   uv_y				 = 0.0f;
		float		   uv_w				 = 0.0f;
		float		   uv_h				 = 0.0f;
	};

	struct font
	{
		glyph		 glyph_info[128];
		atlas*		 _atlas					= nullptr;
		unsigned int _atlas_required_height = 0;
		unsigned int _atlas_pos				= 0;
		float		 _scale					= 0.0f;
		int			 ascent					= 0;
		int			 descent				= 0;
		int			 line_gap				= 0;
		unsigned int size					= 0;

		~font();
	};

	class atlas
	{
	public:
		struct slice
		{
			slice(unsigned int pos_y, unsigned int h) : pos(pos_y), height(h) {};
			unsigned int pos	= 0;
			unsigned int height = 0;
		};

		atlas(unsigned int width, unsigned int height);
		~atlas();

		bool				  add_font(font* font);
		void				  remove_font(font* font);
		bool				  empty() { return _fonts.empty(); }
		inline unsigned int	  get_width() const { return _width; }
		inline unsigned int	  get_height() const { return _height; }
		inline unsigned char* get_data() const { return _data; }
		inline unsigned int	  get_data_size() const { return _data_size; }

	private:
		unsigned int	   _width			 = 0;
		unsigned int	   _height			 = 0;
		pod_vector<slice*> _available_slices = {};
		pod_vector<font*>  _fonts			 = {};
		unsigned char*	   _data			 = nullptr;
		unsigned int	   _data_size		 = 0;
	};

	typedef std::function<void(atlas*)> atlas_cb;

	class font_manager
	{
	public:
		font_manager() {};
		~font_manager()
		{
			ASSERT(_atlases.empty());
			ASSERT(_fonts.empty());
		};

		static inline font_manager& get()
		{
			static font_manager fm;
			return fm;
		}

		void init();
		void uninit();

		font* load_font(const char* file, unsigned int size, unsigned int range_start = 0, unsigned int range_end = 128, bool is_sdf = false, int sdf_padding = 3, int sdf_edge = 128, float sdf_distance = 32.0f);
		void  unload_font(font* fnt);

		inline void	 set_atlas_created_callback(atlas_cb cb) { _atlas_created_cb = cb; }
		inline void	 set_atlas_updated_callback(atlas_cb cb) { _atlas_updated_cb = cb; }
		inline void	 set_atlas_destroyed_callback(atlas_cb cb) { _atlas_destroyed_cb = cb; }
		inline font* get_icons_font() const { return _icons_font; }

	private:
		void  find_atlas(font* fnt);
		font* load_font(const unsigned char* data, unsigned int data_size, unsigned int size, unsigned int range0, unsigned int range1, bool is_sdf, int sdf_padding, int sdf_edge, float sdf_distance);

	private:
		pod_vector<atlas*> _atlases;
		pod_vector<font*>  _fonts;
		atlas_cb		   _atlas_created_cb   = nullptr;
		atlas_cb		   _atlas_updated_cb   = nullptr;
		atlas_cb		   _atlas_destroyed_cb = nullptr;
		font*			   _icons_font		   = nullptr;
	};

}

#ifdef VEKT_IMPL

#ifndef VEKT_NO_STB_IMPL
#define STB_TRUETYPE_IMPLEMENTATION
#endif

#include "stb_truetype.h"

namespace vekt
{
	config_data config = {};

	////////////////////////////////////////////////////////////////////////////////
	// :: WIDGET IMPL
	////////////////////////////////////////////////////////////////////////////////

	void widget::add_child(widget* w)
	{
		_widget_data.children.push_back(w);
		w->_widget_data.parent = this;
	}

	void widget::remove_child(widget* w)
	{
		_widget_data.children.remove(w);
		w->_widget_data.parent = nullptr;
	}

	void widget::set_visible(bool is_visible, bool recursive)
	{
		if (is_visible)
			_widget_data.flags &= ~widget_flags::wf_invisible;
		else
			_widget_data.flags |= widget_flags::wf_invisible;

		if (recursive)
		{
			for (widget* w : _widget_data.children)
				w->set_visible(is_visible, recursive);
		}
	}

	void widget::pos_pass()
	{
		if (_widget_data.flags & widget_flags::wf_pos_x_absolute)
			_widget_data.final_pos.x = _widget_data.pos.x;
		else if (_widget_data.parent && _widget_data.flags & widget_flags::wf_pos_x_relative)
		{
			const float parent_width = _widget_data.parent->_widget_data.final_size.x - _widget_data.parent->_widget_data.margins.left - _widget_data.parent->_widget_data.margins.right;

			if (_widget_data.flags & widget_flags::wf_pos_anchor_x_end)
				_widget_data.final_pos.x = (_widget_data.parent->_widget_data.final_pos.x + _widget_data.parent->_widget_data.margins.left) + (parent_width * _widget_data.pos.x) - _widget_data.final_size.x;
			else if (_widget_data.flags & widget_flags::wf_pos_anchor_x_center)
				_widget_data.final_pos.x = (_widget_data.parent->_widget_data.final_pos.x + _widget_data.parent->_widget_data.margins.left) + (parent_width * _widget_data.pos.x) - _widget_data.final_size.x * 0.5f;
			else
				_widget_data.final_pos.x = (_widget_data.parent->_widget_data.final_pos.x + _widget_data.parent->_widget_data.margins.left) + (parent_width * _widget_data.pos.x);
		}

		if (_widget_data.flags & widget_flags::wf_pos_y_absolute)
			_widget_data.final_pos.y = _widget_data.pos.y;
		else if (_widget_data.parent && _widget_data.flags & widget_flags::wf_pos_y_relative)
		{
			const float parent_height = _widget_data.parent->_widget_data.final_size.y - _widget_data.parent->_widget_data.margins.top - _widget_data.parent->_widget_data.margins.bottom;

			if (_widget_data.flags & widget_flags::wf_pos_anchor_y_end)
				_widget_data.final_pos.y = (_widget_data.parent->_widget_data.final_pos.y + _widget_data.parent->_widget_data.margins.top) + (parent_height * _widget_data.pos.y) - _widget_data.final_size.y;
			else if (_widget_data.flags & widget_flags::wf_pos_anchor_y_center)
				_widget_data.final_pos.y = (_widget_data.parent->_widget_data.final_pos.y + _widget_data.parent->_widget_data.margins.top) + (parent_height * _widget_data.pos.y) - _widget_data.final_size.y * 0.5f;
			else
				_widget_data.final_pos.y = (_widget_data.parent->_widget_data.final_pos.y + _widget_data.parent->_widget_data.margins.top) + (parent_height * _widget_data.pos.y);
		}

		if (_widget_data.custom_pos_pass) _widget_data.custom_pos_pass(this);
	}

	void widget::pos_pass_children()
	{
		for (widget* w : _widget_data.children)
		{
			w->pos_pass();
			w->pos_pass_children();
			w->pos_pass_post();
		}
	}

	void widget::pos_pass_post()
	{
		if (_widget_data.child_positioning == child_positioning::row)
		{
			float child_x = _widget_data.final_pos.x + _widget_data.margins.left;

			for (widget* w : _widget_data.children)
			{
				w->_widget_data.final_pos.x = child_x;
				child_x += _widget_data.spacing + w->_widget_data.final_size.x;
			}
		}
		else if (_widget_data.child_positioning == child_positioning::column)
		{
			float child_y = _widget_data.final_pos.y + _widget_data.margins.top;

			for (widget* w : _widget_data.children)
			{
				w->_widget_data.final_pos.y = child_y;
				child_y += _widget_data.spacing + w->_widget_data.final_size.y;
			}
		}

		if (_widget_data.parent) _widget_data.final_pos += _widget_data.parent->_widget_data.scroll_offset;
	}

	void widget::size_pass()
	{

		if (_widget_gfx.type == gfx_type::text)
		{
			gfx_text& txt = get_gfx_text();
			if (txt._dirty)
			{
				txt._dirty				= false;
				_widget_data.final_size = builder::get_text_size(get_gfx_text());
			}

			return;
		}

		if (_widget_data.flags & widget_flags::wf_size_x_absolute)
			_widget_data.final_size.x = _widget_data.size.x;
		else if (_widget_data.parent && _widget_data.flags & widget_flags::wf_size_x_relative)
			_widget_data.final_size.x = (_widget_data.parent->_widget_data.final_size.x - _widget_data.parent->_widget_data.margins.left - _widget_data.parent->_widget_data.margins.right) * _widget_data.size.x;

		if (_widget_data.flags & widget_flags::wf_size_y_absolute)
			_widget_data.final_size.y = _widget_data.size.y;
		else if (_widget_data.parent && _widget_data.flags & widget_flags::wf_size_y_relative)
			_widget_data.final_size.y = (_widget_data.parent->_widget_data.final_size.y - _widget_data.parent->_widget_data.margins.top - _widget_data.parent->_widget_data.margins.bottom) * _widget_data.size.y;

		size_copy_check();

		if (_widget_data.custom_size_pass) _widget_data.custom_size_pass(this);
	}

	void widget::size_pass_children(pod_vector<widget*>& fill_x, pod_vector<widget*>& fill_y)
	{
		for (widget* w : _widget_data.children)
		{
			w->size_pass();
			w->size_pass_children(fill_x, fill_y);
			w->size_pass_post(fill_x, fill_y);
		}
	}

	void widget::size_pass_post(pod_vector<widget*>& fill_x, pod_vector<widget*>& fill_y)
	{
		/*
			Max/Total behaviour.
		*/
		if (_widget_data.flags & widget_flags::wf_size_x_max_children)
		{
			_widget_data.final_size.x = _widget_data.margins.left + _widget_data.margins.right;

			float max = 0.0f;

			for (widget* w : _widget_data.children)
				max = math::max(w->_widget_data.final_size.x, max);

			_widget_data.final_size.x += max;
		}
		else if (_widget_data.flags & widget_flags::wf_size_x_total_children)
		{
			_widget_data.final_size.x = _widget_data.margins.left + _widget_data.margins.right;
			for (widget* w : _widget_data.children)
				_widget_data.final_size.x += w->_widget_data.final_size.x + _widget_data.spacing;
			if (!_widget_data.children.empty()) _widget_data.final_size.x -= _widget_data.spacing;
		}

		if (_widget_data.flags & widget_flags::wf_size_y_max_children)
		{
			_widget_data.final_size.y = _widget_data.margins.top + _widget_data.margins.bottom;

			float max = 0.0f;

			for (widget* w : _widget_data.children)
				max = math::max(w->_widget_data.final_size.y, max);

			_widget_data.final_size.y += max;
		}
		else if (_widget_data.flags & widget_flags::wf_size_y_total_children)
		{
			_widget_data.final_size.y = _widget_data.margins.top + _widget_data.margins.bottom;
			for (widget* w : _widget_data.children)
				_widget_data.final_size.y += w->_widget_data.final_size.y + _widget_data.spacing;
			if (!_widget_data.children.empty()) _widget_data.final_size.y -= _widget_data.spacing;
		}

		size_copy_check();

		/*
			Expand/Fill behaviour.
		*/
		float non_fill_total_x = -_widget_data.spacing, non_fill_total_y = -_widget_data.spacing;

		fill_x.resize(0);
		fill_y.resize(0);

		for (widget* w : _widget_data.children)
		{
			if (w->_widget_data.flags & widget_flags::wf_size_x_fill) { fill_x.push_back(w); }
			else { non_fill_total_x += w->_widget_data.final_size.x + _widget_data.spacing; }

			if (w->_widget_data.flags & widget_flags::wf_size_y_fill) { fill_y.push_back(w); }
			else { non_fill_total_y += w->_widget_data.final_size.y + _widget_data.spacing; }
		}

		if (!fill_x.empty())
		{
			const float diff  = _widget_data.final_size.x - non_fill_total_x;
			const float share = diff / static_cast<float>(fill_x.size());
			for (widget* w : fill_x)
				w->_widget_data.final_size.x = share;
		}

		if (!fill_y.empty())
		{
			const float diff  = _widget_data.final_size.y - non_fill_total_y;
			const float share = diff / static_cast<float>(fill_y.size());
			for (widget* w : fill_y)
				w->_widget_data.final_size.y = share;
		}
	}

	void widget::size_copy_check()
	{
		if (_widget_data.flags & widget_flags::wf_size_x_copy_y)
			_widget_data.final_size.x = _widget_data.final_size.y * (_widget_data.flags & widget_flags::wf_size_x_relative ? _widget_data.size.x : 1.0f);
		else if (_widget_data.flags & widget_flags::wf_size_y_copy_x)
			_widget_data.final_size.y = _widget_data.final_size.x * (_widget_data.flags & widget_flags::wf_size_y_relative ? _widget_data.size.y : 1.0f);
	}

	bool widget::draw_pass_clip_check(builder& builder)
	{
		if (_widget_gfx.type == gfx_type::filled_rect && get_gfx_filled_rect().clip_children) { return builder.push_to_clip_stack(get_clip_rect()); }
		else if (_widget_gfx.type == gfx_type::stroke_rect && get_gfx_stroke_rect().clip_children) { return builder.push_to_clip_stack(get_clip_rect()); }
		return false;
	}

	void widget::draw_pass(builder& builder)
	{
		if (_widget_gfx.type == gfx_type::filled_rect)
		{
			builder.add_filled_rect(_widget_gfx.get_data<gfx_filled_rect>(), _widget_data.final_pos, _widget_data.final_pos + _widget_data.final_size, _widget_gfx.draw_order, _widget_gfx.user_data, _is_hovered, _press_states[0]);
		}
		else if (_widget_gfx.type == gfx_type::stroke_rect)
		{
			builder.add_stroke_rect(_widget_gfx.get_data<gfx_stroke_rect>(), _widget_data.final_pos, _widget_data.final_pos + _widget_data.final_size, _widget_gfx.draw_order, _widget_gfx.user_data, _is_hovered, _press_states[0]);
		}
		else if (_widget_gfx.type == gfx_type::text) { builder.add_text(_widget_gfx.get_data<gfx_text>(), _widget_data.final_pos, _widget_data.final_size, _widget_gfx.draw_order, _widget_gfx.user_data, _is_hovered, _press_states[0]); }
	}

	void widget::draw_pass_children(builder& builder)
	{
		const bool clip_children = (_widget_gfx.type == gfx_type::filled_rect && get_gfx_filled_rect().clip_children) || _widget_gfx.type == gfx_type::stroke_rect && get_gfx_stroke_rect().clip_children;
		if (clip_children) builder.push_to_clip_stack_if_intersects(get_clip_rect());

		for (widget* w : _widget_data.children)
		{
			// For this child, whether it clips its own children or not, we check if it intersects into currect clip rect and draw only if so.
			const vec4 intersection = builder.calculate_intersection(builder.get_current_clip(), w->get_clip_rect());
			if (intersection.z <= 0 || intersection.w <= 0) continue;

			w->draw_pass(builder);
			w->draw_pass_children(builder);
		}

		if (clip_children) builder.pop_clip_stack();
	}

	void widget::draw_pass_clip_check_end(builder& builder)
	{
		if (_widget_gfx.type == gfx_type::filled_rect && get_gfx_filled_rect().clip_children || (_widget_gfx.type == gfx_type::stroke_rect && get_gfx_stroke_rect().clip_children)) { builder.pop_clip_stack(); }
	}

	////////////////////////////////////////////////////////////////////////////////
	// :: BUILDER IMPL
	////////////////////////////////////////////////////////////////////////////////

	void builder::init(const init_config& conf)
	{
		ASSERT(conf.vertex_buffer_sz > 0 && conf.index_buffer_sz > 0 && conf.vertex_buffer_sz > 0 && conf.widget_buffer_sz > 0);
		ASSERT(conf.buffer_count * sizeof(vertex) < conf.vertex_buffer_sz && conf.buffer_count * sizeof(index) < conf.index_buffer_sz);

		const unsigned int widget_count = static_cast<unsigned int>(conf.widget_buffer_sz / sizeof(widget));
		_widget_pool.init(widget_count);

		const size_t vertex_count = conf.vertex_buffer_sz / sizeof(vertex);
		const size_t index_count  = conf.index_buffer_sz / sizeof(index);
		_vertex_count_per_buffer  = static_cast<unsigned int>(vertex_count / conf.buffer_count);
		_index_count_per_buffer	  = static_cast<unsigned int>(index_count / conf.buffer_count);
		_buffer_count			  = conf.buffer_count;

		_vertex_buffer = reinterpret_cast<vertex*>(MALLOC(sizeof(vertex) * vertex_count));
		_index_buffer  = reinterpret_cast<index*>(MALLOC(sizeof(index) * index_count));

		for (size_t i = 0; i < vertex_count; i++)
			new (&_vertex_buffer[i]) vertex();
		for (size_t i = 0; i < index_count; i++)
			new (&_index_buffer[i]) index();
	}

	void builder::uninit()
	{
		_widget_pool.clear();

		if (_vertex_buffer) FREE(_vertex_buffer);
		if (_index_buffer) FREE(_index_buffer);

		_vertex_buffer = nullptr;
		_index_buffer  = nullptr;
	}

	void builder::build(const vec2& screen_size)
	{
		ASSERT(_root);

		_draw_buffers.resize(0);
		_clip_stack.resize(0);
		_buffer_counter = 0;

		/* size & pos & draw */
		_reuse_fill_x.resize(0);
		_reuse_fill_y.resize(0);
		builder& bd					   = *this;
		_root->_widget_data.pos		   = {0.0f, 0.0f};
		_root->_widget_data.size	   = screen_size;
		_root->get_data_widget().flags = widget_flags::wf_pos_x_absolute | widget_flags::wf_pos_y_absolute | widget_flags::wf_size_x_absolute | widget_flags::wf_size_y_absolute;
		_root->size_pass();
		_root->size_pass_children(_reuse_fill_x, _reuse_fill_y);
		_root->size_pass_post(_reuse_fill_x, _reuse_fill_y);
		_root->pos_pass();
		_root->pos_pass_children();
		_root->pos_pass_post();

		_root->draw_pass(bd);

		_clip_stack.push_back({0.0f, 0.0f, screen_size.x, screen_size.y});
		_root->draw_pass_children(bd);
		_clip_stack.remove(_clip_stack.size() - 1);
	}

	void builder::flush()
	{
		if (!_on_draw) return;
		std::sort(_draw_buffers.begin(), _draw_buffers.end(), [](const draw_buffer& a, const draw_buffer& b) { return a.draw_order < b.draw_order; });

		for (draw_buffer& db : _draw_buffers)
			_on_draw(db);
	}

	void builder::on_mouse_move(const vec2& mouse)
	{
		const vec2 delta = mouse - _mouse_position;
		_mouse_position	 = mouse;

		for (widget* w : _press_state_history)
		{
			if (w->get_data_widget().on_mouse_dragged) w->get_data_widget().on_mouse_dragged(w, mouse, delta);
		}

		pass_hover_state(_root, mouse);
	}

	input_event_result builder::on_mouse_event(const mouse_event& ev)
	{
		ASSERT(_root);

		if (_input_layers.empty())
		{
			V_ERR("vekt::builder::on_mouse_event -> No input layers are added to the builder!");
			return input_event_result::not_handled;
		}

		widget* last_widget = nullptr;
		for (const input_layer& layer : _input_layers)
		{
			if (ev.type == input_event_type::pressed)
			{
				widget* w = find_widget_at(layer.root, _mouse_position);
				if (w)
				{
					w->_press_states[ev.button] = true;
					_press_state_history.push_back(w);
				}
			}

			if (ev.type == input_event_type::released)
			{
				widget* w = find_widget_at(layer.root, _mouse_position);
				if (w)
				{

					if (w->_press_states[ev.button] && w->get_data_widget().on_mouse_clicked) { w->get_data_widget().on_mouse_clicked(w, ev); }

					w->_press_states[ev.button] = false;
				}

				for (widget* w : _press_state_history)
					w->_press_states[ev.button] = false;
				_press_state_history.resize(0);
			}

			input_event_result res = layer.root->on_event_internal(ev, last_widget);
			if (res == input_event_result::handled) { return res; }
		}

		return input_event_result::not_handled;
	}

	input_event_result builder::on_mouse_wheel_event(const mouse_wheel_event& ev)
	{
		ASSERT(_root);

		if (_input_layers.empty())
		{
			V_ERR("vekt::builder::on_mouse_wheel_event -> No input layers are added to the builder!");
			return input_event_result::not_handled;
		}

		widget* last_widget = nullptr;
		for (const input_layer& layer : _input_layers)
		{
			input_event_result res = layer.root->on_event_internal(ev, last_widget);
			if (res == input_event_result::handled) return res;
		}

		return input_event_result::not_handled;
	}

	input_event_result builder::on_key_event(const key_event& ev)
	{
		ASSERT(_root);

		if (_input_layers.empty())
		{
			V_ERR("vekt::builder::on_key_event -> No input layers are added to the builder!");
			return input_event_result::not_handled;
		}

		widget* last_widget = nullptr;
		for (const input_layer& layer : _input_layers)
		{
			input_event_result res = layer.root->on_event_internal(ev, last_widget);
			if (res == input_event_result::handled) return res;
		}

		return input_event_result::not_handled;
	}

	void builder::add_input_layer(unsigned int priority, widget* root)
	{
		for (input_layer& layer : _input_layers)
		{
			if (layer.priority == priority)
			{
				V_WARN("vekt::builder::add_input_layer -> Input layer with this priority already exists, overriding it's root! priority: %d", priority);
				layer.root = root;
				return;
			}
		}

		_input_layers.push_back({.priority = priority, .root = root});

		std::sort(_input_layers.begin(), _input_layers.end(), [](const input_layer& layer0, const input_layer& layer1) -> bool { return layer0.priority < layer1.priority; });
	}

	void builder::remove_input_layer(unsigned int priority)
	{
		for (input_layer& layer : _input_layers)
		{
			if (layer.priority == priority)
			{
				_input_layers.remove(layer);
				std::sort(_input_layers.begin(), _input_layers.end(), [](const input_layer& layer0, const input_layer& layer1) -> bool { return layer0.priority < layer1.priority; });
				return;
			}
		}
		V_ERR("vekt::remove_input_layer -> No input layer with the given priority exists! priority: %d", priority);
	}

	widget* builder::find_widget_at(widget* current_widget, const vec2& mouse)
	{
		if (!current_widget->get_is_visible()) { return nullptr; }

		const vec4 bounds = current_widget->get_clip_rect();
		if (!bounds.is_point_inside(mouse)) return nullptr;

		widget* found = nullptr;

		for (widget* w : current_widget->get_data_widget().children)
		{
			found = find_widget_at(w, mouse);
			if (found) return found;
		}

		return current_widget->get_data_widget().receive_input ? current_widget : nullptr;
	}

	void builder::pass_hover_state(widget* w, const vec2& mouse)
	{
		const vec4 clip	   = w->get_clip_rect();
		const bool hovered = clip.is_point_inside(mouse);

		if (w->get_data_widget().on_hover_end && w->_is_hovered && !hovered) w->get_data_widget().on_hover_end(w);
		if (w->get_data_widget().on_hover_begin && !w->_is_hovered && hovered) w->get_data_widget().on_hover_begin(w);
		w->_is_hovered = hovered;

		for (widget* c : w->get_data_widget().children)
			pass_hover_state(c, mouse);
	}

	void builder::add_filled_rect(const gfx_filled_rect& rect, const vec2& min, const vec2& max, unsigned int draw_order, void* user_data, bool use_hovered, bool use_pressed)
	{
		draw_buffer* db = get_draw_buffer(draw_order, user_data);

		const vec4 color_start = (use_pressed && rect.pressed_color.w > 0.001f) ? rect.pressed_color : ((use_hovered && rect.hovered_color.w > 0.001f) ? rect.hovered_color : rect.color_start);
		const vec4 color_end   = (use_pressed && rect.pressed_color.w > 0.001f) ? rect.pressed_color : ((use_hovered && rect.hovered_color.w > 0.001f) ? rect.hovered_color : rect.color_end);

		_reuse_outer_path.resize(0);
		_reuse_outline_path.resize(0);

		const bool has_aa		= rect.aa_thickness > 0;
		const bool has_rounding = rect.rounding > 0.0f;
		const bool has_outline	= rect.outline_thickness > 0;

		if (has_rounding)
			generate_rounded_rect(_reuse_outer_path, min, max, rect.rounding, rect.segments);
		else
			generate_sharp_rect(_reuse_outer_path, min, max);

		if (has_outline) { generate_offset_rect(_reuse_outline_path, _reuse_outer_path, -static_cast<float>(rect.outline_thickness)); }

		_reuse_aa_outer_path.resize(0);

		if (has_aa)
		{
			if (has_outline)
				generate_offset_rect(_reuse_aa_outer_path, _reuse_outline_path, -static_cast<float>(rect.aa_thickness));
			else
				generate_offset_rect(_reuse_aa_outer_path, _reuse_outer_path, -static_cast<float>(rect.aa_thickness));
		}

		const unsigned int out_start = db->vertex_count;
		add_vertices(db, _reuse_outer_path, color_start, color_end, rect.color_direction, min, max);

		if (has_rounding)
		{
			const unsigned int central_start = db->vertex_count;
			add_central_vertex(db, color_start, color_end, min, max);
			add_filled_rect_central(db, out_start, central_start, _reuse_outer_path.size());
		}
		else
			add_filled_rect(db, out_start, _reuse_outer_path.size());

		unsigned int outline_start = 0;

		if (has_outline)
		{
			// add original vertices
			const unsigned int copy_start = db->vertex_count;
			add_vertices(db, _reuse_outer_path, rect.outline_color, rect.outline_color, direction::horizontal, min, max);

			outline_start = db->vertex_count;
			add_vertices(db, _reuse_outline_path, rect.outline_color, rect.outline_color, vekt::direction::horizontal, min, max);
			add_strip(db, outline_start, copy_start, _reuse_outline_path.size(), false);
		}

		if (has_aa)
		{
			const unsigned int out_aa_start = db->vertex_count;
			add_vertices_aa(db, _reuse_aa_outer_path, out_start, 0.0f, min, max);

			if (has_outline)
				add_strip(db, out_aa_start, outline_start, _reuse_aa_outer_path.size(), false);
			else
				add_strip(db, out_aa_start, out_start, _reuse_aa_outer_path.size(), false);
		}
	}

	void builder::add_stroke_rect(const gfx_stroke_rect& rect, const vec2& min, const vec2& max, unsigned int draw_order, void* user_data, bool use_hovered, bool use_pressed)
	{
		draw_buffer* db			 = get_draw_buffer(draw_order, user_data);
		const vec4	 color_start = (use_pressed && rect.pressed_color.w > 0.001f) ? rect.pressed_color : ((use_hovered && rect.hovered_color.w > 0.001f) ? rect.hovered_color : rect.color_start);
		const vec4	 color_end	 = (use_pressed && rect.pressed_color.w > 0.001f) ? rect.pressed_color : ((use_hovered && rect.hovered_color.w > 0.001f) ? rect.hovered_color : rect.color_end);
		_reuse_outer_path.resize(0);
		_reuse_inner_path.resize(0);

		const bool has_stroke	= rect.thickness > 0;
		const bool has_aa		= rect.aa_thickness > 0;
		const bool has_rounding = rect.rounding > 0.0f;

		if (has_rounding)
		{
			generate_rounded_rect(_reuse_outer_path, min, max, rect.rounding, rect.segments);
			generate_rounded_rect(_reuse_inner_path, min + vec2(rect.thickness, rect.thickness), max - vec2(rect.thickness, rect.thickness), rect.rounding, rect.segments);
		}
		else
		{
			generate_sharp_rect(_reuse_outer_path, min, max);
			generate_offset_rect(_reuse_inner_path, _reuse_outer_path, static_cast<float>(rect.thickness));
		}

		_reuse_aa_outer_path.resize(0);
		_reuse_aa_inner_path.resize(0);

		if (has_aa)
		{
			generate_offset_rect(_reuse_aa_outer_path, _reuse_outer_path, -static_cast<float>(rect.aa_thickness));
			if (has_stroke && !_reuse_inner_path.empty()) { generate_offset_rect(_reuse_aa_inner_path, _reuse_inner_path, static_cast<float>(rect.aa_thickness)); }
		}

		// Original stroke
		const unsigned int out_start = db->vertex_count;
		add_vertices(db, _reuse_outer_path, color_start, color_end, rect.color_direction, min, max);
		const unsigned int in_start = db->vertex_count;
		add_vertices(db, _reuse_inner_path, color_start, color_end, rect.color_direction, min, max);
		add_strip(db, out_start, in_start, _reuse_outer_path.size(), false);

		if (has_aa)
		{
			// outer aa
			const unsigned int out_aa_start = db->vertex_count;
			add_vertices_aa(db, _reuse_aa_outer_path, out_start, 0.0f, min, max);
			add_strip(db, out_aa_start, out_start, _reuse_aa_outer_path.size(), false);

			//// inner aa
			const unsigned int in_aa_start = db->vertex_count;
			add_vertices_aa(db, _reuse_aa_inner_path, in_start, 0.0f, min, max);
			add_strip(db, in_start, in_aa_start, _reuse_aa_inner_path.size(), false);
		}
	}

	void builder::add_text(const gfx_text& text, const vec2& position, const vec2& size, unsigned int draw_order, void* user_data, bool use_hovered, bool use_pressed)
	{
		if (text.target_font == nullptr)
		{
			V_ERR("vekt::builder::add_text() -> No font is set!");
			return;
		}

		const vec4 color_start = (use_pressed && text.pressed_color.w > 0.001f) ? text.pressed_color : ((use_hovered && text.hovered_color.w > 0.001f) ? text.hovered_color : text.color_start);
		const vec4 color_end   = (use_pressed && text.pressed_color.w > 0.001f) ? text.pressed_color : ((use_hovered && text.hovered_color.w > 0.001f) ? text.hovered_color : text.color_end);

		draw_buffer* db			 = get_draw_buffer(draw_order, user_data, text.target_font);
		const float	 pixel_scale = text.target_font->_scale;

		const unsigned int start_vertices_idx = db->vertex_count;
		const unsigned int start_indices_idx  = db->index_count;
		const unsigned int char_count		  = static_cast<unsigned int>(text.text.size());

		unsigned int vtx_counter = 0;
		unsigned int idx_counter = 0;

		vec2	   pen = position;
		const vec2 end = pen + size;

		auto draw_char = [&](const glyph& g, unsigned long c, unsigned long previous_char) {
			if (previous_char != 0)
			{
				const float kern_amt = static_cast<float>(text.target_font->glyph_info[previous_char].kern_advance[c]) * pixel_scale;
				pen.x += kern_amt;
			}

			const float quad_left	= pen.x + g.x_offset;
			const float quad_top	= pen.y + g.y_offset;
			const float quad_right	= quad_left + g.width;
			const float quad_bottom = quad_top + g.height;

			vertex& v0 = db->add_get_vertex();
			vertex& v1 = db->add_get_vertex();
			vertex& v2 = db->add_get_vertex();
			vertex& v3 = db->add_get_vertex();

			v0.pos = {quad_left, quad_top};
			v1.pos = {quad_right, quad_top};
			v2.pos = {quad_right, quad_bottom};
			v3.pos = {quad_left, quad_bottom};

			auto set_col = [&](vertex& vtx) {
				const float x0 = math::remap(vtx.pos.x, position.x, end.x, 0.0f, 1.0f);
				const float y0 = math::remap(vtx.pos.y, position.y, end.y, 0.0f, 1.0f);
				vtx.color	   = text.color_direction == direction::horizontal ? vec4::lerp(color_start, color_end, x0) : vec4::lerp(color_start, color_end, y0);
			};

			set_col(v0);
			set_col(v1);
			set_col(v2);
			set_col(v3);

			v0.uv = vec2(g.uv_x, g.uv_y);
			v1.uv = v0.uv + vec2(g.uv_w, 0.0f);
			v2.uv = v1.uv + vec2(0.0f, g.uv_h);
			v3.uv = v0.uv + vec2(0.0f, g.uv_h);

			db->add_index(start_vertices_idx + vtx_counter);
			db->add_index(start_vertices_idx + vtx_counter + 1);
			db->add_index(start_vertices_idx + vtx_counter + 3);

			db->add_index(start_vertices_idx + vtx_counter + 1);
			db->add_index(start_vertices_idx + vtx_counter + 2);
			db->add_index(start_vertices_idx + vtx_counter + 3);

			vtx_counter += 4;
			idx_counter += 6;

			pen.x += g.advance_x * pixel_scale + static_cast<float>(text.spacing);
		};

		const uint8_t* c;
		float		   max_y_offset = 0;
		for (c = (uint8_t*)text.text.c_str(); *c; c++)
		{
			auto		 character = *c;
			const glyph& ch		   = text.target_font->glyph_info[character];
			max_y_offset		   = math::max(max_y_offset, -ch.y_offset);
		}

		pen.y += max_y_offset;

		unsigned long previous_char = 0;
		for (c = (uint8_t*)text.text.c_str(); *c; c++)
		{
			auto		 character = *c;
			const glyph& ch		   = text.target_font->glyph_info[character];
			draw_char(ch, character, previous_char);
			previous_char = character;
		}
	}

	vec2 builder::get_text_size(const gfx_text& text)
	{
		if (text.target_font == nullptr)
		{
			V_ERR("vekt::builder::get_text_size() -> No font is set!");
			return vec2();
		}

		const font* fnt	  = text.target_font;
		const float scale = fnt->_scale;

		float total_x = 0.0f;
		float max_y	  = 0.0f;

		const char* str = text.text.c_str();
		for (size_t i = 0; str[i]; ++i)
		{
			const uint8_t c0 = static_cast<uint8_t>(str[i]);
			const glyph&  g0 = fnt->glyph_info[c0];

			total_x += g0.advance_x * scale;

			if (str[i + 1])
			{
				const uint8_t c1 = static_cast<uint8_t>(str[i + 1]);
				total_x += g0.kern_advance[c1] * scale;
			}

			total_x += static_cast<float>(text.spacing);
			max_y = math::max(max_y, static_cast<float>(g0.height));
		}

		return vec2(total_x - text.spacing, max_y); // remove last spacing
	}

	void builder::add_strip(draw_buffer* db, unsigned int outer_start, unsigned int inner_start, unsigned int size, bool add_ccw)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			const unsigned int p1_curr = outer_start + i;
			const unsigned int p1_next = outer_start + (i + 1) % size;
			const unsigned int p2_curr = inner_start + i;
			const unsigned int p2_next = inner_start + (i + 1) % size;
			db->add_index(p1_curr);

			if (add_ccw)
			{
				db->add_index(p2_curr);
				db->add_index(p1_next);
			}
			else
			{
				db->add_index(p1_next);
				db->add_index(p2_curr);
			}

			db->add_index(p1_next);
			if (add_ccw)
			{
				db->add_index(p2_curr);
				db->add_index(p2_next);
			}
			else
			{
				db->add_index(p2_next);
				db->add_index(p2_curr);
			}
		}
	}

	void builder::add_filled_rect(draw_buffer* db, unsigned int start, unsigned int size)
	{
		if (size != 4) return;

		db->add_index(start);
		db->add_index(start + 1);
		db->add_index(start + 3);
		db->add_index(start + 1);
		db->add_index(start + 2);
		db->add_index(start + 3);
	}

	void builder::add_filled_rect_central(draw_buffer* db, unsigned int start, unsigned int central_start, unsigned int size)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			db->add_index(central_start);
			db->add_index(start + i);
			db->add_index(start + ((i + 1) % (size)));
		}
	}

	void builder::add_vertices_aa(draw_buffer* db, const pod_vector<vec2>& path, unsigned int original_vertices_idx, float alpha, const vec2& min, const vec2& max)
	{
		const unsigned int start_vtx_idx = db->vertex_count;

		for (unsigned int i = 0; i < path.size(); i++)
		{
			vertex& vtx = db->add_get_vertex();
			vtx.pos		= path[i];
			vtx.color	= db->vertex_start[original_vertices_idx + i].color;
			vtx.color.w = alpha;
			vtx.uv.x	= math::remap(vtx.pos.x, min.x, max.x, 0.0f, 1.0f);
			vtx.uv.y	= math::remap(vtx.pos.y, min.y, max.y, 0.0f, 1.0f);
		}
	}

	namespace
	{
		float calculate_signed_area(const pod_vector<vec2>& path)
		{
			float area = 0.0f;
			for (size_t i = 0; i < path.size(); ++i)
			{
				const vec2& p1 = path[i];
				const vec2& p2 = path[(i + 1) % path.size()];
				area += (p1.x * p2.y - p2.x * p1.y);
			}
			return area * 0.5f;
		}
	}

	void builder::generate_offset_rect(pod_vector<vec2>& out_path, const pod_vector<vec2>& base_path, float distance)
	{
		if (base_path.size() < 2) return;
		out_path.resize(base_path.size());

		const size_t num_points = base_path.size();

		for (size_t i = 0; i < num_points; ++i)
		{
			// Get the current, previous, and next points
			const vec2& p_curr = base_path[i];
			const vec2& p_prev = base_path[(i + num_points - 1) % num_points];
			const vec2& p_next = base_path[(i + 1) % num_points];

			const vec2 tangent1		= (p_curr - p_prev).normalized();
			const vec2 tangent2		= (p_next - p_curr).normalized();
			const vec2 normal1		= {-tangent1.y, tangent1.x};
			const vec2 normal2		= {-tangent2.y, tangent2.x};
			const vec2 miter_vector = (normal1 + normal2).normalized();

			// Calculate the offset vertex
			const vec2 path = p_curr + miter_vector * distance;
			out_path[i]		= path;
		}
	}

	void builder::generate_offset_rounded_rect(pod_vector<vec2>& out_path, const pod_vector<vec2>& base_path, float amount) {}

	void builder::add_vertices(draw_buffer* db, const pod_vector<vec2>& path, const vec4& color_start, const vec4& color_end, direction direction, const vec2& min, const vec2& max)
	{
		const unsigned int start_vtx_idx = db->vertex_count;

		for (unsigned int i = 0; i < path.size(); i++)
		{
			vertex& vtx = db->add_get_vertex();
			vtx.pos		= path[i];

			const float ratio = direction == direction::horizontal ? math::remap(vtx.pos.x, min.x, max.x, 0.0f, 1.0f) : math::remap(vtx.pos.y, min.y, max.y, 0.0f, 1.0f);
			vtx.color.x		  = math::lerp(color_start.x, color_end.x, ratio);
			vtx.color.y		  = math::lerp(color_start.y, color_end.y, ratio);
			vtx.color.z		  = math::lerp(color_start.z, color_end.z, ratio);
			vtx.color.w		  = math::lerp(color_start.w, color_end.w, ratio);
			vtx.uv.x		  = math::remap(vtx.pos.x, min.x, max.x, 0.0f, 1.0f);
			vtx.uv.y		  = math::remap(vtx.pos.y, min.y, max.y, 0.0f, 1.0f);
		}
	}

	void builder::add_central_vertex(draw_buffer* db, const vec4& color_start, const vec4& color_end, const vec2& min, const vec2& max)
	{
		vertex& vtx = db->add_get_vertex();
		vtx.pos		= (min + max) * 0.5f;
		vtx.color	= (color_start + color_end) * 0.5f;
		vtx.uv		= vec2(0.5f, 0.5f);
	}

	void builder::generate_rounded_rect(pod_vector<vec2>& out_path, const vec2& min, const vec2& max, float r, int segments)
	{
		r = math::min(r, math::min((max.x - min.x) * 0.5f, (max.y - min.y) * 0.5f)); // Clamp radius

		if (segments == 0) segments = 10;

		segments = math::min(math::max(1, segments), 90);

		// top left
		{
			const vec2 center = vec2(min.x + r, min.y + r);
			for (int i = 0; i <= segments; ++i)
			{

				const float target_angle  = DEG_2_RAD * (270.0f + (90.0f / segments) * i);
				const vec2	point_on_unit = vec2(math::sin(target_angle), -math::cos(target_angle)) * r;
				const vec2	point		  = center + point_on_unit;
				out_path.push_back(point);
			}
		}
		// top right
		{
			const vec2 center = vec2(max.x - r, min.y + r);
			for (int i = 0; i <= segments; ++i)
			{

				const float target_angle  = DEG_2_RAD * ((90.0f / segments) * i);
				const vec2	point_on_unit = vec2(math::sin(target_angle), -math::cos(target_angle)) * r;
				const vec2	point		  = center + point_on_unit;
				out_path.push_back(point);
			}
		}

		// bottom right
		{
			const vec2 center = vec2(max.x - r, max.y - r);
			for (int i = 0; i <= segments; ++i)
			{

				const float target_angle  = DEG_2_RAD * (90.0f + (90.0f / segments) * i);
				const vec2	point_on_unit = vec2(math::sin(target_angle), -math::cos(target_angle)) * r;
				const vec2	point		  = center + point_on_unit;
				out_path.push_back(point);
			}
		}

		// bottom left
		{
			const vec2 center = vec2(min.x + r, max.y - r);
			for (int i = 0; i <= segments; ++i)
			{

				const float target_angle  = DEG_2_RAD * (180.0f + (90.0f / segments) * i);
				const vec2	point_on_unit = vec2(math::sin(target_angle), -math::cos(target_angle)) * r;
				const vec2	point		  = center + point_on_unit;
				out_path.push_back(point);
			}
		}
	}
	void builder::generate_sharp_rect(pod_vector<vec2>& out_path, const vec2& min, const vec2& max)
	{
		out_path.push_back({min.x, min.y}); // Top-left
		out_path.push_back({max.x, min.y}); // Top-right
		out_path.push_back({max.x, max.y}); // Bottom-right
		out_path.push_back({min.x, max.y}); // Bottom-left
	}

	draw_buffer* builder::get_draw_buffer(unsigned int draw_order, void* user_data, font* fnt)
	{
		const vec4& clip = get_current_clip();

		for (draw_buffer& db : _draw_buffers)
		{
			if (db.clip.equals(clip) && db.draw_order == draw_order && db.user_data == user_data && db.used_font == fnt) { return &db; }
		}

		ASSERT(_buffer_counter < _buffer_count);

		draw_buffer db	 = {};
		db.clip			 = clip;
		db.draw_order	 = draw_order;
		db.user_data	 = user_data;
		db.vertex_start	 = _vertex_buffer + _buffer_counter * _vertex_count_per_buffer;
		db.index_start	 = _index_buffer + _buffer_counter * _index_count_per_buffer;
		db.used_font	 = fnt;
		db._max_vertices = _vertex_count_per_buffer;
		db._max_indices	 = _index_count_per_buffer;

		_buffer_counter++;
		_draw_buffers.push_back(db);
		return &_draw_buffers.get_back();
	}

	bool builder::push_to_clip_stack(const vec4& rect)
	{
		vec4	   parent_clip	  = _clip_stack.empty() ? vec4() : _clip_stack[_clip_stack.size() - 1];
		const vec4 effective_clip = calculate_intersection(parent_clip, rect);
		_clip_stack.push_back(effective_clip);
		return effective_clip.z <= 0 || effective_clip.w <= 0;
	}

	bool builder::push_to_clip_stack_if_intersects(const vec4& rect)
	{
		vec4	   parent_clip	  = _clip_stack.empty() ? vec4() : _clip_stack[_clip_stack.size() - 1];
		const vec4 effective_clip = calculate_intersection(parent_clip, rect);
		if (effective_clip.z <= 0 || effective_clip.w <= 0) return false;
		_clip_stack.push_back(effective_clip);
		return true;
	}

	void builder::pop_clip_stack()
	{
		if (_clip_stack.size() > 1) _clip_stack.remove(_clip_stack.size() - 1);
	}

	vec4 builder::calculate_intersection(const vec4& r1, const vec4& r2) const
	{
		float x		 = math::max(r1.x, r2.x);
		float y		 = math::max(r1.y, r2.y);
		float right	 = math::min(r1.x + r1.z, r2.x + r2.z);
		float bottom = math::min(r1.y + r1.w, r2.y + r2.w);

		if (right < x || bottom < y) { return vec4(); }
		return {x, y, right - x, bottom - y};
	}

	widget* builder::widget_horizontal_divider(float height, const vec4& color)
	{
		widget* w = allocate();
		w->set_pos_x(0.0f, helper_pos_type::relative);
		w->set_width(1.0f, helper_size_type::relative);
		w->set_height(height, helper_size_type::absolute);
		w->get_data_widget().debug_name = "Horizontal Divider";
		gfx_filled_rect& rect			= w->get_gfx_filled_rect();
		rect.color_start = rect.color_end = color;
		return w;
	}

	widget* builder::widget_vertical_divider(float width, const vec4& color)
	{
		widget* w = allocate();
		w->set_pos_y(0.0f, helper_pos_type::relative);
		w->set_height(1.0f, helper_size_type::relative);
		w->set_width(width, helper_size_type::absolute);
		w->get_data_widget().debug_name = "Vertical Divider";

		gfx_filled_rect& rect = w->get_gfx_filled_rect();
		rect.color_start = rect.color_end = color;
		return w;
	}

	widget* builder::widget_button(float height, font* fnt, const VEKT_STRING& text, const vec4& color, const vec4& text_color)
	{
		widget* button = allocate();
		button->set_height(height, helper_size_type::absolute);
		button->set_width(1.0f, helper_size_type::relative);
		button->set_pos_x(0.0f, helper_pos_type::relative);
		button->get_data_widget().debug_name	= "Button";
		button->get_data_widget().receive_input = true;

		gfx_filled_rect& rect = button->get_gfx_filled_rect();
		rect.color_start = rect.color_end = color;

		widget* txt = allocate();
		txt->set_pos_x(0.5f, helper_pos_type::relative, helper_anchor_type::center);
		txt->set_pos_y(0.5f, helper_pos_type::relative, helper_anchor_type::center);

		gfx_text& gfx_text	 = txt->get_gfx_text();
		gfx_text.color_start = gfx_text.color_end = text_color;
		gfx_text.set_font(fnt);
		gfx_text.text = text;

		button->add_child(txt);
		return button;
	}

	widget* builder::widget_checkbox(float height, void* sdf_material, const vec4& color, const vec4& check_color)
	{
		widget* box = allocate();
		box->set_width(1.0f, helper_size_type::copy_other);
		box->set_height(height, helper_size_type::absolute);
		box->set_pos_x(0.0f);
		box->get_data_widget().debug_name	 = "Checkbox";
		box->get_data_widget().receive_input = true;

		gfx_filled_rect& rect = box->get_gfx_filled_rect();
		rect.color_start = rect.color_end = color;

		widget* txt = allocate();
		txt->set_pos_x(0.5f, helper_pos_type::relative, helper_anchor_type::center);
		txt->set_pos_y(0.5f, helper_pos_type::relative, helper_anchor_type::center);
		gfx_text& gfx_text	 = txt->get_gfx_text();
		gfx_text.color_start = gfx_text.color_end = check_color;
		gfx_text.set_font(font_manager::get().get_icons_font());
		gfx_text.text				  = "\u0024";
		txt->get_gfx_data().user_data = sdf_material;
		box->add_child(txt);

		return box;
	}

	////////////////////////////////////////////////////////////////////////////////
	// :: ATLAS IMPL
	////////////////////////////////////////////////////////////////////////////////

	atlas::atlas(unsigned int width, unsigned int height)
	{
		_width	= width;
		_height = height;
		_available_slices.push_back(new atlas::slice(0, _height));
		const size_t sz = static_cast<size_t>(width * height);
		_data			= reinterpret_cast<unsigned char*>(MALLOC(sz));
		memset(_data, 0, sz);
		_data_size = width * height;
	}

	atlas::~atlas()
	{
		for (slice* slc : _available_slices)
			delete slc;
		_available_slices.clear();

		FREE(_data);
	}

	bool atlas::add_font(font* font)
	{
		if (font->_atlas_required_height > _height) return false;

		unsigned int best_slice_diff = _height;
		slice*		 best_slice		 = nullptr;

		for (slice* slc : _available_slices)
		{
			if (slc->height < font->_atlas_required_height) continue;

			const unsigned int diff = slc->height - font->_atlas_required_height;
			if (diff < best_slice_diff)
			{
				best_slice_diff = diff;
				best_slice		= slc;
			}
		}

		if (best_slice == nullptr) return false;

		font->_atlas	 = this;
		font->_atlas_pos = best_slice->pos;
		_fonts.push_back(font);

		best_slice->pos += font->_atlas_required_height;
		best_slice->height -= font->_atlas_required_height;

		if (best_slice->height == 0)
		{
			_available_slices.remove(best_slice);
			delete best_slice;
		}

		return true;
	}

	void atlas::remove_font(font* fnt)
	{
		slice* slc = new slice(fnt->_atlas_pos, fnt->_atlas_pos);
		_available_slices.push_back(slc);
		_fonts.remove(fnt);
	}

	void font_manager::find_atlas(font* fnt)
	{
		for (atlas* atl : _atlases)
		{
			if (atl->add_font(fnt)) { return; }
		}

		atlas* atl = new atlas(config.atlas_width, config.atlas_height);
		_atlases.push_back(atl);
		if (_atlas_created_cb) _atlas_created_cb(atl);
		const bool ok = atl->add_font(fnt);
		ASSERT(ok);
	}

	font* font_manager::load_font(const unsigned char* data, unsigned int data_size, unsigned int size, unsigned int range0, unsigned int range1, bool is_sdf, int sdf_padding, int sdf_edge, float sdf_distance)
	{

		stbtt_fontinfo stb_font;
		stbtt_InitFont(&stb_font, data, stbtt_GetFontOffsetForIndex(data, 0));

		font* fnt	= new font();
		fnt->_scale = stbtt_ScaleForPixelHeight(&stb_font, static_cast<float>(size));

		stbtt_GetFontVMetrics(&stb_font, &fnt->ascent, &fnt->descent, &fnt->line_gap);
		fnt->size = size;

		int		  total_width = 0;
		int		  max_height  = 0;
		const int x_padding	  = 2;

		for (int i = range0; i < range1; i++)
		{
			glyph& glyph_info = fnt->glyph_info[i];

			if (is_sdf)
			{
				int x_off, y_off;
				glyph_info.sdf_data = stbtt_GetCodepointSDF(&stb_font, fnt->_scale, i, sdf_padding, sdf_edge, sdf_distance, &glyph_info.width, &glyph_info.height, &x_off, &y_off);
				glyph_info.x_offset = static_cast<float>(x_off);
				glyph_info.y_offset = static_cast<float>(y_off);
			}
			else
			{
				int ix0 = 0, iy0 = 0, ix1 = 0, iy1 = 0;
				stbtt_GetCodepointBitmapBox(&stb_font, i, fnt->_scale, fnt->_scale, &ix0, &iy0, &ix1, &iy1);
				glyph_info.width	= ix1 - ix0;
				glyph_info.height	= iy1 - iy0;
				glyph_info.x_offset = static_cast<float>(ix0);
				glyph_info.y_offset = static_cast<float>(iy0);
			}

			if (glyph_info.width >= 1) total_width += glyph_info.width + x_padding;
			max_height = static_cast<int>(math::max(max_height, glyph_info.height));
			stbtt_GetCodepointHMetrics(&stb_font, i, &glyph_info.advance_x, &glyph_info.left_bearing);

			for (int j = 0; j < 128; j++)
				glyph_info.kern_advance[j] = stbtt_GetCodepointKernAdvance(&stb_font, i, j);
		}

		const int required_rows		= static_cast<int>(math::ceilf(static_cast<float>(total_width) / static_cast<float>(config.atlas_width)));
		const int required_height	= max_height;
		fnt->_atlas_required_height = required_rows * required_height;
		find_atlas(fnt);

		if (fnt->_atlas == nullptr)
		{
			delete fnt;
			V_ERR("vekt::font_manager::load_font -> Failed finding an atlas for the font!");
			return nullptr;
		}

		_fonts.push_back(fnt);

		int current_atlas_pen_x = 0;
		int current_atlas_pen_y = 0;

		for (int i = range0; i < range1; i++)
		{
			glyph& glyph_info = fnt->glyph_info[i];

			if (glyph_info.width <= 0 || glyph_info.height <= 0)
			{
				glyph_info.atlas_x = 0;
				glyph_info.atlas_y = 0;
				continue;
			}

			const int w = glyph_info.width;
			const int h = glyph_info.height;
			if (current_atlas_pen_x + w > static_cast<int>(config.atlas_width))
			{
				current_atlas_pen_x = 0;
				current_atlas_pen_y += max_height;
			}

			if (current_atlas_pen_y + h > static_cast<int>(fnt->_atlas_required_height)) { ASSERT(false); }

			glyph_info.atlas_x = current_atlas_pen_x;
			glyph_info.atlas_y = fnt->_atlas_pos + current_atlas_pen_y;
			glyph_info.uv_x	   = static_cast<float>(glyph_info.atlas_x) / static_cast<float>(fnt->_atlas->get_width());
			glyph_info.uv_y	   = static_cast<float>(glyph_info.atlas_y) / static_cast<float>(fnt->_atlas->get_height());
			glyph_info.uv_w	   = static_cast<float>(w) / static_cast<float>(fnt->_atlas->get_width());
			glyph_info.uv_h	   = static_cast<float>(h) / static_cast<float>(fnt->_atlas->get_height());

			unsigned char* dest_pixel_ptr = fnt->_atlas->get_data() + (glyph_info.atlas_y * fnt->_atlas->get_width()) + glyph_info.atlas_x;

			if (is_sdf)
			{
				int atlas_stride = fnt->_atlas->get_width(); // assuming 1 byte per pixel
				for (int row = 0; row < h; ++row)
				{
					std::memcpy(dest_pixel_ptr + row * atlas_stride, glyph_info.sdf_data + row * w, w);
				}
			}
			else
			{
				stbtt_MakeCodepointBitmap(&stb_font,
										  dest_pixel_ptr,
										  glyph_info.width,			// Output bitmap width
										  glyph_info.height,		// Output bitmap height
										  fnt->_atlas->get_width(), // Atlas stride/pitch
										  fnt->_scale,				// Horizontal scale
										  fnt->_scale,				// Vertical scale
										  i);						// Codepoint
			}

			current_atlas_pen_x += w + x_padding;
		}

		if (_atlas_updated_cb) _atlas_updated_cb(fnt->_atlas);
		return fnt;
	}

	font* font_manager::load_font(const char* filename, unsigned int size, unsigned int range_start, unsigned int range_end, bool is_sdf, int sdf_padding, int sdf_edge, float sdf_distance)
	{
		if (range_start >= range_end)
		{
			V_ERR("vekt::font_manager::load_font -> range_start needs to be smaller than range_end! %s", filename);
			return nullptr;
		}

		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			V_ERR("vekt::font_manager::load_font -> Failed opening font file! %s", filename);
			return nullptr;
		}

		std::streamsize file_size = file.tellg();
		file.seekg(0, std::ios::beg);

		pod_vector<unsigned char> ttf_buffer;
		ttf_buffer.resize(static_cast<unsigned int>(file_size));
		if (!file.read(reinterpret_cast<char*>(ttf_buffer.data()), file_size))
		{
			V_ERR("vekt::font_manager::load_font -> Failed reading font buffer! %s", filename);
			return nullptr;
		}

		return load_font(ttf_buffer.data(), ttf_buffer.size(), size, range_start, range_end, is_sdf, sdf_padding, sdf_edge, sdf_distance);
	}

	void font_manager::unload_font(font* fnt)
	{
		fnt->_atlas->remove_font(fnt);

		if (fnt->_atlas->empty())
		{
			_atlases.remove(fnt->_atlas);

			if (_atlas_destroyed_cb) _atlas_destroyed_cb(fnt->_atlas);

			delete fnt->_atlas;
		}

		_fonts.remove(fnt);
		delete fnt;
	}

	void font_manager::init() { _icons_font = load_font(__icons_ttf, __icons_ttf_len, 32, 32, 37, true, 3, 128, 32); }

	void font_manager::uninit()
	{
		if (_icons_font) unload_font(_icons_font);
		_icons_font = nullptr;

		for (atlas* atl : _atlases)
		{
			if (_atlas_destroyed_cb) _atlas_destroyed_cb(atl);
			delete atl;
		}

		for (font* fnt : _fonts)
			delete fnt;

		_atlases.clear();
		_fonts.clear();
	}

	font::~font()
	{
		for (unsigned int i = 0; i < 128; i++)
		{
			glyph& g = glyph_info[i];

			if (g.sdf_data) stbtt_FreeSDF(g.sdf_data, nullptr);
		}
	}
}

#endif

#endif
