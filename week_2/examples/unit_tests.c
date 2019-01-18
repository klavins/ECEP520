#include "gtest/gtest.h"
#include "examples.h"

namespace {

    TEST(Examples,AlocateAndFree) {
        int x[] = { 0, 1, 2 };
        int y[] = { 3, 4, 5 };
        int * z = join(x, 3, y, 3);
        for (int i=0; i<6; i++ ) {
            ASSERT_EQ(z[i], i);
        }
        free(z);
        
    }

}
