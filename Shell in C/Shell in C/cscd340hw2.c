#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shellUtil.h"
#include "linkedList.h"

#define MAX 100
void parse(char * s, LinkedList * history, FILE * fin, int HISTCOUNT, LinkedList * aliases, int hisFlag);
FILE * loadHistoryFromFile(LinkedList * history);
void createHistory(char * s, int count, LinkedList * history);
void checkS(char * s, LinkedList * history, int HISTCOUNT, LinkedList * aliases, FILE * fin, int background);
void readBatchFile(int * HISTCOUNT, int * HISTFILECOUNT, LinkedList * history, FILE * fin, LinkedList * aliases);

//----------------------------------------------------------------------------------------------------------

int main()
{

	//possibly provide a check that hist count isnt bigger than histFILECOUNT!!
	int * HISTCOUNT = (int*)calloc(1, sizeof(int));
	int * HISTFILECOUNT = (int*)calloc(1, sizeof(int));
	
	FILE * fin = NULL;
	LinkedList * history = linkedList();
	LinkedList * aliases = linkedList();

	char s[MAX];

	fin = loadHistoryFromFile(history);

	//read .mybashrc
	readBatchFile(HISTCOUNT, HISTFILECOUNT, history, fin, aliases);
	
	int HISTCOUNTINT = *HISTCOUNT;
	int HISTFILECOUNTINT = *HISTFILECOUNT;
	
	free(HISTCOUNT);
	free(HISTFILECOUNT);

	printf("?: ");
	fgets(s, MAX, stdin);
	
	strip(s);	
 	while(strcmp(s, "exit") != 0)
  	{
		parse(s, history, fin, HISTCOUNTINT, aliases, 1);
		printf("?: ");
		fgets(s, MAX, stdin);
		strip(s);
	}// end while

	// Writes to History file
	writeFile(fin, history, HISTFILECOUNTINT);
	// Clears data from History Linked List
	if(history != NULL)
		clearList(history);
	if(aliases != NULL)
		clearAList(aliases);


	return 0;
}// end main

//----------------------------------------------------------------------------------------------------------

FILE * loadHistoryFromFile(LinkedList * history){

	FILE * fin;
	int count = 0;

	// Load History into linked list
	fin = openFile();
	count = findCount(fin);
	processFile(fin, count, history);

	rewind(fin);
	return fin;
	
}//end loadHistoryFrom File

//----------------------------------------------------------------------------------------------------------

void parse(char * s, LinkedList * history, FILE * fin, int HISTCOUNT, LinkedList * aliases, int hisFlag){


	char * historyCheck;
	int commandCount = 0;
	int myFlag = 0;
	
		int flag = 0;
		// Add command to history

		strip(s);
		s = trimWhiteSpace(s);

		char * andSign = strstr(s, "&");
		if(andSign != NULL)
		{
			char * entry;
			char copy[100];
			char * saveptr;
			strcpy(copy, s);

			int numberOfAnds = 0;

			int i;
			for (i = 0; copy[i] != '\0'; i++) {
      				if (copy[i] == '&')
         				numberOfAnds ++;
   			}
			
			entry = strtok_r(copy, "&",&saveptr);
			entry = trimWhiteSpace(entry);

			checkS(entry, history, HISTCOUNT, aliases, fin, 1);
			numberOfAnds --;	
			myFlag = 1;

			while(entry != NULL){

				int background = 0;
				if(numberOfAnds > 0)
				{

					numberOfAnds --;
					background = 1;
				}	

				entry = strtok_r(NULL, "&", &saveptr);			
				if(entry != NULL) entry = trimWhiteSpace(entry);
	
				if(entry != NULL)
				checkS(entry, history, HISTCOUNT, aliases, fin, background);
			}
		}

		if(hisFlag == 1)
			createHistory(s, commandCount, history);
	
		if(strcmp(s, "") == 0){
			myFlag = 1;
		}

		//do the rest =)		
		if(myFlag == 0)
		 	checkS(s, history, HISTCOUNT, aliases, fin, 0);
	
}//parse

//----------------------------------------------------------------------------------------------------------	

void checkS(char * s, LinkedList * history, int HISTCOUNT, LinkedList * aliases, FILE * fin, int background){

	char ** argv, ** pargv1, ** pargv2, * out = ">", * in = "<", * pipe = "|", * dir = NULL, * incheck = NULL, * outcheck = NULL, * pipecheck = NULL, * temp = NULL, * token = NULL;
	int argc = 0, stdinCpy = dup(0), stdoutCpy = dup(1), fd = -1, fd2 = -1, count = -1, num = 0, flag = 0, skipFlag = 0;

		char * alii = returnCommandIfExists(s, aliases);
		if(alii != NULL)
		{
			s = returnCommandIfExists(s,aliases);
		}
		
		// Handle !! comand
		if(strcmp(s, "!!") == 0)
		{
			if(history->size > 1){
				temp = (char *)findOneBeforeLast(history);
				strcpy(s, temp);
				strip(s);		
			}
			else
			{
				skipFlag = 1;
			}
		}

		// Handle history command !##
		if(isValidHistory(s) == 0 && skipFlag == 0)
		{
			token = strtok(s, "!");
			num = stringToInt(token);
			temp = (char *)findItem(num, history);
	
			if(temp == NULL)
			{
				printf("?: event not found\n");
				printf("?: ");
				fgets(s, MAX, stdin);
				strip(s);
			}
			else
				strcpy(s, temp);
			strip(s);
		}

		if(isValidAlias(s, aliases) == 0 && skipFlag == 0)
		{	
			skipFlag = 1;

		}
		if(isValidUnAlias(s, aliases) == 0 && skipFlag == 0)
		{
			skipFlag = 1;				
		}

 		// Handle history command
	        if(strcmp(s, "history") == 0 && skipFlag == 0)
		{
			printList(history, HISTCOUNT);
		}
		if(strcmp(s, "alias") == 0 && skipFlag ==0)
		{

			printAliasList(aliases);
			skipFlag = 1;
		}
		
		if(checkPathExtention(s) == 0 && skipFlag ==0)
		{

			skipFlag =1;
		}
		if(checkEchoPath(s) == 0 && skipFlag == 0)
		{
			skipFlag = 1;

		}

		// Remove carriage return
		strip(s);
			
		// Check for input redirection
		incheck = strchr(s, *out);
		// Check for output redirection
		outcheck = strchr(s, *in);
		// Handle output redirection
		if(incheck && skipFlag == 0)
		{
			fd = outputRedirect(s);
			char * saveptr;
			trimWhiteSpace(s);

			if(fd == -1)
			{
				printf("Error: Invalid Command\n");
				flag = 1;
			}
		}
	
		// Handle input redirection
		if(outcheck && skipFlag == 0)
		{
			fd2 = inputRedirect(s);
			
			if(fd2 == -1)
			{
				printf("Error: Invalid Command\n");
				flag = 1;
			}
		}

		// Check for pipe command
		pipecheck = strchr(s, *pipe);
		// Handles Pipe Commands while not allowing redirects
		if(pipecheck && flag == 0 && /*!outcheck && !incheck &&*/ skipFlag == 0)
		{

			int result = isValidPipe(s);
			// Handle missing token after pipe
			
			if(result == -1)
				printf("Error: Invalid Command\n");
			// Handle Pipe Command
			if(result == 2)
			{

				pipeSides(s, &pargv1, &pargv2, aliases);
				pipeIt(pargv1, pargv2, history, aliases, fin);
				clean(pargv1);
				clean(pargv2);
				pargv1 = NULL;
				pargv2 = NULL;
			}
			if(result==3)
				printf("Sorry only double pipe accounted for!\n");
		
				flag = 1;
		}
		alii = returnCommandIfExists(s, aliases);
		if(alii != NULL)
		{
			s = returnCommandIfExists(s,aliases);
		}
		// Handles Directory Change Command
		if(strstr(s, "cd") != NULL && skipFlag == 0)
		{
			if(dir = strtok(s, " "));
			if(dir = strtok(NULL, " "))
			{
				trimWhiteSpace(dir);
				chdir(dir);
				flag = 1;
			}
		}
		// Handles making arguments for any single command
		if(flag == 0 && skipFlag == 0){
		    	argc = makeargs(s, &argv);
			
		}
	    	// Handles Executing and Cleaning any single command
	    	if (argc > 0 && flag == 0 && skipFlag == 0)
	    	{
		    //printargs(argc, argv);// REMOVE
		    if(strcmp(s, "history") != 0)
		    	forkIt(argv, history, aliases, fin, background);
		    			
			clean(/*argc,*/ argv);
		}

		// Handles closing stdout
		if(fd > 0 && skipFlag == 0)
	    	{
		    close(fd);
		    dup2(stdoutCpy, 1);
		}
		// Handles closing stdin
		if(fd2 > 0 && skipFlag == 0)
		{
			close(fd2);
			dup2(stdinCpy, 0);
		}
		// Prints error if makeargs returns error
		if (argc == -1 && skipFlag == 0)
	    		printf("Error: Invalid Command\n");
}

//----------------------------------------------------------------------------------------------------------

void readBatchFile(int * HISTCOUNT, int * HISTFILECOUNT, LinkedList * history, FILE * fin, LinkedList * aliases)
{
	FILE * fin1 = NULL;
	fin1 = fopen(".myshrc","r");
	
	if(fin1 == NULL)
	{
		printf("Could not find the .myshrc batch file, processing without it...\n");		
		return;
	}

	int count = countRecords(fin1, 1);

	int x;
	char temp[100];

	int flagNo = 0;	

	for(x= 0; x<count; x++)
	{
	
		fgets(temp, MAX, fin1);
		strip(temp);

		char * entry;
		char copy[100];
		char * saveptr;

		if(strlen(temp) > 10)
		{
				
			if(temp[0] == 'H' && temp[1] == 'I' && temp[2] == 'S' && temp[3] == 'T' && temp[4] == 'C' && temp[5] == 'O' && temp[6] == 'U' && temp[7] == 'N' && temp[8] == 'T' && temp[9] == '='){

				strcpy(copy, temp);
				entry = strtok_r(copy, "=",&saveptr);
				
				entry = strtok_r(NULL," ",&saveptr);
				*HISTCOUNT = atoi(entry); 
				
				flagNo = 1;			
			}
		}
		if(strlen(temp) > 14)
		{	

			if(temp[0] == 'H' && temp[1] == 'I' && temp[2] == 'S' && temp[3] == 'T' && temp[4] == 'F' && temp[5] == 'I' && temp[6] == 'L' && temp[7] == 'E' && temp[8] == 'C' && temp[9] == 'O' && temp[10] == 'U' && temp[11] == 'N' && temp[12] == 'T' && temp[13] == '='){

				strcpy(copy, temp);
				entry = strtok_r(copy, "=",&saveptr);
				
				entry = strtok_r(NULL," ",&saveptr);
				*HISTFILECOUNT = atoi(entry); 
				flagNo = 1;						
			}
		}

		if(flagNo == 0)
			parse(temp, history, fin, *HISTCOUNT, aliases, 0); //0 for dont create history
		flagNo = 0;
	}

	fclose(fin1);
	fin1 = NULL;

}//end readBatch

//----------------------------------------------------------------------------------------------------------
