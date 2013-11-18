#include "ExLibris.Test.PCH.h"

#include <TextLayout.h>

#include "Mock.GlyphProvider.h"
#include "Mock.TextLayoutVisitor.h"

using namespace ExLibris;

// helper functions

#define TEST_GLYPH_ADD(_glyph) { \
	Glyph* glyph = new Glyph; \
	glyph->index = (unsigned int)_glyph; \
	glyph->metrics = new GlyphMetrics; \
	glyph->metrics->advance = (float)_glyph; \
	face->AddGlyph(glyph); \
}

#define TEST_ADD_PROVIDER_GLYPH(_glyph) { \
	provider->use_glyph_metrics = true; \
	GlyphMetrics metrics; \
	metrics.advance = (float)_glyph; \
	provider->glyph_metrics.insert(std::make_pair(_glyph, metrics)); \
}

#define TEST_ADD_PROVIDER_KERNINGPAIR(_current, _next, _adjustment) { \
	provider->use_kerning_pairs = true; \
	MockGlyphProvider::KerningPair pair; \
	pair.index_left = _current; \
	pair.index_right = _next; \
	pair.adjustment = _adjustment; \
	provider->kerning_pairs.push_back(pair); \
}

Face* CreateFixedWidthFace()
{
	MockGlyphProvider* provider = new MockGlyphProvider(nullptr);

	provider->use_font_metrics = true;
	provider->font_metrics.line_height = 20.0f;

	TEST_ADD_PROVIDER_GLYPH('?');

	return provider->CreateFace(20.0f);
}

Face* CreateDynamicWidthFace()
{
	MockGlyphProvider* provider = new MockGlyphProvider(nullptr);

	provider->use_font_metrics = true;
	provider->font_metrics.line_height = 20.0f;

	TEST_ADD_PROVIDER_GLYPH('A');
	TEST_ADD_PROVIDER_GLYPH('B');
	TEST_ADD_PROVIDER_GLYPH('C');
	TEST_ADD_PROVIDER_GLYPH('D');
	TEST_ADD_PROVIDER_GLYPH('E');
	TEST_ADD_PROVIDER_GLYPH('F');
	TEST_ADD_PROVIDER_GLYPH('G');
	TEST_ADD_PROVIDER_GLYPH('H');
	TEST_ADD_PROVIDER_GLYPH('I');
	TEST_ADD_PROVIDER_GLYPH('J');
	TEST_ADD_PROVIDER_GLYPH('K');
	TEST_ADD_PROVIDER_GLYPH('L');
	TEST_ADD_PROVIDER_GLYPH('M');
	TEST_ADD_PROVIDER_GLYPH('N');
	TEST_ADD_PROVIDER_GLYPH('O');
	TEST_ADD_PROVIDER_GLYPH('P');
	TEST_ADD_PROVIDER_GLYPH('Q');
	TEST_ADD_PROVIDER_GLYPH('R');
	TEST_ADD_PROVIDER_GLYPH('S');
	TEST_ADD_PROVIDER_GLYPH('T');
	TEST_ADD_PROVIDER_GLYPH('U');
	TEST_ADD_PROVIDER_GLYPH('V');
	TEST_ADD_PROVIDER_GLYPH('W');
	TEST_ADD_PROVIDER_GLYPH('X');
	TEST_ADD_PROVIDER_GLYPH('Y');
	TEST_ADD_PROVIDER_GLYPH('Z');
	TEST_ADD_PROVIDER_GLYPH(' ');

	return provider->CreateFace(20.0f);
}

Face* CreateKerningFace()
{
	MockGlyphProvider* provider = new MockGlyphProvider(nullptr);

	provider->use_font_metrics = true;
	provider->font_metrics.line_height = 20.0f;

	TEST_ADD_PROVIDER_GLYPH('x');
	TEST_ADD_PROVIDER_GLYPH('y');
	TEST_ADD_PROVIDER_GLYPH('z');

	TEST_ADD_PROVIDER_KERNINGPAIR('x', 'x', glm::vec2(2.0f, 0.0f));
	TEST_ADD_PROVIDER_KERNINGPAIR('x', 'y', glm::vec2(-4.0f, 0.0f));
	TEST_ADD_PROVIDER_KERNINGPAIR('y', 'x', glm::vec2(5.0f, 0.0f));

	return provider->CreateFace(20.0f);
}

int GetTextGlyphWidth(const std::string& a_Text)
{
	int width = 0;

	for (std::string::const_iterator text_it = a_Text.begin(); text_it != a_Text.end(); ++text_it)
	{
		width += (int)*text_it;
	}

	return width;
}

// unit tests

TEST(TextLayout, Construct)
{
	TextLayout layout;

	layout.Layout();

	EXPECT_EQ(nullptr, layout.GetFace());
	EXPECT_EQ(0, layout.GetText().length());
	EXPECT_FLOAT_EQ(0.0f, layout.GetDimensions().x);
	EXPECT_FLOAT_EQ(0.0f, layout.GetDimensions().y);
	EXPECT_FLOAT_EQ(0.0f, layout.GetSizeHint().x);
	EXPECT_FLOAT_EQ(0.0f, layout.GetSizeHint().y);
}

TEST(TextLayout, SetFace)
{
	TextLayout layout;

	MockGlyphProvider provider(nullptr);
	Face* face = provider.CreateFace(0.0f);

	layout.SetFace(face);

	EXPECT_EQ(face, layout.GetFace());
}

TEST(TextLayout, SetText)
{
	TextLayout layout;

	layout.SetText("Read a book, dummy");

	layout.Layout();

	EXPECT_STREQ("Read a book, dummy", layout.GetText().c_str());
	EXPECT_FLOAT_EQ(0.0f, layout.GetDimensions().x);
	EXPECT_FLOAT_EQ(0.0f, layout.GetDimensions().y);
}

TEST(TextLayout, SetSizeHint)
{
	TextLayout layout;

	layout.SetSizeHint(glm::vec2(155.0f, 32.0f));

	EXPECT_FLOAT_EQ(155.0f, layout.GetSizeHint().x);
	EXPECT_FLOAT_EQ(32.0f, layout.GetSizeHint().y);
}

TEST(TextLayout, DimensionsEmpty)
{
	TextLayout layout;
	layout.Layout();

	EXPECT_FLOAT_EQ(0.0f, layout.GetDimensions().x);
	EXPECT_FLOAT_EQ(0.0f, layout.GetDimensions().y);
}

TEST(TextLayout, DimensionsGlyphsUnknown)
{
	TextLayout layout;
	layout.SetFace(CreateFixedWidthFace());

	layout.SetText("Unknown glyphs.");

	layout.Layout();

	EXPECT_FLOAT_EQ(0.0f, layout.GetDimensions().x);
	EXPECT_FLOAT_EQ(20.0f, layout.GetDimensions().y);
}

TEST(TextLayout, DimensionsFixed)
{
	TextLayout layout;
	layout.SetFace(CreateFixedWidthFace());

	layout.SetText("??????");

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"??????", visitor.lines[0]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);

	EXPECT_VEC2_EQ(layout_width, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsFixedTextBeforeFont)
{
	TextLayout layout;

	layout.SetText("???");

	layout.SetFace(CreateFixedWidthFace());

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"???", visitor.lines[0]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);

	EXPECT_VEC2_EQ(layout_width, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsFixedResize)
{
	TextLayout layout;
	layout.SetFace(CreateFixedWidthFace());

	layout.SetText("????????");
	layout.SetText("??");

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"??", visitor.lines[0]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);

	EXPECT_VEC2_EQ(layout_width, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsDynamic)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("HAPPY");

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"HAPPY", visitor.lines[0]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);

	EXPECT_VEC2_EQ(layout_width, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsDynamicTextBeforeFont)
{
	TextLayout layout;

	layout.SetText("HOTDOGGITY");

	layout.SetFace(CreateDynamicWidthFace());

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"HOTDOGGITY", visitor.lines[0]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);

	EXPECT_VEC2_EQ(layout_width, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsDynamicResize)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("MAGNIFICENT");
	layout.SetText("RIDICULOUS");

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"RIDICULOUS", visitor.lines[0]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);

	EXPECT_VEC2_EQ(layout_width, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsKerning)
{
	Face* face = CreateKerningFace();

	TextLayout layout;
	layout.SetFace(face);

	layout.SetText("zxy");

	layout.Layout();

	EXPECT_VEC2_EQ(10.0f + 0.0f + 10.0f + -4.0f + 10.0f, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsNoKerning)
{
	Face* face = CreateKerningFace();

	TextLayout layout;
	layout.SetFace(face);

	layout.SetText("zzzzz");

	layout.Layout();

	EXPECT_VEC2_EQ(10.0f + 0.0f + 10.0f + 0.0f + 10.0f + 0.0f + 10.0f + 0.0f + 10.0f, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsNewLine)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("GOODNESS ME \nANOTHER LINE");

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"GOODNESS ME ", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"ANOTHER LINE", visitor.lines[1]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);

	EXPECT_VEC2_EQ(layout_width, 40.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsDoubleNewLine)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("NEUTRAL\n\nREVERSE");

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(3, visitor.lines.size());
	EXPECT_STREQ(L"NEUTRAL", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"", visitor.lines[1]->text.c_str());
	EXPECT_STREQ(L"REVERSE", visitor.lines[2]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[2]->width);

	EXPECT_VEC2_EQ(layout_width, 60.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsCarriageReturn)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BALL\rDOG");

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"BALL", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"DOG", visitor.lines[1]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);

	EXPECT_VEC2_EQ(layout_width, 40.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsDoubleCarriageReturn)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BRING US THE GIRL\r\rAND WIPE AWAY THE DEBT");

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(3, visitor.lines.size());
	EXPECT_STREQ(L"BRING US THE GIRL", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"", visitor.lines[1]->text.c_str());
	EXPECT_STREQ(L"AND WIPE AWAY THE DEBT", visitor.lines[2]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[2]->width);

	EXPECT_VEC2_EQ(layout_width, 60.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsUnixNewLine)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("AND WHAT IS THIS\r\nOH WHAT A TREAT");

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"AND WHAT IS THIS", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"OH WHAT A TREAT", visitor.lines[1]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);

	EXPECT_VEC2_EQ(layout_width, 40.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsTripleUnixNewLine)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("TRAIN\r\nWILL DEPART FROM PLATFORM\r\n\r\n\r\nUNKNOWN");

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(5, visitor.lines.size());
	EXPECT_STREQ(L"TRAIN", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"WILL DEPART FROM PLATFORM", visitor.lines[1]->text.c_str());
	EXPECT_STREQ(L"", visitor.lines[2]->text.c_str());
	EXPECT_STREQ(L"", visitor.lines[3]->text.c_str());
	EXPECT_STREQ(L"UNKNOWN", visitor.lines[4]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[2]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[3]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[4]->width);

	EXPECT_VEC2_EQ(layout_width, 100.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsMixedNewLineTypes)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("CONNECT\r\nTO MY\nFACE");

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(3, visitor.lines.size());
	EXPECT_STREQ(L"CONNECT", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"TO MY", visitor.lines[1]->text.c_str());
	EXPECT_STREQ(L"FACE", visitor.lines[2]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[2]->width);

	EXPECT_VEC2_EQ(layout_width, 60.0f, layout.GetDimensions());
}

TEST(TextLayout, DimensionsKerningNewLine)
{
	Face* face = CreateKerningFace();

	TextLayout layout;
	layout.SetFace(face);

	layout.SetText("yzyx\nxxyyz");

	layout.Layout();

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, 10.0f + 0.0f + 10.0f + 0.0f + 10.0f + 5.0f + 10.0f + 0.0f);
	layout_width = std::max<float>(layout_width, 10.0f + 2.0f + 10.0f + -4.0f + 10.0f + 0.0f + 10.0f + 0.0f + 10.0f);

	EXPECT_VEC2_EQ(layout_width, 40.0f, layout.GetDimensions());
}

TEST(TextLayout, AcceptVisitorEmpty)
{
	MockTextLayoutVisitor visitor;

	TextLayout layout;

	layout.Accept(visitor);

	ASSERT_EQ(0, visitor.lines.size());

	ASSERT_EQ(0, visitor.glyphs.size());
}

TEST(TextLayout, AcceptVisitorFont)
{
	MockTextLayoutVisitor visitor;

	TextLayout layout;
	layout.SetFace(CreateFixedWidthFace());

	layout.Accept(visitor);

	ASSERT_EQ(0, visitor.lines.size());
}

TEST(TextLayout, AcceptVisitorTextNoFont)
{
	MockTextLayoutVisitor visitor;

	TextLayout layout;
	layout.SetText("Hi how are ya?");

	layout.Accept(visitor);

	ASSERT_EQ(0, visitor.lines.size());

	ASSERT_EQ(0, visitor.glyphs.size());
}

TEST(TextLayout, AcceptVisitorOneGlyph)
{
	MockTextLayoutVisitor visitor;

	TextLayout layout;
	layout.SetFace(CreateFixedWidthFace());
	layout.SetText("?");

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
	EXPECT_EQ(63, visitor.lines[0]->width);

	ASSERT_EQ(1, visitor.glyphs.size());
	EXPECT_NE(nullptr, visitor.glyphs[0]->metrics);
	EXPECT_FLOAT_EQ(0.0f, visitor.glyphs[0]->x);
	EXPECT_FLOAT_EQ(63.0f, visitor.glyphs[0]->advance);
}

TEST(TextLayout, AcceptVisitorFixedFontText)
{
	MockTextLayoutVisitor visitor;

	TextLayout layout;
	layout.SetFace(CreateFixedWidthFace());
	layout.SetText("???");

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
	EXPECT_EQ(189, visitor.lines[0]->width);

	ASSERT_EQ(3, visitor.glyphs.size());
	EXPECT_NE(nullptr, visitor.glyphs[0]->metrics);
	EXPECT_FLOAT_EQ(0.0f, visitor.glyphs[0]->x);
	EXPECT_FLOAT_EQ(63.0f, visitor.glyphs[0]->advance);
	EXPECT_NE(nullptr, visitor.glyphs[1]->metrics);
	EXPECT_FLOAT_EQ(63.0f, visitor.glyphs[1]->x);
	EXPECT_FLOAT_EQ(63.0f, visitor.glyphs[1]->advance);
	EXPECT_NE(nullptr, visitor.glyphs[2]->metrics);
	EXPECT_FLOAT_EQ(126.0f, visitor.glyphs[2]->x);
	EXPECT_FLOAT_EQ(63.0f, visitor.glyphs[2]->advance);
}

TEST(TextLayout, AcceptVisitorDynamicFontText)
{
	MockTextLayoutVisitor visitor;

	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());
	layout.SetText("ACK");

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
	EXPECT_EQ(GetTextGlyphWidth("ACK"), visitor.lines[0]->width);

	ASSERT_EQ(3, visitor.glyphs.size());
	EXPECT_NE(nullptr, visitor.glyphs[0]->metrics);
	EXPECT_FLOAT_EQ(0.0f, visitor.glyphs[0]->x);
	EXPECT_FLOAT_EQ(65.0f, visitor.glyphs[0]->advance);
	EXPECT_NE(nullptr, visitor.glyphs[1]->metrics);
	EXPECT_FLOAT_EQ(65.0f, visitor.glyphs[1]->x);
	EXPECT_FLOAT_EQ(67.0f, visitor.glyphs[1]->advance);
	EXPECT_NE(nullptr, visitor.glyphs[2]->metrics);
	EXPECT_FLOAT_EQ(132.0f, visitor.glyphs[2]->x);
	EXPECT_FLOAT_EQ(75.0f, visitor.glyphs[2]->advance);
}

TEST(TextLayout, AcceptVisitorKerningFontText)
{
	MockTextLayoutVisitor visitor;

	TextLayout layout;
	layout.SetFace(CreateKerningFace());
	layout.SetText("zzxy");

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
	EXPECT_FLOAT_EQ(10.0f + 0.0f + 10.0f + 0.0f + 10.0f + -4.0f + 10.0f, visitor.lines[0]->width);

	ASSERT_EQ(4, visitor.glyphs.size());
	EXPECT_NE(nullptr, visitor.glyphs[0]->metrics);
	EXPECT_FLOAT_EQ(0.0f, visitor.glyphs[0]->x);
	EXPECT_FLOAT_EQ(10.0f, visitor.glyphs[0]->advance);
	EXPECT_NE(nullptr, visitor.glyphs[1]->metrics);
	EXPECT_FLOAT_EQ(10.0f, visitor.glyphs[1]->x);
	EXPECT_FLOAT_EQ(10.0f, visitor.glyphs[1]->advance);
	EXPECT_NE(nullptr, visitor.glyphs[2]->metrics);
	EXPECT_FLOAT_EQ(20.0f, visitor.glyphs[2]->x);
	EXPECT_FLOAT_EQ(6.0f, visitor.glyphs[2]->advance);
	EXPECT_NE(nullptr, visitor.glyphs[3]->metrics);
	EXPECT_FLOAT_EQ(26.0f, visitor.glyphs[3]->x);
	EXPECT_FLOAT_EQ(10.0f, visitor.glyphs[3]->advance);
}

TEST(TextLayout, LimitsHorizontalIgnoreVerticalIgnore)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("THIS IS YOUR CAPTAIN SPEAKING");

	layout.SetHorizontalSizePolicy(eSizePolicy_Ignore);
	layout.SetVerticalSizePolicy(eSizePolicy_Ignore);

	layout.SetSizeHint(glm::vec2(112.0f, 334.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"THIS IS YOUR CAPTAIN SPEAKING", visitor.lines[0]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);

	EXPECT_VEC2_EQ(layout_width, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalIgnoreVerticalFixed)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("CRASH LANDINGS ARE NOT PART OF THIS EXERCISE");

	layout.SetHorizontalSizePolicy(eSizePolicy_Ignore);
	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(0.0f, 335.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"CRASH LANDINGS ARE NOT PART OF THIS EXERCISE", visitor.lines[0]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);

	EXPECT_VEC2_EQ(layout_width, 335.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalIgnoreVerticalFixedTooSmall)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BRING ME YOUR FINEST GREEN WOMEN");

	layout.SetHorizontalSizePolicy(eSizePolicy_Ignore);
	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(0.0f, 16.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(0, visitor.lines.size());

	float layout_width = 0.0f;
	
	EXPECT_VEC2_EQ(layout_width, 16.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalIgnoreVerticalFixedTooLarge)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("WHY BUILD ONE\nWHEN YOU CAN BUILD\nONE MILLION DEATH RAYS");

	layout.SetHorizontalSizePolicy(eSizePolicy_Ignore);
	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(0.0f, 44.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"WHY BUILD ONE", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"WHEN YOU CAN BUILD", visitor.lines[1]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);

	EXPECT_VEC2_EQ(layout_width, 44.0f, layout.GetDimensions());
}


TEST(TextLayout, LimitsHorizontalIgnoreVerticalFixedNegative)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BRACE FOR IMPACT WITH FROZEN MEATBALL");

	layout.SetHorizontalSizePolicy(eSizePolicy_Ignore);
	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(0.0f, -33.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(0, visitor.lines.size());

	float layout_width = 0.0f;

	EXPECT_VEC2_EQ(layout_width, 0.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalIgnoreVerticalMinimumExpanding)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("ENTERPRISE");

	layout.SetHorizontalSizePolicy(eSizePolicy_Ignore);
	layout.SetVerticalSizePolicy(eSizePolicy_MinimumExpanding);

	layout.SetSizeHint(glm::vec2(100.0f, 24.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"ENTERPRISE", visitor.lines[0]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);

	EXPECT_VEC2_EQ(layout_width, 24.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalIgnoreVerticalMinimumExpandingTwoLines)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BEAM ME UP\nTECHNICIAN GUY");

	layout.SetHorizontalSizePolicy(eSizePolicy_Ignore);
	layout.SetVerticalSizePolicy(eSizePolicy_MinimumExpanding);

	layout.SetSizeHint(glm::vec2(-50.0f, 15.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"BEAM ME UP", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"TECHNICIAN GUY", visitor.lines[1]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);

	EXPECT_VEC2_EQ(layout_width, 40.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalIgnoreVerticalMinimumExpandingNegative)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("TAKE ME TO VENICE\nI WANT TO SEE THE BOATS");

	layout.SetHorizontalSizePolicy(eSizePolicy_Ignore);
	layout.SetVerticalSizePolicy(eSizePolicy_MinimumExpanding);

	layout.SetSizeHint(glm::vec2(500.0f, -30.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"TAKE ME TO VENICE", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"I WANT TO SEE THE BOATS", visitor.lines[1]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);

	EXPECT_VEC2_EQ(layout_width, 40.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalIgnoreVerticalMaximum)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("TO SPACE AND BEYOND\nMY MOTHERS BASEMENT");

	layout.SetHorizontalSizePolicy(eSizePolicy_Ignore);
	layout.SetVerticalSizePolicy(eSizePolicy_Maximum);

	layout.SetSizeHint(glm::vec2(116.0f, 56.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"TO SPACE AND BEYOND", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"MY MOTHERS BASEMENT", visitor.lines[1]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);

	EXPECT_VEC2_EQ(layout_width, 40.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalIgnoreVerticalMaximumThreeLines)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("YOUR PLANS CAN BE FOILED\nSIMPLY BY APPLYING SOME\nORDINARY TINFOIL");

	layout.SetHorizontalSizePolicy(eSizePolicy_Ignore);
	layout.SetVerticalSizePolicy(eSizePolicy_Maximum);

	layout.SetSizeHint(glm::vec2(1868.0f, 43.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"YOUR PLANS CAN BE FOILED", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"SIMPLY BY APPLYING SOME", visitor.lines[1]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);

	EXPECT_VEC2_EQ(layout_width, 40.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalIgnoreVerticalMaximumNegative)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("SUCK ON MY\nTOOTSIE ROLL");

	layout.SetHorizontalSizePolicy(eSizePolicy_Ignore);
	layout.SetVerticalSizePolicy(eSizePolicy_Maximum);

	layout.SetSizeHint(glm::vec2(107.0f, -88.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(0, visitor.lines.size());

	EXPECT_VEC2_EQ(0.0f, 0.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalFixed)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BRING ME A BANANA");

	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);
	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(1120.0f, 20.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"BRING ME A BANANA", visitor.lines[0]->text.c_str());

	EXPECT_VEC2_EQ(1120.0f, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalFixedTooSmall)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("MY GOD IT EVEN HAS A WATERMARK");

	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);
	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(620.0f, 20.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"MY GOD IT ", visitor.lines[0]->text.c_str());

	EXPECT_VEC2_EQ(620.0f, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalFixedTooLarge)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BRING ON THE NEWSPAPERS");

	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);
	layout.SetVerticalSizePolicy(eSizePolicy_Ignore);

	layout.SetSizeHint(glm::vec2(3300.0f, 66.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"BRING ON THE NEWSPAPERS", visitor.lines[0]->text.c_str());

	EXPECT_VEC2_EQ(3300.0, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalFixedNegative)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("TRULY A FEAST ON YOUR EYEBALLS");

	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);
	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(-150.0f, -15.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(0, visitor.lines.size());

	EXPECT_VEC2_EQ(0.0f, 0.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalMinimumExpandingVerticalIgnore)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("FINE I WILL GET MY OWN STARSHIP");

	layout.SetHorizontalSizePolicy(eSizePolicy_MinimumExpanding);
	layout.SetVerticalSizePolicy(eSizePolicy_Ignore);

	layout.SetSizeHint(glm::vec2(12.0f, 35.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"FINE I WILL GET MY OWN STARSHIP", visitor.lines[0]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);

	EXPECT_VEC2_EQ(layout_width, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalMinimumExpandingVerticalIgnoreExpandToMinimum)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("SET PHASERS TO AWESOME");

	layout.SetHorizontalSizePolicy(eSizePolicy_MinimumExpanding);
	layout.SetVerticalSizePolicy(eSizePolicy_Ignore);

	layout.SetSizeHint(glm::vec2(1800.0f, 35.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"SET PHASERS TO AWESOME", visitor.lines[0]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);

	EXPECT_VEC2_EQ(1800.0f, 20.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalMinimumExpandingVerticalMinimumExpanding)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BRING YOUR OWN\nPARTY HAT");

	layout.SetHorizontalSizePolicy(eSizePolicy_MinimumExpanding);
	layout.SetVerticalSizePolicy(eSizePolicy_MinimumExpanding);

	layout.SetSizeHint(glm::vec2(16.0f, 50.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"BRING YOUR OWN", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"PARTY HAT", visitor.lines[1]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);

	EXPECT_VEC2_EQ(layout_width, 50.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalMinimumExpandingVerticalMaximum)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BRING YOUR OWN\nPARTY HAT");

	layout.SetHorizontalSizePolicy(eSizePolicy_MinimumExpanding);
	layout.SetVerticalSizePolicy(eSizePolicy_MinimumExpanding);

	layout.SetSizeHint(glm::vec2(16.0f, 50.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"BRING YOUR OWN", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"PARTY HAT", visitor.lines[1]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);

	EXPECT_VEC2_EQ(layout_width, 50.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalMaximumVerticalIgnore)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("SUCK MY FRUITY PIE");

	layout.SetHorizontalSizePolicy(eSizePolicy_Maximum);
	layout.SetVerticalSizePolicy(eSizePolicy_Ignore);

	layout.SetSizeHint(glm::vec2(610.0f, 116.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(3, visitor.lines.size());
	EXPECT_STREQ(L"SUCK MY ", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"FRUITY ", visitor.lines[1]->text.c_str());
	EXPECT_STREQ(L"PIE", visitor.lines[2]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[2]->width);

	EXPECT_VEC2_EQ(layout_width, 60.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalMaximumVerticalMinimumExpanding)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("DRIVE YOUR AUNT OFF A CLIFF");

	layout.SetHorizontalSizePolicy(eSizePolicy_Maximum);
	layout.SetVerticalSizePolicy(eSizePolicy_MinimumExpanding);

	layout.SetSizeHint(glm::vec2(700.0f, 40.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(3, visitor.lines.size());
	EXPECT_STREQ(L"DRIVE YOUR ", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"AUNT OFF A ", visitor.lines[1]->text.c_str());
	EXPECT_STREQ(L"CLIFF", visitor.lines[2]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[2]->width);

	EXPECT_VEC2_EQ(layout_width, 60.0f, layout.GetDimensions());
}

TEST(TextLayout, LimitsHorizontalMaximumVerticalMaximum)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("SINK INTO THE DEEPEST OCEAN");

	layout.SetHorizontalSizePolicy(eSizePolicy_Maximum);
	layout.SetVerticalSizePolicy(eSizePolicy_Maximum);

	layout.SetSizeHint(glm::vec2(821.0f, 41.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"SINK INTO ", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"THE DEEPEST ", visitor.lines[1]->text.c_str());

	float layout_width = 0.0f;
	layout_width = std::max<float>(layout_width, visitor.lines[0]->width);
	layout_width = std::max<float>(layout_width, visitor.lines[1]->width);

	EXPECT_VEC2_EQ(layout_width, 40.0f, layout.GetDimensions());
}

TEST(TextLayout, LayoutVerticalTop)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("JEWEL OF MY CROWN");

	layout.SetVerticalAlignment(eVerticalAlignment_Top);

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"JEWEL OF MY CROWN", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
}

TEST(TextLayout, LayoutVerticalTopTallBox)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BRING YOUR OWN LASERS");

	layout.SetVerticalAlignment(eVerticalAlignment_Top);

	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(0.0f, 150.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"BRING YOUR OWN LASERS", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
}

TEST(TextLayout, LayoutVerticalTopTallBoxTwoLines)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("THE SASS IS ALMOST\nPALPATABLE");

	layout.SetVerticalAlignment(eVerticalAlignment_Top);

	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(0.0f, 240.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"THE SASS IS ALMOST", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
	EXPECT_STREQ(L"PALPATABLE", visitor.lines[1]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 20.0f, visitor.lines[1]->offset);
}

TEST(TextLayout, LayoutVerticalMiddle)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BRING HER UP TO LUDRICOUS SPEED");

	layout.SetVerticalAlignment(eVerticalAlignment_Middle);

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"BRING HER UP TO LUDRICOUS SPEED", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
}

TEST(TextLayout, LayoutVerticalMiddleTallBox)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("DO ANDROIDS DREAM OF ELECTRIC KEBAB");

	layout.SetVerticalAlignment(eVerticalAlignment_Middle);

	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(0.0f, 153.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"DO ANDROIDS DREAM OF ELECTRIC KEBAB", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 66.5f, visitor.lines[0]->offset);
}

TEST(TextLayout, LayoutVerticalMiddleTallBoxThreeLines)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("THERE ARE\nFIVE LIGHTS\nNO WAIT");

	layout.SetVerticalAlignment(eVerticalAlignment_Middle);

	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(0.0f, 144.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(3, visitor.lines.size());
	EXPECT_STREQ(L"THERE ARE", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 42.0f, visitor.lines[0]->offset);
	EXPECT_STREQ(L"FIVE LIGHTS", visitor.lines[1]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 62.0f, visitor.lines[1]->offset);
	EXPECT_STREQ(L"NO WAIT", visitor.lines[2]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 82.0f, visitor.lines[2]->offset);
}

TEST(TextLayout, LayoutVerticalBottom)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("DODGE THAT ASTEROID PLEASE");

	layout.SetVerticalAlignment(eVerticalAlignment_Bottom);

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"DODGE THAT ASTEROID PLEASE", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
}

TEST(TextLayout, LayoutVerticalBottomTallBox)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("WHAT IS IT WITH YOU AND ROMULAN ALE");

	layout.SetVerticalAlignment(eVerticalAlignment_Bottom);

	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(0.0f, 64.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"WHAT IS IT WITH YOU AND ROMULAN ALE", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 44.0f, visitor.lines[0]->offset);
}

TEST(TextLayout, LayoutVerticalBottomTallBoxTwoLines)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("YOU ARE GOING TO BE FINE\nYOU STILL HAVE MOST OF YOUR VITAL ORGANS");

	layout.SetVerticalAlignment(eVerticalAlignment_Bottom);

	layout.SetVerticalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(0.0f, 500.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"YOU ARE GOING TO BE FINE", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 460.0f, visitor.lines[0]->offset);
	EXPECT_STREQ(L"YOU STILL HAVE MOST OF YOUR VITAL ORGANS", visitor.lines[1]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 480.0f, visitor.lines[1]->offset);
}

TEST(TextLayout, LayoutHorizontalLeft)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("LUNCH HOUR I REPEAT LUNCH HOUR");

	layout.SetHorizontalAlignment(eHorizontalAlignment_Left);

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"LUNCH HOUR I REPEAT LUNCH HOUR", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
}

TEST(TextLayout, LayoutHorizontalLeftWideBox)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("ANOTHER DAY ANOTHER RAVIOLI SANDWICH");

	layout.SetHorizontalAlignment(eHorizontalAlignment_Left);

	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(2800.0f, 113.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"ANOTHER DAY ANOTHER RAVIOLI SANDWICH", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
}

TEST(TextLayout, LayoutHorizontalLeftWideBoxTwoLines)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("WE ARE OUT OF MAYONAISSE\nFIGURES");

	layout.SetHorizontalAlignment(eHorizontalAlignment_Left);

	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(2734.0f, 124.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"WE ARE OUT OF MAYONAISSE", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
	EXPECT_STREQ(L"FIGURES", visitor.lines[1]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 20.0f, visitor.lines[1]->offset);
}

TEST(TextLayout, LayoutHorizontalMiddle)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BRING YOUR KID TO WORK THEY SAID");

	layout.SetHorizontalAlignment(eHorizontalAlignment_Center);

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"BRING YOUR KID TO WORK THEY SAID", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
}

TEST(TextLayout, LayoutHorizontalMiddleWideBox)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("SHE WONT GET SNAPPED IN HALF BY A SEA MONSTER THEY SAID");

	layout.SetHorizontalAlignment(eHorizontalAlignment_Center);

	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(15000.0f, 113.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"SHE WONT GET SNAPPED IN HALF BY A SEA MONSTER THEY SAID", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(5661.5f, 0.0f, visitor.lines[0]->offset);
}

TEST(TextLayout, LayoutHorizontalMiddleWideBoxTwoLines)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("OH IM SURE WE CAN PUT THE PIECES BACK TOGETHER\nIF WE CAN FIND ENOUGH PIECES THAT IS");

	layout.SetHorizontalAlignment(eHorizontalAlignment_Center);

	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(3455.0f, 55.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"OH IM SURE WE CAN PUT THE PIECES BACK TOGETHER", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(191.5f, 0.0f, visitor.lines[0]->offset);
	EXPECT_STREQ(L"IF WE CAN FIND ENOUGH PIECES THAT IS", visitor.lines[1]->text.c_str());
	EXPECT_VEC2_EQ(538.5f, 20.0f, visitor.lines[1]->offset);
}

TEST(TextLayout, LayoutHorizontalRight)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("THIS MONSTER IS NOT GOING BACK IN ITS BOX");

	layout.SetHorizontalAlignment(eHorizontalAlignment_Right);

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"THIS MONSTER IS NOT GOING BACK IN ITS BOX", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(0.0f, 0.0f, visitor.lines[0]->offset);
}

TEST(TextLayout, LayoutHorizontalRightWideBox)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("OUR NEW NEIGHBOUR HAS TOO MANY TENTACLES");

	layout.SetHorizontalAlignment(eHorizontalAlignment_Right);

	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(10477.0f, 234.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(1, visitor.lines.size());
	EXPECT_STREQ(L"OUR NEW NEIGHBOUR HAS TOO MANY TENTACLES", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(7684.0f, 0.0f, visitor.lines[0]->offset);
}

TEST(TextLayout, LayoutHorizontalRightWideBoxTwoLines)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BRING THE SPAGHETTI\nAND FIGHT IT OUT LIKE MEN");

	layout.SetHorizontalAlignment(eHorizontalAlignment_Right);

	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);

	layout.SetSizeHint(glm::vec2(6023.0f, 182.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(2, visitor.lines.size());
	EXPECT_STREQ(L"BRING THE SPAGHETTI", visitor.lines[0]->text.c_str());
	EXPECT_VEC2_EQ(4683.0f, 0.0f, visitor.lines[0]->offset);
	EXPECT_STREQ(L"AND FIGHT IT OUT LIKE MEN", visitor.lines[1]->text.c_str());
	EXPECT_VEC2_EQ(4360.0f, 20.0f, visitor.lines[1]->offset);
}

TEST(TextLayout, WordWrappingSpaces)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("BANANA BANANA      BANANA BANANA BANANA");

	layout.SetWordWrapping(TextLayout::eWordWrapping_Greedy);
	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);
	layout.SetSizeHint(glm::vec2(1080.0f, 33.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(3, visitor.lines.size());
	EXPECT_STREQ(L"BANANA BANANA      ", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"BANANA BANANA ", visitor.lines[1]->text.c_str());
	EXPECT_STREQ(L"BANANA", visitor.lines[2]->text.c_str());
}

TEST(TextLayout, WordWrappingSentence)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("THERE ARE NO WORDS AMONG THE STARS");

	layout.SetWordWrapping(TextLayout::eWordWrapping_Greedy);
	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);
	layout.SetSizeHint(glm::vec2(920.0f, 33.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(3, visitor.lines.size());
	EXPECT_STREQ(L"THERE ARE NO ", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"WORDS AMONG ", visitor.lines[1]->text.c_str());
	EXPECT_STREQ(L"THE STARS", visitor.lines[2]->text.c_str());
}

TEST(TextLayout, WordWrappingLongWord)
{
	TextLayout layout;
	layout.SetFace(CreateDynamicWidthFace());

	layout.SetText("RIOOLWATERZUIVERINGSINSTALLATIESYSTEEM");

	layout.SetWordWrapping(TextLayout::eWordWrapping_Greedy);
	layout.SetHorizontalSizePolicy(eSizePolicy_Fixed);
	layout.SetSizeHint(glm::vec2(1168.0f, 33.0f));

	layout.Layout();

	MockTextLayoutVisitor visitor;

	layout.Accept(visitor);

	ASSERT_EQ(3, visitor.lines.size());
	EXPECT_STREQ(L"RIOOLWATERZUIV", visitor.lines[0]->text.c_str());
	EXPECT_STREQ(L"ERINGSINSTALLAT", visitor.lines[1]->text.c_str());
	EXPECT_STREQ(L"IESYSTEEM", visitor.lines[2]->text.c_str());
}