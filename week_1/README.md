Installing Git
===

Mac
---

Windows
---

Installing Docker
===

Mac
---
Download Docker.dmg and double click on it. Once the Docker icon shows up in your status bar, you can open a terminal and start using docker.

Windows
---
- Download and run installer
- 

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

Compiling and Running C Code
===

To compile the hello world example, do the following from within in the cppenv bash shell:
```bash
cd hello_c
gcc -c hello.c
gcc -c main.c
gcc -o hello *.o
./hello
```

To build the documentation for the examle, use doxygen:

```bash
doxygen -g
doxygen Doxyfile
```

The first line generates a configuration file called Doxyfile and the second builds the documentation. 
You can view the documentation by opening html/index.html in your web browser. 

To clean up, do
```bash
rm -rf html latex Doxyfile doxygen_sqlite3.db *.o hello
cd ..
```

Compiling C++ Code
===

From within in the cppenv bash shell,
```bash
cd hello_cpp
g++ -c hello.c
g++ -c main.c
g++ -o hello *.o
./hello
```
You can build the documentation the same way you did for the C example above.

To clean up, do
```bash
rm -rf html latex Doxyfile doxygen_sqlite3.db *.o hello
```

Using a Makefile
===