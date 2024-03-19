
#include <stdio.h>
#include <string.h>
#include <stdlib.h>			// exit()
#include <unistd.h>			// fork(), getpid(), exec()
#include <sys/wait.h>		// wait()
#include <signal.h>			// signal()
#include <fcntl.h>			// close(), open()  
#define MAX_COMMANDS 10     
#define MAX_COMMAND_LEN 100 
#define CURR_WD_SIZE 10000 
/* Split the input string into tokens */
char **getCommands(char *input,int *k)
{
    char **argument = malloc(MAX_COMMANDS * sizeof(char *));
    char *parsed;
    int i = 0;
    /* Check if > symbol is present for output Redirection as a substring*/
    if (strstr(input, ">") != NULL)
	{
        /*Splitting the tokens for output Redirection*/
        *k=3;
		parsed=strtok(input,">");
        while(parsed != NULL)
        {
            argument[i] = parsed;
            i++;
            parsed = strtok(NULL," ");
        }
        argument[i]=NULL;
	}

    /* Splitting the string using " " as delimiter */
    else
    {
        parsed = strtok(input, " ");

        /* Keep getting tokens while one of the delimiters present in input[] */
        while (parsed != NULL)
        {
            argument[i] = parsed;
            i++;
            parsed = strtok(NULL, " ");
        }

        argument[i] = NULL;
    }
    return argument;
}
/* This function will parse the input string
   into multiple commands or a single command with
   arguments depending on the delimiter (&&, ##, >, or spaces) */
   
int parseInput(char **arg)
{   
    if (strcmp(arg[0], "exit") == 0)
    {
        return 0;
    }

    /* For executing parallel commands */
    int parallel_flag = 0; 
    char **commands = arg;
    while (*commands)
    {
        if (strcmp(*commands, "&&") == 0)
        {
            parallel_flag=1;
            break;
        }
        *commands++;
    }
    if (parallel_flag)                               
    {
        return 1;
    }
    commands=arg;
    /* For executing sequential commands */
    int sequential_flag = 0; 
    while (*commands)
    {
        if (strcmp(*commands, "##") == 0)
        {
            sequential_flag=1;
            break;
        }
        *commands++;
    }
    if (sequential_flag)                               
    {
        return 2;
    }
    commands=arg;
    /* For executing pipe*/
    int pipe_flag = 0; 
    while (*commands)
    {
        if (strcmp(*commands, "|") == 0)
        {
            pipe_flag=1;
            break;
        }
        *commands++;
    }
    if (pipe_flag)                              
    {
        return 3;
    }
    
    /* For execution of single commands */
    return 4; 
}

void executeCommand(char **arg)
{
    if (strlen(arg[0]) == 0)
    {
        return;
    }
    else if(arg[0]==NULL)
    {
        return;
    }
    else if (strcmp(arg[0], "cd") == 0) 
    {
        if(arg[1] != NULL)
        {
            char *path = arg[1];
            int fl=chdir(path);
            if (fl == -1)
            {
                printf("Shell: Incorrect command\n"); 
            }
        }
        else{
            char *path = "/home/sohail";
            int fl=chdir(path);
            if (fl == -1)
            {
                printf("Shell: Incorrect command\n"); 
            }
        }
    }
    else if (strcmp(arg[0], "exit") == 0) 
    {
        exit(0);
    }
    else
    {
        /* forking a child process */
        pid_t rc = fork(); 
        if (rc < 0)       
        {
            printf("Shell: Incorrect command\n");
        }
        else if (rc == 0) 
        {
            /* Signal handling */
            signal(SIGINT, SIG_DFL); //Restores the default behaviour of these signals
            signal(SIGTSTP, SIG_DFL);
            /* Execute other basic commands such as ls,pwd */
            if (execvp(arg[0], arg) < 0) 
            {
                printf("Shell: Incorrect command\n");
            }
            exit(0);
        }
        else 
        {
            /* Let the child process complete */
            wait(NULL); 
            return;
        }
    }
}



 void executeParallelCommands(char **arg)
 {
    char **temp_cmds = arg;
    char *commands_to_execute[MAX_COMMANDS][MAX_COMMAND_LEN];
    int i = 0;
    int j = 0;
    int cnt = 1;//Count of commands to execute

    pid_t rc = 1;
    int status; 
    
    /* Traversing through arguments and storing it in 'temp_cmds' so that commands can be executed parallely */
    while (*temp_cmds)
    {
        if (*temp_cmds && (strcmp(*temp_cmds, "&&") != 0))
        {
            commands_to_execute[i][j++] = *temp_cmds;
        }
        else
        {
            commands_to_execute[i++][j] = NULL;
            j = 0;
            cnt++;
        }
        if (*temp_cmds)
        {
            *temp_cmds++;
        }
    }

    commands_to_execute[i][j] = NULL;
    /* Traversing through all commands */
    for (int k = 0; k < cnt && rc != 0; ++k)
    {
        rc = fork(); 
        if (rc < 0)  
        {
            printf("Shell: Incorrect command\n");
            exit(1);
        }
        else if (rc == 0) 
        {
            /* Signal handling */
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            if(strcmp("cd",commands_to_execute[i][0]) == 0)/*execvp does not support cd*/
            {
                char *path = commands_to_execute[i][1];
                int fl=chdir(path);
                if (fl == -1)
                {
                    printf("Shell: Incorrect command\n"); 
                    return;
                }
                
            }
            else
            {
                execvp(commands_to_execute[k][0], commands_to_execute[k]); 
            }
        }
    }
    while (cnt > 0)
    {
        waitpid(-1, &status, WUNTRACED); /*WUNTRACED This allows you to wait for both terminated and stopped(by signal) child processes.*/
        cnt--; /*The wait() system call suspends execution of the calling process until one of its children terminates. The call wait(&status) is equivalent to: waitpid(-1, &status, 0); The waitpid() system call suspends execution of the calling process until a child specified by pid argument has changed state.*/

    }

 }



void executeSequentialCommands(char **arg) 
{
    char **curr_cmds = arg;
    while (*curr_cmds)
    {
        char *command_to_execute[MAX_COMMAND_LEN];
        int i = 0;
        while (*curr_cmds)
        {
            if (strcmp(*curr_cmds, "##") == 0) 
            {
                break;
            }
            command_to_execute[i] = *curr_cmds;
            i++;
            curr_cmds++;
        }
        command_to_execute[i] = NULL;
        if (*curr_cmds)
        {
            curr_cmds++;
        }
        executeCommand(command_to_execute);
    }
}

void executeCommandRedirection(char **str)
{
    
    int i = 0;
    int cur_size=0;
    char **curr_cmds=str;
    while(*curr_cmds)
    {
        cur_size++;
        *curr_cmds++;

    }
	char *commands[MAX_COMMAND_LEN];
	for (int k = 0; k < cur_size; k++)
	{
		char *original = strdup(str[k]);

		while (i < MAX_COMMAND_LEN)
		{
			commands[i] = strsep(&original, " ");
			if (commands[i] == NULL)
				break;
			if (strlen(commands[i]) != 0)
				i++;
		}
	}

	int n = i;
    pid_t forkChildPID;
	forkChildPID = fork();

	if (forkChildPID < 0)
	{
		printf("Shell: Incorrect command\n");
		exit(0);
	}
	else if (forkChildPID == 0)
	{
		close(STDOUT_FILENO); // Redirecting STDOUT
		open(commands[n - 1], O_CREAT | O_RDWR | O_APPEND);//opens a file in append mode

		commands[n - 1] = NULL; // making name null as it is not required anymore//Since its the file to which we want to redirect 
		execvp(commands[0], commands);

		printf("Shell: Incorrect command\n");
		exit(0);
	}
	else
	{
		wait(NULL);
	}
}

void parseSpace(char *input, char *com[10])
{
    int m = 0;
    char *token;
    char *remaining = input; // Initialize 'remaining' with input

    while ((token = strsep(&remaining, " ")) != NULL)
    {
        com[m] = token;
        m++;
    }

    com[m] = NULL;
}


void executeCommandPipeline(char *a[MAX_COMMANDS], int num_a)
{
    // Create pipes for communication between commands
    int pipes[num_a - 1][2];

    for (int i = 0; i < num_a - 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_a; i++)
    {
        pid_t child_pid = fork();

        if (child_pid == 0)
        {
            // Child process

            // Redirect input from the previous command's output
            if (i > 0)
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]); // Close the read end of the previous pipe
                close(pipes[i - 1][1]); // Close the write end of the previous pipe
            }

            // Redirect output to the next command's input
            if (i < num_a - 1)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][0]); // Close the read end of the current pipe
                close(pipes[i][1]); // Close the write end of the current pipe
            }

            // Close pipe file descriptors
            for (int j = 0; j < num_a - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Execute the command
            char *args[10];
            parseSpace(a[i], args);
            // printf("%s", args[0]);
            execvp(args[0], args);

            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }
        else if (child_pid > 0)
        {
            // Parent process

            // Close pipe file descriptors in the parent
            if (i > 0)
            {
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }
        }
        else
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all child processes to complete
    for (int i = 0; i < num_a; i++)
    {
        wait(NULL);
    }
}
void parsePipe(char *input, char *a[MAX_COMMANDS], int *num_a)
{
    //puts(input);
    *num_a = 0; // Initialize the number of commands to 0

    // Split the input into separate commands based on '|'
    char *token = strtok(input, "|");
    while (token != NULL)
    {
        if (*num_a >= MAX_COMMANDS)
        {
            fprintf(stderr, "Exceeded maximum number of commands in pipeline\n");
            exit(EXIT_FAILURE);
        }

        // Trim leading and trailing spaces from the command
        char *trimmed_command = token;
        while (*trimmed_command == ' ' || *trimmed_command == '\t')
        {
            trimmed_command++;
        }
        size_t len = strlen(trimmed_command);
        while (len > 0 && (trimmed_command[len - 1] == ' ' || trimmed_command[len - 1] == '\t'))
        {
            trimmed_command[--len] = '\0';
        }

        // Store the trimmed command in the 'commands' array
        a[(*num_a)++] = trimmed_command;

        token = strtok(NULL, "|");
    }
}


int main()
{
    signal(SIGINT, SIG_IGN);  /* Ignore signal interrupt (Ctrl+C) */
    signal(SIGTSTP, SIG_IGN); /* Ignore signal of suspending execution (Ctrl+Z) */
    
    while (1)
    {
        char currentDir[CURR_WD_SIZE];   /* Variable to store current working directory */
        char *command = NULL;     /* line ptr for getline function, and if it is set to null, buffer will be allocated to store line */
        int cmd_flag = 0;    /* To keep track of commands to be executed */
        size_t command_size = 0; /*To store size of command*/
        char *cwd=NULL; //Check length of current working directory
        size_t characters;//to check how many characters were entered
        cmd_flag = -1;//flag to check it is which type of instruction
        char **arg = NULL; 
        cwd=getcwd(currentDir, CURR_WD_SIZE);
        
        if (cwd != NULL)
		{
			printf("%s$",currentDir); // Print the prompt as - currentWorkingDirectory$
		}
		else
		{
			printf("Shell: Incorrect command\n"); // Too long path cannot be displayed...
            continue;
		}

        characters=getline(&command, &command_size, stdin);
        int temp=0;//variable to check whether to exit
        int redirect=0;//to check for output redirection
        
        char str[100];
        strcpy(str,command);

        /* Extract token surrounded by delimiter '\n' */
        command = strsep(&command, "\n");

        /* Extract token surrounded by delimiter " " */
        arg = getCommands(command,&redirect);
        /* If only \n or Enter is pressed*/
        if(arg[0]==NULL)
        {
            continue;
        }
        if(redirect == 3)
        {
            // This function is invoked when user wants redirect output of a single command to and output file specificed by user
             executeCommandRedirection(arg);
             continue;
        }

        /* Parse the input to check whether command is a single word or has &&, ## */
        
        cmd_flag = parseInput(arg);
        
        
        switch (cmd_flag)
        {
            /* When user has to exit the shell */
            case 0:
                printf("Exiting shell...\n");
                temp=1;
                break;
             // This function is invoked when user wants to run multiple commands in parallel (commands separated by &&)
            case 1:
                executeParallelCommands(arg); 
                break;
            // This function is invoked when user wants to run multiple commands sequentially (commands separated by ##)
            case 2:
                executeSequentialCommands(arg); 
                break;
            /*This function is invoked when using pipes*/
            case 3:
                str[characters - 1] = '\0';
                char *a[MAX_COMMANDS]; // a stands for commands
                int num_a;
                parsePipe(str, a, &num_a);
                executeCommandPipeline(a, num_a);
                break;
            
            
             /* This function is executed while running a single command */
            default:
                executeCommand(arg); 
                break;
        }
        
       if(temp)
       {
        break;
       }
    }
    return 0;
}
