#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node
{
    void * data;
    struct node * next;
};
typedef struct node Node;


struct linkedlist
{
    Node * head;
    int size;
};
typedef struct linkedlist LinkedList;

struct com {
	
	int num;
	char * theCommand; 
};

typedef struct com commands;

struct ali {

	char * key;
	char * command;

};

typedef struct ali alias;


LinkedList * linkedList();

Node * buildNode(commands * command, int count);
Node * buildAliasNode(alias * ali);
void sort(LinkedList * theList);
void clearList(LinkedList * theList);
void clearAList(LinkedList * theList);
void printList(LinkedList * theList, int HISTCOUNT);
void printAliasList(LinkedList * theList);
void addLast(LinkedList * theList, Node * nn);
void addFirst(LinkedList * theList, Node * nn);
void removeItem(LinkedList * theList, Node * nn);
void * findLast(LinkedList *  history);
void * findOneBeforeLast(LinkedList * history);
void * findItem(int value, LinkedList * history);
void removeCommandIfExists(char * key, LinkedList * aliases, int neededCheck);
char * returnCommandIfExists(char * key, LinkedList * aliases);

#endif // LINKEDLIST_H
