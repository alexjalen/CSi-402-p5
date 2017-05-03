//Alex Pospesel
//ID: 001146589
//NetID: ap212246
//Email: apospesel@albany.edu
//CSI 402
//Program 5
//Due 5/8/16

//error.c file
void die(char * message);
void debug(char * message);
void checkPoint();
FILE * checkOpen(char * name, char * mode, char * failMessage);
void * checkMalloc(size_t s);

//p5a_main.c
treeNode* insertNode(treeNode *node, char* fileName, int fileSize, char* date);
void printBST(treeNode *node);

//p5b_main.c
void addFileCompress(char* fileName,int fileSize, files* fileListHead);
void addFileExtract(char* fileName, int fileSize, files* fileListHead);