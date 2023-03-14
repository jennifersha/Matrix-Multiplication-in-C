
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>

int main(int argc, const char * argv[]) {
    int size = 30; //size of the buffer for directory
    int sizecm = 510; // size of the commands
    char buf[size];
    char linefg[sizecm];
    char ** mnarr;
    char ** mnarr2;
    int arrsiz;
    int arrsiz2;
    int numcmd = 0;
    int numcmdp = 0;
    int numcmdred = 0;
    int wrdlng = 0;
    pid_t x;
    pid_t y;
    int fds[2];
    char *filerot = NULL;
    int redi = 0;
    char *nwfilenm = NULL;
    int errnm = 0;
    struct passwd * nm;
    nm = getpwuid(getuid());
    char* nam = nm -> pw_name;  // get the name of the user
    
   
        if (getcwd(buf, size) == NULL) { // get the directory
        printf("getcwd() error");
        }
 
    
    char bol = '0';
    
    while (bol == '0' || bol == '1') {  // the main loop
        mnarr = (char **)malloc(1*sizeof(char**));
        mnarr2 = (char **)malloc(1*sizeof(char**));
        // full command array
        if (mnarr == NULL) {
            printf("ERR");
            bol = '2';
            break;
        }
        if (mnarr2 == NULL) {
            printf("ERR");
            bol = '2';
            break;
        }
        arrsiz = 0;
        arrsiz2 = 0;
        printf("%s@%s>",nam,buf);
        
        fgets(linefg, size, stdin);
        linefg[strlen(linefg)-1] = '\0';
        
    
        if(strcmp(linefg , "done") == 0) {
            bol = '2';
            int f = open(argv[1], O_WRONLY | O_APPEND);
                        if (f == -1)
                        {
                            printf("Error opening file!\n");
                        }
                        write(f,"done\n", 5);
                        close(f);

            break;   // end
        }
        
        char *tmp = strtok(linefg," ");
        
        
        
        while (tmp != NULL) { // splice commands
            int f = open(argv[1], O_WRONLY | O_APPEND);
            if (f == -1)
            {
                int f = open(argv[1], O_CREAT | O_WRONLY | O_APPEND);
            }

            write(f, tmp, strlen(tmp));
            write(f, " ", 1);
            close(f);
            
           
            
            if (redi > 0) {
                numcmdred++;
                nwfilenm = malloc(strlen(tmp) + 4);
                strcpy(nwfilenm, tmp);
                strcat(nwfilenm, ".txt");
                tmp = strtok(NULL," ");
                continue;
            }
            
            if (strlen(tmp) == 1 && tmp[0] == '|') {
                numcmdp++;
                bol = '1';
                tmp = strtok(NULL," ");
                continue;
            }
            else if (strlen(tmp) > 1 && tmp[strlen(tmp)-1] == '|') {
                numcmdp++;
                if (bol == '0') {
                    bol = '1';
                if (arrsiz > 0) {
                    mnarr = (char **) realloc(mnarr, (arrsiz+1)*sizeof(char**));// add one more space
                }
                arrsiz++;
                wrdlng += strlen(tmp);
                mnarr[arrsiz-1] = (char *) malloc(strlen(tmp)*sizeof(char)); // for the words in the command
                    tmp[strlen(tmp)-1] = '\0';
                strcpy(mnarr[arrsiz-1], tmp);
                }
                tmp = strtok(NULL," ");
                continue;
            }
            else if (strlen(tmp) > 1 && tmp[0] == '|') {
              numcmdp++;
                if (bol == '0') {
                    bol = '1';
                // for the words in the command
                    char * nwchr = "";
                    
                        nwchr = &tmp[1];
                    
                    if (arrsiz2 > 0) { // add one more space
                        mnarr2 = (char **) realloc(mnarr2, (arrsiz2+1)*sizeof(char**));
                    }
                    arrsiz2++;
                    mnarr2[arrsiz2-1] = (char *) malloc(strlen(nwchr)*sizeof(char)); // for the words in the command
                    strcpy(mnarr2[arrsiz2-1], nwchr);
                    
                }
                tmp = strtok(NULL," ");
                continue;
            }
            else  {
                
                
                int h = 0;
                
                while (h < strlen(tmp)) {
                    
                    if (bol == '0') {
                       if (tmp[h] == '|') {
                          numcmdp++;

                            bol = '1';
                      

                        if (arrsiz > 0) {
                            mnarr = (char **) realloc(mnarr, (arrsiz+1)*sizeof(char**));// add one more space
                        }
                        arrsiz++;
                        wrdlng += strlen(tmp);
                        mnarr[arrsiz-1] = (char *) malloc(h*sizeof(char)); // for the words in the command
                        
                        strncpy(mnarr[arrsiz-1], tmp, h);
                           
                            char * nwchr = "";
                          
                            nwchr = &tmp[h+1];
                                           
                          
                            if (arrsiz2 > 0) { // add one more space
                                mnarr2 = (char **) realloc(mnarr2, (arrsiz2+1)*sizeof(char**));
                            }
                            arrsiz2++;
                            mnarr2[arrsiz2-1] = (char *) malloc(strlen(nwchr)*sizeof(char)); // for the words in the command
                            strcpy(mnarr2[arrsiz2-1], nwchr);
                          
                          tmp = strtok(NULL," ");
                        break;
                        }
                        
                   }
                    
                    h++;
                
                }
                
            
        if (tmp != NULL) {
            
            if (strlen(tmp) == 1 && tmp[0] == '>') {
                redi = 1;
                tmp = strtok(NULL," ");
                continue;
            }
            
            if (strlen(tmp) == 1 && tmp[0] == '<') {
                redi = 2;
                tmp = strtok(NULL," ");
                continue;
            }
            
            if (strlen(tmp) == 2 && tmp[0] == '>' && tmp[1] == '>') {
                redi = 3;
                tmp = strtok(NULL," ");
                continue;
            }
            
            if (strlen(tmp) == 2 && tmp[0] == '2' && tmp[1] == '>') {
                redi = 4;
                tmp = strtok(NULL," ");
                continue;
            }
            
            if (bol == '0') {
            if (arrsiz > 0) {
                mnarr = (char **) realloc(mnarr, (arrsiz+1)*sizeof(char**));// add one more space
            }
            arrsiz++;
            wrdlng += strlen(tmp);
            mnarr[arrsiz-1] = (char *) malloc(strlen(tmp)*sizeof(char)); // for the words in the command
            strcpy(mnarr[arrsiz-1], tmp);
            tmp = strtok(NULL," ");
            }
            else if (bol == '1') {
            
            if (arrsiz2 > 0) { // add one more space
                mnarr2 = (char **) realloc(mnarr2, (arrsiz2+1)*sizeof(char**));
            }
            arrsiz2++;
            wrdlng += strlen(tmp);
            mnarr2[arrsiz2-1] = (char *) malloc(strlen(tmp)*sizeof(char)); // for the words in the command
            strcpy(mnarr2[arrsiz2-1], tmp);
            tmp = strtok(NULL," ");
        
            }
                }
            }
        }
        
        if (bol == '1') {
            bol = '0';
        }
        
        if(strcmp(linefg , "cd") == 0) { // free the command and cancel the request
            for (int i = 0 ; i < arrsiz ; i++) {
                free(mnarr[i]);
            }
            free(mnarr);
            for (int i = 0 ; i < arrsiz2 ; i++) {
                free(mnarr2[i]);
            }
            free(mnarr2);
            printf("not supported (yet)\n");
            continue;
        }
        mnarr = (char **) realloc(mnarr, (arrsiz+1)*sizeof(char**)); // add null for the last
        mnarr[arrsiz] = NULL;
        arrsiz++;
        
        if (arrsiz > 0) {

        x = fork();
            
        if (x == 0) {
            
            if (arrsiz2 > 0) {
                 
                pipe(fds); // creates a two sided pipe

                y = fork();
                
                if (y == 0) {
                    
                    close(fds[0]); //closes pipes
                    dup2(fds[1],1); // sets the child output to write pipe
                    
                    if (redi == 2) {
                        
                        int fd = open(nwfilenm, O_RDONLY );
                        if (fd == -1) {
                            fd = open(nwfilenm, O_CREAT | O_RDONLY );
                        }
                        dup2(fd,0);
                    }
                    
                    errnm = execvp(mnarr[0], mnarr);
                    printf("execvp did not execute successfully.\n");
                    close(fds[1]);
                    exit(0);
                    
                }
                else {
                    
                    
                    
                    close(fds[1]); // closes pipes
                    dup2(fds[0],0); // sets the father in to the pipe input
                    
                    
                    
                    if (redi == 1) {
                        
                        int fd = open(nwfilenm, O_WRONLY );
                        if (fd == -1) {
                            fd = open(nwfilenm, O_CREAT |O_WRONLY );
                        }
                        dup2(fd,1);
                    }
                    
                    else if (redi == 3) {
                        
                        int fd = open(nwfilenm, O_WRONLY | O_APPEND);
                        if (fd == -1) {
                            fd = open(nwfilenm, O_CREAT | O_WRONLY | O_APPEND);
                        }
                        dup2(fd,1);
                    }
                    
                    else if (redi == 4) {
                        
                        int fd = open(nwfilenm, O_WRONLY);
                        if (fd == -1) {
                            fd = open(nwfilenm, O_CREAT |O_WRONLY );
                        }
                        dup2(fd,2);
                    }
                    wait(NULL);
                   
                    mnarr2 = (char **) realloc(mnarr2, (arrsiz2+1)*sizeof(char**)); // add null for the last
                    mnarr2[arrsiz2] = NULL;
                    arrsiz2++;
                    execvp(mnarr2[0], mnarr2);
                    close(fds[0]);
                    exit(0);
                }
            }
            
            else {
                if (redi == 1) {
                    
                    int fd = open(nwfilenm, O_WRONLY );
                    if (fd == -1) {
                        fd = open(nwfilenm, O_CREAT |O_WRONLY );
                    }
                    dup2(fd,1);
                }
                
                else if (redi == 2) {
                    
                    int fd = open(nwfilenm, O_RDONLY );
                    if (fd == -1) {
                        fd = open(nwfilenm, O_CREAT | O_RDONLY );
                    }
                    dup2(fd,0);
                }
                
                
                else if (redi == 3) {
                    
                    int fd = open(nwfilenm, O_WRONLY | O_APPEND);
                    if (fd == -1) {
                        fd = open(nwfilenm, O_CREAT | O_WRONLY | O_APPEND);
                    }
                    dup2(fd,1);
                }
                
                else if (redi == 4) {
                    
                    int fd = open(nwfilenm, O_WRONLY);
                    if (fd == -1) {
                        fd = open(nwfilenm, O_CREAT |O_WRONLY );
                    }
                    dup2(fd,2);
                }
                
                

                execvp(mnarr[0], mnarr); // send the command to the son after fork

                
            }
            
        }
        else {
            numcmd++;
            
            wait(NULL);
            wait(NULL);
            if (errnm == -1) {
                printf("error");
            }
            int f = open(argv[1], O_WRONLY | O_APPEND);
                        if (f == -1)
                        {
                            printf("Error opening file!\n");
                            exit(1);
                        }
                        write(f,"\n", 1);
                        close(f);
            
        }
        }
        // free all data in the array
        for (int i = 0 ; i < arrsiz ; i++) {
            free(mnarr[i]);
        }
        free(mnarr);
           
        for (int i = 0 ; i < arrsiz2 ; i++) {
            free(mnarr2[i]);
        }
        free(mnarr2);
        redi = 0;
        
    }
    
    double avlen = (wrdlng+4)/(numcmd+1);
    free(filerot);
    printf("number of commands: %d \n",numcmd+1);
    printf("Total length of all commands: %d \n",wrdlng+5);
    printf("Average  length of all commands: %f \n",avlen);
    printf("number of commands that include pipe : %d \n",numcmdp);
    printf("number of commands that include redirection : %d \n",numcmdred);
    printf("See You Next Time !\n");
    
    return 0;
}
