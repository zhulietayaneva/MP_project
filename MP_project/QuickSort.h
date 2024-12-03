#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <vector>
#include <future>
#include <algorithm>


void quicksort_parallel(int start, int end, std::vector<int>& arr, int max_depth);

// Declaration of partition function
int partition(int start, int end, std::vector<int>& arr);

#endif // QUICKSORT_H
