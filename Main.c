/*************************************************************************************************************************   
This is to certify that this project is my own work, based on my personal efforts in studying and applying the concepts 
learned.  I have constructed the functions and their respective algorithms and corresponding code by myself.  The 
program was run, tested, and debugged by my own efforts.  I further certify that I have not copied in part or whole or 
otherwise plagiarized the work of other students and/or persons. 
            PieIsSpy and <2nd groupmate>
**************************************************************************************************************************/

/*
Milestone 1 : February 28  
a. Menu options and transitions /
b. Preliminary outline of functions to be created  /
c. Tokenize /
d. Removal of symbols /
e. Declaration of Data Structure needed /

Milestone 2: March 7  
a. Add Entry /
b. Add Translations /
c. Display all entries /
d. View Words /
e. Search Word 	/
f. Search Translation 
 
Milestone 3: March 14  
a. Delete Entry /
b. Delete Translation 
c. Translate Text Input
*/

#include "Functions.c"

int
main()
{
    // variable declaration
    int option;
    char trash;
    
    printf("Simple Translator Activated! Welcome!\n");
    do
    {
        printf("-----Main Menu-----\n");
        printf("1. Manage Data Menu\n");
        printf("2. Translate Menu\n");
        printf("3. Exit\n");
        printf("Please enter a number: ");
        scanf("%d", &option);

        printf("\n");
        if (option == 1)
            ManageDataMenu();
        else if (option == 2)
        {
            scanf("%c", &trash);
            TranslateMenu();
        }
        else if (option != 3)
            printf("Error! Invalid input.\n");

        if (option != 3)
            printf("\n");
            
    } while (option != 3);

    printf("Simple Translator Deactivated! Have a great day!\n");

    return 0;
}