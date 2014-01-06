#include "ExLibris.Test.PCH.h"

#include <RtfDomElement.h>

using namespace ExLibris;

TEST(RtfDomElement, Construct)
{
	RtfDomElement e;

	EXPECT_EQ(nullptr, e.GetParent());
	EXPECT_EQ(nullptr, e.GetSiblingNext());
	EXPECT_EQ(nullptr, e.GetSiblingPrevious());
	EXPECT_EQ(0, e.GetChildrenCount());
}

TEST(RtfDomElement, ConstructWithParent)
{
	RtfDomElement root;

	RtfDomElement e(&root);

	EXPECT_EQ(&root, e.GetParent());
	EXPECT_EQ(nullptr, e.GetSiblingPrevious());
	EXPECT_EQ(nullptr, e.GetSiblingNext());
	EXPECT_EQ(0, e.GetChildrenCount());
}

TEST(RtfDomElement, AddChild)
{
	RtfDomElement root;

	RtfDomElement* c = root.AddChild();

	EXPECT_EQ(1, root.GetChildrenCount());

	EXPECT_EQ(&root, c->GetParent());
	EXPECT_EQ(nullptr, c->GetSiblingPrevious());
	EXPECT_EQ(nullptr, c->GetSiblingNext());
}

TEST(RtfDomElement, AddTwoChildren)
{
	RtfDomElement root;

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
	RtfDomElement root;

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