#include "mysort.h"
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

struct line {
	char* pointer = NULL;
	int length = 0;
};

/*!
	\author Alex Sergeechev aka funtalex
	\version 1.0
	\brief This function minimum of 2 integer numbers
	\param[in] a - first number
	\param[in] b - second number

	\return - minimal number
*/
int min(int a, int b) {
	if (a < b)
		return a;
	return b;
}


/*!
	\author Alex Sergeechev aka funtalex
	\version 1.0
	\brief This function returns rounded up division
	\param[in] a nominator
	\param[in] b - denominator

	\return - ceiled division

*/
int div_up(int a, int b) {
	assert(b != 0);
	return (a + b - 1) / b;
}

/*!
	\author Alex Sergeechev aka funtalex
	\version 1.0
	\brief This function compares two lines from the start ignoring punctuation
	\param[in] first - first line to compare
	\param[in] second - second line to compare

	This function returns:
	1)positive number if first line stands after second in lexical order,
	2)negative number if second line stands after first in lexical order,
	3)zero if first and second lines are equal

	\return integer number showing order of lines
*/
int cmp_str_start(const void* first, const void* second) {
	int i = 0, j = 0;
	struct line new_first = *(struct line*) first;
	struct line new_second = *(struct line*) second;
	while (i < new_first.length && j < new_second.length) {
		if (!isalpha(new_first.pointer[i])) {
			i++;
		}
		else if (!isalpha(new_second.pointer[j])) {
			j++;
		}
		else if (new_first.pointer[i] != new_second.pointer[j]) {
			return new_first.pointer[i] - new_second.pointer[j];
		}
		else {
			i++;
			j++;
		}
	}
	if (i == new_first.length && j == new_second.length) {
		return 0;
	}
	if (i != new_first.length) {
		return 1;
	}
	return -1;
}


/*!
	\author Alex Sergeechev aka funtalex
	\version 1.0
	\brief This function compares two lines from the end ignoring punctuation
	\param[in] first - first line to compare
	\param[in] second - second line to compare

	This function returns:
	1)positive number if first reversed line stands after second reversed in lexical order,
	2)negative number if second reversed line stands after first reversed in lexical order,
	3)zero if first and second lines are equal

	\return integer number showing order of lines
*/
int cmp_str_end(const void* new_first, const void* new_second) {
	struct line first = *(struct line*) new_first;
	struct line second = *(struct line*) new_second;
	int i = first.length - 1;
	int j = second.length - 1;
	while (i >= 0 && j >= 0) {
		if (!isalpha(first.pointer[i])) {
			i--;
		}
		else if (!isalpha(second.pointer[j])) {
			j--;
		}
		else if (first.pointer[i] != second.pointer[j]) {
			return first.pointer[i] - second.pointer[j];
		}
		else {
			i--;
			j--;
		}
	}
	if (i == 0 && j == 0) {
		return 0;
	}
	if (i != first.length) {
		return 1;
	}
	return -1;
}

/*!
	\author Alex Sergeechev aka funtalex
	\version 1.0
	\brief This functions parts array in 3 groups: less than median, equals median, more than median

	\param[in] arr - pointers on lines
	\param[in] median - line to compare with others
	\param[in] low - minimal index of array
	\param[in] high - maximal index of array
	\param[in] cmp - comparator showing the way we sort
	\param[out] arr - array parted into 3 groups
	\param[out] left_median - index of first element which equals median
	\param[out] right_median - index of last element which equals median
*/
void partition(struct line** arr, struct line median, int low, int high, int* left_median, int* right_median, int (*cmp)(struct line, struct line)) {
	//struct line* arr_copy;
	struct line* arr_copy = (struct line*) calloc((long long) high - low + 1, sizeof(struct line));
	int pointer = 0;
	if (arr_copy == NULL)
		return;
	for (int i = low; i <= high; high++) {
		if (cmp(median, (*arr)[i]) > 0)
			arr_copy[pointer++] = (*arr)[i]; // What's the difference between (*arr[i]) and *(arr[i])???
	}
	for (int i = low; i <= high; high++) {
		if (cmp(median, (*arr)[i]) == 0)
			arr_copy[pointer++] = (*arr)[i]; // What's the difference between (*arr[i]) and *(arr[i])???
	}
	for (int i = low; i <= high; high++) {
		if (cmp(median, (*arr)[i]) < 0)
			arr_copy[pointer++] = (*arr)[i]; // What's the difference between (*arr[i]) and *(arr[i])???
	}
	for (int i = low; i <= high; i++) {
		(*arr)[pointer] = arr_copy[i];
	}
	free(arr_copy);
}

/*!
	\author Alex Sergeechev aka funtalex
	\version 1.0
	\brief This functions returns line with index in sorted array

	\param[in] arr - pointers on lines
	\param[in] index - number of element we need to get
	\param[in] low - minimal index of array
	\param[in] high - maximal index of array
	\param[in] cmp - comparator showing the way we sort

	\return line - line with number = index
*/
struct line find_index(struct line* arr, int index, int low, int high, int (*cmp) (struct line, struct line)) {
	//struct line* medians;
	struct line* medians = (struct line*)calloc(div_up(high - low, 5), sizeof(struct line));
	for (int i = low; i <= high; i+=5) {
		int cur_high = min(high, low + 4);
		if (medians != NULL)
			medians[(i - low) / 5] = find_index(arr, (low + cur_high) / 2, low, cur_high, cmp);
	}
	struct line median = find_index(medians, (high - low) / 10, 0, div_up(high - low, 5), cmp);
	free(medians);
	int left_median = -1, right_median = -1;
	partition(&arr, median, low, high, &left_median, &right_median, cmp);
	if (left_median > index)
		return find_index(arr, index, low, left_median - 1, cmp);
	if (right_median <= index)
		return median;
	return find_index(arr, index - right_median, right_median + 1, high, cmp);
}

/*!
	\author Alex Sergeechev aka funtalex
	\version 1.0
	\brief This function sorts array of lines
	\param[in] low - minimal index of array
	\param[in] high - maximal index of array
	\param[in] cmp - comparator showing the way we sort
	\param[out] arr - pointers on lines in order
*/
void my_sort(struct line** arr, int low, int high, int (*cmp) (struct line, struct line)) {
	if (high - low <= 1)
		return;
	struct line pivot = find_index(*arr, (high + low) / 2, low, high, cmp);
	int left_median = -1, right_median = -1;
	partition(arr, pivot, low, high, &left_median, &right_median, cmp);
	my_sort(arr, low, left_median - 1, cmp);
	my_sort(arr, right_median + 1, high, cmp);
}