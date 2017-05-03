//Alex Pospesel
//ID: 001146589
//NetID: ap212246
//Email: apospesel@albany.edu
//CSI 402
//Program 5
//Due 5/8/16

//PART A

#include "headers.h"
#include "prototypes.h"

/*
 *
 *
 *
 *
 EXTRA CREDIT WITH Binary Search TREE!
 *
 *
 *
 *
 */

int main (int argc, char *argv[]){
	
	//check command line args
	if((argc == 3) && (strcmp("-s", argv[2]) != 0)){
		die("Invalid flag.");
		
	}else if(argc > 3){
		
		die("Too many arguments.");
	}else if(argc == 1){
		die("Not enough arguments.");
	}

	//append a '/' to the end of the filepath given
	char* path = checkMalloc(sizeof(char) * (strlen(argv[1]) + 1));
	sprintf(path, "%s/", argv[1]);

	//printf("Path is: %s\n", path);

	//try to open the directory
	DIR *directory;
	if((directory = opendir(path)) == NULL){
		die("Invalid directory path.");
	}
	
	struct dirent *directoryPath;

	struct stat statBuffer;
	
	treeNode *rootPtr = NULL;
	
	char temp[256];
	
	//needed for date format
	struct tm *lastModDate;

	char lastModDateBuf[256];

	//print name, size and date of all hidden files
	while((directoryPath = readdir(directory)) != NULL){
		//check first letter for "."
		if(directoryPath->d_name[0] == '.'){
			if((strcmp(".", directoryPath->d_name) != 0) && (strcmp("..", directoryPath->d_name) != 0)){

				//not . or ..
				//append file name to file path
				sprintf(temp, "%s%s", path, directoryPath->d_name);

				if(stat(temp, &statBuffer) < 0){
					//error in making stat of file
					fprintf(stderr, "Error in stat function for: %s\n", directoryPath->d_name);
				}else{
					//save fileSize to an int for easier access
					int fileSize = (intmax_t)statBuffer.st_size;
					
					//get time in specific form (month, day, year)
					lastModDate = localtime(&statBuffer.st_mtime);
					
					strftime(lastModDateBuf, 256, "%B %d %Y", lastModDate);
					
					//save files into a bst for extra credit!
					rootPtr = insertNode(rootPtr, directoryPath->d_name, fileSize, lastModDateBuf);
				}
				
				//clear temp for new string
				strcpy(temp, "");
			}
		}
	}
	
	//print bst
	printBST(rootPtr);
	
	//exit with no issues
	exit(0);
}

/*******************************************************
*****************EXTRA CREDIT***************************
********************************************************/
treeNode* insertNode(treeNode *node, char* fileName, int fileSize, char* date){
	
	//if node is null then create a new node
	if(node == NULL){
		//make node and allocate memory
		treeNode *new_node;
		new_node = (treeNode *)malloc(sizeof(treeNode));

		//check if malloc fails
		if(new_node == NULL){
			printf("Malloc error.");
			exit(1);
		}

		//set the fields
		
		//fileName
		strcpy(new_node->fileName, fileName);
		
		//fileSize
		new_node->logicalSize = fileSize;
		
		//file modify date
		strcpy(new_node->date, date);
		
		new_node->leftChild = NULL;
		new_node->rightChild = NULL;
		
		//return new_node pointer
		return new_node;
		
		
	//tree not empty
	}else{
		//if data is less than for string compare
		if(fileSize > node->logicalSize){
			node->rightChild = insertNode(node->rightChild, fileName, fileSize, date);
			
		}else{
			//must be greater than
			node->leftChild = insertNode(node->leftChild, fileName, fileSize, date);	
		}
	}
}

//recurseivly print the BST
void printBST(treeNode* node){
	if(node == NULL){
		return;
	}
	printBST(node->leftChild);
	printf("%s\t\t%d\t\t%s\n", node->fileName, node->logicalSize, node->date);
	//printf("%d\t%s\t%s\n", node->logicalSize, node->date, node->fileName);
	printBST(node->rightChild);
}