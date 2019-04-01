#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_VALUE 65535 /* Linked list has values between 0 & 2^16-1 */

/* Variable initialization */
int n = 0; /* Number of nodes */
int m = 0; /* Number of random operations*/

float m_member_fraction = 0.0; /* Fraction of member operation */
float m_insert_fraction = 0.0; /* Fraction of insert operation */
float m_delete_fraction = 0.0; /* Fraction of delete operation */

/* Node definition */
struct list_node_s {
    int data;
    struct list_node_s *next;
};

int Insert(int value, struct list_node_s **head_p);

int Member(int value, struct list_node_s *head_p);

int Delete(int value, struct list_node_s **head_p);

void vaildateInput(int argc, char *argv[]);

double execution();

int main(int argc, char *argv[]) {

    float timeSum = 0.0;
    float timeSquaredSum = 0.0;
    vaildateInput(argc, argv);
    for (int j = 0; j < 100; j++) {
        float elapsedTime = execution();
        timeSum += elapsedTime;
        timeSquaredSum += elapsedTime * elapsedTime;
    }

    float mean = timeSum / 100;
    float std = sqrt((timeSquaredSum / 100) - mean * mean);
    printf("Mean: %.5f secs\n Std: %.5f \n", mean, std);

}

double execution() {

    struct list_node_s *head_p = NULL;  /* start with empty list */

    /* Create linked list*/
    for (int i = 0; i < n; i++) {
        Insert(rand() % MAX_VALUE, &head_p);
    }

    int total = 0, member_count = 0, insert_count = 0, delete_count = 0;
    int m_member = 0, m_insert = 0, m_delete = 0;
//    struct timeval time_begin, time_end;

    m_member = m * m_member_fraction;
    m_insert = m * m_insert_fraction;
    m_delete = m - (m_member + m_insert);

    /* Start clock */
    clock_t start_time = clock();

    /* Execute m operations */
    while (total < m) {
        int random_value = rand() % MAX_VALUE;
        int random_selection = rand() % 3;

        if (random_selection == 0 && member_count < m_member) {
            Member(random_value, head_p);
            member_count++;
        } else if (random_selection == 1 && insert_count < m_insert) {
            Insert(random_value, &head_p);
            insert_count++;
        } else if (random_selection == 2 && delete_count < m_delete) {
            Delete(random_value, &head_p);
            delete_count++;
        }
        total = member_count + insert_count + delete_count;
    }
    /* Stop clock*/
    clock_t end_time = clock();

    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time spent: %.5f secs\n", cpu_time_used);

    return cpu_time_used;
}

void vaildateInput(int argc, char *argv[]) {

    if (argc != 6) {
        fprintf(stderr,
                "Usage: ./Serial_LinkedList <n> <m> <m_member_fraction> <m_insert_fraction> <m_delete_fraction>\n");
        exit(0);
    }

    n = (int) strtol(argv[1], NULL, 10);
    m = (int) strtol(argv[2], NULL, 10);

    m_member_fraction = (float) atof(argv[3]);
    m_insert_fraction = (float) atof(argv[4]);
    m_delete_fraction = (float) atof(argv[5]);

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
