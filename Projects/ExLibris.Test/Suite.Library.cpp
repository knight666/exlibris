#include "ExLibris.Test.PCH.h"

#include <Family.h>
#include <Library.h>

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