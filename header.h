#pragma once
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CMD_COUNT 3
#define DEFAULT "\033[0m"
#define BRIGHT "\033[1m"
#define YELLOW "\033[33m" 
#define CYAN "\033[36m"
#define LINE "============================================\n"
#define MAX_LEN_PATH 256

extern const int ERR_CODE;
extern const int MAXTOKEN;
 

char *chik_cmd[CMD_COUNT];
extern int (*chik_func[CMD_COUNT])(char **);

int shell_chik(); 
char* read_chik();    //чтение
char** parsing_chik(char* command, int *count_token); //парсинг  
int exec_chik(char** command_tokens);    //исполнение

int chik_cd(char** commands_token);
int chik_help(char** commands_token);
int chik_exit(char** commands_token);
int run_program(char** commands_token);
