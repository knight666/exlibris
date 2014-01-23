#include "ExLibris.Test.PCH.h"

#include <RtfDomElement.h>

#include <RtfDomDocument.h>
#include <RtfTextFormat.h>

using namespace ExLibris;

TEST(RtfDomElement, Construct)
{
	RtfDomDocument doc(nullptr);
	RtfDomElement e(doc);

	EXPECT_EQ(nullptr, e.GetParent());
	EXPECT_EQ(nullptr, e.GetSiblingNext());
	EXPECT_EQ(nullptr, e.GetSiblingPrevious());
	EXPECT_EQ(0, e.GetChildrenCount());
}

TEST(RtfDomElement, ConstructWithParent)
{
	RtfDomDocument doc(nullptr);
	RtfDomElement root(doc);

	RtfDomElement* e = root.AddChild();

	EXPECT_EQ(&root, e->GetParent());
	EXPECT_EQ(nullptr, e->GetSiblingPrevious());
	EXPECT_EQ(nullptr, e->GetSiblingNext());
	EXPECT_EQ(0, e->GetChildrenCount());
}

TEST(RtfDomElement, AddChild)
{
	RtfDomDocument doc(nullptr);
	RtfDomElement root(doc);

	RtfDomElement* c = root.AddChild();

	EXPECT_EQ(1, root.GetChildrenCount());

	EXPECT_EQ(&root, c->GetParent());
	EXPECT_EQ(nullptr, c->GetSiblingPrevious());
	EXPECT_EQ(nullptr, c->GetSiblingNext());
}

TEST(RtfDomElement, AddTwoChildren)
{
	RtfDomDocument doc(nullptr);
	RtfDomElement root(doc);

	RtfDomElement* c1 = root.AddChild();
	RtfDomElement* c2 = root.AddChild();

	EXPECT_EQ(2, root.GetChildrenCount());

	EXPECT_EQ(nullptr, c1->GetSiblingPrevious());
	EXPECT_EQ(c2, c1->GetSiblingNext());

	EXPECT_EQ(c1, c2->GetSiblingPrevious());
	EXPECT_EQ(nullptr, c2->GetSiblingNext());
}

TEST(RtfDomElement, AddGrandChild)
{
	RtfDomDocument doc(nullptr);
	RtfDomElement root(doc);

	RtfDomElement* c1 = root.AddChild();
	RtfDomElement* c2 = root.AddChild();
	RtfDomElement* g1 = c2->AddChild();
	RtfDomElement* c3 = root.AddChild();

	EXPECT_EQ(3, root.GetChildrenCount());

	EXPECT_EQ(nullptr, c1->GetSiblingPrevious());
	EXPECT_EQ(c2, c1->GetSiblingNext());

	EXPECT_EQ(c1, c2->GetSiblingPrevious());
	EXPECT_EQ(c3, c2->GetSiblingNext());

	EXPECT_EQ(c2, c3->GetSiblingPrevious());
	EXPECT_EQ(nullptr, c3->GetSiblingNext());

	EXPECT_EQ(1, c2->GetChildrenCount());
	EXPECT_EQ(c2, g1->GetParent());
	EXPECT_EQ(&root, g1->GetParent()->GetParent());
}

TEST(RtfDomElement, ParseText)
{
	RtfDomDocument doc(nullptr);
	RtfDomElement e(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Text;
	t.value = "This is whack.";

	EXPECT_EQ(IRtfParseable::eResult_Handled, e.Parse(s, t));
	EXPECT_STREQ("This is whack.", e.InnerText.c_str());
}

TEST(RtfDomElement, ParseTextFormatProperty)
{
	RtfDomDocument doc(nullptr);
	RtfDomElement e(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "pca";

	EXPECT_EQ(IRtfParseable::eResult_Handled, e.Parse(s, t));
	EXPECT_EQ(Rtf::eCharacterSet_Oem, e.GetTextFormat().GetCharacterSet());
}

TEST(RtfDomElement, ParseUnhandled)
{
	RtfDomDocument doc(nullptr);
	RtfDomElement e(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "windmills";

	EXPECT_EQ(IRtfParseable::eResult_Propagate, e.Parse(s, t));
}