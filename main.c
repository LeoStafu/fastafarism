#include "fastafarism.h"


int main(int argc, char *argv[]) {

	//=================================================== GET OPTIONS, TEST FASTA AND GET MAX LINE LENGTH =======================
	struct Options opt;

	// Read options
	if(readOpt(argc, argv, &opt)) {
		fprintf(stderr, "Problem occured while reading options.\n");
		exit(1);
	}

	// Print all options if DEBUG activated
	if(DEBUG) {
		printf("Filename : %s\n", opt.filename);
		printf("TypeSearch = %d\n",opt.typeSearch );
		printf("Query : %s\n", opt.searchQuery);
		printf("Translate : %d\n", opt.doTranslate);
		printf("Occurrence : %s\n", opt.occurrence);
	}

	if(opt.filename == NULL){
		fprintf(stderr, "No filename given in parameters.\n");
		exit(1);
	}
	
	// Get max length in order to read lines and sequences properly
	int maxLength = getMaxLineLength(opt.filename);
	
	// Test if file is of FASTA type
	if(!isFasta(opt.filename, maxLength)) {
		fprintf(stderr, "Problem : File is no FASTA file.\n");
		exit(1);
	}else if(DEBUG) {
		printf("File is a FASTA file.\n");
	}

	//=========================================== INITIALISATIONS ===========================================================
	
	// Opening FILES
	FILE *IN ;
	IN = fopen(opt.filename, "r");
	if(IN == NULL) {
		fprintf(stderr, "Problem reading file %s.\n", opt.filename);
		exit(1);
	}

	FILE* OUT_TRANS = NULL;
	if(opt.doTranslate != 0) {
		OUT_TRANS = fopen("outTranslation.fasta", "w");
		if(OUT_TRANS == NULL){
			fprintf(stderr, "Problem opening output file.\n");
			exit(1);
		}
	}

	FILE* OUT_SCH = NULL;
	if(opt.typeSearch != 0) {	
		OUT_SCH = fopen("outResearch.fasta", "w");
		if(OUT_SCH == NULL){
			fprintf(stderr, "Problem opening output file.\n");
			exit(1);
		}
	}

	FILE *OUT_SQC = NULL;
	if(opt.doAssembly != 0) {
		OUT_SQC = fopen("outAssembly.fasta", "w");
		if(OUT_SQC == NULL){
			fprintf(stderr, "Problem opening output file.\n");
			exit(1);
		}
	}

	// Structure containing sequences of the FASTA file
	Sequence s1;
	s1.comment = NULL;
	s1.content = NULL;

	// Structure for dictionnary
	nodeDico* root = malloc(sizeof(nodeDico));
	initNodeDico(root);

	int nbrFound =0;
	

	//=========================================== EXECUTE REQUESTED QUERIES =======================================================
	
	do{	
		readSeq(IN, &s1, maxLength);
		
		// Translation
		if(opt.doTranslate != 0) {
			doTranslation(&s1, opt.doTranslate, OUT_TRANS);
		}
		// If search in dico, better create dico first ;)
		if(opt.typeSearch == 4 || opt.typeSearch == 5) {			
			insertionDico(root, s1.content);
		}
		// Search (exept in dictionnary)
		if(opt.typeSearch > 0 && opt.typeSearch < 4) {			
			doSearch(s1, opt, OUT_SCH, &nbrFound);
		}
	}while(!feof(IN));

	//Search whole sequence in dictionnary
	if(opt.typeSearch == 4 ) {	
		if(searchDico(root, opt.searchQuery, 0))
			printf("\nQuery sequence is in the dictionnary !\n");
		else
			printf("\nQuery sequence is not in the dictionnary !\n");
	}

	//Search number of prefix in dictionnary
	if(opt.typeSearch == 5) {
		printf("\nNumber of sequences with prefix %s : %d!\n", opt.searchQuery,  searchDico(root, opt.searchQuery, 1));
	}

	//Sequencing for the 1st sequence in the file
	if(opt.doAssembly ==1){
		size_t i = 0;
		fseek(IN,0,SEEK_SET);
		readSeq(IN, &s1, maxLength);
		int b = 15;
		int size = strlen(s1.content);
		int nReads  = (size*b)/SIZEREADS;
		char **reads = randomReads(s1.content,SIZEREADS,b);
		printContigs(reads,nReads,size, OUT_SQC);
		while (i < nReads)
		{
			free(reads[i]);
			i++;
		}
		free(reads);
	}

	//=========================================== FREE AND CLEAN =======================================
	
	// Close files
	fclose(IN);
	if(opt.doTranslate != 0){
		fclose(OUT_TRANS);	
	}
	if(opt.typeSearch != 0){
		fclose(OUT_SCH);
	}
	if(opt.doAssembly != 0) {
		fclose(OUT_SQC);
	}

	// Free dictionnary
	if(root != NULL){
		freeDico(root);
	}

	// Other free
	if (opt.occurrence)
		free(opt.occurrence);
	if (s1.comment)
		free(s1.comment);
	if (s1.content)
		free(s1.content);	

	return 0;
}

