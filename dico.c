#include "fastafarism.h"



//=====================================================================================================================
// Init parameters of a nodeDico.
//=====================================================================================================================
void initNodeDico(nodeDico *node) {
	int i;

	node->prefix = 0;
	node->final = 0;
	for (i = 0; i < 4; i++)
		node->nt[i] = NULL;
}


//=====================================================================================================================
// Insert the current sequence in the dictionnary.
//=====================================================================================================================
void insertionDico(nodeDico *root, char* seq){
	
	if(DEBUG){
		printf("ROOT \t%p\n", root );
		printf("A :\t%p\n", root->nt[0] );
		printf("T/U :\t%p\n", root->nt[1] );
		printf("G :\t%p\n", root->nt[2] );
		printf("C :\t%p\n", root->nt[3] );
		printf("Final :\t%d\n", root->final );
		printf("Prefix :\t%d\n", root->prefix );
	}

	root->prefix = (root->prefix)+1 ;

	if(seq[0] == '\0'){
		root->final = 1;
	}else{
		switch(seq[0]){
			case 'A' :
				if(root->nt[0] == NULL){
					root->nt[0] = malloc(sizeof(nodeDico));
					initNodeDico(root->nt[0]);
				}
				insertionDico(root->nt[0], seq+1);
			break; 

			case 'T' : 
				if(root->nt[1] == NULL){
					root->nt[1] = malloc(sizeof(nodeDico));
					initNodeDico(root->nt[1]);
				}
				insertionDico(root->nt[1], seq+1);
			break; 

			case 'U' :
				if(root->nt[1] == NULL){
					root->nt[1] = malloc(sizeof(nodeDico));
					initNodeDico(root->nt[1]);
				}
				insertionDico(root->nt[1], seq+1);
			break; 

			case 'G' :
				if(root->nt[2] == NULL){
					root->nt[2] = malloc(sizeof(nodeDico));
					initNodeDico(root->nt[2]);
				}
				insertionDico(root->nt[2], seq+1);
			break; 

			case 'C' :
				if(root->nt[3] == NULL){
					root->nt[3] = malloc(sizeof(nodeDico));
					initNodeDico(root->nt[3]);
				}
				insertionDico(root->nt[3], seq+1);
			break; 		
		}
	}
}

//=====================================================================================================================
// Research a prefix or a whole sequence in the dictionnary created from a FASTA file.
// Return 1 if found, 0 if not and n for sum of prefixes.
// Typesearch : 0 = whole sequence, 1 = number of prefixes.
//=====================================================================================================================
int searchDico(nodeDico *root, char* seq, int type){

	int currentNt;

	if(seq[0] != '\0'){
		switch(seq[0]){
			case 'A': currentNt = 0; break ;
			case 'T' : currentNt = 1; break ;
			case 'U' : currentNt = 1; break ;
			case 'G' : currentNt = 2; break ;
			case 'C' : currentNt = 3; break ;
			default : return 0; break;
		}
		if( root->nt[currentNt] != NULL){
			return searchDico((root->nt)[currentNt], seq+1, type);
		}else{
			return 0;
		}

	}else{

		if(type == 1){
			return root->prefix;
		}
		else if(root->final == 1){
			return 1;
		}else{
			return 0;
		}
	}
}

//=====================================================================================================================
// Free the dictionnary linked list.
//=====================================================================================================================
void freeDico(nodeDico *root){

	int i;

	for(i =0; i<4; i++){
		if(root->nt[i] != NULL){
			freeDico(root->nt[i]);
		}
	}
	free(root);
}















