/*
  Nathan Bush
  CSE 4600 Spring 2022
  Assignment 1, Part 1

  part1_2.cpp
  Outputs specified strings based on whether numbers between 1-50 are divisible by 2, 5, both, or not.
  Same requirements as part 1, except each output string is handled by its own thread.
*/

#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

// global variables
#define N 50
int thread[4] = {1, 2, 3, 4};
int DISPLAYED_COUNT = 0;
int array[N];
pthread_mutex_t mutex;
pthread_cond_t checked1, checked2, checked3, start, displayed;
pthread_t id1, id2, id3, id4;

// function prototypes
void* csusb(void*);
void* csu(void*);
void* sb(void*);
void* number(void*);

int main() {
  // build array
  for (int i = 0; i < N; i++) {
    array[i] = i + 1;
  }

  // create threads and give them their appropriate functions
  pthread_create(&id1, NULL, csusb, &thread[0]);
  pthread_create(&id2, NULL, csu, &thread[1]);
  pthread_create(&id3, NULL, sb, &thread[2]);
  pthread_create(&id4, NULL, number, &thread[3]);

  // give threads time to initialize and enter the appropriate wait conditions
  sleep(1);
  pthread_cond_signal(&start);

  // join threads when task complete
  pthread_join(id1, NULL);
  pthread_join(id2, NULL);
  pthread_join(id3, NULL);
  pthread_join(id4, NULL);
  
  cout << endl;
  return 0;
}

void* csusb(void* thread_number) {
  // wait to acquire lock and start execution
  pthread_cond_wait(&start, &mutex);

  // iterate through array
  while (DISPLAYED_COUNT < N) {
    // display CSUSB if conditions are met
    if(array[DISPLAYED_COUNT] % 2 == 0 && array[DISPLAYED_COUNT] % 5 == 0) {
      cout << "CSUSB ";
      DISPLAYED_COUNT++;
    }
    // signal that array has been checked, wait for next thread to print to console
    else {
      pthread_cond_signal(&checked1);
      pthread_cond_wait(&displayed, &mutex);
    }
  }

  // finished iterating, signal and release lock before returning
  pthread_cond_signal(&checked1);
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void* csu(void* thread_number) {
  while(1) {
    // wait for signal to acquire lock and execute
    pthread_cond_wait(&checked1, &mutex);
    // continue to iterate while there are still values to check
    if (DISPLAYED_COUNT < N) {
      // display CSU if conditions are met, signal that a value was displayed
      if (array[DISPLAYED_COUNT] % 2 == 0){
        cout << "CSU ";
        DISPLAYED_COUNT++;
        pthread_cond_signal(&displayed);
      }
      // signal that array has been checked
      else {
        pthread_cond_signal(&checked2);     
      }
    }
    else {
      break;
    }
  }
  // finished iterating, signal and release lock before returning
  pthread_cond_signal(&checked2);
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void* sb(void* thread_number) {
  while(1){
    // wait for signal to acquire lock and execute
    pthread_cond_wait(&checked2, &mutex);
    // continue to iterate while there are still values to check
    if (DISPLAYED_COUNT < N - 1) {
      // display CSU if conditions are met, signal that a value was displayed
      if (array[DISPLAYED_COUNT] % 5 == 0) {
        cout << "SB ";
        DISPLAYED_COUNT++;
        pthread_cond_signal(&displayed);
      }
      else {
        pthread_cond_signal(&checked3);
      }
    }
    else{
      break;
    }
  }
  // finished iterating, signal and release lock before returning
  pthread_cond_signal(&checked3);
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void* number(void* thread_number) {
  while (1) {
    // wait for signal to acquire lock and execute, proceed to display
    if (DISPLAYED_COUNT < N - 1) {
      pthread_cond_wait(&checked3, &mutex);
      cout << array[DISPLAYED_COUNT] << " ";
      DISPLAYED_COUNT++;
      pthread_cond_signal(&displayed);
    }
    else {
      break;
    }
  }
  // finished iterating, release lock before returning
  pthread_mutex_unlock(&mutex);
  return NULL;
}