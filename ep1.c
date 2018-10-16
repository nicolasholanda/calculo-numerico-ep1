#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#define ZERO 0.00000000001


void decimal_p_n(double dec, int base){
    int resto = 0, i=0, parte_int = dec;
    dec-=parte_int;
    char *bits = malloc((sizeof(dec)*8)+1);
    //PARTE INTEIRA
    while(parte_int >= 1){
        int aux = (parte_int%base);
        char c;
        if(aux>9){
            c = aux+7+'0';
            bits[i] = c;
        }
        else{
            bits[i] = aux+'0';
        }
        parte_int = parte_int/base;
        i++;
    }
    for(--i;i>=0;i--){
        printf("%c",bits[i]);
    }
    printf(",");
    //PARTE FRACIONÁRIA
    i=1;
    parte_int=dec;
    while(dec>0 && i<20){
        dec*=base;
        parte_int=dec;
        dec -= parte_int;
        if(parte_int>9){
            printf("%c",parte_int+7+'0');
        }
        else{
            printf("%d",parte_int);
        }
        i++;
    }
    free(bits);
}

void printa_matriz(int tam, double matriz[tam][tam+1]){
    int i=0, j=0;
    for(i=0;i<tam;i++){
        for(j=0;j<tam+1;j++){
            printf("%.4f ",matriz[i][j]);
        }
        printf("\n");
    }
}

int eh_diagonal(int tam, double matriz[tam][tam+1]){
    int i,j;
    for(i=0;i<tam;i++){
        for(j=0;j<tam;j++){
            if(matriz[i][j]!=0 && i!=j){
                return 0;
            }
        }
    }
    return 1;
}

void jordan(int tam, double matriz[tam][tam+1]){
    int i=0, j=0, k=0, pivo, aux, ordem[tam]; //ordem é um vetor para rastrear as variáveis Xi, caso haja troca de colunas
    double mults[tam]; //multiplicadores de cada linha
    double x[tam]; //os X's do sl
    //inicializando as variáveis
    for(i=0; i<tam; i++){
        ordem[i]=i;
        x[i]=0;
    }
    //pivotando a matriz
    for(i=0;i<tam;i++){
        pivo=matriz[i][i];
        if(pivo==0.0){
            j=i+1;
            while(j<tam && matriz[i][j]==0.0){
                j++;
            }
            if(j<tam){
                for(k=0;k<tam;k++){
                    aux = matriz[k][i];
                    matriz[k][i] = matriz[k][j];
                    matriz[k][j] = aux;
                    ordem[i]=j;
                    ordem[j]=i;
                    pivo=matriz[i][i];
                }
            }
            else{
                for(k=0;k<tam;k++){
                    x[i]=0;
                    matriz[k][i]=0;
                }
            }
        }
        if(pivo!=0.0){
            for(j=0;j<tam;j++){
                if(j!=i){
                    mults[j]=-1*(matriz[j][i]/pivo);
                    for(k=0;k<tam+1;k++){
                        matriz[j][k] += ( matriz[i][k] * mults[j] );
                        if(fabs(matriz[j][k])<=ZERO){
                            matriz[j][k]=0;
                        }
                    }
                }
            }
        }
    }
    //análise de soluções
    i=tam-1;
    while(i>=0){
        if(matriz[i][i]==0 && matriz[i][tam]!=0){
            printf("\nSistema incompativel");
            break;
        }
        else if(matriz[i][i]==0 && matriz[i][tam]==0){
            x[ ordem[i] ]=0;
        }
        else{
            x[ ordem[i] ] = matriz[i][tam];
            for(j=0;j<tam;j++){
                if(j!=i){
                    x[ ordem[i] ]-=(matriz[i][j] * x[ordem[j]]);
                }
            }
            x[ ordem[i] ] /= matriz[i][i];
        }
        printf("X%d = %f\n",ordem[i], x[ordem[i]] );
        i--;
    }
    if(eh_diagonal(tam, matriz)){
        printf("\nMatriz diagonal\n");
    }
    else{
        printf("\n Matriz nao diagonal\n");
    }
    printf("\n--------------------------------\n");
    printa_matriz(tam,matriz);
    printf("\n---------------------------------\n\n");
}

int ler_matriz(){
    FILE *arq;
    arq = fopen("sl.txt","r");
    if(arq==NULL){
        printf("Algo errado aconteceu!\n");
        return -1;
    }
    else{
        char aux;
        char *num=malloc(sizeof(char)*5);
        strcpy(num," ");
        int mult = 1;
        int i=0,j=0,cont=0;
        while(aux!='\n'){
            aux=fgetc(arq);
            num[cont]=aux;
            cont++;
        }
        cont=0;
        int tam = atoi(num);
        double matriz[tam][tam+1];
        strcpy(num," ");
        while(1){
            aux=fgetc(arq);
            if(aux=='-'){
                mult=-1;
            }
            else if(aux==' '){
                matriz[i][j]=atof(num)*mult;
                strcpy(num," ");
                mult=1;
                cont=0;
                j++;
            }
            else if(aux=='\n'){
                matriz[i][j]=atof(num)*mult;
                printf("\n");
                strcpy(num," ");
                mult=1;
                cont=0;
                i++;
                j=0;
            }
            else if(aux==EOF){
                matriz[i][j]=atof(num)*mult;
                break;
            }
            else{
                num[cont]=aux;
                cont++;
            }
        }
        jordan(tam,matriz);
    }
    fclose(arq);
}

void menu(){
    char op = ' ';
    float bin;
    while(op!='F'){
        printf("-----------EP 1 - CALCULO NUMERICO------------\n");
        printf("C - Conversao\n");
        printf("S - Sistema linear\n");
        printf("E - Equacao algebrica\n");
        printf("F - Finalizar\n");
        printf("Digite sua opcao: ");
        scanf("%c", &op);
        op = toupper(op);
        setbuf(stdin, NULL);
        switch(op){
            case 'C':
                system("cls");
                printf("Digite um numero decimal: ");
                scanf("%f",&bin);
                setbuf(stdin, NULL);
                printf("Binario: ");
                decimal_p_n(bin,2);
                printf("\n");
                printf("Octal: ");
                decimal_p_n(bin,8);
                printf("\n");
                printf("Hexadecimal: ");
                decimal_p_n(bin,16);
                printf("\n");
            break;
            case 'S':
                system("cls");
                ler_matriz();
            break;
            case 'F':
                system("cls");
                printf("Ate logo!\n");
            break;
            default:
                system("cls");
                printf("Opcao invalida!");
            break;
        }
    }
}

int main(){
    menu();
    return 0;
}
