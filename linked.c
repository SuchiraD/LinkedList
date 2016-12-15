#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 10
#define N 65536
#define pop populate()

struct node {
    int value;
    struct node *next;
};

struct node* head = NULL;
struct node* current = NULL;

int randomArray[M];

int member(int value) {
    struct node* current_p = head;

    while (current_p != NULL && current_p->value < value) {
        current_p = current_p->next;
    }

    if(current_p == NULL || current_p->value > value) {
        return 0;
    } else
        return 1;
}

//insert
int insert(int value) {
    struct node* current_p = head;
    struct node* pred_p = NULL;
    struct node* temp_p;

    while (current_p != NULL && current_p->value < value) {
        pred_p = current_p;
        current_p = current_p->next;
    }

    if(current_p == NULL || current_p->value > value) {
        temp_p = malloc(sizeof(struct node));
        temp_p->value = value;
        temp_p->next = current_p;
        if(pred_p == NULL) {
            head = temp_p;
        } else
            pred_p->next = temp_p;

        return 1;
    } else
        return 0;
}

int delete(int value) {
    struct node* current_p = head;
    struct node* pred_p = NULL;

    while (current_p != NULL && current_p->value < value) {
        pred_p = current_p;
        current_p = current_p->next;
    }

    if(current_p == NULL || current_p->value == value) {
        if(pred_p == NULL) {
            head = current_p->next;
        } else
            pred_p->next = current_p->next;
        free(current_p);

        return 1;
    } else
        return 0;
}

//The Knuth algorithm for random unique numbers
void uniqueRandomNums() {
    int in, im;

    im = 0;

    for (in = 0; in < N && im < M; ++in) {
        int rn = N - in;
        int rm = M - im;
        if (rand() % rn < rm)
            /* Take it */
            randomArray[im++] = in;
    }

}

void populate() {
    uniqueRandomNums();
    for (int i = 0; i < M; i++ ) {
        insert(randomArray[i]);
        printf("%d\n", randomArray[i]);
    }
}

int main() {
    //Changing the PRNG seed according to the current time stamp
    srand((unsigned int) time(NULL));
    pop;

}

