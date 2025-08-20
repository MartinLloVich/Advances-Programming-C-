Advanced Programming Practices
This repository contains the practical assignments for the Advanced Programming course, implementing various process communication and synchronization mechanisms in operating systems using the C language.

📋  Included Practices

1. Practice 1 - Pipes (MartinLoring_Practica1_Main.c)
Implementation of a number guessing game using pipes for communication between parent and child processes.

    Child process generates a random number (0-255)

    Parent process uses binary search to guess the number

    Bidirectional communication through two pipes


2. Practice 2 - Message Queues (Pract2_Productor.c, Pract2_Receptor.c)
Number guessing system using message queues.

    Producer: generates random number and responds to guesses

    Receiver: uses binary search to guess the number

    Different message types (1 for guesses, 2 for responses)


3. Practice 3 - Shared Memory (Pract3_proceso1.c, Pract3_proceso2.c, Pract3_common.h)
Guessing game implemented with shared memory and turn control.

    Process 1: generates sequential attempts

    Process 2: contains the secret number and responds

    Access control through turno variable in shared memory


4. Practice 4 - Semaphores (Pract4_Productor.c, Pract4_Consumidor.c, Pract4_Common.c)
Implementation of the producer-consumer problem using semaphores.

    Circular buffer of size 5

    Three semaphores: EMPTY, FULL, MUTEX

    Proper process synchronization


5. Practice 5 - Threads (Pract5_Main.c, Pract5_ProblemaLostWakeup.c)
    Programs demonstrating thread usage and synchronization with mutexes and condition variables.

    Basic thread synchronization

    Solution to the "lost wakeup" problem

    pthreads usage for concurrency


6. Practice 6 - Trees (Pract6_Main.c, Pract6_Sort_Tree.c)
Implementation of sorting algorithms using binary trees.

    Vector to binary tree conversion

    In-order traversal sorting

    Dynamic memory management



🛠️  Technologies Used

    Language: C

    Systems: Linux/Unix

    Libraries:

    System V IPC (Message Queues, Shared Memory, Semaphores)

    Pthreads for multithreading

    System calls for pipes and processes


📁  Repository Structure

    Advanced-Programming/


    ├── Practice1_Pipes/
    │   └── MartinLoring_Practica1_Main.c


    ├── Practice2_MessageQueues/
    │   ├── Pract2_Productor.c
    │   └── Pract2_Receptor.c


    ├── Practice3_SharedMemory/
    │   ├── Pract3_proceso1.c
    │   ├── Pract3_proceso2.c
    │   └── Pract3_common.h


    ├── Practice4_Semaphores/
    │   ├── Pract4_Productor.c
    │   ├── Pract4_Consumidor.c
    │   └── Pract4_Common.c


    ├── Practice5_Threads/
    │   ├── Pract5_Main.c
    │   └── Pract5_ProblemaLostWakeup.c


    └── Practice6_Trees/
    ├── Pract6_Main.c
    └── Pract6_Sort_Tree

    
🚀  Compilation and Execution
    Each practice can be compiled individually. For example:

    Compile pipes practice
    gcc MartinLoring_Practica1_Main.c -o practice1

    Compile threads practice
    gcc Pract5_Main.c -o practice5 -lpthread

    Execute
    ./practice1

    
📚 Applied Concepts

    Inter-process communication (IPC)

    Process synchronization

    Shared memory management

    Concurrent programming with threads

    Data structures (binary trees)

    Error handling and system resource management


👨‍💻  Author
    Martin Loring - Advanced Programming course assignments


📄  License
    This project is for educational purposes as part of university curriculum.

