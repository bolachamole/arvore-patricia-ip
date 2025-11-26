#include <stdlib.h>
#include <stdint.h>

typedef struct No tipoNo;
struct No{
	uint32_t prefixo;
	short int skip;
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

int insere_no(triePatricia* arvore, tipoNo* raiz, uint32_t prefixo){
	tipoNo* novo = criar_no(prefixo);
	if(raiz == NULL){
		raiz = novo;
		arvore->tamanho++;
		return 0;
	} else if((raiz->prefixo ^ prefixo) != 0){
		raiz->skip = conta_zeros(raiz->prefixo ^ prefixo);
		if(( (prefixo & (1 << (raiz->skip - 1))) >> (raiz->skip - 1) ) == 0){
			insere_no(arvore, raiz->esq, prefixo);
		} else{
			insere_no(arvore, raiz->dir, prefixo);
		}
	}
	return 1;
}

void deleta_no(triePatricia* arvore, uint32_t prefixo){
}

void comprime_caminho(triePatricia* arvore){
}

tipoNo* busca_prefixo_mais_longo(triePatricia* arvore){
}
