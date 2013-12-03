#pragma once

#include <IFontLoader.h>

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
			, family_name("MockFamily")
			, weight(eWeight_Normal)
			, style(eStyle_None)
		{
		}

		IGlyphProvider* LoadGlyphProvider(std::istream& a_Stream)
		{
			if (stream_not_valid)
			{
				return nullptr;
			}

			MockGlyphProvider* provider = new MockGlyphProvider(m_Library, family_name);
			provider->SetWeight(weight);
			provider->SetStyle(style);

			return provider;
		}

	public:

		bool stream_not_valid;
		std::string family_name;
		Weight weight;
		Style style;
	
	}; // class MockFontLoader

}; // namespace ExLibris