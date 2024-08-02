#include <stdio.h>
#include <stdlib.h>

// Constants for colors
#define BLACK 1
#define RED 2

// Type Definition for Pointer on Node Struct
typedef struct node *pp;

// Node Struct Definition
struct node {
  int key;
  int color;
  pp left;
  pp right;
  pp parent;
};


const char* color(int color);
void LeftRotate(pp *root, pp x);
void RightRotate(pp *root, pp x);
void RBInsert(pp *root, int key);
void RBInsertFixUp(pp *root, pp *z);
void Print(pp *root);
pp Search(pp root, int key, int *check);
void RBDelete(pp *root, int key);
void RBTransplant(pp *root, pp u, pp v);
void RBDeleteFixUp(pp *root, pp x);
pp TreeMinimum(pp x);

int main(void) {
  
  int key; 
  pp root = NULL;
  char answer = '_';
  
  while(answer != 'Q' && answer != 'q') {
  	printf("\nA Program to represent a Sparse Array\n");
    printf(" Options:\n");
    printf(" I: Insert node\n");
    printf(" P: Print the Red Black Tree\n");
	printf(" D: Delete node\n");
    printf(" S: Search node\n");
    printf(" Q: Quit Program\n");
    printf("\n--------------------------------------------------------------\n");
    printf("\nSelect a proper option: (I/P/D/S/Q): ", answer);

    fflush(stdin);
    answer = getchar();
    getchar();

    printf("Your choice was: %c", answer);
    printf("\n--------------------------------------------------------------\n");

    if(answer != 'I' && answer != 'i' && answer != 'P' && answer != 'p' && answer != 'D' && answer != 'd' && answer != 'Q' &&
       answer != 'q' && answer != 'S' && answer != 's') {
      printf("\n\nInvalid input, please try again.\n");
    }

    if(answer == 'I' || answer == 'i') {
     printf("\n\nInput the key (key > 0) of a node z that you want to insert: ");
	 scanf("%d", &key);
      
     while((key < 1) || (getchar() != '\n')){
	  	printf("\nWrong node please insert again: ");
	  	while (getchar() != '\n'); // Clear the input buffer
	  	scanf("%d", &key);
	  }
		RBInsert(&root, key);
	}
	  
    if(answer == 'P' || answer == 'p') {
      printf("\n\nPrint the Red Black Tree\n"); 
      if (root == NULL) {
        printf("The Red Black Tree is empty, there is nothing to print\n");
	  } else {
      	for(int n = 0; n < 23; n++){
			printf("-");
	  	}
	  printf("\n");
	  Print(&root);
      printf("\n");
      }
	}

    if(answer == 'D' || answer == 'd') {   
	 if (root == NULL) {
        printf("The Red Black Tree is empty, there is nothing to delete\n");
	} else {
     	printf("\n\nInput the key (key > 0) of a node z that you want to delete: ");
	 	scanf("%d", &key);
      
     	while((key < 1) || (getchar() != '\n')){
	  		printf("\nWrong node please insert again: ");
	  		while (getchar() != '\n'); // Clear the input buffer
	  		scanf("%d", &key);
	  	}
	  	RBDelete(&root, key);
    	}
	}
    
  if(answer == 'S' || answer == 's') {
  	if (root == NULL) {
        printf("The Red Black Tree is empty, there is nothing to search\n");
	} else {
     	printf("\n\nInput the key (key > 0) of a node z that you want to search: ");
	 	scanf("%d", &key);
      
     	while((key < 1) || (getchar() != '\n')){
	  		printf("\nWrong node please insert again: ");
	  		while (getchar() != '\n'); // Clear the input buffer
	  		scanf("%d", &key);
	  	}
	  	int check;
			pp u;
		  	u = Search(root, key, &check);
	  	if (check == 0){
	  		printf("\nNode %d was not found\n", key);
		  } else if (check == 1){
		  	printf("\nNode %d exists and has color %s", u->key, color(u->color));
		  	if (u->parent != NULL || u->left != NULL || u->right != NULL){
				printf(" and");
				if (u->parent != NULL){
					printf(" parent %d", u->parent->key);
				}	
				if (u->left != NULL){
					printf(" left child %d", u->left->key);
				}
				if (u->right != NULL){
					printf(" right child %d", u->right->key);
				}
			}
			printf("\n");
		  	}
    	}
  	}	
 }
  printf("\n\nThe program has halted.");
  return 0;
}   
  
const char* color(int color) {
    switch (color) {
        case BLACK:
            return "Black";
        case RED:
            return "Red"; 
        default:
            return "Error"; // Handle invalid color values
    }
}

pp Search(pp root, int key, int *check){
	*check = 0;
	while (root != NULL && key != root->key){
		if (key < root->key){
			root = root->left;
		}
		else {
			root = root->right;
		}
	}
	if (root == NULL){
			*check = 0;
	} else if (root != NULL){
		*check = 1;
	}
	return root;
}

pp TreeMinimum(pp x){
	while(x->left != NULL){
		x = x->left;
	}
	return x;
}

void RBDelete(pp *root, int key){
	int check;
	pp z = (pp)malloc(sizeof(struct node));
	z = Search((*root), key, &check);
	if (check == 0){
		printf("\nNode %d doesn't exist\n", key);
		return;
	}
	pp x = NULL;
	pp y = z;
	int yOGcolor = y->color;
	if (z->left == NULL){
		x = z->right;
		RBTransplant(root, z, z->right);
	}
	else if (z->right == NULL){
		x = z->left;
		RBTransplant(root, z, z->left);
	} else {
		y = TreeMinimum(z->right);
		yOGcolor = y->color;
		x = y->right;
		if (y->parent == z){
			if (x != NULL){
				x->parent = y;
			}
		} else {
			RBTransplant(root, y, y->right);
			y->right = z->right;
			if (y->right != NULL){
					y->right->parent = y;
			}
		}
		RBTransplant(root, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	if (yOGcolor == BLACK){
		RBDeleteFixUp(root, x);
	}
   	free(z);
}

void RBDeleteFixUp(pp *root, pp x){
	while(x != NULL && x != (*root) && x->color == BLACK){
		if(x == x->parent->left){
		    pp w = x->parent->right;
			if(w !=NULL && w->color == RED){
				w->color = BLACK;
				x->parent->color = RED;
				LeftRotate(root, x->parent);
				w = x->parent->right;
			}
			if (w != NULL){
				if (w->left != NULL && w->left->color == BLACK && (w->right == NULL || w->right->color == BLACK)){
					w->color = RED;
					x = x->parent;
				} else {
					if(w->right != NULL){
						if (w->right->color == BLACK){
							w->left->color = BLACK;
							w->color = RED;
							RightRotate(root, w);
							w = x->parent->right;
						}
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					if (w->right != NULL){
						w->right->color = BLACK;
					}
					LeftRotate(root, x->parent);
					x = (*root);
					}
				}
		} else {
			pp w = x->parent->left;
			if(w != NULL && w->color == RED){
				w->color = BLACK;
				x->parent->color = RED;
				RightRotate(root, x->parent);
				w = x->parent->left;
			}
			if (w != NULL){
				if (w->right != NULL && w->right->color == BLACK && (w->left == NULL ||  w->left->color == BLACK)){
					w->color = RED;
					x = x->parent;
				} else {
					if (w->left != NULL){
						if (w->left->color == BLACK){
							w->right->color = BLACK;
							w->color = RED;
							LeftRotate(root, w);
							w = x->parent->left;
						}
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					if (w->left != NULL){
						w->left->color = BLACK;
					}
				RightRotate(root, x->parent);
				x = (*root);
				}
			}
		}
	}	
	if (x != NULL){
		x->color = BLACK;
 	}
}
	

void RBTransplant(pp *root, pp u, pp v){	
	if (u->parent == NULL){
		(*root) = v;
	} else if (u == u->parent->left){
		u->parent->left = v;
	} else{
		u->parent->right = v;
	}
	if (v != NULL) {
	    v->parent = u->parent;
	}
}

void LeftRotate(pp *root, pp x){
	pp y = NULL;
	y = x->right;
	x->right = y->left;
	if (y->left != NULL){
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == NULL){
		(*root) = y;		
	}
	else if (x == x->parent->left){
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}
	
void RightRotate(pp *root, pp x){
	pp y = NULL;
	y = x->left;
	x->left = y->right;
	if (y->right != NULL){
		y->right->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == NULL){
		(*root) = y;	
	}
	else if (x->parent != NULL){
		if	(x == x->parent->right){
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
	}
	y->right = x;
	x->parent = y;
}

void RBInsert(pp *root, int key){
	int check;
	pp u;
	u = Search((*root), key, &check);
    if (check == 1){
		printf("\nNode %d already exists and has color %s\n", u->key, color(u->color));
		return;
	}
	
	pp z = (pp)malloc(sizeof(struct node));
	z->key = key;
	pp x, y;
	y = NULL;
	x = (*root);
	while (x != NULL){
		y = x;
		if (z->key < x->key){
			x = x->left;
		}
		else{
			x = x->right;
		}
	}
	z->parent = y;
	if (y == NULL){
		(*root) = z;
	}
	else if(z->key < y->key){
		y->left = z;		
	}
	else{
		y->right = z;
	}
	z->left = NULL;
	z->right = NULL;
	z->color = RED;
	RBInsertFixUp(root, &z);
}

void RBInsertFixUp(pp *root, pp *z){	
	pp y = NULL;
	while ((*z)->parent != NULL  && (*z)->parent->color == RED){
		if ((*z)->parent == (*z)->parent->parent->left){
			y = (*z)->parent->parent->right;
			if (y != NULL && y->color == RED){
				(*z)->parent->color = BLACK;
				y->color = BLACK;
				(*z)->parent->parent->color = RED;
				(*z) = (*z)->parent->parent;
			} else {
				if ((*z) == (*z)->parent->right){
					(*z) = (*z)->parent;
					LeftRotate(root, (*z));
				}
				(*z)->parent->color = BLACK;
				(*z)->parent->parent->color = RED;
				RightRotate (root, ((*z)->parent->parent));
			}
		}
    	else { 
        y = (*z)->parent->parent->left;
        if (y != NULL  && y->color == RED) {
            (*z)->parent->color = BLACK;           
            y->color = BLACK;                      
            (*z)->parent->parent->color = RED;      
            (*z) = (*z)->parent->parent;            
        } else {
            if ((*z) == (*z)->parent->left) {
                (*z) = (*z)->parent;     
                RightRotate(root, (*z));          
            }
            (*z)->parent->color = BLACK;           
            (*z)->parent->parent->color = RED;   
            LeftRotate(root, ((*z)->parent->parent)); 
        	}
    	}
	}  
	(*root)->color = BLACK;
}

void Print(pp *root)
{
	if ((*root)->left!=NULL)
	{
		Print(&(*root)->left);
	}
	printf("Node %d with color %s", (*root)->key, color((*root)->color));
	if ((*root)->parent != NULL || (*root)->left != NULL || (*root)->right != NULL){
		printf(" and");
		if ((*root)->parent != NULL){
		printf(" parent %d", (*root)->parent->key);
		}
		if ((*root)->left != NULL){
			printf(" left child %d", (*root)->left->key);
		}
		if ((*root)->right != NULL){
			printf(" right child %d", (*root)->right->key);
		}
	}
	printf("\n");
	if ((*root)->right!=NULL)
	{
		Print(&(*root)->right);
	}	
}
