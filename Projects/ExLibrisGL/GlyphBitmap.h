#ifndef _GLYPHBITMAP_H_
#define _GLYPHBITMAP_H_

namespace ExLibris
{

	struct GlyphBitmap
	{

		GlyphBitmap()
			: width(0)
			, height(0)
			, data(nullptr)
		{
		}

		~GlyphBitmap()
		{
			if (data != nullptr)
			{
				delete [] data;
			}
		}

	public:

		unsigned int width;
		unsigned int height;
		unsigned char* data;

	}; // struct GlyphBitmap

}; // namespace ExLibris

#endif