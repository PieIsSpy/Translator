// Library inclusions
#include <stdio.h>
#include <string.h>

// constant definitions
#define MAX_WORD 21
#define MAX_FILENAME 31
#define MAX_PAIRS 10
#define MAX_ENTRIES 150

// string typedefs
typedef char aWord[MAX_WORD];
typedef char aFile[MAX_FILENAME];
typedef char aSentence[151];

/*
    struct: pairTag
    
    typedef: pairType
    
    members:
        language: a string that can contain 20 chars, excluding \0
        translation: a string that can contain 20 chars, excluding \0
*/
struct pairTag {
    aWord language;
    aWord translation;
};
typedef struct pairTag pairType;

/*
    struct: entryTag

    typedef: entryType
    
    members:
        pair: an array of struct that contains the pair
        numPairs: an int that determines how many pairs there are in an entry (max 10)
*/
struct entryTag {
    pairType pair[MAX_PAIRS];
    int numPairs;
};
typedef struct entryTag entryType;

/*
    struct: arrayEntryTag

    typedef: arrayEntryType

    members:
        entry: an array of existing entries containing the num of pairs and the pairs themselves
        numEntries: an int that determines how many entries there are in the program (max 150)
*/
struct arrayEntryTag {
    entryType entry[MAX_ENTRIES];
    int numEntries;
};
typedef struct arrayEntryTag arrayEntryType;

//helper functions
int GetString(char String[], int max);
int findPair(aWord language, aWord translation, pairType pair[], int numPairs);
int addPair(aWord language, aWord translation);
void singleEntry(entryType entry[], int index);
void swapPair(pairType *pair1, pairType *pair2);
int findMinPair(pairType pair[], int start, int end);
void sortPairs(entryType *entry, int numPairs);
void swapEntry(entryType *entry1, entryType *entry2);
int findLanguage(pairType pair[], int numPairs, aWord language);
int findMinEntry(entryType entry[], int start, int end);
void sortEntries(entryType entry[], int numEntries);
void readEntryFile(arrayEntryType *entries, FILE *fp);
void translateWord(arrayEntryType entries, aWord word, aWord sourceLanguage, aWord targetLanguage);
void translateSentence(arrayEntryType entries, aSentence sentence, aWord sourceLanguage, aWord targetLanguage, FILE *fp, int mode);

// main functions
void ManageDataMenu();
    void AddEntry(arrayEntryType *entries);
    void AddTranslations(arrayEntryType *entries);
    void DeleteEntry(arrayEntryType *entries);
    void DeleteTranslation(arrayEntryType *entries);
    void DisplayEntries(arrayEntryType entries);
    void SearchWord(arrayEntryType entries);
    void SearchTranslation(arrayEntryType entries);
    void Import(arrayEntryType *entries);
    void Export(arrayEntryType entries);


void TranslateMenu();
    void TranslateTextInput(arrayEntryType entries);
    void TranslateTextFile(arrayEntryType entries);