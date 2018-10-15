#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>


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
        int matriz[tam][tam+1];
        strcpy(num," ");
        while(1){
            aux=fgetc(arq);
            if(aux=='-'){
                mult=-1;
            }
            else if(aux==' '){
                matriz[i][j]=atoi(num)*mult;
                strcpy(num," ");
                mult=1;
                cont=0;
                j++;
            }
            else if(aux=='\n'){
                matriz[i][j]=atoi(num)*mult;
                printf("\n");
                strcpy(num," ");
                mult=1;
                cont=0;
                i++;
                j=0;
            }
            else if(aux==EOF){
                matriz[i][j]=atoi(num)*mult;
                break;
            }
            else{
                num[cont]=aux;
                cont++;
            }
        }
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
                printf("Digite um numero: ");
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
