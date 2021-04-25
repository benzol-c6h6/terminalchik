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
#define ERR_CODE -1
#define MAXTOKEN 16
 
struct sigaction sig;
struct process 
{
    pid_t pid;
    char **tokens;
    int count_tokens;
    int is_background;
};

char *chik_cmd[CMD_COUNT];
extern void (*chik_func[CMD_COUNT])(struct process*);

void shell_chik(); 
char* read_chik();    //чтение
void parsing_chik(char* string_command, struct process* command);
void exec_chik(struct process* command);    //исполнение

void chik_cd(struct process* command);
void chik_help(struct process* command);
void chik_exit(struct process* command);
void run_program(struct process* command);
void kill_parent(int signum);
void kill_child(int signum);