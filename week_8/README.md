Week 8
===

Projects
===

New Docker Image
===

The docker container we have been using has been upgraded to include the httplib library. You can build it from the new Dockerfile is in cppenv-http in this repository with the commands
```bash
cd cppenv-http
docker build -t cppenv-http .
cd ..
docker run -v $PWD:/source -it cppenv-http bash
```
or just use Prof. Klavins' pre-compiled image via Dockerhub with the command
```bash
docker run -v $PWD:/source -it klavins/ecep520:cppenv-http bash
```

Within this container, you can use the JSON library simply by including it.
```c++
#include <httplib/httplib.h>
```

HTTP
===

HTTPS
===

Threads
===

Mutex
===

Adding HTTP to Elma
===

Exercises
===