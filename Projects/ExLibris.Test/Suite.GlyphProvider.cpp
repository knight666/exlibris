#include "ExLibris.Test.PCH.h"

#include <Face.h>
#include <Library.h>

#include "Mock.GlyphProvider.h"

using namespace ExLibris;

TEST(GlyphProvider, GetMatchScoreBestMatch)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->SetWeight(eWeight_Bold);
	provider->SetStyle(eStyle_None);

	EXPECT_EQ(110, provider->GetMatchScore(16.0f, eWeight_Bold, eStyle_None));
}

TEST(GlyphProvider, GetMatchScoreSizeNotAvailable)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->size_blacklist.insert(25.0f);

	EXPECT_EQ(-1, provider->GetMatchScore(25.0f, eWeight_Normal, eStyle_None));
}

TEST(GlyphProvider, GetMatchScoreIncorrectWeight)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->SetWeight(eWeight_Bold);

	EXPECT_EQ(10, provider->GetMatchScore(55.2f, eWeight_Normal, eStyle_None));
}

TEST(GlyphProvider, GetMatchScoreIncorrectStyle)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->SetStyle(eStyle_Italicized);

	EXPECT_EQ(100, provider->GetMatchScore(17.87f, eWeight_Normal, eStyle_None));
}

TEST(GlyphProvider, GetMatchScoreIncorrectWeightAndStyle)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->SetWeight(eWeight_Normal);
	provider->SetStyle(eStyle_Italicized);

	EXPECT_EQ(0, provider->GetMatchScore(12.33f, eWeight_Bold, eStyle_None));
}

TEST(GlyphProvider, CreateMetrics)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);

	GlyphMetrics* metrics = provider->CreateMetrics(16.0f, (int)'8');
	ASSERT_NE(nullptr, metrics);

	EXPECT_VEC2_EQ(4.0f, 8.0f, metrics->offset);
	EXPECT_FLOAT_EQ(12.0f, metrics->advance);
	EXPECT_VEC2_EQ(2.0f, 2.0f, metrics->bounding_box.GetMinimum());
	EXPECT_VEC2_EQ(10.0f, 10.0f, metrics->bounding_box.GetMaximum());
}

TEST(GlyphProvider, CreateMetricsNotFound)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->codepoint_blacklist.insert(45);

	GlyphMetrics* metrics = provider->CreateMetrics(14.2f, 45);
	EXPECT_EQ(nullptr, metrics);
}

TEST(GlyphProvider, CreateMetricsSizeNotAvailable)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->size_blacklist.insert(47.8f);

	GlyphMetrics* metrics = provider->CreateMetrics(47.8f, 122);
	EXPECT_EQ(nullptr, metrics);
}

TEST(GlyphProvider, CreateBitmap)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);

	GlyphBitmap* bitmap = provider->CreateBitmap(17.8f, 167);
	ASSERT_NE(nullptr, bitmap);

	EXPECT_EQ(8, bitmap->width);
	EXPECT_EQ(8, bitmap->height);
}

TEST(GlyphProvider, CreateBitmapNotFound)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->codepoint_blacklist.insert(56);

	GlyphBitmap* bitmap = provider->CreateBitmap(65.1f, 56);
	EXPECT_EQ(nullptr, bitmap);
}

TEST(GlyphProvider, CreateBitmapSizeNotAvailable)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->size_blacklist.insert(12.0f);

	GlyphBitmap* bitmap = provider->CreateBitmap(12.0f, 177);
	EXPECT_EQ(nullptr, bitmap);
}

TEST(GlyphProvider, CreateOutline)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);

	CurvePath* outline = provider->CreateOutline(22.5f, 1143);
	ASSERT_NE(nullptr, outline);

	EXPECT_EQ(5, outline->GetCommandCount());
	EXPECT_EQ(5, outline->GetPositionCount());
}

TEST(GlyphProvider, CreateOutlineNotFound)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->codepoint_blacklist.insert(157);

	CurvePath* outline = provider->CreateOutline(8.9f, 157);
	EXPECT_EQ(nullptr, outline);
}

TEST(GlyphProvider, CreateOutlineSizeNotAvailable)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->size_blacklist.insert(16.8f);

	CurvePath* outline = provider->CreateOutline(16.8f, 25);
	EXPECT_EQ(nullptr, outline);
}

TEST(GlyphProvider, GetKerningAdjustment)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);

	glm::vec2 adjustment;
	ASSERT_TRUE(provider->TryGetKerningAdjustment(adjustment, 17.9f, 16, 17));
	
	EXPECT_VEC2_EQ(-1.5f, 5.6f, adjustment);
}

TEST(GlyphProvider, GetKerningAdjustmentNotFound)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->codepoint_blacklist.insert(192);

	glm::vec2 adjustment;
	EXPECT_FALSE(provider->TryGetKerningAdjustment(adjustment, 33.6f, 192, 17));
}

TEST(GlyphProvider, GetKerningAdjustmentSizeNotAvailable)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->size_blacklist.insert(22.8f);

	glm::vec2 adjustment;
	EXPECT_FALSE(provider->TryGetKerningAdjustment(adjustment, 22.8f, 33, 67));
}

TEST(GlyphProvider, CreateFace)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);

	Face* face = provider->CreateFace(66.0f);
	ASSERT_NE(nullptr, face);

	EXPECT_EQ(nullptr, face->GetFamily());
	EXPECT_FLOAT_EQ(66.0f, face->GetSize());
	EXPECT_FLOAT_EQ(66.0f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(66.0f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(66.0f / -3.0f, face->GetDescent());
}

TEST(GlyphProvider, CreateFaceSizeNotAvailable)
{
	Library* library = new Library;

	MockGlyphProvider* provider = new MockGlyphProvider(library);
	provider->size_blacklist.insert(14.8f);

	Face* face = provider->CreateFace(14.8f);
	EXPECT_EQ(nullptr, face);
}