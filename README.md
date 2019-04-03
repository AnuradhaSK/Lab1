# Lab1

Serial:

complie:
gcc -g -Wall -o serial_linked_list Serial_LinkedList.c -lm

run:
./serial_linked_list 1000 10000 0.99 0.005 0.005 10

Mutex:

complie:
gcc -g -Wall -o mutex_linked_list Mutex_LinkedList.c -lm -lpthread

run:
./mutex_linked_list 1000 10000 0.99 0.005 0.005 10 10 2

RW:

complie:
gcc -g -Wall -o rwl_linked_list Mutex_LinkedList.c -lm -lpthread

run:
./rwl_linked_list 1000 10000 0.99 0.005 0.005 10 10 2





