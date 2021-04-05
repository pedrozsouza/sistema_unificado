#include <stdio.h>
#include <locale.h>

#define TOTAL_AULAS 36.0 // uma disciplina com 36 aulas
#define QTD_ALUNOS 50	//(a quantidade de alunos na disciplina deve ser fornecida pelo programador)
#define LIMPAR "cls"
#define ARQ "alunos.txt"


typedef struct{
	char nome[101];
	int matricula;
	float notas[4];
	int faltas;
} aluno;



void imprimir_dados(aluno a)
{
	printf("Nome: %s\n", a.nome);
	printf("Matr�cula: %i\n", a.matricula);
	printf("Nota 1: %f\n", a.notas[0]);
	printf("Nota 2: %f\n", a.notas[1]);
	printf("Nota 3: %f\n", a.notas[2]);
	printf("Nota 4: %f\n", a.notas[3]);
	printf("Faltas: %i\n", a.faltas);	
}

aluno pesquisar(char *txtarq, int matricula)
{
	FILE *arq = fopen(txtarq, "r");
	aluno tmp;
	while (!feof(arq))
	{
		fscanf(arq,"%s %i %f %f %f %f %i\n", tmp.nome, &tmp.matricula, &tmp.notas[0], &tmp.notas[1], &tmp.notas[2], &tmp.notas[3], &tmp.faltas);
		if (tmp.matricula==matricula)
		{
			break;
		}
		else
		{
			tmp.matricula = -1;
		}
	}
	fclose(arq);
	
	return tmp;
}

int contar_qtd_alunos(char *txtarq)
{
	FILE *arq = fopen(txtarq, "r");
	aluno tmp;
	int c = 0;
	while (!feof(arq))
	{
		fscanf(arq,"%s %i %f %f %f %f %i\n", tmp.nome, &tmp.matricula, &tmp.notas[0], &tmp.notas[1], &tmp.notas[2], &tmp.notas[3], &tmp.faltas);
		c++;
	}
	fclose(arq);
	return c;
}


void removerAlunoDoArquivo(char *txtarq, int matriculapesq)
{
	aluno tmp;
	FILE* arq = fopen(txtarq,"r"); // abrir arquivo em modo leitura
	FILE* arqNovo = fopen("temp.txt","w"); // abrir arquivo em modo escrita
	
	while(!feof(arq))
	{
		fscanf(arq,"%s %i %f %f %f %f %i\n", tmp.nome, &tmp.matricula, &tmp.notas[0], &tmp.notas[1], &tmp.notas[2], &tmp.notas[3], &tmp.faltas);
		if (tmp.matricula!=matriculapesq) // se a matricula lida for diferente da desejada para exluir, grava no arquivo novo
		{
			// escrever dados no arquivo novo
			fprintf(arqNovo,"%s %i %f %f %f %f %i\n", tmp.nome, tmp.matricula, tmp.notas[0], tmp.notas[1], tmp.notas[2], tmp.notas[3], tmp.faltas); 
		}
	}
	fclose(arq); // fechar o uso do arquivo
	fclose(arqNovo); // fechar o uso do arquivo

    // windows - redefinindo os arquivos
	system("del alunos.txt");
	system("rename temp.txt alunos.txt");
}


int menu()
{
    int resp;

    printf("1 - Cadastro de aluno\n");
    printf("2 - Consulta de informa��es de aluno\n");
    printf("3 - Atualiza��o de informa��es de aluno\n");
    printf("4 - Remo��o de aluno\n");
    printf("5 - Relat�rio\n");
    printf("6 - Sair\n");
    scanf("%i", &resp);

    getchar();  //limpar buffer

    return resp;
}



void cadastrar(char *txtarq)
{
	int i;
	aluno novo, pesq;
	FILE *arq = fopen(txtarq, "a+");
	
	if (contar_qtd_alunos(txtarq)>=QTD_ALUNOS)
	{
		printf("A quantidade m�xima de alunos (%i) foi atingida!\n\n",QTD_ALUNOS);
	}
	else
	{
	    printf("Informe os dados do registro para cadastro\n\n");
	   
	    printf("Matr�cula: ");
	    scanf("%i", &novo.matricula);
	    getchar();  //limpar buffer
	    
	    pesq = pesquisar(txtarq, novo.matricula);
	    
	    if (pesq.matricula!=-1)
	    {
	    	imprimir_dados(pesq);
	    	printf("\n\nEsta matr�cula j� existe cadastrada!\n\n");
		}
		else
		{
		    printf("Nome: ");
		    gets(novo.nome);
		    
			for(i=0;i<4;i++) // 4 notas
			{
				do
				{
					printf("Entre com a nota %i (m�nimo 0, m�ximo 10): ", i+1);
					scanf("%f", &novo.notas[i]);
				} while (novo.notas[i]<0 || novo.notas[i]>10);
			}
			
			do
			{
				printf("Entre com o n�mero de faltas (m�nimo 0, m�ximo: %.0f): ", TOTAL_AULAS);
				scanf("%i", &novo.faltas);
			} while (novo.faltas<0 || novo.faltas>36);
								
		    getchar();  //limpar buffer
		
		    if (arq!=NULL)
		    {
		    	fprintf(arq,"%s %i %.2f %.2f %.2f %.2f %i\n", novo.nome, novo.matricula, novo.notas[0], novo.notas[1], novo.notas[2], novo.notas[3], novo.faltas);
		        printf("Registro incluido com sucesso!\n\n");
		    }
		    else
		    {
		        printf("\nErro ao adicionar registo! E02\n\n");
		    }
		}
	}
	
    fclose(arq);
}


void consultar(char *txtarq)
{
	aluno pesq;
	int matriculapesq;
	
    printf("Consulta de cadastro\n\n");
   
    printf("Informe a matr�cula: ");
    scanf("%i", &matriculapesq);
    getchar();  //limpar buffer
    
    pesq = pesquisar(txtarq, matriculapesq);
    
	if(pesq.matricula!=-1)
	{
		printf("\n\n");
		imprimir_dados(pesq);
	}
	else
	{
		printf("Matr�cula n�o encontrada!\n\n");
	}
}


void atualizar(char *txtarq)
{
	aluno novo, pesq;
	int matriculapesq, i;
	FILE* arq;
	
    printf("Altera��o de cadastro\n\n");
   
    printf("Informe a matr�cula: ");
    scanf("%i", &matriculapesq);
    getchar();  //limpar buffer
	

	pesq = pesquisar(txtarq, matriculapesq);
	
	if(pesq.matricula!=-1)
	{
		printf("\n\n");
		imprimir_dados(pesq);
		
		printf("\n\nInforme os novos dados para altera��o\n\n");
	   
	    novo.matricula = pesq.matricula;

	    printf("Nome: ");
	    gets(novo.nome);
	    
		for(i=0;i<4;i++) // 4 notas
		{
			do
			{
				printf("Entre com a nota %i (m�nimo 0, m�ximo 10): ", i+1);
				scanf("%f", &novo.notas[i]);
			} while (novo.notas[i]<0 || novo.notas[i]>10);
		}
			
		do
		{
			printf("Entre com o n�mero de faltas (m�nimo 0, m�ximo: %.0f): ", TOTAL_AULAS);
			scanf("%i", &novo.faltas);
		} while (novo.faltas<0 || novo.faltas>36);
							
	    getchar();  //limpar buffer
	

		removerAlunoDoArquivo(txtarq, pesq.matricula);
	
		// incluir registro
        arq = fopen(txtarq, "a+");
    	fprintf(arq,"%s %i %.2f %.2f %.2f %.2f %i\n", novo.nome, novo.matricula, novo.notas[0], novo.notas[1], novo.notas[2], novo.notas[3], novo.faltas);
        printf("\n\nRegistro alterado com sucesso!\n\n");
        fclose(arq);
	        
	}
	else
	{
		printf("Matr�cula n�o encontrada!\n\n");
	}
	
}



void remover(char *txtarq)
{
	aluno pesq, tmp;
	int matriculapesq;
	FILE* arq;
	
    printf("Exclus�o de cadastro\n\n");
   
    printf("Informe a matr�cula: ");
    scanf("%i", &matriculapesq);
    getchar();  //limpar buffer
	
	pesq = pesquisar(txtarq, matriculapesq);
	
	if(pesq.matricula!=-1)
	{
		printf("\n\n");
		imprimir_dados(pesq);
	
		removerAlunoDoArquivo(txtarq, pesq.matricula);

        printf("\n\nRegistro removido com sucesso!\n\n");
	        
	}
	else
	{
		printf("Matr�cula n�o encontrada!\n\n");
	}
	
}


void listar(char *txtarq)
{
	FILE *arq = fopen(txtarq, "r");
	aluno tmp;
	float media, frequencia;
	char situacao[10];

	while (!feof(arq))
	{
		fscanf(arq,"%s %i %f %f %f %f %i\n", tmp.nome, &tmp.matricula, &tmp.notas[0], &tmp.notas[1], &tmp.notas[2], &tmp.notas[3], &tmp.faltas);
	
		media = (tmp.notas[0]+tmp.notas[1]+tmp.notas[2]+tmp.notas[3])/4;
		frequencia = 100 - (tmp.faltas/TOTAL_AULAS*100);
		
		strcpy(situacao, "REPROVADO");
		if (media>=7 && frequencia>=60)
			strcpy(situacao, "APROVADO");

		printf("Nome: %s\t\tMatr�cula: %i\nM�dia = (%2.2f + %2.2f + %2.2f + %2.2f) / 4 => %2.2f\nFaltas: %i\tFreq.: %2.2f\t\tSitua��o: %s\n\n", tmp.nome, tmp.matricula, tmp.notas[0], tmp.notas[1], tmp.notas[2], tmp.notas[3], media, tmp.faltas, frequencia, situacao);
	}
	fclose(arq);
	
	printf("\n\n");
}

int main()
{
	setlocale(LC_ALL,"");
	

	int resp = 0;

	while(resp!=6)
    {
        system(LIMPAR);
        resp = menu();

        switch(resp)
        {
            case 1:
                cadastrar(ARQ);
                break;
            case 2:
                consultar(ARQ);
                break;
            case 3:
                atualizar(ARQ);
                break;
            case 4:
                remover(ARQ);
                break;
            case 5:
                listar(ARQ);
                break;
            case 6:
                // n�o faz nada, pois sair� do loop
                break;
            default:
                printf("Op��o inv�lida! Tente novamente.\n");
                break;
        }

        if (resp!=6)
        {
            printf("Pressione <ENTER> para continuar\n");
            getchar();
        }
    }

	return 0;	
}
