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
		EXPECT_NO_THROW({
			result = m_Document->ParseFromSource(&m_Input);
		});
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