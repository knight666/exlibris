#include "ExLibris.Test.PCH.h"

#include <RtfDomDocument.h>

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