Week 4: Introduction to C++
===

Review
===
Last time we covered
- abstract data types
- constructors and destructors
- naming conventions
- hiding memory allocation from the user
- getters, setters
- the DRY principle
- common array operations like push, pop, and map
- a (clunky) way)to make arrays of any type
Today we will see how these ideas map onto C++, and how C++ makes it easier, while simultaneously creating some possibly confusing sophisticiation.

Difficulties Scaling C projects
===

- Concern for naming collisions leads to very long function names 
- Cannot associate methods with the objects they act upon         
- Difficult to re-use or build upon code                          
- Hard to create arbitrarily typed objects and functions           
- Hard to make your API look good                                  
- Abstract data types are dealt with differently than built in types 

History
===
See the Powerpoint slides in this same directory.

Some New C++ Types and Modifiers
===

References
---

Essentially an alias for a variable, references allow you to refer to the same object with a different name. Somewhat confusingly, references are declared using the `&` operation, which in C (and therefore also in C++) is the *dereference* operator. Here's an example declaration of a reference:
```c++
int x = 1;
int &y = x;
```
Note that we are defining `y` to refer to `x`. You can't just do
```c++
int &z
```
because you are not saying what `z` refers to. 

You can now do anything with `y` that you can with `x`. For example,
```c++
x = 3;
y == x; // true
y = 4;
x == y; // true
```

References are most useful in functions to make an argument *call by reference* instead of *call by value*. For example, the method `magnitude` defined below takes a const reference to a point. 
```c++
typedef struct {
    double x, y, z;
} Point;

double magnitude(const Point &p) {
    return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
}
```
You can use this method without caring that it is call by reference, as in:
```c++
Point p = { 0,3,4 };
ASSERT_DOUBLE_EQ(magnitude(p), 5);
```
The benefit speed. If you call by value, then the entire point structure, with its three doubles, is copied to call the function. With call by reference, only a reference is created, which is a much smaller object. Note that almost every modern scripted language, such as Python and Javascript, use call by reference for arrays, hashes, and other objects, because these objects can get quite large and copying them every time you call a function is innefficient.

A function can also return a reference. For example, suppose you want to compare the magnitudes of two points and return the one with a smaller magnitude. Copying all those points is a waste of time and space, so we use references instead. For example, if `smaller` is defined as
```c++
Point &smaller(Point &p, Point &q) {
    if ( magnitude(p) <= magnitude(q) ) {
        return p;
    } else {
        return q;
    }
}
```
then you can write
```c++
Point &r = smaller(p,q);
```
after which `r` will refer to the smaller of the two points. Let's say `p` is smaller. If you then do
```c++
r.x = -1;
```
you will also be setting `p.x`.

**WARNING:** Do not return a reference to a locally declared variable. Local variables are deleted from memory when the function completes, so you would be returning a reference to nothing. For example:
```c++
Point &f() {
    Point p = {1,2,3};
    Point &q = p;
    return q;
}
```
will compile just fine, but is meaningless to the point of dangerous.

Auto Declarations
---

Many languages allow you to declare variables without specifically saying what type they are. Somewhat late to the game, C++ added this feature in the 2011 specification. For example, if you have a function declared as follows, 
```c++
Point * create_point();
```
you can use auto to avoid clutter in your code:
```c++
auto p = create_point();
```

Namespaces
===

In C++ the notion of a namespace was introduced to avoid cluttering the global namespace with lots of function and type definitions. One of the main namespaces we will use is `std`, which is the namespace the standard library uses. For example, the C++ method `cout`, which is used for printing (instead of `printf`) is a method declared in the `std` namespace. To use it, you have to do use the `::` operator, which accesses the namespace.
```c++
#include <iostream>

// later ...
std::cout << "Hello world\n";
```

This may look just like added `std_` to the beginning of everything, but you can open a namespace to use its methods for the duration of a function, as in
```c++
void print_stuff() {
    using std;
    cout << "First line\n";
    cout << "Second line\n";
}
```

cout
===
By the way, `cout` is an *output stream* that sends a stream of charaters to the terminal (other streams can do to files, for example). The `<<` operator allows you to join a number of objects together to make a strea. For example,
```c++
std::cout "The value of x is " << x << "\n";
```
Think of `<<` like a function that converts what comes after it to a string. Thus `<<(x)` should be a string representation of whatever `x` is. the `<iostream>` library defines `<<` for all the standard types, like `int` and `double`. If you create a new type, however, you have to define `<<` for it. Later in README we will show how to define `<<` for our array ADT.

Exception Handlers
===

One of the big difficulties in C is what to do when some exception occurs at run time. Usually, developers handle this by either returning a special value, such as NULL or -1, from a function in which an error occurred, or by setting up an error data structure inside an ADT to keep track of errors (like we did with the rpn calculator). In C++, however, you can define exception handlers explicitly, as well as either choose to ignore exceptions (so they are passed to the function that calls you function), or handle them explicitly. Here is an example of how to throw an exception
```c++
#include <stdexcept>

double invert(int x, int y) {
    if ( x == 0 ) {
        throw std::invalid_argument("First argument cannot be zero");
    } else {
        return y/x;
    }
}
```
This code throws an exception in some cases. The particular exception it throws has a type `std::invalid_argument`, which is an ADT defined in the `<stdexcept>` library. There are several other exceptions in the library as well, which you can read about [here](http://www.cplusplus.com/reference/stdexcept/). We will discuss defining new types of exceptions later in the class. To catch this exception, you would do something like:
```c++
try {
    invert(0,1);
    ASSERT_EQ(true,false);
} catch (std::invalid_argument e ) {
    ASSERT_STREQ(e.what(), "First argument cannot be zero");
}
```
Note that if you do not catch an exception in a given function, it is passed up to the function that called it, all the way up to the operating system. For example, suppose you had another function
```c++
void show(x,y) {
    std::cout << invert(x,y) << "\n";
}
```
If whatever function calls `show` sends in a zero for the first argument, then `invert` will throw an exception. Since this exception is not handled in `show` it will be passed up the chain to whatever called show. Thus, you can do:
```c++
try {
    show(0,1);
} catch (std::invalid_argument e ) {
   // do something here
}
```
And if you do not look for any exceptions, you will end up with a runtime error.

Conveniently, Google Test will catch all exceptions and the tests that produced them as failures, print out the exception's `what` method, and proceed without crashing the whole program.

Memory Allocation in C++
===

Memory allocation in C++ is handled by the language, and not by a library of methods as with `malloc`. Two operators, `new` and `delete` handle everything. For example:
```c++
int * x = new int;
double * p = new double[10];

// later ...
delete x;
delete[] p;
```
Note that unlike `malloc`, `new` returns a pointer to a specific type of object. In the first case an `int` and in the second case an array of `doubles`. You do not need to cast anything. The benefit is that C++ will do some type checking for you and report compile time errors that C could not. Also note that when you delete an object, you have to use the corresponding form. To delete a single object, use `delete` and to delete an array, use `delete[]`. 

Classes and Objects
===

C++ solves the problem of having objects own their methods with classes. A `class` is a description of an abstract data type's data structures and methods all within one `struct`-like entity. As an example of a class, we can write the array class we have been working on as follows:

```c++
class DoubleArray {

public:

    // Constructors
    DoubleArray();
    DoubleArray(double a, double b, double step);     // replaces the _range method
    DoubleArray(const DoubleArray& other);            // replaces the _copy method

    // Assignment operator
    DoubleArray& operator=(const DoubleArray& other); // new! lets you do array2 = array1;

    // Destructor
    ~DoubleArray();                        // Replaces destroy, deleting the buffer
                                           // and also deletes the array object itself.

    // Getters
    double get(int index) const;           // Notice all our methods now do not
    int size() const;                      // take a pointer to the array as the
                                           // first object
    // Setters
    void set(int index, double value);

private:

    int capacity,                          // Our data goes here. It is private, meaning
        origin,                            // the user cannot access it. 
        end;

    double * buffer;

    const int INITIAL_CAPACITY = 10;       // Instead of a global #define, we use an class
                                           // defined constant, that is only available within
                                           // the class

    int index_to_offset(int index) const;  // No more static methods! These will be available
    int offset_to_index(int offset) const; // to the class, but not the user.
    bool out_of_buffer(int offset) const;
    void extend_buffer(void);

};
```
The above essentially defines the interface to our new class. It says what methods are available and what arguments they take. It also defines some private data that only the class methods will have access to. 

To use the `DoubleArray` class, we would do something like:
```c++
TEST(DoubleArray, Construction) {
    DoubleArray a,             // use the default constructor
                b(0,1,0.1);    // use the range constructor
    a.set(4, 1);               // use the dot operator to access the `set` method
    ASSERT_EQ(b.get(2), 0.2);  // use the dot operator to access the `get` method
}
```
Notice that we do not have to explicitly delete `a` and `b` when we are done with them. This is because when they go out of scope, C++ will automatically call the method `~DoubleArray`, which is specially named with the `~` to let C++ know to call it whenever it needs to delete a `DoubleArray`. 

On the other hand, we still need to use `delete` if we happened to use `new` to allocate a `DoubleArray`. For example,
```c++
auto a = new DoubleArray();
auto b = new DooubleArray(0,1,0.25);

// later ...
delete a, b;
```

Note that when declaring pointers to objects, you can write
```c++
auto c = new DoubleArray; // Compiles, but does not call the constructor! Bad!
```
in which case you get an object of type `DoubleArray` for which the constructor has not been called. Since in our case, the constructor allocated memory, the result of the above code is a `DoubleArray` in which the buffer is uninitialized. Which is bad!

Constructors
---

The constructor in a class is responsible for
- initializing variables
- allocating memory (if needed)
If the objects constructed do not need memory allocation, then C++ provides a default constructor, so you do not need to define one. For example, if you defined class as
```c++
class Imaginary {
    private:
      double re, im;
};
then you can simple make a new Imaginary number with
```c++
Imaginary x;
```
You can add constructors that do different initializations. For example, the code
```c++
class Imaginary {
    public:
      Imaginary(double a, double b) : re(a), im(b) {};
      Imaginary(double a) : re(a), im(b) {};
    private:
      double re, im;
};
```
defines two new constructors. They the C++ initialization syntax to initialize the `re` and `im` fields. The first is equivalent to
```c++
Imaginary(double a, double b) {
    re = a;
    im = b;
}
```
so there is nothing magical about the syntax. Notice that the two constructors have the same name. C++ can tell the difference between them by what arguments are passed. In fact, there is a third constructor, the default constructor, which takes no arguments (as before). Thus, you can write:
```c++
  Imaginary w, x(), y(1,2), z(3);
```
and C++ will know which constructor to use. 

If memory needs to be allocated, you must at least override the default constructor (the one that takes no arguments). In the `DoubleArray` class we defined the constructor:
```c++
DoubleArray();
```
which we implement in `dynamic_array.cc` with
```c++
DoubleArray::DoubleArray() {                     // Note the namespace :: operator
    buffer = new double[INITIAL_CAPACITY]();     // We use new instead of calloc
    capacity = INITIAL_CAPACITY;                 // Note that we can refer to member variables
    origin = capacity / 2;                       // like buffer, capacity, origin, and end
    end = origin;                                // implicitely.
}
```
Note that the contructor does not explictly return a value. In fact, it is called after an object is created either in a variable declaration or when using `new`.

The other constructors can build on this code, without having to do everything again, by using the initialization syntax to initialize the object being constructed using the default constructor (the one with no arguments), and then adding more to the object in the body of the function.
```c++
DoubleArray::DoubleArray(double a, double b, double step) : DoubleArray() {
    int i = 0;
    for ( double x=a; x<=b; x += step ) {
        set(i, x);
        i+=1;
    }
}
```

The Copy Constructor and Assignment
---

To support syntax like the following
```c++
  DoubleArray x(0,1,0.1), y;
  y = x;
```
C++ needs to know what it means to assign one `DoubleArray` to have the value of another. What we want `=` to mean is
- delete the buffer in y
- allocate a new buffer in y
- copy the contents of the buffer in x to the buffer in y
- initialize origin, end, etc.
Here's how to say that in C++:
```c++
DoubleArray& DoubleArray::operator=(const DoubleArray& other) {
    if ( this != &other) {
        delete[] buffer; // don't forget this or you'll get a memory leak!
        buffer = new double[other.capacity]();
        capacity = other.capacity;
        origin = other.origin;
        end = origin;
        for ( int i=0; i<other.size(); i++) {
            set(i,other.get(i));
        }
    }
    return *this;
}
```
Think of this code as defining a method called `=` that would be called with `set y to be =(x)` (psuedocode). 
This method uses the keyword `this`, which refers to the object being operated on. In our C array class, we passed a pointer (usually called `da`) to the object as the first argument to every function. In C++ the pointer is implicit, but can be access if needed with `this`. In the above code, `this` refers to the assignee (the left hand side of `y=x`) and `other` refers to the object being copied. Thus, the first line checks that these two are not the same (if they are, there is nothing to do -- although you could leave this out and truely make a copy).

Also note that the method access the private members of `other`, which is okay because this is a class method. Finally, notice that the method returns a reference to the result of the copy. This is actually not needed to assign the left hand side, but is needed because an expression like `y=x` itself has a value, which should by a reference to `y`. This is so you can do things like
```c++
y = x = z;
```
which is equivalent to 
```c++
y = (x = z);
```
Returning a reference to `*this` is called *method chaining*.

Using the assignment operator, we can define another constructor, called the copy constructor, as follows:
```c++
DoubleArray::DoubleArray(const DoubleArray& other) : DoubleArray() {
    *this = other;
}
```
It is quite common for these two methods to be intertwined thusly.

The Destructor
---

The desctructor is responsible for deallocating memory. If none of your constructors allocate memory, then *you do not need a destructor*. However, if they do, then you have to have one. For ``DoubleArray` we just write
```c++
DoubleArray::~DoubleArray() {
    delete[] buffer;
}
```
Now, whenever a `DoubleArray` goes out of scope, its destructor is called. It is also called whenever `delete` is called on a `DoubleArray` made with `new`. 

Getters and Setters
---

The getters are straightforward. Notice that we can throw an exception instead of a runtime error when exceptions occur.
```c++
double DoubleArray::get(int index) const {
    if (index < 0) {
        throw std::range_error("Negative index in array");
    }
    if ( index >= size() ) {
        return 0;
    } else {
        return buffer[index_to_offset(index)];
    }
}

void DoubleArray::set(int index, double value) {
    if (index < 0) {
        throw std::range_error("Negative index in array");
    }
    while ( out_of_buffer(index_to_offset(index) ) ) {
        extend_buffer();
    }
    buffer[index_to_offset(index)] = value;
    if ( index >= size() ) {
        end = index_to_offset(index+1);
    }
}
```

Private Methods
---

All of the private methods in `DoubleArray` are essentially the same as in our C array ADT, except they use `new` and `delete` (e.g. in `extend_buffer`). See the in this week's directory for details.

Templates
===

The `DoubleArray` class has the same issues as our C `DynamicArray` ADT did: it only contains doubles. However, C++ provides a powerful way to make new objects that refer to arbitrary types. The approach uses `templates`.

Here is an example of function definition that uses a template to compare two objects:

```c++
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
```
As long as `<` is defined for the the type `T`, then this template can be used. For example, 
```c++
int c = compare(1.0, 2.0)
```
works. However, 
```c++
Imaginary x(1,2), y(3,4);
int c = compare(x,y);
```
will give the error:
```bash
error: no match for 'operator<' (operand types are 'const {anonymous}::Imaginary' and 'const {anonymous}::Imaginary')
```
However, if you defined `<` for imaginaries as follows:
```c++
bool operator< (const Imaginary& lhs, const Imaginary& rhs) {
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
```
Then `compare` will work just fine.

To apply templates to our array class, we have to replace references to `double` with a type name:
```c++
template <typename ElementType>                      // ElementType is the type variable
class TypedArray {

public:

    TypedArray();
    TypedArray(const TypedArray& other);             

    // Copy constructor
    TypedArray& operator=(const TypedArray& other);

    // Destructor
    ~TypedArray();

    // Getters
    ElementType &get(int index);
    ElementType &safe_get(int index) const;         // This is new. See below.
    int size() const;

    // Setters
    void set(int index, ElementType value);

private:

    int capacity,
        origin,
        end;

    ElementType * buffer;   

    const int INITIAL_CAPACITY = 10;

    int index_to_offset(int index) const;
    int offset_to_index(int offset) const;
    bool out_of_buffer(int offset) const;
    void extend_buffer(void);    

};
```

With this definition of array, we can make arrays of anything. 
```c++
TypedArray<Point> b;
b.set(0, Point(1,2,3));
b.set(1, Point(2,3,4));
b.set(20, Point(3,4,5));
double r = b.get(20).x;
```
Or, for an even nicer example, using arrays of arrays of doubles, we can make a matrix:
```c++
TypedArray<TypedArray<double>> m;

for (int i=0; i<30; i++) {
    for (int j=0; j<30; j++) {
        m.get(i).set(j,0.5*i*j);
    }
}
```

Where to put Template Methods
---

Template methods need to appear in header files. This is because the compiler can only compile things like `TypedArray<int>` and not arbitrary typed arrays. Thus, any file that uses a templated class needs to include all the templated methods, and not just the class definition, because compiling it will require instantiating the template on the specific types used in the file.

Memory Allocation with Arbitrary Types
---
The main differnce between `DoubleArray` and `TypedArray` has to do with the buffer. For example, in the constructor, we can still use `new`, but we also have to make sure to call the constructor for whatever class `ElementType` stands for, which we do with with the parentheses at the end of the statement.
```c++
template <typename ElementType>
TypedArray<ElementType>::TypedArray() {
    buffer = new ElementType[INITIAL_CAPACITY](); // Note: Calles ElementType constructor
    capacity = INITIAL_CAPACITY;    
    origin = capacity / 2;
    end = origin;    
}
```
The same line shows up in the assignment operator, and a similar line shows up in the private method `extend_buffer`. 

Default Values
---

Another issue is this: In the `DoubleArray` class, we used zero as the default value when the user asked for an element outside the size of the array. To do that here, we need a "zero" for any object. One idea is to use the default constructor to create a new element to return in such cases, and also to extend pad array with default values all the way up to the newly accessed index:
```c++
template <typename ElementType>
ElementType &TypedArray<ElementType>::get(int index) {
    if (index < 0) {
        throw std::range_error("Out of range index in array");
    }
    if ( index >= size() ) {
        ElementType x;    // A temporary value to use with set.
        set(index, x);    // `set` will create a buffer of default values.
    } 
    return buffer[index_to_offset(index)];
}
```

Get Returns a Reference
---

For the line
```c++
m.get(i).set(j,0.5*i*j);
```
to work, the call to `get` cannot copy the array at index `i` in `m`, because then we would be chaning a copy of the array when we called `set` after that. Thus, `get` should return a reference. Also, note that because `get` might actually change the array, the array iteself is not guaranteed to be the same after the get. However, we may have other methods that expect getting an element at a particular index to leave the array unchanged, for example, when copying the array. Thus, we add a new method called `safe_get`:
```c++
template <typename ElementType>
ElementType &TypedArray<ElementType>::safe_get(int index) const {
    if (index < 0 || index >= size() ) {
        throw std::range_error("Out of range index in array");
    }
    return buffer[index_to_offset(index)];
}
```
Note the extra `const` at the end of the declaration. A `const` in that position refers to `this`, that is, the object that owns the method. Since the object itself is not an argument (it is only so implicitly), C++ uses this syntax to require the method to not change the underlying array data. This is required when copying an array using the assignment operator, because the argument `other` is specified to be a const.
```c++
template <typename ElementType>
TypedArray<ElementType>& TypedArray<ElementType>::operator=(const TypedArray<ElementType>& other) {
    if ( this != &other) {
        delete[] buffer; // don't forget this or you'll get a memory leak!
        buffer = new ElementType[other.capacity]();
        capacity = other.capacity;
        origin = other.origin;
        end = origin;
        for ( int i=0; i<other.size(); i++) {
            set(i,other.safe_get(i));
        }
    }
    return *this;
}
```
In the above method, if we replace `safe_get` with just `get` we get the (extremely helpful) compile time error:
```bash
error: passing 'const TypedArray<double>' as 'this' argument of 'ElementType& TypedArray<ElementType>::get(int) [with ElementType = double]' discards qualifiers [-fpermissive]
```
The term "discards qualifiers" refers to the `const` after the method declaration. The last bit is a suggestion that you compile with the `g++` flag `-fpermissive` which will let you let away with such things. However, that is a bad idea. Instead, you should try to understand what is happening and come up with a solution like we did with `safe_get`. 

Interfacing with Streams
---
To print arrays, we can overload the `<<` operator:
```c++
template <typename ElementType>
std::ostream &operator<<(std::ostream &os, TypedArray<ElementType> &array)
{
    os << '[';
    for (int i=0; i<array.size(); i++ ) {
        os << array.get(i);
        if ( i < array.size() - 1 ) {
            os << ",";
        }
    }
    os << ']';
    return os;
}
```
This method is not declared inside the class definition, since it is not owned by `TypedArray`. In any case, now we can write:
```c++
TypedArray<TypedArray<double>> m;

for (int i=0; i<3; i++) {
    for (int j=0; j<3; j++) {
        m.get(i).set(j,0.5*i*j);
    }
}

std::cout << m << "\n"; 
```
which yields
```bash
[[0,0,0],[0,0.5,1],[0,1,2]]
```
The `<<` operator is called nine times. Three times with `TypedArra<double>` arguments, and for each of those times, three more times for `double` arguments. Since the `iostream` library defines `<<` for doubles, we do not need to do that. If we had an array of array of Imaginary numbers, for example, we would need to define `<<` for them as well.

Summary
===

This week, we learned to take an abstract data type written in C and rewrite it in C++. We learned to use the C++ notions of references, namespaces, exception handlers, overloading functions and operators, classes, constructors, destructors, and templates. 

C++ has many other additions beyond what we have covered here. For example, a class can inherit all the methods of another class, but also add more methods. Classes can have "friends". You can overload all the operators. There is also an extensive "standard template library" which we will introduce next week. We will touch on a few other aspects of C++, in future lectures, but the subset of C++ we have defined here is fairly complete. 

It is important to understand that C++ is a huge, messy language with many ways to do the same thing. Some very bad programs have been written in C++ (and in pretty much ever other language). This document is meant to introduce C++ but also to introduce conventions. For example, every class you write that does memory allocation should have a default constructor, a copy constructor, an assignment operator, and a destructor. As you delve deeper into C++, study good code examples and try to use their conventions. Although it is easy to make up a new one, doing so will confuse your collaborators and possibly even you.

A good book on C++ is the "C++ Primer" by Stanley Lippman. Consider reading as much of this book as you can, especially if you expect to be writing C++ programs for your job. There is a lot more covered there than is covered in that class.

Exercises (DRAFT)
===
1. Port the imaginary ADT you wrote to C++. Overload the `*` and `+` operators for your class.
1. Port the fraction ADT you wrote to C++. Overload the `*` and `+` operators for your class.
1. Create a `TypedMatrix` that uses `TypedArray` for its internal data. Since `TypedArray` does its own memory allocation, your class should not have to do so. Write the following methods
- A default constructor that makes a 1x1 matrix.
- A constructor that makes an mxn matrix.
- A method `get(i,j)` that returns the element in the ith row and jth column.
- A method `set(i,j,value)` that sets the element in the ith row and jth column.
- A method `add(const TypedArray& M)` that adds the array `M` to `this` and returns a new array.
- A method `multiply(const TypedArray& M)` that multiplies `this` and `M`, returning a new array.
- Overload `<<` so you can print out the matrix nicely, with newlines after each row.
All of your methods should throw exceptions if indices are out of range, or if `add` and/or `multiply` are aksed to operate on arrays of incompatible sizes. Test your method on `TypedMatrix<double>`, `TypedMatrix<Imaginary>`, and `TypedMatrix<Fraction>`. 
1. One other problem, probably involving memory allocation, TBA.