#include "ExLibris.Test.PCH.h"

#include <CurvePath.h>
#include <Face.h>
#include <Family.h>
#include <GlyphBitmap.h>
#include <GlyphMetrics.h>
#include <GlyphProviderFreetype.h>
#include <Library.h>

using namespace ExLibris;

class GlyphProviderFreetypeContext
	: public ::testing::Test
{

public:

	void SetUp()
	{
		FT_Error errors = 0;

		errors = FT_Init_FreeType(&library_freetype);
		ASSERT_EQ(FT_Err_Ok, errors);

		std::fstream in_stream("Fonts/Roboto/Roboto-Regular.ttf", std::ios::in | std::ios::binary);

		in_stream.seekg(0, std::ios_base::end);
		std::streamoff font_file_size = in_stream.tellg();
		in_stream.seekg(0, std::ios_base::beg);

		ASSERT_GT(font_file_size, 0);

		FT_Byte* font_file_data = new FT_Byte[(unsigned int)font_file_size];
		in_stream.read((char*)font_file_data, font_file_size);

		FT_Face font_loaded = nullptr;
		errors = FT_New_Memory_Face(library_freetype, font_file_data, (FT_Long)font_file_size, 0, &font_loaded);
		ASSERT_EQ(FT_Err_Ok, errors);

		library = new Library;
		provider = new GlyphProviderFreetype(library, font_loaded, font_file_data, (size_t)font_file_size);
	}

	void TearDown()
	{
		delete provider;
		delete library;

		FT_Done_FreeType(library_freetype);
	}

protected:

	Library* library;
	GlyphProviderFreetype* provider;

	FT_Library library_freetype;

};

TEST_F(GlyphProviderFreetypeContext, Construct)
{
	ASSERT_NE(nullptr, provider->GetFamily());
	EXPECT_STREQ("Roboto", provider->GetFamily()->GetName().c_str());
	EXPECT_EQ(eStyle_None, provider->GetStyle());
	EXPECT_EQ(eWeight_Normal, provider->GetWeight());
}

TEST_F(GlyphProviderFreetypeContext, CreateMetrics)
{
	GlyphMetrics* metrics = provider->CreateMetrics(24.0f, (int)'A');
	ASSERT_NE(nullptr, metrics);

	EXPECT_FLOAT_EQ(21.0f, metrics->advance);
	EXPECT_VEC2_EQ(0.0f, -24.0f, metrics->offset);
	EXPECT_VEC2_EQ(0.3125000f, 19.000000f, metrics->bounding_box.GetMinimum());
	EXPECT_VEC2_EQ(20.406250f, 42.000000f, metrics->bounding_box.GetMaximum());
}

TEST_F(GlyphProviderFreetypeContext, CreateMetricsNotFound)
{
	GlyphMetrics* metrics = provider->CreateMetrics(16.7f, 0x0200);
	EXPECT_EQ(nullptr, metrics);
}

TEST_F(GlyphProviderFreetypeContext, CreateBitmap)
{
	GlyphBitmap* bitmap = provider->CreateBitmap(18.0f, (int)'0');
	ASSERT_NE(nullptr, bitmap);

	EXPECT_EQ(12, bitmap->width);
	EXPECT_EQ(17, bitmap->height);
}

TEST_F(GlyphProviderFreetypeContext, CreateBitmapNotFound)
{
	GlyphBitmap* bitmap = provider->CreateBitmap(44.2f, 0x03FF);
	EXPECT_EQ(nullptr, bitmap);
}

TEST_F(GlyphProviderFreetypeContext, CreateOutline)
{
	CurvePath* outline = provider->CreateOutline(12.0f, (int)'-');
	ASSERT_NE(nullptr, outline);

	EXPECT_EQ(5, outline->GetPositionCount());
	EXPECT_VEC2_EQ(3.8281250f, 1.2031250f, outline->GetPosition(0));
	EXPECT_VEC2_EQ(0.0000000f, 1.2031250f, outline->GetPosition(1));
	EXPECT_VEC2_EQ(0.0000000f, 0.0000000f, outline->GetPosition(2));
	EXPECT_VEC2_EQ(3.8281250f, 0.0000000f, outline->GetPosition(3));
	EXPECT_VEC2_EQ(3.8281250f, 1.2031250f, outline->GetPosition(4));
}

TEST_F(GlyphProviderFreetypeContext, CreateOutlineNotFound)
{
	CurvePath* outline = provider->CreateOutline(24.6f, 0x1FFF);
	EXPECT_EQ(nullptr, outline);
}

TEST_F(GlyphProviderFreetypeContext, CreateFace)
{
	Face* face = provider->CreateFace(16.0f);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("Roboto", face->GetFamily()->GetName().c_str());
	EXPECT_FLOAT_EQ(16.0f, face->GetSize());
	EXPECT_FLOAT_EQ(28.0f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(22.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-6.0f, face->GetDescent());
}