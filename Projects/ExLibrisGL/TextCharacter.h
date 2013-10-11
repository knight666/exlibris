#ifndef _TEXTCHARACTER_H_
#define _TEXTCHARACTER_H_

namespace ExLibris
{
	struct Glyph;
}

namespace ExLibris
{

	struct TextCharacter
	{
	
	public:

		enum Type
		{
			eType_Character,
			eType_Whitespace,
			eType_NewLine,
			eType_End
		};

	public:
	
		TextCharacter()
			: type(eType_End)
			, identifier(0)
			, glyph(nullptr)
			, x(0.0f)
			, advance(0.0f)
		{
		}

		Type type;
		unsigned int identifier;
		const Glyph* glyph;
		float x;
		float advance;
	
	}; // struct TextCharacter

}; // namespace ExLibris

#endif