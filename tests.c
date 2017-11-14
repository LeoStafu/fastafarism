#include "fastafarism.h"


//=====================================================================================================================
// Test is filename is a fasta file. 
//=====================================================================================================================
int isFasta(char *filename, int maxLineLength){

	int testFasta = 1;
	int firstLetter;

	FILE *IN ;
	IN = fopen(filename, "r");
	if(IN == NULL){
		fprintf(stderr, "Problem : cannot open file %s\n", filename );
		return 0;
	}

	char *read = malloc(maxLineLength+1);
	
	while(!feof(IN)){
		fgets(read, maxLineLength +1, IN);
		firstLetter = (int)read[0];
		//if first letter is neither '>' nor a letter (capital or not) nor an empty line
		if(	(read[0] != '>') && (!isalpha(firstLetter)) && firstLetter !=10 )
		{
			testFasta = 0;
			printf("Coucou !\n");
		}
	}

	free(read);
	fclose(IN);
	return testFasta;
}


//=====================================================================================================================
// Test if f contains nucleic sequences. 
// Returns : 0 = nothing, 1 = dna, 2 = rna, 3 = protein.
//=====================================================================================================================
int typeSeq(Sequence *s){
	int c;
	int dna = 0;
	int rna = 0;
	int prot = 0;
	int i=0;

	while( (c = s->content[i]) != '\0' && dna+rna+prot<3) {
		if(!isalpha(c))
			return 0;
		if(!isupper(c))
			c = (char)toupper(c);
		if(c!='A' && c!='T' && c!='G' && c!='C' && c!='N') 
			dna = 1;
		if(c!='A' && c!='U' && c!='G' && c!='C' && c!='N')
			rna = 1;
		if(c=='B' || c=='J' || c=='O' || c=='U')
			prot = 1;
	}
	
	if(dna == 0) {
		return 1;
	}
	if(rna == 0) {
		return 2;
	}
	if(prot == 0) {
		return 3;
	}
		
	return 0;
}