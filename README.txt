# Lab1
Open a terminal on lab1_150301V_150379N/Lab1 folder.
Then execute following commands.

Serial:

Implement a linked list as a serial program
File        : Serial_LinkedList.c
Compile     : gcc -g -Wall -o serial_linked_list Serial_LinkedList.c -lm
Execute     : ./serial_linked_list <n> <m> <m_member_fraction> <m_insert_fraction> <m_delete_fraction> <repetition sample size>
        eg  : ./serial_linked_list 1000 10000 0.99 0.005 0.005 20

Mutex:

Implement a linked list using one mutex to the entire linked list
File        : Mutex_LinkedList.c
Compile     : gcc -g -Wall -o mutex_linked_list Mutex_LinkedList.c -lm -lpthread
Execute     : ./mutex_linked_list <n> <m> <m_member_fraction> <m_insert_fraction> <m_delete_fraction> <repetition sample size> <thread_count>
        eg  : ./mutex_linked_list 1000 10000 0.99 0.005 0.005 20 2



Read Write Lock:

Implement a linked list using read write lock to the entire linked list
File        : rwl_LinkedList.c
Compile     : gcc -g -Wall -o rwl_linked_list rwl_LinkedList.c -lm -lpthread
Execute     : ./rwl_linked_list <n> <m> <m_member_fraction> <m_insert_fraction> <m_delete_fraction> <repetition sample size> <thread_count>
        eg  : ./rwl_linked_list 1000 10000 0.99 0.005 0.005 20 2







