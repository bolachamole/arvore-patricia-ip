#include <stdlib.h>
#include <stdint.h>

#define MAX_BITS 32 //IPv4
#define tipoBits uint32_t

typedef struct{
	tipoBits bits;
	short int tamanho;
}prefixo;

typedef struct No tipoNo;
struct No{
	prefixo* ip;
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

tipoNo* criar_no(prefixo* ip){
	tipoNo* novo = malloc(sizeof(tipoNo));
	novo->ip = ip;
	novo->skip = MAX_BITS;
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

char conta_zeros(tipoBits bits){
	char quant_zeros = 0;
	if (bits == 0) return MAX_BITS;
	if (bits & 0xffff0000) bits &= 0xffff0000; else quant_zeros += 16;
	if (bits & 0xff00ff00) bits &= 0xff00ff00; else quant_zeros += 8;
	if (bits & 0xf0f0f0f0) bits &= 0xf0f0f0f0; else quant_zeros += 4;
	if (bits & 0xcccccccc) bits &= 0xcccccccc; else quant_zeros += 2;
	if (bits & 0xaaaaaaaa) bits &= 0xaaaaaaaa; else quant_zeros += 1;
	return quant_zeros;
}


tipoBits compara_bits(prefixo* bitsNovo, prefixo* bitsFilho){
	short int i;
	short int tam1 = bitsNovo->tamanho;
	short int tam2 = bitsFilho->tamanho;
	tipoBits bits = bitsNovo->bits;

	if(tam1 < tam2){
		for(i = 0; i < (tam2 - tam1); i++){
			bits <<= 1;
		}
	} else if(tam1 > tam2){
		for(i = 0; i < (tam1 - tam2); i++){
			bits >>= 1;
		}
	}
	return bits ^ bitsFilho->bits;
}

char compara_n_bits(prefixo* bitsNovo, prefixo* bitsFilho, short int nInicio, short int nFim){
	short int i;
	tipoBits bits1 = bitsNovo->bits;
	tipoBits bits2 = bitsFilho->bits;
	char bit1, bit2;

	for(i = 0; i < nInicio; i++){
		bits1 <<= 1;
		bits2 <<= 1;
	}
	for(i = 0; i < (nFim - nInicio); i++){
		bit1 = (bits1 >> (MAX_BITS-1)) & 1;
		bit2 = (bits2 >> (MAX_BITS-1)) & 1;
		if(bit1 != bit2) return bit1;
		bits1 <<= 1;
		bits2 <<= 1;
	}

	return MAX_BITS;
}

int insere_no(triePatricia* arvore, prefixo* bits){
	tipoNo* novo = criar_no(bits);
	tipoNo* atual = arvore->raiz;

	if(atual == NULL){
		arvore->raiz = novo;
		arvore->tamanho++;
		return 0;
	}
	int onde = 0;
	short int tam = bits->tamanho;
	tipoNo* pai = atual->pai;
	while(atual->skip < MAX_BITS){
		if(pai != NULL){
			if(compara_n_bits(novo->ip, atual->ip, pai->skip, atual->skip) == 0){
				atual = atual->esq;
				onde = 0;
			} else{
				atual = atual->dir;
				onde = 1;
			}
		} else{
			if(compara_n_bits(novo->ip, atual->ip, 0, atual->skip) == 0){
				atual = atual->esq;
				onde = 0;
			} else{
				atual = atual->dir;
				onde = 1;
			}
		}
		pai = atual->pai;
	}
	if((tam == atual->ip->tamanho) && (compara_bits(novo->ip, atual->ip) == 0)){
		free(novo);
		return 1; //pois ja esta na arvore
	}
	tipoNo* novo_pai = criar_no(NULL);
	novo_pai->skip = conta_zeros(compara_bits(novo->ip, atual->ip));
	if(onde == 0){
		novo_pai->esq = novo;
		novo_pai->dir = pai;
	}else if(onde == 1){
		novo_pai->esq = pai;
		novo_pai->dir = novo;
	}
	pai->pai = novo_pai;
	novo->pai = novo_pai;
	
	return 0;
}

prefixo* busca_prefixo_mais_longo(triePatricia* arvore, prefixo* bits){
	tipoNo* atual = arvore->raiz;
	if(atual == NULL) return NULL;

	prefixo* match = NULL;
	tipoNo* pai = atual->pai;
	while(atual->skip < MAX_BITS){
		if(pai != NULL){
			if(compara_n_bits(bits, atual->ip, pai->skip, atual->skip) == 0){
				atual = atual->esq;
			} else{
				atual = atual->dir;
			}
		} else{
			if(compara_n_bits(bits, atual->ip, 0, atual->skip) == 0){
				atual = atual->esq;
			} else{
				atual = atual->dir;
			}
		}
		pai = atual->pai;
	}
	if(compara_bits(bits, atual->ip) == 0){
		match = atual->ip;
	}
	return match;
}

prefixo* remove_no(triePatricia* arvore, prefixo* bits){
	tipoNo* atual = arvore->raiz;
	if(atual == NULL) return NULL;
	
	int onde = 0;
	short int tam = bits->tamanho;
	tipoNo* pai = atual->pai;
	while(atual->skip < MAX_BITS){	
		if(pai != NULL){
			if(compara_n_bits(bits, atual->ip, pai->skip, atual->skip) == 0){
				atual = atual->esq;
				onde = 0;
			} else{
				atual = atual->dir;
				onde = 1;
			}
		} else{
			if(compara_n_bits(bits, atual->ip, 0, atual->skip) == 0){
				atual = atual->esq;
				onde = 0;
			} else{
				atual = atual->dir;
				onde = 1;
			}
		}
		pai = atual->pai;
	}
	if((tam == atual->ip->tamanho) && (compara_bits(bits, atual->ip) == 0)){
		if(pai != NULL){
			if(onde == 0) pai->dir->pai = pai->pai;
			else if(onde == 1) pai->esq->pai = pai->pai;
			free(pai);
		}
		free(atual);
		return bits;
	}
	return NULL;
}

