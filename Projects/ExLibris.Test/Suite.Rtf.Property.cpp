#include "ExLibris.Test.PCH.h"

#include <RtfProperty.h>

using namespace ExLibris;

TEST(RtfProperty, Construct)
{
	unsigned int specified = 0;
	RtfProperty<int, 3> p(123, specified);

	EXPECT_FALSE(p.IsSet());
	EXPECT_EQ(123, p.Get());
	EXPECT_EQ(0, specified);
}

TEST(RtfProperty, ConstructPointer)
{
	unsigned int specified = 0;
	RtfProperty<float*, 2> p(nullptr, specified);

	EXPECT_EQ(nullptr, p.Get());
	EXPECT_FALSE(p.IsSet());
	EXPECT_EQ(0, specified);
}

TEST(RtfProperty, SetValue)
{
	unsigned int specified = 0;
	RtfProperty<int, 2> p(0, specified);

	p = 187;

	EXPECT_EQ(187, p.Get());
	EXPECT_TRUE(p.IsSet());
	EXPECT_EQ(1 << 2, specified);
}

TEST(RtfProperty, SetValuePointer)
{
	float value = 9.12f;
	unsigned int specified = 0;
	RtfProperty<const float*, 16> p(nullptr, specified);

	p = &value;

	EXPECT_EQ(&value, p.Get());
	EXPECT_TRUE(p.IsSet());
	EXPECT_EQ(1 << 16, specified);
}