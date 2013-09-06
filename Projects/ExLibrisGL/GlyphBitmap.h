#ifndef _GLYPHBITMAP_H_
#define _GLYPHBITMAP_H_

namespace ExLibris
{

	struct GlyphBitmap
	{

	public:

		GlyphBitmap();
		~GlyphBitmap();

	public:

		unsigned int width;
		unsigned int height;
		unsigned char* data;

	}; // struct GlyphBitmap

}; // namespace ExLibris

#endif