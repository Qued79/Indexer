#include "hash.h"
#include <stdio.h>
#include <stdlib.h>

struct Node *makeNode(void* x,struct Node *y){


	struct Node *root;			/*exactly what you expect*/
	root = malloc(sizeof(struct Node));	/*dynamically allocate memory for nodes (must free later)*/
	root->next= y;
	root->data = x;
	root->ptrct = 0;			/*This will be the count of static pointers to this node*/
	return root;
}



void destroyNode(struct Node *a, void(*DestructFuncT)(void*)){
if(a!=NULL){					/*First make sure you weren't passed a bad value*/
	if(a->next!=NULL){(a->next->ptrct)--;}	/*Since we will be deleting the node it will no longer point to its next*/
	if(a->ptrct==0)				/*Only delete if the ptrct =0. This shouldn't be necessary, except that I can't expect*/
	{					/*the user to know about this fact, so I'm safeguarding against ignorance...and segfaults*/
		DestructFuncT(a->data);		/*Call the destruct function then free the node*/
		if(a!=NULL){free(a);}
	}
	   }
}

void destroyAll(struct Node *root, void (*DestructFuncT)(void*)){
	struct Node *tmp;			/*This function does exactly what it says and destroys every node in the given LL*/
	while(root!=NULL){			/*regardless of their pointer count. This will leave dangling pointers. Nuclear option.*/
		tmp=root;
		DestructFuncT(tmp->data);	/*Walks across the list destroying everything in its wake*/
		root=root->next;
		if(tmp!=NULL){free(tmp);}
	}	
}




struct Node *orderedAdd(void* data, struct Node *root, int (*CompareFuncT)(void*,void*)){
	struct Node *curr=root;
	struct Node *prev=0;
	if(root==NULL){/*Should only be reached in case of repeated deletes, then new insertion as to make a LL you need some data, but this works too*/
		struct Node *a = makeNode(data,NULL);
		a->ptrct=1;/*this added 1 is because as I stated before, this shouldn't be reached, but if it is, I know it must be the new root of the list*/

		return a;
	}
/*Walk until you find the correct location for the insert (until you find a number>= to the data being inserted or you hit the end of the list)*/
	while(   CompareFuncT(data,(curr->data))==-1 && (curr->next)!=0){
		prev=curr;
		curr=(curr->next);
						   		        }

/*In case of equality do not add*/
	if(CompareFuncT(data,(curr->data))==0){
	((struct hashnode*)curr)->frequency++;
	return root;
}

/*In case of end of list insert*/
	if(CompareFuncT((curr->data),data)==1){
		struct Node *a = makeNode(data,curr->next);
		a->ptrct=1;
		curr->next = a;
				}
/*General case: location found, make a new node with next pointing to curr, point prev to new node*/
/*Also handles root insertion*/
	else{
		struct Node *a = makeNode(data,curr);
		a->ptrct=1;/*Whether it is root or in the middle, this will have one static pointer to it by the end of this block*/
		if(curr==root){root=a;}
		else{prev->next = a;}
    	    }
	return root;
}

struct Node *simpleAdd(void* data, struct Node *root, int(*CompareFuncT)(void*,void*)){
	struct Node *curr=root;
	if(root==NULL){
		if (data!=NULL){root = makeNode(data,NULL);}
		return root;
	}
	while(curr!=NULL){
		if(CompareFuncT(data,(curr->data))==0){
			((struct hashnode*)(curr->data))->frequency++;
			//destroyhNode(((struct hashnode*)(data)));/*Not being added to the structure so it could not be freed later. Instead free it now.*/
			return root;
		}
		curr=(curr->next);
	}
	curr = makeNode(data,root);
	root=curr;
	return root;
}


struct Node *deleteEntry(void* data, struct Node *root, int (*CompareFuncT)(void*,void*), void (*DestructFuncT)(void *)  ){
	struct Node *delete=root; 
	struct Node *deletePrev=0;
	if(root==NULL){return NULL;}	
	/*Walk until target matches the current node's data or until you hit the end of the list*/
	while(CompareFuncT(data,(delete->data))!=0  &&   ((delete->next)!=0)){
		deletePrev=delete;
		delete=delete->next;
					  	  			     }
	/*If you found the target, delete the node being careful to not delete the root of the list*/
	if(CompareFuncT(data,(delete->data))==0){
		if(delete==root){
			root = root->next;
			(delete->ptrct)--;
			if(root!=NULL){(root->ptrct)++;}/*since root ptr moved to a new node, that new node needs to have its pointer count incremented*/
			if(delete->ptrct==0)destroyNode(delete,DestructFuncT);
				}
		else{
			deletePrev->next = delete->next;
			(delete->ptrct)--;	      /*Subtract one from the count of pointers to the 'delete' mode*/
			if(delete->ptrct==0){
				destroyNode(delete,DestructFuncT);
					    }/*Only delete if there are no longer any pointers to the node, don't want dangling iterators*/
	    	    }
			      }
	return root;



						}



void *searchL(void* data, struct Node *root, int (*CompareFuncT)(void*,void*)){
	struct Node *search=root;

	if(root==NULL){return NULL;}

	/*Walk until target matches the current node's data or until you hit the end of the list*/
	while(CompareFuncT(data,(search->data))!=0  &&   ((search->next)!=0)){
	
		search=search->next;
					  	  			     }
	/*If you found the target, delete the node being careful to not delete the root of the list*/
	if(CompareFuncT(data,(search->data))==0){
		
			return search->data;
	    	   				}
			      
	return NULL;
}








