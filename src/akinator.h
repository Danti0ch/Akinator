#ifndef AKIN_H
#define AKIN_H

#include "tree.h"
#include "my_stack.h"

enum MENU_CODES{

	EXIT_CODE = 0,
	GUESS_OBJ_CODE,
	FEAT_SET_CODE,
	SIMILARITY_CODE,
	SHOW_BASE_CODE
};

enum ERR_CODES{
	REQ_LEN_EXCEED
};

// for get features
const int VALID_NODE   = 1;
const int INVALID_NODE = 0;

const int MAX_REQ_LEN  = 50;
const int MAX_BUF_SIZE = 600;

const char DATA_FILE_NAME[20] = "data.txt";

//---------------------FUNCTIONS-DECLARATION-------------------------------------//

void ShowModes();

void FillTree(tree* Tree);

void GuessObj(tree* Tree);

void ShowFeatures(tree* Tree, char* name);

int GetFeatures(node* nod, stack_t* Features, stack_t* notFeatures, char const * name);

void ShowSimilarities(tree* Tree, char* name1, char* name2);

void SaveTree(node const * parent, char const * output_file_name);

void MakeTreeStr(node const * parent, char* buffer, int* n_written);

int IsBufferOverflowed(char const * buffer, int buf_size);

#define TO_LOG(code) printf("read error: %d\n", (code));

#endif
