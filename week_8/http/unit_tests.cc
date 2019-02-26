#include <thread>         // std::thread
#include <mutex>          // std::mutex


#include <gtest/gtest.h>
#include <json/json.h>
#include <iostream>
#include <string>
#include <chrono>

namespace {


    std::mutex mtx;           // mutex for critical section

    void print_block (int n, char c) {
      // critical section (exclusive access to std::cout signaled by locking mtx):
      mtx.lock();
      for (int i=0; i<n; ++i) { 
        std::cout << c; 
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
      std::cout << '\n';
      mtx.unlock();
    }

    TEST(MUTEX,MUTEX) {

      std::thread th1 (print_block,150,'*');
      std::thread th2 (print_block,150,'$');

      th1.join();
      th2.join();

    }

}
