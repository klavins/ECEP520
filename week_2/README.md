Week 2: Data types in C
===

Review of Week 1
===

Last week we focused on the installation and use of several development tools. The aims were to give you a solid environment in which to develop code in C and C++, as well as to make sure the class as a whole are using the same tools.

-**Visual Code**: Use this editor to edit your source files, header files, and Makefile. Other similar editors such as Atom or Sumblime are fine to use as well. 

-`terminal` or `command prompt`: Everyone should have some kind of terminal window into which they can enter and run `git` and `docker` commands. On a Mac, this is the program called `Terminal`. On a PC, this is a program called `command prompt`. On a PC, if you installed the Docker toolbox version of Docker, then you should use the Docker `Quickstart` shell. We will call the environment that you run git and docker commands the "host environment".

-`git`: Use `git` to keep track of versions of your code, to save your code to `github`, and to retrieve example code and lecture notes from the course repository. To be clear, you will put your code in a repository called `ECE590` under your username. Meanwhile, Prof. Klavins will put example code and course notes in a repository called `ECEP520` under the username `klavins`. You should have cloned the `ECEP520` repository onto your local computer. To get the latest version, run the following `git` command from within the `ECEP520` directory in your host environment:
```bash
git pull origin master
```
which will retrieve the latest changes from github. Note that you can run `git` from either your host environment or the container environment that you get by executing `docker run` with `bash` (see below).

-`docker`: You should have Docker running on your computer by now. For the next several weeks we will continue to use the `cppenv` image to start a container that has the gnu compiler tools, the Google test library, and the Doxygen API documentation tool.

-`bash`: Within your host environment you will execute a command like
```bash
docker run -v /path/to/ECE590:/source -it cppenv bash
```
you will get a `bash` prompt that allows you to run commands within the "container environment". You will be initially placed the the `/source` directory onto which the `-v` option to docker will have mounted the `/path/to/ECE590` directory in your host environment. This directory and its children are the only host directories available within the container environment. You should be familiar with bash commands like `pwd`, `ls`, and `cd` at this point.

-`gcc` and `g++`: These commands are available in the container environment and are used to compile C and C++ source files into object files and executables. Note that some of you may also have these commands available in your host environments, but will not have libraries like `gtest` installed on your host environment. So you should only use these commands in the container environment.

-`make`: This command is also available in the container environment and is a way to script a bunch of long `gcc` or `g++` commands together, to compile only changed files, and to keep track of files and directories. You will normally just use the course Makefile and edit a few lines to tell make what files are part of your source code.

-`gtest`: The is a C library that is installed in the cppenv image and available to `gcc` and `g++` within the container environment. Specifically, the cppenv conatiner has the gtest include files installed in 
```bash
/usr/local/include/gtest
```
which you get by putting
```c
#include "gtest/gtest.h"
```
in your source files. The gtest shared object library (which is like a unix DLL) is located at
```bash
/usr/local/lib/libgtest.a
```
which you link when you compile with the `-lgest` option to `g++`. 

To review, you should now have
- git installed;
- docker installed;
- an editor such as Visual Code installed; 
- a github repo called ECE590; and
- a directory in your ECE590 repo called hw_1 containing all of your homework one code;

The C Programming Language
===

Most of you have encountered C at some point. We will not review all of the details of C syntax. Many guides exist online and book by Kerninghan and Ritchie is an excellent source. If you are not familiar with the following concepts in C, you should review them before attempting the homework

- `if` statements
- `for` loops
- `while` loops
- `do` ... `while` loops
- `switch` ... `case` statements
- operators such as `*`, `+`, `-`, `%`, `||`, `&&`, `>`, `<`, `>=`, `<=`, `!`, `++`, and `--`
- the `a ? b : c` syntax for inline if statments

In this lecture, we will assume these concepts are straightforward and mainly talk about C's type system, which is the most complex aspect of C programming.

Types in C
===

In C, every variable, function argument, and function return value needs to have a declared type. One of the basic types in C is `int`, which refers to a positive, zero, or negative integer. The following bit of code uses the `int` type to define a function that takes an `int` argument, has two local `int` variables, and returns an `int` value. 
```c
int sum(int n) {
  int i,
      s = 0;
  for (i=0; i<n; i++) {
    s = s + i;
  }
  return s;
}
```
Note that when `s` is declared it is also assigned an initial value. Initializating a variable when it is declared is optional, but often a good idea. You can also write the above function like this:
```c
int sum(int n) {
  int s = 0;
  for (int i=0; i<n; i++) {
    s = s + i;
  }
  return s;
}
```
In this case, the variable `i` is delcared within the scope of the for loop and not in the broader scope of the function. In general, you can declare local variables in any block, whether it is a function block, a `for` loop or an `if` statement. In particular, you can declare variables in google test `TEST` blocks. 
```c++
TEST(MyTest, LocalOne) {
  int x = 5;
}

TEST(MyTest, LocalTwo) {
  double x = 6; /* different variable than in the previous block */
}
```
The void type
---

In C, an object with type `void` has no value. Usually we use `void` to refer to functions that do not return anything, such as
```c
void say_hello() {
  printf("hello\n");
  return;
}
```
Note that you cannot declare a variable of type `void`.

Basic Types
---
The following code demonstrates most of the basic integer types in C along with some initializations showing what sorts of values they take. Note that character values such as `'a'`, `'b'` and `'c'` are shorthand for ASCII integer values, in this case `97`, `98`, and `99`. 
```c
char a = 97;             /* one byte */
unsigned char b = 'b';
signed char c = -99;
short d;                 /* two bytes */
unsigned short e;
int f;                   /* two or four bytes */
unsigned it g; 
long h;                  /* four bytes */
unsigned long i;
```
To see how many bytes a type has, you can use the `sizeof` function, as in 
```c
printf("The size of an int on this machine is %d\n", sizeof(int));
```
In addition , the following floating point types are available:
```c
float x;                 /* four bytes, 6 decimal places     */
double y;                /* eight bytes, 15 decmial places   */
long double z;           /* sixteen bytes, 19 decimal places */
```
You trade storage space and speed as you increase the size of these types.

The minimum and maximum values of variables with these types for the particular C implemtation you are working with are noted in the `<limits.h>` header file. If you include this header in a source file, open the file with Visial Studio Code, and follow its definition, you'll see all sorts of C pre-processor macros defining limits. To use the definitions, for example, do:
```c
#include <limits.h>
#include <stdio.h>
...
printf("The minimum value of INT = %d\n", INT_MIN);
printf("The maximum value of INT = %d\n", INT_MAX);
...
```
which on an Intel machine will print out
```bash
The minimum value of INT = -2147483648
The maximum value of INT = 2147483647
```

Modifiers
---
Type declarations can be further modified with the keywords 

- `register`: This keyword is used to strongly suggest to the compiler that the variable should be stored in a register instead of in RAM. You would do something like this:
    ```c
    void f(int x) {
      register int i;
      for(i=0; i<10; i++) {
        /* do something */
      }
    }
    ```
    However, most compilers know how to figure out when to use a register for a counter without the `register` keyword, so you do not almost certainly will not need to use this modifer.

- `volatile`: This keyword is used to refer to locations in memory that might change do to something happening outside of the code. It prevents the compiler from assuming that a volatile variable that is assigned only once in the code will never change and subsequently optimizing it out of existence. For example, on an embedded device, if you know that location 0x5555 was a globally available register being written to by, for example, a sensor or interrupt handler, then you could do
    ```c
    voif f() {
      volatile int * x = 0x5555; /* x is a pointer to the 
                                  * location 0x5555 (see pointers
                                  * below) */
      while ( *x == 0 ) {
        /* wait */
      }
      /* do something because *x changed, 
       * presumably due to some outside event */
    }
    ```
- `static`: A static variable is one that preserves its value even after it has gone out of scope. For example, compare the following two functions
    ```c
    int f() {
      int x = 0;
      x++;
      return x;
    }

    int g() {
      static int x = 0; /* note: must be a literal constant,
                         *  not a computed value */
      x++;
      return x;
    }
    ```
    If you call `f` twice in a row, you will get the value `1` each time. If you call `g` twice in a row, you will get `1` the first time and `2` the second time. Thus, the function `g` is using `x` as local storage. It initializes the variable the first time it is called, but does not reinitialize it upon subsequent calls to the function.

    The `static` keyword is also used in a *totally different way* to declare functions as local to the file in which they are defined. If a function is defined without static, as in
    ```c
    int f(int x) {
      return x+1;
    }
    ```
    then it is globally available to your code (assuming your code includes its declaration). If a function is defined as static, then it is only available in that file:
    ```c
    static int f(int x) {
      return x+1;
    }
    ```
    If you put this in a file called `my_source.c`, then only codw within `my_source.c` can use the function `f`. This is a way to make private functions that you do not want users of an API to access. 

- `const`: This keyword is used in variable declarations to make symbols that refer to constants. For example
    ```c
    int f() {
      const double PI = "3.14159"; 
      /* etc */
    }
    ```
    The compiler will complain if you attempt to reassign `PI` later in the code for `f`.

    The use of `const` gets complicated when combined with pointers (see below). In short, you can define a constant pointer with
    ```c
    int * const ptr;
    ```
    and a pointer to a constant value with
    ```c
    const int * ptr;
    ```

    Finally, if you have a function that takes a pointer to a value and want to enforce that the function does not modify the pointed to value, then you would define the argument to the function as follows:
    ```c
    void f(const int * p) {
      /* do things like print *p but don't
       * modify it */
    }
    ```
    The follow example will produce a compile error because the function attempts to change the value pointed to by `x`.
    ```c
    int f ( const int * x ) {
        *x = *x + 1;
        return *x;
    }    
    ```
    Compiling this code gives the error
    ```bash
     error: assignment of read-only location '* x'
     *x = *x + 1;
        ^
    ```

- `extern`: This keyword is used in certain circumstances to declare functions without defining them, and to tell the compiler to go ahead and link your code expecting that the function will be defined somewhere else. We will likely not need it, although you will see it a lot in header files we include.

Structures
---
A structure in C is like a record or dictionary in other languages. It is a way to define a new type of object to store information that belongs together. For example, you might use the following structure to keep track of the information associated with a new data type you are defining called point and then declare some points.
```c
struct point {
  double x, y, z;
};
struct point p, q;
```
You can then refer to the components of a point with the `.` notation as in `p.x` or `q.z`. If you do not name the `struct` then you can declare `p` and `q` directly, but then cannot declare more structs of the same type:
```c
struct {
  double x, y, z;
} p, q;
```
If you would like to avoid having to write `struct point` over and over, you can also make a type definition as in the following example:
```c
typedef struct point
  double x, y, z;
} Point;
Point p, q;
```
which also delcared `p` and `q` to be of type `struct point`. 

You can initialize a struct using either of the following notations:
```c
Point p = (Point) { .x = 1.0, .y = 2.0, .z = 3.0 };
Point q = (Point) { 1.0, 2.0, 3.0 };
```
In the latter case, the order in which the members of the struct were declared determines the order in which the initializers should appear. Note that the syntax `(Point)` is an example of coercion. The value in the curly braces is not techically a `Point` but a raw struct constant. To assign it to `p`, you need to explicitly *cast* or *coerce* the raw value into a `Point`.

Unions
---
A `union` is like a `struct`, but with all members using the smae memory location. A `union` allows you to use only **one** of the members of the union at the same time. For example,
```c
typedef union thing {
  int x;
  double y;
  float z;
} Thing;
Thing t;
```
In this case, the addresses in memory of `t.x`, `t.y` and `t.z` are all the same. If we replaced the above with a `struct`, they would all be different. Unions were used back in the days when memory was scares. There is no really good reason to use them any more. In fact, they are pretty dangerous.

Enums
---
An `enum` is a way to enumerate the possible values a variable might have. For example
```c
typedef enum status {
  IDLE, RUNNING, ERROR
} Status;
Status x = IDLE;
```
defines a variable of type `Status` whose values are either `IDLE`, `RUNNING` or `ERROR`. These values are *not* strings. They are symbols. However, in C (but not in C++), the compiler actually just uses the integers 0, 1, 2, ... internally to represent the values in an enum. Thus, you will notice that you can treat them like integers, but you should make every effort not to do so, since other compilers may use different numbers to represent an enum's values. 

Pointers
---
The most difficult aspect of C is its use of pointers, which most other higher level languages like Python or Javascript do not have. When you declare a variable, the C compiler has to store it in memory somewhere. The location in memory of the value of a variable is called its *address*. So a pointer variable is a variable whose value is an address. 

There are two operators in C that you use to change back and forth between a variable's value and its address. The first is the `&` operator, which finds a variables address. For example,
```c
int p = 1;
printf("The value of p is %d and the address of p is %x.\n", p, &p);
```
which would print out something like:
```bash
The value of p is 1 and the address of p is e5788eac.
```
See the [documentation](http://www.cplusplus.com/reference/cstdio/printf) for `printf` for details about what %d and %x do. The hexadecimal number e5788eac is the physical address in memory of the first byte of the integer p. 

The second operator is the `*` operator, which dereferences a pointer and is also used to declare a pointer. For example, suppose `p` and `ptr` were declared as follows:
```c
int p = 1;
int * ptr = &p;
```
then `p` is an integer and `ptr` is the address of `p` in memory. If you would like to get the value pointed to by `ptr`, then you would use the syntax `*p`. For example, 
```c
int q = *ptr; /* q is 1 */
*ptr = 2;     /* indirectly changes p to 2 */
```

One of the places pointers are used extensively in C is with pointers to structs. This is because a struct can be quite large, and passing structs around by copying everything in them is a waste of time and space. When you have a variable that is a pointer to a struct, then you use the `->` operator instead of the `.` operator, as in the following example:
```c
typedef {
  double x, y, z;
} Point;
Point * ptr;
ptr->x = 3;
```
Actually, `ptr->x` is really just shorthand for
```c
(*p).x
```
but is more prefered. 

You can also define pointers to functions ... TODO

Arrays
---

Strings are arrays of chars;
---
Strings in C are contiguous regions of one byte memory addresses whose values are usually interpreted as characters. To declare and initialize strings, you do something like:
```c
char x[] = "Hi";
char y[3] = { 'H', 'i', '\0' };
char z[3];
char z[0] = 'H';
char z[1] = 'i';
char z[2] = '\0';
```
The special character `\0` is called the **null** character and is used to terminate strings so that functions that manipulate them know when to stop.

Exercises
===

1. If you are unfamiliar with C or need a refresher, you should spend a few hours reading through a good tutorial, such as [this one](https://www.tutorialspoint.com/cprogramming/index.htm). 
1. For homework 2 you will write a set of functions that are mostly unrelated, but for convenience we will put into one source file and one header file. Your homework 2 directory should look like
    ```bash
    - ECE590
      - hw_2
        - main.c
        - unit_tests.c 
        - solutions.h
        - solutions.c
        - Makefile
    ```
    the `unit_tests.h` file will include `solutions.h`, which should have all your function declarations in it. The implementations of those functions should be put in `solutions.c`. 
1. Write a function called `reverse_in_place` which takes an array and its length and reverses it in place. Something like the following tests should apply.
    ```c
    TEST(HW2,ReverseInPlace) {
        int x[] = {10,20,30,40,50};
        reverse_in_place(x,5);
        ASSERT_EQ(x[0],50);
        ASSERT_EQ(x[1],40);       
        ASSERT_EQ(x[2],30);   
        ASSERT_EQ(x[3],20);   
        ASSERT_EQ(x[4],10);                            
    }
    ```
1. Write a function called `reverse` that takes an array and its length and returns a new array that is the reverse of the given array. The function should use `calloc` to create space for the new array. A test for this might look like
    ```c
    TEST(HW2,Reverse) {
        int x[] = {10,20,30,40,50};
        int * y = reverse(x,5);
        ASSERT_EQ(y[0],50);
        ASSERT_EQ(y[1],40);       
        ASSERT_EQ(y[2],30);   
        ASSERT_EQ(y[3],20);   
        ASSERT_EQ(y[4],10);  
        free(y);
    }
    ```
  1. Write a function called `num_instances` that takes an array of integers and a value and returns the number of instances of that value in the array. 
     ```c
     TEST(HW2,NumInstances) {
       int a[] = { 1, 1, 2, 3, 1, 4, 5, 2, 20, 5 };
       ASSERT_EQ(num_instances(a,1), 3);
     }
     ```
  1. Write a function called `to_set` that takes an array and returns a new array with all duplicates removed. For example, the following test should pass.
      ```c
      TEST(HW2,ToSet) {
        int a[] = { 1, 1, 2, 3, 1, 4, 5, 2, 20, 5 };
        int * b = to_set(a);
        ASSERT_EQ(num_instances(b,1),1);
        ASSERT_EQ(num_instances(b,2),1);
        ASSERT_EQ(num_instances(b,42),0);        
      }
      ```
1. Write a function called `map` which takes an array of doubles, its length, and a function pointer that returns a newly allocated array whose values are the values of the function argument applied to the array argument. Here is a test that should pass:
    ```c
    double f(double x) {
      return x*x;
    }

    TEST(HW2,Map) {
      double x[] = { 1.1, 2.2, 3.3 };
      double * y = map(x,3,f);
      for (int i=0; i<3; i++ ) {
        ASSERT_EQ(y[i],x[i]*x[i]);
      }
      free(y);
    }
    ```
1. Write a function called `map_to_point_array` that is similar to `map` above, but which takes an array of point structures and returns a function mapped onto the array. You should put the following `typedef` in your `solutions.h` file:
    ```c
    typedef struct {
      double x, y, z;
    } Point;
    ```
    Then tests like the following should pass.
    ```c
    Point negate(Point p) {
      return = (Point) { -p.x, -p.y, -p.z };
    }

    TEST(HW2,PointMap) {
      Point a[] = { (Point) { 1,2,3 }, (Point) { 2,3,4 } };
      Point * b = map_to_point_array(a,2,negate);
      for(int i=0; i<2; i++) {
        ASSERT_EQ(b[i].x,-a[i].x);
        ASSERT_EQ(b[i].y,-a[i].y);
        ASSERT_EQ(b[i].z,-a[i].z);
      }
      free(b);
    }   
    ```

