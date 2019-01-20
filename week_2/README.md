Week 2: Data types in C
===

Review of Week 1
===

Last week we focused on the installation and use of several development tools. The aims were to give you a solid environment in which to develop code in C and C++, as well as to make sure the class as a whole are using the same tools.

-**Visual Code**: Use this editor to edit your source files, header files, and Makefile. Other similar editors such as Atom or Sublime are fine to use as well. 

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

-`make`: This command is also available in the container environment and is a way to script a bunch of long `gcc` or `g++` commands together, to compile only changed files, and to keep track of files and directories. You will normally just use the course Makefile and edit a few lines to tell make what files are part of your source code. We have made a few changes to the Makefile, by the way. First, you no longer need to add your source and header files, because we have changed the `HEADERS` and `SOURCES` definitions to:
```make
HEADERS     := $(wildcard *.h)
SOURCES     := $(wildcard *.c)
```
Second, we have removed documentation generation from the `all` definition. Now, to make the executable, you just type
```make
make
```
and to make the documentation, you type
```
make docs
```

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

printf
===

One of the most useful functions in C is the `printf`. You can use it to format strings together with variables to print information to the shell. The declaration of `printf` is
```c
int printf ( const char * format, ... );
```
which means it takes a pointer to a (null terminated) string of characters and an optional set of arguments. The optional arguments are values that will be interpoloated into the string based on 'format specifiers', one for each type. For example,
```c
int x = 1;
double y = 2.3;
char z[] = "uw";
printf("x = %d, y = %lf, z = '%s'\n", x, y, z );
```
prints out the values of `x`, `y`, and `z` in a nicely formatted way. See the [documentation](http://www.cplusplus.com/reference/cstdio/printf) for `printf` for a list of the other format specifiers and modifiers available to `printf`.

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
  double x = 6.28; /* different variable than in the previous block */
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
unsigned int g; 
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
typedef struct point {
  double x, y, z;
} Point;
Point p, q;
```
which also delcared `p` and `q` to be of type `struct point`. 

You can initialize a struct using either of the following notations:
```c
Point p = { .x = 1.0, .y = 2.0, .z = 3.0 };
Point q = { 1.0, 2.0, 3.0 };
```
The order in which the members of the struct were declared determines the order in which the initializers should appear. 

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
In this case, the addresses in memory of `t.x`, `t.y` and `t.z` are all the same. If we replaced the above with a `struct`, they would all be different. 

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
The hexadecimal number e5788eac is the physical address in memory of the first byte of the integer p. 

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
typedef struct {
  double x, y, z;
} Point;
Point * ptr;
ptr->x = 3;
```
Actually, `ptr->x` is really just shorthand for
```c
(*ptr).x
```
but is more preferred. 

You can also define pointers to functions. The syntax is tricky. For example, the following defines a function `add` and a pointer `f_ptr` to it.
```c
int add(int n, int m) {
  return n+m;
}
int (* f_ptr) (int,int);
f_ptr = add;
```
You can use this syntax to send functions to other functions as arguments. For example, the following function applies a function to a value and returns the value.
```c
int apply(int (*f) (int), int x) {
  return f(x);
}
```

Arrays
---
Arrays in C are contiguous regions of memory that contain strings of values.
Arrays can be declared with the `[]` operator as follows:
```c
int x[10];                             /* an array of 10 integers */
Point plist[20];                       /* An array of 20 Point structures */
double y[] = { 2.1, 3.2, 4.3, 5.4, };  /* Array of four doubles with initial values */
```
Arrays are zero indexed. Elements can be assigned and retrieved using the `[]` operator as well. For example,
```c
x[0] = 1;
x[1] = x[0];
plist[5] = (Point) { 3.1, 4.1, 5.9 };
y[3] = plist[5].y;
```
In the above cases, `x`, `plist` and `y` are just pointers to the beginning of the memory location for the arrays they represent. The `[]` operator is just shorthand for pointer arithmetic. Thus, the above code is equivalent to the following:
```c
*x = 1;
*(x+1) = *(x);
*(plist + 5) = (Point) { 3.1, 4.1, 5.9 };
*(y+3) = (plist+5)->y;
```

**Warning**: Arrays in C are not bounds checked. For example, the following code may compile just fine, but in fact contains a serious error.
```c
int a[10];
a[12] = 5;
ASSERT_EQ(a[12], 5);
ASSERT_EQ(a[13], 0);
```
This compiles and runs without error in the `cppenv` container, but it is just luck. The memory locations at `a+12` and `a+13` just happen to be unused. If you do try to write to `a[1000]` or larger, you will almost certainly encounter either
- a segmentation fault, meaning that you wrote a value outside the memory reserved by the OS for your application;
- strange behavior, meaning you wrote a value to some other data structure's portion of memory.
To catch errors like this, you can use a tool called 'Address Sanitizer'. To use it, we modify the Makefile as follows
```c
CFLAGS      := -fsanitize=address -ggdb
LIB         := -lgtest -lpthread -lasan
```
Now, the code still compiles, but when you run it you get all sorts of useful error information from `asan`.

Memory Allocation
---
When you declare arrays as with the above, you know at compile time how big they should be. However, often you do not know this, and may also need to write functions that return arrays. To dynamically allocate memory in C, you use the functions `malloc` and `calloc`, which are available in `stdlib`. For example, to dynamically allocate memory for 10 doubles, you can do:
```c
double * a = (double *) malloc(10*sizeof(double));
```
or
```c
double * a = (double *) calloc(10,sizeof(double)); /* also inits array to zeros */
```
Now `a` can be used just like a normal array, with elements `a[0]`, `a[1]` and so on. Note that 'malloc' and 'calloc' return `void *` pointers, because they do not have any way of knowing what type of array you want. Thus, we have to *type cast* or *coerce* the value returned into the correct type. That is what the `(double *)` notation does.

It is important to note that if you declare a pointer and allocate memory for it in a function, then the pointer disappears when the function is complete, but the memory allocated does not. Thus, when you are done using the memory, your code must give the memory back to the operating sytem using the `free` function, also in `stdlib.h`. For example,
```c
void f() {
  int * a = (int *) calloc(1000,sizeof(int));
  /* do stuff with a */
  free(a);
}
```
This issue becomes particularly important when you use functions that allocate memory for you. For example, here is a function that joins two arrays together into a new array:
```c
int * join(const int * a, int length_a, const int * b, int length_b) {
    int * c = (int *) calloc(length_a+length_b, sizeof(int));
    for (int i=0; i<length_a; i++ ) {
        c[i] = a[i];
    }
    for (int i=0; i<length_b; i++ ) {
        c[i+length_a] = b[i];
    }
    return c;
}
```
To use this function and then free the result, you might do
```c
TEST(Examples,AlocateAndFree) {
    int a[] = { 0, 1, 2 };
    int b[] = { 3, 4, 5 };
    int * c = join(a, 3, b, 3);
    for (int i=0; i<6; i++ ) {
        ASSERT_EQ(c[i], i);
    }
    free(c);
}
```
Repeated failure to free the result of `join` will result in a *memory leak*, which will eventually use up all the RAM on your computer, causing a crash. These are hard to catch. Memory leaks are in fact one of the biggest issues plaguing modern software. Modern languages have *garbage collectors* to clean up unused memory, but (a) they don't work in every case and (b) they are written in C or C++ by humans who make mistakes.

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

Exercises (Due Friday 25 Jan at 11:59pm)
===

- If you are unfamiliar with C or need a refresher, you should spend a few hours reading through a good tutorial, such as [this one](https://www.tutorialspoint.com/cprogramming/index.htm). 
- For homework 2 you will write a set of functions that are mostly unrelated, but for convenience we will put into one source file and one header file. Your homework 2 directory should look like
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

---

1. Write a function called `running_total` that keeps track of the sum of the arguments it has been called with over time. The following test should pass.
    ```c
    TEST(HW2,RunningTotal) {
      ASSERT_EQ(running_total(1),  1);
      ASSERT_EQ(running_total(1),  2);
      ASSERT_EQ(running_total(5),  7);
      ASSERT_EQ(running_total(-3), 4);      
    }       
    ```   
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
  1. Write a function called `num_instances` that takes an array of integers, a length, and a value and returns the number of instances of that value in the array. 
     ```c
     TEST(HW2,NumInstances) {
       int a[] = { 1, 1, 2, 3, 1, 4, 5, 2, 20, 5 };
       ASSERT_EQ(num_instances(a,10,1), 3);
     }
     ```
  1. Write a function called `to_set` that takes an array, its length, a pointer to an integer, and returns a new array with all duplicates removed. As a side effect, it should change the value pointed to by its third argument to the length of the set. For example, the following test should pass.
      ```c
      TEST(HW2,ToSet) {
        int a[] = { 1, 1, 2, 3, 1, 4, 5, 2, 20, 5 };
        int n;
        int * b = to_set(a, 10, &n);
        ASSERT_EQ(n,6);
        ASSERT_EQ(num_instances(b,6,1),1);
        ASSERT_EQ(num_instances(b,6,2),1);
        ASSERT_EQ(num_instances(b,6,42),0);        
      }
      ```
1. Write a function called `map` which takes an array of Points, its length, and a function pointer that returns a newly allocated array whose values are the values of the function argument applied to the array argument. You should put the following `typedef` in your `solutions.h` file:
    ```c
    typedef struct {
      double x, y, z;
    } Point;
    ```
    Then tests like the following should pass.
    ```c
    Point negate(Point p) {
      return = { -p.x, -p.y, -p.z };
    }

    TEST(HW2,PointMap) {
      Point a[] = { { 1,2,3 }, { 2,3,4 } };
      Point * b = map(a,2,negate);
      for(int i=0; i<2; i++) {
        ASSERT_EQ(b[i].x,-a[i].x);
        ASSERT_EQ(b[i].y,-a[i].y);
        ASSERT_EQ(b[i].z,-a[i].z);
      }
      free(b);
    }   
    ```
1. Write a reverse polish notation (RPN) calculator in C with functions `rpn_init`, `rpn_push`, `rpn_add`, `rpn_negate`, `rpn_multiply`, `rpn_pop`, `rpn_error`. The way an RPN calculator works is as follows.

   - The `init` method creates a new stack (allocated using `calloc` and pointed to by
     a static variable declared in `solutions.c`). If the `init` method has been called
     already, then it frees the old stack and creates a new stack. It should also clear errors.
   - The `rpn_push` method pushes its argument onto the stack.
   - The `rpn_negate` method negates the value on the top of the stack.
   - The `rpn_add` method pops the top two values off the top of the stack and pushes their product onto the stack.
   - The `rpn_multiply` method pops the top two values off of the stack and pushes their product onto the stack.
   - The `rpn_pop` method pops the top value off the stack and returns it. 
   - The `rpn_error` method should return an enum value, either
   ```c
   OK, POP_ERROR, NEGATE_ERROR, MULT_ERROR, ADD_ERROR, or OVERFLOW_ERROR
   ```
    if there have been errors since the last call to `init`. Errors include trying to `pop` or `negate` an empty stack, trying to apply `add` or `multiply` to a stack with fewer than two values on it, or having the result of a computation be greater than the maximum value a `double` can hold. `pop` should return 0 when the calculator is in an error mode.

   An example test that should pass is
   ```c
   #include <float.h> /* includes DBL_MAX */

   TEST(HW2,RPN) {
     rpn_init();
     rpn_push(0.5);
     rpn_push(2.0);
     rpn_push(1.0);
     rpn_add();
     rpn_multiply();
     rpn_negate();
     ASSERT_EQUAL(rpn_pop(),-1.5);
     ASSERT_EQUAL(rpn_error(), OK);
     ASSERT_EQUAL(rpn_pop(), 0);
     ASSERT_EQUAL(rpn_error(), POP_ERROR);
     rpn_init();
     ASSERT_EQUAL(rpn_error(), OK);
     rpn_push(DBL_MAX);
     rpn_push(DBL_MAX);
     ASSERT_EQUAL(rpn_add(),0);
     ASSERT_EQUAL(rpn_error(), OVERFLOW_ERROR);
   }
   ```
   Note that you will not know ahead of time how much space to reserve for your stack, so you should:

   - Choose an initial size, say 100.
   - If the user is about to exceed in a call to `push`, then you should use the `realloc` method to reallocate your array. See [here](https://www.tutorialspoint.com/c_standard_library/c_function_realloc.htm) for details about how to use this function.

