#define _GNU_SOURCE
#include "fastafarism.h"


//=====================================================================================================================
// Research a sequence or a name in content or comment. 
// Return 1 if found.
// Typesearch : 1 = gene name, 2 = sequence.
//=====================================================================================================================
int searchSeqOrName(Sequence *s, char *query, int typeSearch){
	char *chain = NULL;
	
	if(typeSearch == 1){
		chain = s->comment;
	}
	else{
		chain = s->content;
	}

	if(strcasestr(chain,query) != NULL){
		return 1;
	}

	return 0;
}


//=====================================================================================================================
// Research a subsequence in content.
// Return if 1 found.
//=====================================================================================================================
int searchSubSeq(Sequence *s, char* query){

	int i = 0;
	int j = 0;

	char* str = s->content;

	while((j <= strlen(query)-1) && (i <= strlen(str)-1) ){

		if(str[i]==query[j]) j++;
		i++;
	}

	if(j > strlen(query)-1){
		return 1;
	}else{
		return 0;
	}
}


//=====================================================================================================================
// Print the sequences found for a type of research, in an output file.
//=====================================================================================================================
void doSearch(Sequence seq, Options opt, FILE* OUT_SCH, int *nbrFound){

	if(*nbrFound < atoi(opt.occurrence) || strcmp(opt.occurrence, "all") == 0 ){

		switch(opt.typeSearch){
			// Search in comment
			case 1 :
				if(searchSeqOrName(&seq, opt.searchQuery, 1) == 1){
					printSeq(seq, OUT_SCH);
					(*nbrFound)++;
				}
			break;

			// Search in sequence
			case 2 :
				if(searchSeqOrName(&seq, opt.searchQuery, 2) == 1){
					printSeq(seq, OUT_SCH);
					(*nbrFound)++;
				}
			break;

			// Search subsequences
			case 3 :
				if(searchSubSeq(&seq, opt.searchQuery) == 1){
					printSeq(seq, OUT_SCH);
					nbrFound++;
				}
			break;
		}
	}
}