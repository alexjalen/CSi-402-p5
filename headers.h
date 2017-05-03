//Alex Pospesel
//ID: 001146589
//NetID: ap212246
//Email: apospesel@albany.edu
//CSI 402
//Program 5
//Due 5/8/16

//include
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

//structs
typedef struct treeNode{
	struct treeNode *leftChild;
	struct treeNode *rightChild;
	char fileName[256];
	int logicalSize;
	char date[256];
}treeNode;

typedef struct files{
	int nameLength;
	unsigned int fileSize;
	char *fileName;
	char *fileContents;
	struct files *next;
}files;