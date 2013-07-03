#ifndef LIST_H
#define LIST_H

struct Node {
	void* data;
	struct Node* next;
};

struct Node* list_new(struct Node* root);				// add a new node to the list and return a ptr to it
void list_add(struct Node* root, void* data);	// add a new node and set its data field

#endif
