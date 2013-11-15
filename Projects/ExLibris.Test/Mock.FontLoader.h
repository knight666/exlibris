#pragma once

#include <IFontLoader.h>

#include "Mock.Font.h"
#include "Mock.GlyphProvider.h"

namespace ExLibris
{

	class MockFontLoader
		: public IFontLoader
	{
	
	public:
	
		MockFontLoader(Library* a_Library)
			: IFontLoader(a_Library)
			, stream_not_valid(false)
		{
		}
	
		IFont* LoadFont(std::istream& a_Stream)
		{
			if (stream_not_valid)
			{
				return nullptr;
			}

			return new MockFont(m_Library->CreateFamily("MockFamily"));
		}

		IGlyphProvider* LoadGlyphProvider(std::istream& a_Stream)
		{
			if (stream_not_valid)
			{
				return nullptr;
			}

			return new MockGlyphProvider(m_Library);
		}

	public:

		bool stream_not_valid;
	
	}; // class MockFontLoader

}; // namespace ExLibris