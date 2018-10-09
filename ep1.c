#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

double decimal_p_bin(double dec){
    double bin=0.0;
    int parte_int = dec;
    dec-=parte_int;
    int resto = 0;
    int bits[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int i = 0;
    //PARTE INTEIRA
    while(parte_int >= 1){
        resto = parte_int%2;
        bits[i] = resto;
        parte_int = parte_int/2;
        i++;
    }
    for(i=14;i>=0;i--){
        bin = bin + bits[i]*pow(10,i);
        //printf("%d * 10**%d = %f\n",bits[i],i,bin);
    }
    //PARTE FRACIONÁRIA
    i=-1;
    parte_int=dec;
    while(dec>0 && i>-20){
        dec*=2;
        parte_int=dec;
        dec -= parte_int;
        bin += parte_int*pow(10,i);
        i--;
    }
    return bin;
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
                printf("%f em bin: %.20f\n",bin,decimal_p_bin(bin));
            break;
            default:
                system("cls");
                printf("Opcao invalida!");
        }
    }
}

int main(){
    //float bin = decimal_p_bin(221.0);
    menu();
    //printf("\n %f",bin);
    return 0;
}
