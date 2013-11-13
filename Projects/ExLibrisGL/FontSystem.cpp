/*
 * This file is a part of the ExLibris project.
 *
 * Copyright (C) 2013 Quinten Lansu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */

#include "ExLibrisGL.PCH.h"

#include "FontSystem.h"

#include "BoundingBox.h"
#include "FontFace.h"
#include "Library.h"

namespace ExLibris
{

	// encoded using systembuilder.py
	// for reference, please see systemfont.tga

	static const unsigned int s_GlyphBitmapsEncoded[] = {
		0x00000000, 0x00000000, 0x00000000, /*   */
		0x08080808, 0x08080800, 0x08080000, /* ! */
		0x14141400, 0x00000000, 0x00000000, /* " */
		0x00000012, 0x127F2424, 0xFE484800, /* # */
		0x083C4A48, 0x483C1212, 0x523C1000, /* $ */
		0x00006052, 0x5428142A, 0x4A060000, /* % */
		0x00182424, 0x18284542, 0x26190000, /* & */
		0x08080800, 0x00000000, 0x00000000, /* ' */
		0x04080810, 0x10101010, 0x10080804, /* ( */
		0x10080804, 0x04040404, 0x04080810, /* ) */
		0x00082A1C, 0x2A080000, 0x00000000, /* * */
		0x00000008, 0x08087F08, 0x08080000, /* + */
		0x00000000, 0x00000000, 0x08081000, /* , */
		0x00000000, 0x00007E00, 0x00000000, /* - */
		0x00000000, 0x00000000, 0x08080000, /* . */
		0x02020404, 0x08081010, 0x20200000, /* / */
		0x3C46464A, 0x4A525262, 0x623C0000, /* 0 */
		0x08182808, 0x08080808, 0x083E0000, /* 1 */
		0x3C420202, 0x04081020, 0x406E0000, /* 2 */
		0x3C420202, 0x1C020202, 0x423C0000, /* 3 */
		0x10102024, 0x44447E04, 0x04040000, /* 4 */
		0x7E404040, 0x7C020202, 0x027C0000, /* 5 */
		0x3C424040, 0x7C424242, 0x423C0000, /* 6 */
		0x7E020204, 0x04080810, 0x10100000, /* 7 */
		0x3C424242, 0x3C424242, 0x423C0000, /* 8 */
		0x3C424242, 0x3E020202, 0x423C0000, /* 9 */
		0x00000000, 0x08080000, 0x08080000, /* : */
		0x00000000, 0x08080000, 0x08081000, /* ; */
		0x00000408, 0x10204020, 0x10080400, /* < */
		0x00000000, 0x7E00007E, 0x00000000, /* = */
		0x00002010, 0x08040204, 0x08102000, /* > */
		0x3C420202, 0x04080800, 0x08080000, /* ? */
		0x003C4281, 0x9DA5A5A5, 0xBD854238, /* @ */
		0x3C424242, 0x7E424242, 0x42420000, /* A */
		0x7C424242, 0x7C424242, 0x427C0000, /* B */
		0x1C224040, 0x40404040, 0x221C0000, /* C */
		0x78444242, 0x42424242, 0x44780000, /* D */
		0x7E404040, 0x78404040, 0x407E0000, /* E */
		0x7E404040, 0x78404040, 0x40400000, /* F */
		0x3C424040, 0x4E424242, 0x423C0000, /* G */
		0x42424242, 0x7E424242, 0x42420000, /* H */
		0x3E080808, 0x08080808, 0x083E0000, /* I */
		0x3E080808, 0x08080808, 0x48300000, /* J */
		0x42444850, 0x70484442, 0x42420000, /* K */
		0x40404040, 0x40404040, 0x407E0000, /* L */
		0x41635549, 0x41414141, 0x41410000, /* M */
		0x42626252, 0x524A4A46, 0x46420000, /* N */
		0x3C424242, 0x42424242, 0x423C0000, /* O */
		0x7C424242, 0x7C404040, 0x40400000, /* P */
		0x3C424242, 0x42424A4A, 0x463C0200, /* Q */
		0x7C424242, 0x7C444242, 0x42420000, /* R */
		0x3C424040, 0x3C020202, 0x423C0000, /* S */
		0x7F080808, 0x08080808, 0x08080000, /* T */
		0x42424242, 0x42424242, 0x423C0000, /* U */
		0x42424242, 0x42242424, 0x18180000, /* V */
		0x41414141, 0x4949492A, 0x2A360000, /* W */
		0x42422424, 0x18182424, 0x42420000, /* X */
		0x41412222, 0x14080808, 0x08080000, /* Y */
		0x7E020404, 0x08101020, 0x407E0000, /* Z */
		0x7E404040, 0x40404040, 0x4040407E, /* [ */
		0x40402020, 0x10100808, 0x04040000, /* \ */
		0x7E020202, 0x02020202, 0x0202027E, /* ] */
		0x08141422, 0x00000000, 0x00000000, /* ^ */
		0x00000000, 0x00000000, 0x000000FF, /* _ */
		0x10080000, 0x00000000, 0x00000000, /* ` */
		0x00000000, 0x3C023E42, 0x423E0000, /* a */
		0x40404040, 0x7C424242, 0x427C0000, /* b */
		0x00000000, 0x3C424040, 0x423C0000, /* c */
		0x02020202, 0x3E424242, 0x423E0000, /* d */
		0x00000000, 0x3C42427C, 0x403E0000, /* e */
		0x0C101010, 0x7C101010, 0x10100000, /* f */
		0x00000000, 0x3C424242, 0x3E02027C, /* g */
		0x40404040, 0x7C424242, 0x42420000, /* h */
		0x00080000, 0x18080808, 0x08080000, /* i */
		0x00080000, 0x18080808, 0x08080830, /* j */
		0x40404044, 0x48784844, 0x44440000, /* k */
		0x10101010, 0x10101010, 0x10080000, /* l */
		0x00000000, 0x56694949, 0x49410000, /* m */
		0x00000000, 0x5C624242, 0x42420000, /* n */
		0x00000000, 0x3C424242, 0x423C0000, /* o */
		0x00000000, 0x7C424242, 0x427C4040, /* p */
		0x00000000, 0x3E424242, 0x423E0202, /* q */
		0x00000000, 0x5C624040, 0x40400000, /* r */
		0x00000000, 0x3C42403C, 0x027C0000, /* s */
		0x00001010, 0x3C101010, 0x100C0000, /* t */
		0x00000000, 0x42424242, 0x463A0000, /* u */
		0x00000000, 0x42422424, 0x18180000, /* v */
		0x00000000, 0x4141492A, 0x2A360000, /* w */
		0x00000000, 0x42241818, 0x24420000, /* x */
		0x00000000, 0x42422414, 0x08081060, /* y */
		0x00000000, 0x7E040810, 0x207E0000, /* z */
		0x0C101010, 0x10201010, 0x10100C00, /* { */
		0x08080808, 0x08080808, 0x08080800, /* | */
		0x18040404, 0x04020404, 0x04041800, /* } */
		0x00000031, 0x49460000, 0x00000000, /* ~ */
		0x7E424242, 0x42424242, 0x4242427E, /* invalid */
	};

	FontSystem::FontSystem(Family* a_Family)
		: IFont(a_Family)
		, m_Face(nullptr)
	{
	}
	
	FontSystem::~FontSystem()
	{
		if (m_Face != nullptr)
		{
			delete m_Face;
		}
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

	FontFace* FontSystem::CreateFace(const FaceOptions& a_Options)
	{
		if (m_Face == nullptr)
		{
			m_Face = new FontFace(this);
			m_Face->SetSize(12.0f);
			m_Face->SetLineHeight(16.0f);

			BoundingBox glyph_bounding_box(glm::vec2(0.0f, 0.0f), glm::vec2(8.0f, 12.0f));

			for (unsigned int index = 32; index < 128; ++index)
			{
				Glyph* glyph = new Glyph;

				glyph->index = index;

				glyph->metrics = new GlyphMetrics;
				glyph->metrics->advance = 8;
				glyph->metrics->bounding_box = glyph_bounding_box;

				glyph->bitmap = _DecodeBitmap(index - 32);

				m_Face->AddGlyph(glyph);
			}
		}

		return m_Face;
	}

	GlyphBitmap* FontSystem::_DecodeBitmap(unsigned int a_Index) const
	{
		GlyphBitmap* bitmap = new GlyphBitmap;
		bitmap->width = 8;
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
			for (unsigned int x = 0; x < 8; ++x)
			{
				unsigned int encoded_mask = (1 << encoded_bit);

				*dst_upper++  = ((encoded_upper  & encoded_mask) > 0) ? 0xFFFFFFFF : 0x00000000;
				*dst_middle++ = ((encoded_middle & encoded_mask) > 0) ? 0xFFFFFFFF : 0x00000000;
				*dst_lower++  = ((encoded_lower  & encoded_mask) > 0) ? 0xFFFFFFFF : 0x00000000;

				encoded_bit--;
			}
		}

		return bitmap;
	}

}; // namespace ExLibris