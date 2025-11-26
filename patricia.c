#include <stdlib.h>
#include <stdint.h>

typedef struct No tipoNo;
struct No{
	void* prefixo;
	short int skip;
	tipoNo* pai;
	tipoNo* esq;
	tipoNo* dir;
};

typedef struct patricia triePatricia;
struct patricia{
	tipoNo* raiz;
	short int tamanho;
	int num_inserts;
	int num_buscas;
	int num_compara;
	int num_remocoes;
};

tipoNo* criar_no(void* prefixo){
	tipoNo* novo = malloc(sizeof(tipoNo));
	novo->prefixo = prefixo;
	novo->skip = 0;
	novo->pai = NULL;
	novo->esq = NULL;
	novo->dir = NULL;
	return novo;
}

triePatricia* cria_arvore(){
	triePatricia* arvore = malloc(sizeof(triePatricia));
	arvore->raiz = NULL;
	arvore->tamanho = 0;
	arvore->num_inserts = 0;
	arvore->num_buscas = 0;
	arvore->num_compara = 0;
	arvore->num_remocoes = 0;
	return arvore;
}

void insere_no(triePatricia* arvore, void* novo){
}

void deleta_no(triePatricia* arvore, void* prefixo){
}

void comprime_caminho(triePatricia* arvore){
}

tipoNo* busca_prefixo_mais_longo(triePatricia* arvore){
}
