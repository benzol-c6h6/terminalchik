#include "header.h"

char *chik_cmd[CMD_COUNT] = 
{
    "cd",
    "help",
    "exit"
};

void (*chik_func[CMD_COUNT])(struct process*) = 
{
    chik_cd,
    chik_help,
    chik_exit
};

void shell_chik()
{
    char *string_command;
    struct process command;
  
    char path[MAX_LEN_PATH];
    //чтение
    //проверка пути
    getcwd(path, MAX_LEN_PATH);
    printf(BRIGHT YELLOW "\nchik:" CYAN " %s " DEFAULT "> ", path);
    string_command = read_chik();

    //парсинг
    parsing_chik(string_command, &command);
    if (command.tokens == NULL)
    {
        return;
    }

    //исполнение
    exec_chik(&command);
    free(string_command);
    free(command.tokens);
    return;
}

char* read_chik()  
{
    char* read_cmd = NULL;
    size_t bufsize = 0;
    ssize_t size_command = getline(&read_cmd, &bufsize, stdin);
    if (size_command == ERR_CODE)
    {
        printf("Ошибка чтения команды!\n");
        perror("");
        return NULL;
    }
    read_cmd[size_command - 1] = '\0';
    //printf("size_command = %li, bufsize = %lu, strlen(buf) = %lu, command = <%s>\n", size_command, bufsize, strlen(read_cmd), read_cmd);
    return read_cmd;
}

//TODO: утечка на echo с 2 словами и больше
void parsing_chik(char* string_command, struct process* command)
{
    char *tr;
    char *temp = strtok_r(string_command, " \0", &tr);


    if (temp == NULL)
        return;
    
    command->tokens = malloc(sizeof(char)*(MAXTOKEN));
    if (command->tokens == NULL)
    {
        perror("malloc");
        return;
    }

    command->count_tokens = 0;
    while (temp != NULL)
    {
        command->tokens[command->count_tokens] = temp;
        //printf("token[%i] = <%s>", command->count_tokens, temp);
        command->count_tokens++;
        temp = strtok_r(NULL, " \0", &tr);
    }
    
    if(strcmp(command->tokens[(command->count_tokens) - 1], "&") == 0)
    {
        command->is_background = 1;
        command->count_tokens--;
    }
    else 
        command->is_background = 0;

    return;
}

void exec_chik(struct process* command)
{
   for (int i = 0; i < CMD_COUNT; i++)
   {
    if (strncmp(chik_cmd[i], command->tokens[0], strlen(command->tokens[0])) == 0)
        {
           (*chik_func[i])(command);
           return;
        }
   }
    run_program(command);
    return;
}

void run_program(struct process* command)
{
    pid_t wpid;

    command->pid = fork();

    switch(command->pid)
    {
    case ERR_CODE:    //ошибка
        perror("fork:");
        return;

    case 0:     //код потомка
        if (execvp(command->tokens[0], command->tokens) == -1)
        {
            perror("execvp");
            kill(getppid(), SIGINT);
            exit(0);
        }
        break;

    default:    //код родительского процееса
        if (command->is_background)
        {
            printf("Запущен фоновой процесс\n");
            break;
        }
        sig.sa_handler = &kill_child;
        sigaction(SIGINT, &sig, NULL);

        wpid = waitpid(command->pid, NULL, 0);

        sig.sa_handler = &kill_parent;
        sigaction(SIGINT, &sig, NULL);
        
        if(errno != 0 && errno != EINTR)
            perror("waitpid");
    }

    return;
}


void chik_cd(struct process* command)
{
    if (chdir(command->tokens[1]) == ERR_CODE)
        perror("cd");

    return;
}

void chik_help(struct process* command)
{
    printf("Встроенные команды в терминальчик:\n");
    printf("\t" BRIGHT "cd <путь>" DEFAULT " - переход в директорию\n");
    printf("\t" BRIGHT "help" DEFAULT " - маленькая подсказка о встроенных командах\n");
    printf("\t" BRIGHT "exit" DEFAULT " - выход из терминальчика\n");
    
    return;
}

void chik_exit(struct process* command)
{
    printf("Выполняется exit\n");
    exit(0);
}

void kill_child(int signum)
{
    sig.sa_handler = &kill_parent;
    printf("\nПринудительное завершение процесса потомка\n");
} 

void kill_parent(int signum)
{
    printf("\nПринудительное завершение терминала\n");
    chik_exit(NULL);
} 