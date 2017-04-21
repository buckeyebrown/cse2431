/*
* Dylan Brown
* CSE 2431
* Shell A
* Lab 3
* Due: 04/20
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 50 /* 50 chars per line, per command, should be enough. */
#define MAXCMDHISTORY 10

char commandHistory[MAXCMDHISTORY][MAXLINE];
int numberOfCommands;

void addToHistory(int pos, char* inputBuff);
void historyCommand();

/** The setup() routine reads in the next command line string storing it in the input buffer.
The line is separated into distinct tokens using whitespace as delimiters.  Setup also 
modifies the args parameter so that it holds points to the null-terminated strings  which 
are the tokens in the most recent user command line as well as a NULL pointer, indicating the
end of the argument list, which comes after the string pointers that have been assigned to
args. ***/

void setup(char inputBuff[], char *args[],int *background, FILE* fp)
{
    int length,  /* Num characters in the command line */
        i,       /* Index for inputBuff arrray          */
        j,       /* Where to place the next parameter into args[] */
        start;   /* Beginning of next command parameter */

    /* Read what the user enters */
    length = read(STDIN_FILENO, inputBuff, MAXLINE);  

    j = 0;
    start = -1;

    if (length == 0){
        saveHistory(fp);
        exit(0);
    }

    if (length < 0){
        perror("error reading command");
	exit(-1);           /* Terminate with error code of -1 */
    }
    
    if (strncmp(inputBuff, "rr", 2) == 0){
            memset(inputBuff, 0, sizeof(inputBuff));
            printf("%s", commandHistory[numberOfCommands - 1]);
            strcpy(inputBuff, commandHistory[numberOfCommands - 1]);
        if ((strncmp(inputBuff, "history", 7) == 0) || (strncmp(inputBuff, "h", 1) == 0)){
            historyCommand();
        }       
    }
    else if(strncmp(inputBuff, "r", 1) == 0){
        if(length < 4){ // For r0 through r9
            int commandNum = inputBuff[1] - '0';
            memset(inputBuff, 0, sizeof(inputBuff));
            printf("%s", commandHistory[commandNum]);
            strcpy(inputBuff, commandHistory[commandNum]);
        }
        if ((strncmp(inputBuff, "history", 7) == 0) || (strncmp(inputBuff, "h", 1) == 0)){
            historyCommand();
        }
    }

    // Add the Command to history
    int pos = numberOfCommands % MAXCMDHISTORY;
    addToHistory(pos, inputBuff);

    /* Examine every character in the input buffer */
    for (i = 0; i < length; i++) {
 
        switch (inputBuff[i]){
        case ' ':
        case '\t' :          /* Argument separators */

            if(start != -1){
                args[j] = &inputBuff[start];    /* Set up pointer */
                j++;
            }

            inputBuff[i] = '\0'; /* Add a null char; make a C string */
            start = -1;
            break;

        case '\n':             /* Final char examined */
            if (start != -1){
                args[j] = &inputBuff[start];     
                j++;
            }

            inputBuff[i] = '\0';
            args[j] = NULL; /* No more arguments to this command */
            break;

        case '&':
            *background = 1;
            inputBuff[i] = '\0';
            break;
            
        default :             /* Some other character */
            if (start == -1)
                start = i;
	}
 
    }    
    args[j] = NULL; /* Just in case the input line was > 80 */
    
} 

void exitProgram(FILE *fp)
{
    saveHistory(fp);
    fclose(fp); //Close file pointer
    exit(0);
}

void loadHistory(FILE *fp){
    /**
    int x = 0;
    char buffer[MAXLINE];
    while(!feof(fp)){
        strcpy(buffer, "\0");

        fgets(buffer, MAXLINE, fp);
        if (strcmp(buffer, "\0") != 0){
            strcpy(commandHistory[x], buffer);
            printf("%s\n", commandHistory);
        }
        x++;
    }
    */
    fread(commandHistory, sizeof(char), sizeof(commandHistory), fp);
    printf("History loaded.\n");
    rewind(fp);
    char c;
    while(!feof(fp)){
        c = fgetc(fp);
        if(c == '\n'){
            ++numberOfCommands;
                printf("commands: %d\n", numberOfCommands);
            }
    }
    rewind(fp);
}

void saveHistory(FILE *fp){
    /**
    int x = 0;
    char buffer[MAXLINE];
    while (x < MAXCMDHISTORY){
        strcpy(buffer, "\0");
        strcpy(buffer, commandHistory[x]);
        if(strcmp(buffer,"\0") != 0){        
            fprintf(fp, "%s", buffer);
        }
        x++;
    }*/
    fwrite(commandHistory, sizeof(char), sizeof(commandHistory), fp);
    printf("History written to file.\n");
}

int main(void)
{
    char inputBuff[MAXLINE]; /* Input buffer  to hold the command entered */
    char *args[MAXLINE/2+1]; /* Command line arguments */
    char historyBufferForFile[MAXCMDHISTORY * MAXLINE];
    int background;         /* Equals 1 if a command is followed by '&', else 0 */
    numberOfCommands = 0;

    int a = 0;
    while (a < MAXCMDHISTORY){
        memset(commandHistory[a], '\0', sizeof(commandHistory[a]));
        a++;
    }
    memset(inputBuff, '\0', sizeof(inputBuff));

    FILE* fp = fopen("commandhistory.txt", "ab+"); //Open File
    loadHistory(fp);

    while (1){            /* Program terminates normally inside setup */

	pid_t pid;
	char **command_args_ptr = &args[1];
	background = 0;

	printf("CSE2431Sh$ ");  /* Shell prompt */
        fflush(0);

        setup(inputBuff, args, &background, fp);       /* Get next command */

    if ((strncmp(inputBuff, "history", 7) == 0) || (strncmp(inputBuff, "h", 1) == 0)){
        historyCommand();
    }
    else if ((strncmp(inputBuff, "exit", 4) == 0) || (strstr(inputBuff, "exit") != NULL)){
        exitProgram(fp);
    }
    
            /* Fill in the code for these steps:  
         (1) Fork a child process using fork(),
         (2) The child process will invoke execvp(),
         (3) If bkgnd == 0, the parent will wait; 
            otherwise returns to the setup() function. */
        pid = fork();
            if (pid == 0) {
                //printf("**\n%s\n", args[0]);
                //printf("$$\n%s\n", args);
                execvp(args[0], args);
            }
            else {
                if (background == 0) wait(pid);
            }        
    }

}

void addToHistory(int pos, char* inputBuff){
    memset((commandHistory[numberOfCommands % MAXCMDHISTORY]), 0, sizeof(commandHistory[numberOfCommands % MAXCMDHISTORY]));
    strcpy(commandHistory[numberOfCommands % MAXCMDHISTORY], inputBuff);

    ++numberOfCommands;
}

void historyCommand(){
    int top;
    if (numberOfCommands < MAXCMDHISTORY){
        top = numberOfCommands;
    }
    else{
        top = MAXCMDHISTORY;
    }

    int a = 0;
    while (a < top){
        int b = 0;
        printf("Command #%d: ", a);
        while (commandHistory[a][b] != '\n' && commandHistory[a][b] != '\0'){
            printf("%c", commandHistory[a][b]);
            b++;
        }
        printf("\n");
        a++;
    }
}