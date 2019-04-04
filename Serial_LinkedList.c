#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_VALUE 65536 /* Linked list has values between 0 & 2^16-1 */

/* Variable initialization */
int n = 0; /* Number of nodes */
int m = 0; /* Number of random operations*/

float m_member_fraction = 0.0; /* Fraction of member operation */
float m_insert_fraction = 0.0; /* Fraction of insert operation */
float m_delete_fraction = 0.0; /* Fraction of delete operation */

int number_of_repeats = 0; /* Number of repetitions for the number of given operations*/
int times = 0; /* Number of times to do repeats*/
int sample_size = 0; /* ((100*z*s)/(r*x_bar))^2 */

float timeSum = 0.0;
float timeSquaredSum = 0.0;

float mean = 0.0; /* Mean */
float std = 0.0; /* Standard deviation */

/* Node definition */
struct list_node_s {
    int data;
    struct list_node_s *next;
};

int Insert(int value, struct list_node_s **head_pp);

int Member(int value, struct list_node_s *head_p);

int Delete(int value, struct list_node_s **head_pp);

void vaildateInput(int argc, char *argv[]);

double execution();

void Free_list(struct list_node_s **head_pp);

int Is_empty(struct list_node_s *head_p);

int main(int argc, char *argv[]) {

    /* Validate inputs */
    vaildateInput(argc, argv);
    /* Repeat to get the mean and std for a sufficient sample size */
    while (0 > number_of_repeats - sample_size || 5 < number_of_repeats - sample_size) {

        if (times > 0) {
            number_of_repeats = sample_size;
        }
        timeSum = 0.0;
        timeSquaredSum = 0.0;
        /* Repeat the number of m operations for the selected sufficient times */
        for (int j = 0; j < number_of_repeats; j++) {
            float elapsedTime = execution();
            timeSum += elapsedTime;
            timeSquaredSum += elapsedTime * elapsedTime;
        }

        mean = timeSum / number_of_repeats;
        std = sqrt((timeSquaredSum / number_of_repeats) - (mean * mean));
        printf("Mean: %.5f secs\n Std: %.5f \n", mean, std);

        sample_size = pow(((100 * 1.96 * std) / (5 * mean)), 2);
        if (sample_size == 0 || sample_size == 1) {
            sample_size = 2;
        }
        printf("sample size given from formula----------------%d\n", sample_size);
        times++;
    }
    printf("Finished!!\n");
    printf("Mean: %.5f secs\n Std: %.5f \n", mean, std);

}

double execution() {

    struct list_node_s *head_p = NULL;  /* start with empty list */

    /* Create linked list*/
    int count = 0;
    while (count < n) {

        if (Insert(rand() % MAX_VALUE, &head_p) == 1) {
            count++;
        }
    }

    int total = 0, member_count = 0, insert_count = 0, delete_count = 0; /* Variables to count member, insert, delete ops*/
    int m_member = 0, m_insert = 0, m_delete = 0; /* Allocated member, insert and delete operations*/

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
            total++;
        } else if (random_selection == 1 && insert_count < m_insert) {
            Insert(random_value, &head_p);
            insert_count++;
            total++;
        } else if (random_selection == 2 && delete_count < m_delete) {
            Delete(random_value, &head_p);
            delete_count++;
            total++;
        }
    }
    /* Stop clock*/
    clock_t end_time = clock();

    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time spent: %.5f secs\n", cpu_time_used);
    Free_list(&head_p);

    return cpu_time_used;
}

void vaildateInput(int argc, char *argv[]) {

    if (argc != 7) {
        fprintf(stderr,
                "Usage: ./<executable_name> <n> <m> <m_member_fraction> <m_insert_fraction> <m_delete_fraction> <repetition sample size>\n");
        exit(0);
    }

    n = (int) strtol(argv[1], NULL, 10);
    m = (int) strtol(argv[2], NULL, 10);

    m_member_fraction = (float) atof(argv[3]);
    m_insert_fraction = (float) atof(argv[4]);
    m_delete_fraction = (float) atof(argv[5]);

    number_of_repeats = (int) strtol(argv[6], NULL, 10);

    if (n < 0) {
        fprintf(stderr, "n: number of nodes in linked list -> should be a positive value.\n");
        exit(0);
    } else if (m < 0) {
        fprintf(stderr, "m: number of operations on the linked list -> should be a positive value.\n");
        exit(0);
    } else if (m_member_fraction < 0) {
        fprintf(stderr,
                "m_member_fraction: fraction of operations for member function: should be a value between 0-1.\n");
        exit(0);
    } else if (m_insert_fraction < 0) {
        fprintf(stderr,
                "m_insert_fraction: fraction of operations for insert function: should be a value between 0-1.\n");
        exit(0);
    } else if (m_delete_fraction < 0) {
        fprintf(stderr,
                "m_delete_fraction: fraction of operations for delete function: should be a value between 0-1.\n");
        exit(0);
    } else if (m_member_fraction + m_insert_fraction + m_delete_fraction != 1) {
        fprintf(stderr, "The sum of three fractions should be 1.\n");
        exit(0);
    } else if (number_of_repeats < 0) {
        fprintf(stderr, "The repetition sample count should be a positive value .\n");
        exit(0);
    }

}

/* Free_list */
void Free_list(struct list_node_s **head_pp) {
    struct list_node_s *curr_p;
    struct list_node_s *succ_p;

    if (Is_empty(*head_pp)) return;
    curr_p = *head_pp;
    succ_p = curr_p->next;
    while (succ_p != NULL) {
        free(curr_p);
        curr_p = succ_p;
        succ_p = curr_p->next;
    }
    free(curr_p);
    *head_pp = NULL;
}

/* Is_empty */
int Is_empty(struct list_node_s *head_p) {
    if (head_p == NULL)
        return 1;
    else
        return 0;
}


/* Member function */
int Member(int value, struct list_node_s *head_p) {
    struct list_node_s *curr_p = head_p;
    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    } else {
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
            free(curr_p);
        } else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    } else { /* Value is not in list */
        return 0;
    }
}
