//Alex Pospesel
//ID: 001146589
//NetID: ap212246
//Email: apospesel@albany.edu
//CSI 402
//Program 5
//Due 5/8/16

#include "headers.h"

void die(char * message){
	fprintf(stderr, "%s\n", message);
	fflush(stderr);
	exit(1);
}

void debug(char * message){
	printf("%s\n", message);
	fflush(stdout);
}

void checkPoint(){
	printf("%s\t%d\n", __FILE__, __LINE__);
	fflush(stdout);
}

FILE * checkOpen(char * name, char * mode, char * failMessage){
	FILE * file = fopen(name, mode);
	if(file == NULL){
		die(failMessage);
	}else{
		return file;
	}
}

void * checkMalloc(size_t s){
	void * p = malloc(s);
	if(p == NULL){
		die("Malloc failed.");
	}else{
		return p;
	}
}