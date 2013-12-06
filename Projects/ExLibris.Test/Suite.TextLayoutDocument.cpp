#include "ExLibris.Test.PCH.h"

#include <Family.h>
#include <FontLoaderFreetype.h>
#include <Library.h>
#include <TextFormat.h>
#include <TextLayoutCharacter.h>
#include <TextLayoutDocument.h>
#include <TextLayoutLine.h>
#include <TextParserMarkdown.h>

#include "Mock.GlyphProvider.h"

using namespace ExLibris;

class TextLayoutDocumentContext
	: public ::testing::Test
{

public:

	void SetUp()
	{
		library = new Library;

		provider_regular = new MockGlyphProvider(library, "DocumentContext");
		provider_regular->SetWeight(eWeight_Normal);
		provider_regular->SetStyle(eStyle_None);

		provider_bold = new MockGlyphProvider(library, "DocumentContext");
		provider_bold->SetWeight(eWeight_Bold);
		provider_bold->SetStyle(eStyle_None);

		provider_italic = new MockGlyphProvider(library, "DocumentContext");
		provider_italic->SetWeight(eWeight_Normal);
		provider_italic->SetStyle(eStyle_Italicized);

		provider_bolditalic = new MockGlyphProvider(library, "DocumentContext");
		provider_bolditalic->SetWeight(eWeight_Bold);
		provider_bolditalic->SetStyle(eStyle_Italicized);

		format = new TextFormat(library);
		format->SetFamilyName("DocumentContext");
		format->SetSize(12.0f);

		parser = new TextParserMarkdown;

		document = new TextLayoutDocument(library);
		document->SetDefaultTextFormat(*format);
		document->SetParser(parser);
	}

	void TearDown()
	{
		delete document;
		delete format;
		delete library;
	}

protected:

	Library* library;
	MockGlyphProvider* provider_regular;
	MockGlyphProvider* provider_bold;
	MockGlyphProvider* provider_italic;
	MockGlyphProvider* provider_bolditalic;
	TextFormat* format;
	TextLayoutDocument* document;
	TextParserMarkdown* parser;

};

TEST_F(TextLayoutDocumentContext, SetCharacter)
{
	document->SetText("Y");
	document->Layout();

	const std::vector<TextLayoutLine*>& lines = document->GetLines();
	EXPECT_EQ(1, lines.size());

	TextLayoutLine* line = lines[0];
	EXPECT_EQ(1, line->GetChildrenCount());
	EXPECT_VEC2_EQ(2.0f, 2.0f, line->GetElementGeometry().GetMinimum());
	EXPECT_VEC2_EQ(10.0f, 10.0f, line->GetElementGeometry().GetMaximum());
	EXPECT_VEC2_EQ(0.0f, -2.0f, line->GetLayoutGeometry().GetMinimum());
	EXPECT_VEC2_EQ(12.0f, 16.0f, line->GetLayoutGeometry().GetMaximum());

	TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(0));
	ASSERT_NE(nullptr, character);

	EXPECT_EQ('Y', character->GetCodepoint());
	EXPECT_VEC2_EQ(0.0f, 0.0f, character->GetPosition());
	EXPECT_VEC2_EQ(0.0f, 0.0f, character->GetKerningAdjustment());
	EXPECT_VEC2_EQ(2.0f, 2.0f, character->GetElementGeometry().GetMinimum());
	EXPECT_VEC2_EQ(10.0f, 10.0f, character->GetElementGeometry().GetMaximum());
	EXPECT_VEC2_EQ(0.0f, -2.0f, character->GetLayoutGeometry().GetMinimum());
	EXPECT_VEC2_EQ(12.0f, 16.0f, character->GetLayoutGeometry().GetMaximum());
}

TEST(TextLayoutDocument, ParseText)
{
	Library* lib = new Library;
	lib->AddLoader(new FontLoaderFreetype(lib));
	lib->MapFontToFile("Fonts/Roboto/Roboto-Regular.ttf");
	lib->MapFontToFile("Fonts/Roboto/Roboto-Italic.ttf");

	TextLayoutDocument document(lib);
	
	TextFormat tf(lib);
	tf.SetFamilyName("Roboto");
	tf.SetSize(12.0f);
	document.SetDefaultTextFormat(tf);

	document.SetParser(new TextParserMarkdown());

	document.SetText("I'm on a *motorized* vehicle.");

	document.Layout();

	const std::vector<TextLayoutLine*>& lines = document.GetLines();
	for (std::vector<TextLayoutLine*>::const_iterator line_it = lines.begin(); line_it != lines.end(); ++line_it)
	{
		TextLayoutLine* line = *line_it;

		for (size_t index = 0; index < line->GetChildrenCount(); ++index)
		{
			TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(index));

			int i = 0;
		}
	}

	int i = 0;
}