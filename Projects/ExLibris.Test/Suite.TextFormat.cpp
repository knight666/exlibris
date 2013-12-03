#include "ExLibris.Test.PCH.h"

#include <Face.h>
#include <Family.h>
#include <Library.h>
#include <TextFormat.h>

#include "Mock.FontLoader.h"

using namespace ExLibris;

TEST(TextFormat, Construct)
{
	Library lib;
	TextFormat tf(&lib);

	Face* f = tf.GetFace();
	ASSERT_NE(nullptr, f);
	EXPECT_FLOAT_EQ(12.0f, f->GetSize());
	EXPECT_EQ(eWeight_Normal, f->GetWeight());
	EXPECT_EQ(eStyle_None, f->GetStyle());

	Family* fm = tf.GetFamily();
	ASSERT_NE(nullptr, fm);
	EXPECT_STREQ("System", fm->GetName().c_str());

	EXPECT_FALSE(tf.HasProperty(TextFormat::eProperty_FamilyName));
	EXPECT_STREQ("", tf.GetFamilyName().c_str());

	EXPECT_FALSE(tf.HasProperty(TextFormat::eProperty_Size));
	EXPECT_FLOAT_EQ(0.0f, tf.GetSize());

	EXPECT_FALSE(tf.HasProperty(TextFormat::eProperty_Weight));
	EXPECT_EQ(eWeight_Normal, tf.GetWeight());

	EXPECT_FALSE(tf.HasProperty(TextFormat::eProperty_Style));
	EXPECT_EQ(eStyle_None, tf.GetStyle());

	EXPECT_FALSE(tf.HasProperty(TextFormat::eProperty_Color));
	EXPECT_COLOR_EQ(0.0f, 0.0f, 0.0f, 1.0f, tf.GetColor());

	EXPECT_FALSE(tf.HasProperty(TextFormat::eProperty_BackgroundColor));
	EXPECT_COLOR_EQ(1.0f, 1.0f, 1.0f, 1.0f, tf.GetBackgroundColor());
}

TEST(TextFormat, SetFamilyName)
{
	Library lib;
	TextFormat tf(&lib);

	tf.SetFamilyName("Brilliance");
	EXPECT_TRUE(tf.HasProperty(TextFormat::eProperty_FamilyName));
	EXPECT_STREQ("Brilliance", tf.GetFamilyName().c_str());
}

TEST(TextFormat, SetSize)
{
	Library lib;
	TextFormat tf(&lib);

	tf.SetSize(116.0f);
	EXPECT_TRUE(tf.HasProperty(TextFormat::eProperty_Size));
	EXPECT_FLOAT_EQ(116.0f, tf.GetSize());
}

TEST(TextFormat, SetWeight)
{
	Library lib;
	TextFormat tf(&lib);

	tf.SetWeight(eWeight_Bold);
	EXPECT_TRUE(tf.HasProperty(TextFormat::eProperty_Weight));
	EXPECT_EQ(eWeight_Bold, tf.GetWeight());
}

TEST(TextFormat, SetStyle)
{
	Library lib;
	TextFormat tf(&lib);

	tf.SetStyle(eStyle_Italicized);
	EXPECT_TRUE(tf.HasProperty(TextFormat::eProperty_Style));
	EXPECT_EQ(eStyle_Italicized, tf.GetStyle());
}

TEST(TextFormat, SetColor)
{
	Library lib;
	TextFormat tf(&lib);

	tf.SetColor(glm::vec4(1.0f, 0.4f, 0.25f, 0.8f));
	EXPECT_TRUE(tf.HasProperty(TextFormat::eProperty_Color));
	EXPECT_COLOR_EQ(1.0f, 0.4f, 0.25f, 0.8f, tf.GetColor());
}

TEST(TextFormat, SetBackgroundColor)
{
	Library lib;
	TextFormat tf(&lib);

	tf.SetBackgroundColor(glm::vec4(0.56f, 0.23f, 0.85f, 0.95f));
	EXPECT_TRUE(tf.HasProperty(TextFormat::eProperty_BackgroundColor));
	EXPECT_COLOR_EQ(0.56f, 0.23f, 0.85f, 0.95f, tf.GetBackgroundColor());
}

TEST(TextFormat, GetFaceAndFamily)
{
	Library lib;

	MockFontLoader* loader = new MockFontLoader(&lib);
	loader->family_name = "Magic Shop";
	loader->weight = eWeight_Bold;
	lib.AddLoader(loader);

	std::stringstream ss;
	lib.MapFontToStream(ss);

	TextFormat tf(&lib);
	tf.SetFamilyName("Magic Shop");
	tf.SetSize(25.8f);
	tf.SetWeight(eWeight_Bold);

	Face* f = tf.GetFace();
	ASSERT_NE(nullptr, f);
	EXPECT_FLOAT_EQ(25.8f, f->GetSize());
	EXPECT_EQ(eWeight_Bold, f->GetWeight());
	EXPECT_EQ(eStyle_None, f->GetStyle());

	Family* fm = f->GetFamily();
	ASSERT_NE(nullptr, fm);
	EXPECT_STREQ("Magic Shop", fm->GetName().c_str());
}