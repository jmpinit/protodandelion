#ifndef LIST_H
#define LIST_H

typedef struct Node {
	void* data;
	struct Node* next;
} Node;

Node* list_new(Node* root);				// add a new node to the list and return a ptr to it
void list_add(Node* root, void* data);	// add a new node and set its data field

#endif
