Week 7: Events and Finite State Machines
===

Architecture
===

JSON
===

Adding JSON to your code (and to Elma)
---

The docker container we have been using has been upgraded to include the nlohmann/json library. You can build it from the new Dockerfile is in cppenv-json in this repository with the commands
```bash
cd cppenv-json
docker build -t cppenv-json .
cd ..
docker run -v $PWD:/source -it cppenv-json bash
```
or just use Prof. Klavins' pre-compiled image via Dockerhub with the command
```bash
docker run -v $PWD:/source -it klavins/ecep520:cppenv-json bash
```

Within this container, you can use the JSON library simply by including it.
```c++
#include <json/json.h>

using nlohmann::json; 
```

What is JSON?
---

The JSON C++ Library
---

Events
===

Finite State Machines
===

States
---

Transitions
---

Example
---