// Test that Thread objects are reused.
// RUN: %clangxx_hwasan -mllvm -hwasan-instrument-stack=0 %s -o %t && %env_hwasan_opts=verbose_threads=1 %run %t 2>&1 | FileCheck %s

#include <assert.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sanitizer/hwasan_interface.h>

#include "../utils.h"

pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;

void *threadfn(void *) {
  pthread_mutex_lock(UNTAG(&mu));
  pthread_mutex_unlock(UNTAG(&mu));
  return nullptr;
}

void start_stop_threads() {
  constexpr int N = 4;
  pthread_t threads[N];

  pthread_mutex_lock(UNTAG(&mu));
  for (auto &t : threads)
    pthread_create(&t, nullptr, threadfn, nullptr);
  pthread_mutex_unlock(UNTAG(&mu));

  for (auto &t : threads)
    pthread_join(t, nullptr);
}

int main() {
  // Cut off initial threads.
  // CHECK: === test start ===
  untag_fprintf(stderr, "=== test start ===\n");

  // CHECK: Creating  : T{{[0-9]+}} [[A:0x[0-9a-f]+]] stack:
  // CHECK: Creating  : T{{[0-9]+}} [[B:0x[0-9a-f]+]] stack:
  start_stop_threads();

  // CHECK-DAG: Creating  : T{{[0-9]+}} [[A]] stack:
  // CHECK-DAG: Creating  : T{{[0-9]+}} [[B]] stack:
  start_stop_threads();

  // CHECK-DAG: Creating  : T{{[0-9]+}} [[A]] stack:
  // CHECK-DAG: Creating  : T{{[0-9]+}} [[B]] stack:
  start_stop_threads();

  return 0;
}
