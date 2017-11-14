#include "fastafarism.h"



//=====================================================================================================================
// Print a sequence in an output file.
//=====================================================================================================================
void printSeq(Sequence s, FILE* OUT){

	fprintf(OUT, "%s",s.comment);
	int i;

	for(i = 0; i<strlen(s.content); i++){		
		if(i % 60 == 0 && i != 0){
			fprintf(OUT, "\n");
		}
		fprintf(OUT, "%c",(s.content)[i]);
	}
	fprintf(OUT, "\n");
}


//=====================================================================================================================
// Read options.
// Return 1 if problem, 0 if ok.
//=====================================================================================================================
int readOpt(int argc, char *argv[], Options *opt){

	int o;
	
	//===== OPTIONS DEFAULT VALUES =====
	opt->filename = NULL;
	opt->typeSearch = 0;
	opt->searchQuery = NULL;
	opt->doTranslate = 0;
	opt->occurrence = malloc(sizeof("1"));
	strcpy(opt->occurrence,"1");
	opt->doAssembly = 0;

	if(DEBUG){ printf("Option initialisées.\n"); }

	while ((o = getopt (argc, argv, "af:hn:q:s:t:")) != -1){
    	switch (o){	
			case 'a' : opt->doAssembly = 1; break;
			case 'f' : opt->filename = optarg; break;
			case 'h' : 
				printf("Usage : ./a.out [OPTION (+ arg)] ...\n");
				printf("\t-a       \tSequencing and assembly from the first sequence in FASTA file.\n");
				printf("\t-f FILE  \tSpecify a file to get sequences from (needed).\n");
				printf("\t-h       \tDisplay this message.\n");
				printf("\t-n NUMBER\tNumber of occurences wanted when research performed.\n");
				printf("\t-q QUERY \tSpecify the querry to match for research (needed if -s).\n");
				printf("\t-s NUMBER\tSpecify the type of research.\n");
				printf("\t         \t1 : research in sequences' name\n");
				printf("\t         \t2 : research in sequences\n");
				printf("\t         \t3 : search a sub-sequence in sequences\n");
				printf("\t         \t4 : search a whole sequence in dictionary\n");
				printf("\t         \t5 : number of prefixes in dictionary\n");
				printf("\t-t NUMBER\tSpecify the number of frame for translation and translate.\n");
				exit(0);
			case 'n': opt->occurrence = optarg; break;
			case 'q': opt->searchQuery = optarg; break;
			case 's': opt->typeSearch = atoi(optarg); break;
			case 't': opt->doTranslate = atoi(optarg); break;
			case '?':
      			if (optopt == 'q' || optopt == 'f' || optopt == 's' || optopt == 'n' || optopt == 't')
          			fprintf (stderr, "Option -%c requires an argument.\nFor help : ./a.out -h\n", optopt);
       			else if (isprint (optopt))
          			fprintf (stderr, "Unknown option -%c.\nFor help : ./a.out -h\n", optopt);
       			return 1;
      		default: break;
    	}
	}

	// If only filename, nothing to do
	if(opt->filename && opt->typeSearch == 0 && opt->doTranslate == 0 && opt->doAssembly == 0){
		fprintf(stderr, "Problem : Nothing to do. Specify other arguments.\nFor help : ./a.out -h\n"); 
		return 1;
	}

	// typesearch must be between [0-5]
	if(opt->typeSearch < 0 || opt->typeSearch > 5){
		fprintf(stderr, "Problem : Invalid search Type.\nFor help : ./a.out -h\n"); 
		return 1;
	}

	// typeSearch needs a searchQuery
	if(opt->typeSearch > 0 && !opt->searchQuery){
		fprintf(stderr, "Problem : Research needs a query.\nFor help : ./a.out -h\n"); 
		return 1;
	}

	// searchQuery needs a typeSearch
	if(opt->typeSearch == 0 && opt->searchQuery){
		fprintf(stderr, "Problem : You gave a query but no typeSearch specified. No research performed.\nFor help : ./a.out -h\n"); 
		return 1;
	}

	// doTranslate must be between [0-6]
	if(opt->doTranslate < 0 || opt->doTranslate > 6){
		fprintf(stderr, "Problem : Invalid number of frame for translation. Must be beetween 1 and 6.\nFor help : ./a.out -h\n"); 
		return 1;
	}

	return 0;
}


//=====================================================================================================================
// Read the length of the longest line in filename.
//=====================================================================================================================
int getMaxLineLength(char* filename){ 

	int max;
	
	char *command = "wc -L " ;
	char *command2 = (char *) malloc( strlen(command)*sizeof(char) + strlen(filename)*sizeof(char) + 1 );
	strcpy(command2,command);
	strcat(command2,filename);
	FILE *fcmd = popen(command2, "r");
	if (fcmd == NULL) {
		printf("Failed to run command wc -L\n" );
		exit(1);
	}else{
		fscanf(fcmd, "%d ", &max);	
		if(DEBUG){ printf("Max : %d\n", max); }
	}

	free(command2);
	fclose(fcmd);
	//by security, higher value
	return max+2;
}


//=====================================================================================================================
// Read FASTA file and stock the current sequence in a struct.
// Return 0 if meet a new sequence after the current one.
//=====================================================================================================================
int readSeq(FILE *fd, Sequence *s, int maxLength) {

	char line[maxLength];
	if (s->content)
		free(s->content);
	if (s->comment)
		free(s->comment);
	s->comment = NULL;
	s->content = NULL;

	// Get Comment
	if(fgets(line,maxLength,fd) == NULL) { err(1,"fgets failure"); }
	if(line[0] == '>'){            
		s->comment = strdup(line);
		if (s->comment == NULL) { err(1,"s.comment : stdrup failure"); }
	} 

	// Get sequence (content)	
	while(fgets(line,maxLength,fd) != NULL){ 
		
		if(line[0] == '>') {
			fseek(fd,-strlen(line),SEEK_CUR);
			return 0;
		}
		
		line[strlen(line)-1] = '\0';     //replace /n in the sequence file with /0

		if (s->content == NULL){
			s->content = strdup(line);
			if (s->content == NULL) { err(1,"s.content : stdrup failure"); }
		} else { 
			s->content = (char*)realloc(s->content, sizeof(char)*strlen(s->content) + sizeof(char)*strlen(line) +1);
			if (s->content == NULL){ err(1,"s.content : realloc failure"); }
			s->content = strcat(s->content, line);
		}
	}
	return 1;
}