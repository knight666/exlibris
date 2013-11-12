#include "ExLibris.Test.PCH.h"

#include <Exception.h>
#include <Family.h>
#include <FontFace.h>
#include <Library.h>

#include "Mock.Font.h"

using namespace ExLibris;

TEST(Library, Construct)
{
	Library* lib = new Library;

	EXPECT_EQ(0, lib->GetFamilyCount());
}

TEST(Library, CreateFamily)
{
	Library* lib = new Library;

	Family* fam = lib->CreateFamily("Arial");

	ASSERT_NE(nullptr, fam);
	EXPECT_EQ(lib, fam->GetLibrary());
	EXPECT_STREQ("Arial", fam->GetName().c_str());

	EXPECT_EQ(1, lib->GetFamilyCount());
}

TEST(Library, CreateFamilyTwice)
{
	Library* lib = new Library;

	Family* fam = lib->CreateFamily("Papyrus");
	Family* fam_again = lib->CreateFamily("Papyrus");
	ASSERT_EQ(fam, fam_again);

	EXPECT_EQ(1, lib->GetFamilyCount());
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

TEST(Library, RequestFace)
{
	Library* lib = new Library;

	Family* fam = lib->CreateFamily("Roboto");
	MockFont* fnt = new MockFont(fam);

	FaceRequest fr;
	fr.SetFamilyName("Roboto");
	fr.SetSize(16.0f);

	FontFace* face = lib->RequestFace(fr);
	EXPECT_FLOAT_EQ(16.0f, face->GetSize());
	EXPECT_STREQ("Roboto", face->GetFamily()->GetName().c_str());
}

TEST(Library, RequestFaceFamilyNotFound)
{
	Library* lib = new Library;

	FaceRequest fr;
	fr.SetFamilyName("Comic Sans MS");

	FontFace* face = nullptr;
	
	EXPECT_THROW({
		face = lib->RequestFace(fr);
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
		face = lib->RequestFace(fr);
	}, Exception);

	EXPECT_EQ(nullptr, face);
}