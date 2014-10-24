#include "linkedList.h"


//----------------------------------------------------------------------------------------------------------
LinkedList * linkedList(){	

	LinkedList * linkedList;
	linkedList = calloc(1, sizeof(LinkedList));
	linkedList -> head = NULL;
	linkedList -> size = 0;

	return linkedList;
}

//----------------------------------------------------------------------------------------------------------
Node * buildNode(commands * command, int count){ 

	Node * curr = calloc(1, sizeof(Node));
	
	//char array[100];
	//int i = 0;
	//do{
	//	printf("\nPlease enter the word: ");
	//	scanf("%s", array);
	//	i = strlen(array);
	//}while(i == 0);

	//assigning data to node
	
	
	curr->data = command;	
	return curr;

}//end BuildNode

//----------------------------------------------------------------------------------------------------------

Node * buildAliasNode(alias * ali){

	Node * curr = calloc(1, sizeof(Node));
	curr -> data = ali;
	return curr;

}


//----------------------------------------------------------------------------------------------------------

void sort(LinkedList * theList){


	if(theList->size == 0 || theList->size == 1){}
	else{

	    Node * temp;
	    Node * after;
	    temp = theList -> head;
	    after = temp->next;
	    char * word;

	    while(after != NULL)
	    {
		   while(after != temp)
		   {
		   	if(strcmp((char*)after->data, (char*)temp->data) < 0)
		   	{
		        	word = temp->data;
		        	temp->data = after->data;
		        	after->data = word;
		    	}// End if
		        temp = temp->next;
		   }// End inner while
		    temp = theList -> head;
		    after = after->next;
	    }// End outer while
	}
}//end sort

//----------------------------------------------------------------------------------------------------------

void clearList(LinkedList * theList){


	if(theList -> size == 0){
		free(theList);
		return;
	}
 	
	
	Node * cur;
  	cur = theList -> head;
  	Node * after;
  	while(cur->next != NULL)
  	{
		commands * command = cur->data;
		free(command->theCommand);
		free(command);

		after = cur->next;
		//free(cur->data);
		free(cur);
		cur = after;
   	}
	
	commands * command = cur->data;
	free(command->theCommand);
	free(command);	
	
	//free(cur->data);
	free(cur);

	//theList -> head = NULL;
	theList->size = 0;

	free(theList);

}//end clearList

//----------------------------------------------------------------------------------------------------------

void clearAList(LinkedList * theList){


	if(theList -> size == 0){
		free(theList);
		theList = NULL;
		return;
	}


	Node * curr = theList->head;
	Node * after;

	while(curr->next != NULL)
	{
		alias * ali = curr->data;
		free(ali->key);
		free(ali->command);
		free(ali);
		
		after = curr->next;
		free(curr);
		curr = after;
	}

	alias * ali = curr->data;
	free(ali->key);
	free(ali->command);
	free(ali);
	free(curr);
	theList->size = 0;
	free(theList);

	return;

}//end clearList


//----------------------------------------------------------------------------------------------------------

void printList(LinkedList * theList, int HISTCOUNT){

	int move = 0;
	int x;

	Node * curr = theList->head;
	commands * command;

	if(theList -> size > HISTCOUNT)
		move = theList -> size - HISTCOUNT;


	if(HISTCOUNT == -1)
		move = 0;


	for(x = 0; x < move; x ++)
		curr = curr -> next;


	printf("\nYour History includes the following:\n");

	
	//If list is empty
	if(theList->size == 0){
		printf("\nNothin here yet :)\n");
	}
	else{
	//Else print the list
		while(curr != NULL){
			command = curr->data;
			printf("%d %s \n",command->num, command->theCommand);
			curr = curr->next;
		}
	}

	printf("\n");

}//end printList

//----------------------------------------------------------------------------------------------------------

void printAliasList(LinkedList * theList){

	Node * curr = theList->head;
	alias * ali;



	printf("\nYour aliases are:\n");

	
	//If list is empty
	if(theList->size == 0){
		printf("\nNothin here yet :)\n");
	}
	else{
	//Else print the list
		while(curr != NULL){
			ali = curr->data;
			printf("alias %s='%s'\n",(char*)ali->key, (char*)ali->command);
			curr = curr->next;
		}
	}

	printf("\n");

}//end printList

//----------------------------------------------------------------------------------------------------------

void addLast(LinkedList * theList, Node * nn){

	if(theList->size == 0){
	
		theList->head = nn;

	}//end if
	else{

	  	Node * curr = theList -> head;
		while(curr ->next != NULL)
		{
			curr = curr->next;
		}
		curr -> next = nn;
	}

	  theList -> size ++;

}//end add Last

//----------------------------------------------------------------------------------------------------------

void addFirst(LinkedList * theList, Node * nn){


	nn-> next = theList -> head;
	theList -> head = nn;
	theList -> size ++;
}//end addFirst

//----------------------------------------------------------------------------------------------------------

void removeItem(LinkedList * theList, Node * nn){

	int flag = 0;
   	 Node * prev, * cur, * temp;
 	cur = theList -> head;

	 if (strcmp((char*)nn->data, (char*)cur->data)==0) 
	 {
        	temp = cur;
        	cur = cur->next;
        	free(temp->data);
        	free(temp);
        	theList->size --;
        	flag = 1;
        	printf("Removed %s from the list!\n", (char*)nn->data);
    	}
    	else {
        	prev = theList->head;
        	cur = theList->head->next;
 
        while (cur != NULL && strcmp((char*)nn->data, (char*)cur->data) !=0 ) 
        {
            prev = cur;
            cur = cur->next;
        }
 
        if (cur != NULL) {
            temp = cur;
            prev->next = cur->next;
            free(temp->data);
            free(temp);
            theList->size --;
            flag = 1;
            printf("Removed %s from the list!\n", (char*)nn->data);
        }
    }
    if(flag == 0)
		printf("%s was not found in the list.\n", (char*)nn->data);



	free(nn->data);
	free(nn);


}//end Remove Item

//----------------------------------------------------------------------------------------------------------

/*
 * Find last element in list for !! command.
 */
void * findLast(LinkedList * history)
{
	commands * com;
	Node * head = history->head;

	

    	if(history-> size == 0)
        	return NULL;
    	if (head->next == NULL){
		com = head->data;
        	return com->theCommand;

	}

    else
    {
        Node * cur = head;
	Node * next = cur->next;
        while(cur->next != NULL)
            cur = cur->next;

	com = cur->data;
	
        return com->theCommand;
    }

}//end findLast

//----------------------------------------------------------------------------------------------------------

/*
 * Find last element in list for !! command.
 */
void * findOneBeforeLast(LinkedList * history)
{
	commands * com;
	Node * head = history->head;

    	if(history-> size == 0)
        	return NULL;
    	if (head->next == NULL){
		com = head->data;
        	return com->theCommand;

	}

    else
    {
        Node * cur = head;
	Node * next = cur->next;
        while(next->next != NULL){
            cur = cur->next;
	    next = next -> next;
	}
	com = cur->data;
	
	
        return com->theCommand;
    }

}//end findLast

//----------------------------------------------------------------------------------------------------------

/*
 * Searches the list for the specified command by command #.
 */
void * findItem(int value, LinkedList * history)
{
	Node * temp;
	temp = history -> head;
    	int count = 1;
    	if(history -> head == NULL)
        	return;

	while(temp->next)
    	{
        	if(count == value)
        	{
			commands * com = temp ->data;
        	    	return com->theCommand;
        	}
        	temp = temp->next;
        	count++;
    	}
    return NULL;

}//end findItem

//----------------------------------------------------------------------------------------------------------

void removeCommandIfExists(char * key, LinkedList * aliases, int neededCheck)
{

	int done = 1;

	if(aliases->size ==0){

		if(neededCheck)
			printf("Command not Found!\n");
		return;
	}

	
	Node * curr = aliases -> head;
	alias * ali;
	Node * prev;	

	ali = curr->data;
	if(strcmp(ali->key, key)==0)
	{
		Node * next = NULL;		

		if(aliases->size > 1)
		{
			next = curr ->next;
		}
		done = 0;
		free(ali->key);
		free(ali->command);
		free(ali);
		free(curr);

		aliases->size --;
		curr = NULL;

		aliases->head = next;

	}
	else{
	
		

		for(curr; curr != NULL; curr = curr->next)
		{
			
			
			ali = curr->data;
			if(strcmp(ali->key,key)==0)
			{
				
				done = 0;
				//curr = prev;
				//prev = curr;
				prev->next = curr->next;
			
			
				free(ali->key);
				free(ali->command);
				free(ali);
				free(curr);
				curr = NULL;
				aliases->size --;

				return;
			}
		
			prev = curr;
		}

	}

	if(done ==1 && neededCheck == 1)
		printf("Command Not Found\n");
}

//----------------------------------------------------------------------------------------------------------

char * returnCommandIfExists(char * key, LinkedList * aliases)
{

	if(aliases->size ==0){

		return NULL;
	}

	Node * curr = aliases -> head;
	alias * ali;
	ali = curr->data;

	if(strcmp(ali->key, key)==0)
	{
		return ali->command;
	}
	else{
	
		for(curr; curr != NULL; curr = curr->next)
		{
			ali = curr->data;
			if(strcmp(ali->key,key)==0)
			{
				return ali->command;
			}	
		}
	}
	return NULL;
}//end returnCommandIfExists


//----------------------------------------------------------------------------------------------------------



