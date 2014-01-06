#include "ExLibris.Test.PCH.h"

#include <RtfDomDocument.h>

using namespace ExLibris;

TEST(RtfDomDocument, Construct)
{
	RtfDomDocument doc;

	EXPECT_EQ(eRtfCharacterSet_Invalid, doc.GetCharacterSet());
}

TEST(RtfDomDocument, CharacterSet)
{
	RtfDomDocument doc;

	doc.SetCharacterSet(eRtfCharacterSet_AppleMacintosh);

	EXPECT_EQ(eRtfCharacterSet_AppleMacintosh, doc.GetCharacterSet());
}

TEST(RtfDomDocument, GetFont)
{
	RtfDomDocument doc;

	RtfFont& f0 = doc.GetFont(0);

	EXPECT_STREQ("", f0.name.c_str());
	EXPECT_EQ(RtfFont::eFamilyType_Nil, f0.family);
}

TEST(RtfDomDocument, GetFontTwice)
{
	RtfDomDocument doc;

	RtfFont& f13 = doc.GetFont(13);
	f13.name = "Roman Gothic Bold";
	f13.family = RtfFont::eFamilyType_Roman;

	RtfFont& f13_again = doc.GetFont(13);

	EXPECT_STREQ("Roman Gothic Bold", f13_again.name.c_str());
	EXPECT_EQ(RtfFont::eFamilyType_Roman, f13_again.family);
}