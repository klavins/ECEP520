Week 10: Projects
===

Elma on Dockerhub
---

**New in the Docker Container**
- Now called klavins/elma:latest or klavins/elma:v1.1.0 etc.
- Includes [screen](https://linuxize.com/post/how-to-use-linux-screen/), for having multiple terminals open on the same image.
- Installs the Elma library:
    - `/usr/local/src/elma` - The source and build diretory
    - `/usr/local/include/elma` - Header files
    - `/usr/local/lib/libelma.a` - The shared library
    See the Example Project for details on how to use these resources.

**Continuous Integration**
- Dockerbug build configured to build latest docker image upon every push and every pull request

Changes to Elma
---
- Pulled priority upgrade. Edited tests.
- Multiple manager runners
    ```c++
    // Run for a certain amount of time
    Manager& run(high_resolution_clock::duration runtime);

    // Run indefinitely or until halt() (TODO: needs C interrupt handler)
    Manager& run();

    // Run until a condition becomes true
    Manager& run(std::function<bool()> condition);
    ```
    The last method was required to get some tests to pass. For example, the priority methods ran for 100 ms, but on Dockerhub, not much happens in 100ms.
- A new halt() method in processes, which tells the manager to stop, calling all processes' stop methods.
- A flag that should recognize Raspberry pi v3 B in the Makefile.
- A method in `State` for getting a pointer to the containing state machine.

Raspberry Pi
---
Installation instructions for the pi (and any other UNIX) no included in Elma README.

Running Multiple Processes
---

- Important: The following will not work (unless you do all sorts of port mapping):
      - Do `docker run ...` in a terminal
      - Start a server
      - Do `docker run ...` in another terminal
      - Try to talk to the server in the first terminal

    This is because you are running two completely separate virtual machines. 

- When you run a process, like the `temperature_server` it doesn't stop, which ties up the console. You can run it in the background with
    ```c++
    examples/bin/temperature_server &
    ```
    Then you can see it in your process list by typing `ps waux` which returns a list of all processes running in the container, something like
    ```
    USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
    root         1  0.0  0.1  20248  3192 pts/0    Ss   18:51   0:00 bash
    root        51  0.1  0.4 21474911772 9460 pts/0 S   18:53   0:00 ./examples/bin/temperature_server
    root        52  0.0  0.1  17500  2084 pts/0    R+   18:53   0:00 ps waux
    ```
    or by typing `jobs` which returns a list of the jobs you have started from this particular console
    ```
    [1]+  Running                 ./examples/bin/temperature_server &
    ```
    To stop the temperature server, you can use either the process id or the job id. 
    ```bash
    kill 51
    ```
    or
    ```bash
    kill %1
    ```
    Note that if you start a process in the forground (without the &), then you can type `ctrl-Z`, which pauses it. Then you can type `bg` which puts it in the background.

- Sometimes, your server might need to log information to standard output. In this case, running it in the background will work, but all of its output will get in the way of what you are doing. You can do two things.
    - Pipe its output to a file
        ```bash
        ./examples/bin/temperature_server > /tmp/output &
        ```
    - Use the [screen](https://linuxize.com/post/how-to-use-linux-screen/) command to open multiple subterminals. For example:
        ```bash
        screen
        [return]
        [Ctrl-a c]
        ps waux
        [Ctrl-a 0]
        [Ctrl-a ?]
        ```

Example Project
---

**If you are extending Elma**:
- Then fork the [elma](https://github.com/klavinslab/elma) repo. 
- If you have minor fixes to Elma, make a pull request.
- You may be invited to make a pull request for your entire project after grading is done.

**If you are building an extended example**: 
- The start with the [elma_project](https://github.com/klavinslab/elma_project) repo.

Note, the stopwatch uses [ncurses](http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/) for its user interface.

Review the code in the elma project for details.

What to do Next Year
---
- Memory allocation *after* STL
- Skip arbitrary array class (teach templates some other way)
- Start with Elma sooner
- Review similar event loop managers / schedulers
- More extended examples
- More emphasis on testing / debugging
- Deeper dive into networking
- Raspberry pi GPIO drivers
- Team projects?
- What else?