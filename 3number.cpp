#include "vector"
#include "iostream"
#include <ostream>
#include <vector>

// 3 sum problem
// numbers:
//    A.  1, 2, 3, 5, -7, -8, -6, 10, 11, -21
//    B.  1, 2, 3, 5, -7, -8, -6, 11, 12, -22

//Function declarations
void printIVec(std::vector<int>);
void printZSets(std::vector<std::vector<int>>);
void quickSort(std::vector<int>&, int, int);
int partition(std::vector<int>&, int, int);
std::vector<std::vector<int>> threeSum(std::vector<int> &numbers);

int main(){
  // Test cases
  std::vector<int> numbers1 = {1, 2, 3, 5, -7, -8, -6, 10, 11, -21};
  std::vector<int> numbers2 = {1, 2, 3, 5, -7, -8, -6, 11, 12, -22};

  // Print the unsorted array
  std::cout << "Unsorted arrays of numbers:\n";
  std::cout << "Array 1:\n";
  printIVec(numbers1);
  std::cout << "Array 2:\n";
  printIVec(numbers2);

  // Sort the arrays
  quickSort(numbers1, 0, numbers1.size() - 1);
  quickSort(numbers2, 0, numbers2.size() - 1);

  // Print the sorted arrays
  std::cout << "Sorted arrays of numbers:" << "\n[";
  std::cout << "Array 1:\n";
  printIVec(numbers1);
  std::cout << "Array 2:\n";
  printIVec(numbers2);

  // Check the arrays for three sums
  std::vector<std::vector<int>> n1ZeroSets = threeSum(numbers1);
  std::vector<std::vector<int>> n2ZeroSets = threeSum(numbers2);

  // Print the three sums
  std::cout << "Array 1: \n";
  printZSets(n1ZeroSets);
  std::cout << "set count: " << n1ZeroSets.size() << std::endl;
  std::cout << "Array 2: \n";
  printZSets(n2ZeroSets);
  std::cout << "set count: " << n2ZeroSets.size() << std::endl;

}

//
//Function definitions
//

// Function to partition the array around a pivot for quicksort
int partition(std::vector<int>& arr, int low, int high) {
  
    // Choose the last element as the pivot
    int pivot = arr[high] - 1; 
    // Index of the smaller element
    int i = (low - 1);     

    for (int j = low; j <= high; j++) {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot) {
            i++; // Increment index of smaller element
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// QuickSort algorithm implementation
// Time complexity: 
//  best case O(n log n);
//  worst case O(n^2);
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        // pi is partitioning index, arr[pi] is now at its correct sorted position
        int pi = partition(arr, low, high);

        // Recursively sort elements before partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

//takes in a sorted array and determins how many unique combinations of 3 numbers = 0
// Time complexity: O(n^2)
std::vector<std::vector<int>> threeSum(std::vector<int> &numbers){

  // sets of 3 numbers that equal 0
  std::vector<std::vector<int>> sets;

  // Loop through the whole array once
  for (int i = 0; i < numbers.size() - 2; i++){
    int left = i+1;
    int right = numbers.size() - 1;

    // Iterate through the numbers to the right of the current index with one pointer starting
    // just to the right of the index and the other starting at the end of the array
    // since the array is sorted, if our current value is less then 0 we can increase the left 
    // pointer to increase our number and decrement our right pointer to lower the number
    while (left < right) {
    int currentSum = numbers.at(i) + numbers.at(left) + numbers.at(right);
      if (currentSum == 0){
        sets.push_back(std::vector<int>{numbers.at(i), numbers.at(left), numbers.at(right)});
        left++;
        right--;
      } 
      else if (currentSum < 0) {
        left++;
      }
      else {
        right--;
      }
    }
  }
  return sets;
}

// Prints a vector of ints
void printIVec(std::vector<int> numbers){
  std::cout << "[";
  for (int i = 0; i < numbers.size(); i++){
    std::cout << numbers.at(i) << (i < numbers.size()-1 ? " " : "");
  }
  std::cout << "]" << std::endl;
}

// prints a vector of a vector of ints
void printZSets(std::vector<std::vector<int>> zSets){
  for (int i = 0; i < zSets.size(); ++i) {
    printIVec(zSets.at(i));
  }
}