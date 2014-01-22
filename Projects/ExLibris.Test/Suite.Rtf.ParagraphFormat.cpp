#include "ExLibris.Test.PCH.h"

#include <RtfParagraphFormat.h>

#include <RtfDomDocument.h>

using namespace ExLibris;

TEST(RtfParagraphFormat, Construct)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	EXPECT_FLOAT_EQ(0.0f, pf.GetSpaceBefore().GetValue(RtfUnit::eType_Twips));
	EXPECT_FLOAT_EQ(0.0f, pf.GetSpaceAfter().GetValue(RtfUnit::eType_Twips));
}