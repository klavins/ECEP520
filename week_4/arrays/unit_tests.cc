#include <math.h>
#include <float.h> /* defines DBL_EPSILON */
#include <assert.h>
#include "double_array.h"
#include "typed_array.h"
#include "gtest/gtest.h"

namespace {

    class Point {
        public:
        Point(double x, double y, double z) : x(x), y(y), z(z) {};
        Point() : x(0), y(0), z(0) {};

        double magnitude(const Point &p) {
            return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
        }

        double x, y, z;
    };

    double magnitude(const Point &p) {
        return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
    }

    bool operator< (const Point& lhs, const Point& rhs) {
        double m1 = magnitude(lhs),
                m2 = magnitude(rhs);
        if ( m1 < m2 ) {
            return 1;
        } else if ( m1 > m2 ) {
            return -1;
        } else {
            return 0;
        }
    }    

    Point &smaller(Point &p, Point &q) {
        if ( magnitude(p) <= magnitude(q) ) {
            return p;
        } else {
            return q;
        }
    }

    Point &f() {
        Point p = {1,2,3};
        Point &q = p;
        return q;
    }

    std::ostream &operator<<(std::ostream &os, const Point &p)
    {
        os << '{' << p.x << "," << p.y << "," << p.z << '}';
        return os;
    }

    double invert(int x, int y) {
        if ( x == 0 ) {
            throw std::invalid_argument("First argument cannot be zero");
        } else {
            return y/x;
        }
    }    

    class Imaginary {
        public:
        Imaginary(double x, double y) : re(x), im(y) {}
        private:
        double re, im;
    }; 

    template<typename T>
    int compare(const T& x, const T& y) {
        if ( x < y ) {
            return -1;
        } else if ( y < x ) {
            return 1;
        } else {
            return 0;
        }
    }

    TEST(Templates,Functions) {
        compare<int>(1,2);
        compare<double>(3.0, 4.0);
        compare<Point>((Point){1,2,3}, (Point) {3,-4,2});
    }

    TEST(Im,Im) {
        Imaginary x();
    }

    TEST(References,References) {
        Point p = { 0,3,4 }, q;
        Point &r = smaller(p, q);
        ASSERT_DOUBLE_EQ(magnitude(p), 5);
        ASSERT_EQ(r.x, 0);
        ASSERT_EQ(r.y, 0);
        ASSERT_EQ(r.z, 0);
        r.x = -1;
        ASSERT_DOUBLE_EQ(r.x, q.x);
    }

    TEST(Except,Except) {
        try {
            invert(0,1);
            ASSERT_EQ(true,false);
        } catch (std::invalid_argument e ) {
            ASSERT_STREQ(e.what(), "First argument cannot be zero");
        }
    }

    TEST(Memory,NewDelete) {
        int * x = new int;
        double * p = new double[10];
        delete x;
        delete[] p;        
    }

    TEST(DoubleArray, ConstructSetGet) {       
        DoubleArray a, b(0,1,0.1);
        auto c = new DoubleArray();
        a.set(4, 1);
        ASSERT_EQ(a.get(4), 1); 
        double x = 0;
        for ( int i=0; i<b.size(); i++ ) {
            ASSERT_EQ(b.get(i),x);
            x+=0.1;
        }
        delete c;
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

    TEST(Assert,Assert) {
        // assert(0);
        ASSERT_DEATH(int x = 1+1,"");
        // try {
        //     assert(0);
        //     ASSERT_EQ(0,1);
        // } catch (...) {
        //     ASSERT_EQ(1,1);
        // }
    }

    TEST(TypedArray, Matrix) {

        TypedArray<TypedArray<double>> m;

        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                m.get(i).set(j,0.5*i*j);
            }
        }

        std::cout << m << "\n"; 

        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                ASSERT_DOUBLE_EQ(m.get(i).get(j),0.5*i*j);
            }
        }

    }

}