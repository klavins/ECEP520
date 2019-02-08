Week 5: The Standard Template Library
===

So far in the course we have covered the basics of C and C++, and focused in particular on writing abstract data types and classes that encapsulate data structures and the algorithms that act upon them. In particular, last week we introduced C++ data types, classes, and templates. 

This week, we introduce the C++ Standard Template Library (STL). It contains, among other things, a `vector` template that is just like our `TypedArray`, but scaled and optimized for production purposes. The library includes varitions on templated, dynamic, sequential containers such as queues, lists, and arrays. It also includes maps, sets, and a set of generic algoprithms you can use to operate on these objects. Most C++ programs make heavy use of these templates and algorithms, avoiding dynamic memory management as much possible. 

Next week, we will begin to focus on the types and classes needed to represent common tasks in embedded systems. We will introduce classes for sensors, effectors, processes, a process manager, and a scheduler. Together, these classes will allow us to write complex, event driven, reactive systems that are at the heart of embedded systems code, robotics control systems, and similar systems. 

Sequential Containers
===

A *conainer class* is one that contains some other class. Our `TypedArray` class is an example. A *sequential* class is one in which the elements are stored and access sequentially, with indices 1, 2, ... and so on. The STL includes several templated sequential containers:
- `vector`: Like our `TypedArray` class, except you can't `get` elements at an index until something has been put at that index.
- `string`: A vector of characters with some special operations for manipulating strings.
- `array`: A fixed-size array whose size must be declared at compile time. No dynamic memory allocation, so possibly faster.
- `deque`: A double ended queue. Like a `vector` but optimized for adding elements to both ends (like our `TypedArray`).
- `list`: A doubly linked list.
- `forward_list`: A singly linked list.
The most commonly used containers are `vector` and `string`, which we will cover here. The rest of the containers have fairly specialized uses, and many guides and textbooks advise that you simply don't use them.

In the following notes, we will use `vector`s and `string`s for all of our examples. Many of the methods that act on these objects also act on the other ones. 

Constructors
---

To use the `vector` container, you need to first include it:
```c++
#include <vector>
using std::vector;
```
To make a new `vector`, you use the template sytnax we've already learned. For example
```c++
vector<double> t;          // vector of doubles
vector<Imaginary> u;       // vector of Imaginaries
vector<vector<double>> v;  // essentially a matrix of doubles
```
You can also initialize a `vector` when constructing it.
```c++
vector<int> t = { 1, 2, 3 };  // vector of three ints
vector<double> u(10,3.14);    // vector of 10 3.14s
vector<double> v = u;         // copy constructor with assignment operator
```

In the STL, `string`s are a bit more than just pointers to `char`s as with C. A string is a first class objects that is quite similar to a `vector`. Strings can be constructed and initialized similarly.
```c++
string s;
string t = "hello";
string u = t;
```
Note that the following code
```c++
vector<string> v = { "embedded", "systems", "are", "cool" };
```
involves the constructor for the `string` class, which takes a `char *` aw an argument. Thus, although it may look like this snippet makes an array of `char *` values, it actually builds a vector of strings.

Vectors and Strings have many methods in common, since they are based on essentially the same code. You can test if they are empty, find their sizes, compare them, and get their individual elements. So for vectors, you can write:
```c++
vector<int> v = { 1, 2, 3 };
v.push_back(4);
v[0];      // 1
v.back();  // 3
v.front(); // 1
v.empty(); // false
v.size();  // 4
v == v;    // true
v > v;     // false
v.pop_back();   // changes the array to { 1, 2 } (returns void)
```
and for strings, you can write
```c++
vector<int> s = "abc";
s.push_back('d');
s[0];      // 'a'
s.back();  // 'c'
s.front(); // 'a'
s.empty(); // false
s.size();  // 4
s == s;    // true
s > s;     // false
v.pop_back();   // changes the array to { 1, 2 } (returns void)
```

The STL also defines the methods
```c++
s.clear();                    // clears the string
s.insert(s.begin()+1, 'x');   // insert an 'x' into position 1
q.erase(5,2);                 // remove two elements starting at position 5, reindexing subsequent elements
```

One method that can be useful is called `emplace`. It is use to construct new elements of the base type of, for example, a `vector`. This is needed because construct elements and then pushing them into the vector can be cumbersome. Thus, if you have a class `Imaginary` with a constructor that takes two arguments, the real part and the imaginary part, you can do
```c++
vector<Imaginarey> r;
r.emplace(r.begin(),2.0,3.0);
```
which creates a vector whose first element is the imaginary numer `2+3i`.

A new for loop form is available in C++ as well, allowing you to interate over the elements in a sequential container easily. FOr example, to iterate over the characters in a string, you can do:
```c++
for ( char c : s ) {
    cout << c << "\n";
}
```
Or for a vector, you can write
```c++
vector<string> v = { "i", "am", "on", "a", "very", "bumpy", "flight" };
for (string s : v ) {
    cout << s << "!\n";
}
```
Note that the `for` pattern can request either a copy of the element, or a reference to it. You might want a copy if you are going to change the elements as you iterate through them, as in the following code:
```c++
vector<string> v = { "i", "am", "on", "a", "very", "bumpy", "flight" };
for (string &s : v ) {
    s.push_back('!');
}
```

Because strings are made of characters, it is often helpful to know the methods that are availale for manipulating them.
```c++
int isalnum(int c);
int isalpha(int c);
int isblank(int c);  
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);
```
Their names should be self-explanitory. These are in the library `<cctype>`. 

For example, you can use the `toupper` method to convert a string to all caps:
```c++
for ( char &c : q ) {
    c = toupper(c);
}
```

The string container also defines a number of string specific methods, such as `assert`, `append`, `replace`, `substr`, and `find` whose definitions are mainly self-explanatory. There are also a number of methods for converting between strings and numbers, such as `to_string` and `atoi`. 
```

Interestingly, since vectors and strings are dynamic classes much like the `TypedArray` class we built earily, they have a capacity, which you can find with a simple method
```c++
v.capacity(); // some number bigger than v.size();
```
Also, if you need to keepo the amount of space an sequential container takes up to a minimum, you can do:
```c++
v.shrink_to_fit();
```

Iterators
---

Before the `for(x:v)` form was introduced, the way to iterate through containers like `vector`s and `string`s was to use `iterators`. For example, the following code also changes all the characters in a string to caps:
```c++
for ( string::iterator i = q.begin(); i != q.end(); i++ ) {
    *i = tolower(*i);
}  
```
Here, `q.begin()` returns an iterator that refers to the element at the beginning of the array, while `q.end()` refers to a non-element that is "one past" the end of the array. The notation `i++` is overloaded for iterators to mean "increment the iterator". Finally, the '*' operator is overloaded to return a reference to the element the iterator refers to. 

Iterators are a good place to use the `auto` keyword. For example, an iterator to a `vector` of `vector`s of `double`s would be declared and initialized by 
```c++
vector<vector<double>>::iterator i = v.begin();
```
and it is a lot easier (and clearer) to write
```c++
auto i = v.begin();
```
I suspect that usages similar to this one were the reason the `auto` keyword was introduced in the first places.

Iterators can be manipulates like indices into normal C arrays, via overloaded operators for `+`, `-`, `<`, and so on. For example
```c++
auto i = v.begin();
auto j = i + v.size() / 2;
i < j; // true
```
It should be noted that iterators are *not* numbers. For example, if you do `cout << i`, you will get a compiler error because `i` is not some kind of number for which `<<` is defined, and furthermore, `<<` is not defined for iterators.

*Notes:* If you would like to access the elements of a `vector` or `string` in reverse, you can use `v.rbegin()` and `v.rend()` methods. If a `vector` or `string` was declared `const`, you can use `v.cbegin()` and `v.cend()`, which return `const_iterators` instead of `iterators` (just use `auto` if you are unsure). 

Generic Algorithms
---

The STL uses function polymorphism to let you apply simple algorithms, such as finding and sorting, to pretty much any sequential container. One such method is called `find`. Here are a few examples:
```c++
vector<string> v = { "The", "quick", "brown", "fox" };
string s = "this is a string";
int a[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};      

auto f1 = find(v.begin(), v.end(), "quick");
cout << *f1; // "quick"

auto f2 = find(s.begin(), s.end(), 'i');
cout << *f2; // `i`

auto f3 = find(begin(a), end(a), 6);
cout << *f3; // 6
```
In the first two cases, `find` is using the `begin` and `end` methods of the `vector` and `string` containers. In the last case, we use the global methods `begin` and `end` to construct new iterators for the C style array `a`, which doesn't have any methods of its own. 

The `find` algorithm works on *any* object that defines `==`. Other algorithms, such as `sort` work on amy object that defines `<`. That's whjy they are called *generic*.

Here are a few more *read only* algorithms (ones that do not change their arguments) like find:
```c++
vector<int> v = { 1, 2, 3, 4, 5 }, 
            w = { 2,3,4,5,6,6,7,8 };
accumulate(v.begin(), v.end(), 0);    // sum of elements in v
count(v.begin(), v.end(), 3);         // number of occurences of 3 in v
equal(v.begin(), v.end(), w.begin()); // true if first v.size() elements are equal to the
                                      // first v.size() elements of w, which must have equal
                                      // or greater length
```
These work equally well on any sequential container of values for which operations like `==` and `+` are defined.

Algorithms that change the underlying array, include things like `sort` and `fill`:
```c++
sort(v.begin(), v.end());                 // sort the elements
fill(v.begin(), v.end(), 1);              // replace element with third argument
fill_n(v.begin(), 3, 1);                  // replace three elements at the beginning with 1
copy(v.begin(), v.end(), w.begin() + 3);  // copy v into w starting at position 3
replace(v.begin(), v.end(), 3,-1);        // replace element at position 3 with -1
```

Lambda Expressions
===

You can define functions as in two ways in C++. The first is the standard way, as in this example which squares its argument
```c++
double square(double x) {
    return x*x;
}
```
The other is with what are called `lambda` expressions, named after the *lambda calculus* invented by Alonzo Church in the 1930s as a way to capture thne essence of computation mathematically. A lambda expressioin defining the same function as above is:
```c++
[](double x) { return x*x; }
```
It is the same function, but it doesn't have a name. You can use evaluate lambda expressions by applying them like functions, as in:
```c++
([](double x) { return x*x; })(3.0); // 9.0
```
or you can set a variable equal to a lambda expression, and apply it later, as in
```c++
auto square = [](double x) { return x*x; }
square(3.0); // 9.0
```
This becomes useful when using generic algorithms, such as `transform`, which take functions as arguments:
```c++
vector<int> v = { 1, 2, 3, 4, 5 };
transform(
    v.begin(),    // start of elements to transform
    v.end(),      // end of elements to transform
    v.begin,      // start of where to put elements to transform
    [](double x) { return x*x; }
);
```
Note that lambda expressions do not return function pointers. Rather, they return function *objects*, which are instiations of a *function* class which overrides the `()` application operator. 

The square brackets of a lambda expression can list variables to *capture* from the surrounding scope. For example, if we define a function like this:
```c++
void add_to_all(vector<int> &v, int x) {
    transform(
        v.begin(), 
        v.end(), 
        v.begin(), 
        [](int y) { return x+y; } // WRONG!
    );
}
```
the C++ compiler will complain that the lambda expression does not have access to the variable `x` in its scope. We can fix this by *capturing* `x` as follows:
```c++
void add_to_all(vector<int> &v, int x) {
    transform(
        v.begin(), 
        v.end(), 
        v.begin(), 
        [x](int y) { return x+y; } // RIGHT!
    );
}
```

Associative Containers
===

Associative containers are different than sequential containers in that they index the container with keys, kind of like a struct, dictionary, or database. The associative containers defined in the STL are 'map', 'set', 'multimap', and 'multiset'. We will focus here on 'map', which can be used to illustrate the main features.

 In a `map`, a set of keys are used to index a set of values. For example, you might define a `map` as follows:
 ```c++
    map<string,string> person;
    m["First"] = "Alan";
    m["Last"] = "Turing";
    m["job"] = "Code breaker";
 ```
Note that the `map` template takes two types, the key type and the value type. They do not have to be the same:
```c++
    map<string, vector<int>> sequence;
    sequence["ints"] = { 1, 2, 3, 4, 5, 6 };
    sequence ["squares"] = { 1, 4, 9, 16, 25, 36 };
    sequence["fib"] = { 1, 1, 2, 3, 5, 8, 13 };    
```
or another (inefficient) way to make an array of doubles
```c++
map<unsigned int, double> a;
a[0] = 123;
a[5] = 34;
int x = a[3]; // 0. Non-existent keys map to default values of the value type
```
Some implementations of Javascript actually do this for arrays.

You can iterate through the keys and value of a `map` with iterators. The order in which the interators visit the elements of the map is not guaranteed, so do not depend on it. For example, using the definition of `m` above, 
```c++
for(auto i = m.begin(); i != m.end(); i++) {
    cout << (*i).first << ": " << (*i).second << "\n";
}
```
When you dereference an iterator in a `map` you get a `pair` object (also defined by the STL). It will have the same types as the map. In the above example, `*i` as the type `pair<string,string>`. The pair object has a `first` and `second` field that references to the actual key and value of the map to which the iterator refers.

You can erase key/value pairs from a map using either keys or iterators.
```c++
m.erase("First");
m.erase(m.begin());
```

File I/O
===

Learn to read and write files [here](http://www.cplusplus.com/reference/fstream/fstream/).

Exercises
===
1. Write a function called `sort_by_magnitude` that takes a reference to a vector of doubles and sorts it with a comparison function that says that x < y if and only if the absolute value of x is less than the absoilute value of y. For example, -5 would be great than 4 because |-5| > |4|. Use the generic algorithm `sort` (which you can look up on the c++ reference we page) and a lambda expression for your comparison function.
1. Rewrite the `TypedMatrix` class with `vector`s instead of `TypedArrays`. The interface to the user should be identical to what wS specifid in that previous homework.
1. Write a method
    ```c++
    TypedMatrix<double> read_matrix_csv(const string path);
    ```
    that reads a comma separated value (CSV) file of doubles into a matrix and returns it. If there are any errors in the format of the CVS file, or missing values (so the CSV does not represent a matrix), throw an exception. Spaces and tabs should be ok in between commas, but newlines should only be used to terminate a row of the matrix.
1. Write a method
    ```c++
    void write_matrix_csv(const TypedMatrix<double> &matrix);
    ```
    that writes a comma separated value (CSV) file of doubles from a matrix. Test that you can write and read a matrix and get the same matrix back.
1. Write a method
    ```c++
    map<string, int> occurance_map(const string path);
    ```
    that reads in an ascii text file and returns an assocation where each key is a word in the text file and each value is the number of occurances of that word. Ignore punctuation and numbers. The method should be case-insensitive and should store the keys as lowercase. A word is definited by an string consisting entirely of alpha-numeric characters or apostrophes (single quote characteris). For example, if the file contained
    ```
    This is a sentence. Don't think of wier_d strings as words. Really, 123 is a nice number. 
    ```
    you would include *sentence*, *don't*,  and *123* but not ., ,, or wier_d. Using single quotes as quotes, as in
    ```
    'I should use double quotes'
    ```
    is a user error and will catch the "words" 
    ```
    'I
    should
    use
    double
    quotes'
    ```
1. One more exercise, TBA.