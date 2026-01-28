#include "vector"
#include "iostream"
#include <ostream>
#include <vector>

// 3 sum problem
// numbers: 4, -12, 5, 0, -3, 8, 1, -2, 9, -7, 2, -5, 6, -1, 3, -10, 11, -4, 7, -6

//Function declarations
void sort(std::vector<int>&);
std::vector<std::vector<int>> threeSum(std::vector<int> &numbers);

int main(){
  std::vector<int> numbers = {4, -12, 5, 0, -3, 8, 1, -2, 9, -7, 2, -5, 6, -1, 3, -10, 11, -4, 7, -6};

  // Print the unsorted array
  std::cout << "Unsorted array of numbers:\n[";
  for (int i = 0; i < numbers.size(); i++){
    std::cout << numbers.at(i) << (i < numbers.size()-1 ? " " : "");
  }
  std::cout << "]" << std::endl;

  // Sort the array
  sort(numbers);

  // Print the sorted array
  std::cout << "Sorted array of numbers:" << "\n[";
  for (int i = 0; i <  numbers.size(); i++){
    std::cout << numbers.at(i) << (i < numbers.size()-1 ? " " : "");
  }
  std::cout << "]" << std::endl;

  // Check the array for three sums
  std::vector<std::vector<int>> zeroSets = threeSum(numbers);

  // Print the three sums
  for (int i = 0; i < zeroSets.size(); ++i) {
    std::cout << "[";
    for (int j = 0; j < zeroSets.at(i).size(); ++j) {
        std::cout << zeroSets.at(i).at(j);
        if (j < zeroSets.at(i).size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << (i < zeroSets.size() - 1 ? ", " : "") << std::endl;
  }
  std::cout << "set count: " << zeroSets.size() << std::endl;

}

//
//Function definitions
//

//takes in an array of numbers and sort it numericly
void sort(std::vector<int> &numbers){
    int swap = 0;
    for (int i = 0; i < numbers.size(); i++){
        for (int j = i; j < numbers.size(); j++){
            if(numbers.at(j) < numbers.at(i)){
                swap = numbers.at(i);
                numbers.at(i) = numbers.at(j);
                numbers.at(j) = swap;
            }
        }
    }   
}

//takes in an array and determins how many unique combinations of 3 numbers = 0
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
