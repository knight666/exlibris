#include "ExLibris.Test.PCH.h"

#include <fstream>

TEST(Streambuf, ReadLineFromFile)
{
	std::fstream stream("Tests/hello.txt", std::ios::in);
	ASSERT_TRUE(stream.is_open());

	std::basic_streambuf<char>* stream_buffer = stream.rdbuf();
	ASSERT_NE(nullptr, stream_buffer);

	std::istream in(stream_buffer);

	char line[1024] = { 0 };
	in.getline(line, 1024, '\n');

	EXPECT_STREQ("Thank you for using ExLibris!", line);
}

TEST(Streambuf, ReadAndGetOffset)
{
	std::fstream stream("Tests/hello.txt", std::ios::in);
	ASSERT_TRUE(stream.is_open());

	std::basic_streambuf<char>* stream_buffer = stream.rdbuf();
	ASSERT_NE(nullptr, stream_buffer);

	std::istream in(stream_buffer);

	unsigned int data = 0;
	in.read((char*)&data, 4);

	EXPECT_EQ(4, in.tellg());
}

TEST(Streambuf, ReadAll)
{
	std::fstream stream("Tests/hello.txt", std::ios::in);
	ASSERT_TRUE(stream.is_open());

	std::basic_streambuf<char>* stream_buffer = stream.rdbuf();
	ASSERT_NE(nullptr, stream_buffer);

	std::istream in(stream_buffer);

	in.seekg(0, std::ios::end);
	std::size_t file_size = (std::size_t)in.tellg();

	EXPECT_EQ(29, file_size);

	in.seekg(0, std::ios::beg);

	char* file_data = new char[file_size + 1];
	std::memset(file_data, 0, file_size + 1);
	in.read(file_data, file_size);

	EXPECT_STREQ("Thank you for using ExLibris!", file_data);
}