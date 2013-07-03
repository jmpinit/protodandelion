#include <stdlib.h>
#include "list.h"

Node* list_get_index(Node* root, unsigned int i) {
	if(root == NULL) return NULL;

	unsigned int pos = 0;
	Node* current = root;
	while(current != NULL) {
		if(pos == i) return current;
		current = current->next;
		pos++;
	}

	return NULL;
}

Node* list_new(Node* current) {
	if(current->next == NULL && current->data == NULL) {
		return current;
	} else {
		Node* newnode = calloc(1, sizeof(Node));

		while(current->next != NULL)
			current = current->next;

		current->next = newnode;

		return newnode;
	}
}

void list_add(Node* root, void* data) {
	Node* newnode = list_new(root);
	newnode->data = data;
}
