#include <math.h>
#include <float.h> /* defines DBL_EPSILON */
#include "double_array.h"
#include "typed_array.h"
#include "gtest/gtest.h"

class Point {
    public:
       Point(double x, double y, double z) : x(x), y(y), z(z) {};
       Point() : x(0), y(0), z(0) {};
       double x, y, z;
};

std::ostream &operator<<(std::ostream &os, const Point &p)
{
    os << '{' << p.x << "," << p.y << "," << p.z << '}';
    return os;
}

namespace {

    TEST(DoubleArray, ConstructSetGet) {       
        DoubleArray a, b(0,1,0.1);
        a.set(4, 1);
        ASSERT_EQ(a.get(4), 1); 
        double x = 0;
        for ( int i=0; i<b.size(); i++ ) {
            ASSERT_EQ(b.get(i),x);
            x+=0.1;
        }
    }

    TEST(DoubleArray, Assignment) {
        DoubleArray a, b;  
        a.set(4, 1);
        b = a;
        ASSERT_EQ(a.get(4), 1);
        ASSERT_EQ(b.get(4), 1);                
    }    

    TEST(DoubleArray, CopyConstructor) {
        DoubleArray a;
        a.set(4, 1);        
        DoubleArray b(a);
        ASSERT_EQ(a.get(4), 1);
        ASSERT_EQ(b.get(4), 1);            
    }

    TEST(DoubleArray, Exceptions) {
        DoubleArray a;
        ASSERT_THROW(a.set(-1,0), std::range_error);
        ASSERT_THROW(a.get(-1), std::range_error);
        try {
            a.get(-1);
            ASSERT_EQ(0,1); // Should not reach this code
        } catch ( std::range_error e ) {
            ASSERT_STREQ(e.what(), "Negative index in array");
        }
    }

    TEST(TypedArray, Construction) {
        TypedArray<Point> b;
        b.set(0, Point(1,2,3));
        b.set(1, Point(2,3,4));
        b.set(20, Point(3,4,5));
        ASSERT_EQ(b.get(0).x, 1);
        ASSERT_EQ(b.get(1).y, 3);
        ASSERT_EQ(b.get(20).z, 5);
    }

    TEST(TypedArray, Matrix) {

        TypedArray<TypedArray<double>> m;

        for (int i=0; i<30; i++) {
            for (int j=0; j<30; j++) {
                m.get(i).set(j,0.5*i*j);
            }
        }

        std::cout << m << "\n"; 

        for (int i=0; i<30; i++) {
            for (int j=0; j<30; j++) {
                ASSERT_DOUBLE_EQ(m.get(i).get(j),0.5*i*j);
            }
        }

    }

}