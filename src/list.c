#include <stdlib.h>
#include "list.h"

struct Node* list_get_index(struct Node* root, unsigned int i) {
	if(root == NULL) return NULL;

	unsigned int pos = 0;
	struct Node* current = root;
	while(current != NULL) {
		if(pos == i) return current;
		current = current->next;
		pos++;
	}

	return NULL;
}

struct Node* list_new(struct Node* current) {
	if(current->next == NULL && current->data == NULL) {
		return current;
	} else {
		struct Node* newnode = calloc(1, sizeof(struct Node));

		while(current->next != NULL)
			current = current->next;

		current->next = newnode;

		return newnode;
	}
}

void list_add(struct Node* root, void* data) {
	struct Node* newnode = list_new(root);
	newnode->data = data;
}
