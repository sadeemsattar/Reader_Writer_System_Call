#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include "conio.h"
struct BNode
{
    int key, heightFactor;
    struct BNode *left, *right;
    char message[100];
};

struct BNode *newNode(int key, char msg[])
{
    struct BNode *temp = (struct BNode *)malloc(sizeof(struct BNode));
    temp->key = key;
    strcpy(temp->message, msg);
    temp->heightFactor = 1;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

int get_height_factor(struct BNode *node)
{
    if (node == 0)
        return 0;
    return node->heightFactor;
}
//Get the height balance factor
//Leftsubtree subtract rightsubtree
int get_Balancefactor(struct BNode *node)
{
    if (node == 0)
        return 0;
    return get_height_factor(node->left) - get_height_factor(node->right);
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

//Right Rotate the subtree
struct BNode *rightRotation(struct BNode *y)
{
    struct BNode *x = y->left;
    struct BNode *T2 = x->right;

    //Perform Rotation
    x->right = y;
    y->left = T2;

    //Adjust the new heights factor
    y->heightFactor = 1 + max(get_height_factor(y->left), get_height_factor(y->right));
    x->heightFactor = 1 + max(get_height_factor(x->left), get_height_factor(x->right));

    return x;
}

//Left Rotate the subtree
struct BNode *leftRotation(struct BNode *x)
{
    struct BNode *y = x->right;
    struct BNode *T2 = y->left;

    //Perform Rotation
    x->left = y;
    y->right = T2;

    //Adjust the new heights factor
    x->heightFactor = 1 + max(get_height_factor(x->left), get_height_factor(x->right));
    y->heightFactor = 1 + max(get_height_factor(y->left), get_height_factor(y->right));

    return y;
}

struct BNode *Insert(struct BNode *node, int key, char msg[])
{
    struct BNode *temp = newNode(key, msg);

    if (node == NULL)
    {
        node = temp;
        temp = 0;
    }
    else
    {
        struct BNode *previous = NULL;
        struct BNode *current = node;
        while (current != NULL)
        {
            if (current->key == temp->key)
            {
                printf("Data Already Exist\n");
                return node;
            }
            previous = current;
            if (current->key > temp->key)
                current = current->left;
            else if (current->key < temp->key)
                current = current->right;
        }
        if (previous->key > temp->key)
            previous->left = temp;
        else
            previous->right = temp;
        temp = 0;
        current = 0;
        previous = 0;
        free(temp);
        free(current);
        free(previous);
    }
    int balance = get_Balancefactor(node);

    //left left case
    if (balance > 1 && key < node->left->key)
        return rightRotation(node);

    //right right case
    if (balance < -1 && key > node->right->key)
        return leftRotation(node);

    //left right case
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotation(node->left);
        return rightRotation(node);
    }

    //right left case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotation(node->right);
        return leftRotation(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

void InOrder_Traversal(struct BNode *root)
{
    struct BNode *current = root;
    Inorder_Print(current);
    printf("\n");
}

void Inorder_Print(struct BNode *node)
{
    if (node)
    {
        Inorder_Print(node->left);
        puts(node->message);
        Inorder_Print(node->right);
    }
}

void DeleteAll(struct BNode *node)
{
    if (node)
    {
        DeleteAll(node->left);
        DeleteAll(node->right);
        node->left = 0;
        node->right = 0;
        free(node);
    }
}

bool IsKey(int key, struct BNode *root)
{
    if (!root)
        return false;
    struct BNode *current = root;
    while (current != 0)
    {
        if (current->key == key)
            return true;
        if (current->key > key)
            current = current->left;
        else if (current->key < key)
            current = current->right;
    }
    current = 0;
    free(current);
    return false;
}

struct BNode *root = NULL;
sem_t mutex, write_msg, sem3;
int readers_count = 0;

//Readers - Writer Problem Implementation

//Writer Execution
void *Writer(void *ID)
{
    //do
   // {
        sleep(1);
        int writer_id = (int)ID;
        sem_wait(&write_msg);
        int key = 0;
        char msg[100];
        int choice;
        do
        {
            printf("\n\nWriter NUMBER #%d:", writer_id);
            printf("\n\n1.Write a message in a Tree.");
            printf("\n\n2.Exit.");

            printf("\n\nEnter Your Choice: ");
            scanf("%d", &choice);
            if (choice <= 0 || choice > 2)
            {
                printf("\n\nRedo...Enter any key");
                getch();
            }
        } while (choice <= 0 || choice > 2);

        if (choice == 1)
        {
            do
            {
                printf("\nEnter a key: ");
                scanf("%d", &key);
                fflush(stdin);
                if (IsKey(key, root))
                {
                    printf("\nKey already exist...Press any key to Redo\n");
                    getch();
                }
            } while (IsKey(key, root));
            printf("\nEnter a message: ");
            //fgets(msg, sizeof(msg), stdin);
            scanf("%s", msg);
            if (strcmp(msg, "\n") == 0)
            {
                printf("\nMessage Error: Proper Message Require\n");
                exit(0);
            }
            root = Insert(root, key, msg);
        }
        else if (choice == 2)
        {
            printf("\n\nWriter NUMBER %d Exit.", writer_id);
            pthread_cancel(pthread_self());
        }
        sem_post(&write_msg);
    //} while (1);
}

//Reader Execution
void *Reader(void *ID)
{
fflush(stdin);
    //do
    sleep(2);
    if(sem_wait(&write_msg)==0){
        
        int reader_id = (int)ID;
        if (root == NULL)
            //sleep(1);
            
        sem_wait(&mutex);
        readers_count += 1;
       /* if (readers_count == 1)
            sem_wait(&write_msg);*/
        sem_post(&mutex);

        //Critical Section
	sem_wait(&sem3);
        int choice=0;
        do
        {
            printf("\n\nReader NUMBER #%d:", reader_id);
            printf("\n\n1.Read a message from a Tree.");
            printf("\n\n2.Exit.");

            printf("\n\nEnter Your Choice:");
            fflush(stdin);
            scanf("%d", &choice);
            if (choice <= 0 || choice > 2)
            {
                printf("\n\nRedo...Enter any key");
                getch();
            }
        } while (choice <= 0 || choice > 2);
	sem_post(&sem3);

        if (choice == 1)
        {
            InOrder_Traversal(root);
        }
        else if (choice == 2)
        {
            printf("\n\nReader NUMBER %d Exit.", reader_id);
            pthread_cancel(pthread_self());
        }
        //End of Critical Section

        sem_wait(&mutex);
        readers_count -= 1;
        if (readers_count == 0)
            sem_post(&write_msg);
        sem_post(&mutex);
        
       sem_post(&write_msg);
    }// while (1);
    //sem_post(write_msg);
}

int main()
{
    int i = 0, readers = 0, writers = 0;
    sem_init(&mutex, 0, 1);
    sem_init(&write_msg, 0, 1);
    sem_init(&sem3, 0, 1);
    pthread_t Readers[100], Writers[100];
    printf("\n\nEnter number of Readers - MAX(100): ");
    if (scanf("%d", &readers) < 1 || readers > 100)
    {
        printf("\nOut of Bound");
        exit(0);
    }
    printf("\n\nEnter number of Writers - MAX(100): ");
    if (scanf("%d", &writers) < 1 || writers > 100)
    {
        printf("\nOut of Bound");
        exit(0);
    }

    for (i = 0; i < writers; i++)
        pthread_create(&Writers[i], NULL, Writer, (void *)i);
        
    for (i = 0; i < readers; i++)
        pthread_create(&Readers[i], NULL, Reader, (void *)i);
    for (i = 0; i < writers; i++)
        pthread_join(Writers[i], NULL);
    for (i = 0; i < readers; i++)
        pthread_join(Readers[i], NULL);
    sem_destroy(&mutex);
    sem_destroy(&write_msg);
    sem_destroy(&sem3);
    printf("\n\n");
    return 0;
}
/*
int main() {
    struct BNode *root = NULL;
    root = Insert(root, 10, "A");
    root = Insert(root, 20, "B");
    root = Insert(root, 30, "C");
    InOrder_Traversal(root);
    return 0;
}
*/
