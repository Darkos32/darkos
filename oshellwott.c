#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "signal.h"

#define ANSI_COLOR_BLUE "\x1b[34m"
const int FOREGROUND = 0;
const int BACKGROUND = 1;
const int RUNNING = 1;
const int STOPPED = 0;
const int DONE = 2;
int loop = 1;

typedef struct struct_1
{
    //* Armazena todos os dados indispensáveis de um processo
    pid_t pid;
    int ground, situacao,posicao;
    char *path;

} programa;

programa *init_programa(char *path, int ground, int situacao, int posicao, pid_t pid)
{
   
    programa *novo;
    novo = malloc(sizeof(programa *));
    novo->path = path;
    novo->ground = ground;
    novo->situacao = situacao;
    novo->pid = pid;
    novo->posicao = posicao;
    return novo;
}

typedef struct struct_2
{
    int posicao_atual,tam;
    programa **atuais,**anteriores;
}jobs;

jobs *init_jobs(int tam, int atual)
{
    jobs *novo;
    novo = malloc(sizeof(jobs *));
    novo->tam = tam;
    novo->atuais = malloc(tam * sizeof(programa *));
    novo->anteriores = malloc(tam * sizeof(programa *));
    for (size_t i = 0; i < tam; i++)
    {
        novo->atuais[i] = NULL;
        novo->anteriores[i] = NULL;
    }

    novo->posicao_atual = atual;
    return novo;
}

typedef struct pilha_programas
{
    programa **itens;
    int topo;
    int tam;
} stack;

stack *initStack(int tam)
{
    stack *pilha;
    pilha = malloc(sizeof(stack *));
    pilha->itens = malloc(tam * sizeof(programa *));
    pilha->topo = 0;
    pilha->tam = tam;
    return pilha;
}

void stack_overflow(stack *pilha)
{
    if (pilha->topo == pilha->tam - 2)
    {
        programa **temp;
        temp = malloc(2 * pilha->tam * sizeof(programa *));
        for (size_t i = 0; i < pilha->topo; i++)
        {
            temp[i] = pilha->itens[i];
        }
        pilha->itens = temp;
    }
}

void push(stack *pilha, programa *no)
{
    stack_overflow(pilha);
    pilha->itens[pilha->topo++] = no;
}

programa *pop(stack *pilha)
{
    if (pilha->topo == 0)
    {
        return NULL;
    }

    pilha->topo--;
    return pilha->itens[pilha->topo];
}
programa  *programa_foreground;
jobs *job;
stack *pilha_de_acessos;

void print_programa(programa* processo,int flag){
    char marcador, *situacao;
    programa* ultimo_acesso = pop(pilha_de_acessos), *penultimo_acesso  = pop(pilha_de_acessos);
    if (processo == ultimo_acesso)
    {
        marcador = '+';
    }else if (processo == penultimo_acesso)
    {
        marcador = '-';
    }else
    {
        marcador = ' ';
    }
    push(pilha_de_acessos,penultimo_acesso);
    push(pilha_de_acessos, ultimo_acesso);
    switch (processo->situacao)
    {
    case 1 :
        situacao = "Running";
        break;

    case 0 :
        situacao = "Stopped";
        break;
    case 2:
        situacao = "Done";
        break;
    }
    switch (flag)
    {
    case 0:
        
        printf("[%d]%c %s           %s\n", processo->posicao, marcador, situacao, processo->path);
        break;

    
    case 1:
        printf("[%d]%c %d  %s           %s\n", processo->posicao, marcador, processo->pid, situacao, processo->path);
        break;
    case 2:
        printf("[%d] %d\n", processo->posicao, processo->pid);
    }
}

void **jobs_array_overflow()
{
    programa **novo_atual,**novo_anterior;
    novo_atual = malloc(2 * job->tam * sizeof(programa *));
    novo_anterior = malloc(2 * job->tam * sizeof(programa *));
    for (size_t i = 0; i < job->tam; i++)
    {
        novo_atual[i] = job->atuais[i];
        novo_anterior[i] = job->anteriores[i];
        novo_atual[i + job->tam] = NULL;
        novo_anterior[i + job->tam] = NULL;
    }
    job->tam = 2 * job->tam;
    job->atuais = novo_atual;
    job->anteriores = novo_anterior;
}

void update_status(){
    pid_t retorno;
    for (size_t i = 0; job->atuais[i] !=NULL; i++)
    {
        retorno = waitpid(job->atuais[i]->pid, NULL, WNOHANG);
        if (retorno!=0)
        {
            
            job->atuais[i]->situacao = DONE;
        }
    }
}

void get_jobs(char **args)
{

    update_status();
    
    if (args[1] == NULL)
    {
        
        for (size_t i = 0; job->atuais[i]!=NULL ; i++)
        {
            print_programa(job->atuais[i], 0);
           
        }
    }
}

void add_to_jobs(programa* processo){

    push(pilha_de_acessos, processo);
    if (job->posicao_atual == job->tam)
    {
        
        jobs_array_overflow();
    }
    job->anteriores = job->atuais;
    job->atuais[job->posicao_atual - 1] = processo;
    job->posicao_atual++;
}

int potencia(int base, int expoente)
{
    if (expoente < 0)
    {
        printf("ERRO! ESSA FUNÇAO NAO ACEITA EXPOENTES NEGATIVOS ");
        return -1;
    }
    int resultado = 1;
    for (size_t i = 0; i < expoente; i++)
    {
        resultado *= base;
    }
    return resultado;
}

int parse_int_simples(char entrada)
{
    //** Converte caracteres para números inteiros de 1 digitos*/
    switch ((int)entrada)
    {
    case 48:
        return 0;
        break;
    case 49:
        return 1;
        break;
    case 50:
        return 2;
        break;
    case 51:
        return 3;
        break;
    case 52:
        return 4;
        break;
    case 53:
        return 5;
        break;
    case 54:
        return 6;
        break;
    case 55:
        return 7;
        break;
    case 56:
        return 8;
        break;
    case 57:
        return 9;
        break;

    default:
        return -1;
        break;
    }
}

int parse_int_complexo(char *entrada)
{
    //*parse_int para números com mais de um dígito*/
    if (strlen(entrada) == 1)
    {
        return parse_int_simples(entrada[0]);
    }

    int maior_expoente = strlen(entrada) - 1, numero = 0;
    for (size_t i = 0; i < strlen(entrada); i++)
    {
        numero += parse_int_simples(entrada[i]) * potencia(10, maior_expoente - i);
    }
    return numero;
}

void put_on_foreground(char** args){
    int status, posicao;
    if (args[1] == NULL)
    {
        programa_foreground = pop(pilha_de_acessos);
        
    }else
    {
        posicao = parse_int_complexo(args[1]) - 1;
        programa_foreground = job->atuais[posicao];
    }

    printf("%s\n", programa_foreground->path);
    kill(programa_foreground->pid, SIGCONT);
    waitpid(programa_foreground->pid, &status, WUNTRACED);
    if (WIFSTOPPED(status))
    {
        programa_foreground->situacao = STOPPED;
        push(pilha_de_acessos, programa_foreground);
        print_programa(programa_foreground, 2);
    }
}

void put_on_background(char ** args){
    int status, posicao;
    programa *processo;
    if (args[1] == NULL)
    {
        processo = pop(pilha_de_acessos);
    }else
    {
        posicao = parse_int_complexo(args[1]) - 1;
        processo = job->atuais[posicao];
    }
    printf("%s\n", processo->path);
    kill(programa_foreground->pid, SIGCONT);
}

char *copiar(char *saida, int inicio, int fim)
{
    char *novo;
    novo = malloc(strlen(saida) * sizeof(char));
    for (size_t i = inicio; i < fim; i++)
    {
        novo[i - inicio] = saida[i];
    }
    novo[fim] = '\0';
    return novo;
}

char **overflow(char **array, int tam)
{
    char **novo;
    novo = malloc(2 * tam * sizeof(char *));
    for (size_t i = 0; i < tam; i++)
    {
        novo[i] = array[i];
    }
    return novo;
}

char **parse_linha(char *comando)
{
    int tamanho_linha = strlen(comando);
    int cabecote = 0, j = 0, tam = 10;
    char **args;
    args = malloc(tam * sizeof(char *));
    for (size_t i = 0; i < tamanho_linha; i++)
    {
        if (comando[i] == ' ')
        {
            args[j] = copiar(comando, cabecote, i);
            j++;
            cabecote = i + 1;
            if (j == tam)
            {
                args = overflow(args, tam);
                tam *= 2;
            }
        }
        if (i + 1 == strlen(comando))
        {
            args[j] = copiar(comando, cabecote, i);
        }
    }
    for (size_t i = j + 1; i < tam; i++)
    {
        args[i] = NULL;
    }

    return args;
}

int is_background(char **args)
{
    for (size_t i = 0; args[i] != NULL; i++)
    {
        if (!strcmp("&", args[i]))
        {
            return 1;
        }
    }
    return 0;
}

void handle_SIGINT(int sig){
    if (programa_foreground != NULL)
    {
        kill(programa_foreground->pid, SIGKILL);
    }
    
    
}

void handle_SIGTSTP(int sig){
    if (programa_foreground != NULL)
    {
        kill(programa_foreground->pid, SIGSTOP);
    }
    
   
}

void executar_programa(char **args)
{
    
    int status,ground = is_background(args) ? BACKGROUND : FOREGROUND;
    pid_t pid;
    if ((pid = fork()) == 0)
    {
       
        if(execv(args[0], args)==-1){
            printf("oshellwott: %s: No such file or directory\n",args[0]);
        }
    }
    else
    {
        programa *processo_filho;
        processo_filho = init_programa(args[0], ground, RUNNING, job->posicao_atual, pid);
        if (ground == FOREGROUND)
        {
            programa_foreground = processo_filho;
            waitpid(pid, &status, WUNTRACED);
            if (WIFSTOPPED(status))
            {
                processo_filho->situacao = STOPPED;
                add_to_jobs(processo_filho);
                puts("");
                print_programa(processo_filho, 0);       
            }
        }else
        {
            add_to_jobs(processo_filho);
            print_programa(processo_filho, 2);
        }
        
    }
}

void executar_comando(char **args)
{
    if (!strcmp(args[0], "cd"))
    {
        chdir(args[1]);
    }else if (!strcmp("exit",args[0]))
    {
        loop = 0;
    }else if (!strcmp("jobs",args[0]))
    { 
        get_jobs(args);
    }else if (!strcmp("fg",args[0]))
    {
        put_on_foreground(args);
    }
     else
    {
        executar_programa(args);
    }
}

int main()
{
    char a[100];
    job = init_jobs(10, 1);
    pilha_de_acessos = initStack(10);
    char comando[1024], **args;
    signal(SIGINT, handle_SIGINT);
    signal(SIGTSTP, handle_SIGTSTP);
    do
    {
        printf(ANSI_COLOR_BLUE "oshellwott~%s> " ANSI_COLOR_BLUE, getcwd(a, 100));
        fgets(comando, 1024, stdin);
        args = parse_linha(comando);
        executar_comando(args);

    } while (loop);

    return 0;
}
