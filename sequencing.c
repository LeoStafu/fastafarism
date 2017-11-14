#define _GNU_SOURCE
#include "fastafarism.h"


//=====================================================================================================================
// Generate random reads from a sequence and store them in a string array.
//=====================================================================================================================
char **randomReads(char *sequence, int sizeReads, int b) {
	int i;
	int size = strlen(sequence);
	int nReads = (size*b)/sizeReads;
	uint16_t buf;

	char **reads = calloc(nReads,sizeof(char*));

	FILE *fd;
	fd = fopen("/dev/urandom", "r");
	if(fd == NULL) { 
		fprintf(stderr, "Problem occured while opening /dev/urandom.\n");
		exit(1);
	};

	for(i = 0; i < nReads; i++) {
		fread(&buf,sizeof(buf),1,fd) ;
		reads[i] = strndup(sequence+(buf%size),sizeReads);
	}

	fclose(fd);
	return reads;
}


//=====================================================================================================================
// Return the maximum elongation from reads[A].
//=====================================================================================================================
char *assembly(char **reads, int nReads, int sizeSeq, int A, int strin) {

	char *result = calloc((sizeSeq+1), sizeof(char)); //result is, in best case as long as the original seq
	strcpy(result,reads[A]);
	strcpy(reads[A],"\0");
	char *prefixA;
	char *suffixA;
	char *pre = NULL;
	char *suf = NULL;
	int B;
	int flag = 1;                // flag if embroidery happend during this browse = 1


	// Case strlen(reads[A]) < strin
	if(strlen(result) < strin){
		return result;
	}

	while(flag != 0) {

		flag = 0;

		for(B= A+1 ; B<nReads ; B++){
			prefixA = strndup(result,strin);
			suffixA = strndup(result+strlen(result)-strin, strin);
			pre = strcasestr(reads[B],prefixA);
			suf = strcasestr(reads[B],suffixA);
			free(prefixA);
			free(suffixA);

			// Case reads[B] is a subsequence of result
			if(strcasestr(result,reads[B])) {
				strcpy(reads[B],"\0");
				continue;
			}

			// Prefix assembly if found
			if(pre != NULL) {
				flag = 1;
				// Concatenation
				memmove(result+strlen(reads[B])-strlen(pre), result, strlen(result));  // Strcpy had unpredicted behavior when overlaps  
				strncpy(result,reads[B],strlen(reads[B]));
				// Erase
				strcpy(reads[B],"\0");
				pre = NULL;
				continue;
			}
				
			// Suffix assembly if found
			if(suf != NULL) {
				flag = 1;
				// Concatenation
				strcpy(result+strlen(result)-strin,suf);
				// Erase
				strcpy(reads[B],"\0");
				suf = NULL;
				continue;
			}
		}
	}
	if(DEBUG){ printf("%s\n",result); }
	return result;
}


//=====================================================================================================================
// Print contigs returned by function assembly in a file. (multiple if gaps in randomReads)
//=====================================================================================================================
void printContigs(char **reads, int nReads, int sizeSeq, FILE* OUT_SQC) {

	int A;
	int strin = 15;     // stringency for assembly of 2 reads
	int countContigs = 1;
  
	Sequence s2;
	s2.content = NULL;
	s2.comment = malloc(16*sizeof(char));

	for(A = 0; A < nReads ; A++){

		if(DEBUG){
			int i;
			printf("START %d : Available reads\n",A);
			for (i = 0; i < nReads; i++){
				if(strcmp(reads[i],"\0") != 0) { printf("%s\n",reads[i]); }
			}
			printf("END %d\n",A);
		}

		if ((strcmp(reads[A], "\0") == 0)) { continue; }
		sprintf(s2.comment, ">Contig %d\n", countContigs);
		countContigs++;
		s2.content = assembly(reads,nReads,sizeSeq,A,strin);
		printSeq(s2,OUT_SQC);
	}

	if (s2.comment) { free(s2.comment); }
	if (s2.content) { free(s2.content); }
}