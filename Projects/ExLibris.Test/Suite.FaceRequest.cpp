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