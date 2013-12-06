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
#include "Mock.TextParser.h"

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

		parser = new MockTextParser;
		parser->ignore_input = true;

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
	MockTextParser* parser;

};

TEST_F(TextLayoutDocumentContext, LayoutCharacter)
{
	parser->AddTokenString("Y");
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

TEST_F(TextLayoutDocumentContext, LayoutTwoCharacters)
{
	parser->AddTokenString("H6");
	document->Layout();

	const std::vector<TextLayoutLine*>& lines = document->GetLines();
	EXPECT_EQ(1, lines.size());

	TextLayoutLine* line = lines[0];
	EXPECT_EQ(2, line->GetChildrenCount());
	EXPECT_VEC2_EQ(2.0f, 2.0f, line->GetElementGeometry().GetMinimum());
	EXPECT_VEC2_EQ(2.0f + 12.0f + 8.0f, 10.0f, line->GetElementGeometry().GetMaximum());
	EXPECT_VEC2_EQ(0.0f, -2.0f, line->GetLayoutGeometry().GetMinimum());
	EXPECT_VEC2_EQ(0.0f + 12.0f + -1.5f + 12.0f, 16.0f, line->GetLayoutGeometry().GetMaximum());

	{
		TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(0));
		ASSERT_NE(nullptr, character);
		EXPECT_EQ('H', character->GetCodepoint());
		EXPECT_VEC2_EQ(0.0f, 0.0f, character->GetPosition());
		EXPECT_VEC2_EQ(0.0f, 0.0f, character->GetKerningAdjustment());
	}
	
	{
		TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(1));
		ASSERT_NE(nullptr, character);
		EXPECT_EQ('6', character->GetCodepoint());
		EXPECT_VEC2_EQ(12.0f, 0.0f, character->GetPosition());
		EXPECT_VEC2_EQ(-1.5f, 5.6f, character->GetKerningAdjustment());
	}
}

TEST_F(TextLayoutDocumentContext, FormatCharacterDefault)
{
	parser->AddTokenString("7");
	document->Layout();

	const std::vector<TextLayoutLine*>& lines = document->GetLines();
	ASSERT_EQ(1, lines.size());

	TextLayoutLine* line = lines[0];
	ASSERT_EQ(1, line->GetChildrenCount());

	TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(0));
	ASSERT_NE(nullptr, character);

	TextFormat* format = character->GetTextFormat();
	ASSERT_NE(nullptr, format);
	EXPECT_STREQ("DocumentContext", format->GetFamilyName().c_str());
	EXPECT_FLOAT_EQ(12.0f, format->GetSize());
	EXPECT_EQ(eWeight_Normal, format->GetWeight());
	EXPECT_EQ(eStyle_None, format->GetStyle());
}

TEST_F(TextLayoutDocumentContext, FormatCharacterBold)
{
	{
		TextParserToken token;
		token.changes = TextParserToken::eChanged_Weight;
		token.weight = eWeight_Bold;

		parser->AddTokenStyle(token);
	}
	{
		parser->AddTokenString("Q");
	}

	document->Layout();

	const std::vector<TextLayoutLine*>& lines = document->GetLines();
	ASSERT_EQ(1, lines.size());

	TextLayoutLine* line = lines[0];
	ASSERT_EQ(1, line->GetChildrenCount());

	TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(0));
	ASSERT_NE(nullptr, character);

	TextFormat* format = character->GetTextFormat();
	ASSERT_NE(nullptr, format);
	EXPECT_STREQ("DocumentContext", format->GetFamilyName().c_str());
	EXPECT_FLOAT_EQ(12.0f, format->GetSize());
	EXPECT_EQ(eWeight_Bold, format->GetWeight());
	EXPECT_EQ(eStyle_None, format->GetStyle());
}

TEST_F(TextLayoutDocumentContext, FormatCharacterItalic)
{
	{
		TextParserToken token;
		token.changes = TextParserToken::eChanged_Style;
		token.style = eStyle_Italicized;

		parser->AddTokenStyle(token);
	}
	{
		parser->AddTokenString("n");
	}

	document->Layout();

	const std::vector<TextLayoutLine*>& lines = document->GetLines();
	ASSERT_EQ(1, lines.size());

	TextLayoutLine* line = lines[0];
	ASSERT_EQ(1, line->GetChildrenCount());

	TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(0));
	ASSERT_NE(nullptr, character);

	TextFormat* format = character->GetTextFormat();
	ASSERT_NE(nullptr, format);
	EXPECT_STREQ("DocumentContext", format->GetFamilyName().c_str());
	EXPECT_FLOAT_EQ(12.0f, format->GetSize());
	EXPECT_EQ(eWeight_Normal, format->GetWeight());
	EXPECT_EQ(eStyle_Italicized, format->GetStyle());
}

TEST_F(TextLayoutDocumentContext, FormatCharacterBoldAndItalic)
{
	{
		TextParserToken token;
		token.changes = TextParserToken::eChanged_Weight | TextParserToken::eChanged_Style;
		token.weight = eWeight_Bold;
		token.style = eStyle_Italicized;

		parser->AddTokenStyle(token);
	}
	{
		parser->AddTokenString("n");
	}

	document->Layout();

	const std::vector<TextLayoutLine*>& lines = document->GetLines();
	ASSERT_EQ(1, lines.size());

	TextLayoutLine* line = lines[0];
	ASSERT_EQ(1, line->GetChildrenCount());

	TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(0));
	ASSERT_NE(nullptr, character);

	TextFormat* format = character->GetTextFormat();
	ASSERT_NE(nullptr, format);
	EXPECT_STREQ("DocumentContext", format->GetFamilyName().c_str());
	EXPECT_FLOAT_EQ(12.0f, format->GetSize());
	EXPECT_EQ(eWeight_Bold, format->GetWeight());
	EXPECT_EQ(eStyle_Italicized, format->GetStyle());
}

TEST_F(TextLayoutDocumentContext, FormatBoldAndItalicCharacter)
{
	{
		TextParserToken token;
		token.changes = TextParserToken::eChanged_Weight;
		token.weight = eWeight_Bold;

		parser->AddTokenStyle(token);
	}
	{
		parser->AddTokenString("@");
	}
	{
		TextParserToken token;
		token.changes = TextParserToken::eChanged_Weight | TextParserToken::eChanged_Style;
		token.weight = eWeight_Normal;
		token.style = eStyle_Italicized;

		parser->AddTokenStyle(token);
	}
	{
		parser->AddTokenString("#");
	}

	document->Layout();

	const std::vector<TextLayoutLine*>& lines = document->GetLines();
	ASSERT_EQ(1, lines.size());

	TextLayoutLine* line = lines[0];
	ASSERT_EQ(2, line->GetChildrenCount());

	{
		TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(0));
		ASSERT_NE(nullptr, character);

		TextFormat* format = character->GetTextFormat();
		ASSERT_NE(nullptr, format);
		EXPECT_STREQ("DocumentContext", format->GetFamilyName().c_str());
		EXPECT_FLOAT_EQ(12.0f, format->GetSize());
		EXPECT_EQ(eWeight_Bold, format->GetWeight());
		EXPECT_EQ(eStyle_None, format->GetStyle());
	}

	{
		TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(1));
		ASSERT_NE(nullptr, character);

		TextFormat* format = character->GetTextFormat();
		ASSERT_NE(nullptr, format);
		EXPECT_STREQ("DocumentContext", format->GetFamilyName().c_str());
		EXPECT_FLOAT_EQ(12.0f, format->GetSize());
		EXPECT_EQ(eWeight_Normal, format->GetWeight());
		EXPECT_EQ(eStyle_Italicized, format->GetStyle());
	}
}

TEST_F(TextLayoutDocumentContext, FormatSentence)
{
	{
		parser->AddTokenString("I ");
	}
	{
		TextParserToken token;
		token.changes = TextParserToken::eChanged_Style;
		token.style = eStyle_Italicized;

		parser->AddTokenStyle(token);
	}
	{
		parser->AddTokenString("drove");
	}
	{
		TextParserToken token;
		token.changes = TextParserToken::eChanged_Style;
		token.style = eStyle_None;

		parser->AddTokenStyle(token);
	}
	{
		parser->AddTokenString(" to ");
	}
	{
		TextParserToken token;
		token.changes = TextParserToken::eChanged_Weight;
		token.weight = eWeight_Bold;

		parser->AddTokenStyle(token);
	}
	{
		parser->AddTokenString("you");
	}
	{
		TextParserToken token;
		token.changes = TextParserToken::eChanged_Weight;
		token.weight = eWeight_Normal;

		parser->AddTokenStyle(token);
	}
	document->Layout();

	const std::vector<TextLayoutLine*>& lines = document->GetLines();
	ASSERT_EQ(1, lines.size());

	TextLayoutLine* line = lines[0];
	ASSERT_EQ(1 + 1 + 5 + 1 + 2 + 1 + 3, line->GetChildrenCount());

	{
		TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(0));
		ASSERT_NE(nullptr, character);

		TextFormat* format = character->GetTextFormat();
		ASSERT_NE(nullptr, format);
		EXPECT_STREQ("DocumentContext", format->GetFamilyName().c_str());
		EXPECT_FLOAT_EQ(12.0f, format->GetSize());
		EXPECT_EQ(eWeight_Normal, format->GetWeight());
		EXPECT_EQ(eStyle_None, format->GetStyle());
	}

	{
		TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(2));
		ASSERT_NE(nullptr, character);

		TextFormat* format = character->GetTextFormat();
		ASSERT_NE(nullptr, format);
		EXPECT_STREQ("DocumentContext", format->GetFamilyName().c_str());
		EXPECT_FLOAT_EQ(12.0f, format->GetSize());
		EXPECT_EQ(eWeight_Normal, format->GetWeight());
		EXPECT_EQ(eStyle_Italicized, format->GetStyle());
	}

	{
		TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(7));
		ASSERT_NE(nullptr, character);

		TextFormat* format = character->GetTextFormat();
		ASSERT_NE(nullptr, format);
		EXPECT_STREQ("DocumentContext", format->GetFamilyName().c_str());
		EXPECT_FLOAT_EQ(12.0f, format->GetSize());
		EXPECT_EQ(eWeight_Normal, format->GetWeight());
		EXPECT_EQ(eStyle_None, format->GetStyle());
	}

	{
		TextLayoutCharacter* character = dynamic_cast<TextLayoutCharacter*>(line->GetChildAtIndex(11));
		ASSERT_NE(nullptr, character);

		TextFormat* format = character->GetTextFormat();
		ASSERT_NE(nullptr, format);
		EXPECT_STREQ("DocumentContext", format->GetFamilyName().c_str());
		EXPECT_FLOAT_EQ(12.0f, format->GetSize());
		EXPECT_EQ(eWeight_Bold, format->GetWeight());
		EXPECT_EQ(eStyle_None, format->GetStyle());
	}
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