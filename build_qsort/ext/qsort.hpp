/**
 * @file qsort.hpp
 * alternate quick sort implementation.
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

template <typename Bi1, typename Bi2>
  Bi2 swap_ranges_backward_(Bi1 first1, Bi1 last1, Bi2 last2)
  {
	typedef typename std::reverse_iterator<Bi1> ri1;
	typedef typename std::reverse_iterator<Bi2> ri2;
	return std::swap_ranges(ri1(last1), ri1(first1), ri2(last2)).base();
  }

template <typename Ran, typename Cmp>
  std::pair<Ran, Ran>
  partition_unguarded_(Ran first, Ran last,
	typename std::iterator_traits<Ran>::value_type pivot, Cmp comp)
  {
	Ran l_head = first;
	Ran l_tail = first;

	Ran r_head = last;
	Ran r_tail = last;

	while ( true )
	 {
	   while (comp(*l_tail, pivot))
	      ++l_tail;
	   --r_head;
	   while (comp(pivot, *r_head))
	      --r_head;

	   if (l_tail < r_head)
	      std::iter_swap(l_tail, r_head);
	   else
	      break;

	   if (!comp(*l_tail, pivot))
	      std::iter_swap(l_tail, l_head++);
	   if (!comp(pivot, *r_head))
	      std::iter_swap(r_head, --r_tail);
	   ++l_tail;
	 }

	if ((l_tail - l_head) <= (l_head - first))
	   l_tail = std::swap_ranges(l_head, l_tail, first);
	else
	   l_tail = algo::swap_ranges_backward_(first, l_head, l_tail);

	++r_head;
	if ((r_tail - r_head) <= (last - r_tail))
	   r_head = algo::swap_ranges_backward_(r_head, r_tail, last);
	else
	   r_head = std::swap_ranges(r_tail, last, r_head);
	return std::pair<Ran, Ran>(l_tail, r_head);
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
	      split = algo::iter_median_(first, split, last, comp);
	      std::pair<Ran, Ran> eq = algo::partition_unguarded_(first, last,
	              *split, comp);
	      algo::introspective_sort_(first, eq.first, --depth, comp);
	      first = eq.second;
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

