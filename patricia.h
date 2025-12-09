#include <stdlib.h>
#include <stdint.h>

#define MAX_BITS 32 //IPv4
#define tipoBits uint32_t

typedef struct{
	tipoBits bits;
	short int tamanho;
}prefixo;

typedef struct patricia triePatricia;
triePatricia* cria_arvore();
int insere_no(triePatricia* arvore, prefixo* bits);
prefixo* busca_prefixo_mais_longo(triePatricia* arvore, prefixo* bits);
prefixo* remove_no(triePatricia* arvore, prefixo* bits);
