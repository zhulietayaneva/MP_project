#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <vector>
#include <future>
#include <algorithm>
#include "QuickSort.h"


int partition(int start, int end, std::vector<int>& arr) {
    int i = start;
    int j = end;

    // Decide random pivot
    int pivoted = rand() % (j - i + 1) + i;

    // Swap the pivot with the end element of the array
    int t = arr[j];
    arr[j] = arr[pivoted];
    arr[pivoted] = t;
    j--;

    // Partitioning
    while (i <= j) {
        if (arr[i] <= arr[end]) {
            i++;
            continue;
        }
        if (arr[j] >= arr[end]) {
            j--;
            continue;
        }
        t = arr[j];
        arr[j] = arr[i];
        arr[i] = t;
        j--;
        i++;
    }

    // Swap pivot to correct position
    t = arr[j + 1];
    arr[j + 1] = arr[end];
    arr[end] = t;
    return j + 1;
}

// Function to implement parallel quicksort using OpenMP

void quicksort_parallel(int start, int end, std::vector<int>& arr, int max_depth) {
    if (start >= end) {
        return;
    }

    // Find partition index
    int p = partition(start, end, arr);

    // If max_depth <= 0, use single-threaded sort
    if (max_depth <= 0) {
        std::sort(arr.begin() + start, arr.begin() + end + 1);
        return;
    }

    // Parallel section for left and right partition
    #pragma omp parallel sections
     {
            #pragma omp section
            {
                quicksort_parallel(start, p - 1, arr, max_depth - 1);
            }
             #pragma omp section
            {
                quicksort_parallel(p + 1, end, arr, max_depth - 1);
            }
     }
}

#endif // QUICKSORT_H