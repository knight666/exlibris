#include "ExLibris.Test.PCH.h"

#include <FreetypeErrors.h>

using namespace ExLibris;

TEST(FreetypeErrors, TranslateOutOfMemory)
{
	EXPECT_STREQ("out of memory", Freetype::TranslateError(FT_Err_Out_Of_Memory).c_str());
}

TEST(FreetypeErrors, TranslateUnknown)
{
	EXPECT_STREQ("Unknown error: (-1)", Freetype::TranslateError(-1).c_str());
}