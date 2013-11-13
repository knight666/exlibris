#include "ExLibris.Test.PCH.h"

#include <FaceRequest.h>

using namespace ExLibris;

TEST(FaceRequest, Construct)
{
	FaceRequest fr;

	EXPECT_STREQ("", fr.GetFamilyName().c_str());
	EXPECT_FALSE(fr.HasFamilyName());
	EXPECT_FLOAT_EQ(0.0f, fr.GetSize());
	EXPECT_FALSE(fr.HasSize());
	EXPECT_EQ(eWeight_Normal, fr.GetWeight());
	EXPECT_FALSE(fr.HasWeight());
	EXPECT_EQ(eStyle_None, fr.GetStyle());
	EXPECT_FALSE(fr.HasStyle());
}

TEST(FaceRequest, FamilyName)
{
	FaceRequest fr;

	fr.SetFamilyName("Arial");

	EXPECT_STREQ("Arial", fr.GetFamilyName().c_str());
	EXPECT_TRUE(fr.HasFamilyName());
}

TEST(FaceRequest, Size)
{
	FaceRequest fr;

	fr.SetSize(16.0f);

	EXPECT_FLOAT_EQ(16.0f, fr.GetSize());
	EXPECT_TRUE(fr.HasSize());
}

TEST(FaceRequest, Weight)
{
	FaceRequest fr;

	fr.SetWeight(eWeight_Bold);

	EXPECT_EQ(eWeight_Bold, fr.GetWeight());
	EXPECT_TRUE(fr.HasWeight());
}

TEST(FaceRequest, Style)
{
	FaceRequest fr;

	fr.SetStyle(eStyle_Italicized);

	EXPECT_EQ(eStyle_Italicized, fr.GetStyle());
	EXPECT_TRUE(fr.HasStyle());
}