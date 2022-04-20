#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h> // assert
#include <time.h> // time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( void);

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree);

/* internal function (not mandatory)
*/
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data);

/* internal function (not mandatory)
*/
static void _insert( NODE **root, NODE *newPtr);

NODE *_makeNode( int data);

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey);

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete( NODE *root, int dltKey, int *success);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
//int *BST_Retrieve( TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
//static NODE *_retrieve( NODE *root, int key);

/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree);
static void _traverse( NODE *root);

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);
/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	int data;
	
	// creates a null tree
	tree = BST_Create();
	
	if (!tree)
	{
		printf( "Cannot create a tree!\n");
		return 100;
	}

	fprintf( stdout, "How many numbers will you insert into a BST: ");

	int numbers;
	scanf( "%d", &numbers);

	fprintf( stdout, "Inserting: ");
	
	srand( time(NULL));
	for (int i = 0; i < numbers; i++)
	{
		data = rand() % (numbers*3) + 1; // random number (1 ~ numbers * 3)
		
		fprintf( stdout, "%d ", data);
		
		// insert funtion call
		int ret = BST_Insert( tree, data);
		if (!ret) break;
 	}
	fprintf( stdout, "\n");
			
	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	while (1)
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		int ret = scanf( "%d", &num);
		if (ret != 1) break;
		
		ret = BST_Delete( tree, num);
		if (!ret)
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
		if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	BST_Destroy( tree);

	return 0;
}

TREE *BST_Create( void){
    TREE* newTree = (TREE*)malloc(sizeof(TREE));
    if(newTree == NULL) return NULL;
    return newTree;
}

void BST_Destroy( TREE *pTree){
    if(pTree)
    {
        _destroy( pTree->root);
    }
    free( pTree);
}

static void _destroy( NODE *root){
    if(root == NULL) return;
    _destroy(root->left);
    _destroy(root->right);
    free(root);
}

int BST_Insert( TREE *pTree, int data){
    NODE* temp = _makeNode(data);
    if(temp == NULL) return 0;
    else{
        _insert(&(pTree->root), temp);
    }
    return 1;
}

static void _insert( NODE **root, NODE *newPtr){
    if(*root == NULL){
        *root = newPtr;
    }else{
        if((*root)->data > newPtr->data){
            _insert(&((*root)->left), newPtr);
        }else{
            _insert(&((*root)->right), newPtr);
        }
    }
    return;
}

NODE *_makeNode( int data){
    NODE* newNode = (NODE*)malloc(sizeof(NODE)); // need free
    if(newNode == NULL) return NULL;
    else{
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->data = data;
    }
    return newNode;
}

int BST_Delete( TREE *pTree, int dltKey){
    int suc;
    NODE* temp = _delete(pTree->root, dltKey, &suc);
    
    if(suc == 1){
        pTree->root = temp;
        return 1;
    }else if(suc == 0) return 0;
    return 1;
}

static NODE *_delete( NODE *root, int dltKey, int *success){
    NODE *cur, *parent, *exch, *exchTemp;
    parent = NULL;
    cur = root;
    
    while( (cur != NULL) && (cur->data != dltKey)){
        parent = cur;
        if(dltKey < cur->data) cur = cur->left;
        else if(dltKey >= cur->data) cur = cur->right;
    }
    if(cur == NULL){
        *success = 0;
        return root;
    }
    
    if( (cur->left == NULL) && (cur->right == NULL)){
        if(parent != NULL){
            if(parent->left == cur){
                parent->left = NULL;
                *success = 1;
                return root;
            }
            else if(parent-> right == cur){
                parent->right = NULL;
                *success = 1;
                return root;
            }
        }else{
            // root == dltKey
            *success = 1;
            return NULL;
        }
    }else if( (cur->left == NULL) || (cur->right == NULL)){
        if(parent != NULL){
            if(cur->left != NULL){
                if(parent->left != NULL){
                    parent->left = cur->left;
                    cur = NULL;
                    *success = 1;
                    return root;
                }else if(parent->right != NULL){
                    parent->right = cur->left;
                    cur = NULL;
                    *success = 1;
                    return root;
                }
            }else if(cur->right != NULL){
                if(parent->left != NULL){
                    parent->left = cur->right;
                    cur = NULL;
                    *success = 1;
                    return root;
                }else if(parent->right != NULL){
                    parent->right = cur->right;
                    cur = NULL;
                    *success = 1;
                    return root;
                }
            }
        }else{
            if(cur->left != NULL){
                *success = 1;
                return cur->left;
            }else if(cur->right != NULL){
                *success = 1;
                return cur->right;
            }
        }
    }else{
        
        exchTemp = cur;
        exch = cur->right;
        while(exch->left != NULL){
            exchTemp = exch;
            exch = exch->left;
        }
        if(exchTemp->right == exch) exchTemp->right = exch->right;
        else exchTemp->left = exch->right;
        cur->data = exch->data;
        cur = exch;
    }
    cur = NULL;
    *success = 1;
    return root;
}

void BST_Traverse( TREE *pTree){
    _traverse( pTree->root);
    return;
}
static void _traverse( NODE *root){
    if(root == NULL) return;
    else{
        _traverse(root->left);
        printf("%d ", root->data);
        _traverse(root->right);
    }
    return;
}

void printTree( TREE *pTree){
    _infix_print(pTree->root, 0);
    return;
}

static void _infix_print( NODE *root, int level){
    int i;
    if(root == NULL) return;
    _infix_print(root->right, level+1);
    for(i=0; i<level; i++){
        printf("\t");
    }
    printf("%d\n", root->data);
    _infix_print(root->left, level+1);
    return;
}

int BST_Empty( TREE *pTree){
    if(pTree->root == NULL) return 1;
    return 0;
}

