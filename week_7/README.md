Week 7: Events and Finite State Machines
===

Note about projects
===

Projects will be assigned in week 8 lecture and due the Friday of finals week. I will give you a list of topics to choose from. 

Architecture
===
Last week we built a simple process manager that included an abstract base class, `Process`, from which users can derive their own processes. We also defined a `Channel` class that enabled processes to send streams of values to each other.

![Elma Architecture](images/elma-structure.png)

This week we will
- Upgrade the `Channel` class to enable sending *JSON* data, instead of just `double` values.
- Add a new interprocess communication `Event` class.
- Add a set of classes for defining *Finite State Machines*.
- Improve the test coverage of Elma.

The resulting structure looks like the above. Orange indicates classes that we will define today.

JSON
===

The *Javascript Object Notation* or [JSON](http://www.json.org/) standard is a file and data format used almost ubiquitously in networked systems. It has become the standard way to represent arbitrary data types in a structured way, and to allow applications to communicate without much overhead in defining data structures and types. It is even used in some of the most popular databases, such as MongoDB, as the fundamental data structure for defining documents.

Some examples of where JSON is used include
- [Vehicle APIs](https://www.scip.ch/en/?labs.20180405)
- [Industrial Automation](https://openautomationsoftware.com/)
- [Databases](https://www.mongodb.com/)
- [Google Maps](https://www.sitepoint.com/google-maps-json-file/)
- [NOAA Data Services](https://www.ncdc.noaa.gov/access-data-service/api/v1/data?dataset=global-summary-of-the-year&dataTypes=DP01,DP05,DP10,DSND,DSNW,DT00,DT32,DX32,DX70,DX90,SNOW,PRCP&stations=ASN00084027&startDate=1952-01-01&endDate=1970-12-31&includeAttributes=true&format=json)

Data Representation in JSON
---

JSON allows a broad range of data to be represented as strings of characters. For example,
```json
{
    "tas": [
        {
            "first": "Justin",
            "last": "Vrana"
        },
        {
            "first": "Kung-Hung",
            "last": "Lu",
            "aka": "Henry"
        }
    ],
    "weeks": [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ],
    "version": 2.0
}
```
In a JSON string, you can represent
- Atomic values
    - strings delimited by double quotes
    - numbers, with or without decimals
    - true and false
    - null
- Arrays of values atomic values, objects and/or otjer arrays, delimited by [ and ]. 
- Objects with key value pairs, where values are atomic, objects, or arrays

The two key methods that one needs to support JSON in an application are

- *Serialization*, also knownn as *stringification*: Converting an application data structure into a JSON string.
- *Deserialization*, also known as *parsing*: Converting a JSON string into an application data structure.

For example, open the console on your web browser and type
```javascript
str = '{ "a": 1, "b": [2,3], "k": { "c": { "d": true } } }';
x = JSON.parse(str); 
x.k.c.d; // true
JSON.stringify(x);
```

Note that if your string has a format error in it, you will get an error when you try to deserialize it:
```javascript
JSON.parse('{"x": [1,2}')
```
which results in the useful message:
```
VM282:1 Uncaught SyntaxError: Unexpected token } in JSON at position 10
    at JSON.parse (<anonymous>)
    at <anonymous>:1:6
```

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

The JSON C++ Library
---

The main reason is that compilers cannot type check JSON values at compile time (since they do not know what they are until the applciations run).

Upgrading Channels to use JSON
===

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

Elevator: https://courses.cs.washington.edu/courses/cse466/02au/Lectures/State-models.pdf

New Directory Structure
===

Exercises
===

