/**
 * @file qsort_smasher.cpp
 * test suite that generates problematic sequences for quicksort.
 */

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <chrono>

#include <cassert>
#include <cmath>
#include <ctime>

#include "qsort_smasher.hpp"

// utility.
template <typename ...Tp>
  void unused(Tp&&...)
  {}

// Function.
std::shared_ptr<Function> Function::create(pointer p, const std::string& s)
{
	assert(p != nullptr);
	return std::shared_ptr<Function>(new Function(p, s));
}

Function::Function(pointer p, const std::string& s)
	: m_func(p), m_desc(s)
{}

inline Function::pointer Function::func() const
{	return m_func;
}

inline const std::string& Function::desc() const
{	return m_desc;
}

namespace // private.
{
// Strategy (sequence generation).
struct Strategy {
	virtual ~Strategy() {}
	virtual void init() = 0;
	virtual int generate(int n, int m, int i) = 0;
	virtual const char* desc() const = 0;
};

struct Sawtooth : public Strategy {
	void init() {}
	int generate(int n, int m, int i)
		{ return i % m; unused(n);
		}
	const char* desc() const
		{ return "sawtooth";
		}
};

struct Random : public Strategy {
	void init() {}
	int generate(int n, int m, int i)
		{ return std::rand() % m; unused(n, i);
		}
	const char* desc() const
		{ return "random";
		}
};

struct Stagger : public Strategy {
	void init() {}
	int generate(int n, int m, int i)
		{ return (i*m + 1) % n;
		}
	const char* desc() const
		{ return "stagger";
		}
};

struct Plateau : public Strategy {
	void init() {}
	int generate(int n, int m, int i)
		{ return std::min(i, m); unused(n);
		}
	const char* desc() const
		{ return "plateau";
		}
};

struct Shuffle : public Strategy {
	void init()
		{ j = 0; k = 1;
		}
	int generate(int n, int m, int i)
		{ return (std::rand() % m) ? (j+=2) : (k+=2); unused(n, i);
		}
	const char* desc() const
		{ return "shuffle";
		}
private:
	int j, k;
};

// smasher.
double mticks()
{
	typedef std::chrono::duration<double, std::milli> duration;
	typedef std::chrono::high_resolution_clock clock;

	static clock::time_point start = clock::now();
	duration elapsed = clock::now() - start;
	return elapsed.count();
}

void write_header(std::ostream& outstream)
{
	std::string s0 = "N";
	std::string s1 = "M";
	std::string s2 = "strategy";
	std::string s3 = "variant";
	std::string s4 = "millisec";

	for ( int i = 0; i < 2; i++ )
	 {
	   outstream << std::setw(10) << std::left << s0;
	   outstream << std::setw(10) << std::left << s1;
	   outstream << std::setw(12) << std::left << s2;
	   outstream << std::setw(12) << std::left << s3;
	   outstream << std::setw(16) << std::left << s4;
	 }
	outstream << std::endl;

	for ( int i = 0; i < 2; i++ )
	 {
	   outstream << std::setw(10) << std::left << std::string(s0.size(), '-');
	   outstream << std::setw(10) << std::left << std::string(s1.size(), '-');
	   outstream << std::setw(12) << std::left << std::string(s2.size(), '-');
	   outstream << std::setw(12) << std::left << std::string(s3.size(), '-');
	   outstream << std::setw(16) << std::left << std::string(s4.size(), '-');
	 }
	outstream << std::endl;
}

void write_result(int n, int m, const std::string& strategy,
	const std::string& variant, double millisec, bool newline,
	std::ostream& outstream)
{
	outstream << std::setw(10) << std::left << n;
	outstream << std::setw(10) << std::left << m;
	outstream << std::setw(12) << std::left << strategy;
	outstream << std::setw(12) << std::left << variant;
	outstream << std::setw(16) << std::left << std::fixed << millisec;
	if (newline)
	   outstream << std::endl;
}

void smasher(const std::vector<std::shared_ptr<Strategy>>& strategies,
	Function::pointer sort_fn, int nmax, std::ostream& outstream)
{
	double test_time_total = 0.0;
	for ( int n = 10; n <= nmax; n *= 10 )
	 {
	   std::unique_ptr<int> base_ptr(new int[n]);
	   std::unique_ptr<int> ctrl_ptr(new int[n]);
	   std::unique_ptr<int> test_ptr(new int[n]);

	   for ( auto strategy : strategies )
	    {
	      int* base = base_ptr.get();
	      int* ctrl = ctrl_ptr.get();
	      int* test = test_ptr.get();

	      write_header(outstream);

	      double test_time = 0.0;
	      for ( int m = 1; m < n*2; m *= 2 )
	       {
	         double t;
	         // generate strategy sequence.
	         strategy->init();
	         for ( int i = 0; i < n; i++ )
	            base[i] = strategy->generate(n, m, i);

	         // copy and sort control.
	         std::copy(base, base + n, ctrl);
	         std::sort(ctrl, ctrl + n);

	         // identity.
	         std::copy(base, base + n, test);
	         t = mticks(); /**/ sort_fn(test, test + n);
	         t = mticks() - t; /**/ test_time += t;
	         assert(std::equal(test, test + n, ctrl));
	         write_result(n, m, strategy->desc(), "ident", t, false,
	                 outstream);

	         // reversed.
	         std::copy(base, base + n, test);
	         std::reverse(test, test + n);
	         t = mticks(); /**/ sort_fn(test, test + n);
	         t = mticks() - t; /**/ test_time += t;
	         assert(std::equal(test, test + n, ctrl));
	         write_result(n, m, strategy->desc(), "reverse", t, true,
	                 outstream);

	         // reverse front.
	         std::copy(base, base + n, test);
	         std::reverse(test, test + n/2);
	         t = mticks(); /**/ sort_fn(test, test + n);
	         t = mticks() - t; /**/ test_time += t;
	         assert(std::equal(test, test + n, ctrl));
	         write_result(n, m, strategy->desc(), "revfront", t, false,
	                 outstream);

	         // reverse back.
	         std::copy(base, base + n, test);
	         std::reverse(test + n/2, test + n);
	         t = mticks(); /**/ sort_fn(test, test + n);
	         t = mticks() - t; /**/ test_time += t;
	         assert(std::equal(test, test + n, ctrl));
	         write_result(n, m, strategy->desc(), "revback", t, true,
	                 outstream);

	         // sorted.
	         std::copy(base, base + n, test);
	         std::sort(test, test + n);
	         t = mticks(); /**/ sort_fn(test, test + n);
	         t = mticks() - t; /**/ test_time += t;
	         assert(std::equal(test, test + n, ctrl));
	         write_result(n, m, strategy->desc(), "sorted", t, false,
	                 outstream);

	         // dither.
	         std::copy(base, base + n, test);
	         for ( int i = 0; i < n; i++ ) test[i] += i % 5;
	         // this modifies the base sequence so we need to copy
	         // and sort control again.
	         std::copy(test, test + n, ctrl);
	         std::sort(ctrl, ctrl + n);
	         t = mticks(); /**/ sort_fn(test, test + n);
	         t = mticks() - t; /**/ test_time += t;
	         assert(std::equal(test, test + n, ctrl));
	         write_result(n, m, strategy->desc(), "dither", t, true,
	                 outstream);
	       }
	      test_time_total += test_time;
	      outstream << "test time: " << test_time/1000 << " s.";
	      outstream << std::endl;
	      outstream << std::endl;
	    }
	 }
	outstream << "test time total: " << test_time_total/1000 << " s.";
	outstream << std::endl;
}

// smasher UI.
void boxed_text(const std::string& msg, char border)
{
	std::string line(msg.size()+4, border);
	std::cout << line;
	std::cout << std::endl;
	std::cout << border << ' ' << msg << ' ' << border;
	std::cout << std::endl;
	std::cout << line;
}

int smasher_ui_getopt(const std::string& msg)
{
	std::cout << msg << "> " << std::flush;
	int opt;
	if (std::cin >> opt)
	   return opt;
	std::cin.clear();
	std::cin.ignore(0x7FFF, '\n');
	return -1;
}

int smasher_ui(const std::vector<std::shared_ptr<Strategy>>& strategies,
	const std::vector<std::shared_ptr<Function>>& functions,
	std::ostream& outstream)
{
	int size, option;
	boxed_text("Welcome to the quicksort smasher.", '*');
	std::cout << std::endl;

	// select function.
	size = functions.size();
	while ( true )
	 {
	   std::cout << std::endl;
	   for ( int i = 0; i < size; i++ )
	      std::cout << i+1 << ") " << functions[i]->desc() << std::endl;
	   std::cout << "0) quit" << std::endl;
	   option = smasher_ui_getopt("Please select a function");
	   if (option >= 0 && option <= size )
	      break;
	   std::cout << "Oops, bad option, try again." << std::endl;
	 }

	if (option == 0)
	   return -1;
	std::shared_ptr<Function> optfunc = functions[option-1];
	std::cout << "Using function " << optfunc->desc();
	std::cout << "." << std::endl;

	// select strategy.
	size = strategies.size();
	while ( true )
	 {
	   std::cout << std::endl;
	   for ( int i = 0; i < size; i++ )
	      std::cout << i+1 << ") " << strategies[i]->desc() << std::endl;
	   std::cout << size+1 << ") all" << std::endl;
	   std::cout << "0) quit" << std::endl;
	   option = smasher_ui_getopt("Please select a strategy");
	   if (option >= 0 && option <= size+1 )
	      break;
	   std::cout << "Oops, bad option, try again." << std::endl;
	 }

	if (option == 0)
	   return -1;
	std::vector<std::shared_ptr<Strategy>> optstrat;
	if (option != size+1)
	   optstrat.push_back(strategies[option-1]);
	else
	   optstrat = strategies;

	size = optstrat.size();
	for ( int i = 0; i < size; i++ )
	 {
	   std::cout << "Using strategy " << optstrat[i]->desc();
	   std::cout << "." << std::endl;
	 }

	// select maximum test size.
	size = 8;
	while ( true )
	 {
	   std::cout << std::endl;
	   for ( int i = 1; i < size; i++ )
	      std::cout << i << ") " << (int) std::pow(10, i) << std::endl;
	   std::cout << "0) quit" << std::endl;
	   option = smasher_ui_getopt("Please select maximum size");
	   if (option >= 0 && option < size )
	      break;
	   std::cout << "Oops, bad option, try again." << std::endl;
	 }

	if (option == 0)
	   return -1;
	int optsize = (int) std::pow(10, option);

	if (outstream != std::cout)
	 {
	   std::cout << "Starting test, please wait..." << std::endl;
	   outstream << "Function: " << optfunc->desc() << std::endl;
	   outstream << std::endl;
	 }

	// smash.
	std::srand(std::time(0));
	smasher(optstrat, optfunc->func(), optsize, outstream);

	if (outstream != std::cout)
	   std::cout << "Result output to user stream." << std::endl;

	std::cout << std::endl;
	std::cout << "Thank you for using the quicksort smasher." << std::endl;
	return 0;
}
} // end private.

void smasher_ui(const std::vector<std::shared_ptr<Function>>& functions,
	std::ostream& outstream)
{
	// strategies.
	std::vector<std::shared_ptr<Strategy>> strategies {
		std::shared_ptr<Strategy>(new Random),
		std::shared_ptr<Strategy>(new Sawtooth),
		std::shared_ptr<Strategy>(new Stagger),
		std::shared_ptr<Strategy>(new Plateau),
		std::shared_ptr<Strategy>(new Shuffle)
	};
	int result = smasher_ui(strategies, functions, outstream);
	if (result != 0)
	   std::cout << "Quit detected. Goodbye." << std::endl;
}

void smasher_ui(const std::vector<std::shared_ptr<Function>>& functions)
{
	smasher_ui(functions, std::cout);
}

