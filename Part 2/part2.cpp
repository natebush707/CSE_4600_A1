/*
  Nathan Bush
  CSE 4600 Spring 2022
  Assignment 1, Part 2

  Adapted some logic as seen in provided video: https://www.youtube.com/watch?v=zo-tA6HK5Rs&ab_channel=FooSo
    - specifically:
      - calculating number of iterations in thread logic
      - using a buffer to pass variables to the thread

  part2.cpp
  Performs matrix multiplication using multithreading. Each dot product is calculated by a different thread.
*/

#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

// hardcoding matrices to match example
int matrixA[4][3] = {{5, 2, 3}, {4, 5, 7}, {6, 3, 7}, {1, 3, 4}};
int matrixB[3][4] = {{4, 5, 6, 1}, {3, 2, 3, 5}, {2, 8, 7, 7}};

// dynamically construct matrix C to have the appropriate dimensions
const int ROWS_A = sizeof(matrixA)/sizeof(matrixA[0]);
const int COLS_B = sizeof(matrixB[0])/sizeof(int);
int matrixC[ROWS_A][COLS_B];
const int ITERS = min(ROWS_A, COLS_B) - 1;

// helper function prototypes
void* dot_product(void*);

template <size_t rows, size_t cols>
void display_matrix(int(&matrix)[rows][cols], char);

int main() {
  // initialize required number of threads (one for each dot product)
  pthread_t threads[ROWS_A*COLS_B];

  char buffer[100];
  int thread_count = 0;

  // run threads
  for(int i = 0; i < ROWS_A; i++){
    for(int j = 0; j < COLS_B; j++){
      sprintf( buffer, "%d %d", i, j);
      pthread_create(&threads[thread_count], NULL, dot_product, (void *)buffer);
      pthread_join(threads[thread_count], NULL);
      thread_count++;
    }
  }

  cout << "Matrix multiplication results: " << endl << endl;
  // display matrices
  display_matrix(matrixA, 'A');
  display_matrix(matrixB, 'B');
  display_matrix(matrixC, 'C');
  
  return 0;
}


void* dot_product(void *position){
  // pull in row and column from buffer
  int row, col;
  sscanf((char*) position, "%d %d", &row, &col);

  // calculate dot product
  int result = 0;
  for (int i = 0; i < ITERS; i++)
    result += matrixA[row][i] * matrixB[i][col];

  // update matrixC cell with result
  matrixC[row][col] = result;
  return NULL;
}

// helper function to display matrices
template <size_t rows, size_t cols>
void display_matrix(int(&matrix)[rows][cols], char name) {
  cout << "Matrix " << name << ":" << endl;
  for (int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      cout << matrix[i][j] << " ";
    }
    cout << endl << endl;
  }
  cout << endl;
}