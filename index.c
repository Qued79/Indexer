#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include "tokenizer.h"


struct hashtable *table;

int applyme(const char *fpath, const struct stat *sb, int tflag){
FILE *fp = fopen(fpath,"r+");
if(tflag == FTW_F){tokenizeAndStoreFile(fp,fpath,table);}
	return 0;
}
/*
 *Compare two hashnodes, first accodding to the strcmp of their words.
 *If equal, then sort them according to their frequency.
 *If even that is equal, sort according to the strcmp of their paths.
 *The point of this method is really to be fed to qsort such that it sorts in the above order. [word>freqency>path]
 * */
int compareHashNodes(const void* x, const void* y){
	 struct hashnode *a = *(struct hashnode **)x;
	 struct hashnode *b = *(struct hashnode **)y;
	
//	printf("\n%s\n",(a->word));
//	printf("\n%s\n",(b->word));
	
	int i = strcmp((a->word),(b->word));	
	if(i!=0){return i;}
	if((a->frequency) > (b->frequency)){return (-1);}
	if((a->frequency) < (b->frequency)){return (1);}
	return strcmp(a->path,b->path);
}


int main (int argc, char* argv[]){
	
	if(argc!=3){printf("Bad input - incorrect number of arguments\n");return 1;}
	FILE *file= fopen(argv[2],"r");
	if(file==NULL){printf("file/directory given to index does not exist\n");return 1;}
	fclose(file);/*only opened it to check that it existed*/
	file = fopen(argv[1],"r");
	if(file!=NULL){/*Handle the case of being given an output file that already exists. Prompt user for action*/
		char a[15];
		fclose(file);
		printf("Output file exists. Would you like to overwrite it? [y/n]:   ");
		scanf("%c",a);
		if(a[0]!='y'){return 1;}
		     }
	
	
	FILE *outfile= fopen(argv[1],"w+");/*Open the outfile for real this time*/
	table = makeTable();
	ftw(argv[2],applyme,100);/*The ftw function applies the given method [applyme] recursively across directories [and to all files in them]*/
	

	struct Node* curr = table->keylist;
	if(curr==NULL){deleteTable(table); fclose(outfile); return 0;}
	struct Node* next = curr->next; 	
	int size=1,j=0;
	while(next!=NULL){/*Walk across the keylist to get the number of elements in the hashtable - Really only necessary for the output at the end, but helpful for building sortMe*/
		size++;
		curr=next;
		next=curr->next;
	}	
	
	struct hashnode *sortMe[size];/*Will contain the list of hashnodes to be sorted by qsort and then output*/	
	curr = table->keylist;
	for(j=0;j<size;j++){/*Populate the sortMe list*/
		sortMe[j] = search(((struct hashnode*)(curr->data))->key,table);
		curr=curr->next;
	}
	
	qsort(sortMe,size,(sizeof(struct hashnode*)),compareHashNodes);
	
	/*Output in JSON format. This is ugly, but don't blame me, blame JSON*/
	fprintf(outfile,"{\"list\" : [\n");
	for(j=0;j<size;j++){
		if(j==0){fprintf(outfile,"\t\"%s\" : [\n",(sortMe[j]->word));
			fprintf(outfile,"\t\t{\"%s\" : %d}",(sortMe[j]->path),(sortMe[j]->frequency));
			if(j+1==size){fprintf(outfile,"\t]}\n]}\n");break;}
			continue;
			}
		if(strcmp((sortMe[j-1]->word),(sortMe[j]->word))!=0){fprintf(outfile,"\t\"%s\" : [",(sortMe[j]->word));}
		if(strcmp((sortMe[j]->word),(sortMe[j+1]->word))==0){fprintf(outfile,"\n\t\t{\"%s\" : %d},",(sortMe[j]->path),(sortMe[j]->frequency));}
		else{fprintf(outfile,"\n\t\t{\"%s\" : %d}\n\t\t]},\n\n",(sortMe[j]->path),(sortMe[j]->frequency));}
		if(j+2==size){
		if(strcmp((sortMe[j]->word),(sortMe[j+1]->word))!=0){fprintf(outfile,"\t\"%s\" : [",(sortMe[j+1]->word));}	
		fprintf(outfile,"\n\t\t{\"%s\" : %d}",(sortMe[j+1]->path),(sortMe[j+1]->frequency));
		fprintf(outfile,"\n\t]}\n]}\n"); break;}
	}
		
	deleteTable(table);/*Chain reaction data clearing*/
	fclose(outfile);/*Close the output file to finish*/
	
}

