#include "shellUtil.h"


//----------------------------------------------------------------------------------------------------------

/*
 * Opens History File .mysh_history
 */
FILE * openFile()
{
	FILE * fin = NULL;
	fin = fopen(".mysh_history","a+");
	return fin;
}// End openFile

//----------------------------------------------------------------------------------------------------------

/*
 * Counts # of History Commands in History File
 * Used for finding last 200 commands in File.
 */
int findCount(FILE * fin)// Finds word count
{
	int x, count = 0;

	if (!fin) 
		return -1;
	else
	{
		while ( (x = fgetc(fin)) != EOF)// Counts each line in file
     
 		if (x == '\n' || x == '\r')// if x reaches a carriage return or new line feed
 	        	++count;
 	}
         return count;
}// End findCount

//----------------------------------------------------------------------------------------------------------

/*
 * Converts a given string to an integer.
 */
int stringToInt(char * s)
{
	int result = 0, negative = 1, x = 0;

			for(; x < strlen(s); x++){
				if(s[x] == '-'){
					negative = -1;
					x = 1;
				}
				result *= 10; // Add trailing zero
				result += (s[x] - '0');// Give int value
			}


			return result * negative;
}// End stringToInt

//----------------------------------------------------------------------------------------------------------

/*
 * Adds the last 200 History Commands from the History File into a Linked List.
 */
void processFile(FILE * fin, int count, LinkedList * historyList)//struct Node ** head
{

	char temp[MAX];
	int last = 0, x = 0;
	int count2 = 0;
	
	if(count == 0)
		return;

	if(count > 200)
		last = count - 200;
	
	if(fin == NULL)
		openFile();
	else
	{
		rewind(fin);
		while(!feof(fin))
		{
			count2 ++;
			fgets(temp, MAX, fin);
			x++;	
			
			strip(temp);

			if(x > last && count > 200 || count < 201)
				createHistory(temp, count2, historyList);  
		}//while
	}//else
}// End processFile

//----------------------------------------------------------------------------------------------------------

/*
 * Writes to History File upon exiting shell.
 */
void writeFile(FILE * fin, LinkedList * myList, int HISTFILECOUNT)//struct Node ** head
{
	int writeCount = 1;
	commands * com;

	Node * curr = myList->head;

	if(myList->size == 0)
		return;

	if(myList == NULL)
		return;
	else 
	{
		fclose(fin);
		fin = NULL;
		fin = fopen(".mysh_history","w+");
		Node * cur = curr;

		if(HISTFILECOUNT < myList->size){	

				int move = (myList->size) - HISTFILECOUNT;
			
				int x;
				for(x = 0; x < move; x ++){
					cur = cur -> next;
				}
		}


		while(cur != NULL)
		{	

			if(writeCount < 200){
	
				com  = cur -> data;
				char * temp = com -> theCommand;
				fprintf(fin, "%s\n", temp);
			}
			
			cur = cur->next;
			writeCount++;
		}
	}

	fclose(fin);
	fin = NULL;
}// End writeFile

//----------------------------------------------------------------------------------------------------------

/*
 * If a command is longer than 100 characters,
 * this removes the excess characters and returns and Invalid Command Error.
 */
void clearBuffer(void) 
{
    int ch;

    while ((ch = getchar()) != '\n') 
    {
        if (ch < 0)
            exit(EXIT_FAILURE);
    }
}// End clearBuffer

//----------------------------------------------------------------------------------------------------------

/*
 * Strips the command of any carriage return.
 */
void strip(char *s)
{
  int len;
  len = strlen(s);
  if(s[len - 2] == '\r')
    s[len -2] = '\0';

  else if(s[len - 1] == '\n')
    s[len -1] = '\0';
}// end strip

//----------------------------------------------------------------------------------------------------------

/*
 * Trims the leading and trailing white space from a string.
 */
char * trimWhiteSpace(char * word)
{
  	char *end;

  	while(isspace(*word))
  		word++;

  	if(*word == 0)
    		return word;

  	end = word + strlen(word) - 1;

  	while(end > word && isspace(*end)) end--;
  		*(end+1) = 0;

  return word;
}// End trimWhiteSpace

//----------------------------------------------------------------------------------------------------------

/*
 * Frees the passed in allocated array.
 */
void clean(char **argv)
{

  int x;
  for(x = 0; argv[x] != '\0'; x++)
  {
    	free(argv[x]);
    	argv[x] = NULL;
  }
    free(argv[x]);
    free(argv);

}// end clean

//----------------------------------------------------------------------------------------------------------

/*
 * Checks if the command contains valid operators.
 * Returns 1 when valid and -1 when not valid.
 */
int isValidOperator(char * s)
{
	int x, flag;
	for(x = 0; x < strlen(s); x++)
	{
		if (s[x] =='!' || s[x] == '<' || s[x] == '>' || s[x] == '|')
			flag = 0;
		
		else
			flag = -1;
	}
	if(flag == 0)
		return 0;
	else 
		return -1;
}// End isValidOperator

//----------------------------------------------------------------------------------------------------------

/*
 * Checks if the !# history command contains the correct syntax.
 * Returns -1 upon invalid syntax.
 */
int isValidHistory(char * s)
{
	int x, flag;
	if(s[0] != '!')
		return -1;
	else
	{
		for(x = 1; x < strlen(s); x++)
		{
			if(isdigit(s[x]))
				flag = 0;
			else
				return -1;
		}
	}
	return flag;
}

//----------------------------------------------------------------------------------------------------------

/*
 * Checks if the command is a valid pipe command.
 * Returns -1 upon invalid command.
 */
int isValidPipe(char * s)
{
	char temp [MAX];
	strcpy(temp, s);
	char * token1, * token2, * token3, * check, * pipe = "|";
	int x, count = 0;

	for(x = 0; x < strlen(s); x++)
	{
		if(s[x] == '|')
			count++;
	}

	if((token1 = strtok(temp, pipe)) != NULL);
	else
		return -1;

	if((token2 = strtok(NULL, pipe)) != NULL);
	else
		return -1;

	if((token3 = strtok(NULL, pipe)) != NULL)
	{	
		if(token3 != NULL && count > 2)
			return -1;

		else
			return 3;
	}	
	if(token3 == NULL && count == 2)
		return -1;

	else
		return 2;
}// End isValidPipe

//----------------------------------------------------------------------------------------------------------

/*
 * Checks command if it contains legal characters.
 * Returns -1 upon invalid character.
 */
int isValidCharacter(char * s)
{
	int x, flag;
	for(x = 0; x < strlen(s); x++)
	{
		if(isalpha(s[x]) || isspace(s[x]) || isdigit(s[x]) || s[x] == '<' || s[x] == '>' || s[x] == '|' || s[x] == '!' || s[x] == '-' || s[x] == '.' || s[x] == '/' || s[x] == '_' || s[x] == '\n' || s[x] == '\r')
		flag = 0;
	else
		return -1;
	}
	return flag;
}


//----------------------------------------------------------------------------------------------------------

/*
 * Checks if the command contains valid operators.
 * Returns 1 when valid and -1 when not valid.
 */
int isValidWord(char * s)
{
	int x, flag;
	for(x = 0; x < strlen(s); x++)
	{
		if(isalpha(s[x]) || isspace(s[x]) || isdigit(s[x]) || s[x] == '!' || s[x] == '-' || s[x] == '.' || s[x] == '/' || s[x] == '_')
			flag = 0;
	else
		return -1;
	}
	return flag;
}// End isValidWord


//----------------------------------------------------------------------------------------------------------

/*
 * Checks if the first word in the command is valid.
 * Returns -1 upon invalid word.
 */
int isValidFirstWord(char * s)
{
	char temp [MAX];
	strcpy(temp, s);
	char * token;
	token = strtok(temp, " ");

	if(isValidWord(token) == 0)
		return 0;
	else 
		return -1;
}// End isValidFirstWord

//----------------------------------------------------------------------------------------------------------

/*
 * Breaks the command down into executable arguments.
 */
int makeargs(char *s, char *** argv)
{
	int flagAnd = 0;
	char * entry;
	char copy[100];
	char * saveptr, saveptr2;
	strcpy(copy, s);

	int count,x;

	count = getCount(s);

	if(count < 1)
		return -1;
	
	entry = strtok_r(copy, " ", &saveptr);	
	*argv = (char**)calloc(count + 1, sizeof(char*));

	for(x = 0; x < count; x ++){
			
		strip(entry);

		(*argv)[x] = (char*)calloc(strlen(entry)+1, sizeof(char));
		strcpy((*argv)[x], entry);
     		entry = strtok_r(NULL," ", &saveptr);

	}//end for

	 (*argv)[count] = '\0';
	
   	return count;	

}// end makeArgs

//----------------------------------------------------------------------------------------------------------

/*
 * Fork and Execs single commands.
 */
void forkIt(char ** argv, LinkedList * history, LinkedList * aliases, FILE * fin, int background)
{

	int c = 0, count = 0;
	int status, exstat;
	pid_t pid;		


	//KILL ZOMBIES!!
	int reapedPid;
	do {
    		reapedPid = waitpid(-1,NULL,WNOHANG);
	} while (reapedPid > 0);


	pid = fork();
	if(pid < 0)
	{
   		printf("Error: Forking Failed.\n");
   		exit(-1);
	}
	if(pid == 0)
	{
  		exstat = execvp(*argv, argv);
  		if(exstat < 0)
  		{
			while(argv[c] !=NULL){
				count++;
				c++;
			}
			
			clean(argv);

			clearList(history);
			clearAList(aliases);
			fclose(fin);
			fin == NULL;
    			printf("Error: Exec Failed\n");
    			exit(-1);
  		}

	}
	else if( background == 0)
		 waitpid(pid, &status, 0);


}// end forkIt


//----------------------------------------------------------------------------------------------------------

/*
 * Handles Output Redirection to file.
 */
int outputRedirect(char * s)
{
	int fd;
	char * filename, * token;

	int i;
	int num = 0;
	for (i = 0; s[i] != '\0'; i++) {
      		if (s[i] == '>')
         		num ++;
   	}	


	if((token = strtok(s, ">")) != NULL);
	else
		return -1;
	
	if((filename = strtok(NULL, ">")) != NULL);
	else
		return -1;

	filename = trimWhiteSpace(filename);
	
	
	if(num == 1){
		
		FILE * fin = fopen(filename, "w+");
		fclose(fin);
		fin = NULL;
		fd = open(filename, O_CREAT | O_WRONLY);//O_RDWR | O_CREAT, S_IRUSR | S_IWUSR
	}
	
	if(num == 2)
		fd = open(filename, O_WRONLY|O_APPEND);

	if(fd < 0)
	{
		perror("Failed to open file");
		return -1;
	}
	else
		dup2(fd, 1);// stdout

	return fd;
}// End outputRedirect

//----------------------------------------------------------------------------------------------------------

/*
 * Handles Input Redirection from file.
 */
int inputRedirect(char * s)
{
	int fd;
	char * filename, * token;


	if((token = strtok(s, "<")) != NULL);
		else
			return -1;
	if((filename = strtok(NULL, "<")) != NULL);
		else
			return -1;

	filename = trimWhiteSpace(filename);

	fd = open(filename, O_RDONLY);

	if(fd < 0)
	{
		perror("Failed to open file");
		return -1;
	}
	else
		dup2(fd, 0);// stdin

return fd;
}// End inputRedirect

//----------------------------------------------------------------------------------------------------------

/*
 * Handles single pipe command.
*/
void clear(int argc, char **argv)
{
	int i = 0;
	for(i; i<argc + 1; i++)
		free(argv[i]);	
	free(argv);
}// end clear


//----------------------------------------------------------------------------------------------------------

void pipeIt(char ** prePipe, char ** postPipe, LinkedList * historyList, LinkedList * aliases, FILE * fin)
{

	int count = 0, c = 0;

	int fd[2], exstat, status;
	pid_t pid, pid2;

	if (pipe(fd) != 0) 
	{
	   perror("Pipe Failed");
	   exit(-1);
	}

	if((pid = fork()) == -1)
	{
	   perror("Forking Failed");
	   exit(-1); 
	}

	if(pid == 0)
	{
		// Child
		close(fd[0]);
		close(1);
		dup(fd[1]);
		close(fd[1]);		

		exstat = execvp(prePipe[0], prePipe);


		if(exstat < 0)
		{
			
			clean(prePipe);
			clean(postPipe);
			
			clearList(historyList);
			clearAList(aliases);
			fclose(fin);
			fin =NULL;

			perror("Exec Failed for prePipe Command\n");
			exit(-1);
		}
	}// end Child

	else
	{
		if((pid2 = fork()) == -1) //going into grandchild
		{
			perror("Double Forking Failed");
			exit(-1);
		}
		if(pid2 == 0)
		{
			// Grandchild
			close(fd[1]);
			close(0);
			dup(fd[0]);
			close(fd[0]);

		    	exstat = execvp(postPipe[0], postPipe);

			if(exstat < 0)
			{
			
				clean(postPipe);

				clean(prePipe);				

				clearList(historyList);
				clearAList(aliases);
				fclose(fin);
				fin = NULL;
				perror("Exec Failed for postPipe Command\n");
				exit(-1);
			}

		}// end Grandchild

		else
		{
			// Parent
			close(fd[0]);
			close(fd[1]);

			// Wait for Child and Grandchild to finish.
			waitpid(pid, &status, 0);
			waitpid(pid2, &status, 0);

		}// end Parent

	}// end else

}// end pipeIt

//----------------------------------------------------------------------------------------------------------

void createHistory(char * s, int count, LinkedList * history){
	
	if(strcmp(s, "") == 0){
		return;
	}

	char * temp = ((char*)findLast(history));	
	
	if(temp != NULL){
		if(strcmp(s, temp) == 0)
			return; 
	}	

	int i = strlen(s);

	commands * command = calloc(1, sizeof(commands));
	command -> num = history->size + 1;
	char * charptr = (char *)calloc(i + 1,sizeof(char));
	strcpy(charptr, s);	
	command -> theCommand = charptr;
	Node * nn = buildNode(command, history->size + 1);

	commands * com = nn->data;
	addLast(history, nn);
	
}//end makeHistory

//----------------------------------------------------------------------------------------------------------

int getCount(char * s){

	
	char * entry;
	char copy[100];
	char * saveptr;
	strcpy(copy, s);
	entry = strtok_r(copy, " ",&saveptr);

	int count,x;

	for(count = 0; entry != NULL; count ++)
	{
		entry = strtok_r(NULL," ",&saveptr); 
	}//end for

	return count;

}//end getCount

//----------------------------------------------------------------------------------------------------------

void pipeSides(char * s, char *** prePipe, char *** postPipe, LinkedList * aliases)
{
	char * token;

	token = strtok(s,"|");

	int count1 = getCount(token);
	
	char * l = "";

	*prePipe = (char**)calloc(count1 + 1, sizeof(char*));
	
	strip(token);
	token = trimWhiteSpace(token);

	char * alii = returnCommandIfExists(token, aliases);
	if(alii != NULL)
	{
		token = returnCommandIfExists(token,aliases);
	}


	makePipeArgs(*prePipe, token, count1);

	token = strtok(NULL, "|");
			
	int count2 = getCount(token);

	*postPipe = (char**)calloc(count2 + 1, sizeof(char*));
	
	strip(token);
	token = trimWhiteSpace(token);

	alii = returnCommandIfExists(token, aliases);
	if(alii != NULL)
	{
		token = returnCommandIfExists(token,aliases);
	}	


	makePipeArgs(*postPipe, token, count2);

}// end pipeSides

//----------------------------------------------------------------------------------------------------------

int makePipeArgs(char ** pipeSide, char * whatGoes, int count)
{
	char * entry;
	char copy[100];
	char * saveptr;;
	char copy2[100];	
	int x;	

	strcpy(copy, whatGoes);

	if(count < 1)
		return -1;

	entry = strtok_r(copy, " ", &saveptr);
	for(x = 0; x < count; x ++){

		strip(entry);
		pipeSide[x] = (char*)calloc(strlen(entry)+1, sizeof(char));
	     	strcpy(pipeSide[x], entry);
     		entry = strtok_r(NULL," ", &saveptr);
	}//end for

	 pipeSide[count] = '\0';
   	return count;	

}// end makeArgs

//----------------------------------------------------------------------------------------------------------

int isValidAlias(char * s, LinkedList * aliases){

	int valueToReturn = -1;

	if(strlen(s) < 6)
		return -1;

	if(s[0] == 'a' && s[1] == 'l' && s[2] == 'i' && s[3] == 'a' && s[4] == 's' && s[5] == ' ')
	{

		valueToReturn = 0;
		int skipFlag = 0;
		alias * ali = calloc(1, sizeof(alias));
		char temp[MAX];
		char * entry, *saveptr;
		strcpy(temp, s);
		
		entry = strtok_r(temp, " ", &saveptr);	
		entry = strtok_r(NULL, "=", &saveptr);

		if(entry == NULL)
		{
			free(ali);
			return 0;
		}			

		ali -> key = (char*)calloc(strlen(entry) + 1, sizeof(char));
		strcpy(ali->key, entry);

		entry = strtok_r(NULL, "", &saveptr);
		
		if(entry == NULL)
		{
			free(ali->key);
			free(ali);

			printf("Command Not Found!\n");
			return 0;

		}

		int l = strlen(entry);
		if(l > 2 && (entry[0] == '\'' || entry[0] == '"') && (entry[l - 1] == '\'' || entry[l-1] == '"'))
		{	
			char *end;
  			
  			end = entry + strlen(entry) - 1;
			entry++;/////////
			*(end) = 0;
		}	
		else
		{
			skipFlag =1;
			printf("Command not Found\n");
		}

		if(skipFlag == 0)
		{
			ali -> command = (char*)calloc(strlen(entry) + 1, sizeof(char));
			strcpy(ali->command, entry);
		//add check if there is already this alias... maybe...
			removeCommandIfExists(ali->key, aliases, 0);				
	
			Node * newNode = buildAliasNode(ali);
			addLast(aliases, newNode);
			return 0;
			//add termination character!~~!~~
		}
		else
		{//if skipFlag is on...

			free(ali -> key);
			free(ali);
		}
	}

	fflush(stdout);
	return valueToReturn;

}//end isValidAlias

//----------------------------------------------------------------------------------------------------------

int isValidUnAlias(char * s, LinkedList * aliases){

	if(strlen(s) < 8)
		return -1;

	char * entry, *saveptr;
	char temp[MAX];
	
	strcpy(temp, s);

	if(s[0] == 'u' && s[1] == 'n' && s[2] == 'a' && s[3] == 'l' && s[4] == 'i' && s[5] == 'a' && s[6] == 's' && s[7] == ' '){

		entry = strtok_r(temp, " ", &saveptr);
		entry = strtok_r(NULL,"",&saveptr);
		removeCommandIfExists(entry, aliases, 1);	
		return 0;
	}

	fflush(stdout);
	return -1;



}//end isValidUnAlias

//----------------------------------------------------------------------------------------------------------

int checkPathExtention(char * s){


	if(strlen(s) < 11)
		return -1;

	char * yo, *input;
	char temp[MAX];
	strcpy(temp, s);

	input = strtok_r(temp, ":", &yo); 

	if(strcmp(input, "PATH=$PATH") != 0)
		return -1;

	

	char i[1000];

	input = strtok_r(NULL, "", &yo);




	char * env = getenv("PATH");

	strcpy(i, env);
	strcat(i,":");
	strcat(i, input);
	setenv("PATH", i,1);//1 to overwrite	

	return 0;
}

//----------------------------------------------------------------------------------------------------------

int checkEchoPath(char * s){

	char * yo, *input;
	char temp[MAX];
	strcpy(temp, s);

	input = strtok_r(temp, " ", &yo);

	if(strcmp(input,"echo") != 0)
		return -1;


	input = strtok_r(NULL, "", &yo);

	if(strcmp(input,"$PATH") == 0){

		printf("%s\n", getenv("PATH"));	
	}
	
	return 0;

}

//----------------------------------------------------------------------------------------------------------

int countRecords(FILE * fin, int lines)
{
	int ch;
	int count = 0;

	do{
   		ch = fgetc(fin);
   		if( ch== '\n') count++;   
   	}while( ch != EOF );   

	if(count == 0)
		printf("ERROR - lines counted in file is 0");
	else{
		rewind(fin);
		return count/lines; 
	}


	
	return 0;

} //END COUNT RECORDS


//----------------------------------------------------------------------------------------------------------

