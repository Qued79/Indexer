
struct Node{
	void* data;	
	struct Node *next;
	int ptrct;
};

struct Node *makeNode(void* x,struct Node *y);/*create a linked list node with data 'x'*/
void destroyAll(struct Node *root, void (*DestructFuncT)(void*));/*DO NOT CALL UNLESS YOU KNOW WHAT YOU'RE DOING. DESTROYS THE WHOLE LIST*/
void destroyNode(struct Node *node,void (*DestructFuncT)(void*));/*Delete this node from the list. Is only actually freed if all internal static pointers to it are gone*/
struct Node *orderedAdd(void* data, struct Node *root, int (*CompareFuncT)(void*,void*));/*Place data in the correct position in ordered LL root*/
struct Node *simpleAdd(void* data, struct Node *root, int (*CompareFuncT)(void*,void*));/*Place data in the correct position in ordered LL root*/
struct Node *deleteEntry(void* data, struct Node *root, int (*CompareFuncT)(void*,void*), void (*DestructFuncT)(void*));/*Remove data from the LL*/
void *searchL(void* data, struct Node *root, int (*CompareFuncT)(void*,void*));
