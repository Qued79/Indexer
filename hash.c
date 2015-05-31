#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "hash.h"

#define SIZE 10000



struct hashtable *makeTable(){
	struct hashtable *A = malloc(sizeof(struct hashtable));
	A->keylist = NULL;
	int i;
	for(i=0;i<SIZE;i++){
		A->dict[i]=NULL;	
		           }
	return A;
}

struct Word *MakeWord(char *a){
	struct Word *x = malloc(sizeof(struct Word));
	x->data = a;
return x;
}

int CompareWords(void* x, void*y){
	struct Word *a = (struct Word*)x;
	struct Word *b = (struct Word*)y;
	return strcmp(a->data,b->data);
}

/*Make a hashnode, dynamically allocating all of its data. The key for the hashnodes is word///path Since this only handles words and paths, the triple slash would be impossible[or at least very difficult] to create normally*/
struct hashnode *makeHashNode(char* word, const char* path, struct hashtable* table){
	struct hashnode *A = malloc(sizeof(struct hashnode));	
	char *j =(char*) malloc(strlen(path)+1);		
	char *i = (char*) malloc(strlen(path)+strlen(word)+4);
	char *k = (char*) malloc(strlen(word)+1);
	strcpy(j,path);	
	strcpy(k,word);	
	A->word = k;
	A->path = j;
	A->frequency = 1;	
	
	strcpy(i,path);
	strcat(i,"///");
	strcat(i,word);
	
	A->key = i;
	return A;
}



/*A hash function that should be good enough for this purpose*/
int hash(char* hashMe){
	int i,x;
	x=0;
	for(i=0;i<strlen(hashMe);i++){
		x += ((int)hashMe[i]);
		x^=21841;
	}
	x=x%SIZE;
	if(x<0){x+=SIZE;}	/*Necessary because % isn't really modulo and can return a negative number between 0 and -SIZE. Add SIZE to map to [0,SIZE-1]*/
	return x;
}


/*Called update because it isn't really add so much as add unique elements, and update frequencies otherwise. General use would have the latter be more common, so I named it that.*/
int update(char* word, const char* path, struct hashtable* table){
	int aHash;	
	struct hashnode *tmp;
	struct hashnode *a = makeHashNode(word,path,table);
	aHash = hash(a->key);
	tmp = search(a->key,table);
	if(tmp==0){/*If not already in the hashtable then add to both the keylist and the table*/
		table->dict[aHash]= simpleAdd(a,table->dict[aHash],CompareHashNodes);	
		table->keylist = simpleAdd(a,table->keylist,CompareHashNodes);
	}
	else{/*Otherwise, just update the frequency and delete the redundant hashnode*/
		tmp->frequency++;
		destroyhNode(a);
	    }
	return 0;
}

/*Just as the name says. The void*s are because I was trying my hand at information hiding. That's why you'll see them throughout*/
int CompareHashNodes(void* x, void*y){
	struct hashnode* i = (struct hashnode*)x;
	struct hashnode* j = (struct hashnode*)y;
	if(i==NULL){return 1;}
	if(j==NULL){return -1;}
	return (-1*(strcmp(i->key,j->key)));
}

int CompareKeys(void* x, void* y){
	char* a = (char*) x;
	struct hashnode *b = (struct hashnode*)y;
	char* j = b->key;
return strcmp(a,j);
}


void destroyhNode(void* aNode){
	if(aNode == NULL){return;}
	struct hashnode *a = (struct hashnode*)aNode;
	if(a->path!=NULL){free(a->path);}
	if(a->key!=NULL){free(a->key);}
	if(a->word!=NULL){free(a->word);}
	free(a);
}

/*Given a key, search the table for that element. Most of the heavy lifting is done by a LL search [since this hashtable uses chaining and the LL is a separate class]*/
struct hashnode *search(char* target, struct hashtable* table){
	int aHash = hash(target);	
	struct hashnode *a = searchL(target,table->dict[aHash],CompareKeys);
	return a;
}


void deleteTable(struct hashtable* table){
	int i;
	struct Node* tmp;
	if(table==NULL){return;}
	for(i=0;i<SIZE;i++){
		if(table->dict[i]!=NULL){destroyAll(table->dict[i],destroyhNode);}/*Walk through the table freeing every LL*/
			   }
	while((table->keylist)!=NULL){/*Free the LL structure that held the keys. The data was already deleted with the table since they shared hashnodes*/
		tmp = table->keylist; /*I figured I would do it that way to prevent the need for extra data. Why keep two copies of a string when you only need one?*/
		table->keylist = table->keylist->next;
		free(tmp);
				     }
	free(table);
}






