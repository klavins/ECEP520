#include "gtest/gtest.h"
#include <iostream>
#include <vector>

namespace {

  using namespace std;

  TEST(Vector,Vector) {
      vector<double> v;
      for (int i=0; i< 50; i++) {
        v.push_back(i);

      }

      for (double &i : v)
        i *= i;
      for (double &i : v)        
        std::cout << i << " "; 
    
      std::cout << "\n";

      std::cout << v.capacity() << "\n";

      v.shrink_to_fit();

      std::cout << v.capacity() << "\n";
      v.push_back(1);
      std::cout << v.capacity() << "\n";      

  }

}