#include "stdio.h"
#include "stdlib.h"
#include "string.h"
char* copiar( char *saida, int inicio, int fim)
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
int main(int argc, char const *argv[])
{
    char a[100], *b;
    b = "jorge oliveira";
    printf("%s", b);
    return 0;
}
