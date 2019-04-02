#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define MAX_VALUE 65535 /* Linked list has values between 0 & 2^16-1 */

/* Variable initialization */
int n = 0; /* Number of nodes */
int m = 0; /* Number of random operations*/

float m_member_fraction = 0.0; /* Fraction of member operation */
float m_insert_fraction = 0.0; /* Fraction of insert operation */
float m_delete_fraction = 0.0; /* Fraction of delete operation */

int total = 0, member_count_total = 0, insert_count_total = 0, delete_count_total = 0;
int m_member = 0, m_insert = 0, m_delete = 0;

struct list_node_s *head_p = NULL;  /* start with empty list */

int thread_count = 0;
pthread_mutex_t mutex;
pthread_mutex_t count_mutex;

/* Node definition */
struct list_node_s {
    int data;
    struct list_node_s *next;
};

int Member(int value, struct list_node_s *head_p);

int Insert(int value, struct list_node_s **head_p);

int Delete(int value, struct list_node_s **head_p);

void vaildateInput(int argc, char *argv[]);

double execution();

void *thread_function(void *rank);

int main(int argc, char *argv[]) {

    vaildateInput(argc, argv);

    float timeSum = 0.0;
    float timeSquaredSum = 0.0;

    for (int j = 0; j < 100; j++) {
        float elapsedTime = execution();
        timeSum += elapsedTime;
        timeSquaredSum2 += elapsedTime * elapsedTime;
        member_count = 0;
        insert_count = 0;
        delete_count = 0;
    }

    float mean = timeSum / 100;
    float std = sqrt((timeSquaredSum / 100) - mean * mean);
    printf("Mean: %.5f secs\n Std: %.5f \n", mean, std);

}

void *thread_function(void *rank) {

    int thread_member_count = 0;
    int thread_insert_count = 0;
    int thread_delete_count = 0;

    int ops_per_thread = m/thread_count;
    int ops;

    /* Execute m operations */

}

double execution() {

    long thread;
    pthread_t *thread_handles;
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    /* Create linked list*/
    int count = 0;
    while (count < n) {
//        int val =rand()%MAX_VALUE;
//        printf("%d%s",val,"\n");
        int status = Insert(rand()%MAX_VALUE, &head_p);
        if(status == 1) {
            count++;
        }
    }

    m_member = m * m_member_fraction;
    m_insert = m * m_insert_fraction;
    m_delete = m - (m_member + m_insert);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&count_mutex, NULL);

    /* Start clock */
    clock_t start_time = clock();

    /* Create thread and invoke thread function */
    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, thread_function, (void *) thread);
    }

    for(thread =0;thread < thread_count;thread++){
        pthread_join(thread_handles[thread], NULL);
    }
    /* Stop clock*/
    clock_t end_time = clock();

    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time spent: %.5f secs\n", cpu_time_used);

    Free_list();
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&count_mutex);
    free(thread_handles);

    return cpu_time_used;
}

void vaildateInput(int argc, char *argv[]) {

    if (argc != 7) {
        fprintf(stderr,
                "Usage: ./Serial_LinkedList <thread_count> <n> <m> <m_member_fraction> <m_insert_fraction> <m_delete_fraction>\n");
        exit(0);
    }

    thread_count = (int) strtol(argv[1], NULL, 10);
    if (thread_count <= 0) {
        fprintf(stderr, "Number of threads should be greater than 0");
        exit(0);
    }

    n = (int) strtol(argv[2], NULL, 10);
    m = (int) strtol(argv[3], NULL, 10);

    m_member_fraction = (float) atof(argv[4]);
    m_insert_fraction = (float) atof(argv[5]);
    m_delete_fraction = (float) atof(argv[6]);

    if (n < 0 || m < 0 || m_member_fraction < 0 || m_insert_fraction < 0 || m_delete_fraction < 0 ||
        m_member_fraction + m_insert_fraction + m_delete_fraction != 1) {
        fprintf(stderr,
                "error in input values.\n"
                "n: number of nodes in linked list -> should be a positive value.\n"
                "m: number of operations on the linked list -> should be a positive value.\n"
                "m_member_fraction: fraction of operations for member function: should be a value between 0-1.\n"
                "m_insert_fraction: fraction of operations for insert function: should be a value between 0-1.\n"
                "m_delete_fraction: fraction of operations for delete function: should be a value between 0-1.\n");
        exit(0);
    }

}

/* Member function */
int Member(int value, struct list_node_s *head_p) {
    struct list_node_s *curr_p = head_p;
    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value) {
//        printf("%d is not in the list\n", value);
        return 0;
    } else {
//        printf("%d is in the list\n", value);
        return 1;
    }
}

/* Insert function */
int Insert(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL) /* New first node */
            *head_pp = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    } else { /* value already in list */
//        printf("%d is already in the list\n", value);
        return 0;
    }
}

/* Delete function */
int Delete(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;

    /* Find value */
    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) { /* Deleting first node in list */
            *head_pp = curr_p->next;
//            printf("Freeing %d\n", value);
            free(curr_p);
        } else {
            pred_p->next = curr_p->next;
//            printf("Freeing %d\n", value);
            free(curr_p);
        }
        return 1;
    } else { /* Value is not in list */
//        printf("%d is not in the list\n", value);
        return 0;
    }
}

