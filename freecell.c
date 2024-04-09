#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

typedef struct no{
    short num;
    char naipe;
    struct no *prox;
} tCarta;

tCarta *primMesa[8];
tCarta *primNaipe[4];
tCarta *temp[4];
tCarta *primMonte = NULL;
tCarta *ultMesa[8];
tCarta *ultNaipe[4];

bool confereCores(tCarta *c1, tCarta *c2);
void inicializa();//inicializa em null
void gerarBaralho();
void embaralhaBaralho();
void ultimoMesa();//encontra ultimo da mesa
void distribuirMesa();
void imprime();
void moveMesaNaipe();
void moveMesaTemp();
void moveTempMesa();
void moveNaipeTemp();
void moveTempNaipe();
void moveNaipeMes();
void moveMesMes();
void limpa();
void eFigura(tCarta *carta);

int main(int argc, char **argv){
    int op=0;
    srand(time(NULL));
    inicializa();
    gerarBaralho();
    embaralhaBaralho();
    distribuirMesa();

    while(op!=8){
        imprime();
        printf("(1)Move Mesa-Naipe\n");
        printf("(2)Move Mesa-Temp\n");
        printf("(3)Move Temp-Mesa\n");
        printf("(4)Move Naipe-Temp\n");
        printf("(5)Move Temp-Naipe\n");
        printf("(6)Move Naipe-Mes\n");
        printf("(7)Move Mes-Mes\n");
        printf("(8)Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();
        switch(op){
            case 1:
                moveMesaNaipe();
                break;
            case 2:
                moveMesaTemp();
                break;
            case 3:
                moveTempMesa();
                break;
            case 4:
                moveNaipeTemp();
                break;
            case 5:
                moveTempNaipe();
                break;
            case 6:
                moveNaipeMes();
                break;
            case 7: 
                moveMesMes();
                break;
            default:
                printf("Opcao Invalida.\n");
                break;
        } 
    }
    limpa();
    return 0;
}

void inicializa(){//inicializa em null
    int i;
    for(i=0;i<8;i++){
        primMesa[i] = NULL;
        ultMesa[i] = NULL;
    }
    for (i = 0; i < 4; i++){
        primNaipe[i]=NULL;
        temp[i]=NULL;
        ultNaipe[i]=NULL;
    } 
}

void gerarBaralho(){
    tCarta *novo;
    int i,j;
    for(i=3; i<7; i++){
        for(j=1;j<14;j++){
            novo = (tCarta *)malloc(sizeof(tCarta));
            novo->num = j;
            novo->naipe = i;
            novo->prox = primMonte;
            primMonte = novo;
        }
    }
}//end gerarBaralho

void embaralhaBaralho(){
    //sortea duas posições e inverte de lugar as cartas dessas posições
    int i, j, num1, num2;
    tCarta *carta1, *ant1, *carta2, *ant2, *proxi;
    for (i = 0; i < 100; i++){
        num1 = rand()%52;
        num2 = abs(num1-14);
        ant1 = NULL;
        ant2 = NULL;
        carta1 = primMonte;
        carta2 = primMonte;
        if(num1>0 && abs(num2)!=0){//confere se pos são válidas
            for(j=0;j<num1;j++){//encontra carta1
                ant1 = carta1;
                carta1 = carta1->prox;
            }
            for(j=0;j<num2;j++){//encontra carta 2
                ant2 = carta2;
                carta2 = carta2->prox;
            }
            //troca de lugar
            proxi = carta1->prox;
            ant1->prox = carta2;
            ant2->prox = carta1;
            carta1->prox = carta2->prox;
            carta2->prox = proxi;
        }
    }
}//end embaralhaBaralho

void distribuirMesa(){
    //função incrementa uma carta por pilha da mesa
    int i=0;
    tCarta *atual, *ant=NULL;
    while (primMonte != NULL){
        atual = primMonte;
        primMonte = primMonte->prox;
        if (primMesa[i] == NULL){//para a primeira carta
            primMesa[i] = atual;
            primMesa[i]->prox = NULL;
        } 
        else{
            ant = primMesa[i];
            primMesa[i] = atual;
            primMesa[i]->prox = ant;
        } 
        i = (i+1)%8;//muda para a proxima pilha
    }
    ultimoMesa();
}//end distribuirMesa

void ultimoMesa(){
    int i;
    tCarta *atual;
    for(i=0; i<8; i++){
        atual = primMesa[i];
        while(atual->prox != NULL){
            atual = atual->prox;
        }
        ultMesa[i] = atual;
    }
}

void imprime(){
    int i;
    tCarta *atual;
    //imprime temp
    printf("[TEMP] = ");
    for(i=0;i<4;i++){
        if(temp[i] != NULL)
            eFigura(temp[i]);
        else 
            printf("%d-[  ] ", i);
        
    }
    printf("\n");
    for(i=0;i<4;i++){
        atual = primNaipe[i];
        printf("[NAIPE %d]=", i);
        while (atual != NULL){
            eFigura(atual);
            atual = atual->prox;
        }
        printf("\n");
    }
    printf("\n");
    //imprime mesas
    for(i=0;i<8;i++){
        atual = primMesa[i];
        printf("[MESA %d]=", i);
        while (atual != NULL){
            eFigura(atual);
            atual = atual->prox;
        }
        if(primMesa[i] == NULL){
            printf(" ");
        }
        printf("\n");
    }
}//end imprime

bool confereCores(tCarta *c1, tCarta *c2){
    if(((c1->naipe == 3) || (c1->naipe == 4))&&((c2->naipe == 5) || (c2->naipe == 6)))
        return true;
    if(((c2->naipe == 3) || (c2->naipe == 4))&&((c1->naipe == 5) || (c1->naipe <= 6)))
        return true;
    return false;
}

bool verificaMonte(tCarta *carta){
    while(carta->prox != NULL){
        if(confereCores(carta, carta->prox) && carta->num == carta->prox->num+1)
            carta = carta->prox;
        else
            return false;
    }
    return true;
}

void moveMesaNaipe(){
    int posM, posN;
    tCarta *aux, *ant=NULL;
    printf("De que pilha da mesa deseja mover a carta?(0-7)");
    scanf("%d", &posM);
    printf("Para qual posicao doa Naipes?(0-3)");
    scanf("%d", &posN);
    if(posM>=0 && posM<8 && posN>=0 && posN<4){//confere se os valores estão dentro do esperado
        if(primMesa[posM] != NULL){
            aux = primMesa[posM];
            while (aux->prox != NULL){//encontra a carta que será retirada da mesa
                ant = aux;
                aux = aux->prox;
            }
            if((primNaipe[posN] == NULL && aux->num == 1) || ((primNaipe[posN] != NULL) && (aux->naipe == ultNaipe[posN]->naipe) && (aux->num - 1 == ultNaipe[posN]->num))){
                if(ant == NULL){
                    primMesa[posM]= NULL;
                    ultMesa[posM] = NULL;
                }else{
                    ant->prox = NULL;
                    ultMesa[posM] = ant;
                }
                if(primNaipe[posN] == NULL)
                    primNaipe[posN] = aux;
                else
                    ultNaipe[posN]->prox=aux;
                ultNaipe[posN] = aux;
                aux->prox = NULL;
            }else
                printf("\nNao e possivel mover esta carta para este naipe\n");
        }else
            printf("\nNao ha cartas nessa pilha da mesa\n");
    }else
        printf("\nPosicao invalida\n");
}//end moveMesaNape

void moveMesaTemp(){
    int posM, posT;
    tCarta *aux, *ant=NULL;
    printf("De que pilha da mesa deseja mover a carta?(0-7)");
    scanf("%d", &posM);
    printf("Para qual posicao do Temporario?(0-3)");
    scanf("%d", &posT);
    if(posM>=0 && posM<8 && posT>=0 && posT<4){//confere se os valores estão dentro do esperado
        if(temp[posT] == NULL && primMesa[posM] != NULL){//confere se há carta na mesa e se a posição do temp está vazia
            aux = primMesa[posM];
            while (aux->prox != NULL){//encontra a carta que será retirada da mesa
                ant = aux;
                aux = aux->prox;
            }
            temp[posT] = aux;//atribui a carta a posição do temp
            if(ant == NULL) {//corrige a mesa modificada
                primMesa[posM] = NULL;
                ultMesa[posM] = NULL;
            }else{
                ant->prox = NULL;
                ultMesa[posM] = ant;
            }
        }else 
            printf("\nPosicao do temporario ocupada ou mesa vazia\n");
    }else
        printf("\nVoce digitou uma posicao invalida, por favor tente novamente com um valor dentro do esperado!\n");
}//end moveMesaTemp

void moveTempMesa(){ 
    int posM, posT;
    tCarta *atual;
    printf("De que posicao do Temporario deseja mover a carta?(0-3)");
    scanf("%d", &posT);
    printf("Para qual pilha da mesa?(0-7)");
    scanf("%d", &posM);
    if(temp[posT] != NULL){//confere se os valores estão dentro do esperado
        if(posM>=0 && posM<8 && posT>=0 && posT<4){
            atual = temp[posT];
            if(primMesa[posM] == NULL){
                atual->prox = NULL;
                primMesa[posM] = atual;
                ultMesa[posM] = atual;
                temp[posT] = NULL;
            }
            else{
                if((temp[posT]->num) == ((ultMesa[posM]->num)-1)){
                    if(confereCores(temp[posT], ultMesa[posM])){
                        temp[posT]->prox = NULL;
                        ultMesa[posM]->prox = temp[posT];
                        ultMesa[posM] = temp[posT];
                        temp[posT] = NULL;
                    }else
                        printf("\nPosicao invalida, cartas de mesma cor.\n");
                }else
                    printf("\nCarta da mesa não pode receber carta do temporario.\n");
            }
        }else 
            printf("\nVoce digitou uma posicao invalida, por favor tente novamente com um valor dentro do esperado!\n");
    }
}//end moveTempMesa

void moveNaipeTemp(){
    int posN, posT;
    tCarta *atual, *ant=NULL;
    printf("De qual pilha de naipes deseja mover a carta?(0-3)");
    scanf("%d", &posN);
    printf("Para qual posição do temporario?(0-3)");
    scanf("%d", &posT);
    //confere se os valores estão dentro do esperado
    if(posN>=0 && posN<4 && posT>=0 && posT<4){
        if(primNaipe[posN] != NULL && temp[posT] == NULL){
            atual = primNaipe[posN];
            while(atual->prox != NULL){
                ant = atual;
                atual = atual->prox;
            }
            temp[posT] = atual;
            if(ant == NULL){
                primNaipe[posN] = NULL;
                ultNaipe[posN] = NULL;
            }else{
                ant->prox = NULL;
                ultNaipe[posN] = ant;
            }
        }else
            printf("Posicao do temporario ocupada ou pilha de naipe vazia, tente novamente\n");
    }else
        printf("Posicoes invalidas\n");
}

void moveTempNaipe(){
    int posN, posT;
    tCarta *aux;
    printf("De qual posição do temporario deseja mover a carta?(0-3)");
    scanf("%d", &posT);
    printf("Para qual pilha de naipes?(0-3)");
    scanf("%d", &posN);  
    if(posN>=0 && posN<4 && posT>=0 && posT<4){
        if(temp[posT] != NULL){
            aux = temp[posT];
            if((primNaipe[posN] == NULL && aux->num == 1) || ((primNaipe[posN] != NULL) && (aux->naipe == primNaipe[posN]->naipe) && (aux->num - 1 == primNaipe[posN]->num))){
                if(primNaipe[posN] == NULL){
                    primNaipe[posN] = aux;
                }else{
                    ultNaipe[posN]->prox = aux;
                }
                ultNaipe[posN] = aux;
                temp[posT] = NULL;
            }
        }else
           printf("Posicao do temporario vazia, tente novamente\n"); 
    }else
        printf("Posicoes invalidas\n");
}

void moveNaipeMes(){
    int posM, posN;
    tCarta *atual, *ant=NULL;
    printf("De que pilha de naipes deseja mover a carta?(0-3)");
    scanf("%d", &posN);
    printf("Para qual pilha da mesa?(0-7)");
    scanf("%d", &posM);
    if(posM>=0 && posM<8 && posN>=0 && posN<4){
        if(primNaipe[posN] != NULL){
            atual = primNaipe[posN];
            while(atual->prox != NULL){
                ant = atual;
                atual = atual->prox;
            }
            if(primMesa[posM] == NULL || (primMesa[posM] != NULL && atual->num == (ultMesa[posM]->num)-1 && confereCores(atual, ultMesa[posM]))){
                if(primMesa[posM] == NULL)
                    primMesa[posM] = atual;
                else
                    ultMesa[posM]->prox = atual;
                if(ant == NULL){
                    primNaipe[posN] = NULL;
                    ultNaipe[posN] = NULL;
                }else{
                    ant->prox = NULL;
                    ultNaipe[posN] = ant;
                }
                atual->prox = NULL;
                ultMesa[posM] = atual;
            }else
                printf("Essa carta não pode ser movida para esta pilha da mesa");
        }else
            printf("Nao ha cartas nesta pilha de naipes");
    }else
        printf("Posicoes invalidas\n");
}

void moveMesMes(){
    int posO, posD, op, quant=0, i, j=0;
    tCarta *atual, *ant=NULL, *aux;
    printf("De qual pilha da mesa deseja mover a carta?(0-3)");
    scanf("%d", &posO);
    printf("Para qual pilha?(0-3)");
    scanf("%d", &posD);
    if(posO>=0 && posO<8 && posD>=0 && posD<8){
        if(primMesa[posO] != NULL){
            printf("1 - Se deseja mover uma carta\n2 - Se deseja mover um monte de cartas\n");//Verifica tipo de movimento
            scanf("%d", &op);
            if(op == 1){
                atual = primMesa[posO];
                while(atual->prox != NULL){
                    ant = atual;
                    atual = atual->prox;
                }
                if(primMesa[posD] == NULL)
                    primMesa[posD] = atual;
                else
                    ultMesa[posD]->prox = atual;
                if(ant == NULL){
                    primMesa[posO] = NULL;
                    ultMesa[posO] = NULL;
                }else{
                    ant->prox = NULL;
                    ultMesa[posO] = ant;
                }
                atual->prox = NULL;
                ultMesa[posD] = atual;
            }if(op == 2){
                atual = primMesa[posO];
                while(atual != NULL){
                    atual = atual->prox;
                    quant++;
                }
                printf("Digite a posicao na pilha da carta que inicia o monte desejado(0-%d):", quant-1);
                scanf("%d", &i);
                if(i>=0 && i<=quant){
                    ant = NULL;
                    atual = primMesa[posO];
                    while(j != i){
                        ant = atual;
                        atual = atual->prox;
                        j++;
                    }
                    if(verificaMonte(atual) && ultMesa[posD]->num == (atual->num+1) && confereCores(ultMesa[posD], atual)){
                        if(primMesa[posD] == NULL)
                            primMesa[posD] = atual;
                        else
                            ultMesa[posD]->prox = atual;
                        if(ant == NULL){
                            primMesa[posO] = NULL;
                            ultMesa[posO] = NULL;
                        }else{
                            ant->prox = NULL;
                            ultMesa[posO] = ant;
                        }
                        aux = primMesa[posD];
                        while(aux->prox != NULL)
                            aux = aux->prox;
                        ultMesa[posD] = aux;
                    }else
                        printf("Monte não pode ser movido\n");
                }else
                    printf("Posicao de carta invalida\n");
            }else
                printf("Opção invalida\n");
        }else
            printf("Pilha de origem vazia\n");
    }else
        printf("Posicao invalidas\n");
}

void eFigura(tCarta *carta){
    int num = carta->num;
    switch (num){
        case  1:
            printf("[ A/%c] ", carta->naipe);
            break;
        case 11:
            printf("[ J/%c] ", carta->naipe);
            break;
        case 12:
            printf("[ Q/%c] ", carta->naipe);
            break;
        case 13:
            printf("[ K/%c] ", carta->naipe);
            break;
        default:
            printf("[%02d/%c] ", carta->num, carta->naipe);
            break;
    }
}
void limpa(){
    int i;
    tCarta *atual;
    tCarta *apaga;
    for(i=0;i<8;i++){
        atual = primMesa[i];
        while(atual != NULL){
            apaga = atual;
            atual = atual->prox;
            free(apaga);
        }
    }
    for (i = 0; i < 4; i++){
        atual = primNaipe[i];
        while(atual != NULL){
            apaga = atual;
            atual = atual->prox;
            free(apaga);
        }
        free(temp[i]);
    }
}


