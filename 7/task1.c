#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int a = 0; // The variable a is global static for the entire program,
           // so it will be shared by both threads of execution.

/**
  Below follows the text of the function
  that will be associated with the 2nd thread.

  Parameter dummy is used only for type compatibility.
  For the same reason, the function returns void *.
**/
void *mythread(void *dummy) {
  pthread_t mythid;
  //
  // Note that mythid variable is a dynamic local variable of mythread ()
  // function, that is, it is pushed on the stack and, therefore, is not shared
  // by the threads.
  //
  mythid = pthread_self();

  a = a + 1;
  printf("Thread %u, Calculation result = %d\n", (unsigned)mythid, a);
  return NULL;
}

void *thread2(void *dummy) {
  int sum = 0;
  for (int i = 0; i < 100000; i++) {
    sum += i;
  }
  sleep(2);
  printf("th2 result = %i\n", sum);
  return NULL;
}

/**
  The main() function is also an associated function of the main thread.
**/
int main() {
  pthread_t thid, thid2, mythid;
  int result;
  //
  // Try to create a new thread of execution associated with the mythread ()
  // function. Pass it NULL as a parameter. If it succeeds, the identifier of
  // the new thread is written into the thid variable. If an error occurs, stop
  // working.
  //
  result = pthread_create(&thid, (pthread_attr_t *)NULL, mythread, NULL);

  if (result != 0) {
    printf("Error on thread create, return value = %d\n", result);
    exit(-1);
  }

  result = pthread_create(&thid2, (pthread_attr_t *)NULL, thread2, NULL);

  if (result != 0) {
    printf("Error on second thread create, return value = %d\n", result);
    exit(-1);
  }

  printf("Threads creatssed, thids = %u, %u\n", (unsigned)thid,
         (unsigned)thid2);

  mythid = pthread_self();

  a = a + 1;

  printf("Thread %u, Calculation result = %d\n", (unsigned)mythid, a);
  //
  // Wait for the spawned thread to terminate, not caring what value it returns.
  // If this function is not called, the main() function may end before
  // the spawned thread is executed, which will automatically cause it to
  // terminate, distorting the results.
  //
  pthread_join(thid, (void **)NULL);
  pthread_join(thid2, (void **)NULL);

  printf("bye bye \n");

  return 0;
}
