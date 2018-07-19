// distance dev.cpp : Defines the entry point for the console application.

#include <iostream>

#include <distant/virtual_memory.hpp>

#define NOMINMAX

// a a a a - - - - b b b b b b b b c c - - - - - -
//struct test5 { int a; double b; short c; };

//struct test1 { short a; char b; char* c; short d; char e; };
//using test1_tuple = std::tuple<short, char, char*, short, char>;
// a a b - c c c c d d e -

//using tuple_t = std::tuple<short, char, char*, short, char>;
// c c - - p p p p s s c -
//sizeof(tuple_t) == 16

// a a a a b b b b c c - - - - - - x x x x x x x x y y y y y y y y z z z z - - - -

struct test1 { short a; char b; char* c; short d; char e; };
using test1_tuple = std::tuple<short, char, char*, short, char>;

struct test2 { char a; char b; short c; int d; };
using test2_tuple = std::tuple<char, char, short, int>;

struct test3 { char a; short b; int c; char d; };
using test3_tuple = std::tuple<char, short, int, char>;

struct test4 { char a; short b; char c; int d; };
using test4_tuple = std::tuple<char, short, char, int>;

struct test5 { int a; double b; short c; };
using test5_tuple = std::tuple<int, double, short>;

struct test6 { int a; int b; short c; double d; double e; int f; };
using test6_tuple = std::tuple<int, int, short, double, double, int>;

#define TEST_ALIGNED_OFFSET_OF(test, i, field) \
	if (offsetof(test, field) != aligned::aligned_offset_of<i, test ##_tuple>()) std::cout << "[ERROR] Test failed on " #test " index " #i "\n\n";

#define DISPLAY_ALIGNED_OFFSET_OF(test, i, field)\
	std::cout << #test << "[" << i << "]: " << offsetof(test, field) << ", " << aligned::aligned_offset_of<i, test ##_tuple>() << '\n';

int main()
{
	using namespace distant;
	using namespace utility;

	auto current = distant::current_process();

	TEST_ALIGNED_OFFSET_OF(test1, 0, a);
	TEST_ALIGNED_OFFSET_OF(test1, 1, b);
	TEST_ALIGNED_OFFSET_OF(test1, 2, c);
	TEST_ALIGNED_OFFSET_OF(test1, 3, d);
	TEST_ALIGNED_OFFSET_OF(test1, 4, e);

	TEST_ALIGNED_OFFSET_OF(test2, 0, a);
	TEST_ALIGNED_OFFSET_OF(test2, 1, b);
	TEST_ALIGNED_OFFSET_OF(test2, 2, c);
	TEST_ALIGNED_OFFSET_OF(test2, 3, d);

	TEST_ALIGNED_OFFSET_OF(test3, 0, a);
	TEST_ALIGNED_OFFSET_OF(test3, 1, b);
	TEST_ALIGNED_OFFSET_OF(test3, 2, c);
	TEST_ALIGNED_OFFSET_OF(test3, 3, d);

	TEST_ALIGNED_OFFSET_OF(test4, 0, a);
	TEST_ALIGNED_OFFSET_OF(test4, 1, b);
	TEST_ALIGNED_OFFSET_OF(test4, 2, c);
	TEST_ALIGNED_OFFSET_OF(test4, 3, d);

	TEST_ALIGNED_OFFSET_OF(test5, 0, a);
	TEST_ALIGNED_OFFSET_OF(test5, 1, b);
	TEST_ALIGNED_OFFSET_OF(test5, 2, c);

	TEST_ALIGNED_OFFSET_OF(test6, 0, a);
	TEST_ALIGNED_OFFSET_OF(test6, 1, b);
	TEST_ALIGNED_OFFSET_OF(test6, 2, c);
	TEST_ALIGNED_OFFSET_OF(test6, 3, d);
	TEST_ALIGNED_OFFSET_OF(test6, 4, e);
	TEST_ALIGNED_OFFSET_OF(test6, 5, f);

	DISPLAY_ALIGNED_OFFSET_OF(test1, 1, b)

	return 0;
}
