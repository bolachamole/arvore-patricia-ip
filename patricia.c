#include <stdlib.h>
#include <stdint.h>

typedef struct No tipoNo;
struct No{
	uint32_t prefixo;
	short int skip;
	short int eh_prefixo;
	tipoNo* pai;
	tipoNo* esq;
	tipoNo* dir;
};

typedef struct patricia triePatricia;
struct patricia{
	tipoNo* raiz;
	int tamanho;
};

tipoNo* criar_no(uint32_t prefixo){
	tipoNo* novo = malloc(sizeof(tipoNo));
	novo->prefixo = prefixo;
	novo->skip = 1;
	novo->eh_prefixo = 0;
	novo->pai = NULL;
	novo->esq = NULL;
	novo->dir = NULL;
	return novo;
}

triePatricia* cria_arvore(){
	triePatricia* arvore = malloc(sizeof(triePatricia));
	arvore->raiz = NULL;
	arvore->tamanho = 0;
	return arvore;
}

short int conta_zeros(uint32_t prefixo){
	short int quant_zeros = 0;
	if (prefixo == 0) return 1;
	if (prefixo & 0xffff0000) prefixo &= 0xffff0000; else quant_zeros += 16;
    if (prefixo & 0xff00ff00) prefixo &= 0xff00ff00; else quant_zeros += 8;
    if (prefixo & 0xf0f0f0f0) prefixo &= 0xf0f0f0f0; else quant_zeros += 4;
    if (prefixo & 0xcccccccc) prefixo &= 0xcccccccc; else quant_zeros += 2;
    if (prefixo & 0xaaaaaaaa) prefixo &= 0xaaaaaaaa; else quant_zeros += 1;
	return quant_zeros;
}

tipoNo* insere_no_filhos(triePatricia* arvore, tipoNo* atual, tipoNo* pai, uint32_t prefixo){
	tipoNo* novo = criar_no(prefixo);
	if(atual == NULL){
		atual = novo;
		atual->pai = pai;
		arvore->tamanho++;
		return novo;
	} else if((atual->prefixo ^ prefixo) != 0){
		atual->skip = conta_zeros(atual->prefixo ^ prefixo);
		if(( (prefixo & (1 << (atual->skip - 1))) >> (atual->skip - 1) ) == 0){
			insere_no_filhos(arvore, atual->esq, atual, prefixo);
		} else{
			insere_no_filhos(arvore, atual->dir, atual, prefixo);
		}
	}
	return atual;
}

int insere_no(triePatricia* arvore, uint32_t prefixo){
	tipoNo* novo = criar_no(prefixo);
	if(arvore->raiz == NULL){
		arvore->raiz = novo;
		arvore->raiz->eh_prefixo = 1;
		arvore->tamanho++;
		return 0;
	} else{
		novo = insere_no_filhos(arvore, arvore->raiz, NULL, prefixo);
		if(novo){
			arvore->raiz = novo;
			return 0;
		}
	}
	return 1;
}

tipoNo* busca_prefixo_mais_longo(triePatricia* arvore, uint32_t prefixo){
	tipoNo* atual = arvore->raiz;
	tipoNo* match = NULL;

	while(atual != NULL){
		if((atual->eh_prefixo == 1) && ((atual->prefixo ^ prefixo) == 0)){
			match = atual;
		}
		if(( (prefixo & (1 << (atual->skip-1))) >> (atual->skip-1) ) == 0){
			atual = atual->esq;
		} else{
			atual = atual->dir;
		}
	}
	return match;
}

void deleta_no(triePatricia* arvore, uint32_t prefixo){
}

void comprime_caminho(triePatricia* arvore){
}
