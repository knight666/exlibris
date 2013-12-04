#include "ExLibris.Test.PCH.h"

#include <FontLoaderFreetype.h>
#include <Library.h>
#include <TextFormat.h>
#include <TextLayoutCharacter.h>
#include <TextLayoutDocument.h>
#include <TextLayoutLine.h>
#include <TextParserMarkdown.h>

using namespace ExLibris;

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