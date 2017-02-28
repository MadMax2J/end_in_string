//Access to Standard Library functions
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Preprocessor constants
#define INPUT_BUF 255   //User input buffer used for filename.
#define ENDING "ed"

//Function Prototypes
FILE* getValidFile(FILE *inputFilePtr);
char** populateArrayFromFile(FILE *inputFilePtr, char **dataStorageAry, size_t *numStrPtr);
void printWordsWithRequiredEnding(char **dataStorageAry, size_t arraySize, char endingString[]);


int main() {

    char **dataStorageAry = NULL;   //String Array declaration, initialised to NULL (i.e. Pointing nowhere!)
    size_t numberOfStrings = 0;     //Holds the number of strings identified. Used for array boundary control.
    FILE *inputFilePtr = NULL;      //A File Pointer to the Input File.

    //1. Get a valid filename from the user and open it.
    inputFilePtr = getValidFile(inputFilePtr);

    //2. Populate a fully dynamically allocated array with the data from file.
    dataStorageAry = populateArrayFromFile(inputFilePtr, dataStorageAry, &numberOfStrings);
    fclose(inputFilePtr);           //Finished with the file. Close the file.

    //3. Find the elements of the array that end with ENDING and print to the screen.
    printWordsWithRequiredEnding(dataStorageAry, numberOfStrings, ENDING);

    //4. Assuming successful completion of the program...
    return 0;

}//End of main()

/**
 * Function getValidFile
 * This function will continuously prompt the user for a filename, until a valid name for a file we can open is received.
 *
 * @param inputFilePtr - the file pointer to be initialised
 * @return - the initialised file pointer
 */
FILE* getValidFile(FILE *inputFilePtr){
    char inputFilename[INPUT_BUF];  //A holder for the input data from the user.
    printf("%s", "Please enter the name of a text file :");
    gets(inputFilename);            //Store the user input into the holder.

    //Repeat this...###
    do {
        //Check if I can open the file...
        if ((inputFilePtr = fopen(inputFilename, "r")) == NULL) { //If opening fails...
            printf("\nOops! Error opening input file '%s'.\n", inputFilename);
            puts("Please ensure you typed the path and filename in correctly and try again.\n\n");

            //Try again...
            printf("%s", "Please enter the name of a text file :");
            gets(inputFilename);
        }//End of if

    }while(!inputFilePtr);  //###...until we have a valid FILE pointer (i.e an open file)

    //We now have a valid open file, so...
    puts("Thank You!");

    //Return the valid FILE Pointer
    return inputFilePtr;

}//End of function getValidFile

/**
 * Function populateArrayFromFile
 * This function dynamically allocates the appropriate memory for our data structure and populates it with data from file.
 *
 * @param inputFilePtr - A FILE pointer to an open file.
 * @param dataStorageAry - The data structure to hold data from the file.
 * @param numberOfStrings - A counter to record the number of Strings identified.
 * @return - My dynamically allocated and populated data structure.
 */
char** populateArrayFromFile(FILE *inputFilePtr, char **dataStorageAry, size_t *numberOfStrings){
    char temp[BUFSIZ] = ""; //A temporary holder for each record. ## BUFSIZ is declared in stdio.h as 512 ##

    while(!feof(inputFilePtr)){             //Repeat this till we reach the End Of File
        fscanf(inputFilePtr, "%s", temp);   //Read in a record as a String and save it to temp
        (*numberOfStrings)++;               //Increment the number of records identified.

    }//Back to while condition
    ////We have counted the number of Strings

    rewind(inputFilePtr);                   //Send the FILE Pointer back to the beginning of the file.


    //Initialise my pointer array with enough space for a pointer to each string.
    dataStorageAry = calloc(*numberOfStrings, sizeof(char*));
    //Confirm that there was sufficient memory for the allocation...
    if(dataStorageAry == NULL){
        puts("\n\n #### FATAL ERROR - Insufficient Memory ####\n\n");

        //Since this is a Fatal error, we need to exit the program...
        fclose(inputFilePtr);   //Close the file
        exit(-1);
    }

    size_t lengthOfRecord = 0;              //A holder for the number of chars in each String
    for(size_t i = 0; i < *numberOfStrings; i++){   //Iterate for each String previously counted...
        fscanf(inputFilePtr, "%s", temp);   //Read in the String and save it to the temp holder
        lengthOfRecord = strlen(temp);      //Get the length of the String

        //Point each char* to an appropriately allocated amount of memory, required to store
        // the number of chars in the String, plus the NULL char...
        dataStorageAry[i] = calloc(lengthOfRecord + 1, sizeof(char));
        if(dataStorageAry[i] == NULL){
            puts("\n\n #### FATAL ERROR - Insufficient Memory ####\n\n");

            //Since this is a Fatal error, we need to exit the program...
            fclose(inputFilePtr);   //Close the file
            exit(-1);
        }
        //Put the String stored in our temp holder, into our Data Structure
        strcpy(dataStorageAry[i], temp);

    }//End for loop
    ////Array is now populated

    //Print a summary for the user...
    printf("\nWe identified %u words.\n", (unsigned int)*numberOfStrings);

    //Return our populated data structure
    return dataStorageAry;

}//End of function populateArrayFromFile

/**
 * Function printWordsWithRequiredEnding
 * This function iterates through out data structure and outputs the words ending in ENDING
 *
 * @param dataStorageAry - Our Data structure
 * @param arraySize - The number of Strings in our data structure
 * @param endingString - The ENDING string we're looking for
 */
void printWordsWithRequiredEnding(char **dataStorageAry, size_t arraySize, char endingString[]) {
    //Print some informative headings...
    printf("%5s%20s\n", "Index", "Word");
    printf("%25s\n", "##### ###################");

    char *temp1 = NULL; //A temp pointer to the last number of chars of each string we're checking
    //Iterate through the data structure...
    for(size_t i = 0; i < arraySize; i++){
        //We get a pointer to the end of the String, then step back the pointer, the number of chars in our ENDING String.
        temp1 = strrchr(dataStorageAry[i], '\0') - strlen(endingString);
        //Then we check to see if the last chars of the String in the Data structure, match the ENDING String.
        if (!strcmp(temp1, endingString)){  //strcmp returns 0, or Logical FALSE, if there is a match.
            //Output the matched String to the screen...
            printf("%5u%20s\n", (unsigned int)i, dataStorageAry[i]);
        }//End of if

    }//End of for

}//End of function printWordsWithRequiredEnding