#include "Helper.c"


/*
==============================Main Functions===========================
*/


/*
    ManageDataMenu lets the user pick a function that is included in
    Manage Data functionality. The array entries can only be saved through
    Export function and all data will be lost after calling main menu again
*/
void
ManageDataMenu()
{
    // array of string for printing
    char aMenu[10][19] = {
                            "Add Entry", //
                            "Add Translations", //
                            "Delete Entry", //
                            "Delete Translation", //
                            "Display Entries", //
                            "Search Word", //
                            "Search Translation", //
                            "Import File",
                            "Export File", //
                            "Main Menu" //
                        };

    // variable declaration
    int i;
    int option;
    char trash;
    arrayEntryType entries;

    // variable initialization
    entries.numEntries = 0;
    for (i = 0; i < MAX_ENTRIES; i++)
        entries.entry[i].numPairs = 0;

    // options menu
    do
    {
        printf("-----Manage Data-----\n");
        for (i = 0; i < 10; i++)
        {
            if (i < 9)
                printf("%-2d. %s\n", i + 1, aMenu[i]);
            else
                 printf("%-2d. %s\n", 0, aMenu[i]);
        }
    
        printf("Please enter a number: ");
        scanf("%d", &option);

        if (option > 0 && option < 10)
            scanf("%c", &trash);
        printf("\n");

        // call function
        switch (option)
        {
            case 1:
                AddEntry(&entries);
                break;

            case 2:
                AddTranslations(&entries);
                break;

            case 3:
                DeleteEntry(&entries);
                break;
            
            case 4:
                DeleteTranslation(&entries);
                break;

            case 5:
                DisplayEntries(entries);
                break;

            case 6:
                SearchWord(entries);
                break;

            case 7:
                SearchTranslation(entries);
                break;

            case 8:
                Import(&entries);
                break;

            case 9:
                Export(entries);
                break;

            case 0:
                break;

            default:
                printf("ERROR: Invalid input.\n");
        }
    } while (option != 0);
}



/*
    AddEntry adds an entry to contain 10 pairs of language - translation pair

    @param entry - an array that contains all entries of pairs
*/
void
AddEntry(arrayEntryType *entries)
{
    // variables
    int i;
    int condition, first = 1, found;
    char input = 'a', trash;
    int localPair = 0, currentEntry = entries->numEntries;

    /*
        condition 1 = function loop
        condition 2 = function terminate
        condition 3 = function's first run
    */

    printf("Entering language - translation pair:\n");    
    do
    {
        // loop until valid input
        printf("Type 'ND' to terminate\n");
        condition = addPair(entries->entry[currentEntry].pair[localPair].language, entries->entry[currentEntry].pair[localPair].translation);
        printf("\n");

        // if its the first pair of the entry and there are multiple entries
        if (first && currentEntry > 0)
        {
            // find similar entries
            for (i = 0; i < currentEntry; i++)
            {
                found = findPair(entries->entry[currentEntry].pair[localPair].language, entries->entry[currentEntry].pair[localPair].translation, entries->entry[i].pair, entries->entry[i].numPairs);

                if (found > -1)
                {
                    printf("Entry #%d found with similar translation\n", i + 1);
                    singleEntry(entries->entry, found);
                    printf("\n");
                    condition = 3;
                }
            }

            if (condition == 3)
            {
                do
                {
                    printf("Is this a new entry (Y/N): ");
                    scanf("%c", &input);
                    scanf("%c", &trash);

                    if (input == 'Y' || input == 'y')
                    {
                        // flag down first run condition, continue running the function
                        condition = 1;
                        first = 0;
                    }
                    else if (input == 'N' || input == 'n')
                    {
                        // clear everything for this entry
                        localPair = 0;
                        strcpy(entries->entry[currentEntry].pair[localPair].translation, "");
                        strcpy(entries->entry[currentEntry].pair[localPair].language, "");

                        // terminate
                        condition = 2;
                        
                    }
                    else
                        printf("ERROR: Invalid input.");
                } while (input != 'Y' && input != 'y' && input != 'N' && input != 'n');
            }
        }

        if (condition == 1)
            localPair++;

    } while (condition != 2 && localPair < MAX_PAIRS); // loop until ND was input or max pairs have reached

    // update number of entries, then print the entry
    if (condition == 2 && localPair > 0)
    {
        // display it
        printf("Entry #%d added!\n", currentEntry + 1);
        printf("Pairs: %d\n", localPair);
        printf("%-20sTranslation\n", "Language");

        for (i = 0; i < localPair; i++)
            printf("%-20s%s\n", entries->entry[currentEntry].pair[i].language, entries->entry[currentEntry].pair[i].translation);

        // update counters
        (entries->numEntries)++;
        entries->entry[currentEntry].numPairs = localPair;

        // sort entries
        sortEntries(entries->entry, entries->numEntries);
    }
    else
        printf("No entry was added.\n");

    printf("\n");
}

/*
    AddTranslations adds language-translation pairs to an existing entry

    @param entries - array containing all entries
*/
void AddTranslations(arrayEntryType *entries) {
	char trash;
    aWord language, translation;
    int found[MAX_ENTRIES]; // Stores indices of matching entries
    int numFound = 0, chosenIndex, condition, i;
    char input;

    // Ask for language and translation pair
    do
    {
        printf("Enter the language of an existing entry: ");
        condition = GetString(language, MAX_WORD);
        
        if (!condition)
            printf("ERROR: Invalid Input\n");
    } while (!condition);
    
    do
    {
        printf("Enter the translation of an existing entry: ");
        condition = GetString(translation, MAX_WORD);
        
        if (!condition)
            printf("ERROR: Invalid Input\n");
    } while (!condition);

    // Search for entries containing the given pair
    for (i = 0; i < entries->numEntries; i++) {
        if (findPair(language, translation, entries->entry[i].pair, entries->entry[i].numPairs) != -1) {
            found[numFound] = i; // Store index of matching entry
            numFound++;
        }
    }

    // If no matching entry exists, inform user and return
    if (numFound == 0) {
        printf("No existing entry found. Use Add Entry first.\n");
        return;
    }

    // Display matching entries
    printf("Matching Entries:\n");
    for (i = 0; i < numFound; i++) {
        printf("Entry %d#:\n", i + 1);
        singleEntry(entries->entry, found[i]);
        printf("\n");
    }

    // Ask user to select an entry if multiple exist
    chosenIndex = 0;
    while (numFound > 1) {
        printf("Select an entry number to add translation: ");
        scanf("%d", &chosenIndex);
        scanf("%c", &trash);
        chosenIndex--;
        if (chosenIndex >= 0 && chosenIndex < numFound) {
            numFound = 1; // Confirm valid selection
        }
    }

    // Loop to add translations until user decides to stop or limit is reached
    input = 'Y';
    while (entries->entry[found[chosenIndex]].numPairs < MAX_PAIRS && (input == 'Y' || input == 'y') && numFound) {
        do
        {
            printf("Type 'ND' to cancel\n");
            printf("Enter new language: ");
            condition = GetString(language, MAX_WORD);

            if (!condition)
                printf("ERROR: Invalid Input\n");
        } while (!condition);
        
        if (condition == 2) {
            numFound = 0; // Stop if "ND" is entered
        }

        if (numFound) {
            do
            {
                printf("Enter new translation: ");
                condition = GetString(translation, MAX_WORD);

                if (!condition)
                    printf("ERROR: Invalid Input\n");
            } while (!condition);
            
            if (condition == 2) {
                numFound = 0;
            }
        }

        if (numFound) {
            // Add the new language-translation pair
            strcpy(entries->entry[found[chosenIndex]].pair[entries->entry[found[chosenIndex]].numPairs].language, language);
            strcpy(entries->entry[found[chosenIndex]].pair[entries->entry[found[chosenIndex]].numPairs].translation, translation);
            entries->entry[found[chosenIndex]].numPairs++;
            printf("Translation added successfully!\n\n");

            printf("Updated Translations:\n");
            sortEntries(entries->entry, entries->numEntries);
			singleEntry(entries->entry, found[chosenIndex]);
            printf("\n");
        }
        else
            printf("No translations added.\n");

        // If max translations reached, stop adding
        if (entries->entry[found[chosenIndex]].numPairs == MAX_PAIRS) {
            printf("Maximum translations reached for this entry.\n\n");
            numFound = 0;
        }

        // Ask user if they want to add more translations
        if (numFound) {
            printf("Do you want to add more translations? (Y/N): ");
            scanf(" %c", &input);
            scanf("%c", &trash);
        }
    }

    printf("\n");
}

/*
    DeleteEntry deletes an existing entry in the list

    @param entries - array of current existing entries to be deleted
*/
void DeleteEntry(arrayEntryType *entries) {
    int entryIndex, i = 0;
    char choice;
    int condition = 1;

    // Check if there are entries to delete
    if (entries->numEntries == 0) 
        printf("No entries available to delete.\n");
    else 
    {
        // "Similar" display entries, but assume it is INT input
        do
        {
            printf("Entry %d#:\n", i + 1);
            printf("Pair count: %d\n", entries -> entry[i].numPairs);
            singleEntry(entries -> entry, i);
            printf("\n");

            // Ask user for input
            printf("Enter the number of the displayed entry to be deleted\n");
            if (i < entries->numEntries - 1)
                printf("Enter 0 to view next entry\n");
            if (i > 0)
                printf("Enter -1 to view previous entry\n");

            printf("Enter -2 to cancel\n");

            printf("Enter input: ");
            scanf("%d", &entryIndex);

            printf("\n");

            if (entryIndex == 0)
                i++;
            else if (entryIndex == -1)
                i--;
            else if (entryIndex == -2)
            {
                printf("Deletion canceled. Returning to menu.\n");
                condition = 0;
            }
            else if (entryIndex < -2 || entryIndex > entries->numEntries) // invalid input
            {
                do
                {
                    printf("ERROR: Invalid Input.\n");
                    printf("Would you like to continue a deletion? (Y/N): ");
                    scanf(" %c", &choice);

                    if (choice == 'N' || choice == 'n')
                        condition = 0;
                } while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');
            }
            else // valid input
            {
                do
                {
                    // Ask for confirmation before deleting
                    printf("Are you sure you want to delete Entry #%d? (Y/N): ", entryIndex);
                    scanf(" %c", &choice);
            
                    if (choice == 'Y' || choice == 'y') {
                        // Converts each array to its actual index like entry is 1 but index is 0
                        entryIndex--;

                        // ARRAY SHIFT (shift the entries back to fill the gap of the deleted entry)
                        for (i = entryIndex; i < entries->numEntries - 1; i++) {
                            entries->entry[i] = entries->entry[i + 1];
                        }

                        // minus entryy count
                        entries->numEntries--;

                        printf("Entry #%d was deleted.\n", entryIndex + 1);
                    } 
                    else if (choice == 'N' || choice == 'n')
                    {
                        printf("Deletion Cancelled.\n");
                    }
                    else
                        printf("ERROR: Invalid Input.\n");
                } while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');
            }

        } while (condition && entries->numEntries > 0);
    }

    printf("\n");
}

/*
    DeleteTranslation lets the user pick an entry where they would delete from, then prompts
    them to choose for the number of the pair to be deleted

    @param entries - the array of entries the user will select to delete from
*/
void DeleteTranslation(arrayEntryType *entries) {
    int entryIndex, pairIndex, i;
    char choice;
    int condition = 1;

    // To check if there are any entries at all
    if (entries->numEntries == 0) {
        printf("No entries available to delete translations from.\n");
    } else {
        do {
            // Display entries one by one by calling function DisplayEntries()
            printf("Here is the list of entries. Type 'X' if you have found which to modify.\n\n");
            DisplayEntries(*entries);
            
            // Ask user to choose an entry to which to modify
            printf("Enter the entry number to delete a translation from (0 to cancel): ");
            scanf("%d", &entryIndex);

			//If user types 0 cancells the transaction
            if (entryIndex == 0) {
                printf("Deletion canceled. Returning to menu.\n");
                condition = 0;
            } 
			else if (entryIndex >= 1 && entryIndex <= entries->numEntries) {
                entryIndex--; // Convert to array index
                do {
                    // Display translations for chosen entry
                    printf("Entry %d:\n", entryIndex + 1);
                    //for loop to add numbers on which translation for visibility
                    for (i = 0; i < entries->entry[entryIndex].numPairs; i++) {
                        printf("%d. %s - %s\n", i + 1, entries->entry[entryIndex].pair[i].language, entries->entry[entryIndex].pair[i].translation);
                    }
                    printf("\n");

                    // Ask user to choose a translation pair
                    printf("Enter the translation number to delete (0 to cancel): ");
                    scanf("%d", &pairIndex);

                    if (pairIndex == 0) {
                        printf("Translation deletion canceled. Returning to menu.\n");
                        condition = 0;
                    } 
					else if (pairIndex >= 1 && pairIndex <= entries->entry[entryIndex].numPairs) {
                        pairIndex--; // Convert to array index

                        // ARRAY SHIFT for the deleted translation 
                        for (i = pairIndex; i < entries->entry[entryIndex].numPairs - 1; i++) {
                            entries->entry[entryIndex].pair[i] = entries->entry[entryIndex].pair[i + 1];
                        }
						
						// Minus the numpairs
                        entries->entry[entryIndex].numPairs--;
                        printf("Translation deleted successfully!\n");

                        // If no translations delete the ENTIRE entry
                        if (entries->entry[entryIndex].numPairs == 0) {
                            printf("No more translations left. Deleting entire entry.\n");
                            for (i = entryIndex; i < entries->numEntries - 1; i++) {
                                entries->entry[i] = entries->entry[i + 1];
                            }
                            entries->numEntries--;
                            condition = 0;
                        } 
						else {
                            // Ask if the user wants to delete another translation from the same entry
                            printf("Do you want to delete another translation from this entry? (Y/N): ");
                            scanf(" %c", &choice);
                            if (choice != 'Y' && choice != 'y') {
                                condition = 0;
                            }
                        }
                    } 
					else {
                        printf("ERROR: Invalid translation number.\n");
                    }
                } while (condition);
            } 
			else {
                printf("ERROR: Invalid entry number.\n");
            }
        } while (condition);
    }

    printf("\n");
}

/*
    DisplayEntries lets the user see all entries one by one

    @param entries - array of currently existing entries
*/
void
DisplayEntries(arrayEntryType entries)
{
    int i = 0;
    char input, trash;

    if (entries.numEntries > 0)
    {
        do
        {
            printf("Entry %d#:\n", i + 1);
            printf("Pair count: %d\n", entries.entry[i].numPairs);
            singleEntry(entries.entry, i);
            printf("\n");

            if (i > 0)
                printf("P for previous entry\n");
            if (i < entries.numEntries - 1)
                printf("N for next entry\n");

            printf("X to exit\n");
        
            printf("Please enter a letter: ");
            scanf("%c", &input);
            scanf("%c", &trash);

            printf("\n");

            if ((input == 'N' || input == 'n') && i < entries.numEntries - 1)
                i++;
            else if ((input == 'P' || input == 'p') && i > 0)
                i--;
            else if (input != 'X' && input != 'x')
                printf("ERROR: Invalid Input.\n");

        } while (input != 'X' && input != 'x');
    }
    else
        printf("No entries found. Please add an entry first.\n");

    printf("\n");
}

/*
    SearchWord views all entries with a specified translation word

    @param entries = list of all currently existing entries
*/
void 
SearchWord(arrayEntryType entries)
{
    // declarations
    arrayEntryType found;
    aWord translation;
    int condition, i, j, numFound = 0;
    
    // input
    do
    {
        printf("Please enter the word to search: ");
        condition = GetString(translation, MAX_WORD);

        if (!condition)
            printf("ERROR: Invalid Input");
    } while (!condition);

    // find entries with that word
    for (i = 0; i < entries.numEntries; i++)
    {
        j = 0;
        condition = 1;
        while (j < entries.entry[i].numPairs && condition)
        {
            if (strcmp(entries.entry[i].pair[j].translation, translation) == 0)
            {
                found.entry[numFound] = entries.entry[i];
                numFound++;
                condition = !condition;
            }

            j++;
        }
    }

    found.numEntries = numFound;

    if (numFound > 0)
        DisplayEntries(found);
    else
        printf("No entries found with the word '%s'.\n", translation);

    printf("\n");
}

/*
    SearchTranslation views all entries with the specified language-translation pair

    @param entries - array of all existing entries
*/
void
SearchTranslation(arrayEntryType entries)
{
    arrayEntryType found;
    aWord language, translation;
    int condition, i, j, numFound = 0;

    do
    {
        printf("Please enter the language of the word: ");
        condition = GetString(language, MAX_WORD);

        if (!condition)
            printf("ERROR: Invalid Input.\n");
    } while (!condition);

    do
    {
        printf("Please enter the word to search: ");
        condition = GetString(translation, MAX_WORD);

        if (!condition)
            printf("ERROR: Invalid Input.\n");
    } while (!condition);
    
    // find entries with that word
    for (i = 0; i < entries.numEntries; i++)
    {
        j = 0;
        condition = 1;
        while (j < entries.entry[i].numPairs && condition)
        {
            if (strcmp(entries.entry[i].pair[j].translation, translation) == 0 && strcmp(entries.entry[i].pair[j].language, language) == 0)
            {
                found.entry[numFound] = entries.entry[i];
                numFound++;
                condition = !condition;
            }

            j++;
        }
    }

    found.numEntries = numFound;

    if (numFound > 0)
        DisplayEntries(found);
    else
        printf("No entries found with the language-translation pair '%s - %s'.\n", language, translation);

    printf("\n");
}

/*
    Export lets the user export all existing entries from ManageData to be saved into
    a txt file

    @param entries - array of all existing entries
*/
void
Export(arrayEntryType entries)
{
    FILE *fp;
    aFile filename;
    int condition;
    int i = 0, j;

    if (entries.numEntries > 0)
    {
        do
        {
            printf("Please enter the filename to export (include .txt to the filename): ");
            condition = GetString(filename, MAX_FILENAME);

            if (!condition)
                printf("ERROR: Invalid Input.\n");
        } while (!condition);

        fp = fopen(filename, "w");

        for (i = 0; i < entries.numEntries; i++)
        {
            for (j = 0; j < entries.entry[i].numPairs; j++)
                fprintf(fp, "%s: %s\n", entries.entry[i].pair[j].language, entries.entry[i].pair[j].translation);

            fprintf(fp, "\n");
        }

        printf("File exported!\n");
        fclose(fp);
    }
    else
        printf("No existing entries found. Please use Add Entry first.\n");

    printf("\n");
}

/*
    Import lets the user import entries from a given txt file and lets them choose
    if the entries will be added to the current existing entries

    @param entries - array of all existing entries

    Pre-condition: The txt file given is formatted as follows:

    <language>: <translation><newline>
    <language>: <translation><newline>
    <newline>
    <language>: <translation><newline>
    <language>: <translation><newline>
    <newline>
    <EOF>
*/
void
Import(arrayEntryType *entries)
{
    FILE *fp;
    aFile filename;

    arrayEntryType holder;
    char decision, trash;
    
    int i = 0;
    int currentEntry = entries->numEntries;
    int localEntry;
    int condition;

    do
    {
        printf("Please enter the filename to import (include .txt to the filename): ");
        condition = GetString(filename, MAX_FILENAME);

        if (!condition)
            printf("ERROR: Invalid Input.\n");
    } while (!condition);

    if ((fp = fopen(filename, "r")) != NULL)
    {
        readEntryFile(&holder, fp);
        localEntry = holder.numEntries;

        if (currentEntry + localEntry > MAX_ENTRIES)
            printf("ERROR: Exceeded maximum amount of entry.\n");
        else if (localEntry == 0)
            printf("No entries were found in the import.\n");
        else
        {
            i = 0;
            while (i < localEntry)
            {
                do
                {
                    singleEntry(holder.entry, i);
                    printf("\n");

                    printf("Would you like to add this entry to the current entries?\n");
                    printf("V to add entry\n");
                    printf("X to skip entry\n");
                    printf("Please enter a letter: ");
                    scanf("%c", &decision);
                    scanf("%c", &trash);

                    if (decision == 'v' || decision == 'V')
                    {
                        entries->entry[currentEntry] = holder.entry[i];
                        currentEntry++;
                        sortEntries(entries->entry, currentEntry);
                    }
                    else if (decision != 'x' && decision != 'X')
                        printf("ERROR: Invalid Input.\n");
                } while (decision != 'v' && decision != 'V' && decision != 'x' && decision != 'X');

                printf("\n");
                i++;
            }

            if (entries->numEntries == currentEntry)
                printf("No entries were imported.\n");
            else
                entries->numEntries = currentEntry;        
        }

        fclose(fp);
    }
    else
        printf("ERROR: file '%s' does not exist.\n", filename);

    printf("\n");
}

/*
    TranslateMenu lets the user type in the txt file of the entries to be used for translation
    and asks them from where they will translate from
*/
void TranslateMenu() {
    arrayEntryType entries;
    aFile filename;

    int option;
    char trash;
    int valid;

    FILE *fp;

    do
    {
        printf("Please enter source file to be used (include .txt to the filename): ");
        valid = GetString(filename, MAX_FILENAME);

        if (!valid)
            printf("ERROR: Invalid File Name.\n");
    } while (!valid);

    printf("\n");

    if ((fp = fopen(filename, "r")) != NULL)
    {
        readEntryFile(&entries, fp);
        fclose(fp);

        sortEntries(entries.entry, entries.numEntries);

        do {
            printf("-----Translate Menu-----\n");
            printf("1. Translate Text Input\n");
            printf("2. Translate Text File\n");
            printf("3. Return to Main Menu\n");
            printf("Please enter a number: ");
            
            scanf("%d", &option);
            
            if (option > 0 && option < 3)
                scanf("%c", &trash);
            printf("\n");
    
            switch (option) {
                case 1:
                    TranslateTextInput(entries);
                    break;
                case 2:
                    TranslateTextFile(entries);
                    break;
                case 3:
                    printf("Returning to Main Menu...\n");
                    break;
                default:
                    printf("ERROR: Invalid input.\n");
            }
        } while (option != 3);
    }
    else
        printf("ERROR: %s does not exist.\n", filename);

    printf("\n");
}

/*
    TranslateTextInput lets the user type out the text to be translated,
    what language is it on and the language they would like it to be translated to

    @param entries - array of existing entries
*/
void TranslateTextInput(arrayEntryType entries) {
    aSentence sentence;
    aWord sourceLanguage, targetLanguage;
    
    char choice;
    char trash;
    int continueTranslation = 1;
    int condition;

    // this does nothing, only to initialize the fp inside translateSentence
    FILE *bait;

    bait = NULL;

    do
    {
        printf("Enter the language of the text: ");
        condition = GetString(sourceLanguage, MAX_WORD);

        if (!condition)
            printf("ERROR: Invalid Input.\n");
    } while (!condition);


    do
    {
        printf("Enter the language to translate: ");
        condition = GetString(targetLanguage, MAX_WORD);

        if (!condition)
            printf("ERROR: Invalid Input.\n");
    } while (!condition);

    do {
        printf("Enter text to translate (max 150 chars, type 'EXIT' to return): ");
        GetString(sentence, 151);
        printf("\n");

        if (strcmp(sentence, "EXIT") == 0) {
            printf("Returning to Translate Menu...\n");
            continueTranslation = 0;
        } else {

            printf("Translated text: \n");
            translateSentence(entries, sentence, sourceLanguage, targetLanguage, bait, 0);
            printf("\n");

            do {
                printf("Translate another text? (Y/N): ");
                scanf(" %c", &choice);
                scanf("%c", &trash);
                if (choice == 'N' || choice == 'n') {
                    continueTranslation = 0;
                    printf("\n");
                }
                else if (choice != 'Y' && choice != 'y')
                    printf("ERROR: Invalid Input.\n");
            } while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');
        }
    } while (continueTranslation);

    printf("\n");
}

/*
    TranslateTextFile lets the user type out the file name of the txt file to be translated
    what language is it on, the language they would like it to be translated to
    and the file name they would like to save it to

    @param entries - array of existing entries
*/
void
TranslateTextFile(arrayEntryType entries)
{
    aFile filename, saveAs;
    aWord sourceLanugage, targetLanguage;
    aSentence sentence;

    int condition;
    int res, i = 0;
    char read;

    FILE *fp;

    do
    {
        printf("Enter language of the source file: ");
        condition = GetString(sourceLanugage, MAX_WORD);

        if (!condition)
            printf("ERROR: Invalid Input.\n");
    } while (!condition);

    do
    {
        printf("Enter the language of the translation: ");
        condition = GetString(targetLanguage, MAX_WORD);

        if (!condition)
            printf("ERROR: Invalid Input.\n");
    } while (!condition);

    do
    {
        printf("Enter source file to translate (include .txt to the filename): ");
        condition = GetString(filename, MAX_FILENAME);

        if (!condition)
            printf("ERROR: Invalid Input.\n");
    } while (!condition);

    do
    {
        printf("Enter name of the saved file: ");
        condition = GetString(saveAs, MAX_FILENAME);

        if (!condition)
            printf("ERROR: Invalid Input.\n");
    } while (!condition);


    if ((fp = fopen(filename, "r")) != NULL)
    {
        while ((res = fscanf(fp, "%c", &read) == 1))
        {
            sentence[i] = read;
            i++;
        }

        sentence[i] = '\0';

        printf("Source text: \n");
        printf("%s\n", sentence);

        printf("Translation: \n");
        fp = fopen(saveAs, "w");
        translateSentence(entries, sentence, sourceLanugage, targetLanguage, fp, 1);
        fclose(fp);
    }
    else
        printf("ERROR: %s does not exist.\n", filename);

    printf("\n");
}