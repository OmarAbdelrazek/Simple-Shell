#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/wait.h>
int flag = 0;
int firstOpen = 0;


void childSignal(int pid)
{
//if file exist delete it
    if(firstOpen == 0)
    {
        remove("log.txt");
        firstOpen = 1;
    }

    //open text file
    FILE *ptr = fopen("/home/omar/Desktop/OS/NewSimpleShell/simpleshell/log.txt","ab");
    if(ptr == NULL)
    {
        printf("ERROR");
        exit(1);
    }
    else
    {
        fprintf(ptr,"a child (%d) process terminated!!\n",pid);
    }
    fclose(ptr);


}


char** get_input()

{
    int s, i, j;
    char x;
// this function checks char by char, x is the char
//i and j is iterators and , s = 0 => Outside the word ,s = 1 => Inside the word
    char **input = malloc(10*sizeof(char*));
    if(input == NULL)
    {
        exit(1);
    }
    for(i=0; i<10; ++i)
    {
        input[i] = malloc(10*sizeof(char));
    }
    while(1)
    {
        putchar('$');
        s = i = j = 0;

        while((x = getchar()) != '\n')
        {
            if(x == ' ')
            {
                if(s)
                {
                    input[i++][j] = 0;
                    j = s = 0;
                }
            }
            else if(x == '&')
            {
                flag = 1;
                continue;
            }
            else
            {
                s = 1;
                input[i][j++] = x;
            }
        }

        input[i++][j]=0;
        input[i] = NULL;
        if(strcmp(input[0], ""))
        {
            break;
        }
    }
    return input;
}

int main()
{

    char **userInput;
    int pid;
    while(1)
    {
        userInput = get_input();
        signal(SIGCHLD,childSignal);
//exit the shell
        if(!strcmp(userInput[0], "exit"))
        {
            free(userInput);
            exit(0);
        }
        else if(strcmp(userInput[0],"cd") == 0) {
        //chdir returns a 0 when succeesful, less than a 0 when it failed
            if(chdir(userInput[1]) < 0) {
                perror(userInput[1]);
            }
        }

        else
        {
            pid = fork();
            if(pid < 0)
            {
                printf("fork faild");
                free(userInput);
                exit(1);
            }
            else if(pid == 0)
            {
                execvp(userInput[0], userInput);


            }

            else
            {
                if(flag == 0)
                {
                    wait(pid);

                }


            }


        }

        free(userInput);


    }

    return 0;
}
