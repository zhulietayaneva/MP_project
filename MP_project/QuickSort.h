#pragma once

#include <vector>
#include <future>
#include <algorithm>


void quicksort_parallel(int start, int end, std::vector<int>& arr, int max_depth);

// Declaration of partition function
int partition(int start, int end, std::vector<int>& arr);

void generateRandomArray(std::vector<int>& arr, size_t size, int range);

void printArray(const std::vector<int>& arr);
