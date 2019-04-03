#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define MAX_VALUE 65536 /* Linked list has values between 0 & 2^16-1 */

/* Variable initialization */
int n = 0; /* Number of nodes */
int m = 0; /* Number of random operations*/

float m_member_fraction = 0.0; /* Fraction of member operation */
float m_insert_fraction = 0.0; /* Fraction of insert operation */
float m_delete_fraction = 0.0; /* Fraction of delete operation */

int number_of_repeats = 0;

//int total = 0, member_count_total = 0, insert_count_total = 0, delete_count_total = 0;
int m_member = 0, m_insert = 0, m_delete = 0;
int member_remainder = 0, insert_remainder = 0, delete_remainder = 0;

struct list_node_s *head_p = NULL;  /* start with empty list */

int thread_count = 0;
pthread_mutex_t mutex;
//pthread_mutex_t count_mutex;

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

void *thread_function(void *id);

void Free_list(struct list_node_s **head_pp);

int Is_empty(struct list_node_s *head_p);

int main(int argc, char *argv[]) {

    vaildateInput(argc, argv);

    float timeSum = 0.0;
    float timeSquaredSum = 0.0;

    for (int j = 0; j < number_of_repeats; j++) {
        float elapsedTime = execution();
        timeSum += elapsedTime;
        timeSquaredSum += elapsedTime * elapsedTime;
//        member_count_total = 0;
//        insert_count_total = 0;
//        delete_count_total = 0;
    }

    float mean = timeSum / number_of_repeats;
    float std = sqrt((timeSquaredSum / number_of_repeats) - mean * mean);
    printf("Mean: %.5f secs\n Std: %.5f \n", mean, std);

}

void *thread_function(void *id) {
    long rank = (long) id;

    int thread_member = 0, thread_insert = 0, thread_delete = 0;
    int thread_total = 0;
//    int thread_member_count = 0, thread_insert_count = 0, thread_delete_count = 0;

    int ops_per_thread = m / thread_count;

    /* Calculate number of member operations per thread */
    if (member_remainder == 0) {
        thread_member = m_member / thread_count;
    } else {
        if (member_remainder > rank) {
            thread_member = (m_member / thread_count) + 1;
        } else {
            thread_member = m_member / thread_count;
        }
    }

    /* Calculate number of insert operations per thread */
    if (insert_remainder == 0) {
        thread_insert = m_insert / thread_count;
    } else {
        if (member_remainder <= rank && rank < member_remainder + insert_remainder) {
            thread_insert = m_insert / thread_count + 1;
        } else if (member_remainder + insert_remainder > thread_count &&
                   (insert_remainder - (thread_count - member_remainder)) > rank) {
            thread_insert = m_insert / thread_count + 1;
        } else {
            thread_insert = m_insert / thread_count;
        }
    }

    /* Calculate number of delete operations per thread */
    if (delete_remainder == 0) {
        thread_delete = m_delete / thread_count;
    } else {
        thread_delete = ops_per_thread - (thread_member + thread_insert);
    }
//    printf("Thread id ------------------------> %d \n", rank);
//    printf("Thread member count %d\n", thread_member);
//    printf("Thread insert count %d\n", thread_insert);
//    printf("Thread delete count %d\n", thread_delete);


    /* Execute m operations */
    while (thread_total < ops_per_thread) {
        int ops = rand() % 3;
//        printf("%d ----> %d \n", ops, rank);
        int random_value = rand() % MAX_VALUE;

        if (ops == 0 && thread_member != 0) {
//            printf("member operation \n");
            pthread_mutex_lock(&mutex);
            Member(random_value, head_p);
            pthread_mutex_unlock(&mutex);
            thread_member--;
            thread_total++;
        } else if (ops == 1 && thread_insert != 0) {
//            printf("insert operation \n");
            pthread_mutex_lock(&mutex);
            Insert(random_value, &head_p);
            pthread_mutex_unlock(&mutex);
            thread_insert--;
            thread_total++;
        } else if (ops == 2 && thread_delete != 0) {
//            printf("delete operation \n");
            pthread_mutex_lock(&mutex);
            Delete(random_value, &head_p);
            pthread_mutex_unlock(&mutex);
            thread_delete--;
            thread_total++;
        }
//        thread_total = thread_member_count + thread_insert_count + thread_delete_count;
    }
//    pthread_mutex_lock(&count_mutex);
//    member_count_total += thread_member_count;
//    insert_count_total += thread_insert_count;
//    delete_count_total += thread_delete_count;
//    pthread_mutex_unlock(&count_mutex);

    return NULL;
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
        if (Insert(rand() % MAX_VALUE, &head_p) == 1) {
            count++;
        }
    }

    m_member = m * m_member_fraction;
    m_insert = m * m_insert_fraction;
    m_delete = m - (m_member + m_insert);

    /* Number of operations remain when operations are equally distributed to threads */
    member_remainder = m_member % thread_count;
    insert_remainder = m_insert % thread_count;
    delete_remainder = m_delete % thread_count;

    pthread_mutex_init(&mutex, NULL);
//    pthread_mutex_init(&count_mutex, NULL);

    /* Start clock */
    clock_t start_time = clock();

    /* Create thread and invoke thread function */
    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, thread_function, (void *) thread);
    }

    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    /* Stop clock*/
    clock_t end_time = clock();
//    printf("member count %d\n", member_count_total);
//    printf("insert count %d\n", insert_count_total);
//    printf("delete count %d\n", delete_count_total);

    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time spent: %.5f secs\n", cpu_time_used);

    Free_list(&head_p);
    pthread_mutex_destroy(&mutex);
//    pthread_mutex_destroy(&count_mutex);
    free(thread_handles);

    return cpu_time_used;
}

void vaildateInput(int argc, char *argv[]) {

    if (argc != 8) {
        fprintf(stderr,
                "Usage: ./<executable name> <n> <m> <m_member_fraction> <m_insert_fraction> <m_delete_fraction> <repetition sample size> <thread_count>\n");
        exit(0);
    }

    n = (int) strtol(argv[1], NULL, 10);
    m = (int) strtol(argv[2], NULL, 10);

    m_member_fraction = (float) atof(argv[3]);
    m_insert_fraction = (float) atof(argv[4]);
    m_delete_fraction = (float) atof(argv[5]);

    number_of_repeats = (int) strtol(argv[6], NULL, 10);

    thread_count = (int) strtol(argv[7], NULL, 10);

    if (n < 0 ){
        fprintf(stderr, "n: number of nodes in linked list -> should be a positive value.\n");
        exit(0);
    } else if (m < 0) {
        fprintf(stderr, "m: number of operations on the linked list -> should be a positive value.\n");
        exit(0);
    } else if (m_member_fraction < 0) {
        fprintf(stderr, "m_member_fraction: fraction of operations for member function: should be a value between 0-1.\n");
        exit(0);
    } else if (m_insert_fraction < 0) {
        fprintf(stderr, "m_insert_fraction: fraction of operations for insert function: should be a value between 0-1.\n");
        exit(0);
    } else if (m_delete_fraction < 0) {
        fprintf(stderr, "m_delete_fraction: fraction of operations for delete function: should be a value between 0-1.\n");
        exit(0);
    } else if (m_member_fraction + m_insert_fraction + m_delete_fraction != 1) {
        fprintf(stderr, "The sum of three fractions should be 1.\n");
        exit(0);
    } else if (number_of_repeats < 0) {
        fprintf(stderr, "The repetition sample count should be a positive value .\n");
        exit(0);
    }else if (thread_count <= 0) {
        fprintf(stderr, "Number of threads should be greater than 0");
        exit(0);
    }
}

void Free_list(struct list_node_s **head_pp) {
    struct list_node_s *curr_p;
    struct list_node_s *succ_p;

    if (Is_empty(*head_pp)) return;
    curr_p = *head_pp;
    succ_p = curr_p->next;
    while (succ_p != NULL) {
//        printf("Freeing %d\n", curr_p->data);
        free(curr_p);
        curr_p = succ_p;
        succ_p = curr_p->next;
    }
//    printf("Freeing %d\n", curr_p->data);
    free(curr_p);
    *head_pp = NULL;
}  /* Free_list */


int Is_empty(struct list_node_s *head_p) {
    if (head_p == NULL)
        return 1;
    else
        return 0;
}  /* Is_empty */


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

