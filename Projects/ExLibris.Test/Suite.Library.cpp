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

TEST(Library, MapFontAndFindFamily)
{
	Library* library = new Library;

	MockFontLoader* loader = new MockFontLoader(library);
	library->AddLoader(loader);

	std::stringstream ss;
	EXPECT_TRUE(library->MapFontToStream(ss));

	Family* fam = library->FindFamily("MockFamily");
	ASSERT_NE(nullptr, fam);

	EXPECT_EQ(1, fam->GetGlyphProviderCount());
}

TEST(Library, FailedToMapFont)
{
	Library* library = new Library;

	MockFontLoader* loader = new MockFontLoader(library);
	loader->stream_not_valid = true;

	library->AddLoader(loader);

	std::stringstream ss;
	EXPECT_FALSE(library->MapFontToStream(ss));
}

class LibraryFontMappingContext
	: public ::testing::Test
{

public:

	void SetUp()
	{
		library = new Library;
		provider = new MockGlyphProvider(library);

		family = library->FindFamily("MockFamily");
		ASSERT_NE(nullptr, family);

		family->AddGlyphProvider(provider);
	}

	void TearDown()
	{
		delete library;
	}

protected:

	Library* library;
	Family* family;
	MockGlyphProvider* provider;

};

TEST_F(LibraryFontMappingContext, RequestFace)
{
	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(16.0f);
	fr.SetWeight(eWeight_Normal);
	fr.SetStyle(eStyle_None);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(16.0f, face->GetSize());
	EXPECT_FLOAT_EQ(16.0f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(16.0f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(16.0f / 3.0f), face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceDefault)
{
	FaceRequest fr;

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("System", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(12.0f, face->GetSize());
	EXPECT_FLOAT_EQ(16.0f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(0.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(0.0f, face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceFamilyNotFound)
{
	FaceRequest fr;
	fr.SetFamilyName("AwesomeBro");

	EXPECT_THROW({
		Face* face = library->RequestFace(fr);
	}, Exception);
}

TEST_F(LibraryFontMappingContext, RequestFaceFamilyNotSpecified)
{
	FaceRequest fr;
	fr.SetSize(25.5f);
	fr.SetWeight(eWeight_Normal);
	fr.SetStyle(eStyle_None);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("System", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(12.0f, face->GetSize());
	EXPECT_FLOAT_EQ(16.0f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(0.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(0.0f, face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceSizeNotAvailable)
{
	provider->size_blacklist.insert(34.97f);

	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(34.97f);
	fr.SetWeight(eWeight_Normal);
	fr.SetStyle(eStyle_None);

	Face* face = library->RequestFace(fr);
	EXPECT_EQ(nullptr, face);
}

TEST_F(LibraryFontMappingContext, RequestFaceSizeNotSpecified)
{
	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetWeight(eWeight_Normal);
	fr.SetStyle(eStyle_None);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(10.0f, face->GetSize());
	EXPECT_FLOAT_EQ(10.0f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(10.0f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(10.0f / 3.0f), face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceWeightNotSpecified)
{
	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(118.8f);
	fr.SetStyle(eStyle_None);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(118.8f, face->GetSize());
	EXPECT_FLOAT_EQ(118.8f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(118.8f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(118.8f / 3.0f), face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceWeightMatchOnlyAvailable)
{
	provider->SetWeight(eWeight_Bold);

	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(35.0f);
	fr.SetWeight(eWeight_Normal);
	fr.SetStyle(eStyle_None);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Bold, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(35.0f, face->GetSize());
	EXPECT_FLOAT_EQ(35.0f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(35.0f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(35.0f / 3.0f), face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceWeightBestMatch)
{
	MockGlyphProvider* provider_bestmatch = new MockGlyphProvider(library);
	provider_bestmatch->SetWeight(eWeight_Bold);

	family->AddGlyphProvider(provider_bestmatch);

	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(44.42f);
	fr.SetWeight(eWeight_Bold);
	fr.SetStyle(eStyle_None);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Bold, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(44.42f, face->GetSize());
	EXPECT_FLOAT_EQ(44.42f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(44.42f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(44.42f / 3.0f), face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceStyleNotSpecified)
{
	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(28.5f);
	fr.SetWeight(eWeight_Normal);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(28.5f, face->GetSize());
	EXPECT_FLOAT_EQ(28.5f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(28.5f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(28.5f / 3.0f), face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceStyleMatchOnlyAvailable)
{
	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(15.8f);
	fr.SetWeight(eWeight_Normal);
	fr.SetStyle(eStyle_Italicized);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(15.8f, face->GetSize());
	EXPECT_FLOAT_EQ(15.8f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(15.8f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(15.8f / 3.0f), face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceStyleBestMatch)
{
	MockGlyphProvider* provider_bestmatch = new MockGlyphProvider(library);
	provider_bestmatch->SetStyle(eStyle_Italicized);

	family->AddGlyphProvider(provider_bestmatch);

	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(178.8f);
	fr.SetWeight(eWeight_Normal);
	fr.SetStyle(eStyle_Italicized);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_Italicized, face->GetStyle());
	EXPECT_FLOAT_EQ(178.8f, face->GetSize());
	EXPECT_FLOAT_EQ(178.8f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(178.8f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(178.8f / 3.0f), face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceWeightAndStyleMatchOnlyAvailable)
{
	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(12.8f);
	fr.SetWeight(eWeight_Bold);
	fr.SetStyle(eStyle_Italicized);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_None, face->GetStyle());
	EXPECT_FLOAT_EQ(12.8f, face->GetSize());
	EXPECT_FLOAT_EQ(12.8f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(12.8f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(12.8f / 3.0f), face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceWeightAndStyleMatchWeightButNotStyle)
{
	MockGlyphProvider* provider_match = new MockGlyphProvider(library);
	provider_match->SetWeight(eWeight_Bold);
	provider_match->SetStyle(eStyle_Italicized);

	family->AddGlyphProvider(provider_match);

	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(17.86f);
	fr.SetWeight(eWeight_Bold);
	fr.SetStyle(eStyle_None);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Bold, face->GetWeight());
	EXPECT_EQ(eStyle_Italicized, face->GetStyle());
	EXPECT_FLOAT_EQ(17.86f, face->GetSize());
	EXPECT_FLOAT_EQ(17.86f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(17.86f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(17.86f / 3.0f), face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceWeightAndStyleMatchStyleButNotWeight)
{
	MockGlyphProvider* provider_match = new MockGlyphProvider(library);
	provider_match->SetWeight(eWeight_Normal);
	provider_match->SetStyle(eStyle_Italicized);

	family->AddGlyphProvider(provider_match);

	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(47.98f);
	fr.SetWeight(eWeight_Bold);
	fr.SetStyle(eStyle_Italicized);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Normal, face->GetWeight());
	EXPECT_EQ(eStyle_Italicized, face->GetStyle());
	EXPECT_FLOAT_EQ(47.98f, face->GetSize());
	EXPECT_FLOAT_EQ(47.98f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(47.98f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(47.98f / 3.0f), face->GetDescent());
}

TEST_F(LibraryFontMappingContext, RequestFaceWeightAndStyleBestMatch)
{
	MockGlyphProvider* provider_style = new MockGlyphProvider(library);
	provider_style->SetWeight(eWeight_Normal);
	provider_style->SetStyle(eStyle_Italicized);

	family->AddGlyphProvider(provider_style);

	MockGlyphProvider* provider_weight_and_style = new MockGlyphProvider(library);
	provider_weight_and_style->SetWeight(eWeight_Bold);
	provider_weight_and_style->SetStyle(eStyle_Italicized);

	family->AddGlyphProvider(provider_weight_and_style);

	FaceRequest fr;
	fr.SetFamilyName("MockFamily");
	fr.SetSize(45.8f);
	fr.SetWeight(eWeight_Bold);
	fr.SetStyle(eStyle_Italicized);

	Face* face = library->RequestFace(fr);
	ASSERT_NE(nullptr, face);

	ASSERT_NE(nullptr, face->GetFamily());
	EXPECT_STREQ("MockFamily", face->GetFamily()->GetName().c_str());
	EXPECT_EQ(eWeight_Bold, face->GetWeight());
	EXPECT_EQ(eStyle_Italicized, face->GetStyle());
	EXPECT_FLOAT_EQ(45.8f, face->GetSize());
	EXPECT_FLOAT_EQ(45.8f * 1.5f, face->GetLineHeight());
	EXPECT_FLOAT_EQ(45.8f / 2.0f, face->GetAscent());
	EXPECT_FLOAT_EQ(-(45.8f / 3.0f), face->GetDescent());
}