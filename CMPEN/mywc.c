/********************************** 	 
*		
*      			mywc
*      File: 	mywc.c
*    Author: 	Yehya Awad
*    E-mail:	yehya@psu.edu
*    Course: 	CMPEN 441
* Professor: 	Dr. Meng Su
*      Date:    8/27/2015
*  Tab Size: 	4
*
*
* Description *
*
* Program that counts the words, lines, and characters in a file.
*
* Compiling *
*
* Command I used: gcc -o mywc Lab-1-Part-2.c
*
* Usage (in shell) *
*
* ./executableName [file name...] [options ex. -l -m]
* For example if the executable name is 'mywc' then
* 	
*		./mywc [file name..] [-l -w...]
* Ex. 	./mywc myfile.txt 
* Ex. 	./mywc myotherfile.txt -l -w
* Ex. 	./mywc -l -w labhomework.c   (order doesn't matter)
*
* Other notes *
*
* 	- Order of the options and file name doesn't matter.
* 	- Only one filename is accepted.
*
* Testing *
* 
* 	- Tested with multiple files including this one to match output with
*		stndard unix util wc command.
* 
*		wc command: wc Lab-1-Part-2.c
*    				206     806    4824 Lab-1-Part-2.c
*	  mywc command: ./mywc Lab-1-Part-2.c
*					File: Lab-1-Part-2.c
*					Lines: 206
*					Chars: 4824
*					Words: 806
* 
* Academic Integrity *
*
* Academic Integrity Statement: I certify that, while others may have assisted me 
* in brain storming, debugging and validating this program, the program itself 
* is my own work. I understand that submitting code which is the work of other 
* individuals is a violation of the course Academic Integrity Policy and may 
* result in a zero credit for the assignment, or course failure and a report to 
* the Academic Dishonesty Board. I also understand that if I knowingly give my 
* original work to another individual that it could also result in a zero 
* credit for the assignment, or course failure and a 
* report to the Academic Dishonesty Board.
* 
* Assisted by and Assisted line numbers: None
* Your name: Yehya Awad
* Your section: 002
*
**********************************/

/* Bool data type */
typedef int bool;
#define true 1
#define false 0

/* Header files */
#include <stdio.h> 

/* Prototypes */
void process(); // processes file and arguments
void count(); // counts words, lines, and files
void report(); // prints result 

/* Main function 
* @param argc : Number of arguments passed when running file
* @param argv : Array holding char arguments
*/
int 
main (int argc, char *argv[]) {

	// output variables
	int res_lines, res_words, res_chars;
	// file pointer
	FILE *myfile; 

	if (argc == 1) // if no filename is passed
	{
		printf("Error: Missing file name.\n");
	} else if (argc > 5) // if there are more than 5 arguments
	{
		printf("Error: There are too many arguments.\n");
	} else 
	{
		process(myfile, argc, argv);
	}
	
	return 0;
}

/** Processes file and arguments
* @param file : The file being processed
* @param argc : Argument count
* @parma argv : Arguments array
*/
void 
process(FILE * file, int argc, char *argv[]) {
	// r for reading mode
	const char *mode = "r"; 
	// options -l -w -m
	bool opt_l = false, opt_w = false, opt_m = false, fileFound = false;

	int i;
	for (i = 1; i < argc; ++i)
	{
		// If not an option then it is the file name
		if (argv[i][0] != '-')
		{
			if (fileFound) { // If a strange argument is passed in
				printf("Error: Invalid argument '%s' passed.\n", argv[i] );
				return;
			}
			// printf("%s\n",argv[i]);
			file = fopen(argv[i],mode);
			if (file == NULL && !fileFound) // if file does not exist
			{
				printf("Error: File %s does not exist.\n", argv[i] );
				return;
			}
			fileFound = true;
		} else if (argv[i][0] == '-')
		{
			const char tempOption = argv[i][1];
			switch (tempOption) {
				case 'l': 
					opt_l = true;
					break;
				case 'w': 
					opt_w = true;
					break;
				case 'm': 
					opt_m = true;
					break;
				default: 
					printf("Error: Invalid option passed.\n" );
					printf("Usage: Options available: \n\n" );
					printf("-l : Print Lines count\n" );
					printf("-w : Print Words count\n" );
					printf("-m : Print Characters count\n" );
					return;
			}
		}
	}

	count(argv, file, opt_l, opt_m, opt_w);

	return;
}

/** Counts lines, words, and characters 
* @parma argv : Arguments array
* @param file : the file being counted
* @param opt_l : option to print lines
* @param opt_m : option to print characters
* @param opt_w : option to print words
*/
void 
count(char *argv[], FILE * file, bool opt_l, bool opt_m, bool opt_w) {
	int c_lines = 0, c_words = 0, c_chars = 0, previousChar = ' ';

	while (true) {
		int currentChar = getc(file);

		/* Condition for counting words
		* Figured out how to match it to the 'wc' command.
		* Took a couple of tests to figure it out. Turns out it was the \t tabs 
		* that was putting it off a few. 
		*/
		bool wordCondition = ( (currentChar != ' ' && currentChar != '\t') && (previousChar == ' ' || previousChar == '\n' || previousChar == '\t')) ;

		if (feof(file)) {
			break;
		} else if (currentChar == '\n' ) {
			c_lines++;
		} else if (wordCondition)  {
			c_words++;
		}

		c_chars++;

		previousChar = currentChar;
	}

	report(argv, c_chars, c_lines, c_words, opt_l, opt_m, opt_w);

	return;
}

/** Prints the result of counting the given file 
* @parma argv : Arguments array
* @param c_chars : chars count being printed to console
* @param c_lines : lines count being printed to console
* @param c_words : words count being printed to console
* @param opt_l : option for printing lines
* @param opt_m : option for printing characters
* @param opt_w : option for printing words
*/
void 
report(char *argv[], int c_chars, int c_lines, int c_words, bool opt_l, bool opt_m, bool opt_w) {

	// This bool is true when no option is passed into the arguments
	const bool printAll = !(opt_l || opt_w || opt_m);

	// Printing file name
	printf("File: %s\n", argv[1]);

	// Printing Values
	if (opt_l || printAll)
	{
		printf("Lines: %i \n", c_lines );
	}
	if (opt_m || printAll)
	{
		printf("Chars: %i \n", c_chars );
	}
	if (opt_w || printAll)
	{
		printf("Words: %i \n", c_words );
	}
	
	return;
}
