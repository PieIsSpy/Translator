#include "Headers.h"

/*
============================Helper Functions===========================
*/


/*
    GetString takes all chars except \n as inputs and rearranges them into
    a string

    @param String - the string that will contain the inputs of the user
    @param max - maximum character count of the string

    Pre-condition: max is an element of positive int

    @returns 1 if the input is valid, 2 if the input was 'ND', and 0 if the input was invalid
*/
int
GetString(char String[], int max)
{
    // variables
    aSentence container;
    int condition = 1;
    char input;
    int i = 0;

    // gets chars until input is \n or max char is met
    do
    {
        scanf("%c", &input);

        if (input != '\n')
        {
            container[i] = input;
            i++;
        }
    } while (input != '\n' && i < max);

    // null byte terminator for the very end of the string
    container[i] = '\0';

    // check if 1 <= n <= 20 chars (excluding \0)
    if (strlen(container) >= 1 && strlen(container) < max - 1)
    {
        if (strcmp(container, "ND") != 0)
            strcpy(String, container);
        else
            condition = 2;
    }
    else
        condition = 0;

    return condition;
}

/*
    findPair returns the index of the first instance the key was found

    @param language - the string containing the language
    @param translation - the string containing the translation
    @param pair - the array containing language-translation pairs
    @param numPairs - how many pairs are there in an entry

    Pre-condition: Assume that numPairs is 1 <= numPairs <= 10.
                   Assume that the pairs of an entry is also sorted.

    @returns the index of the language-translation pair in an entry
*/
int
findPair(aWord language, aWord translation, pairType pair[], int numPairs)
{
    int condition = -1;
    int i = 0;

    while (i < numPairs && condition == -1)
    {
        if (strcmp(translation, pair[i].translation) == 0 && strcmp(language, pair[i].language) == 0)
            condition = i;

        i++;
    }

    return condition;
}

/*
    addPair asks the user for inputs for a language-translation pair

    @param language - the string where the language will be stored
    @param translation - the string where the translation will be stored

    @returns the condition whether the user inputted 'ND' for language (2)
             or not (1)
*/
int
addPair(aWord language, aWord translation)
{
    int condition = 1;

    do
    {
        printf("Please enter the language of the word: ");
        condition = GetString(language, MAX_WORD);
        
        if (condition == 0)
            printf("ERROR: Invalid input.\n");
    } while (!condition);

    // check if ND was not inputted
    if (condition != 2)
    {
        // loop until valid input
        do
        {
            printf("Please enter the word: ");
            condition = GetString(translation, MAX_WORD);

            if (condition == 0)
                printf("ERROR: Invalid input.\n");
        } while (!condition);
    }

    return condition;
}

/*
    singleEntry displays all pairs contained in an entry, given an index

    @param entry - array of entries to select from
    @param index - the given index of an entry to be displayed
*/
void
singleEntry(entryType entry[], int index)
{
    int i;

    printf("%-20sTranslation\n", "Language");
    for (i = 0; i < entry[index].numPairs; i++)
        printf("%-20s%s\n", entry[index].pair[i].language, entry[index].pair[i].translation);
}

/*
    swapPair swamps the data between two pairType structs

    @param pair1 - first pair to be swapped
    @param pair2 - second pair to be swapped
*/
void
swapPair(pairType *pair1, pairType *pair2)
{
    pairType temp;

    temp = *pair1;
    *pair1 = *pair2;
    *pair2 = temp;
}

/*
    findMinPair finds the index of the pair that contains the least ASCII value
    in language. If both of them have the same language, it will compare for the translation instead

    @param pair - the array containing all pairs of an entry
    @param start - the index where to start searching
    @param end - the number where the loop whill end
    
    @returns the index where the min ASCII value was found
*/
int
findMinPair(pairType pair[], int start, int end)
{
    int min = start;
    int i;

    for (i = min + 1; i < end; i++)
        if (strcmp(pair[min].language, pair[i].language) > 0) // if min has greater ascii val than index in language comparison
            min = i;
        else if (strcmp(pair[min].language, pair[i].language) == 0) // if min == index in language comparison
            if (strcmp(pair[min].translation, pair[i].translation) > 0) // if min > index ascii val for translation
                min = i;

    return min;
}

/*
    sortPairs utilizes selection sort to sort the pairs in increasing order of ASCII values

    @param entry - the entry containing the language-translation pair
    @param numPairs - how many pairs are there in an entry

    Pre-condition: there must be atleast 1 pair existing in the entry
*/
void
sortPairs(entryType *entry, int numPairs)
{
    int i, min;

    for (i = 0; i < numPairs; i++)
    {
        min = findMinPair(entry->pair, i, numPairs);

        if (min != i)
            swapPair(&entry->pair[i], &entry->pair[min]);
    }
}

/*
    swapEntry swaps the data between two entryType structs

    @param entry1 - first struct to be swapped
    @param entry2 - second struct to be swapped
*/
void
swapEntry(entryType *entry1, entryType *entry2)
{
    entryType temp;

    temp = *entry1;
    *entry1 = *entry2;
    *entry2 = temp;
}

/*
    findLanguage finds the index of the language-translation pair containing a specified language

    @param pair - the array of pair structures containing the language and translation strings
    @param numPairs - number of pairs present in an entry
    @param language - specified language to find

    Pre-condition: there is atleast one language-translation pair present in the array of pairs

    @returns the index of the pair where the first instance of the language was found, -1 if not found
*/
int
findLanguage(pairType pair[], int numPairs, aWord language)
{
    int i = 0, found = -1;

    while (i < numPairs && found == -1)
    {
        if (strcmp(pair[i].language, language) == 0)
            found = i;

        i++;
    }

    return found;
}

/*
    findMinEntry finds the index number of the pair of an entry that has an english language
    and compares it to another entry that has an english language and compares them based
    on the ASCII value of the translation words

    @param entry - the array consisting of the language-translation pairs
    @param start - which index will it start searching from
    @param end - which index before it will it end

    Pre-condition: there is atleast one language-translation pair present in an entry
                    the entries have already been sorted by language alphabetically
                    the entries containing english language have been moved to the first indices
    
    @returns the index of entry where the min ASCII value was found, 
    returns the starting index instead if there exist no english pairs in all entries
*/
int
findMinEntry(entryType entry[], int start, int end)
{
    // starting index
    int min = start;
    int engMin = findLanguage(entry[min].pair, entry[min].numPairs, "English"), engIndex;
    int i;

    for (i = min + 1; i < end; i++)
    {
        // find index of english pair
        engIndex = findLanguage(entry[i].pair, entry[i].numPairs, "English");
        
        // if min is greater, take its index
        if (engIndex > -1 && engMin > -1)
        {
            if (strcmp(entry[min].pair[engMin].translation, entry[i].pair[engIndex].translation) > 0)
            {
                engMin = engIndex;
                min = i;
            }
        }
        else if (engMin == -1 && engIndex > -1) // else if current min index has no english, but current index have english
        {
            engMin = engIndex;
            min = i;
        }
    }

    return min;
}

/*
    sortEntries first moves all entries containing english language into the first few indices
    then sorts the entries that containing the language english based on its translation word
    
    @param entry - array of entries containing all language translation pair
    @param numEntries - how many entries currently exists

    Pre-condition: all entries must have atleast 1 pair if it exists
*/
void
sortEntries(entryType entry[], int numEntries)
{
    int i, acc = 0, min;

    // sort all entries
    for (i = 0; i < numEntries; i++)
        sortPairs(&entry[i], entry[i].numPairs);

    // move entries with english to the first few indices
    for (i = 0; i < numEntries; i++)
    {
        if (findLanguage(entry[i].pair, entry[i].numPairs, "English") > -1)
        {
            swapEntry(&entry[acc], &entry[i]);
            acc++;
        }
    }

    // sort english entries alphabetically
    for (i = 0; i < acc; i++)
    {
        min = findMinEntry(entry, i, acc);

        if (i != min)
            swapEntry(&entry[min], &entry[i]);
    }
}

/*
    readFile reads the text file of a given file pointer per character until EOF

    @param entries - array of entries that will hold all language-translation pair
    @param fp - the file pointer that will point to the specified file name
*/
void
readEntryFile(arrayEntryType *entries, FILE *fp)
{
    aWord word;
    char read, previous;

    int i = 0, j = 0;
    int res;
    int nextLine;

    int localEntry = 0;
    int localPair = 0;
    
    int condition = 0;

    while (!condition)
    {
        while((res = fscanf(fp, "%c", &read)) == 1)
        {
            word[i] = read;
            nextLine = 0;

            // detect if its already a new entry
            if (read == previous && previous == '\n')
            {
                entries -> entry[localEntry].numPairs = localPair;
                localPair = 0;
                nextLine = 1;
                localEntry++;
            }

            // detect if the character is ':' or a newline but not a new entry
            if (read == ':' || (read == '\n' && !nextLine))
            {
                word[i] = '\0';
                i = 0;
                
                if (j % 2 == 0)
                {
                    strcpy(entries -> entry[localEntry].pair[localPair].language, word);
                    strcpy(word, "");
                }
                else
                {
                    strcpy(entries -> entry[localEntry].pair[localPair].translation, word);
                    strcpy(word, "");
                    localPair++;
                }

                j++;
            }
            else if (read != ' ' && !nextLine) // if a space or a new entry is detected, dont increment
                i++;

            previous = read;
        }

        // if reached eof
        if (res == -1)
        {
            entries -> numEntries = localEntry;
            sortEntries(entries -> entry, localEntry);
            condition = 1;
        }
    }
}

/*
    translateWord finds the word and its language in entries, then translated to its equivalent when found in an entry

    @param entries - array that contains all entries
    @param word - the word to be searched in all entries
    @param sourceLanguage - the language of the word to be searched
    @param targetLanguage - the language that the word will be translated
*/
void
translateWord(arrayEntryType entries, aWord word, aWord sourceLanguage, aWord targetLanguage)
{
    int inEntry = -1, hasEquivalent = -1;
    int i = 0;

    // search all entries for the first instance of the pair
    while (i < entries.numEntries && inEntry == -1)
    {
        // this only returns the index INSIDE the entry, use i instead if it's found in the entry itself
        inEntry = findPair(sourceLanguage, word, entries.entry[i].pair, entries.entry[i].numPairs);

        if (inEntry == -1)
            i++;
    }

    if (inEntry > -1)
        hasEquivalent = findLanguage(entries.entry[i].pair, entries.entry[i].numPairs, targetLanguage);

    if (hasEquivalent > -1)
        strcpy(word, entries.entry[i].pair[hasEquivalent].translation);
}

void
translateSentence(arrayEntryType entries, aSentence sentence, aWord sourceLanguage, aWord targetLanguage, FILE *fp, int mode)
{
    aSentence translation;
    aWord temp;
    char symbols[5] = ",.!?";
    
    int len = strlen(sentence);
    int i, j, k = 0;
    int isSymbols, newSentence;

    strcpy(translation, "");
    
    for (i = 0; i <= len; i++)
    {
        j = 0;
        isSymbols = 0;
        newSentence = 0;

        temp[k] = sentence[i];

        // chdck if separator
        while(!isSymbols && j < 5)
        {
            if (sentence[i] == symbols[j])
            isSymbols = 1;

            j++;
        }

        // if its end of sentence
        if (temp[k] == ' ' || temp[k] == '\0' || temp[k] == '\n')
        {
            if (temp[k] == '\n')
                newSentence = 1;

            temp[k] = '\0';
            k = 0;
            
            // translate the word
            translateWord(entries, temp, sourceLanguage, targetLanguage);

            // concatinate to translated sentence
            strcat(translation, temp);
            strcat(translation, " ");

            // clear temp holder
            strcpy(temp, "");

            // if its a new sentence
            if (newSentence)
            {
                printf("%s\n", translation);
                
                if (mode)
                    fprintf(fp, "%s\n", translation);

                strcpy(translation, "");
            }
        }
        else if (!isSymbols) // ignore symbols
            k++;
    }

    printf("%s\n", translation);
}