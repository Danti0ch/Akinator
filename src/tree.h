#ifndef TREE_H
#define TREE_H


// TODO: мета инфа в дампе

const int MAX_VAL_LEN = 100;

const int LEFT_NODE_CODE  = 0;
const int RIGHT_NODE_CODE = 1;

char const GVIZ_DOT_NAME[50]   = "tmp.dot";
char const DUMP_IMAGE_NAME[50] = "img.svg";

struct node{
	char* val;

	node* parent;
	node* left;
	node* right;

	char visited;
};

struct tree{
	node root;
};

//---------------------FUNCTIONS-DECLARATION-------------------------------------//

void TreeConstructor(tree* Tree);

node* CreateNode(node* parent, char const * val, unsigned int val_size);

void TreeDestructor(tree* Tree);

void ShowTree(tree* Tree);

int IsLeaf(node const * nod);

void DestructSubNode(node* nod, int mode);

void SwapNodes(node* nod1, node* nod2);

#endif
