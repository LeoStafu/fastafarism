#include "fastafarism.h"


Sequence initialiseSequence(Sequence s) {
    s.content = NULL;
    s.comment = NULL;
    return s;
}


void freeSequence(Sequence s) {
    if (s.content != NULL)
        free(s.content);
    if (s.comment != NULL)
        free(s.comment);
}

//=====================================================================================================================
// Translation of a sequence into an amino acid sequence.
//=====================================================================================================================
Sequence translateSeq(char *code, Sequence s, int start){
    int i, j, k=0;
    size_t sizeContent = strlen(s.content);
    Sequence seqOUT;
    seqOUT.comment = strdup(s.comment);
    seqOUT.content = malloc(sizeContent/3 + sizeContent % 3 + 5);
    seqOUT.content[0] = '\0';
    char base;
    int res;
    int unknown;

    for(i=start ; i<sizeContent ; i += 3){
        res = 0x000000;
        unknown = 0;
        for(j=0 ; j<3 && i+j<sizeContent ; j++){
            base = s.content[i+j];
            switch(base) {
            	case 'T':
            		res += 0;
                    break;
                case 'U':
                    res += 0;
                    break;
                case 'C':
                    res = res | (0x01 << ((2-j) * 2));
                    break;
                case 'A':
                    res = res | (0x02 << ((2-j) * 2));
                    break;
                case 'G':
                    res = res | (0x03 << ((2-j) * 2));
                    break;
                default:
                    unknown = 1;
                    break;
            }
        }
        if (unknown == 0) {
            (seqOUT.content)[k] = code[res];
            k++;
        } else {
            printf("?");
        }
    }
    seqOUT.content[k] = '\0';
    return seqOUT;
}


//=====================================================================================================================
// Return the reverse complement of a sequence.
//=====================================================================================================================
Sequence reverseCmpl(Sequence *s){

    Sequence res;
    res.comment = strdup(s->comment);
    res.comment[strlen(res.comment)-1] = '\0';
    res.comment = (char*)realloc(res.comment, strlen(s->comment)+21);
    strcat(res.comment, " : ReverseComplement\n");

    int i;
    int j=0;
    res.content = malloc(strlen(s->content)+1);

    for(i=strlen(s->content)-1; i>=0; i--){

        switch(s->content[i]){
            case 'A' : res.content[j]='T'; break;
            case 'T' : res.content[j]='A'; break;
            case 'C' : res.content[j]='G'; break;
            case 'G' : res.content[j]='C'; break;
        }
        j++;
    }
    res.content[j] = '\0';
    return res;
}


//=====================================================================================================================
// Translation of a sequence in 6 different frames.
//=====================================================================================================================
void doTranslation(Sequence *seqToTranslate, int optTranslate, FILE* OUT){


	char code[64] = {'F','F','L','L','S','S','S','S','Y','Y','*','*','C','C','*','W',
	                'L','L','L','L','P','P','P','P','H','H','Q','Q','R','R','R','R',
	                'I','I','I','M','T','T','T','T','N','N','K','K','S','S','R','R',
	                'V','V','V','V','A','A','A','A','D','D','E','E','G','G','G','G'};

	int i = 0;	
	Sequence seqTrad = initialiseSequence(seqTrad);

	for(i = 0; (i < optTranslate) && (i < 3) ; i++){
		seqTrad = translateSeq(code, *seqToTranslate, i);
		seqTrad.comment[strlen(seqTrad.comment)-1] = '\0';
		seqTrad.comment = (char*)realloc(seqTrad.comment, strlen(seqTrad.comment) + 13);
		switch(i){
			case 0 : strcat(seqTrad.comment, " : Frame +1\n"); break;
			case 1 : strcat(seqTrad.comment, " : Frame +2\n"); break;
			case 2 : strcat(seqTrad.comment, " : Frame +3\n"); break;
		}
		printSeq(seqTrad, OUT);
        if (seqTrad.content != NULL)
            free(seqTrad.content);
        if (seqTrad.comment != NULL)
            free(seqTrad.comment);
	}
		
	if(optTranslate > 3){
		//If we need to translate in more
        Sequence revCmpl;
		revCmpl = initialiseSequence(revCmpl);
        revCmpl = reverseCmpl(seqToTranslate);
		//printf("%s", revCmpl.comment);
		//printf("%s\n", revCmpl.content);

		for(i = 0; i < (optTranslate - 3); i++){
			seqTrad = translateSeq(code, revCmpl, i);

			seqTrad.comment[strlen(seqTrad.comment)-1] = '\0';
			seqTrad.comment = (char*)realloc(seqTrad.comment, strlen(seqTrad.comment) + 15);
			switch(i){
				case 0 : strcat(seqTrad.comment, " : Frame -1\n"); break;
				case 1 : strcat(seqTrad.comment, " : Frame -2\n"); break;
				case 2 : strcat(seqTrad.comment, " : Frame -3\n"); break;
			}
			printSeq(seqTrad, OUT);
            freeSequence(seqTrad);
		}
        freeSequence(revCmpl);
	}
}