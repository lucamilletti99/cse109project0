/* 
 * Program: prog0.c
 * Name: Luca Milletti
 * LIN: 810848667
 */

/* preprocessor directives (you can add libraries) */
#include <stdio.h>    
#include <string.h>  
#include <errno.h>    
#include <stdlib.h>   

/* global variables (I would prefer you not add more) */
static FILE *Ofile = NULL;  /* the static keyword makes the variable private; cannot be seen by other programs */

/* prototypes (you can add functions; these are all DE used) */
unsigned int parseArgs(int, char *[]);  /* notice how the variable names are not needed */
FILE *Fopen(const char *, const char *);
void parseFile(FILE *, unsigned int, unsigned int *, unsigned int *, unsigned int *); 
void printResults(char *[], unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void Fclose(FILE *);
void bail(int, const char *);
void help();

/* main (don't modify anything in main */
int main(int argc, char *argv[]) {
  
  unsigned int mode;
  FILE *Ifile1 = NULL;
  FILE *Ifile2 = NULL;
  unsigned int cc1, wc1, lc1, cc2, wc2, lc2;
  cc1 = wc1 = lc1 = cc2 = wc2 = lc2 = 0;
  
  mode = parseArgs(argc, argv);
  Ifile1 = Fopen(argv[1], "r");
  Ifile2 = Fopen(argv[2], "r");
  if (Ofile == NULL) Ofile = stdout;
  parseFile(Ifile1, mode, &cc1, &wc1, &lc1);
  parseFile(Ifile2, mode, &cc2, &wc2, &lc2);
  printResults(argv, cc1, wc1, lc1, cc2, wc2, lc2);
  Fclose(Ifile1);
  Fclose(Ifile2);
  Fclose(Ofile);
  
  if (wc1 + wc2 < 100) bail(30, "Fewer than 100 words total in input files");

  return 0;
}

/* functions */
unsigned int parseArgs(int argc, char *argv[]) {
  unsigned int mode = 0; //U is 1, L is 2, F is 3, 0 is no mode specified
  switch(argc){
    case 3: //2 input files
      break;
    case 5: //output file or mode specifed
      if (strcmp(argv[3], "-a") == 0){ //if mode is specified
        if(strcasecmp(argv[4], "U") == 0 || strcasecmp(argv[4], "L") == 0 || strcasecmp(argv[4], "F") == 0){ //compares two strings
          if (strcasecmp(argv[4], "U") == 0) mode = 1; //uppercase is mode 1
          if (strcasecmp(argv[4], "L") == 0) mode = 2; //lowercase is mode 2
          if (strcasecmp(argv[4], "F") == 0) mode = 3; //Flipped is mode 3
          break;
        }
        else{//U,L,F not input after -a
          bail(11, "invalid mode");
          break;
        }
      }
      else if(strcasecmp(argv[3], "-o") == 0){ //if file output is specified
        Ofile = Fopen(argv[4], "w");
        break;
      }
      else{ //-o or -a not input
        bail(10, "choices are '-o filename' or '-a mode'");
        break;
      }
    case 7: //mode and output file specified
        if (strcmp(argv[3], "-a") == 0 && strcmp(argv[5], "-o") == 0){ //if -a is first and -o is second
          Ofile = Fopen(argv[6], "w");
          if(strcasecmp(argv[4], "U") == 0 || strcasecmp(argv[4], "L") == 0 || strcasecmp(argv[4], "F") == 0){
            if (strcasecmp(argv[4], "U") == 0) mode = 1;
            else if (strcasecmp(argv[4], "L") == 0) mode = 2;
            else if (strcasecmp(argv[4], "F") == 0) mode = 3;
            break;
          }
          else{//U,L,F not input after -a
            bail(11, "invalid mode");
            break;
          }
        }
        else if(strcmp(argv[3], "-o") == 0 && strcmp(argv[5], "-a") == 0){ //if -o is first and -a is second
          Ofile = Fopen(argv[4], "w");
          if(strcasecmp(argv[6], "U") == 0 || strcasecmp(argv[6], "L") == 0 || strcasecmp(argv[6], "F") == 0){
            if (strcasecmp(argv[4], "U") == 0) mode = 1;
            else if (strcasecmp(argv[4], "L") == 0) mode = 2;
            else if (strcasecmp(argv[4], "F") == 0) mode = 3;
            break;
          }
          else{
            bail(11, "invalid mode");
            break;
          }
          break;
        }
        else{ //-o or -a not input
          bail(10, "choices are '-o filename' or '-a mode'");
          break;
        }
      break;
    default: //not enough arguments or incomplete argument
      help();
      break;
  }
  return mode;
}

void parseFile(FILE *file, unsigned int mode, unsigned int *cc, unsigned int *wc, unsigned int *lc) {
  char ch = NULL;
  while((ch = fgetc(file))!= EOF){ //while we aren't at the "end of file" terminator character "\0"
    *cc = *cc + 1; //everytime a character is read, the character count increases
    if(ch == '\n'){ //if a newline is processed, linecount is incremented as well as word count
      *lc = *lc + 1; //everytime a newline is processed, the line count is increased
      *wc = *wc +1; //also the word count is increased
    } 
    else if(ch == ' '||ch == '\t') *wc = *wc + 1; //if a tab or space is detected, a word count is incremented
    
    if (mode == 0) fputc(ch, Ofile); //writes character to the end of the file
    else if(mode == 1){ //'U' is input mode uppercase
      fputc(toupper(ch), Ofile);
    }
    else if (mode == 2){ //'L' is input mode lowercase
      fputc(tolower(ch), Ofile);
    }
    else if(mode == 3){ //'F' is input mode flipped
      if (ch >= 97 && ch <= 122) fputc(toupper(ch),Ofile);//use ASCII values to change lowercase to uppercase
      else if (ch >= 65 && ch <= 90) fputc(tolower(ch),Ofile); 
      else fputc(ch,Ofile); //if it is a non alphabetical character
    }
    
  }
}

void printResults(char *argv[], unsigned int cc1, unsigned int wc1, unsigned int lc1, unsigned int cc2, unsigned int wc2, unsigned int lc2) {
  fprintf(stdout, "%d %d %d %s\n",lc1,wc1,cc1,argv[1]); //prints to Stdout all of the necessary elements
  fprintf(stdout, "%d %d %d %s\n",lc2,wc2,cc2,argv[2]);
}

FILE *Fopen(const char *path, const char *mode) { 
  FILE *fp = NULL;
  if((fp=fopen(path, mode)) != NULL) return fp; //error checks that fp is not Null
  else{ //if it is null, this file open error is printed to stderr
   bail(20, "File open error\n");
   exit(20);
  }
}

void Fclose(FILE *fp) { 
  if(fclose(fp)!= 0){ //error checks to see if fclocse(fp) correctly closes File *fp
    bail(21, "File close error\n"); 
    exit(21); 
  }
}

void bail(int rc, const char *msg) {  //bails with code RC and message 'msg'
  fprintf(stderr, "(%d) %s\n",rc,msg); //made sure to print the error msg to stderr
  exit(rc);
}

void help() { //this function was taken directly from Prof Erle's lecture
  fprintf(stdout, "prog0 concatenates two text files and counts the number of lines, words, and chars in each file.\n\n");
  fprintf(stdout, "By default, the concatenated files and the number of lines, words, and chars are written to stdout.\n\n");
  fprintf(stdout, "If the user passes '-o filename' on the command line, the program will write the concatenated\n");
  fprintf(stdout, "files to a file named 'filename'. The number of lines, words, and chars still appear on stdout.\n\n");
  fprintf(stdout, "If the user passes '-a mode' on the command line, where mode is either 'U', 'L', or 'F', the case \n");
  fprintf(stdout, "of the characters in the input files are either converted to uppercase, lowercase, or flipped.\n\n");
  fprintf(stdout, "If no arguments are provided or if the wrong number of arguments are provided, this help message\n");
  fprintf(stdout, "is provided on stdout.\n\n");
  fprintf(stdout, "The program is called as follows with the order of the options reversible.\n\n");
  fprintf(stdout, "prog0 file1 file2 [-o filename] [-a mode] \n\n");
  fprintf(stdout, "Return codes are as follows. Any error message is printed to stderr.\n");
  fprintf(stdout, "0 - normal execution (even if it results in help message)\n");
  fprintf(stdout, "10 - invalid option\n");
  fprintf(stdout, "11 - invalid mode\n");
  fprintf(stdout, "20 - file open error\n");
  fprintf(stdout, "21 - file close error\n");
  fprintf(stdout, "30 - fewer than 100 total words in both files\n\n");
  exit(0);
}
