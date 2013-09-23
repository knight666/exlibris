#ifndef _EXLIBRIS_TEST_PCH_H_
#define _EXLIBRIS_TEST_PCH_H_

// STL

#include <map>
#include <string>
#include <vector>

// Freetype

#include <ft2build.h>
#include <freetype/freetype.h>

// Google Test

#include <gtest/gtest.h>

#define EXPECT_VEC2_EQ(_expected_x, _expected_y, _actual) { \
	EXPECT_FLOAT_EQ(_expected_x, _actual.x); \
	EXPECT_FLOAT_EQ(_expected_y, _actual.y); \
}

#endif