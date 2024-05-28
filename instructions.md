# Threading

[[_TOC_]]

## Purpose

To build a program in C containing 4 threads and a small Python program that allows the user to send commands to the threading program.

## Intent
The developer will learn the basics of multi-threading and implementing thread-safety. The threads are simple in that they do not communicate with each other, but they do share access to the same structures. The developer will learn to safely handle multiple threads accessing the same data. The developer will demonstrate the ability to:
* Create a design plan.
* Use Test Driven Development (TDD) in their projects.
* Generate documentation.
* Use proper header and .c file syntax to create a library.
* Learn how to spawn, use, and exit threads.
* Learn how to control threads from the main program.
* Learn how to use mutexes to safely access data in threads.

## Learning Objectives
The developer will gain the ability to work with multiple threads using the C programming language. The developer will learn how to handle accessing shared memory across threads safely. The developer will learn how to control spawned threads from within the main program.

## Requirements
* Any data accessible by multiple threads should use mutexes.
* Use of global variables is limited to 1 boolean type. Threads can receive structures as an argument.
* Threads should not handle the responsibilities of other threads, they should only perform their designated tasks.

Provided is a sender program that sends 1 packet per second to UDP port 6002 on localhost. The packet structure is as follows:

<table>
<tr><td colspan=3><h3>Packet Structure</h3></td></tr>
<tr><td>packet ID</td><td>16 bit unsigned int</td><td>starts at 0 and increments by 1 per packet sent </td></tr>
<tr><td>size</td><td>16 bit unsigned int</td><td>size of packet in bytes including header. This is a fixed size of 368 bytes for this project</td></tr>
<tr><td>option</td><td>16 bit unsigned int</td><td>selected randomly:
<ul>
<li>0: do nothing to data, </li>
<li>1: add 1 to each item in data, </li>
<li>2: subtract 1 to each item in data, </li>
<li>3: multiply each item in data by 3, </li>
<li>4: square each item in data</li>
</ul>
</td></tr>
<tr><td>data</td><td>32 bit float [10]</td><td>An array of 10 random 32 bit float integers</td></tr>
</table>

### Receiver Thread
* Receives packets on UDP port 6002 of localhost.
* Puts packets into an array, implemented with the following requirements:
    * The array will start with 10 slots.
    * The array will be dynamically resized as more slots are needed, up to 100 slots.
* The thread will discard incoming packets while the array is full.

### Processor Threads
* Both processor threads can process:
    * NOOP commands (Packet Option 0)
* 1 processor thread can process:
    * add commands (Packet Option 1)
    * sub commands (Packet Option 2)
* 1 processor thread can process:
    * mult commands (Packet Option 3)
    * square commands (Packet Option 4)
* Both threads will pull packets from the receiver array.
* Both threads will store results in a new array with the following requirements:
    * The array will start with 10 slots.
    * The array will be dynamically resized as more slots are needed, up to 100 slots.
    * The array will store the following structure:
        * `packet ID`: 16 bit unsigned int
        * `size`: the size in bytes of the entire result
        * `data`: 64 bit double [10], perform the operations on the original packets data and store in this array.
* The threads will pause if the receiver array is empty or the result array is full.

### Responder thread
* Receives commands from the Python command program.
* Listens on UDP port 6003 on localhost.
* Accepts the following commands and send the results to the command program:
    * `query <packet ID>`: pulls the result with that packet ID from the result array.
    * `status`: returns whether each array is full.
    * `list`: lists all packet IDs currently in the result array.
    * `pause`: tell the processor threads to pause the processing of packets, or to continue if already paused.
    * `shutdown`: tell the program to gracefully shutdown, including proper shutdown of each thread and freeing of memory.

### Command program
* Written in Python
* Allows the user to input commands to send to the responder thread and outputs the results.
* The commands will be sent to UDP port 6003 on localhost and are as follows:
    * `query <packet ID>`: receives the result data associated with that packet ID.
    * `status`: receives the status of each array, whether they are full or not at a minimum.
    * `list`: receives a list of all packet IDs currently in the result array.
    * `pause`: receives a success or failure, pauses or continues the processor threads.
    * `shutdown`: receives a success or failure, tells the program to gracefully shutdown.


## Timeline
This project is expected to take approximately 2 weeks. The mentor should perform a code review at least once per week.

### Week 1
* Create design plan
* Create `makefile`
* Create headers
* Create test functions
* Receiver thread

### Week 2
* Processor threads
* Responder thread
* Command program
* Final testing and bug fixes
* Final review

## Advice
* Think about which functions control the locks.
* Remember `pthread_mutex_trylock()`.
* Automated tests can run thousands of times where you can run a manual test once. Plus, they can run without you!

## JQR Items
This project should cover the following JQR items:
* 4.1.10
* 4.1.13
* 4.1.18
* 4.1.25
* 4.2.2
* 4.2.5
* 4.2.6
* 4.2.7
* 4.2.8
* 4.5.2
* 4.5.4
* 4.8.1
* 4.8.2
* 4.8.3


<style>
tr td {vertical-align: top;}
</style>
