#include <algorithm>
#include <random>
#include <vector>

// Create a static random number generator
static std::mt19937 rng(std::random_device{}());

// Partitioning function
int partition(std::vector<int>& a, int lo, int hi) {    
    // 1. Pick a random pivot and move it to a[lo]
    std::uniform_int_distribution<int> dist(lo, hi);
    int pivotIndex = dist(rng);
    std::swap(a[lo], a[pivotIndex]);

    // 2. original two-pointer partition logic is the same as the textbook example
    int i = lo, j = hi + 1;
    while (true) {
        while (a[++i] < a[lo])
            if (i == hi) break;
        while (a[lo] < a[--j])
            if (j == lo) break;
        if (i >= j) break;
        std::swap(a[i], a[j]);
    }
    // 3. Place pivot in its correct position
    std::swap(a[lo], a[j]); 
    return j;
}

// Child quicksort function
void quicksort(std::vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;
    int j = partition(a, lo, hi);
    quicksort(a, lo, j - 1);
    quicksort(a, j + 1, hi);
}

// Initial quicksort function
void quicksort(std::vector<int>& a) {
    if (a.empty()) return;
    quicksort(a, 0, a.size() - 1);
}