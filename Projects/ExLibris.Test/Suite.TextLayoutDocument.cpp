#include "ExLibris.Test.PCH.h"

#include <FontLoaderFreetype.h>
#include <Library.h>
#include <TextLayoutDocument.h>
#include <TextParserMarkdown.h>

using namespace ExLibris;

TEST(TextLayoutDocument, ParseText)
{
	Library* lib = new Library;
	lib->AddLoader(new FontLoaderFreetype(lib));
	lib->MapFontToFile("Fonts/Roboto/Roboto-Regular.ttf");
	lib->MapFontToFile("Fonts/Roboto/Roboto-Italic.ttf");

	TextLayoutDocument document(lib);
	document.SetDefaultFamily("Roboto");
	document.SetDefaultSize(12.0f);
	document.SetParser(new TextParserMarkdown());

	document.SetText("I'm on a *motorized* vehicle.");

	document.Layout();

	int i = 0;
}