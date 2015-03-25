/**
 * @file qsort.hpp
 * quick sort implementation.
 *
 * - the quicksort algorithm's average case complexity is n*log(n), however,
 *   worst case is quadratic. this implements introspective sort which will
 *   guarantee n*log(n) behavior for any sequence.
 * - this is an unstable sort.
 */

#ifndef _qsort_mm_hpp_
#define _qsort_mm_hpp_ 1

#include <algorithm>
#include <iterator>
#include <cmath> // std::log2

namespace algo
{
template<typename In, typename Cmp>
  In iter_median_3_(In a, In b, In c, Cmp comp)
  {
	if (comp(*a, *b))
	 {
	   if (comp(*b, *c)) return b;
	   if (comp(*a, *c)) return c;
	   return a;
	 }
	if (comp(*a, *c)) return a;
	if (comp(*b, *c)) return c;
	return b;
  }

template<typename Ran, typename Cmp>
  inline Ran iter_median_(Ran first, Ran split, Ran last, Cmp comp)
  {
	typename std::iterator_traits<Ran>::difference_type threshold = 80;

	if (last - first <= threshold)
	   return algo::iter_median_3_(first, split, last-1, comp);

	auto d = (last - first) / 8;
	Ran a = algo::iter_median_3_(first, first+d, first+d*2, comp);
	Ran b = algo::iter_median_3_(split-d, split, split+d, comp);
	Ran c = algo::iter_median_3_(last-d*2, last-d, last-1, comp);
	return  algo::iter_median_3_(a, b, c, comp);
  }

template<typename Ran, typename Cmp>
  Ran partition_unguarded_(Ran first, Ran last,
	typename std::iterator_traits<Ran>::value_type pivot, Cmp comp)
  {
	while ( true )
	 {
	   while ( comp(*first, pivot) )
	      ++first;
	   --last;
	   while ( comp(pivot, *last) )
	      --last;
	   if (first < last)
	      std::iter_swap(first, last);
	   else
	      return first;
	   ++first;
	 }
  }

template<typename Bi, typename Cmp>
  void insertion_sort_(Bi first, Bi last, Cmp comp)
  {
	if (first == last)
	   return;

	for ( Bi i = first; ++i != last; )
	 {
	   typename std::iterator_traits<Bi>::value_type temp = std::move(*i);
	   if (comp(temp, *first))
	    {
	      Bi next = i;
	      std::move_backward(first, i, ++next);
	      *first = std::move(temp);
	    }
	   else
	    {
	      Bi j = i;
	      for ( Bi next = i; comp(temp, *--next); j = next )
	         *j = std::move(*next);
	      *j = std::move(temp);
	    }
	 }
  }

template<typename Ran, typename Cmp>
  void introspective_sort_(Ran first, Ran last, long depth, Cmp comp)
  {
	typename std::iterator_traits<Ran>::difference_type threshold = 16;

	while (last - first > threshold)
	   if (depth == 0)
	    {
	      std::partial_sort(first, last, last, comp);
	      return;
	    }
	   else
	    {
	      Ran split = first + (last - first) / 2;
	      std::iter_swap(split,
	              algo::iter_median_(first, split, last, comp));
	      split = algo::partition_unguarded_(first, last, *split, comp);

	      algo::introspective_sort_(first, split, --depth, comp);
	      first = split;
	    }
	algo::insertion_sort_(first, last, comp);
  }

/**
 * @brief ascending order elements of sequence.
 *
 * @param  first  iterator to start of sequence.
 * @param  last   iterator to end of sequence.
 * @param  comp   comparator object.
 *
 * @note usage is identical to std::sort.
 */
template<typename Ran, typename Cmp>
  void qsort(Ran first, Ran last, Cmp comp)
  {
	if (first != last)
	 {
	   long depth = static_cast<long>(2.0*std::log2(last - first));
	   algo::introspective_sort_(first, last, depth, comp);
	 }
  }

/**
 * @brief ascending order elements of sequence (less).
 *
 * @param  first  iterator to start of sequence.
 * @param  last   iterator to end of sequence.
 *
 * @note usage is identical to std::sort.
 */
template<typename Ran>
  void qsort(Ran first, Ran last)
  {
	typedef typename std::iterator_traits<Ran>::value_type value_type;
	algo::qsort(first, last, std::less<value_type>());
  }
} // namespace algo.

#endif // _qsort_mm_hpp_

