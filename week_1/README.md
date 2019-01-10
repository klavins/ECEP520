Installing Git
===

Mac
---
- Install the Xcode Command Line Tools.

Windows
---
- Go to git-scm.com/download/win

Installing Docker
===

Mac
---
- Go to https://www.docker.com/

Download `Docker.dmg` and double click on it. Once the Docker icon shows up in your status bar, you can open a terminal and start using docker.

Windows 10 Professional or Enterprise
---
- Go to https://www.docker.com/
- Download and run installer

Windows 10 Home and possibly Windows 8
---
- Go to https://docs.docker.com/toolbox/toolbox_install_windows/
- Download and run installer
- Run Docker Quickstart

Getting the code for week 1
===
```bash
git clone https://github.com/klavins/ECEP520.git
```

Building and starting the Docker Image
===
```bash
cd ECEP520
docker build -t cppenv cppenv
docker run -v $PWD/week_1:/source -it cppenv bash
```
The build can take about 5 minutes depending on how fast your connection and machine are. Alternatively, you can skip the build step and run the `cppenv` container created with the `Dockerfile` in this repo via Prof. Klavins' Dockerhub account with
```bash
docker run -v $PWD/week_1:/source -it klavins/ecep520:cppenv bash
```
On windows you will have to use the full path, as in
```bash
docker run -v /c/Users/You/ECEP520/week_1:/source -it klavins/ecep520:cppenv bash
```

Compiling and Running C Code
===

To compile the hello world example, do the following from within in the `cppenv` bash shell:
```bash
cd hello_c
gcc -c hello.c
gcc -c main.c
gcc -o hello *.o
./hello
```

To build the documentation for the example, use `doxygen`:

```bash
doxygen -g
doxygen Doxyfile
```

The first line generates a configuration file called `Doxyfile` and the second builds the documentation. 
You can view the documentation by opening `html/index.html` in your web browser. 

To clean up, do
```bash
rm -rf html latex Doxyfile doxygen_sqlite3.db *.o hello
cd ..
```

Compiling C++ Code
===

From within in the `cppenv` bash shell,
```bash
cd hello_cpp
g++ -c hello.cc
g++ -c main.cc
g++ -o hello *.o
./hello
```
You can build the documentation the same way you did for the C example above.

To clean up, do
```bash
rm -rf html latex Doxyfile doxygen_sqlite3.db *.o hello
```

Compiling Tests and Linking Libraries
===

In the fractions directory is a simple Fraction api and some unit tests for it. These tests use [Google Test](https://github.com/google/googletest), a library that the `cppenv` Docker container already has installed in it. To build the executable for the fractions tests, do
```bash
cd fractions
g++ -c fraction.c -std=c++11
g++ -c unit_tests.c -std=c++11
g++ -c main.c -std=c++11
g++ -o test *.o -lgtest -lpthread
```
Note that although the Fraction api is written in C, the test library is written in C++, so we use the `g++` compiler and linker to build the code. Also, Google Test uses the C++11 standard, which is not the default for C++, so we have to tell g++ top use it. Finally, the linker needs to know to link the shared object library for Google Test, called `gtest`, and also the PThread library, called `pthread`, which Google Test users.

To run the tests in `unit_tests.c`, simply do
```bash
test
```

Using a Makefile
===
Having to type `g++` commands over and over is tedious. To manage the build process, we will use [make](https://www.gnu.org/software/make/). The fractions directory has a Makefile in it with generic definitions for making a new project. To run make, just do
```bash
make
```
from within the fractions directory. This will make two new directories, bin and build. The former will have the executable, test, in it. The latter will have all the `*.o` files. Then it builds and links everything. The Makefile also build the documentation. 

To just build the code with no documentation, you can do
```bash
make exec
```

You can clean up by doing
```bash
make clean
```
or
```bash
make spotless
```
to really clean up.

Exercises (Due Friday 18 Jan at 11:59pm)
===

1. If you are new to Linux, go through a tutorial, such as the one at [linuxcommand.org](http://linuxcommand.org/). Do the "Learning the Shell" part. Don't worry about writing scripts.

1. Make a new directory in your ECE590 Github repo called `hw_1`. Copy the contents of the `fractions` directory from `week_1` to that directory. This will be the starting point of these exercises. 

1. Note: To avoid committing compiled objects to github, you should make the following .gitignore file in the root of your repo (i.e. in ECE590):
    ```
    bin/
    build/
    html/
    latex/
    docs.config
    doxygen_sqlite3.db
    ```
    If you make any changes to docs.config, you might not want to ignore that particular file. But for now, we are not using any of its details.

1. Add a function to the fractions example called `reduce`. It should take one Fraction argument and return a Fraction in lowest terms. Add tests into unit_test.c of the following form:
    ```c
    Fraction a = (Fraction) { 21, 33 };
    EXPECT_EQ(reduce(a).num,7);
    EXPECT_EQ(reduce(a).den,11);
    ```
   We will test your code with our own tests and try many different situations (multiple common factors, negative numbers, zero in the numerator, etc.). Be sure to use the exact same name `reduce` for your function.

1. Make a new header file called `imaginary.h`. Similar to the fraction.h file, define a structure with a two members, `real` and `im` which should have type `double`. Add function prototypes for `add`, `negate`, `conjugate`, `multiply`, and `magnitude`. Be sure to document them using Doxygen. Functions that return a scalar should return `double`.

    Next, make a source file called `imaginary.c` and put the definitions of the functions into it. For the `magnitude` function you will need a square root function, which can be obtained by adding the [C math library](http://www.cplusplus.com/reference/cmath/) as follows:

    ```c
    #include <math.h>
    ```

    at the top of your `imaginary.c` file. The function itself is called `sqrt` and takes one argument.
    
    Write tests for each of your functions. 

    Note that you will need to edit the Makefile to include the header and source files and edit main.c and unit_test.c to include your new header file.

    Remember, we will compile your code against our own tests, so be sure to name your functions and struct members exactly as specified here and to test edges cases.
