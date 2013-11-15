#include "ExLibris.Test.PCH.h"

#include <Exception.h>
#include <Family.h>
#include <FontFace.h>
#include <Library.h>

#include "Mock.Font.h"
#include "Mock.FontLoader.h"

using namespace ExLibris;

TEST(Library, Construct)
{
	Library* lib = new Library;

	EXPECT_EQ(1, lib->GetFamilyCount());
}

TEST(Library, CreateFamily)
{
	Library* lib = new Library;

	Family* fam = lib->CreateFamily("Arial");

	ASSERT_NE(nullptr, fam);
	EXPECT_EQ(lib, fam->GetLibrary());
	EXPECT_STREQ("Arial", fam->GetName().c_str());

	EXPECT_EQ(2, lib->GetFamilyCount());
}

TEST(Library, CreateFamilyTwice)
{
	Library* lib = new Library;

	Family* fam = lib->CreateFamily("Papyrus");
	Family* fam_again = lib->CreateFamily("Papyrus");
	ASSERT_EQ(fam, fam_again);

	EXPECT_EQ(2, lib->GetFamilyCount());
}

TEST(Library, FindFamily)
{
	Library* lib = new Library;

	Family* fam = lib->CreateFamily("Comic Sans MS");

	Family* fam_found = lib->FindFamily("Comic Sans MS");
	ASSERT_EQ(fam, fam_found);
}

TEST(Library, FindFamilyEmpty)
{
	Library* lib = new Library;

	Family* fam = lib->FindFamily("Adobe Jensen Pro");
	ASSERT_EQ(nullptr, fam);
}

TEST(Library, FindFamilyNotFound)
{
	Library* lib = new Library;

	lib->CreateFamily("Magic Numbers");

	Family* fam = lib->FindFamily("Regular");
	ASSERT_EQ(nullptr, fam);
}

TEST(Library, RequestFontFace)
{
	Library* lib = new Library;

	Family* fam = lib->CreateFamily("Roboto");
	MockFont* fnt = new MockFont(fam);

	FaceRequest fr;
	fr.SetFamilyName("Roboto");
	fr.SetSize(16.0f);

	FontFace* face = lib->RequestFontFace(fr);
	EXPECT_FLOAT_EQ(16.0f, face->GetSize());
	EXPECT_STREQ("Roboto", face->GetFamily()->GetName().c_str());
}

TEST(Library, RequestFontFaceEmpty)
{
	Library* lib = new Library;

	FaceRequest fr;

	FontFace* face = lib->RequestFontFace(fr);
	EXPECT_STREQ("System", face->GetFamily()->GetName().c_str());
	EXPECT_FLOAT_EQ(12.0f, face->GetSize());
}

TEST(Library, RequestFaceFamilyNotFound)
{
	Library* lib = new Library;

	FaceRequest fr;
	fr.SetFamilyName("Comic Sans MS");

	FontFace* face = nullptr;
	
	EXPECT_THROW({
		face = lib->RequestFontFace(fr);
	}, Exception);

	EXPECT_EQ(nullptr, face);
}

TEST(Library, RequestFaceFontNotFound)
{
	Library* lib = new Library;

	Family* fam = lib->CreateFamily("Gravity");

	FaceRequest fr;
	fr.SetFamilyName("Gravity");
	fr.SetWeight(eWeight_Bold);

	FontFace* face = nullptr;
	
	EXPECT_THROW({
		face = lib->RequestFontFace(fr);
	}, Exception);

	EXPECT_EQ(nullptr, face);
}

class LibraryFontMappingContext
	: public ::testing::Test
{

public:

	void SetUp()
	{
		library = new Library;

		loader = new MockFontLoader(library);
		library->AddLoader(loader);
	}

	void TearDown()
	{
		delete library;
	}

protected:

	Library* library;
	MockFontLoader* loader;

};

TEST_F(LibraryFontMappingContext, MapFontAndFindFamily)
{
	std::stringstream ss;
	EXPECT_TRUE(library->MapFontToStream(ss));

	Family* fam = library->FindFamily("MockFamily");
	ASSERT_NE(nullptr, fam);

	EXPECT_EQ(1, fam->GetGlyphProviderCount());
}

TEST_F(LibraryFontMappingContext, FailedToMapFont)
{
	loader->stream_not_valid = true;

	std::stringstream ss;
	EXPECT_FALSE(library->MapFontToStream(ss));
}

TEST_F(LibraryFontMappingContext, RequestFace)
{
	std::stringstream ss;
	EXPECT_TRUE(library->MapFontToStream(ss));

	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(16.0f);
	fr.SetWeight(eWeight_Normal);
	fr.SetStyle(eStyle_None);

	Face* face = library->RequestFace(fr);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_FLOAT_EQ(16.0f, face->GetSize());
	EXPECT_FLOAT_EQ(16.0f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(16.0f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(16.0f / 3.0f), face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceFamilyNotFound)
{
	std::stringstream ss;
	EXPECT_TRUE(library->MapFontToStream(ss));

	FaceRequest fr;
	fr.SetFamilyName("AwesomeBro");

	EXPECT_THROW({
		Face* face = library->RequestFace(fr);
	}, Exception);
}