#include "FrameworkGL.PCH.h"

#include "FontSystem.h"

#include <FontFace.h>
#include <Library.h>

namespace Framework
{

	// encoded using systembuilder.py
	// for reference, please see systemfont.tga

	static const unsigned int s_GlyphBitmapsEncoded[] = {
		0x00000000, 0x00000000, 0x00000000, /*   */
		0x08080808, 0x08080800, 0x00080000, /* ! */
		0x22220000, 0x00000000, 0x00000000, /* " */
		0x00002222, 0xAA22AA22, 0x22000000, /* # */
		0x00288A88, 0x882A2222, 0xA8000000, /* $ */
		0x00008282, 0x08282082, 0x82000000, /* % */
		0x00288080, 0xA2288A82, 0x28000000, /* & */
		0x08080000, 0x00000000, 0x00000000, /* ' */
		0x02082020, 0x20202008, 0x02000000, /* ( */
		0x20080202, 0x02020208, 0x20000000, /* ) */
		0x082A0822, 0x00000000, 0x00000000, /* * */
		0x00000008, 0x08AA0808, 0x00000000, /* + */
		0x00000000, 0x00000028, 0x28082000, /* , */
		0x00000000, 0x00AA0000, 0x00000000, /* - */
		0x00000000, 0x00000028, 0x28000000, /* . */
		0x02020208, 0x08082020, 0x20000000, /* / */
		0x2882828A, 0x8AA2A282, 0x28000000, /* 0 */
		0x08280808, 0x08080808, 0x2A000000, /* 1 */
		0x28820202, 0x08282080, 0xAA000000, /* 2 */
		0x28820228, 0x02020282, 0x28000000, /* 3 */
		0x08202080, 0x88AA0808, 0x08000000, /* 4 */
		0xAA8080A8, 0x02020202, 0xA8000000, /* 5 */
		0x288280A8, 0x82828282, 0x28000000, /* 6 */
		0xAA020208, 0x08282020, 0x20000000, /* 7 */
		0x28828228, 0x82828282, 0x28000000, /* 8 */
		0x2882822A, 0x02020282, 0x28000000, /* 9 */
		0x00000028, 0x28000028, 0x28000000, /* : */
		0x00000028, 0x28000028, 0x28082000, /* ; */
		0x00000208, 0x20808020, 0x08020000, /* < */
		0x00000000, 0xAA0000AA, 0x00000000, /* = */
		0x00008020, 0x08020208, 0x20800000, /* > */
		0x28820202, 0x08080800, 0x00080000, /* ? */
		0x00288282, 0x8A8A8082, 0x28000000, /* @ */
		0x28828282, 0xAA828282, 0x82000000, /* A */
		0xA8828282, 0xA8828282, 0xA8000000, /* B */
		0x28828080, 0x80808082, 0x28000000, /* C */
		0xA0888282, 0x82828282, 0xA8000000, /* D */
		0xAA808080, 0xA8808080, 0xAA000000, /* E */
		0xAA808080, 0xA8808080, 0x80000000, /* F */
		0x2A808080, 0x8A828282, 0x2A000000, /* G */
		0x82828282, 0xAA828282, 0x82000000, /* H */
		0x2A080808, 0x08080808, 0x2A000000, /* I */
		0x2A080808, 0x08080888, 0x20000000, /* J */
		0x82828888, 0xA0888282, 0x82000000, /* K */
		0x80808080, 0x80808080, 0xAA000000, /* L */
		0x2AA2A2A2, 0x82828282, 0x82000000, /* M */
		0x8282A2A2, 0x8A8A8A82, 0x82000000, /* N */
		0x28828282, 0x82828282, 0x28000000, /* O */
		0xA8828282, 0x82A88080, 0x80000000, /* P */
		0x28828282, 0x828A8A8A, 0x2A000000, /* Q */
		0x28828282, 0xA8888282, 0x82000000, /* R */
		0x28828080, 0x28020282, 0x28000000, /* S */
		0xAA202020, 0x20202020, 0x20000000, /* T */
		0x82828282, 0x82828282, 0x2A000000, /* U */
		0x82828282, 0x82822222, 0x08000000, /* V */
		0x82828282, 0x8A8A8A8A, 0x28000000, /* W */
		0x82828228, 0x28288282, 0x82000000, /* X */
		0x82828282, 0x28080808, 0x08000000, /* Y */
		0xAA020808, 0x20208080, 0xAA000000, /* Z */
		0x002A2020, 0x20202020, 0x202A0000, /* [ */
		0x20202008, 0x08080202, 0x02000000, /* \ */
		0x002A0202, 0x02020202, 0x022A0000, /* ] */
		0x082A8200, 0x00000000, 0x00000000, /* ^ */
		0x00000000, 0x00000000, 0xAA000000, /* _ */
		0x0080A020, 0x00000000, 0x00000000, /* ` */
		0x00000028, 0x022A8282, 0x2A000000, /* a */
		0x808080A8, 0x82828282, 0x28000000, /* b */
		0x00000028, 0x82808082, 0x28000000, /* c */
		0x0202022A, 0x82828282, 0x2A000000, /* d */
		0x00000028, 0x8282A880, 0x2A000000, /* e */
		0x0A2020AA, 0x20202020, 0x20000000, /* f */
		0x00000028, 0x82828282, 0x2A0202A8, /* g */
		0x808080A8, 0x82828282, 0x82000000, /* h */
		0x08000028, 0x08080808, 0x2A000000, /* i */
		0x08000028, 0x08080808, 0x080808A0, /* j */
		0x80808082, 0x88A88282, 0x82000000, /* k */
		0x20202020, 0x20202020, 0x08000000, /* l */
		0x0000002A, 0xA2A28282, 0x82000000, /* m */
		0x000000A8, 0x82828282, 0x82000000, /* n */
		0x00000028, 0x82828282, 0x28000000, /* o */
		0x00000028, 0x82828282, 0xA8808080, /* p */
		0x00000028, 0x82828282, 0x2A020202, /* q */
		0x00000028, 0x80808080, 0x80000000, /* r */
		0x00000028, 0x82802802, 0xA8000000, /* s */
		0x002020A8, 0x20202020, 0x08000000, /* t */
		0x00000082, 0x82828282, 0x2A000000, /* u */
		0x00000082, 0x82828222, 0x08000000, /* v */
		0x00000082, 0x82828A8A, 0x28000000, /* w */
		0x00000082, 0x82282882, 0x82000000, /* x */
		0x00000082, 0x82828222, 0x080808A0, /* y */
		0x000000AA, 0x02082080, 0xAA000000, /* z */
		0x02080808, 0x20080808, 0x02000000, /* { */
		0x08080808, 0x08080808, 0x08000000, /* | */
		0x80202020, 0x08202020, 0x80000000, /* } */
		0x00000000, 0x22AA8800, 0x00000000, /* ~ */
		0xAA828282, 0x82828282, 0xAA000000, /* invalid */
	};

	FontSystem::FontSystem()
		: IFont(nullptr)
	{
	}
	
	FontSystem::~FontSystem()
	{
	}

	unsigned int FontSystem::GetIndexFromCodepoint(unsigned int a_CodepointUtf32) const
	{
		if (a_CodepointUtf32 < 32 || a_CodepointUtf32 > 127)
		{
			return 127;
		}
		else
		{
			return a_CodepointUtf32;
		}
	}

	ExLibris::FontFace* FontSystem::CreateFace(float a_Size)
	{
		ExLibris::FontFace* face = new ExLibris::FontFace(this);
		face->SetSize(12.0f);
		face->SetLineHeight(14.0f);

		for (unsigned int index = 32; index < 128; ++index)
		{
			ExLibris::Glyph* glyph = new ExLibris::Glyph;

			glyph->index = index;

			glyph->metrics = new ExLibris::GlyphMetrics;
			glyph->metrics->advance = 6;

			glyph->bitmap = _DecodeBitmap(index - 32);

			face->AddGlyph(glyph);
		}

		return face;
	}

	ExLibris::GlyphBitmap* FontSystem::_DecodeBitmap(unsigned int a_Index) const
	{
		ExLibris::GlyphBitmap* bitmap = new ExLibris::GlyphBitmap;
		bitmap->width = 4;
		bitmap->height = 12;
		bitmap->data = new unsigned char[bitmap->width * bitmap->height * 4];

		unsigned int encoded_upper  = s_GlyphBitmapsEncoded[a_Index * 3];
		unsigned int encoded_middle = s_GlyphBitmapsEncoded[a_Index * 3 + 1];
		unsigned int encoded_lower  = s_GlyphBitmapsEncoded[a_Index * 3 + 2];

		unsigned int dst_pitch = bitmap->width * 4;
		unsigned int* dst_upper  = (unsigned int*)(bitmap->data);
		unsigned int* dst_middle = (unsigned int*)(bitmap->data + (dst_pitch * 4));
		unsigned int* dst_lower  = (unsigned int*)(bitmap->data + (dst_pitch * 8));

		unsigned int encoded_bit = 31;

		for (unsigned int y = 0; y < 4; ++y)
		{
			for (unsigned int x = 0; x < 4; ++x)
			{
				unsigned int encoded_mask = (1 << encoded_bit);

				*dst_upper++  = ((encoded_upper  & encoded_mask) > 0) ? 0xFFFFFFFF : 0x00000000;
				*dst_middle++ = ((encoded_middle & encoded_mask) > 0) ? 0xFFFFFFFF : 0x00000000;
				*dst_lower++  = ((encoded_lower  & encoded_mask) > 0) ? 0xFFFFFFFF : 0x00000000;

				encoded_bit -= 2;
			}
		}

		return bitmap;
	}

}; // namespace Framework