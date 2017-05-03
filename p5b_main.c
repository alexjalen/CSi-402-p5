//Alex Pospesel
//ID: 001146589
//NetID: ap212246
//Email: apospesel@albany.edu
//CSI 402
//Program 5
//Due 5/8/16

//PART B

#include "headers.h"
#include "prototypes.h"

int main (int argc, char *argv[]){
	
	//check command line args
	if(argc < 3){
		die("Too few arguments.");
	}
	
	if((strcmp(argv[1], "-c") != 0) && (strcmp(argv[1], "-x") != 0) && (strcmp(argv[1], "-p") != 0)){
		die("Incorrect flag.");
	}
	
	//variables
	files* fileList = checkMalloc(sizeof(struct files));
	//set current link to null for empty link
	fileList->next = NULL;
	
	//saved for future return to begginning of list
	files* fileListHead = fileList;
	
	//stat buffer for file
	struct stat statBuffer;
	
	//fileSize variables
	int fileSize;
	
	//read flag
	if(strcmp(argv[1], "-c") == 0){
		//create archive and compress files inside
		//printf("-c\n");
		
		//printf("Number of args is: %d\n", argc);
		
		//loop files and add them to the fileList singly linked list
		int i = 3;
		for(i; i < argc; i++){
			
			//TESTING
			//printf("%s\n", argv[i]);
			
			
			if(stat(argv[i], &statBuffer) < 0){
					//error in making stat of file
					fprintf(stderr, "Error in stat function for: %s\n", argv[i]);
					die("");
				}else{
					//save fileSize to an int for easier access
					fileSize = (intmax_t)statBuffer.st_size;
					
					//add file to linked list
					
					addFileCompress(argv[i], fileSize, fileList);
				}
		}
		
		//get number of files inputted
		int numOfFiles = argc - 3;
		
		char numOfFilesString[1];
		
		sprintf(numOfFilesString, "%c", (char)numOfFiles);
		
		//nameLength string for write()
		char nameLengthString[1];
		unsigned char fileSizeString[5];
		
		//create/open the archive file
		//open file for reading
		int fileDesArchive;

		if((fileDesArchive = open(argv[2], O_WRONLY | O_CREAT, 0777)) == -1){
			die("Failed to create and open archive.");
		}
		
		//write number of files
		if((write(fileDesArchive, numOfFilesString, 1) == -1)){
			die("Failed to write number of files to archive.");
		}
		
		//move next to first link in list
		//fileList = fileList->next;
		
		//write fileName length and fileName for all files
		do{
			//move to next file
			fileList = fileList->next;
			
			sprintf(nameLengthString, "%c", (char)fileList->nameLength);
			
			//write nameLength
			if((write(fileDesArchive, nameLengthString, 1) == -1)){
				die("Failed to write nameLength.");
			}
			
			//TESTING
			//printf("fileName is: %s\n", fileList->fileName);
			
			
			//write fileName
			if((write(fileDesArchive, fileList->fileName, fileList->nameLength) == -1)){
				die("Failed to write fileName.");
			}
			
			//TESTING
			//printf("Size of file: %d\n", fileList->fileSize);
			
			//write fileSize
			if((write(fileDesArchive, &fileList->fileSize, 4) == -1)){
				die("Failed to write fileSize.");
			}
			
		}while(fileList->next != NULL);
		
		
		//go back to begginning of linked list
		fileList = fileListHead;
		
		//print data from fileList to archive
		do{
			//move to next file
			fileList = fileList->next;
			
			//write fileSize
			if((write(fileDesArchive, fileList->fileContents, fileList->fileSize) == -1)){
				die("Failed to write fileContents.");
			}
			
		}while(fileList->next != NULL);
		
		
		//close fileDesArchive
		close(fileDesArchive);
		
	}else if(strcmp(argv[1], "-x") == 0){
		//extract files from archive
		//printf("-x\n");
		//extract

		//open the archive file
		//open file for reading
		int fileReadArchive;

		//char numFilesStr[4];

		unsigned char numFilesChar;

		int numFiles;

		int i;
		
		if((fileReadArchive = open(argv[2], O_RDONLY)) == -1){
			die("Failed to open archive.");
		}

		//read first number for number of files present
		if((read(fileReadArchive, &numFilesChar, 1)) < 0){
			die("Failed to read number of files.");
		}

		//cast numFilesStr as an int
		numFiles = (int)numFilesChar;
		
		unsigned char nameLength;

		char fileName[256];

		unsigned int fileLength;

		//read files into fileList linked list
		for(i = 0; i < numFiles; i++){
			//read fileNameLength
			if(read(fileReadArchive, &nameLength, 1) < 0){
				die("Failed to read length of file name.");
			}

			//read file name
			if(read(fileReadArchive, fileName, (int)nameLength) < 0){
				die("Failed to read file name.");
			}

			//read fileLength
			if(read(fileReadArchive, &fileLength, 4) < 0){
				die("Failed to read file name.");
			}

			//append null terminator on fileName
			fileName[(int)nameLength] = '\0';

			//add file name and fileLength to fileList
			addFileExtract(fileName, (int)fileLength, fileList);
		}

		//TESTING
		/*
		do{
			//move to next file
			fileList = fileList->next;
			
			//print file info
			printf("%s : %d\n", fileList->fileName, fileList->fileSize);
			
		}while(fileList->next != NULL);
		*/

		//read data from archive into fileList
		do{
			//move to next file
			fileList = fileList->next;
			
			if(read(fileReadArchive, fileList->fileContents, fileList->fileSize) < 0){
				die("failed to read data from archive.");
			}

			//need to create and print to files.
			int fileOpenDes;

			if((fileOpenDes = open(fileList->fileName, O_WRONLY | O_CREAT, 0777)) == -1){
				die("Failed to create and open extracted file.");
			}

			//print to file
			if((write(fileOpenDes, fileList->fileContents, fileList->fileSize) == -1)){
				die("Failed to write fileContents to created file.");
			}

			//close file just created
			close(fileOpenDes);
			
		}while(fileList->next != NULL);

		//close 
		close(fileReadArchive);
		
	}else{
		//check for prefix in file namespace and print file name and size
		//printf("-p\n");

		//same as for extracting from archive file//open file for reading
		int fileReadArchive;

		//char numFilesStr[4];

		unsigned char numFilesChar;

		int numFiles;

		int i;
		
		if((fileReadArchive = open(argv[2], O_RDONLY)) == -1){
			die("Failed to open archive.");
		}

		//read first number for number of files present
		if((read(fileReadArchive, &numFilesChar, 1)) < 0){
			die("Failed to read number of files.");
		}

		//cast numFilesStr as an int
		numFiles = (int)numFilesChar;
		
		unsigned char nameLength;

		char fileName[256];

		unsigned int fileLength;

		//read files into fileList linked list
		for(i = 0; i < numFiles; i++){
			//read fileNameLength
			if(read(fileReadArchive, &nameLength, 1) < 0){
				die("Failed to read length of file name.");
			}

			//read file name
			if(read(fileReadArchive, fileName, (int)nameLength) < 0){
				die("Failed to read file name.");
			}

			//read fileLength
			if(read(fileReadArchive, &fileLength, 4) < 0){
				die("Failed to read file name.");
			}

			//append null terminator on fileName
			fileName[(int)nameLength] = '\0';

			//add file name and fileLength to fileList
			addFileExtract(fileName, (int)fileLength, fileList);
		}

		//read fileList for compatable strings from argv[3]
		int foundAnythingFlag = 0;

		do{
			//move to next file
			fileList = fileList->next;
			
			//print file info if string is contained in file name
			if(strstr(fileList->fileName, argv[3]) != NULL){
				//print the file name and size
				printf("%s\t%d\n", fileList->fileName, fileList->fileSize);
				foundAnythingFlag++;
			}


		}while(fileList->next != NULL);

		//if foundAnythingFlag == 0, then nothing found.
		if(foundAnythingFlag == 0){
			printf("No matching files found.\n");
		}


	}
	
	//exit
	exit(0);
}

void addFileCompress(char* fileName, int fileSize, files* fileListHead){
	
		
		files *current = fileListHead;
		files *newNode = checkMalloc(sizeof(struct files));
		
		//set file nameLenth
		newNode->nameLength = strlen(fileName);
		
		//set fileSize
		newNode->fileSize = fileSize;
		
		//malloc file name
		newNode->fileName = checkMalloc(sizeof(char) * strlen(fileName));
		
		//copy in the data
		strcpy(newNode->fileName, fileName);
		
		//malloc fileContents
		newNode->fileContents = checkMalloc(sizeof(char) * newNode->fileSize);
		
		
		//open file for reading
		int fileDesOpen = open(fileName, O_RDONLY);
		
		//read file and save
		int bytesRead;
		bytesRead = read(fileDesOpen, newNode->fileContents, newNode->fileSize);
		
		if(bytesRead == -1){
			die("Failed to read file.");
		}
		
		//close the file
		close(fileDesOpen);
		
		//set next link to null because at end of list
		newNode->next = NULL;
		
		//checkPoint();
		
		//go to end of LL
		while(current->next != NULL){
			current = current->next;
		}
		
		//checkPoint();
		
		//set current to newNode
		current->next = newNode;
		
}

void addFileExtract(char* fileName, int fileSize, files* fileListHead){
	
		
		files *current = fileListHead;
		files *newNode = checkMalloc(sizeof(struct files));
		
		//set file nameLenth
		newNode->nameLength = strlen(fileName);
		
		//set fileSize
		newNode->fileSize = fileSize;
		
		//malloc file name
		newNode->fileName = checkMalloc(sizeof(char) * strlen(fileName));
		
		//copy in the data
		strcpy(newNode->fileName, fileName);
		
		//malloc fileContents
		newNode->fileContents = checkMalloc(sizeof(char) * newNode->fileSize);
		
		//set next link to null because at end of list
		newNode->next = NULL;
		
		//go to end of LL
		while(current->next != NULL){
			current = current->next;
		}
		
		//set current to newNode
		current->next = newNode;
		
}