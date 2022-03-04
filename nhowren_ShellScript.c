#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>


void promptUser(bool isBatch);
void printError();
int  parseInput(char *input, char *splitWords[]);
char *redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[]);
char *executeCommand(char *cmd, bool *isRedirect,  bool *isExit, char* tokens[], char* outputTokens[]);
char getLetter(char *str, int index);
bool exitProgram(char *tokens[], int numTokens);
void launchProcesses(char *tokens[], int numTokens, bool isRedirect);
void changeDirectories(char *tokens[], int numTokens);
void printHelp(char *tokens[], int numTokens);

int main(int argc, char *argv[])
{
    bool checkRead = false;
    bool isRedirect = false;
    FILE *read = NULL;
    FILE *write = NULL;
    FILE *redirect = NULL;
    char *tokens[10];
    char* outputTokens[10];
    bool isExit = false;
    // Immediately check if there is command line arguments for a batchfile name
    // Input and output file stream should be initialized to a batchfile or NULL

    if(argc > 1)
    {
        char input[255];
        read = fopen(argv[1], "r");
        checkRead = read != NULL;
        if(checkRead) 
        {
            while (!feof(read))
            {
                fscanf (read, "%[^\n]%*c", input);
                fprintf(stdout, "%s\n", input);
                char *exCommand = executeCommand(input, &isRedirect, &isExit, tokens, outputTokens);
            }
        }

        else
        {
            printf("Could not read file!\n");
        }
    }

    else if (argc > 2)
    {
        printError();
        return 1;
    }

    else 
    {
        char cmd[255];
        while(isExit != true)
        {
            promptUser(checkRead);
            fscanf (stdin, "%[^\n]%*c", cmd);
            char *exCommand = executeCommand(cmd, &isRedirect, &isExit, tokens, outputTokens);          
        }
    }

    // After performing the batch, promptUser() should display the prompt
    // If the user enters a batch file name on the commandline, then input stream file pointer and a bool variable should open read stream for the file
    // If the user doesnt enter a  batch file name, the batchmode bool should best to false, the input stram file pointer should be set to stdin
    // fgets() or fscanf() to read command line or batchfile
    // If reading batchfile, contents should be displayed
    // Should be parsed into tokens using strtok()
    // Tokens should be passed to executeCommand() function
    // If the users enters "kill" exitProgram()
    // executeCommand() returns the name of any output files if the uer performs redirect in terminal or an epty string if a redirect command is not present
    // If the user is redirecting, the string should be used to safely (perform a null check) open the redirect output file (if its not an empty string)
    // The redirectstream should be used to open the file that you're getting input (should be the first string in strtok)

};

int parseInput(char *input, char *splitWords[])
{
    //provided
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
    // Should output "Shell Program Error Encountered"
    // It is used whenever the user enters nonsense or a command not implemented by me or execvp

    printf("Shell Program Error Encountered\n");
};

void promptUser(bool isBatch)
{
    // Displays the user name, machine hostname, and current working directory username@machineName:/folder/
    // Paramater should be passed by main and be used to check if prompt should be printed

    if (isBatch != true)
    {
        char hostText[255];
        int  hostNum = gethostname(hostText, sizeof(hostText));
        char *username = getenv("USER");
        char cwd[255];
        char *cwdtmp = getcwd(cwd, sizeof(cwd));

        fprintf(stdout, "%s@%s:%s$ ", username, hostText, cwd);
    }
};

char *executeCommand(char *cmd, bool *isRedirect, bool *isExit, char* tokens[], char* outputTokens[])
{
    char *output = "";
    char *command = strdup(cmd);
    strcat(command, "\n");

    // Input char* should be the command provided by the user
    // Use strchr() to check if the redirect symbol ">" is used and store the return in an appropriate variable

    char *redirCom = strchr(command, '>');
    if (redirCom != NULL) 
    {
        output = redirectCommand(command, command, isRedirect, tokens, outputTokens);
        return output;
    }

    // Return from strchr is not null then you should call  redirectCommand() which returns the output file name, so store it, and return the output file name from this function
    // If the return from strchr is null, then youn should parseInput storing the number of returned tokens
    // If the number of tokens is 0 return from function
    // If the return is null call exitProgram()

    else 
    {
        int count = parseInput(cmd, tokens);
        if(count == 0) 
        {
            return NULL;
        }

    // If the user has chosen to exit, you should immediately return the output file name
    // Otherwise you should call changeDirectories, printHelp, and launchProcesses
        else 
        {
            *isExit = exitProgram(tokens, count);
            changeDirectories(tokens, count);
            printHelp(tokens, count);
            launchProcesses(tokens, count, false);
            printf("\n");
            return command;
        }
    }
};

char *redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[])
{
    // The user command passed should be parsed by strtok()
    // Parse twice, once to get input file name and once to get output file name

    char text[100];
    int count = parseInput(special, tokens);
    if (strcmp("cat", tokens[0]) == 0)
    {
        if (access(tokens[1], F_OK) == -1)
        {
            *isRedirect = false;
            printError();
            return NULL;
        }
    }

    int rTally = 0;
    int tTally = 0;
    for (int i = 0; i < strlen(special); i++)
    {
        if (special[i] == '>')
        {
            rTally++;
        }

        if (special[i] == '.')
        {
            tTally++;
        }
    }

    if (rTally > 1 || tTally > 2)
    {
        *isRedirect = false;
        printError();
        return NULL;
    }

    char *tmp = strdup(tokens[count-1]);
    char temp[100];
    int i=0;
    int len = (strlen(tmp) - 5);
    for (i; i < len; i++)
    {
        temp[i] = tmp[i];
    }

    char *output = strcat(temp, ".txt");
    FILE *redirect = freopen(output, "w+", stdout);
    *isRedirect = true;
    launchProcesses(tokens, count, true);
    fclose(redirect);
    freopen("/dev/tty", "w", stdout);
    *isRedirect = false;
    return output;
};

bool exitProgram(char *tokens[], int numTokens)
{
   // Use strcmp() to check if the first char** argument is exit if it is then return true, unless the edge case outline below occurs, if exit is not an argument, then retunrn false.
    int isExit = strcmp("exit", tokens[0]);
    if (isExit == 0)
    {
        if (numTokens < 2)
        {
            return true;
        }
        else
        {
            printError();
            return false;
        }
    }
    else
    {
        return false;
    }
};

void changeDirectories(char *tokens[], int numTokens)
{
    // execvp does not execute the change directories command, to implement this functionality, I will need to write your own changeDirectories function
    // It should use strcmp to check if the first char ** argument is cd, if it is then you should use chdir to change directories to the path specified by the user
    int cdCheck = strcmp("cd", tokens[0]);
    if (cdCheck == 0)
    {
        if (numTokens > 1)
        {
            int x = chdir(tokens[1]);
        }
        else
        {
            printError();
        }
    }
};

void printHelp(char *tokens[], int numTokens)
{
    // execvp does not execute the help command so you will write your own modified help screen. Use strcmp to check if first char arg is help
    // "Noah's example linux shell
    // These shell commands are defined internally
    // help               - prints this screen so you can see available shell commands.
    // cd                 - changes directories to specified path; if not given, defaults to hostname
    // exit               - closes the example shell
    // [input] > [output] - pipes input file into output file
    // And more! If its not explicitly defined here (or in the documentation for the assignment), then the command should try to be executed by launchProcesses
    // Thats how we get ls -la to work here!"
    int isHelp = strcmp("help", tokens[0]);
    if (isHelp == 0)
    {
        if (numTokens < 2)
        {

            printf("\nNoah's example linux shell\n");
            printf("These shell commands are defined internally\n");
            printf("help               - prints this screen so you can see available shell commands.\n");
            printf("cd                 - changes directories to specified path; if not given, defaults to hostname.\n");
            printf("exit               - closes the example shell.\n");
            printf("[input] > [output] - pipes input file into output file.\n");
            printf("And more! If its not explicitly defined here (or in the documentation for the assignment), then the command should try to be executed by launchProcesses.\n");
            printf("Thats how we get ls -la to work here!\n");
        }
        else
        {
            printError();
        }
    }
};

void launchProcesses(char *tokens[], int numTokens, bool isRedirect)
{
    if (strcmp("exit", tokens[0]) == 0 || strcmp("cd", tokens[0]) == 0 || strcmp("help", tokens[0]) == 0)
    {
    }
    // Uses execvp to execute a provided command as a process
    // This function should iterate through each provided argument, and fork a child process for each provided argument
    // Then execvp should be provided the command from the user and any associated argument tokens
    else
    {
        int status;
        char *args[10];
        if (isRedirect == true)
        {
            for (int i = 0; i < numTokens-2; i++)
            {
                args[i] = tokens[i];
            }

            args[numTokens-2] = NULL;
        }

        else
        {
            for (int i = 0; i < numTokens; i++)
            {
                args[i] = tokens[i];
            }

            args[numTokens] = NULL;
        }

   
// Return from execvp should be stored to check for errors
// Finally after checking wait (to make sure execvp was finished) it can move on

        if (fork() == 0) 
        {
            status = execvp(tokens[0], args); 
            if (status == -1) 
            {
                printError();
            }
        }

        else
        {
            wait(&status);
        }
    }
};