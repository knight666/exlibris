#ifndef _EXLIBRIS_TEST_PCH_H_
#define _EXLIBRIS_TEST_PCH_H_

// STL

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

// Freetype

#include <ft2build.h>
#include <freetype/freetype.h>

// Google Test

#include <gtest/gtest.h>

// Macro's

#define EXPECT_VEC2_EQ(_expected_x, _expected_y, _actual) { \
	EXPECT_FLOAT_EQ(_expected_x, _actual.x); \
	EXPECT_FLOAT_EQ(_expected_y, _actual.y); \
}

#include <Macros.h>

#define EXPECT_INDEX_VALID(_index)   EXPECT_NE(EXL_INVALID_INDEX, (_index))
#define EXPECT_INDEX_INVALID(_index) EXPECT_EQ(EXL_INVALID_INDEX, (_index))

#endif