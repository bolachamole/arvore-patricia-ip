#include <stdlib.h>
#include <stdint.h>

#define MAX_BITS 128 //IPv6

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
	novo->skip = MAX_BITS;
	novo->eh_prefixo = 1;
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

char compara_bits(uint32_t prefixo1, uint32_t prefixo2){

}

char compara_n_bits(uint32_t prefixo, short int bitN){
	
}

int insere_no(triePatricia* arvore, uint32_t prefixo){
	tipoNo* novo = criar_no(prefixo);
	tipoNo* atual = arvore->raiz;

	if(atual == NULL){
		arvore->raiz = novo;
		arvore->tamanho++;
		return 0;
	}

	tipoNo* novo_pai = NULL;
	while(atual->skip < MAX_BITS){
		if((atual->eh_prefixo == 1) && ((atual->prefixo ^ prefixo) == 0)){
			free(novo);
			return 1; //pois ja esta na arvore
		}
		if(compara_n_bits(prefixo, atual->skip) == 0){
			atual = atual->esq;
		} else{
			atual = atual->dir;
		}
	}
	novo_pai->skip = conta_zeros(atual->prefixo ^ prefixo);
	
	return 1;
}

tipoNo* busca_prefixo_mais_longo(triePatricia* arvore, uint32_t prefixo){
	tipoNo* atual = arvore->raiz;
	tipoNo* match = NULL;

	while(atual->skip < MAX_BITS){
		char result = compara_bits(atual->prefixo, prefixo);
		if((atual->eh_prefixo == 1) && (result == 0)){
			match = atual;
		}
		else if((atual->eh_prefixo == 1) && (result != 0)){
			return match; //pois o match anterior e o prefixo mais longo concordante
		}
		if(compara_n_bits(prefixo, atual->skip) == 0){
			atual = atual->esq;
		} else{
			atual = atual->dir;
		}
	}
	return match;
}

void remove_no(triePatricia* arvore, uint32_t prefixo){
	tipoNo* atual = arvore->raiz;
	if(atual->skip < MAX_BITS){
		while(compara_bits(atual->prefixo, prefixo) != 0){
			if(compara_n_bits(prefixo, atual->skip) == 0){
				atual = atual->esq;
			} else{
				atual = atual->dir;
			}
		}
		free(atual);
	}
}

