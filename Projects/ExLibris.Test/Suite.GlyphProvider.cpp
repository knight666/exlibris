#include "ExLibris.Test.PCH.h"

#include <Face.h>
#include <GlyphProviderFreetype.h>
#include <Library.h>

using namespace ExLibris;

TEST(GlyphProvider, LoadFace)
{
	Library* library = new Library;
	FontLoaderFreetype* loader = new FontLoaderFreetype(library);

	std::fstream in_stream("Fonts/Roboto/Roboto-BoldItalic.ttf", std::ios::in | std::ios::binary);
	ASSERT_TRUE(in_stream.is_open());

	IGlyphProvider* provider = loader->LoadGlyphProvider(in_stream);

	Face* face = provider->CreateFace(16.0f);
	GlyphBitmap* bitmap = face->GetGlyphBitmap(33);

	int i = 0;
}