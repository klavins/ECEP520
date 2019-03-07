Week 9: Social Coding and Cloud Computing
===

Last Time
---
- HTTP
- Servers
- Clients
- Threads
- HTTP in Elma

Today
---
- Social coding with Github branches, forks, and pull requests
- Cloud computing with AWS

&nbsp;

Social Coding with Github
===

Publishing your project
---

- Clean it up and put it on Github.
- Add a license file.
- Create a README.md.
- Add API documentation with Doxygen and Github pages.
    - Use `make docs` and then push to github
    - Go to Settings and choose docs for github pages
    - Your docs will be at https://yourid.github.io/yourrepo
- Tag a version.
    - Click `Releases` and `Create new release`
    - Use v + major.feature.fix as in v1.2.3
        - Major = total refactor and possibly new API
        - Feature = one or more new features
        - Fix = a bug fix, typo fix, or very minor feature

Contributing to a Project
---

- If you do not have push access, *fork* the project by navigating to the project and clicking `fork`. 
- Create a local copy of the repo.
    ```bash
    git clone https://github.com/yourid/elma.git
    ```
- Although you don't need to, you can create a new branch, in this case called "new_feature", but you can call it whatever you want. Or you can just stick with the master branch.
    ```bash
    git checkout -b new_feature
    ```
- Add and test your code. 
- Commit the code and push it ro your fork.
    ```bash
    git add .
    git commit -m "wrote some amazing code"
    git push origin new_feature
    ```
- Start a pull request. Go to your fork in github anbd click `New pull Request`. Choose:
    - base repository: klavinslab/elma
    - base: master
    - head repository: yourid/elma
    - compare: new_feature
- Click `Create pull request`
- Give your request a title and a description (in the comment area)
- Click `Create pull request`
- Await comments.
- Respond to comments by fixing, committing, and pushing your branch. 
- Eventually the maintainer will pull and close your request.
- Delete your branch. 
    - Click Delete branch.
- Switch back to master locally.
    ```bash
    git checkout master
    ```

Keep up with the main branch
---

Occasionally, the main branch will get updated. You will see near the top of the github page for your fork
```
This branch is 4 commits behind klavinslab:master.
```
In that case, you may want to get whatever the updates are. To do this, you first need to tell git where the fork came from. At the command line do:
```bash
git remote add upstream https://github.com/klavinslab/elma
```
You only need to do this once. 

Then, to update your master branch, do
```bash
git fetch upstream
git checkout master
git merge upstream/master
git push origin master
```

Bug reports
---

First step: Read all exiting issues to see if yours has already been reported.

Go to the `klavinslab/elma` repo and click `Issues` and then `New Issues`.

Label: ![bug](https://placehold.it/15/ff0000/000000?text=+) bug
---
Please be as speific as possible about the bug. If you can give a Google Test that fails, and the resulting message or compile error, that would be the best. Almost all bug reports will be handled by building as simple of a test as possible and then fixing the bug to make the test pass. Bugs that are not clear will likely generate comments and requests for clairifications. 
- Note: If you know how to fix the bug, submit a pull request instead of a bug report.

Label: ![enhancement](https://placehold.it/15/A2EEEE/000000?text=+) enhancement
---

Describe what your requested feature should do. Give example code that should compile if the feature were added. Describe why you think the feature is important, and describe what you think might be the consequences on the code base of adding the feature.

Label: ![help wanted](https://placehold.it/15/008672/000000?text=+) help wanted or ![question](https://placehold.it/15/d876e3/000000?text=+) question
---

If you have a question about why something isn't working or doesn't make sense, the Issues page is a great place to ask it. By asking questions that produce comments chains, you help generate a FAQ for the repository.

&nbsp;


Cloud Computing (on AWS)
===
Applications that need servers that work with users over the Internet have basically three choices:
- Build your own server computers and keep them on site.
- Use servers at a cloud hosting service like [AWS](https://aws.amazon.com/), [Azure](https://azure.microsoft.com/en-us/), [Google Cloud](https://cloud.google.com/), [Heroku](https://www.heroku.com/), etc.
- [Colocate](https://www.coloinseattle.com/): You buy the servers, but keep them at a specialized site.
Each has its pros and cons. For cloud services, you 
- can get started in minutes
- benefit from not having to purchase, maintain, or upgrade computers
- get a high-speed Internet connection
- do not need to worry about power failures, etc.
- you can schedule backups and other serves over the web
- get a more secure system.

You might think the downside is price. For about a year's worth of AWS payments for a server, you can buy your own server. However, you also need to factor in hiring someone to maintain it, deal with UPS, manager the network, etc.

Here, we'll focus on Amazon Web Services (AWS), which is by far the biggest player. In 2018, AWS had about 30% of the [market](https://www.parkmycloud.com/blog/aws-vs-azure-vs-google-cloud-market-share/), while Azure and Google Cloud each had 15% and 8% respectively. Sites using AWS range from Netflix to Autodesk. 

AWS
---

There are many specific services that AWS provides, including:
- EC2: Rent all or part of a computer. Install an OS. Connect via ssh. 
- Volumes: Rent a disk drive.
- RDMS: Rent a computer with database software, like MySQL or Postgres, already installed.
- ECS: Push a Docker container to a server and run it. 
- S3: Data storage with key/value filesystem.
- Redshift: AI managed storage for running a lot of data analysis.
- Glacier: Long term storage on tape
- Lambda: So called serverless computing. Put a simple function coded in your favorite language in the cloud and call it.

In addition, AWS provices 
- Machine learning 
- Text to speech
- Blockchain
- Analytics
- IO
- Robotics (using Gazebo)
- SES: Email server
- SSL Certificates
- IP Addresses

Example Configuration
---

https://www.matillion.com/blog/redshift/data-storage-amazon-redshift-vs-s3/

Example: Running a Server in the Cloud
---

Spin up a new EC2 instance.

Example: Serverless Computing
---

Examples of Lambdas: https://www.simform.com/serverless-aws-lambda-examples/

Make a Lambda:
```javascript
exports.handler = async (event) => {
    let msg = "";
    try {
      msg = JSON.parse(event.body).str.split("").reverse().join("");
    } catch(e) {
      msg = "error: " + e;
    }
    const response = {
        statusCode: 200,
        body: JSON.stringify({ msg: msg })
    };
    return response;
};
```

Example: Making a Container Run a Service
---

First, put `server.cc` and `Makefile` in the same directory as `Dokerfile`, as in the `week_9/server` directory. We have modified the server code to use port 80, the standard web server port. We have also adjusted the Makefile to include only those libraries we need, an d to use th `-O3` option to `g++`, which optimizes the executable for deployment.

Next, modify the `cppenv-http` Dockerfile to build the server:
```docker
# Build server
COPY Makefile /home
COPY server.cc /home
WORKDIR /home
RUN make
```
And to run the server when the container starts, we add:
```docker
# Run server
CMD /home/bin/server
```

To build the container, do:
```bash
docker build -t example_server .
```

and to run it do
```bash
docker run -p 80:80/tcp example_server 
```

Pushing an Image to Dockerhub
---
First, make a Dockerhub account.
```bash
docker tag 96f783ab6761 klavins/ecep520:example_server
docker push klavins/ecep520:example_server
```

Running a Container on AWS
---

AWS provides a [step by step procedure](https://aws.amazon.com/getting-started/tutorials/deploy-docker-containers/). This is a bit out of date though. In particular, you don't need to run ECS on and EC2 anymore, you can use [FARGATE](https://aws.amazon.com/fargate/).

Here is a procedure that currently works:

Choose custom and point to your tagged Dockerhub image (i.e. klavins/ecep520:example_server).
Choose port 80.
Choose Load Balancer. Why do they call it this?

Find the IP address. Go to Clusters > mycluster and click on the task. Find the Public IP. 

Testing the Server
---

```bash
curl -d '{ "x": 123, "y": 234, "temperature": 41.2 }' -X POST 18.215.189.229/save
```

&nbsp;

Exercises
===
1. Write an AWS Lambda function that takes two numbers and an operation and returns the result of that operation applied to those numbers. For example,
    ```json
    {
        "x": 1.2,
        "y": 2.3,
        "op": "multiply"
    }
    ```
    should return
    ```json
    {
        "result": 2.76
    }
    ```
    The operations allowed should be "multiply", "divide", "subtract", and "add". An error message of the form
    ```json
    {
        "error": "explain error here..."
    }
    ```
    should rersult from improperly formed json, missing keys, and unknown operations.

1. Write an Elma process called `MeasureLambda` that calls your AWS Lambda function using the client class and tests its response time. The process should have a constructor of the form
    ```c++
    MeasureLambda(std::string op, double x, double y);
    ```
    It should call the Lambda with those arguments every time it is updated and keep a list of the times it took for each call to return with a response. Provide functions
    ```c++
    double min();
    double max();
    double mean();
    ```
    that report the statistics. As an example, something like the following should compile:
    ```c++
    Manager m;
    MeasureLambda ml("add", 1, 2);
    m.schedule(ml, 1_s)
    .init()
    .run(20_s);

    std::cout << ml.min() << ", " << ml.max << ", " << ml.mean() << "\n";
    ```

1. Update your project README with a description of what you have accomplished this week and how you may need to adjust your milestones.

1. Get some aspect of the code for your project working and report in your README what it was, with pointers to the new code that worked, tests that passed, and so on.
