#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#define ZERO 0.00000000001

/*Função que recebe um número decimal
 *e o converte para uma base n*/
int decimal_p_n(double dec, int base){
    int resto = 0, i=0, parte_int = dec;
    dec-=parte_int;
    char *bits = malloc((sizeof(dec)*8)+1);
    if(bits==NULL){
        printf("\nErro! Tente Novamente.\n");
        return -1;
    }
    /*PARTE INTEIRA*/
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

    /*PARTE FRACIONÁRIA*/
    if(dec>0){
        printf(",");
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
    }
    free(bits);
}

/*Função simples, que printa uma matriz passada por
 *parâmetro.*/
void printa_matriz(int tam, double matriz[tam][tam+1]){
    int i=0, j=0;
    for(i=0;i<tam;i++){
        for(j=0;j<tam+1;j++){
            printf("%.4f ",matriz[i][j]);
        }
        printf("\n");
    }
}

/*Função que percorre toda a matriz e retorna 0
 *para indicar que ela é diagonal ou retorna 1
 *para inidicar que ela não é diagonal. */
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

/*Método usado para pivotar a matriz que foi lida
 *na função ler_matriz e retornar uma matriz diagonal
 *juntamente com a solução do SL, caso exista.*/
void jordan(int tam, double matriz[tam][tam+1]){
    int i=0, j=0, k=0, pivo, aux, ordem[tam]; //ordem é um vetor para rastrear as variáveis Xi, caso haja troca de colunas
    double mults[tam]; //multiplicadores de cada linha
    double x[tam]; //os X's do sl
    //inicializando as variáveis
    for(i=0; i<tam; i++){
        ordem[i]=i;
        x[i]=0;
    }
    /*pivotando a matriz*/
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
    /*Análise de soluções*/
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

/*Função responsável por ler o arquivo de texto sl.txt
 *que deve estar no mesmo diretório que o arquivo ep1.c,
 *devidamente preenchido com o número de variáveis
 *e os coeficientes correspondente ao sl.*/
int ler_matriz(){
    FILE *arq;
    arq = fopen("sl.txt","r");
    if(arq==NULL){
        printf("Certifique-se de que ha um arquivo chamado sl.txt \n e que ele esta no diretorio do projeto!\n");
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

/*Estrutura utilizada para representar uma
 *equação algébrica, onde Px, um vetor de inteiros,
 *corresponde ao polinômio principal, digitado pelo
 *usuário, P1x, P2x e P3x, também vetores de inteiros,
 *correspondem ao polinômios utilizados no Teorema de Lagrange
 *e o tamanho corresponde ao tamanho dos vetores Px, P1x, P2x,
 *e P3x e é equivalente ao grau do polinômio Px incrementado
 *em uma unidade.*/
struct EquacaoAlgebrica
{
	int* Px;
	int* P1x;
	int* P2x;
	int* P3x;
	int tamanho;
};

typedef struct EquacaoAlgebrica EquacaoAlgebrica;

/*Imprime o conteúdo dos vetores Px, P1x, P2x e P3x.*/
void PrintE(EquacaoAlgebrica Equacao)
{
	int i;
	printf("Coeficientes de P(x): ");
	for(i=0;i<Equacao.tamanho;i++)
	{
		printf("%d ",Equacao.Px[i]);
	}
	printf("\nCoeficientes de P1(x): ");
	for(i=0;i<Equacao.tamanho;i++)
	{
		printf("%d ",Equacao.P1x[i]);
	}
	printf("\nCoeficientes de P2(x): ");
	for(i=0;i<Equacao.tamanho;i++)
	{
		printf("%d ",Equacao.P2x[i]);
	}
	printf("\nCoeficientes de P3(x): ");
	for(i=0;i<Equacao.tamanho;i++)
	{
		printf("%d ",Equacao.P3x[i]);
	}
	printf("\n");
}

/*Ajusta os vetores P1x, P2x e P3x caso o An seja negativo
 *afim de obter um polinômio com as mesmas raízes e com An positivo.*/
void AjustaEquacao(EquacaoAlgebrica* Equacao)
{
	int i;
	if(Equacao->P1x[0]<0)
	{
		for(i=0;i<Equacao->tamanho;i++)
		{
			Equacao->P1x[i]=Equacao->P1x[i]*-1;
		}
	}
	if(Equacao->P2x[0]<0)
	{
		for(i=0;i<Equacao->tamanho;i++)
		{
			Equacao->P2x[i]=Equacao->P2x[i]*-1;
		}
	}
	if(Equacao->P3x[0]<0)
	{
		for(i=0;i<Equacao->tamanho;i++)
		{
			Equacao->P3x[i]=Equacao->P3x[i]*-1;
		}
	}
}

/*Configura uma equação algebrica de grau n e aloca
 *os vetores Px, P1x, P2x e P3x na memória com tamanho grau+1.
 *Caso ocorra erro na alocação retorna uma variável do tipo
 *EquacaoAlgebrica, cujo os vetores serão devidamente desalocados
 *fora da função ConfigurarEquacao.
 *Caso nenhum erro ocorra na alocação,o vetor Px é escrito pelo
 *usuário e os vetores P1x, P2x e P3x são configurados a partir
 *do vetor Px digitado pelo usuário.
 *No final uma variável do tipo EquacaoAlgebrica é retornada
 *contendo os vetores devidamente preenchidos.*/
EquacaoAlgebrica ConfigurarEquacao(int grau)
{
	int i=0;
	EquacaoAlgebrica Equacao;
	Equacao.tamanho=grau+1;
	Equacao.Px=malloc(sizeof(int)*(Equacao.tamanho));
	Equacao.P1x=malloc(sizeof(int)*(Equacao.tamanho));
	Equacao.P2x=malloc(sizeof(int)*(Equacao.tamanho));
	Equacao.P3x=malloc(sizeof(int)*(Equacao.tamanho));
	if(Equacao.Px==NULL || Equacao.P1x==NULL || Equacao.P2x==NULL || Equacao.P3x==NULL)
	{
		printf("Erro na aloca%c%co de mem%cria!\n",135,198,162);
		return Equacao;
	}
	for(i=0;i<Equacao.tamanho;i++)
	{
	    setbuf(stdin,NULL);
        printf("a%d: ",Equacao.tamanho-1-i);
        scanf("%d",&Equacao.Px[i]);
	    if(i==0&&Equacao.Px[i]<=0){
            printf("\na%d deve ser maior que 0!\n",Equacao.tamanho-1-i);
            i--;
	    }
	    else if(i==Equacao.tamanho-1&&Equacao.Px[i]==0){
            printf("\na%d deve maior que 0!\n",Equacao.tamanho-1-i);
            i--;
	    }
	}
	for(i=0;i<Equacao.tamanho;i++)
	{
		Equacao.P1x[i]=Equacao.Px[grau-i];
		if(i%2!=0)
		{
			Equacao.P2x[i]=-1*Equacao.Px[i];
			Equacao.P3x[i]=-1*Equacao.P1x[i];
		}
		else
		{
			Equacao.P2x[i]=Equacao.Px[i];
			Equacao.P3x[i]=Equacao.P1x[i];
		}
	}
	AjustaEquacao(&Equacao);
	return Equacao;
}

/*Calcula o limite superior de um determinando polinômio de tamanho
 *tamanho. Esta função se baseia no Teorema de Lagrange, onde
 *L=1+(N-K)√(B/An).
 *No caso desta função, Resultado=1+(N-K)√(B/An).
 *Na função An corresponde ao coeficiente do termo de maior índice
 *do polinômio. B corresponde ao coeficiente negativo, em módulo,
 *cujo o índice do seu termo é maior que os dos restantes coeficientes
 *negativos. K corresponde ao índice do termo cujo seu coeficiente
 *é negativo e é menor dentre todos os coeficientes negativos.
 *N corresponde ao grau do polinômio que é equivalente ao
 *tamanho-1.
 *No final a função retorna o Resultado.*/
double CalculaLimite(int* polinomio, int tamanho)
{
	double Resultado,MenorCoeficienteNegativo=0,An=0,B=0,K=0,N=0;
	int i=0;
	An=(double) polinomio[0];
	N=(double) tamanho-1;
	while(polinomio[i]>=0 && i<tamanho)
	{
		i++;
	}
	K=(double) tamanho-i-1;
	for(i=0;i<tamanho;i++)
	{
		if(polinomio[i]<MenorCoeficienteNegativo)
		{
			MenorCoeficienteNegativo=(double) polinomio[i];
		}
	}
	B=-1*MenorCoeficienteNegativo;
	printf("\nN: %lf An: %lf B: %lf K: %lf ",N,An,B,K);
	Resultado=1.0+pow(B/An,1/(N-K));
	printf("Resultado: %lf\n",Resultado);
	return Resultado;
}

/*Verifica se o vetor Px da Equacao possui ao menos um
 *coeficiente negativo.
 *Caso possuir retorna 1, caso contrário retorna 0.*/
int CheckEquacao(EquacaoAlgebrica Equacao)
{
	int i;
	for(i=0;i<Equacao.tamanho;i++)
	{
		if(Equacao.Px[i]<0)
		{
			return 1;
		}
	}
	return 0;
}

/*Calcula os limites dos intervalos das raízes
 *reais positivas e reais negativas de uma euação
 *algébricas através do Teorema de Lagrange, caso
 *seja possível usá-lo.*/
void TeoremaLagrange(EquacaoAlgebrica Equacao)
{
	double L,L1,L2,L3;
	if(CheckEquacao(Equacao)==1)
	{
		L=CalculaLimite(Equacao.Px,Equacao.tamanho);
		L1=CalculaLimite(Equacao.P1x,Equacao.tamanho);
		L2=CalculaLimite(Equacao.P2x,Equacao.tamanho);
		L3=CalculaLimite(Equacao.P3x,Equacao.tamanho);
		printf("Intervalo das Ra%czes Reais Positivas: %.4lf<=x<=%.4lf\n",161,1/L1,L);
		printf("Intervalo das Ra%czes Reais Negativas: %.4lf<=x<=%.4lf\n",161,-1*L2,-1*(1/L3));
	}
	else
	{
		printf("O polin%cmio n%co possui coeficientes negativos, portanto o Teorema de Lagrange n%co pode ser usado.\n",147,198,198);
	}
}

/*Calcula a imagem de um polinômio com um inteiro valor.*/
double f(int* polinomio, int tamanho, double valor)
{
	int i;
	double resultado=0.0;
	for(i=0;i<tamanho;i++)
	{
		resultado+=(double) polinomio[i]*pow(valor,tamanho-1-i);
	}
	return resultado;
}

/*Realiza o Método da Bisseção numa equação
 *algébrica.
 *Primeiramente é pedido ao usuário que informe
 *um intervalo [a,b] que será utilizado para
 *realizar o método. */
void MetodoBissecao(EquacaoAlgebrica Equacao)
{
	double LimInf, LimSup, fa, fb, Erro=1.0;
	printf("\nM%ctodo da Bisse%c%co\n", 130, 135, 198);
	printf("Defina um intervalo [a,b]\n");
	do
	{
	    setbuf(stdin, NULL);
		printf("Digite o limite inferior do intervalo(a): ");
		scanf("%lf",&LimInf);
		setbuf(stdin, NULL);
		printf("Digite o limite superior do intervalo(b): ");
		scanf("%lf",&LimSup);
		setbuf(stdin, NULL);
		if(LimInf>LimSup)
		{
			printf("Intervalo Inv%clido! Digite novamente...\n",160);
		}
	}while(LimInf>LimSup);
	fa=f(Equacao.Px,Equacao.tamanho,LimInf);
	fb=f(Equacao.Px,Equacao.tamanho,LimSup);
	if((fa*fb)<0)
	{
		int i=0;
		double ErroMaximo=0.0, a=LimInf, b=LimSup, m=0.0, fm=1.0;
		ErroMaximo=pow(10,-8);
		while(i<1000 && Erro>ErroMaximo )//&& fm!=0)
		{
			Erro=(b-a)/2;
			m=(b+a)/2;
			fa=f(Equacao.Px,Equacao.tamanho,a);
			fb=f(Equacao.Px,Equacao.tamanho,b);
			fm=f(Equacao.Px,Equacao.tamanho,m);
			printf("i: %d; a: %.8f; b: %.8f; m: %.8f; fa: %.8f; fb: %.8f; fm: %.8f; Erro: %.8f;\n",i,a,b,m,fa,fb,fm,Erro);
			if((fa*fm)<0.0)
			{
				b=m;
			}
			else if((fb*fm)<0)
			{
				a=m;
			}
			i++;
		}
		printf("Aproxima%c%co da ra%cz contida no intervalo [%.4f,%.4f]: %.4f\n",135,198,161,LimInf,LimSup,m);
	}
	else
	{
		printf("O n%cmero de ra%czes no intervalo [%.4f,%.4f] %c par\n",163,161,LimInf,LimSup,130);
	}
}

/*Chama todas as funções para configurar uma equação algébrica,
 *achar os intervalos de suas raizes reais positivas e
 *reais negativas através do Teorema de Lagrange e realiza
 *o Método da Bisseção num determinado intervalo fornecido
 *pelo usuário.*/
void EquacaoAlg()
{
	int grau;
	EquacaoAlgebrica Equacao;
	printf("Digite o grau da Equa%c%co Alg%cbrica: ",135,198,130);
	scanf("%d",&grau);
	Equacao=ConfigurarEquacao(grau);
	PrintE(Equacao);
	TeoremaLagrange(Equacao);
	MetodoBissecao(Equacao);
}

/*Menu responsável por fazer a interação
 *entre o usuário e todos os algoritmos disponíveis.*/
void menu(){
    char op = ' ';
    int grau,i;
    double num,a,b;
    while(op!='F'){
        printf("-----------EP 1 - C%cLCULO NUM%cRICO------------\n",181,144);
        printf("C - Convers%co\n",198);
        printf("S - Sistema Linear\n");
        printf("E - Equa%c%co Alg%cbrica\n",135,198,130);
        printf("F - Finalizar\n");
        printf("Digite sua op%c%co: ",135,198);
        scanf("%c", &op);
        op = toupper(op);
        setbuf(stdin, NULL);
        switch(op){
            case 'C':
                do{
                    system("clear");
                    printf("Digite um n%cmero decimal positivo: ",163);
                    scanf("%lf",&num);
                    setbuf(stdin, NULL);
                }while(num<0);
                printf("Bin%crio: ",160);
                decimal_p_n(num,2);
                printf("\n");
                printf("Octal: ");
                decimal_p_n(num,8);
                printf("\n");
                printf("Hexadecimal: ");
                decimal_p_n(num,16);
                printf("\n");
            break;
            case 'S':
                system("clear");
                ler_matriz();
            break;

            case 'E':
                system("clear");
                EquacaoAlg();
            break;

            case 'F':
                system("clear");
                printf("At%c logo!\n",130);
            break;
            default:
                system("clear");
                printf("Op%c%co inv%clida!\n",135,198,160);
            break;
        }
    }
}

int main(){
    menu();
    return 0;
}
