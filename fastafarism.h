#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <getopt.h> //if use of getopt_long



#define DEBUG 0
#define SIZEREADS 150 //usually 150-250 for modern sequencers



typedef struct Options {
	char* filename;
	int typeSearch; //0 if not search, 1  search gene name, 2 search seq, 3 subseq, 4 serch seq in Dico, 5 search prefix in Dico
	char* searchQuery; 
	int doTranslate; //0 if not translate, else how much frames ?
	char* occurrence; //how many search results displayed 
	int doAssembly;
}Options; 


typedef struct Sequence {
	char *comment;
	char *content;
}Sequence ;


typedef struct nodeDico{
	int final;
	int prefix;
	struct nodeDico *nt[4];
}nodeDico;



//================= test functions ===================
int isFasta(char *filename, int maxLineLength);
int typeSeq(Sequence *seq);


//================= getInfo functions ===================
void printSeq(Sequence s, FILE* OUT);
int readOpt(int argc, char *argv[], Options * opt);
int getMaxLineLength(char* filename);
int readSeq(FILE *fd, Sequence *s, int maxLength);


//================= translate functions ===================
Sequence translateSeq(char *code, Sequence s, int start);
Sequence reverseCmpl(Sequence *s);
void doTranslation(Sequence *seqToTranslate, int optTranslate, FILE* OUT);


//================= search functions ===================
int searchSubSeq(Sequence *s, char* query);
int searchSeqOrName(Sequence *s, char *query, int typeSearch);
void doSearch(Sequence s, Options opt, FILE* OUT_SCH, int *nbrFound);


//================= sequencing functions ===================
char **randomReads(char *sequence, int sizeReads, int b);
char *assembly(char **reads, int nReads, int sizeSeq, int A, int strin);
void printContigs(char **reads, int nReads, int sizeSeq, FILE* OUT_SQC);


//================= dico functions ===================
void initNodeDico(nodeDico *node);
void insertionDico(nodeDico *root, char* seq);
int searchDico(nodeDico *root, char* seq, int type);
void freeDico(nodeDico *root);

