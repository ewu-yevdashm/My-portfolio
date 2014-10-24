#ifndef SHELLUTIL_H
#define SHELLUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "linkedList.h"

#define MAX 100

void clearBuffer(void);
void strip(char * s);
char * trimWhiteSpace(char * word);
void clean( char **argv);
void printargs(int argc, char **argv);
int stringToInt(char * s);
int isValidCharacter(char * s);
int isValidOperator(char * s);
int isValidHistory(char * s);
int isValidWord(char * s);
int isValidFirstWord(char * s);
int isValidPipe(char * s);
int makeargs(char *s, char *** argv);
void forkIt(char ** argv, LinkedList * history, LinkedList * aliases, FILE * fin, int background);
void pipeIt(char ** prePipe, char ** postPipe, LinkedList * historyList, LinkedList * aliases, FILE * fin);
void processFile(FILE * fin,int count, LinkedList * myList);//struct Node ** head
void writeFile(FILE * fin, LinkedList * myList, int HISTFILECOUNT);//, struct Node * head
int findCount(FILE * fin);
FILE * openFile();
void createHistory(char * s, int count, LinkedList * history);
int getCount(char * s);
void pipeSides(char * s, char *** prePipe, char *** postPipe, LinkedList * aliases);
int makePipeArgs(char ** pipeSide, char * whatGoes, int count);
int isValidAlias(char * s, LinkedList * aliases);
int isValidUnAlias(char * s, LinkedList * aliases);
int checkPathExtention(char * s);
int checkEchoPath(char * s);
int countRecords(FILE * fin, int lines);
#endif
