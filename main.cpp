/* @Author
Student Name: Uzay Dalyan
Student ID: 150170041
Date: 21.12.2019 */

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 10

using namespace std;

struct node{ 
	
	char name[MAX]; 
    int number; 
    node *child; 
    node *sibling;
    node *parent_node;
}; 

struct path{
	
	int data;
	path *after;
};

struct plan{
	
	node *root;
	node *copyroot;
	path *start;
	path *newp;
	void file_message(FILE *ptr2);
	void file_network(FILE* ptr);
	void search(node *tofind, node *traverse);
	void add(node *toadd, node *parent);
	node* convertertonewnode(int number, char name[MAX]);
	node* convertertoparent(int number);
	node* converterformessage(int number);
	void create();
	void path_create();
	void path_add(int data);
	void path_delete();
	void findaim(node *tofind, node *traverse, int* check, int* counter);
	void sendmessage(char messagefromfile2[50], node *tofind);
	node *found;
	node *aim;
	int new_number;
	int parent_number;
	int* check;
	int* counter;
	char new_name[MAX]; 
	char messagefromfile1[50];
	int numberformessage;
	node* filetomessage;
	int* count;
};

void plan::path_delete(){
    
    path *tempdelete, *taildelete;
    
    tempdelete = start;
    
    while(tempdelete->after != NULL){
        
        taildelete = tempdelete->after;
        tempdelete->after = taildelete->after;
        delete taildelete;
    }
    
    delete tempdelete;
    
}

void plan::path_create(){
	
	start = NULL;
}

void plan::path_add(int data){
	
	newp = new path;
	newp->data = data;
	newp->after = NULL;
	
	if(start == NULL){
		
		start = newp;	
	}
	
	else{
		
		newp->after = start;
		start = newp;
	}
}

void plan::file_message(FILE* ptr2){ //reads message from file
	
	int i;

	while(!feof(ptr2)){
		
		i = 0;
	
		while(true){
			
			messagefromfile1[i] = fgetc(ptr2);
			
			if(messagefromfile1[i] == '>'){
				
				messagefromfile1[i] = '\0';
				break;
			}
			
			i++;
		}

		fscanf(ptr2, "%d\n", &numberformessage);
		filetomessage = converterformessage(numberformessage);
		sendmessage(messagefromfile1, filetomessage);
		
	}
}

void plan::file_network(FILE* ptr){ //reads network element from file
	
	copyroot = root;
	
	while(!feof(ptr)){
		
		fscanf(ptr, "%s %d %d", new_name, &new_number, &parent_number);
		
		search(convertertoparent(parent_number), copyroot);
		add(convertertonewnode(new_number, new_name), found);	
	}
}

void plan::create(){ //creates tree
	
	root = new node;
	strcpy(root->name, "CC");
	root->number = 0;
	root->child = NULL;
	root->sibling = NULL;
	root->parent_node = NULL;
} 

void plan::search(node *tofind, node* traverse){ //searchs tree
		
	if(traverse->number == tofind->number && (strcmp("BS",traverse->name) == 0 || strcmp("CC",traverse->name) == 0)){
			
		found = traverse;
		return;
	}
	else{
		
		if(traverse->child){//if child exists search it
		
			search(tofind, traverse->child);
		}
		
		if(traverse->sibling){//if sibling exists search it
		
			search(tofind, traverse->sibling);
		}
	}
}

void plan::add(node *toadd, node *parent){ //adding new element to tree
	
	node *tail;
	
	if(strcmp(parent->name,"BS") == 0 || strcmp(parent->name,"CC") == 0){
		
		if(strcmp(toadd->name,"MH") == 0){// if we are adding mobile host
		
			if(parent->child){//if parent has a child
				
				toadd->sibling = parent->child;
				parent->child = toadd;
				toadd->child = NULL;
				toadd->parent_node = toadd->sibling->parent_node;
				*count = *count + 1;
			}
			
			else{//if parent doesn't have child
			
				parent->child = toadd;
				toadd->sibling = NULL;
				toadd->child = NULL;
				toadd->parent_node = parent;
				*count = *count + 1;
			}
			
		}
		
		else{//if we are adding base station
			
			if(parent->child){//if parent has a child
			
				tail = parent->child;//finding end of children
				while(true){
					
					if(tail->sibling == NULL){
						
						break;
					}
					
					tail = tail->sibling;
				}
				
				tail->sibling = toadd;
				toadd->parent_node = tail->parent_node;
				toadd->sibling = NULL;
				toadd->child = NULL;
				*count = *count + 1;
			}
			
			else{//if parent doesn't have a child
				
				parent->child = toadd;
				toadd->sibling = NULL;
				toadd->child = NULL;
				toadd->parent_node = parent;
				*count = *count + 1;
			}
		}
	}
	
	else{//error
		
		cout << "Error!" << endl;
	}
}

void plan::sendmessage(char messagefromfile2[50], node *tofind){// sends message
	
	path_create();
	copyroot = root;
	check = new int;
	counter = new int;
	*check = 0;
	*counter = 0;
	int a = tofind->number;
	path *temp, *temptail;
	cout << "Traversing:";
	findaim(tofind, copyroot, check, counter);
	
	if(aim != NULL){
		
		int u = aim->number;
		cout << endl;
	
		while(aim != root){ //creating path
			
			aim = aim->parent_node;
			path_add(aim->number);
		}
	
		temp = start;
		cout << "Message:" << messagefromfile2 << " To:";
		while(temp){
			
			temptail = temp;
			temp = temp->after;
			cout << temptail->data << " ";
		}
		
		cout << "mh_" << u << endl;	
		path_delete();
	}
	
	else{
		
		cout << endl << "Can not be reached the mobile host mh_" << a << " at the moment" << endl;
	}
}

void plan::findaim(node *tofind, node *traverse, int *check, int *counter){
	
	
	if(*check == 0){
	
		if(strcmp("BS", traverse->name) == 0 || strcmp("CC", traverse->name) == 0){
			
			cout << traverse->number << " ";
		}
		
		if(traverse->number == tofind->number && (strcmp(tofind->name,traverse->name) == 0)){
				
			aim = traverse;
			*check = 1;
			return;
		}
		
		*counter = *counter + 1;
		
		if(*counter >= *count){
			
			aim = NULL;
			return;
		}
	
		if(traverse->child){//if child exists search it
			
			findaim(tofind, traverse->child, check, counter);
		}
			
		if(traverse->sibling){//if sibling exists search it
		
			findaim(tofind, traverse->sibling, check, counter);
		}
	}		
}

node* plan::convertertonewnode(int number, char name[MAX]){//converts to node
	
	node *newnode;
	newnode = new node;
	newnode->number = number;
	strcpy(newnode->name, name);
	return newnode;
}

node* plan::convertertoparent(int number){//converts a number to node
	
	node *parent_node;
	parent_node = new node;
	parent_node->number = number;
	return parent_node;
	
}

node* plan::converterformessage(int number){//converts a number to node
	
	node *message_node;
	message_node = new node;
	message_node->number = number;
	strcpy(message_node->name, "MH");
	return message_node;
	
}
plan plan1;

int main(int argc, char* argv[]){
	
	if (argc == 3){
	
		FILE* ptr = fopen(argv[1], "r");
		FILE* ptr2 = fopen(argv[2], "r");
		
		plan1.count = new int;
		*(plan1.count) = 1;
		
		plan1.create();
		plan1.path_create();
		
		plan1.file_network(ptr);
		plan1.file_message(ptr2);
		
		fclose(ptr);
		fclose(ptr2);
	}
	
	else{
		
		cout << "Error! You should enter 2 inputs!";
	}

	return 0;
}
