#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_NUMBER 65536

struct node {
    int value;
    struct node *next;
};

struct node* head = NULL;
struct node* current = NULL;

int N = 10;
int M = 10;
int (*randomArray)[];
int memberOpsCount = 0;
int insertOpsCount = 0;
int deleteOpsCount = 0;

int member(int value, int* memberOpsCount) {
    (*memberOpsCount)--;
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
int insert(int value, int* insertOpsCount) {
    if(insertOpsCount != NULL)
        (*insertOpsCount)--;

    if(value > 65535) {
        printf("too long");
    }
    struct node* current_p = head;
    struct node* pred_p = NULL;
    struct node* temp_p;

    while (current_p != NULL && current_p->value < value) {
        pred_p = current_p;
        current_p = current_p->next;
        if(current_p!= NULL && current_p->value > 65535) {
            printf("too long");
        }
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

// Random generated values might not in the list
int delete(int value, int* deleteOpsCount) {
    (*deleteOpsCount)--;
    struct node* current_p = head;
    struct node* pred_p = NULL;

    if(head == NULL)
        return 0;

    while (current_p != NULL && current_p->value < value) {
        pred_p = current_p;
        current_p = current_p->next;
    }

    if(current_p == NULL || current_p->value == value) {
        if(pred_p == NULL) {
            head = current_p==NULL ? NULL: current_p->next;
        } else
            pred_p->next = current_p==NULL ? NULL: current_p->next;
        free(current_p);

        return 1;
    } else
        return 0;
}

//The Knuth algorithm for random unique numbers
void uniqueRandomNums() {
    int in, im;

    im = 0;

    for (in = 0; in < MAX_NUMBER && im < N; ++in) {
        int rn = MAX_NUMBER - in;
        int rm = N - im;
        if (rand() % rn < rm){
            /* Take it */
            *randomArray[im++] = in;
            printf("%d\n", im);
        }
    }

}

int countListElements(struct node* head) {
    struct node* current = head;
    int count = 0;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;
}

void populate() {
    uniqueRandomNums();
    for (int i = 0; i < N; i++ ) {
        if(randomArray[i] > 65535) {
            printf("TOO LARGE\n");
        }
        insert(randomArray[i], NULL);
    }
}

void runAFunction(int value, int* memberOpsCount, int* insertOpsCount, int* deleteOpsCount) {
    int func;

    while (true) {
        func = rand()%3;
        if(func == 0 && *memberOpsCount == 0) {
            continue;
        }
        else if(func == 1 && *insertOpsCount == 0) {
            continue;
        }
        else if(func == 2 && *deleteOpsCount == 0) {
            continue;
        }
        else
            break;
    }

    int returnVal = 0;
    switch (func) {
        case 0:
            returnVal = member(value, memberOpsCount);
            break;

        case 1:
            returnVal = insert(value, insertOpsCount);
            break;

        case 2:
            returnVal = delete(value, deleteOpsCount);
            break;
    }
}

void run() {
    int* moc_p = malloc(sizeof(int*));
    int* ioc_p = malloc(sizeof(int*));
    int* doc_p = malloc(sizeof(int*));
    *moc_p = memberOpsCount;
    *ioc_p = insertOpsCount;
    *doc_p = deleteOpsCount;
    while (*moc_p!=0 || *ioc_p!=0 || *doc_p!=0) {
        runAFunction(abs(rand())%MAX_NUMBER, moc_p, ioc_p, doc_p);
    }
}

int main() {
    //Changing the PRNG seed according to the current time stamp
    srand((unsigned int) time(NULL));

    N = 10000;
    M = 10000;
    float mMember = 0.98;
    float mInsert = 0.01;
    float mDelete = 0.01;

    int array[N];
    randomArray = &array;

    printf("Enter N: ");
//    scanf("%d", &N);

    printf("Enter M: ");
//    scanf("%d", &M);

    printf("Enter Member, Insert, Delete fractions: ");
//    scanf("%f%f%f", &mMember, &mInsert, &mDelete);

    printf("\n");
    populate();
    printf("List's Element count before: %i\n", countListElements(head));

    memberOpsCount = (int) (mMember*M);
    insertOpsCount = (int) (mInsert*M);
    deleteOpsCount = (int) (mDelete*M);

    run();

    printf("List's Element count after: %i\n", countListElements(head));

    printf("FINISHED");
}

