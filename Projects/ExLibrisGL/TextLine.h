#ifndef _TEXTLINE_H_
#define _TEXTLINE_H_

#include "TextCharacter.h"

namespace ExLibris
{

	struct TextLine
	{

		std::vector<TextCharacter*> characters;
		glm::vec2 position;
		glm::vec2 dimensions;

	}; // struct TextLine

}; // namespace ExLibris

#endif