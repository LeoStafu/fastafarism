# UE Programmation II - L3BI 2016/2017 - Sandrine Fartek and Léonard Dubois


Project has been called "Fastafarism" then it is better to compile it with :

`gcc *.c -Wall -o fastafarism`


Configuration is made using option in the command line. There is no specific order
-h option will print the following help message of how options work:

Usage : 
* ./a.out [OPTION (+ arg)] ...
* -a       	Sequencing and assembly from the first sequence in FASTA file.
* -f FILE  	Specify a file to get sequences from (needed).
	-h       	Display this message.
	-n NUMBER	Number of occurences wanted when research performed.
	-q QUERY 	Specify the querry to match for research (needed if -s).
	-s NUMBER	Specify the type of research.
	         	1 : research in sequences' name
	         	2 : research in sequences
	         	3 : search a sub-sequence in sequences
	         	4 : search a whole sequence in dictionary
	         	5 : number of prefixes in dictionary
	-t NUMBER	Specify the number of frame for translation and translate.



Results of research in input file (-s 1|2|3) and translation of input file are printed in specific new fasta files (with explicit names)
Dictionary research (whole sequence or prefixes number) print results directly in terminal

The archive contains two fasta files for test.
S_pombe.fasta is far much bigger. For small tests, use testShortSeq.fasta
