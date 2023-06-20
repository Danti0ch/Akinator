#include <stdio.h>
#include "akinator.h"

int main(){

	tree Tree = {};

	TreeConstructor(&Tree);

	FillTree(&(Tree));

	char buffer[MAX_REQ_LEN]  = "";
	char buffer2[MAX_REQ_LEN] = "";

	int cmd_code = 0;
	
	ShowModes();

	while((scanf("%d", &cmd_code) != 0) && (cmd_code != 0)){
		switch(cmd_code){
			
			case GUESS_OBJ_CODE:
				GuessObj(&(Tree));
				break;

			case FEAT_SET_CODE:
				printf("Who is in your interest?\n");

				scanf("%s", buffer);

				if(IsBufferOverflowed(buffer, MAX_REQ_LEN)){
					TO_LOG(REQ_LEN_EXCEED)
					break;
				}

				ShowFeatures(&Tree, buffer);
				break;

			case SIMILARITY_CODE:
				printf("Who is in your interest?\n");

				scanf("%s", buffer);
				scanf("%s", buffer2);

				ShowSimilarities(&Tree, buffer, buffer2);
				break;

			case SHOW_BASE_CODE:
				ShowTree(&Tree);
				break;

			default:
				break;
		}

		ShowModes();
	}

	SaveTree(&(Tree.root), DATA_FILE_NAME);

	TreeDestructor(&Tree);

	return 0;
}
