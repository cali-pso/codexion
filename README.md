*This project has been created as part of the 42 curriculum by rapoggi*

# Codexion

## Description

This repository is my version of the Codexion project from 42's curriculum.  
It is written in C and aims for a better understanding of concurrency, threads, and mutexes.
The goal is to orchestrate multiple coders competing for limited USB dongles using POSIX threads, mutexes, and smart scheduling before burnout strikes.
  
This implies managing shared resources without stalling progress, choices 
which will be discussed later on in the thread synchronization section.  

## Instructions
  
A Makefile has been provided. Run:  

```bash
    make
```
> to compile the source code into the `codexion` executable.
  
To execute run:  
```bash
    make run
```
> this will run the simulation with default arguments (5 coders, 403ms burnout, etc.) passed to the executable.
   
To execute with custom parameters, run:
```bash
    ./codexion <nb_coders> <tt_burnout> <tt_compile> <tt_debug> <tt_refactor> <nb_compiles> <dngl_cooldown> <scheduler>
```
> scheduler must be exactly one of: `fifo` or `edf`.

```bash
    make clean
```
> removes compiled object files.  
 
```bash
    make fclean
```
> removes compiled objects and the executable file.

## Blocking cases handled

As required by the subject, several concurrency issues were addressed in this solution:

+ **Deadlock prevention & Coffman's conditions:** Deadlocks are prevented by establishing a strict locking hierarchy. Coders always lock the dongle with the lower ID first, and the higher ID second. This breaks the circular wait condition.
+ **Starvation prevention:** An orchestrator thread manages a priority queue to serve coders based on the `fifo` or `edf` (Earliest Deadline First) scheduling policies.
+ **Cooldown handling:** Each dongle tracks its availability timestamp (`free_at`). The orchestrator explicitly verifies that the current time has surpassed the cooldown period before granting access.
+ **Precise burnout detection:** A separate monitor thread constantly checks the `last_compile` timestamp of each coder to ensure burnout logs are displayed within the strict 10 ms tolerance.
+ **Log serialization:** A dedicated global mutex (`output->lock`) surrounds all print statements to guarantee that two messages never interleave on a single line.

## Thread synchronization mechanisms

This project relies heavily on POSIX threading primitives to coordinate access to shared resources.

+ **Dongles:** Each dongle is protected by its own `pthread_mutex_t`.
+ **State Management:** Individual mutexes protect each coder's state (`last_compile`, `compiles_done`) to prevent race conditions between the coders updating their status and the monitor thread reading it. The global system state (running flag) is also protected by a mutex.
+ **Thread-safe communication:** The `pthread_cond_t` (condition variables) are utilized extensively for communication. When a coder finishes a cycle, they submit a request to the orchestrator's priority queue and wait on their personal condition variable. Once the orchestrator secures the required dongles for that coder, it updates their state and broadcasts a signal to wake them up, ensuring efficient, thread-safe communication.

## Resources

<https://man7.org/linux/man-pages/man7/pthreads.7.html>  
For reference about POSIX threads (mutexes, condition variables, joins)  
<https://geeks4geeks.org>  
To answer various C and concurrency questions  
 
AI was used throughout the project as a help for understanding 
complex previously unseen C concurrency concepts.  
 
This repository contains no AI generated code/content.
