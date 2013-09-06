#include "ExLibrisGL.PCH.h"

#include "GlyphBitmap.h"

namespace ExLibris
{

	GlyphBitmap::GlyphBitmap()
		: width(0)
		, height(0)
		, data(nullptr)
	{
	}

	GlyphBitmap::~GlyphBitmap()
	{
		if (data != nullptr)
		{
			delete [] data;
		}
	}

}; // namespace ExLibris