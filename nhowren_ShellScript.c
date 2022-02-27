#include <stdio.h>
typedef enum {false, true} bool;

int parseInput(char *, char *[]);
void printError();
void promptUser(bool);
char *executeCommand(char *, bool *, bool *, char *[], char *[]);
char *redirectCommand(char *, char *, bool *, char *[], char *[]);
bool exitProgram(char *[], int , bool );
void changeDirectories(char *[], int );
void printHelp(char **, int);
void launchProcesses(char *[], int , bool );

int main(int argc, char *argv[])
{

};

int parseInput(char *input, char *splitWords[])
{
    int wordInd = 0;
    splitWords[0] = strtok (input, " ");
    while(splitWords[wordInd] != NULL)
    {
        splitWords[++wordInd] = strtok(NULL, " ");
    }
    return wordInd;
};

void printError()
{
    
};

void promptUser(bool isBatch)
{
    
};

char *executeCommand(char *cmd, bool *isRedirect, bool *isExits, char *tokens[], char *outputTokens[])
{

};

char *redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[])
{
    
};

bool exitProgram(char *tokens[], int numTokens, bool isRedirect)
{
   
};

void changeDirectories(char *tokens[], int numTokens)
{

};

void printHelp(char **, int)
{

};

void launchProcesses(char *tokens[], int numTokens, bool isRedirect)
{
    
};