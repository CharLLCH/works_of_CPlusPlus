/**
 * @file qsort_smasher.hpp
 * test suite that generates problematic sequences for quicksort.
 */

#ifndef _qsort_smasher_mm_hpp_
#define _qsort_smasher_mm_hpp_ 1

#include <ostream>
#include <memory>
#include <string>
#include <vector>

/** @brief wrap function pointer and text descriptor. */
class Function {
public:
	typedef void (*pointer)(int*, int*);
	static std::shared_ptr<Function> create(pointer, const std::string&);

	pointer func() const;
	const std::string& desc() const;
private:
	Function(pointer, const std::string&);

	pointer m_func;
	std::string m_desc;
};

/**
 * @brief quicksort smasher user interface.
 *
 * @param  fn  vector of shared_ptr to Function.
 * @param  os  output stream.
 */
extern void smasher_ui(const std::vector<std::shared_ptr<Function>>& fn,
	std::ostream& os);

/**
 * @brief quicksort smasher user interface.
 *
 * @param  fn  vector of shared_ptr to Function.
 * @note output to std::cout.
 */
extern void smasher_ui(const std::vector<std::shared_ptr<Function>>& fn);

#endif // _qsort_smasher_mm_hpp_

