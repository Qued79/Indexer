#define SIZE 10000
#include "Node.h"
struct hashtable{
	struct Node* keylist;
	struct Node* dict[SIZE];
};

struct Word{
	char* data;
};

struct hashnode{
	char* word;
	char* path;
	char* key;
	int frequency;
};

struct hashtable *makeTable();
int hash(char* hashMe);
int update(char* word, const char* path , struct hashtable* table);
struct hashnode *makeHashNode(char* word, const char* path, struct hashtable* table);
int CompareHashNodes(void* x, void*y);
int comparestrings(void* x, void* y);
void destroyhNode(void* aNode);
struct hashnode *search(char* target, struct hashtable* table);
void deleteTable(struct hashtable* table);
