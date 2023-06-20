#include "akinator.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>

//---------------------SUBFUNCTIONS-DECLARATION-------------------------------------//

unsigned int get_file_size(FILE* fp);

void parse_buf(char* buffer, tree* Tree);

void get_base(char** buffer);

void make_new_feature(node* parent_node, char const * feature_str, char const * new_leaf_val);

//----------------------FUNCTIONS-DEFINITIONS--------------------------------------//

void FillTree(tree* Tree){

	assert(Tree != NULL);

	char* buffer = NULL;

	get_base(&buffer);

	parse_buf(buffer, Tree);

	free(buffer);

	return;
}

void GuessObj(tree* Tree){

	assert(Tree != NULL);

	node* cur_nod = Tree->root.left;

	char req_buf[MAX_REQ_LEN]  = "";
	char val_buf1[MAX_VAL_LEN] = "";
	char val_buf2[MAX_VAL_LEN] = "";

	for(;;){
		if(IsLeaf(cur_nod)){
			printf("Is it %s?\n", cur_nod->val);
		}
		else{
			printf("%s?\n", cur_nod->val);
		}

		scanf("%s", req_buf);

		if(strcmp(req_buf, "yes") == 0){	// yes->y no->n
			if(cur_nod->left != NULL){
				cur_nod = cur_nod->left;
			}
			else{
				printf("easy peasy\n");
				return;
			}
		}

		else if(strcmp(req_buf, "no") == 0){
			
			if(cur_nod->right != NULL){
				cur_nod = cur_nod->right;
			}
			else{
				printf("What is it then?\n");
				getchar();
				scanf("%[^\n]", val_buf1);

				if(IsLeaf(cur_nod)){
					printf("What are the differences between %s and %s?\n", val_buf1, cur_nod->val);
					getchar();

					scanf("%[^\n]", val_buf2);

					make_new_feature(cur_nod, val_buf2, val_buf1);
				}

				else{
					cur_nod->right = CreateNode(cur_nod, val_buf1, strlen(val_buf1) + 1);
				}

				return;
			}
		}
		else if(strcmp(req_buf, "exit") == 0){
			return;
		}
		else{
			printf("I haven't understood, if you wanna exit, type \"exit\"\n");
		}

		memset(val_buf1, '\0', MAX_VAL_LEN);
		memset(val_buf2, '\0', MAX_VAL_LEN);
	}
	return;
}

void make_new_feature(node* parent_node, char const * feature_str, char const * new_leaf_val){

	assert(parent_node != NULL);
	assert(feature_str != NULL);
	assert(new_leaf_val != NULL);

	parent_node->right = CreateNode(parent_node, feature_str, strlen(feature_str) + 1);
	
	SwapNodes(parent_node, parent_node->right);

	parent_node->left = CreateNode(parent_node, new_leaf_val, strlen(new_leaf_val) + 1);

	return;
}

void ShowFeatures(tree* Tree, char* name){

	assert(Tree != NULL);

	stack_t Features = {};
	StackConstructor(&Features, 1);

	stack_t notFeatures = {};
	StackConstructor(&notFeatures, 1);

	int res = GetFeatures(Tree->root.left, &Features, &notFeatures, name);

	if(res == VALID_NODE){

		if(Features.size > 0 || notFeatures.size > 0) printf("%s is ", name);
		while(Features.size > 0){
			printf("%s, ", StackPop(&Features));
		}

		while(notFeatures.size > 0){
			printf("not %s, ", StackPop(&notFeatures));
		}
	}
	else{
		printf("obj not found\n");
	}

	StackDestructor(&Features);
	StackDestructor(&notFeatures);
	
	return;
}

int GetFeatures(node* nod, stack_t* Features, stack_t* notFeatures, char const * name){

	assert(nod 		!= NULL);
	assert(Features != NULL);
	assert(name 	!= NULL);

	if(strcmp(nod->val, name) == 0){
		if(IsLeaf(nod))	return VALID_NODE;
		else            return INVALID_NODE;
	}

	if(IsLeaf(nod))  	return INVALID_NODE;

	if(nod->left != NULL){

		int is_target_node = GetFeatures(nod->left, Features, notFeatures, name);

		if(is_target_node == VALID_NODE){
			StackPush(Features, nod->val);
			return VALID_NODE;
		}
	}
	if(nod->right != NULL){

		int is_target_node = GetFeatures(nod->right, Features, notFeatures, name);

		if(is_target_node == VALID_NODE){
			StackPush(notFeatures, nod->val);
			return VALID_NODE;
		}
	}

	return INVALID_NODE;
}

void ShowSimilarities(tree* Tree, char* name1, char* name2){

	assert(Tree  != NULL);
	assert(name1 != NULL);
	assert(name2 != NULL);

	int res_of_getting_features = 0;

	stack_t Features1 = {};
	StackConstructor(&Features1, 1);

	stack_t notFeatures1 = {};
	StackConstructor(&notFeatures1, 1);

	res_of_getting_features = GetFeatures(Tree->root.left, &Features1, &notFeatures1, name1);

	if(res_of_getting_features == INVALID_NODE){
		
		printf("object %s not found\n", name1);
		StackDestructor(&Features1);
		StackDestructor(&notFeatures1);

		return;
	}

	stack_t Features2 = {};
	StackConstructor(&Features2, 1);

	stack_t notFeatures2= {};
	StackConstructor(&notFeatures2, 1);

	res_of_getting_features = GetFeatures(Tree->root.left, &Features2, &notFeatures2, name2);

	if(res_of_getting_features == INVALID_NODE){
		
		printf("object %s not found\n", name2);
		StackDestructor(&Features2);
		StackDestructor(&notFeatures2);

		return;
	}
	
	printf("%s and %s are similar in that they are ", name1, name2);

	while((Features1.size > 0) && (Features2.size > 0)){

		char buf1[MAX_VAL_LEN]  = "";
		char buf2[MAX_VAL_LEN]  = "";
	
		strcpy(buf1, StackPop(&Features1));
		strcpy(buf2, StackPop(&Features2));
		
		if(strcmp(buf1, buf2) == 0){
			printf("%s, ", buf1);
		}
		else{
			StackPush(&Features1, buf1);
			StackPush(&Features2, buf2);
			break;
		}
	}

	while((notFeatures1.size > 0) && (notFeatures2.size > 0)){

		char buf1[MAX_VAL_LEN]  = "";
		char buf2[MAX_VAL_LEN]  = "";
	
		strcpy(buf1, StackPop(&notFeatures1));
		strcpy(buf2, StackPop(&notFeatures2));
		
		if(strcmp(buf1, buf2) == 0){
			printf("not %s, ", buf1);
		}
		else{
			StackPush(&notFeatures1, buf1);
			StackPush(&notFeatures2, buf2);
			break;
		}
	}
	printf("\n");

	if(Features1.size > 0 || notFeatures1.size > 0){
		printf("%s differs from %s in that it is ", name1, name2);
	}
	while(Features1.size > 0){
		printf("%s, ", StackPop(&Features1));
	}
	while(notFeatures1.size > 0){
		printf("not %s, ", StackPop(&notFeatures1));
	}
	printf("\n");

	if(Features2.size > 0 || notFeatures2.size > 0){
		printf("%s differs from %s in that it is ", name2, name1);
	}
	while(Features2.size > 0){
		printf("%s, ", StackPop(&Features2));
	}
	while(notFeatures2.size > 0){
		printf("not %s, ", StackPop(&notFeatures2));
	}
	printf("\n");

	StackDestructor(&Features1);
	StackDestructor(&notFeatures1);
	
	StackDestructor(&Features2);
	StackDestructor(&notFeatures2);
	return;
}

void parse_buf(char* buffer, tree* Tree){

	assert(buffer != NULL);
	assert(Tree != NULL);

	int n_readen = 0;

	char* val_str = (char*)calloc(MAX_VAL_LEN, sizeof(char));

	node* parent_nod = &(Tree->root);

	while(*buffer != '\0'){

		if(sscanf(buffer, "{ \"%[^\"]\" %n", val_str, &n_readen) == 1){
			
			if(parent_nod->visited){
				parent_nod->visited = 0;

				parent_nod->right = CreateNode(parent_nod, val_str, strlen(val_str) + 1);
				parent_nod = parent_nod->right;
			}

			else{
				parent_nod->visited = 1;

				parent_nod->left = CreateNode(parent_nod, val_str, strlen(val_str) + 1);
				parent_nod = parent_nod->left;
			}

			buffer += n_readen; 
		}

		else if(*buffer == '{'){

			if(parent_nod->visited){
				parent_nod->visited = 0;
				parent_nod->right   = NULL;
			}
			else{
				parent_nod->visited = 1;
				parent_nod->left    = NULL;
			}
			buffer += 2;
		}

		else if(*buffer == '}'){
			parent_nod = parent_nod->parent;
			buffer += 1;
		}

		else{
			break;
		}
	}
}

void get_base(char** buffer){

	FILE* data_file = fopen(DATA_FILE_NAME, "r");
	unsigned int file_size = get_file_size(data_file);

	*buffer = (char*)calloc(file_size, sizeof(char));

	fread(*buffer, sizeof(char), file_size, data_file);

	fclose(data_file);
	return;
}

void MakeTreeStr(node const * parent, char* buffer, int* n_written){

	assert(parent != NULL);
	assert(buffer != NULL);
	assert(n_written != NULL);

	buffer[*n_written] = '{';
	(*n_written)++;

	if(parent->left != NULL){

		int left_val_len = strlen(parent->left->val);

		buffer[*n_written] = '\"';
		(*n_written)++;

		memcpy(buffer + *n_written, parent->left->val, left_val_len);
		*n_written += left_val_len;

		buffer[*n_written] = '\"';
		(*n_written)++;

		MakeTreeStr(parent->left, buffer, n_written);
	}
	buffer[*n_written] = '}';
	(*n_written)++;

	buffer[*n_written] = '{';
	(*n_written)++;

	if(parent->right != NULL){

		int right_val_len = strlen(parent->right->val);

		buffer[*n_written] = '\"';
		(*n_written)++;

		memcpy(buffer + *n_written, parent->right->val, right_val_len);
		*n_written += right_val_len;

		buffer[*n_written] = '\"';
		(*n_written)++;

		MakeTreeStr(parent->right, buffer, n_written);
	}

	buffer[*n_written] = '}';
	(*n_written)++;

	return;
}

void SaveTree(node const * parent, char const * output_file_name){

	assert(parent != NULL);
	assert(output_file_name != NULL);

	char buffer[MAX_BUF_SIZE] = "";
	int n_written = 0;

	MakeTreeStr(parent, buffer, &n_written);
	FILE* output_file = fopen(output_file_name, "w");

	fwrite(buffer, sizeof(char), n_written, output_file);
	fclose(output_file);

	return;
}

void ShowModes(){

	printf("\nChoose mode:\n"
			"1: GuessObjGame\n"
			"2: Features\n"
			"3: Similarities\n"
			"4: Show base\n"
			"0: exit\n\n");
	return;
}

unsigned int get_file_size(FILE* fp){

	assert(fp != NULL);

	fseek(fp, 0, SEEK_END);
    unsigned int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}

int IsBufferOverflowed(char const * buffer, int buf_size){

	assert(buffer != NULL);

	return *(buffer + buf_size) != '\0';
}