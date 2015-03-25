/**
 * @file test.cpp
 * testing quicksort functions.
 *
 * cxx -std=c++11 -O3 qsort_smasher.cpp test.cpp -o test -lm
 */

#include <algorithm>
#include <iostream>
#include <fstream>
#include "qsort_smasher.hpp"
#include "build_qsort.hpp"
#include "qsort.hpp"

void qsort_v1_wrap(int* first, int* last)
{	qsort_v1(first, last);
}

void qsort_v2_wrap(int* first, int* last)
{	qsort_v2(first, last);
}

void qsort_v3_wrap(int* first, int* last)
{	qsort_v3(first, last);
}

void qsort_v4_wrap(int* first, int* last)
{	qsort_v4(first, last);
}

void qsort_v5_wrap(int* first, int* last)
{	qsort_v5(first, last);
}

void qsort_v6_wrap(int* first, int* last)
{	qsort_v6(first, last);
}

int mainloop(int argc, char* argv[])
{
	// functions.
	std::vector<std::shared_ptr<Function>> functions {
		Function::create(qsort_v1_wrap, "qsort_v1"),
		Function::create(qsort_v2_wrap, "qsort_v2"),
		Function::create(qsort_v3_wrap, "qsort_v3"),
		Function::create(qsort_v4_wrap, "qsort_v4"),
		Function::create(qsort_v5_wrap, "qsort_v5"),
		Function::create(qsort_v6_wrap, "qsort_v6"),
		Function::create(algo::qsort<int*>, "algo::qsort"),
		Function::create(std::sort<int*>, "std::sort")
	};

	// smasher.
#ifndef TEST_WANT_FILE_OUTPUT
	smasher_ui(functions);
#else
	const char* path = "./testlog.txt";
	std::ofstream fout(path);
	if (fout)
	   smasher_ui(functions, fout);
	else
	 {
	   std::cerr << __FILE__ << ": error: unable to open `" << path << "'";
	   std::cerr << std::endl;
	   return -1;
	 }
#endif
	return 0;
}

int main(int argc, char* argv[])
{
	return mainloop(argc, argv);
}

