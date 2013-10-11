#include "ExLibrisGL.PCH.h"

#include "TextLayout.h"

namespace ExLibris
{

	TextLayout::TextLayout()
		: m_Face(nullptr)
		, m_GlyphSpace(nullptr)
		, m_HorizontalPolicy(eSizePolicy_Ignore)
		, m_VerticalPolicy(eSizePolicy_Ignore)
		, m_HorizontalAlignment(eHorizontalAlignment_Left)
		, m_VerticalAlignment(eVerticalAlignment_Top)
		, m_WordWrapping(eWordWrapping_Greedy)
		, m_GlyphsDirty(false)
		, m_LinesDirty(false)
		, m_LayoutDirty(false)
	{
		m_GlyphSpace = new TextCharacter;
	}
	
	TextLayout::~TextLayout()
	{
		for (std::vector<TextCharacter*>::iterator glyph_it = m_Glyphs.begin(); glyph_it != m_Glyphs.end(); ++glyph_it)
		{
			delete *glyph_it;
		}
		m_Glyphs.clear();

		for (std::vector<TextLine*>::iterator line_it = m_Lines.begin(); line_it != m_Lines.end(); ++line_it)
		{
			delete *line_it;
		}
		m_Lines.clear();
	}

	FontFace* TextLayout::GetFontFace() const
	{
		return m_Face;
	}

	void TextLayout::SetFontFace(FontFace* a_Face)
	{
		m_Face = a_Face;

		m_GlyphSpace->glyph = m_Face->FindGlyph((unsigned int)' ');
		if (m_GlyphSpace->glyph != nullptr)
		{
			m_GlyphSpace->advance = m_GlyphSpace->glyph->metrics->advance;
		}
		else
		{
			m_GlyphSpace->advance = 0.0f;
		}

		m_GlyphsDirty = true;
	}

	const std::string& TextLayout::GetText() const
	{
		return m_Text;
	}

	void TextLayout::SetText(const std::string& a_Text)
	{
		m_Text = a_Text;

		m_GlyphsDirty = true;
	}

	const glm::vec2& TextLayout::GetSizeHint() const
	{
		return m_SizeHint;
	}

	void TextLayout::SetSizeHint(const glm::vec2& a_SizeHint)
	{
		m_SizeHint = a_SizeHint;

		m_LinesDirty = true;
	}

	void TextLayout::SetHorizontalSizePolicy(SizePolicy a_Policy)
	{
		m_HorizontalPolicy = a_Policy;

		m_LinesDirty = true;
	}

	void TextLayout::SetVerticalSizePolicy(SizePolicy a_Policy)
	{
		m_VerticalPolicy = a_Policy;

		m_LinesDirty = true;
	}

	void TextLayout::SetHorizontalAlignment(HorizontalAlignment a_Alignment)
	{
		m_HorizontalAlignment = a_Alignment;

		m_LayoutDirty = true;
	}

	void TextLayout::SetVerticalAlignment(VerticalAlignment a_Alignment)
	{
		m_VerticalAlignment = a_Alignment;

		m_LayoutDirty = true;
	}

	void TextLayout::SetWordWrapping(WordWrapping a_Wrapping)
	{
		m_WordWrapping = a_Wrapping;

		m_LinesDirty = true;
	}

	const glm::vec2& TextLayout::GetDimensions() const
	{
		return m_Dimensions;
	}

	void TextLayout::Layout()
	{
		if (!(m_GlyphsDirty || m_LinesDirty || m_LayoutDirty))
		{
			return;
		}

		if (m_GlyphsDirty)
		{
			for (std::vector<TextCharacter*>::iterator glyph_it = m_Glyphs.begin(); glyph_it != m_Glyphs.end(); ++glyph_it)
			{
				delete *glyph_it;
			}
			m_Glyphs.clear();

			if (m_Face != nullptr)
			{
				_ConvertTextToGlyphs();

				m_GlyphsDirty = false;
			}

			m_LinesDirty = true;
		}

		if (m_LinesDirty)
		{
			if (m_Face != nullptr)
			{
				_ConvertGlyphsToLines();
			}

			_CalculateDimensions();

			m_LinesDirty = false;
			m_LayoutDirty = true;
		}

		if (m_LayoutDirty)
		{
			_LayoutLines();

			m_LayoutDirty = false;
		}
	}

	void TextLayout::Accept(ITextLayoutVisitor& a_Visitor)
	{
		Layout();

		a_Visitor.VisitTextBegin(m_Face, m_Dimensions);

		for (std::vector<TextLine*>::iterator line_it = m_Lines.begin(); line_it != m_Lines.end(); ++line_it)
		{
			TextLine* line = *line_it;

			a_Visitor.VisitTextLineBegin(line->characters.size(), line->position, line->dimensions.x);

			for (std::vector<TextCharacter*>::iterator glyph_it = line->characters.begin(); glyph_it != line->characters.end(); ++glyph_it)
			{
				TextCharacter* glyph = *glyph_it;

				if (glyph->type == TextCharacter::eType_Character)
				{
					a_Visitor.VisitTextCharacter(glyph->glyph, glyph->x, glyph->advance);
				}
				else if (glyph->type == TextCharacter::eType_Whitespace)
				{
					a_Visitor.VisitTextWhitespace(glyph->identifier, glyph->x, glyph->advance);
				}
			}

			a_Visitor.VisitTextLineEnd();
		}

		a_Visitor.VisitTextEnd();
	}

	std::vector<unsigned int> TextLayout::_AsciiToUtf32(const std::string& a_Text)
	{
		std::vector<unsigned int> result;
		result.reserve(a_Text.size());

		for (std::string::const_iterator char_it = a_Text.begin(); char_it != a_Text.end(); ++char_it)
		{
			result.push_back((unsigned int)*char_it);
		}

		return result;
	}

	void TextLayout::_ConvertTextToGlyphs()
	{
		std::vector<unsigned int> text_utf32 = _AsciiToUtf32(m_Text);

		std::vector<unsigned int>::const_iterator char_next_it = text_utf32.end();
		if (text_utf32.size() > 1)
		{
			char_next_it = text_utf32.begin() + 1;
		}

		unsigned int char_space = (unsigned int)' ';
		unsigned int char_tab = (unsigned int)'\t';
		unsigned int char_carriage_return = (unsigned int)'\r';
		unsigned int char_new_line = (unsigned int)'\n';

		for (std::vector<unsigned int>::const_iterator char_it = text_utf32.begin(); char_it != text_utf32.end(); ++char_it)
		{
			wchar_t character = (wchar_t)*char_it;

			bool next_char_valid = (char_next_it != text_utf32.end());
			int whitespace_count = 0;

			unsigned int glyph_identifier = 0;
			TextCharacter::Type glyph_type = TextCharacter::eType_Character;

			if (character == char_carriage_return)
			{
				if (next_char_valid && (*char_next_it == char_new_line))
				{
					++char_it;
					++char_next_it;

					next_char_valid = (char_next_it != text_utf32.end());
				}

				glyph_type = TextCharacter::eType_NewLine;

				glyph_identifier = char_new_line;
			}
			else if (character == char_new_line)
			{
				glyph_type = TextCharacter::eType_NewLine;

				glyph_identifier = char_new_line;
			}
			else if (character == char_space)
			{
				glyph_type = TextCharacter::eType_Whitespace;
				whitespace_count = 1;

				glyph_identifier = character;
			}
			else if (character == char_tab)
			{
				glyph_type = TextCharacter::eType_Whitespace;
				whitespace_count = 4;

				glyph_identifier = character;
			}

			bool glyph_valid = false;
			const Glyph* glyph_current = nullptr;
			float glyph_advance = 0.0f;

			switch (glyph_type)
			{

			case TextCharacter::eType_Character:
				{
					glyph_current = m_Face->FindGlyph(*char_it);
					if (glyph_current != nullptr)
					{
						glyph_valid = true;

						glyph_identifier = glyph_current->index;
						glyph_advance = glyph_current->metrics->advance;

						if (next_char_valid)
						{
							Glyph* glyph_next = m_Face->FindGlyph(*char_next_it);

							glm::vec2 kerning;
							if (m_Face->TryGetKerning((Glyph*)glyph_current, glyph_next, kerning))
							{
								glyph_advance += kerning.x;
							}
						}
					}

				} break;

			case TextCharacter::eType_Whitespace:
				{
					glyph_valid = true;

					glyph_current = m_GlyphSpace->glyph;
					glyph_advance = m_GlyphSpace->advance * whitespace_count;

				} break;

			case TextCharacter::eType_NewLine:
				{
					glyph_valid = true;

				} break;
			}

			if (glyph_valid)
			{
				TextCharacter* instance = new TextCharacter;
				instance->identifier = glyph_identifier;
				instance->glyph = glyph_current;
				instance->type = glyph_type;
				instance->x = 0.0f;
				instance->advance = glyph_advance;

				m_Glyphs.push_back(instance);
			}

			if (next_char_valid)
			{
				char_next_it++;
			}
		}

		if (m_Glyphs.size() > 0)
		{
			TextCharacter* instance = new TextCharacter;
			instance->type = TextCharacter::eType_End;
			instance->glyph = nullptr;
			instance->x = 0.0f;
			instance->advance = 0.0f;

			m_Glyphs.push_back(instance);
		}
	}

	void TextLayout::_ConvertGlyphsToLines()
	{
		for (std::vector<TextLine*>::iterator line_it = m_Lines.begin(); line_it != m_Lines.end(); ++line_it)
		{
			delete *line_it;
		}
		m_Lines.clear();

		bool fixed_width = (m_HorizontalPolicy == eSizePolicy_Maximum || m_HorizontalPolicy == eSizePolicy_Fixed);
		bool fixed_height = (m_VerticalPolicy == eSizePolicy_Maximum || m_VerticalPolicy == eSizePolicy_Fixed);

		m_Cursor.y = 0.0f;

		if (m_Glyphs.size() == 0 || !_NextLine(fixed_height))
		{
			return;
		}

		switch (m_WordWrapping)
		{

		case eWordWrapping_None:
			{
				_WordWrappingNone(fixed_width, fixed_height);

				break;
			}

		case eWordWrapping_Greedy:
			{
				_WordWrappingGreedy(fixed_width, fixed_height);

				break;
			}

		}
	}

	void TextLayout::_WordWrappingNone(bool a_WidthFixed, bool a_HeightFixed)
	{
		for (std::vector<TextCharacter*>::iterator glyph_it = m_Glyphs.begin(); glyph_it != m_Glyphs.end(); ++glyph_it)
		{
			TextCharacter* glyph = *glyph_it;

			if (glyph->type == TextCharacter::eType_Character || glyph->type == TextCharacter::eType_Whitespace)
			{
				if (a_WidthFixed && m_Cursor.x + glyph->advance >= m_SizeHint.x)
				{
					if (!_NextLine(a_HeightFixed))
					{
						return;
					}
				}

				_AddGlyphToCurrentLine(glyph);
			}
			else if (glyph->type == TextCharacter::eType_NewLine)
			{
				if (!_NextLine(a_HeightFixed))
				{
					return;
				}
			}
			else if (glyph->type == TextCharacter::eType_End)
			{
				break;
			}
		}
	}

	void TextLayout::_WordWrappingGreedy(bool a_WidthFixed, bool a_HeightFixed)
	{
		std::vector<TextCharacter*>::iterator word_start = m_Glyphs.begin();
		std::vector<TextCharacter*>::iterator word_end = word_start;
		float word_start_x = 0.0f;
		float word_length = 0.0f;

		std::vector<TextCharacter*>::iterator glyph_it = m_Glyphs.begin();

		while (glyph_it != m_Glyphs.end())
		{
			TextCharacter* glyph = *glyph_it;

			if (glyph->type == TextCharacter::eType_Character)
			{
				word_length += glyph->advance;
				word_end = glyph_it;
			}
			else
			{
				if (glyph->type == TextCharacter::eType_Whitespace)
				{
					// consume as much whitespace as possible

					while (glyph->type == TextCharacter::eType_Whitespace)
					{
						word_length += glyph->advance;
						word_end = glyph_it;

						glyph = *(++glyph_it);
					}
				}
				else
				{
					word_end = glyph_it;
				}

				bool word_add = false;

				if (a_WidthFixed)
				{
					// does word fit on line?

					if (word_start_x + word_length < m_SizeHint.x)
					{
						// add word to line

						word_add = true;
					}
					else
					{
						// is word longer than a single line?

						if (word_length >= m_SizeHint.x)
						{
							m_Cursor.x = word_start_x;

							for (std::vector<TextCharacter*>::iterator word_glyph_it = word_start; word_glyph_it != word_end; ++word_glyph_it)
							{
								TextCharacter* word_glyph = *word_glyph_it;

								// push characters to next line

								if (m_Cursor.x + word_glyph->advance >= m_SizeHint.x)
								{
									if (!_NextLine(a_HeightFixed))
									{
										return;
									}
								}

								if (word_glyph->type == TextCharacter::eType_Character || word_glyph->type == TextCharacter::eType_Whitespace)
								{
									_AddGlyphToCurrentLine(word_glyph);
								}
							}
						}
						else
						{
							// add word on new line

							if (!_NextLine(a_HeightFixed))
							{
								return;
							}

							word_add = true;
						}
					}
				}
				else
				{
					word_add = true;
				}

				// add word

				if (word_add)
				{
					std::vector<TextCharacter*>::iterator word_glyph_it = word_start;
					while (1)
					{
						TextCharacter* word_glyph = *word_glyph_it;

						if (word_glyph->type == TextCharacter::eType_Character || word_glyph->type == TextCharacter::eType_Whitespace)
						{
							_AddGlyphToCurrentLine(word_glyph);
						}

						if (word_glyph_it == word_end)
						{
							break;
						}

						++word_glyph_it;
					}
				}

				// reset word

				word_length = 0.0f;
				word_start_x = m_Cursor.x;

				word_start = glyph_it;
				word_end = word_start;

				// new line

				if (glyph->type == TextCharacter::eType_NewLine)
				{
					if (!_NextLine(a_HeightFixed))
					{
						return;
					}
				}
			}

			++glyph_it;
		}
	}

	void TextLayout::_CalculateDimensions()
	{
		bool lines_valid = (m_Lines.size() > 0) && (m_Face != nullptr);

		// horizontal

		m_Dimensions.x = 0.0f;

		if (m_HorizontalPolicy == eSizePolicy_Fixed)
		{
			m_Dimensions.x = std::max<float>(m_SizeHint.x, 0.0f);
		}
		else if (lines_valid)
		{
			switch (m_HorizontalPolicy)
			{

			case eSizePolicy_Ignore:
				{
					for (std::vector<TextLine*>::iterator line_it = m_Lines.begin(); line_it != m_Lines.end(); ++line_it)
					{
						TextLine* line = *line_it;

						m_Dimensions.x = std::max<float>(m_Dimensions.x, line->dimensions.x);
					}

					break;
				}

			case eSizePolicy_MinimumExpanding:
				{
					for (std::vector<TextLine*>::iterator line_it = m_Lines.begin(); line_it != m_Lines.end(); ++line_it)
					{
						TextLine* line = *line_it;

						line->dimensions.x = std::max<float>(m_SizeHint.x, line->dimensions.x);

						m_Dimensions.x = std::max<float>(m_Dimensions.x, line->dimensions.x);
					}

					break;
				}

			case eSizePolicy_Maximum:
				{
					for (std::vector<TextLine*>::iterator line_it = m_Lines.begin(); line_it != m_Lines.end(); ++line_it)
					{
						TextLine* line = *line_it;

						line->dimensions.x = std::min<float>(m_SizeHint.x, line->dimensions.x);

						m_Dimensions.x = std::max<float>(m_Dimensions.x, line->dimensions.x);
					}

					break;
				}

			}
		}

		// vertical

		m_Dimensions.y = 0;

		if (m_VerticalPolicy == eSizePolicy_Fixed)
		{
			m_Dimensions.y = std::max<float>(m_SizeHint.y, 0.0f);
		}
		else if (lines_valid)
		{
			float lines_height = m_Lines.size() * m_Face->GetLineHeight();

			switch (m_VerticalPolicy)
			{

			case eSizePolicy_Ignore:
				{
					m_Dimensions.y = lines_height;

					break;
				}

			case eSizePolicy_MinimumExpanding:
				{
					m_Dimensions.y = std::max<float>(lines_height, m_SizeHint.y);

					break;
				}

			case eSizePolicy_Maximum:
				{
					float hint_height = std::max<float>(m_SizeHint.y, 0.0f);
					m_Dimensions.y = std::min<float>(lines_height, hint_height);

					break;
				}

			}
		}
	}

	void TextLayout::_LayoutLines()
	{
		if (m_Lines.size() == 0 || m_Face == nullptr)
		{
			return;
		}

		float lines_height = m_Lines.size() * m_Face->GetLineHeight();

		// vertical

		float offset_y;

		switch (m_VerticalAlignment)
		{

		case eVerticalAlignment_Bottom:
			{
				offset_y = m_Dimensions.y - lines_height;

				break;
			}

		case eVerticalAlignment_Middle:
			{
				offset_y = (m_Dimensions.y - lines_height) / 2.0f;

				break;
			}

		case eVerticalAlignment_Top:
			{
				offset_y = 0;

				break;
			}
		}

		// horizontal

		switch (m_HorizontalAlignment)
		{

		case eHorizontalAlignment_Right:
			{
				for (std::vector<TextLine*>::iterator line_it = m_Lines.begin(); line_it != m_Lines.end(); ++line_it)
				{
					TextLine* line = *line_it;

					line->position.x += m_Dimensions.x - line->dimensions.x;
					line->position.y += offset_y;
				}

				break;
			}

		case eHorizontalAlignment_Center:
			{
				for (std::vector<TextLine*>::iterator line_it = m_Lines.begin(); line_it != m_Lines.end(); ++line_it)
				{
					TextLine* line = *line_it;

					line->position.x += (m_Dimensions.x - line->dimensions.x) / 2.0f;
					line->position.y += offset_y;
				}

				break;
			}

		case eHorizontalAlignment_Left:
			{
				for (std::vector<TextLine*>::iterator line_it = m_Lines.begin(); line_it != m_Lines.end(); ++line_it)
				{
					TextLine* line = *line_it;

					line->position.y += offset_y;
				}

				break;
			}

		}
	}

	bool TextLayout::_NextLine(bool a_HeightFixed)
	{
		if (m_SizeHint.y <= 0.0f && a_HeightFixed)
		{
			return false;
		}

		m_Cursor.x = 0.0f;

		glm::vec2 line_position = m_Cursor;

		if (m_Cursor.y + m_Face->GetLineHeight() > m_SizeHint.y && a_HeightFixed)
		{
			return false;
		}

		m_Cursor.y += m_Face->GetLineHeight();

		m_LineCurrent = new TextLine;
		m_LineCurrent->position = line_position;
		m_LineCurrent->dimensions = glm::vec2(0.0f, m_Face->GetLineHeight());

		m_Lines.push_back(m_LineCurrent);

		return true;
	}

	void TextLayout::_AddGlyphToCurrentLine(TextCharacter* a_Glyph)
	{
		m_LineCurrent->dimensions.x += a_Glyph->advance;
		a_Glyph->x = m_Cursor.x;
		m_Cursor.x += a_Glyph->advance;

		m_LineCurrent->characters.push_back(a_Glyph);
	}

}; // namespace ExLibri