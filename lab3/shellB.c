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
char terminalHistory[MAXCMDHISTORY][MAXLINE];
int numberOfCommands = 0;

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

    if (length == 0)
        exit(0);            /* Cntrl-d was entered, end of user command stream */

    if (length < 0){
        perror("error reading command");
	exit(-1);           /* Terminate with error code of -1 */
    }
    
    if (strncmp(inputBuff, "rr", 2) == 0){
            strcpy(inputBuff, commandHistory[numberOfCommands - 1]);
        if ((strncmp(inputBuff, "history", 7) == 0) || (strncmp(inputBuff, "h", 1) == 0)){
            int top;
            if (numberOfCommands < MAXCMDHISTORY){
                top = numberOfCommands;
            }
            else{
                top = MAXCMDHISTORY;
            }

            int a = 0;
            while (a < top){
                int commandVal = a + 1;
                printf("Command #%d: %s\n", commandVal, terminalHistory[a]);
                a++;
            }
        }       
    }
    else if(strncmp(inputBuff, "r", 1) == 0){
        if(length < 4){ // For r1 through r10
            int commandNum = inputBuff[1] - '0';
            --commandNum;
            strcpy(inputBuff, commandHistory[commandNum]);
        }
        if ((strncmp(inputBuff, "history", 7) == 0) || (strncmp(inputBuff, "h", 1) == 0)){
            int top;
            if (numberOfCommands < MAXCMDHISTORY){
                top = numberOfCommands;
            }
            else{
                top = MAXCMDHISTORY;
            }

            int a = 0;
            while (a < top){
                int commandVal = a + 1;
                printf("Command #%d: %s\n", commandVal, terminalHistory[a]);
                a++;
            }
        }
    }

    // Add the Command to history
    strcpy(commandHistory[numberOfCommands % MAXCMDHISTORY], inputBuff);
//    int pos = numberOfCommands % MAXCMDHISTORY;
//    char num = pos + '0';
//    printf("%c\n", num);
//    fwrite(&pos, 1, sizeof(int), fp);
    //fwrite(num, 1, sizeof(num), fp);
    fwrite(commandHistory[numberOfCommands % MAXCMDHISTORY], 1, sizeof(commandHistory[numberOfCommands % MAXCMDHISTORY]), fp);
    char* newLine = '\n';
    char* zeroLine = '\0';

    int a = 0;
    while (inputBuff[a] != newLine && inputBuff[a] != zeroLine){
        terminalHistory[numberOfCommands % MAXCMDHISTORY][a] = inputBuff[a];
        a++;
    }

    ++numberOfCommands;

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
    
    //Loop to print command line parameters
    //for (i=0; i<j; ++i) {
	//printf("In setup: args[%i] is %s\n", i, args[i]);
	//printf("In setup: (args+1)[0] is %s\n", (args+1)[0]);
    //}
    
} 


int main(void)
{

    char inputBuff[MAXLINE]; /* Input buffer  to hold the command entered */
    char *args[MAXLINE/2+1];/* Command line arguments */
    int background;         /* Equals 1 if a command is followed by '&', else 0 */
    
    int a = 0;
    while (a < MAXCMDHISTORY){
        strcpy(commandHistory[a],"");
        a++;
    }



    while (1){            /* Program terminates normally inside setup */
    FILE* fp = fopen("commandhistory.txt", "ab+"); //Open File

	pid_t pid;
	char **command_args_ptr = &args[1];
	background = 0;

	printf("CSE2431Sh$ ");  /* Shell prompt */
        fflush(0);

        setup(inputBuff, args, &background, fp);       /* Get next command */

    if ((strncmp(inputBuff, "history", 50) == 0) || (strncmp(inputBuff, "h", 50) == 0)){
        int top;
        if (numberOfCommands < MAXCMDHISTORY){
            top = numberOfCommands;
        }
        else{
            top = MAXCMDHISTORY;
        }

        int a = 0;
        while (a < top){
            int commandVal = a + 1;
            printf("Command #%d: %s\n", commandVal, terminalHistory[a]);
            a++;
        }
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
    fclose(fp); //Close file pointer
    }
}
