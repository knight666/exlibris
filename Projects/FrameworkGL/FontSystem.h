#ifndef _FONTSYSTEM_H_
#define _FONTSYSTEM_H_

#include <IFont.h>

namespace ExLibris
{
	class FontFace;
	struct GlyphBitmap;
	class Library;
}

namespace Framework
{

	class FontSystem
		: public ExLibris::IFont
	{
	
	public:
	
		FontSystem();
		~FontSystem();
	
		unsigned int GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const;

		ExLibris::FontFace* CreateFace(const ExLibris::FaceOptions& a_Options);

	private:

		ExLibris::GlyphBitmap* _DecodeBitmap(unsigned int a_Index) const;
	
	}; // class FontSystem

}; // namespace Framework

#endif