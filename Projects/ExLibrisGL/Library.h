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

#pragma once

#include "FaceRequest.h"

namespace ExLibris
{
	class Face;
	class Family;
	class FontFace;
	class IFont;
	class IFontLoader;
	class IGlyphProvider;
}

namespace ExLibris
{

	class Library
	{
	
	public:
	
		Library();
		~Library();

		size_t GetLoaderCount() const;
		void AddLoader(IFontLoader* a_Loader);

		IFont* LoadFont(const std::string& a_Path);
		IFont* LoadFont(std::istream& a_Stream);

		bool MapFontToFile(const std::string& a_Path) const;
		bool MapFontToStream(std::istream& a_Stream) const;

		size_t GetFamilyCount() const;
		Family* CreateFamily(const std::string& a_Name);
		Family* FindFamily(const std::string& a_Name) const;

		FontFace* RequestFontFace(const FaceRequest& a_Request);

		Face* RequestFace(const FaceRequest& a_Request) const;

	private:

		std::vector<IFontLoader*> m_Loaders;
		std::map<std::string, Family*> m_Families;
	
	}; // class Library

}; // namespace ExLibris