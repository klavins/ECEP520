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

Download Docker.dmg and double click on it. Once the Docker icon shows up in your status bar, you can open a terminal and start using docker.

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
The build can take about 5 minutes depending on how fast your connection and machine are. Alternatively, you can skip the build step and run the cppenv container created with the Dockerfile in this repo via Prof. Klavins' Dockerhub account with
```bash
docker run -v $PWD/week_1:/source -it klavins/ecep520:cppenv bash
```
On windows you will have to use the full path, as in
```bash
docker run -v /c/Users/You/ECEP520/week_1:/source -it klavins/ecep520:cppenv bash
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