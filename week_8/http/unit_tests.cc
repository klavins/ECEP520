#include <thread>         // std::thread
#include <mutex>          // std::mutex

#include <gtest/gtest.h>
#include <json/json.h>
#include <iostream>
#include <string>
#include <chrono>

#include <stdlib.h> // rand

namespace {

    std::mutex mtx;           // mutex for critical section
    char buffer[301];
    int index = 0;

    void print_block (int n, char c) {
      // critical section (exclusive access to std::cout signaled by locking mtx):
      // mtx.lock();
      for (int i=0; i<n; ++i) { 
        buffer[index] = c;
        index++;
        std::this_thread::sleep_for(std::chrono::milliseconds(rand()%5));
      }
      // mtx.unlock();
    }

    TEST(MUTEX,MUTEX) {

      buffer[300] = 0;

      std::thread t1 (print_block,150,'*');
      std::thread t2 (print_block,150,'$');

      t1.join();
      t2.join();

      std::cout << buffer << "\n";
      auto desired_result = std::string(150,'*') + std::string(150,'$');
      ASSERT_STREQ(desired_result.c_str(), buffer);

    }

}
