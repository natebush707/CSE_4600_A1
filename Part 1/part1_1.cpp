/*
  Nathan Bush
  CSE 4600 Spring 2022
  Assignment 1, Part 1

  part1_1.cpp
  Outputs specified strings based on whether numbers between 1-50 are divisible by 2, 5, both, or not.
*/

#include <iostream>
#include <string>

using namespace std;

string output(int);

int main() {
  // set array size
  int n = 50;

  // initialize and build array
  int array[n];
  for (int i = 0; i < n; i++) {
    array[i] = i + 1;
  }

  // iterate through array and output appropriate string
  for (int& i: array) {
    cout << output(i) << " ";
  }
  
  return 0;
}

// takes an integer as input and outputs the appropriate string
string output(int i){
  if (i % 2 == 0 && i % 5 == 0)
      return "CSUSB";
  else if (i % 2 == 0)
      return "CSU";
  else if (i % 5 == 0)
    return "SB";
  else
    return to_string(i);
}