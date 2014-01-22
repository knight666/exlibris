#include "ExLibris.Test.PCH.h"

#include <RtfUnit.h>

using namespace ExLibris;

TEST(RtfUnit, Construct)
{
	RtfUnit u;
	
	EXPECT_FLOAT_EQ(0.0f, u.GetValue(RtfUnit::eType_Twips));
}

TEST(RtfUnit, ConstructData)
{
	RtfUnit u(159.2f);

	EXPECT_FLOAT_EQ(159.2f, u.GetValue(RtfUnit::eType_Twips));
}

TEST(RtfUnit, ConstructTwips)
{
	RtfUnit u(RtfUnit::eType_Twips, 8712.2f);

	EXPECT_FLOAT_EQ(8712.2f, u.GetValue(RtfUnit::eType_Twips));
}

TEST(RtfUnit, ConstructPoints)
{
	RtfUnit u(RtfUnit::eType_Points, 444.4f);

	EXPECT_FLOAT_EQ(444.4f, u.GetValue(RtfUnit::eType_Points));
}

TEST(RtfUnit, ConstructDeviceIndependentUnits)
{
	RtfUnit u(RtfUnit::eType_DeviceIndependentUnits, 156.8f);

	EXPECT_FLOAT_EQ(156.8f, u.GetValue(RtfUnit::eType_DeviceIndependentUnits));
}

TEST(RtfUnit, ConstructEnglishMetricUnits)
{
	RtfUnit u(RtfUnit::eType_EnglishMetricUnits, 33.9f);

	EXPECT_FLOAT_EQ(33.9f, u.GetValue(RtfUnit::eType_EnglishMetricUnits));
}

TEST(RtfUnit, ConstructPixels)
{
	RtfUnit u(RtfUnit::eType_Pixels, 4.61f);

	EXPECT_FLOAT_EQ(4.61f, u.GetValue(RtfUnit::eType_Pixels));
}

TEST(RtfUnit, GetValueConverted)
{
	RtfUnit u(2300.0f);
	
	EXPECT_FLOAT_EQ(2300.0f, u.GetValue(RtfUnit::eType_Twips));
	EXPECT_FLOAT_EQ(2300.0f * 20.0f, u.GetValue(RtfUnit::eType_Points));
	EXPECT_FLOAT_EQ(2300.0f / 204.8f, u.GetValue(RtfUnit::eType_DeviceIndependentUnits));
	EXPECT_FLOAT_EQ(2300.0f / 635.0f, u.GetValue(RtfUnit::eType_EnglishMetricUnits));
	EXPECT_FLOAT_EQ(2300.0f * 15.0f, u.GetValue(RtfUnit::eType_Pixels));
}

TEST(RtfUnit, SetValueTwips)
{
	RtfUnit u;
	u.SetValue(RtfUnit::eType_Twips, 17.9f);

	EXPECT_FLOAT_EQ(17.9f, u.GetValue(RtfUnit::eType_Twips));
}

TEST(RtfUnit, SetValuePoints)
{
	RtfUnit u;
	u.SetValue(RtfUnit::eType_Points, 15.0f);

	EXPECT_FLOAT_EQ(15.0f, u.GetValue(RtfUnit::eType_Points));
}

TEST(RtfUnit, SetValueDeviceIndependentUnits)
{
	RtfUnit u;
	u.SetValue(RtfUnit::eType_DeviceIndependentUnits, 1887.9f);

	EXPECT_FLOAT_EQ(1887.9f, u.GetValue(RtfUnit::eType_DeviceIndependentUnits));
}

TEST(RtfUnit, SetValueEnglishMetricUnits)
{
	RtfUnit u;
	u.SetValue(RtfUnit::eType_EnglishMetricUnits, 65.78f);

	EXPECT_FLOAT_EQ(65.78f, u.GetValue(RtfUnit::eType_EnglishMetricUnits));
}

TEST(RtfUnit, SetValuePixels)
{
	RtfUnit u;
	u.SetValue(RtfUnit::eType_Pixels, 15.6f);

	EXPECT_FLOAT_EQ(15.6f, u.GetValue(RtfUnit::eType_Pixels));
}