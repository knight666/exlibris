#include "ExLibris.Test.PCH.h"

#include <CurvePath.h>
#include <Face.h>
#include <Family.h>
#include <GlyphBitmap.h>
#include <GlyphMetrics.h>
#include <GlyphProviderSystem.h>
#include <Library.h>

using namespace ExLibris;

class GlyphProviderSystemContext
	: public ::testing::Test
{

public:

	void SetUp()
	{
		library = new Library;
		provider = new GlyphProviderSystem(library);
	}

	void TearDown()
	{
		delete provider;
		delete library;
	}

protected:

	Library* library;
	GlyphProviderSystem* provider;

};

TEST_F(GlyphProviderSystemContext, Construct)
{
	ASSERT_NE(nullptr, provider->GetFamily());
	EXPECT_STREQ("System", provider->GetFamily()->GetName().c_str());
	EXPECT_FALSE(provider->HasKerning());
	EXPECT_FALSE(provider->IsScalable());
	EXPECT_EQ(eStyle_None, provider->GetStyle());
	EXPECT_EQ(eWeight_Normal, provider->GetWeight());
}

TEST_F(GlyphProviderSystemContext, SizeAvailable)
{
	EXPECT_TRUE(provider->IsSizeAvailable(55.0f));
	EXPECT_TRUE(provider->IsSizeAvailable(4.0f));
	EXPECT_TRUE(provider->IsSizeAvailable(136.551f));
}

TEST_F(GlyphProviderSystemContext, CreateMetrics)
{
	GlyphMetrics* metrics = provider->CreateMetrics(12.0f, (int)'&');
	ASSERT_NE(nullptr, metrics);

	EXPECT_FLOAT_EQ(8.0f, metrics->advance);
	EXPECT_VEC2_EQ(0.0f, 0.0f, metrics->offset);
	EXPECT_VEC2_EQ(0.0f, 0.0f, metrics->bounding_box.GetMinimum());
	EXPECT_VEC2_EQ(8.0f, 12.0f, metrics->bounding_box.GetMaximum());
}

TEST_F(GlyphProviderSystemContext, CreateMetricsNotFound)
{
	GlyphMetrics* metrics = provider->CreateMetrics(12.0f, 0x877);
	ASSERT_NE(nullptr, metrics);

	EXPECT_FLOAT_EQ(8.0f, metrics->advance);
	EXPECT_VEC2_EQ(0.0f, 0.0f, metrics->offset);
	EXPECT_VEC2_EQ(0.0f, 0.0f, metrics->bounding_box.GetMinimum());
	EXPECT_VEC2_EQ(8.0f, 12.0f, metrics->bounding_box.GetMaximum());
}

TEST_F(GlyphProviderSystemContext, CreateBitmap)
{
	GlyphBitmap* bitmap = provider->CreateBitmap(24.0f, 87);
	ASSERT_NE(nullptr, bitmap);

	EXPECT_EQ(8, bitmap->width);
	EXPECT_EQ(12, bitmap->height);
}

TEST_F(GlyphProviderSystemContext, CreateBitmapNotFound)
{
	GlyphBitmap* bitmap = provider->CreateBitmap(24.0f, 0x9999);
	ASSERT_NE(nullptr, bitmap);

	EXPECT_EQ(8, bitmap->width);
	EXPECT_EQ(12, bitmap->height);
}

TEST_F(GlyphProviderSystemContext, CreateOutline)
{
	CurvePath* outline = provider->CreateOutline(6.78f, 117);
	ASSERT_EQ(nullptr, outline);
}

TEST_F(GlyphProviderSystemContext, GetKerningAdjustment)
{
	glm::vec2 adjustment;
	EXPECT_FALSE(provider->TryGetKerningAdjustment(adjustment, 43.1f, 117, 45));
}

TEST_F(GlyphProviderSystemContext, CreateFace)
{
	Face* face = provider->CreateFace(12.0f);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("System", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(12.0f, face->GetSize());
	EXPECT_FLOAT_EQ(16.0f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(0.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(0.0f, face->GetDescent());
}