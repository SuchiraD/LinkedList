#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <math.h>


#define MAX_NUMBER 65536

struct node {
    int value;
    struct node *next;
};

struct node* head = NULL;

pthread_t tid[5];
pthread_rwlock_t lock;

int N = 10;
int M = 10;
int (*randomArray)[];
int memberOpsCount = 0;
int insertOpsCount = 0;
int deleteOpsCount = 0;

int member(int value, int* memberOpsCount) {
//    pthread_mutex_lock(&lock);
    (*memberOpsCount)--;
    struct node* current_p = head;

    while (current_p != NULL && current_p->value < value) {
        current_p = current_p->next;
    }

    if(current_p == NULL || current_p->value > value) {
//        pthread_mutex_unlock(&lock);
        return 0;
    } else {
//        pthread_mutex_unlock(&lock);
        return 1;
    }
}

//insert
int insert(int value, int* insertOpsCount) {
//    pthread_mutex_lock(&lock);
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
        } else {
            pred_p->next = temp_p;
        }

//        pthread_mutex_unlock(&lock);
        return 1;
    } else {
//        pthread_mutex_unlock(&lock);
        return 0;
    }
}

// Random generated values might not in the list
int delete(int value, int* deleteOpsCount) {
//    pthread_mutex_lock(&lock);
    (*deleteOpsCount)--;
    struct node* current_p = head;
    struct node* pred_p = NULL;

    if(head == NULL) {
//       pthread_mutex_unlock(&lock);
        return 0;
    }

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

//        pthread_mutex_unlock(&lock);
        return 1;
    } else {
//        pthread_mutex_unlock(&lock);
        return 0;
    }
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
            (*randomArray)[im++] = in;
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
        if((*randomArray)[i] > 65535) {
            printf("TOO LARGE\n");
        }
        insert((*randomArray)[i], NULL);
    }
}

void runAFunction(int value, int* memberOpsCount, int* insertOpsCount, int* deleteOpsCount) {
    int func;

//    pthread_mutex_lock(&lock);

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
            pthread_rwlock_rdlock(&lock);
            returnVal = member(value, memberOpsCount);
            pthread_rwlock_unlock(&lock);
            break;

        case 1:
            pthread_rwlock_wrlock(&lock);
            returnVal = insert(value, insertOpsCount);
            pthread_rwlock_unlock(&lock);
            break;

        case 2:
            pthread_rwlock_wrlock(&lock);
            returnVal = delete(value, deleteOpsCount);
            pthread_rwlock_unlock(&lock);
            break;
    }

//    pthread_mutex_unlock(&lock);
}

void* run(void* arg) {
//    pthread_mutex_lock(&lock);
    /*pthread_t id = pthread_self();
    if(pthread_equal(id, tid[0]) == 1) {
        printf("Thread id = 1\n");
    } else if (pthread_equal(id, tid[1]) == 1) {
        printf("Thread id = 2\n");
    }
*/
    int* moc_p = malloc(sizeof(int*));
    int* ioc_p = malloc(sizeof(int*));
    int* doc_p = malloc(sizeof(int*));
    *moc_p = memberOpsCount;
    *ioc_p = insertOpsCount;
    *doc_p = deleteOpsCount;

    while (*moc_p!=0 || *ioc_p!=0 || *doc_p!=0) {
        runAFunction(abs(rand())%MAX_NUMBER, moc_p, ioc_p, doc_p);
    }

//    pthread_mutex_unlock(&lock);
}

double calculateAvg(double array_p[], int count) {
    double sum = 0;
    for (int i = 0; i < count; i++) {
        sum += array_p[i];
//        printf("-----------  In AVG  : %f\n", (*array_p)[i]);
    }

    return sum/count;
}

double calculateStd(double array_p[], int count) {
    double mu = calculateAvg(array_p, count);

    double squaredSum = 0;
    for (int i = 0; i < count; i++) {
        double tempVal = (array_p[i] - mu);
        squaredSum += tempVal*tempVal;
//        printf("-----------  In STD  : %f\n", (*array_p)[i]);
    }

    return sqrt(squaredSum/(count));
}

int properSampleSize(double std, double mean) {
    double tempVal = (100*1.960*std/(5*mean));

    return (int) (tempVal*tempVal);
}

int main() {
    //Changing the PRNG seed according to the current time stamp


    int nThreads[] = {1, 2, 4};
    int sampleSize = 110;

    printf("***********   Parallel program with read-write lock   **********\n");

    N = 1000;
    M = 10000;
    float mMember = 0.99;
    float mInsert = 0.005;
    float mDelete = 0.005;

    printf("N = %d\n", N);
//    scanf("%d", &N);

    printf("M = %d\n", M);
//    scanf("%d", &M);

    printf("Enter Member, Insert, Delete fractions: ");
    scanf("%f%f%f", &mMember, &mInsert, &mDelete);


//    printf("List's Element count before: %i\n", countListElements(head));

    memberOpsCount = (int) (mMember*M);
    insertOpsCount = (int) (mInsert*M);
    deleteOpsCount = (int) (mDelete*M);

//    printf("Creating Threads\n");

    double times[sampleSize];

    printf("\nSample size = %d\n", sampleSize);
    for (int nThreadIndex = 0; nThreadIndex < 3; ++nThreadIndex) {
        for (int j = 0; j < sampleSize; j++) {
            srand((unsigned int) time(NULL));
            head = NULL;

            int array[N];
            randomArray = &array;

            populate();

            if (pthread_rwlock_init(&lock, NULL) != 0)
            {
                printf("\n mutex init failed\n");
                return 1;
            }

            clock_t start = clock();
            int i = 0;
            int err;
            while(i < nThreads[nThreadIndex])
            {
                err = pthread_create(&(tid[i]), NULL, &run, NULL);
                if (err != 0)
                    printf("\ncan't create thread :[%s]", strerror(err));
                i++;
            }

//    printf("Threads created\n");

            i = 0;
            while(i < nThreads[nThreadIndex])
            {
                pthread_join(tid[i], NULL);
                i++;
            }
            clock_t end = clock();

            times[j] = ((double) (end - start)) / CLOCKS_PER_SEC;

//        printf("-----------  In Sample run  : %f\n", times[j]);
        }

        pthread_rwlock_destroy(&lock);

        double std = calculateStd(times, sampleSize);
        double mean = calculateAvg(times, sampleSize);

        printf("-----------------------------------\n");
        printf("Number of threads = %d\n", nThreads[nThreadIndex]);
        printf("Standard deviation = %f\n", std);
        printf("Mean = %f\n", mean);
    }



//    printf("List's Element count after: %i\n", countListElements(head));

    printf("FINISHED\n");
}
