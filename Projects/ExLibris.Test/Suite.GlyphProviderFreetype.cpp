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

	virtual std::string GetFontPath()
	{
		return "Fonts/Mathilde/Mathilde.otf";
	}

	void SetUp()
	{
		FT_Error errors = 0;

		errors = FT_Init_FreeType(&library_freetype);
		ASSERT_EQ(FT_Err_Ok, errors);

		std::fstream in_stream(GetFontPath(), std::ios::in | std::ios::binary);
		ASSERT_TRUE(in_stream.is_open());

		in_stream.seekg(0, std::ios_base::end);
		std::streamoff font_file_size = in_stream.tellg();
		ASSERT_GT(font_file_size, 0);

		in_stream.seekg(0, std::ios_base::beg);

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

	std::string font_path;

	FT_Library library_freetype;

};

TEST_F(GlyphProviderFreetypeContext, Construct)
{
	ASSERT_NE(nullptr, provider->GetFamily());
	EXPECT_STREQ("Mathilde", provider->GetFamily()->GetName().c_str());
	EXPECT_TRUE(provider->HasKerning());
	EXPECT_TRUE(provider->IsScalable());
	EXPECT_EQ(eStyle_None, provider->GetStyle());
	EXPECT_EQ(eWeight_Normal, provider->GetWeight());
}

TEST_F(GlyphProviderFreetypeContext, SizeAvailable)
{
	EXPECT_TRUE(provider->IsSizeAvailable(12.0f));
}

TEST_F(GlyphProviderFreetypeContext, SizeAvailableNegative)
{
	EXPECT_TRUE(provider->IsSizeAvailable(-33.9f));
}

TEST_F(GlyphProviderFreetypeContext, CreateMetrics)
{
	GlyphMetrics* metrics = provider->CreateMetrics(24.0f, (int)'A');
	ASSERT_NE(nullptr, metrics);

	EXPECT_FLOAT_EQ(17.0f, metrics->advance);
	EXPECT_VEC2_EQ(0.0f, -16.0f, metrics->offset);
	EXPECT_VEC2_EQ(0.2187500f, 18.828125f, metrics->bounding_box.GetMinimum());
	EXPECT_VEC2_EQ(23.515625f, 36.015625f, metrics->bounding_box.GetMaximum());
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

	EXPECT_EQ(6, bitmap->width);
	EXPECT_EQ(8, bitmap->height);
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

	EXPECT_EQ(23, outline->GetPositionCount());
	EXPECT_VEC2_EQ(0.3281250f, 0.4531250f, outline->GetPosition(0));
}

TEST_F(GlyphProviderFreetypeContext, CreateOutlineNotFound)
{
	CurvePath* outline = provider->CreateOutline(24.6f, 0x1FFF);
	EXPECT_EQ(nullptr, outline);
}

TEST_F(GlyphProviderFreetypeContext, GetKerningAdjustment)
{
	glm::vec2 adjustment;
	EXPECT_TRUE(provider->TryGetKerningAdjustment(adjustment, 24.0f, (int)'D', (int)'a'));

	EXPECT_VEC2_EQ(-1.0f, 0.0f, adjustment);
}

TEST_F(GlyphProviderFreetypeContext, GetKerningAdjustmentNone)
{
	glm::vec2 adjustment;
	EXPECT_FALSE(provider->TryGetKerningAdjustment(adjustment, 16.0f, (int)' ', (int)'!'));
}

TEST_F(GlyphProviderFreetypeContext, GetKerningAdjustmentNotFound)
{
	glm::vec2 adjustment;
	EXPECT_FALSE(provider->TryGetKerningAdjustment(adjustment, 55.0f, 0x2000, (int)'Q'));
}

TEST_F(GlyphProviderFreetypeContext, CreateFace)
{
	Face* face = provider->CreateFace(16.0f);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("Mathilde", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(16.0f, face->GetSize());
	EXPECT_FLOAT_EQ(22.0f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(13.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-10.0f, face->GetDescent());
}

class GlyphProviderFreetypeBitmapFontContext
	: public GlyphProviderFreetypeContext
{

public:

	std::string GetFontPath()
	{
		return "Fonts/00_starmap/00.fon";
	}

};

TEST_F(GlyphProviderFreetypeBitmapFontContext, Construct)
{
	ASSERT_NE(nullptr, provider->GetFamily());
	EXPECT_STREQ("00 Starmap", provider->GetFamily()->GetName().c_str());
	EXPECT_FALSE(provider->HasKerning());
	EXPECT_FALSE(provider->IsScalable());
	EXPECT_EQ(eStyle_None, provider->GetStyle());
	EXPECT_EQ(eWeight_Normal, provider->GetWeight());
}

TEST_F(GlyphProviderFreetypeBitmapFontContext, SizeAvailable)
{
	EXPECT_TRUE(provider->IsSizeAvailable(8.0f));
}

TEST_F(GlyphProviderFreetypeBitmapFontContext, SizeNotAvailable)
{
	EXPECT_FALSE(provider->IsSizeAvailable(32.3f));
}

TEST_F(GlyphProviderFreetypeBitmapFontContext, CreateMetrics)
{
	GlyphMetrics* metrics = provider->CreateMetrics(8.0f, (int)'Z');
	ASSERT_NE(nullptr, metrics);

	EXPECT_FLOAT_EQ(7.0f, metrics->advance);
	EXPECT_VEC2_EQ(0.0f, -7.0f, metrics->offset);
	EXPECT_VEC2_EQ(0.0f, 1.0f, metrics->bounding_box.GetMinimum());
	EXPECT_VEC2_EQ(7.0f, 8.0f, metrics->bounding_box.GetMaximum());
}

TEST_F(GlyphProviderFreetypeBitmapFontContext, CreateMetricsNotFound)
{
	GlyphMetrics* metrics = provider->CreateMetrics(8.0f, 1444);
	EXPECT_EQ(nullptr, metrics);
}

TEST_F(GlyphProviderFreetypeBitmapFontContext, CreateBitmap)
{
	GlyphBitmap* bitmap = provider->CreateBitmap(8.0f, (int)'@');
	ASSERT_NE(nullptr, bitmap);

	EXPECT_EQ(8, bitmap->width);
	EXPECT_EQ(8, bitmap->height);
}

TEST_F(GlyphProviderFreetypeBitmapFontContext, CreateBitmapNotFound)
{
	GlyphBitmap* bitmap = provider->CreateBitmap(8.0f, 0x2000);
	EXPECT_EQ(nullptr, bitmap);
}

TEST_F(GlyphProviderFreetypeBitmapFontContext, CreateOutline)
{
	CurvePath* outline = provider->CreateOutline(8.0f, (int)'7');
	EXPECT_EQ(nullptr, outline);
}