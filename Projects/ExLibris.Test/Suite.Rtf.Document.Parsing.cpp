#include "ExLibris.Test.PCH.h"

#include <RtfDomDocument.h>

#include "Tools.Color.h"

using namespace ExLibris;

class DocumentParsingContext
	: public ::testing::Test
{

protected:

	void SetUp()
	{
		m_World = new RtfWorld();
		m_Document = new RtfDomDocument(m_World);
	}

	void OpenTestFile(const std::string& a_FilePath)
	{
		m_Input.open(a_FilePath, std::ios::in | std::ios::binary);
		ASSERT_TRUE(m_Input.is_open());

		bool result = false;
		try
		{
			result = m_Document->ParseFromSource(&m_Input);
		}
		catch (Exception& e)
		{
			ADD_FAILURE() << e.GetModule() << ": \"" << e.what() << "\" on file \"" << e.GetFilename() << "\" on line " << e.GetLine() << ".";
			return;
		}
		EXPECT_TRUE(result);
	}

	void TearDown()
	{
		delete m_Document;
		delete m_World;
	}

protected:

	RtfWorld* m_World;
	RtfDomDocument* m_Document;
	std::ifstream m_Input;

};

TEST_F(DocumentParsingContext, HelloWorld)
{
	OpenTestFile("Tests/RTF/HelloWorld.rtf");

	RtfFontTable* ft = m_Document->GetFontTable();

	EXPECT_EQ(Rtf::eFamilyType_Swiss, ft->GetFont(0)->GetFamilyType());
	EXPECT_STREQ("Helvetica", ft->GetFont(0)->GetName().c_str());

	RtfDomElement* root = m_Document->GetRootElement();

	EXPECT_EQ(Rtf::eCharacterSet_Ansi, root->GetTextFormat().GetCharacterSet());
	EXPECT_STREQ("Hello World!", root->InnerText.c_str());

	EXPECT_EQ(1, root->GetChildrenCount());
}

TEST_F(DocumentParsingContext, SkipUnknownExtendedCommand)
{
	OpenTestFile("Tests/RTF/UnknownExtendedCommand.rtf");

	RtfDomElement* root = m_Document->GetRootElement();

	EXPECT_EQ(0, root->GetChildrenCount());
	EXPECT_STREQ("This text is totally rad.", root->InnerText.c_str());
}

TEST_F(DocumentParsingContext, FontTable)
{
	OpenTestFile("Tests/RTF/FontTable.rtf");

	RtfFontTable* ft = m_Document->GetFontTable();

	EXPECT_EQ(ft->GetFont(1), ft->GetDefault());

	RtfFont* f0 = ft->GetFont(0);
	EXPECT_STREQ("Magnified", f0->GetName().c_str());
	EXPECT_EQ(Rtf::eFamilyType_Swiss, f0->GetFamilyType());
	EXPECT_EQ(Rtf::eCharacterSet_AppleMacintoshRoman, f0->GetCharacterSet());
	EXPECT_EQ(Rtf::ePitch_Fixed, f0->GetPitch());

	RtfFont* f1 = ft->GetFont(1);
	EXPECT_STREQ("Simplified", f1->GetName().c_str());
	EXPECT_EQ(Rtf::eFamilyType_Modern, f1->GetFamilyType());
	EXPECT_EQ(Rtf::eCharacterSet_Baltic, f1->GetCharacterSet());
	EXPECT_EQ(Rtf::ePitch_Default, f1->GetPitch());

	RtfFont* f2 = ft->GetFont(2);
	EXPECT_STREQ("Testified", f2->GetName().c_str());
	EXPECT_EQ(Rtf::eFamilyType_Roman, f2->GetFamilyType());
	EXPECT_EQ(Rtf::eCharacterSet_ShiftJis, f2->GetCharacterSet());
	EXPECT_EQ(Rtf::ePitch_Variable, f2->GetPitch());

	RtfDomElement* root = m_Document->GetRootElement();
	EXPECT_EQ(ft->GetFont(1), root->GetTextFormat().GetFont());
	EXPECT_STREQ("Bleep bloop.", root->InnerText.c_str());
}

TEST_F(DocumentParsingContext, ColorTable)
{
	OpenTestFile("Tests/RTF/ColorTable.rtf");

	RtfColorTable* ct = m_Document->GetColorTable();

	EXPECT_COLOR_RGBA(0, 0, 0, 255, *ct->GetColor(0));
	EXPECT_COLOR_RGBA(65, 12, 78, 255, *ct->GetColor(1));
	EXPECT_COLOR_RGBA(128, 128, 128, 255, *ct->GetColor(2));
}

TEST_F(DocumentParsingContext, StyleSheet)
{
	OpenTestFile("Tests/RTF/StyleSheet.rtf");

	// style 0
	{
		RtfStyle* style = m_Document->GetStyleSheet()->GetStyle(0);

		ASSERT_NE(nullptr, style);
		EXPECT_STREQ("Standaard", style->GetName().c_str());

		RtfTextFormat& tf = style->GetTextFormat();
		EXPECT_EQ(Rtf::eCharacterSet_Ansi, tf.GetCharacterSet());
		EXPECT_EQ(Rtf::eCharacterEncoding_SingleByteLowAnsi, tf.GetCharacterEncoding());
		ASSERT_NE(nullptr, tf.GetLocale());
		EXPECT_EQ(Rtf::eCountry_Netherlands, tf.GetLocale()->country);
		EXPECT_EQ(Rtf::eLanguage_Dutch, tf.GetLocale()->language);
		ASSERT_NE(nullptr, tf.GetFont());
		EXPECT_STREQ("Times New Roman", tf.GetFont()->GetName().c_str());
		EXPECT_FLOAT_EQ(8.0f, tf.GetFontSize());
		ASSERT_NE(nullptr, tf.GetBackgroundColor());
		EXPECT_COLOR_RGB(0, 0, 0, *tf.GetBackgroundColor());
		ASSERT_NE(nullptr, tf.GetForegroundColor());
		EXPECT_COLOR_RGB(65, 12, 78, *tf.GetForegroundColor());
		EXPECT_TRUE(tf.GetParagraphWidowControl());
		EXPECT_TRUE(tf.IsKerningEnabled());
		EXPECT_EQ(1, tf.GetMinimumKerningSize());

		RtfAssociatedProperties p0 = style->GetCombinedPropertiesForCharacterEncoding(Rtf::eCharacterEncoding_SingleByteLowAnsi);
		EXPECT_EQ(Rtf::eCharacterEncoding_SingleByteLowAnsi, p0.GetCharacterEncoding());
		ASSERT_NE(nullptr, p0.GetFont());
		EXPECT_STREQ("Times New Roman", p0.GetFont()->GetName().c_str());
		EXPECT_FLOAT_EQ(8.0f, p0.GetFontSize());
		ASSERT_NE(nullptr, p0.GetLocale());
		EXPECT_EQ(Rtf::eCountry_Netherlands, p0.GetLocale()->country);
		EXPECT_EQ(Rtf::eLanguage_Dutch, p0.GetLocale()->language);

		RtfAssociatedProperties p1 = style->GetCombinedPropertiesForCharacterEncoding(Rtf::eCharacterEncoding_SingleByteHighAnsi);
		EXPECT_EQ(Rtf::eCharacterEncoding_SingleByteHighAnsi, p1.GetCharacterEncoding());
		ASSERT_NE(nullptr, p1.GetFont());
		EXPECT_STREQ("Times New Roman", p1.GetFont()->GetName().c_str());
		EXPECT_FLOAT_EQ(8.0f, p1.GetFontSize());
		ASSERT_NE(nullptr, p1.GetLocale());
		EXPECT_EQ(Rtf::eCountry_Netherlands, p1.GetLocale()->country);
		EXPECT_EQ(Rtf::eLanguage_Dutch, p1.GetLocale()->language);

		RtfAssociatedProperties p2 = style->GetCombinedPropertiesForCharacterEncoding(Rtf::eCharacterEncoding_DoubleByte);
		EXPECT_EQ(Rtf::eCharacterEncoding_DoubleByte, p2.GetCharacterEncoding());
		ASSERT_NE(nullptr, p2.GetFont());
		EXPECT_STREQ("Symbol", p2.GetFont()->GetName().c_str());
		EXPECT_FLOAT_EQ(16.0f, p2.GetFontSize());
		ASSERT_NE(nullptr, p2.GetLocale());
		EXPECT_EQ(Rtf::eCountry_India, p2.GetLocale()->country);
		EXPECT_EQ(Rtf::eLanguage_Hindi, p2.GetLocale()->language);
	}

	// style 15
	{
		RtfStyle* style = m_Document->GetStyleSheet()->GetStyle(15);

		ASSERT_NE(nullptr, style);
		EXPECT_STREQ("Kop", style->GetName().c_str());

		RtfTextFormat& tf = style->GetTextFormat();
		EXPECT_EQ(Rtf::eCharacterSet_Ansi, tf.GetCharacterSet());
		EXPECT_EQ(Rtf::eCharacterEncoding_SingleByteLowAnsi, tf.GetCharacterEncoding());
		ASSERT_NE(nullptr, tf.GetLocale());
		EXPECT_EQ(Rtf::eCountry_Netherlands, tf.GetLocale()->country);
		EXPECT_EQ(Rtf::eLanguage_Dutch, tf.GetLocale()->language);
		ASSERT_NE(nullptr, tf.GetFont());
		EXPECT_STREQ("Arial", tf.GetFont()->GetName().c_str());
		EXPECT_FLOAT_EQ(14.0f, tf.GetFontSize());
		ASSERT_NE(nullptr, tf.GetBackgroundColor());
		EXPECT_COLOR_RGB(0, 0, 0, *tf.GetBackgroundColor());
		ASSERT_NE(nullptr, tf.GetForegroundColor());
		EXPECT_COLOR_RGB(65, 12, 78, *tf.GetForegroundColor());
		EXPECT_TRUE(tf.GetParagraphWidowControl());
		EXPECT_TRUE(tf.IsKerningEnabled());
		EXPECT_EQ(1, tf.GetMinimumKerningSize());

		RtfAssociatedProperties p0 = style->GetCombinedPropertiesForCharacterEncoding(Rtf::eCharacterEncoding_SingleByteLowAnsi);
		EXPECT_EQ(Rtf::eCharacterEncoding_SingleByteLowAnsi, p0.GetCharacterEncoding());
		ASSERT_NE(nullptr, p0.GetFont());
		EXPECT_STREQ("Arial", p0.GetFont()->GetName().c_str());
		EXPECT_FLOAT_EQ(14.0f, p0.GetFontSize());
		ASSERT_NE(nullptr, p0.GetLocale());
		EXPECT_EQ(Rtf::eCountry_Netherlands, p0.GetLocale()->country);
		EXPECT_EQ(Rtf::eLanguage_Dutch, p0.GetLocale()->language);

		RtfAssociatedProperties p1 = style->GetCombinedPropertiesForCharacterEncoding(Rtf::eCharacterEncoding_SingleByteHighAnsi);
		EXPECT_EQ(Rtf::eCharacterEncoding_SingleByteHighAnsi, p1.GetCharacterEncoding());
		ASSERT_NE(nullptr, p1.GetFont());
		EXPECT_STREQ("Arial", p1.GetFont()->GetName().c_str());
		EXPECT_FLOAT_EQ(14.0f, p1.GetFontSize());
		ASSERT_NE(nullptr, p1.GetLocale());
		EXPECT_EQ(Rtf::eCountry_Netherlands, p1.GetLocale()->country);
		EXPECT_EQ(Rtf::eLanguage_Dutch, p1.GetLocale()->language);

		RtfAssociatedProperties p2 = style->GetCombinedPropertiesForCharacterEncoding(Rtf::eCharacterEncoding_DoubleByte);
		EXPECT_EQ(Rtf::eCharacterEncoding_DoubleByte, p2.GetCharacterEncoding());
		ASSERT_NE(nullptr, p2.GetFont());
		EXPECT_STREQ("Mangal", p2.GetFont()->GetName().c_str());
		EXPECT_FLOAT_EQ(14.0f, p2.GetFontSize());
		ASSERT_NE(nullptr, p2.GetLocale());
		EXPECT_EQ(Rtf::eCountry_India, p2.GetLocale()->country);
		EXPECT_EQ(Rtf::eLanguage_Hindi, p2.GetLocale()->language);
	}
}

TEST_F(DocumentParsingContext, LibreOfficeBasic)
{
	OpenTestFile("Tests/RTF/LibreOfficeBasic.rtf");

	int i = 0;
}