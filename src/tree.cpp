#include "tree.h"
#include "my_stack.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>

//---------------------SUBFUNCTIONS-DECLARATION-------------------------------------//

void generate_image(char const * input_file_name, char const * output_file_name);

void generate_dot(tree const * Tree, char const * file_name);

void write_node_to_dot(node const * nod, int* n_node, FILE* output_file);

void execute_term_cmd(char const * cmd_str, ...);

//----------------------FUNCTIONS-DEFINITIONS--------------------------------------//

void TreeConstructor(tree* Tree){

	assert(Tree != NULL);

	Tree->root.parent  = NULL;
	Tree->root.left    = NULL;
	Tree->root.right   = NULL;
	Tree->root.val 	   = NULL;
	Tree->root.visited = 0;

	return;
}

node* CreateNode(node* parent, char const * val, unsigned int val_size){

	assert(parent != NULL);

	node *new_node = (node*)calloc(1, sizeof(node));

	if(new_node == NULL){
		return NULL;
	}

	new_node->left    = NULL;
	new_node->right   = NULL;
	new_node->parent  = parent;

	new_node->val     = (char*)calloc(val_size, sizeof(char));
	memcpy(new_node->val, val, val_size);

	new_node->visited = 0;
	
	return new_node;
}

void TreeDestructor(tree* Tree){

	assert(Tree != NULL);

	DestructSubNode(Tree->root.left, LEFT_NODE_CODE);

	Tree->root.parent  = NULL;
	Tree->root.left    = NULL;
	Tree->root.right   = NULL;
	Tree->root.val 	   = NULL;
	Tree->root.visited = 0;
}

void DestructSubNode(node* nod, int mode){

	if(nod == NULL) return;

	if(nod->left != NULL) DestructSubNode(nod->left, LEFT_NODE_CODE);

	if(nod->right != NULL) DestructSubNode(nod->right, RIGHT_NODE_CODE);

	if(mode == LEFT_NODE_CODE) nod->parent->left = NULL;
	else nod->parent->right = NULL;

	free(nod->val);
	free(nod);

	return;
}

void SwapNodes(node* nod1, node* nod2){

	assert(nod1 != NULL);
	assert(nod2 != NULL);

	char* tmp = nod1->val;
	nod1->val = nod2->val;
	nod2->val = tmp;

	return;
}

int IsLeaf(node const * nod){
	return (nod->left == NULL) && (nod->right == NULL);
}

void ShowTree(tree* Tree){

	assert(Tree != NULL);

	generate_dot(Tree, GVIZ_DOT_NAME);
	generate_image(GVIZ_DOT_NAME, DUMP_IMAGE_NAME);
	
	return;
}

void generate_image(char const * input_file_name, char const * output_file_name){

	assert(input_file_name != NULL);
	assert(output_file_name     != NULL);
	assert(input_file_name != output_file_name);

	execute_term_cmd("dot -Tsvg %s -o %s", input_file_name, output_file_name);

	return;
}

void generate_dot(tree const * Tree, char const * file_name){

	assert(Tree != NULL);
	assert(file_name != NULL);

	FILE* tmp_file = fopen(file_name, "w");

	//TODO: log
	assert(tmp_file != NULL);

	fprintf(tmp_file, "digraph Tree{\n");

	int n_node = 0;
	write_node_to_dot(Tree->root.left, &n_node, tmp_file);

	fprintf(tmp_file, "}\n");

	fclose(tmp_file);

	return;
}

void write_node_to_dot(node const * nod, int* n_node, FILE* output_file){

	assert(nod != NULL);
	assert(output_file != NULL);

	int cur_n_node = *n_node;

	if(IsLeaf(nod)){
		fprintf(output_file, "%d[label = \"%s\", shape = \"octagon\", style=\"filled\",fillcolor=\"#76D7C4\"];\n", cur_n_node, nod->val);
	}
	else{
		fprintf(output_file, "%d[label = \"%s\", shape = \"octagon\", style=\"filled\",fillcolor=\"#7FB3D5 \"];\n", cur_n_node, nod->val);
	}

	if(nod->left != NULL){
		(*n_node)++;
		fprintf(output_file, "%d->%d[label = \"yes\"];\n", cur_n_node, *n_node);
		write_node_to_dot(nod->left, n_node, output_file);
	}

	if(nod->right != NULL){
		(*n_node)++;
		fprintf(output_file, "%d->%d[label = \"no\"];\n", cur_n_node, *n_node);
		write_node_to_dot(nod->right, n_node, output_file);
	}

	return;
}

void execute_term_cmd(char const * cmd_str, ...){

	assert(cmd_str != NULL);

	va_list args;
    va_start(args, cmd_str);

    int buflen = _vscprintf(cmd_str, args) + 1;
    char *buffer = (char*)calloc(buflen, sizeof(char));

    if (buffer != NULL){
        vsprintf(buffer, cmd_str, args);
    }

    va_end( args );

    WinExec(buffer, 0);

	free(buffer);

	return;
}
