#include <math.h>
#include <float.h> /* defines DBL_EPSILON */
#include "dynamic_array.h"
#include "arbitrary_array.h"
#include "gtest/gtest.h"

#define X 1.2345

typedef struct {
    double x, y, z;
} Point;

char * point_to_string ( void * p ) {
    Point * q = (Point *) p;
    char * str = (char *) calloc(40, sizeof(char));
    snprintf(str,40,"{%.1lf,%.1lf,%.1lf}", q->x, q->y, q->z);
    return str;
}

namespace {

    TEST(Destroy, Destroy) {
        DynamicArray * a = DynamicArray_new();
    }

    TEST(ArbitraryArray,OfPointers) {

        // Create the array that will hold the pointers
        ArbitraryArray * ptrs = ArbitraryArray_new(sizeof(DynamicArray *));

        // Create a couple of dynamic arrays
        DynamicArray * a = DynamicArray_new(),
                     * b = DynamicArray_new();

        // Add the dynamic arrays to the pointer array
        ArbitraryArray_set_from_ptr(ptrs, 0, &a);
        ArbitraryArray_set_from_ptr(ptrs, 1, &b);

        // Get them back. Note that because the array contains pointers,
        // and _get_ptr returns a pointer to a pointer, we need to cast
        // its result as a pointer to a DynamicArray pointer.
        DynamicArray ** ptr_a = (DynamicArray **) ArbitraryArray_get_ptr(ptrs,0);
        DynamicArray ** ptr_b = (DynamicArray **) ArbitraryArray_get_ptr(ptrs,1);

        // Check that the pointers are equal
        ASSERT_EQ(*ptr_a, a);
        ASSERT_EQ(*ptr_b, b);
        ASSERT_NE(*ptr_a, b);
        ASSERT_NE(*ptr_b, a);        
    }

//     TEST(DynamicArray, SmallIndex) {
//         DynamicArray * da = DynamicArray_new();
//         ASSERT_EQ(DynamicArray_size(da),0);
//         DynamicArray_set(da, 0, -X);        
//         DynamicArray_set(da, 3, X);
//         ASSERT_EQ(DynamicArray_size(da),4);
//         DynamicArray_print_debug_info(da);
//         ASSERT_EQ(DynamicArray_get(da,0), -X);
//         ASSERT_EQ(DynamicArray_get(da,3), X);
//         DynamicArray_destroy(da);
//     }

//     TEST(DynamicArray, BigIndex) {
//         DynamicArray * da = DynamicArray_new();
//         DynamicArray_set(da, 8, X);
//         DynamicArray_print_debug_info(da);        
//         ASSERT_EQ(DynamicArray_get(da,8), X);
//         DynamicArray_destroy(da);              
//     }

//     TEST(DynamicArray, ReallyBig) {
//         DynamicArray * da = DynamicArray_new();
//         DynamicArray_set(da, 400, X);
//         DynamicArray_set(da, 200, X/2);        
//         DynamicArray_print_debug_info(da);        
//         ASSERT_EQ(DynamicArray_get(da,200), X/2);
//         ASSERT_EQ(DynamicArray_get(da,400), X);
//         DynamicArray_destroy(da);              
//     }  

//     TEST(DynamicArray, Push) {
//         DynamicArray * da = DynamicArray_new();
//         double x = 1;
//         while ( x < 10 ) {
//             DynamicArray_push(da, x);  
//             x += 0.25;
//         }
//         DynamicArray_print_debug_info(da); 
//         while ( DynamicArray_size(da) > 0 ) {
//             DynamicArray_pop(da);
//         }
//         DynamicArray_print_debug_info(da); 
//         DynamicArray_destroy(da);          
//     }

//     TEST(DynamicArray, PushFront) {
//         DynamicArray * da = DynamicArray_new();
//         double x = 1;
//         while ( x < 10 ) {
//             DynamicArray_push_front(da, x);  
//             x += 0.25;
//         }
//         DynamicArray_print_debug_info(da); 
//         while ( DynamicArray_size(da) > 0 ) {
//             DynamicArray_pop_front(da);
//         }
//         DynamicArray_print_debug_info(da);         
//         DynamicArray_destroy(da);          
//     } 

//     TEST(DynamicArray, Pop) {
//         DynamicArray * da = DynamicArray_new();
//         double x;
//         DynamicArray_push(da, X);  
//         DynamicArray_push(da, X);  
//         x = DynamicArray_pop(da);  
//         DynamicArray_print_debug_info(da);         
//         ASSERT_EQ(DynamicArray_size(da),1);
//         ASSERT_EQ(x,X);
//         ASSERT_EQ(DynamicArray_get(da,1), 0.0);
//         DynamicArray_print_debug_info(da);         
//         DynamicArray_destroy(da);          
//     }

//     TEST(DynamicArray, Map) {
//         DynamicArray * t = DynamicArray_new(),
//                      * y;
//         double s = 0.0;
//         for ( int i=0; i<628; i++ ) {
//             DynamicArray_set(t, i, s);
//             s = s + 0.1;
//         }
//         y = DynamicArray_map(t,sin);
//         for (int i=0; i<DynamicArray_size(t); i++) {
//             ///ASSERT_EQ(DynamicArray_get(y,i), sin(0.1*i));
//             // ASSERT_DOUBLE_EQ(DynamicArray_get(y,i), sin(0.1*i));
//             ASSERT_LT(
//                 abs(DynamicArray_get(y,i) - sin(0.1*i)),
//                 DBL_EPSILON
//             );
//         }
//         DynamicArray_destroy(t);    
//         DynamicArray_destroy(y);                    
//     }        

//    TEST(ArbitraryArray, Basics) {

//        ArbitraryArray * a = ArbitraryArray_new(sizeof(double));
//        ArbitraryArray * b = ArbitraryArray_new(sizeof(Point));
//        ArbitraryArray * c = ArbitraryArray_new(sizeof(ArbitraryArray));

//        ASSERT_EQ(NULL, ArbitraryArray_get_ptr(a,0));
//        ASSERT_EQ(NULL, ArbitraryArray_get_ptr(b,0));
//        ASSERT_EQ(NULL, ArbitraryArray_get_ptr(c,0));

//        double x = 3.14;
//        ArbitraryArray_print_debug_info(a);
//        ArbitraryArray_set_from_ptr(a, 0, &x);
//        ArbitraryArray_print_debug_info(a);
//        ASSERT_EQ(x, *((double *) ArbitraryArray_get_ptr(a,0)));

//        Point p = { 3.1, 4.1, 5.9 };
//        ArbitraryArray_set_from_ptr( b, 9, &p );
//        ArbitraryArray_print_debug_info(b);       

//        Point * q = ((Point *) ArbitraryArray_get_ptr(b,9));
//        ASSERT_EQ(p.x, q->x);
//        ASSERT_EQ(p.y, q->y);
//        ASSERT_EQ(p.z, q->z);

//        ArbitraryArray_destroy(a);
//        ArbitraryArray_destroy(b);
//        ArbitraryArray_destroy(c);

//    }    

}