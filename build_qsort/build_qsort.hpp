/**
 * @file build_qsort.hpp
 * trying to build a better quick sort.
 */

#ifndef _build_qsort_mm_h_
#define _build_qsort_mm_h_ 1

#include <functional>
#include <algorithm>
#include <iterator>

/****************************************************************************
 * the quick sort algorithm is conceptually very simple, yet at the same time
 * can be tricky to implement correctly and efficiently.
 *
 * the main problem is that the running time of quick sort in the worst case
 * is quadratic. a secondary problem is that some authors of quick sort test
 * their creation using only uniformly distributed randomized sequences,
 * accidentally avoiding worst case behavior.
 *
 * the algorithm is: select a value (the pivot) from the sequence, divide the
 * sequence into to subsequences predicated on each element's relation to the
 * pivot value. repeat for subsequences.
 ****************************************************************************/

/****************************************************************************
 * partitioning.
 *
 * partitioning is the core of the quicksort algorithm. there are numerous
 * partitioning functions that behave in their own unique ways.
 *
 * the functions here take a comparison functor to make them more general,
 * but for discussion we will assume the comparison is always less<int>().
 *
 * quicksort is very particular about the behavior of the partition function.
 * the partition it expects is [ less or equal | greater or equal ].
 *
 * this does not mean we cannot make it work with other types of partitions,
 * but it will usually require special handling of edge cases.
 *
 * @note as long as pivot is within the minimum and maximum values of the
 * sequence being partitioned, these functions will never return last, since
 * some value will always be not-less than the pivot.
 ****************************************************************************/

template<typename For, typename Cmp>
  For partition_standard(For first, For last,
	typename std::iterator_traits<For>::value_type pivot, Cmp comp)
  /* partition.
   *
   * @note this is what std::partition does as far as behavior. the signature
   * has been changed to take an argument and comparison object instead of a
   * predicate, but is equivalent to,
   *
   * std::partition(first, last, std::bind2nd(comp, pivot));
   *
   * the partiton generated is [ less | not-less ], unfortunately this is not
   * what the quicksort algorithm specifies, but we can still make it work.
   *
   * @note this function is not used, instead we will use std::partition.
   */
  {
	first = std::find_if_not(first, last, std::bind2nd(comp, pivot));

	for ( For next = first; next != last; ++next )
	   if (comp(*next, pivot))
	    {
	      std::iter_swap(first, next);
	      ++first;
	    }
	return first;
  }

template<typename Ran, typename Cmp>
  Ran partition_squeeze(Ran first, Ran last,
	typename std::iterator_traits<Ran>::value_type pivot, Cmp comp)
  /* bi-directional partition.
   *
   * @note this is an unguarded partition (i.e. pivot must be within minimum
   * and maximum sequence values; sequence length is not zero).
   *
   * for this to converge as used in quicksort, this must never return first.
   * the easiest way to enforce this, is to not use *first as the pivot.
   *
   * however, *first may be used if it is not the lowest value or not unique.
   * median of three enforces this automatically.
   *
   * @note this function behaves as specified by the quicksort algorithm,
   * (i.e. [ less or equal | greater or equal ]).
   */
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

template <typename For, typename Cmp>
  std::pair<For, For>
  partition3_forward(For first, For last,
	typename std::iterator_traits<For>::value_type pivot, Cmp comp)
  /* forward 3-way partitioning.
   *
   * @note 3-way partitioning is a technique used to improve performance when
   * a sequence contains many identical keys. in the case of all equal keys,
   * quicksort using 3-way partition will complete in linear time.
   *
   * normally the subsequences share a boundary, here values equal to pivot
   * are already in final sorted order, therefore, they may be omitted from
   * further evaluation (i.e. the sequence [lower,upper)).
   *
   * @return iterator pair specifying equal range for values equal to pivot.
   */
  {
	For lower = std::partition(first, last, std::bind2nd(comp, pivot));
	For upper = std::partition(lower, last,
		std::not1(std::bind1st(comp, pivot)));
	return std::pair<For, For>(lower, upper);
  }

template <typename Bi, typename Cmp>
  std::pair<Bi, Bi>
  partition3_dijkstra(Bi first, Bi last,
	typename std::iterator_traits<Bi>::value_type pivot, Cmp comp)
  /* Dijkstra 3-way partitioning.
   *
   * @note all of the optimizations i've tried to apply actually make this
   * run slower (e.g. find_first_not).
   *
   * @return iterator pair specifying equal range for values equal to pivot.
   */
  {
	for ( Bi next = first; next != last; )
	   if (comp(pivot, *next))
	    {
	      std::iter_swap(next, --last);
	    }
	   else
	    {
	      if (comp(*next, pivot))
	       {
	         std::iter_swap(next, first);
	         ++first;
	       }
	      ++next;
	    }
	return std::pair<Bi, Bi>(first, last);
  }

template <typename Ran, typename Cmp>
  std::pair<Ran, Ran>
  partition3_bentley_mcilroy(Ran first, Ran last,
	typename std::iterator_traits<Ran>::value_type pivot, Cmp comp)
  /* Bentley-McIlroy 3-way partitioning.
   *
   * @note this is an unguarded partition (i.e. pivot must be within minimum
   * and maximum sequence values; sequence length is not zero).
   *
   * this generates the partition in two parts. first values less and greater
   * are partitioned, any equal values encountered are moved to the edges of
   * the sequence. the second stage rotates the equal values into position.
   *
   * 1st: [ equal | less | greater | equal ]
   * 2nd: [ less | equal | equal | greater ]
   *
   * @return iterator pair specifying equal range for values equal to pivot.
   */
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

	   // compact equal to sequence front.
	   if (!comp(*l_tail, pivot))
	    {
	      std::iter_swap(l_tail, l_head);
	      ++l_head;
	    }

	   // compact equal to sequence back.
	   if (!comp(pivot, *r_head))
	    {
	      --r_tail;
	      std::iter_swap(r_head, r_tail);
	    }
	   ++l_tail;
	 }

	// adjust right head.
	//
	// @note loop exited before swap. r_head references a value less than
	// or equal to pivot; increment to correct.
	++r_head;

	// rotate equal to partition points.
	//
	// @note if head and tail are equal, then all elements are equal to
	// pivot; omit swaps, set range to sequence limits.
	if (l_head == l_tail)
	   l_tail = first;
	else
	 {
	   while ( first != l_head )
	      std::iter_swap(--l_head, --l_tail);
	 }
	if (r_head == r_tail)
	   r_head = last;
	else
	 {
	   while ( r_tail != last )
	      std::iter_swap(r_tail++, r_head++);
	 }
	return std::pair<Ran, Ran>(l_tail, r_head);
  }

/****************************************************************************
 * pivot selection.
 *
 * ideally the pivot would be the actual median value. unfortunately the best
 * way to find it, is to perform a sort on the sequence and select the middle
 * value. since sorting is the problem we're trying to solve, this is not an
 * option.
 *
 * there are selection methods that are able to locate the median in linear
 * time, however, they are complex enough that in practice they usually give
 * negative gains.
 *
 * about the best we can do efficiently, is to try and avoid selecting the
 * worst pivot at each level. there are two common solutions.
 *
 * the first is stochastic sampling (selecting an element at random with all
 * elements having an equal probablility of being chosen), which has been
 * shown to avoid worst case with high probablility.
 *
 * the second is median selection (selecting a number of samples and using
 * some heuristic determine the best choice), the most common being median of
 * three. this works well with sorted and reverse sorted inputs.
 *
 * implementations may use combinations of these. there is a lot of room for
 * experimentation in pivot selection. try new things.
 ****************************************************************************/

template<typename Ran>
  inline Ran iter_random(Ran first, Ran last)
  /* stochastic sampling.
   *
   * @note while this is easy to implement and does a good job at avoiding
   * worst case partitions, it also does a good job of avoiding best case.
   * because pivot selection is completely random, no consideration is given
   * to ordering.
   *
   * by using std::rand() this function has side effects. this should usually
   * be avoided in library code.
   */
  {
	return first + std::rand() % (last - first);
  }

template<typename In, typename Cmp>
  inline In iter_median_3(In a, In b, In c, Cmp comp)
  /* median of three selection.
   *
   * @note this is less resistant to some patterns, but in the average case
   * performs very well. unlike randomized selection, this tries to estimate
   * the actual median value.
   *
   * @note this function expects iterators to reference valid objects.
   */
  {
	auto it_comp = [comp] (In a, In b) { return comp(*a, *b); };
	if (comp(*b, *c))
	   return std::min(std::max(a, b, it_comp), c, it_comp);
	else
	   return std::max(std::min(a, b, it_comp), c, it_comp);
  }

template<typename Ran, typename Cmp>
  inline Ran iter_median_9(Ran first, Ran split, Ran last, Cmp comp)
  /* median of nine selection.
   *
   * @note takes more samples in an attempt to estimate the median value.
   * this improves the estimate and further insulates from problematic
   * patterns, and may be stacked even more (i.e. median of twelve,
   * twenty-seven, etc.).
   *
   * thresholds are those specified by Bentley & McIlroy.
   *
   * @note this function accepts a range. split is expected to be the
   * sequence midpoint (i.e. (last - first) / 2).
   */
  {
	if (last - first <= 7)
	   return split;
	if (last - first <= 40)
	   return iter_median_3(first, split, last-1, comp);

	auto d = (last - first) / 8;
	Ran a = iter_median_3(first, first+d, first+d*2, comp);
	Ran b = iter_median_3(split-d, split, split+d, comp);
	Ran c = iter_median_3(last-d*2, last-d, last-1, comp);
	return  iter_median_3(a, b, c, comp);
  }

/****************************************************************************
 * quicksort implementation.
 *
 * the following functions are used as test cases. the signatures have been
 * changed to accept an indent parameter to allow for 'pretty printing' when
 * viewing partitions as binary trees.
 ****************************************************************************/

#include <iostream>
#include <iomanip>

template<typename For>
  void print_partition(For first, For last, int indent)
  /* output partition data. */
  {
#ifdef BUILD_QSORT_PRINT_PARTITION
	typedef typename std::iterator_traits<For>::value_type value_type;
	if (indent)
	   std::cout << std::setw(indent) << ' ';
	std::copy(first, last, std::ostream_iterator<value_type>(std::cout, " "));
	std::cout << std::endl;
#endif
  }

template<typename Ran>
  void qsort_v1(Ran first, Ran last, int indent = 0)
  /* quicksort (base implementation).
   *
   * @note this uses naive pivot selection and std::partition, which doesn't
   * have the specified behavior and must be corrected for.
   *
   * unfortunately this is the version i see most often. unless a variety of
   * test cases are used, one might be fooled into thinking this version is
   * good enough. it isn't.
   *
   * if all keys are equal, this function's running time is quadratic.
   */
  {
	typedef typename std::iterator_traits<Ran>::value_type value_type;
	// sequence of size <= 1 is already sorted (base case).
	if (last - first <= 1)
	   return;

	// select pivot.
	//
	// @note we select the middle element here. this will work better
	// when dealing with sorted sequences, and although problem cases are
	// less common than when using the first element, it's still a poor
	// choice.
	Ran pivot = first + (last - first)/2;

	// save pivot.
	//
	// @note we can safely swap the pivot to the rear of the sequence
	// and expect it to remain in place (element is already in sorted
	// position and will not be moved).
	std::iter_swap(pivot, last-1);

	// partition. lower bound with respect to pivot.
	//
	// @note we skip evaluating the last element, but it's not a
	// requirement for correct operation (element is already in sorted
	// position and will not be moved).
	Ran lower = std::partition(first, last-1,
		std::bind2nd(std::less<value_type>(), *(last-1)));
	Ran upper = lower+1;

	// restore pivot.
	//
	// @note by replacing this element we're sort of doing a halfassed
	// version of 3-way partitioning, where we only worry about the
	// placement of one element.
	//
	// this allows us to skip this element in future iterations, and
	// it's what guarantees this version of quicksort will converge.
	std::iter_swap(lower, last-1);

	// view partition (preorder).
	//
	// @note allows us to view partitions in tree structure.
	print_partition(first, last, indent);

	// repeat for subsequences.
	qsort_v1(first, lower, indent);
	qsort_v1(upper, last, indent + (upper-first)*2);
  }

template<typename Ran>
  void qsort_v2(Ran first, Ran last, int indent = 0)
  /* quicksort (specified partition).
   *
   * @note this uses the squeeze partition which has the behavior specified
   * by the algorithm.
   *
   * the squeeze also improves running time when all keys are equal. in the
   * case of identical keys this function's running time is n*log(n).
   */
  {
	typedef typename std::iterator_traits<Ran>::value_type value_type;
	if (last - first <= 1)
	   return;

	// select pivot.
	Ran pivot = first + (last - first)/2;

	// partition.
	//
	// @note notice there is no need to shuffle anything around to
	// guarantee convergance.
	Ran lower = partition_squeeze(first, last, *pivot,
		std::less<value_type>());
	Ran upper = lower;

	// view partition (preorder).
	print_partition(first, last, indent);

	// repeat for subsequences.
	qsort_v2(first, lower, indent);
	qsort_v2(upper, last, indent + (upper-first)*2);
  }

template<typename Ran>
  void qsort_v3(Ran first, Ran last, int indent = 0)
  /* quick sort (3-way partition).
   *
   * @note 3-way partitioning improves running time when all keys are equal.
   * in the case of identical keys this function's running time is linear.
   */
  {
	typedef typename std::iterator_traits<Ran>::value_type value_type;
	if (last - first <= 1)
	   return;

	// select pivot.
	Ran pivot = first + (last - first)/2;

	// partition.
	//
	// @note this returns a range specifying the equal range (upper/lower
	// bound) for elements equal to pivot.
	std::pair<Ran, Ran> eq = partition3_forward(first, last, *pivot,
		std::less<value_type>());

	// view partition (preorder).
	print_partition(first, last, indent);

	// repeat for subsequences.
	//
	// @note omitting the sequence [eq.first,eq.second).
	qsort_v3(first, eq.first, indent);
	qsort_v3(eq.second, last, indent + (eq.second-first)*2);
  }

template<typename Ran>
  void qsort_v4(Ran first, Ran last, int indent = 0)
  /* quicksort (3-way partition, median of three pivot selection).
   *
   * @note this uses the same partition as version 3, only improving pivot
   * selection to demonstrate the difference it can make.
   */
  {
	typedef typename std::iterator_traits<Ran>::value_type value_type;
	if (last - first <= 1)
	   return;

	// sequence midpoint.
	Ran split = first + (last - first)/2;

	// select pivot.
	//
	// @note using median of three selection.
	Ran pivot = iter_median_3(first, split, last-1,
		std::less<value_type>());

	// swap pivot to sequence midpoint.
	//
	// @note many versions don't do this. sometimes (not always) this can
	// make a big difference. since this is an estimate of the actual
	// median, it makes sense to place it at the midpoint.
	std::iter_swap(pivot, split);
	pivot = split;

	// partition.
	//
	// @note same partition as version 3.
	std::pair<Ran, Ran> eq = partition3_forward(first, last, *pivot,
		std::less<value_type>());

	// view partition (preorder).
	print_partition(first, last, indent);

	// repeat for subsequences.
	qsort_v4(first, eq.first, indent);
	qsort_v4(eq.second, last, indent + (eq.second-first)*2);
  }

template<typename Ran>
  void qsort_v5(Ran first, Ran last, int indent = 0)
  /* quicksort (3-way partition, randomized pivot selection).
   *
   * @note this is the one function that we will be using randomized pivot
   * selection with, only because this particular algorithm seems to do so
   * much better with it.
   */
  {
	typedef typename std::iterator_traits<Ran>::value_type value_type;
	if (last - first <= 1)
	   return;

	// select pivot.
	//
	// @note randomization gets much better results in this case over
	// median of three selection.
	//
	// we don't do any swapping like with median of three because we know
	// nothing about this value or how it relates to the sequence.
	Ran pivot = iter_random(first, last);

	// partition.
	//
	// @note using Dijkstra 3-way partition.
	std::pair<Ran, Ran> eq = partition3_dijkstra(first, last, *pivot,
		std::less<value_type>());

	// view partition (preorder).
	print_partition(first, last, indent);

	// repeat for subsequences.
	qsort_v5(first, eq.first, indent);
	qsort_v5(eq.second, last, indent + (eq.second-first)*2);
  }

template<typename Ran>
  void qsort_v6(Ran first, Ran last, int indent = 0)
  /* quicksort (3-way partition, median of nine pivot selection).
   *
   * @note this uses what is considered the fastest 3-way partition scheme,
   * along with the median selection recommend for this algorithm.
   */
  {
	typedef typename std::iterator_traits<Ran>::value_type value_type;
	if (last - first <= 1)
	   return;

	// sequence midpoint.
	Ran split = first + (last - first)/2;

	// select pivot.
	//
	// @note using median of nine selection. notice we don't swap the
	// pivot to the midpoint; does not make a difference in this case
	// (does when using median of three).
	Ran pivot = iter_median_9(first, split, last,
		std::less<value_type>());

	// partition.
	//
	// @note using Bentley-McIlroy 3-way partition.
	std::pair<Ran, Ran> eq = partition3_bentley_mcilroy(first, last,
		*pivot, std::less<value_type>());

	// view partition (preorder).
	print_partition(first, last, indent);

	// repeat for subsequences.
	qsort_v6(first, eq.first, indent);
	qsort_v6(eq.second, last, indent + (eq.second-first)*2);
  }

/****************************************************************************
 * final notes.
 *
 * you may be suprised when you see the final implementation of quicksort
 * (@see qsort.hpp) at some of the choices that have been made. the reason it
 * is not a copy of version 6 here, is that while version 6 may perform best
 * within the test suite, the suite is designed to generate sequences that
 * intentionally break quicksort - these are not the common case.
 *
 * any quicksort function should perform reasonably well in all test cases,
 * but you should not tune your code for these. tune your code on real world
 * data sets, then use the test suite to verify you haven't made any breaking
 * changes.
 *
 * the final quicksort performs better than version 6 in the average case,
 * and the code is small and simple, which is a plus IMO when it comes to
 * templated code that must appear in header files.
 *
 * the techniques that it implements that were not covered here are, switch
 * to insertion sort for small sequences and monitering depth and switching
 * to heap sort (std::partial_sort) when some threshold is exceeded. (i.e.
 * implements introspective sort). the insertion sort is the final version
 * from the build_isort project (also available on PSC).
 ****************************************************************************/

/****************************************************************************
 * implementation notes.
 *
 * some implementations prefer to swap the pivot with some sequence element,
 * use a reference to that element as the pivot during partitioning, and
 * afterwards swap the pivot element to it's final position (@see qsort_v1).
 *
 * i believe this is an outmoded trick from the 70's that saves a tiny amount
 * of memory and a single comparison, but creates aliasing issues that are of
 * more concern in modern programming, therefore, taking a local temporary is
 * prefered here.
 *
 * ...
 *
 * if you've studied quicksort, chances are, you've seen every algorithm used
 * here, only in different form, since most implementations use indexing with
 * inclusive sequences.
 *
 * here we use pointers and exclusive sequences, and while this may initially
 * seem confusing, i believe this is a superior way to implement these
 * algorithms in C and C++.
 *
 * using pointers directly, allow us to track fewer variables than indexing,
 * are easier to generalize in C and are simple to translate to iterators in
 * C++.
 *
 * sequences here are more naturally represented as exclusive. the partition
 * function returns a shared boundary(s). the inclusive model needs special
 * adjustment to correct overlapping and 'out of range' boundaries for both
 * internal and initial user calls.
 ****************************************************************************/

#endif // _build_qsort_mm_h_

