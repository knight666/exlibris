#include "ExLibris.Test.PCH.h"

#include <Face.h>
#include <Family.h>

#include "Mock.GlyphProvider.h"

using namespace ExLibris;

class FaceContext
	: public ::testing::Test
{

public:

	void SetUp()
	{
		library = new Library;
		provider = new MockGlyphProvider(library);

		FontMetrics metrics;
		metrics.family = library->CreateFamily("MockFamily");
		metrics.size = 16.0f;
		metrics.line_height = 18.0f;
		metrics.ascent = 12.0f;
		metrics.descent = -4.0f;

		face = new Face(*provider, metrics);
	}

	void TearDown()
	{
		delete face;
		delete library;
	}

protected:

	Library* library;
	MockGlyphProvider* provider;
	Face* face;

};

TEST_F(FaceContext, Construct)
{
	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(16.0f, face->GetSize());
	EXPECT_FLOAT_EQ(18.0f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(12.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-4.0f, face->GetDescent());
}

TEST_F(FaceContext, CreateMetrics)
{
	GlyphMetrics* metrics = face->CreateMetrics((int)'P');
	ASSERT_NE(nullptr, metrics);

	EXPECT_EQ(1, face->GetMetricsLoaded());

	EXPECT_VEC2_EQ(4.0f, 8.0f, metrics->offset);
	EXPECT_FLOAT_EQ(12.0f, metrics->advance);
	EXPECT_VEC2_EQ(2.0f, 2.0f, metrics->bounding_box.GetMinimum());
	EXPECT_VEC2_EQ(10.0f, 10.0f, metrics->bounding_box.GetMaximum());
}

TEST_F(FaceContext, CreateMetricsTwice)
{
	GlyphMetrics* metrics = face->CreateMetrics(0x219);
	GlyphMetrics* metrics_again = face->CreateMetrics(0x219);

	EXPECT_EQ(metrics_again, metrics);

	EXPECT_EQ(1, face->GetMetricsLoaded());
}

TEST_F(FaceContext, CreateMetricsNotFound)
{
	provider->codepoint_blacklist.insert(0x45);

	GlyphMetrics* metrics = face->CreateMetrics(0x45);
	EXPECT_EQ(nullptr, metrics);

	EXPECT_EQ(1, face->GetMetricsLoaded());
}

TEST_F(FaceContext, CreateBitmap)
{
	GlyphBitmap* bitmap = face->CreateBitmap(0x999);
	ASSERT_NE(nullptr, bitmap);

	EXPECT_EQ(1, face->GetBitmapsLoaded());

	EXPECT_EQ(8, bitmap->width);
	EXPECT_EQ(8, bitmap->height);
}

TEST_F(FaceContext, CreateBitmapTwice)
{
	GlyphBitmap* bitmap = face->CreateBitmap(32);
	GlyphBitmap* bitmap_again = face->CreateBitmap(32);

	EXPECT_EQ(bitmap_again, bitmap);

	EXPECT_EQ(1, face->GetBitmapsLoaded());
}

TEST_F(FaceContext, CreateBitmapNotFound)
{
	provider->codepoint_blacklist.insert(788);

	GlyphBitmap* bitmap = face->CreateBitmap(788);
	EXPECT_EQ(nullptr, bitmap);

	EXPECT_EQ(1, face->GetBitmapsLoaded());
}

TEST_F(FaceContext, CreateOutline)
{
	CurvePath* outline = face->CreateOutline(125);
	ASSERT_NE(nullptr, outline);

	EXPECT_EQ(1, face->GetOutlinesLoaded());

	EXPECT_EQ(5, outline->GetCommandCount());
	EXPECT_EQ(5, outline->GetPositionCount());
}

TEST_F(FaceContext, CreateOutlineTwice)
{
	CurvePath* outline = face->CreateOutline((int)'m');
	CurvePath* outline_again = face->CreateOutline((int)'m');

	EXPECT_EQ(outline_again, outline);

	EXPECT_EQ(1, face->GetOutlinesLoaded());
}

TEST_F(FaceContext, CreateOutlineNotFound)
{
	provider->codepoint_blacklist.insert(72718);

	CurvePath* outline = face->CreateOutline(72718);
	EXPECT_EQ(nullptr, outline);

	EXPECT_EQ(1, face->GetOutlinesLoaded());
}

TEST_F(FaceContext, GetKerningAdjustment)
{
	glm::vec2 adjustment;
	ASSERT_TRUE(face->TryGetKerningAdjustment(adjustment, 66, 126));

	EXPECT_VEC2_EQ(-1.5f, 5.6f, adjustment);
}

TEST_F(FaceContext, GetKerningAdjustmentNotFound)
{
	provider->codepoint_blacklist.insert(167);

	glm::vec2 adjustment;
	EXPECT_FALSE(face->TryGetKerningAdjustment(adjustment, 167, 56));
}