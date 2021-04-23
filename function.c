#include "header.h"

const int ERR_CODE = -1;
const int MAXTOKEN = 16;
char *chik_cmd[CMD_COUNT] = {
    "cd",
    "help",
    "exit"
};
int (*chik_func[CMD_COUNT])(char **) = {
    chik_cd,
    chik_help,
    chik_exit
};

int shell_chik()
{
    char *command;
    char **command_tokens;
    int count_token = 0;
    char path[MAX_LEN_PATH];
    //чтение
    //проверка пути
    getcwd(path, MAX_LEN_PATH);
    printf(BRIGHT YELLOW "\nchik:" CYAN " %s " DEFAULT "> ", path);
    command = read_chik();

    //парсинг
    command_tokens = parsing_chik(command, &count_token);
    if (command_tokens == NULL)
    {
        return ERR_CODE;
    }

    //исполнение
    exec_chik(command_tokens);
    free(command);
    free(command_tokens);
    return 0;
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
    //printf("size_command = %li, bufsize = %lu, strlen(buf) = %lu, command = <%s>\n", size_command, bufsize, strlen(command), command);
    return read_cmd;
}

char** parsing_chik(char* command, int *count_token)
{
    char *temp = strtok(command, " \0");
    if (temp == NULL)
    {
        return NULL;
    }
    char **tokens = malloc(sizeof(char)*(MAXTOKEN));
    if (tokens == NULL)
    {
        perror("err");
        return NULL;
    }
    *count_token = 0;
    //tokens[*count_token] = temp;
    while (temp != NULL)
    {
        tokens[*count_token] = temp;
        (*count_token)++;
        temp = strtok(NULL, " \0");
    }
    tokens[*count_token] = NULL;
    return tokens;
}

int exec_chik(char** command_tokens)
{
   for (int i = 0; i < CMD_COUNT; i++)
   {
    if (strncmp(chik_cmd[i], command_tokens[0], strlen(command_tokens[0])) == 0)
        {
           (*chik_func[i])(command_tokens);
           return 0;
        }
   }
    run_program(command_tokens);
    return 0;
}

int run_program(char** commands_token)
{
    pid_t pid; //
    pid_t wpid;

    switch(pid = fork())
    {
    case -1:    //ошибка
        perror("fork:");
        return ERR_CODE;
    case 0:     //код потомка
        if (execvp(commands_token[0], commands_token) == -1)
        {
            perror("execvp");
        }
    default:    //код родительского процееса
        sig.sa_handler = &kill_child;
        sigaction(SIGINT, &sig, NULL);

        wpid = waitpid(pid, NULL, 0);

        sig.sa_handler = &kill_parent;
        sigaction(SIGINT, &sig, NULL);
        
        if(errno != 0 && errno != EINTR)
        {
            perror("waitpid");
        }
    }
    return 0;
}

int chik_cd(char** command_tokens)
{
    printf("Выполняется cd\n");
    return 0;
}

int chik_help(char** command_tokens)
{
    printf("Выполняется help\n");
    return 0;
}

int chik_exit(char** command_tokens)
{
    printf("Выполняется exit\n");
    exit(0);
    return 0;
}

void kill_child(int signum)
{
    printf("\nПринудительное завершение процесса потомка\n");
    sig.sa_handler = &kill_parent;
} 

void kill_parent(int signum)
{
    printf("\nПринудительное завершение терминала\n");
    chik_exit(NULL);
} 