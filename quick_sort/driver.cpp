#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include "quickSort.cpp"

// Driver file created with the help of Claude Opus 4.6 to create test cases. All other code is my own.  

// ─── Helper ─────────────────────────────────────────────────────────
void print(const std::string& label, const std::vector<int>& a) {
    std::cout << label;
    for (int x : a) std::cout << x << " ";
    std::cout << "\n";
}

bool isSorted(const std::vector<int>& a) {
    for (int i = 1; i < a.size(); i++)
        if (a[i] < a[i - 1]) return false;
    return true;
}

// ─── Driver ─────────────────────────────────────────────────────────

int main() {
    // Test 1: hand-picked array
    std::vector<int> v1 = {38, 27, 43, 3, 9, 82, 10};
    print("Before: ", v1);
    quicksort(v1);
    print("After:  ", v1);
    std::cout << "Sorted? " << (isSorted(v1) ? "YES" : "NO") << "\n\n";

    // Test 2: already sorted
    std::vector<int> v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    print("Before: ", v2);
    quicksort(v2);
    print("After:  ", v2);
    std::cout << "Sorted? " << (isSorted(v2) ? "YES" : "NO") << "\n\n";

    // Test 3: reverse sorted
    std::vector<int> v3 = {8, 7, 6, 5, 4, 3, 2, 1};
    print("Before: ", v3);
    quicksort(v3);
    print("After:  ", v3);
    std::cout << "Sorted? " << (isSorted(v3) ? "YES" : "NO") << "\n\n";

    // Test 4: duplicates
    std::vector<int> v4 = {5, 3, 5, 1, 3, 5, 1};
    print("Before: ", v4);
    quicksort(v4);
    print("After:  ", v4);
    std::cout << "Sorted? " << (isSorted(v4) ? "YES" : "NO") << "\n\n";

    // Test 5: single element
    std::vector<int> v5 = {42};
    quicksort(v5);
    std::cout << "Single element: " << (isSorted(v5) ? "YES" : "NO") << "\n\n";

    // Test 6: empty
    std::vector<int> v6 = {};
    quicksort(v6);
    std::cout << "Empty array:    " << (isSorted(v6) ? "YES" : "NO") << "\n\n";

    // Test 7: large random array
    std::vector<int> v7(10000);
    std::uniform_int_distribution<int> dist(0, 99999);
    for (int& x : v7) x = dist(rng);
    quicksort(v7);
    std::cout << "10000 random:   " << (isSorted(v7) ? "YES" : "NO") << "\n";

    return 0;
}