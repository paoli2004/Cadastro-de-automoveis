#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


struct Marca
{
    char nomeMarca[20];
    char paisOrigem[20];
};

struct Data
{
    int dia;
    int mes;
    int ano;
};

struct Automovel
{
    char nomeModelo[20];
    int anoFabricacao;
    char cor[20];
    float valorEstimado;
};

typedef struct
{
    struct Marca marca;
    struct Data data;
    struct Automovel automovel;
} CadastroAutomovel;


void inserirAutomovel(CadastroAutomovel **cadastros, int *count);
void removerAutomovel(CadastroAutomovel **cadastros, int *count);
void exibirCadastros(CadastroAutomovel *cadastros, int count);
void consultarAutomoveis(CadastroAutomovel *cadastros, int count);
void remove_linha(char str[]);
int contarCadastros(FILE *file, CadastroAutomovel **cadastros);
int inserirDadosArquivo(FILE *file, CadastroAutomovel *cadastros, int count);
void botao_menu();
int validarModelo(char nomeModelo[], CadastroAutomovel **cadastros, int *count);


int main()
{
    setlocale(LC_ALL, "Portuguese");
    int repetir = 1;
    FILE *arquivoCadastros = fopen("cadastros.txt", "a+");

    CadastroAutomovel *cadastros = NULL;
    int count = 0;

    if (arquivoCadastros != NULL)
    {
        count = contarCadastros(arquivoCadastros, &cadastros);
        fclose(arquivoCadastros);
    }

    while (repetir)
    {
        int input;
        printf("-----Bem-Vindo ao Cadastro de Automóveis-----\n");
        printf("[1] Inserir Automóvel\n");
        printf("[2] Remover Automóvel\n");
        printf("[3] Exibir Cadastros\n");
        printf("[4] Consultar Automóveis\n");
        printf("[0] Sair do programa.\n");
        printf("O que deseja realizar?\n");
        scanf("%d", &input);
        getchar(); 

        switch (input)
        {
        case 1:
            inserirAutomovel(&cadastros, &count);
            break;
        case 2:
            removerAutomovel(&cadastros, &count);
            break;
        case 3:
            exibirCadastros(cadastros, count);
            break;
        case 4:
            consultarAutomoveis(cadastros, count);
            break;
        case 0:
            arquivoCadastros = fopen("cadastros.txt", "w");  inserirDadosArquivo(arquivoCadastros, cadastros, count);
            fclose(arquivoCadastros);
            repetir = 0;
            break;
        default:
            printf("Opção inválida, tente novamente.\n");
        }
    }

    printf("Programa Encerrado.\n");
    free(cadastros); 
    return 0;
}

// FUNÇÕES

void inserirAutomovel(CadastroAutomovel **cadastros, int *count)
{
    printf("Insira o nome do modelo: ");
    char teste[20];
    fgets(teste, 20, stdin);
    remove_linha(teste);

    
    for (int i = 0; i < *count; i++)
    {
        if (strcmp((*cadastros)[i].automovel.nomeModelo, teste) == 0)
        {
            printf("Modelo já cadastrado, erro no cadastro.\n");
            botao_menu();
            return;
        }
    }

    
    CadastroAutomovel *novoCadastros = realloc(*cadastros, (*count + 1) * sizeof(CadastroAutomovel));
    if (novoCadastros == NULL)
    {
        printf("Erro ao alocar memória.\n");
        botao_menu();
        return;
    }
    *cadastros = novoCadastros;

    CadastroAutomovel *novoCadastro = &(*cadastros)[*count];
    int correto = 0;

    strcpy(novoCadastro->automovel.nomeModelo, teste);

    printf("Insira o nome da marca: ");
    fgets(novoCadastro->marca.nomeMarca, 20, stdin);
    remove_linha(novoCadastro->marca.nomeMarca);

    printf("Insira o país de origem da marca: ");
    fgets(novoCadastro->marca.paisOrigem, 20, stdin);
    remove_linha(novoCadastro->marca.paisOrigem);

    correto = 0;
    while (correto == 0)
    {
        printf("Insira o dia atual: ");
        scanf("%d", &novoCadastro->data.dia);
        if (novoCadastro->data.dia > 0 && novoCadastro->data.dia <= 31)
            correto = 1;
    }

    correto = 0;
    while (correto == 0)
    {
        printf("Insira o mês atual: ");
        scanf("%d", &novoCadastro->data.mes);
        if (novoCadastro->data.mes > 0 && novoCadastro->data.mes <= 12)
            correto = 1;
    }

    correto = 0;
    while (correto == 0)
    {
        printf("Insira o ano atual: ");
        scanf("%d", &novoCadastro->data.ano);
        if (novoCadastro->data.ano > 0)
            correto = 1;
    }

    correto = 0;
    while (correto == 0)
    {
        printf("Insira o ano de fabricação: ");
        scanf("%d", &novoCadastro->automovel.anoFabricacao);
        getchar();
        if (novoCadastro->automovel.anoFabricacao > 0)
            correto = 1;
    }

    printf("Insira a cor do automóvel: ");
    fgets(novoCadastro->automovel.cor, 20, stdin);
    remove_linha(novoCadastro->automovel.cor);

    correto = 0;
    while (correto == 0)
    {
        printf("Insira o valor estimado: ");
        scanf("%f", &novoCadastro->automovel.valorEstimado);
        getchar();
        if (novoCadastro->automovel.valorEstimado > 0)
            correto = 1;
    }

    (*count)++;
    printf("Automóvel cadastrado com sucesso!\n");
    botao_menu();
}


void removerAutomovel(CadastroAutomovel **cadastros, int *count)
{
    if (*count == 0)
    {
        printf("Nenhum cadastro disponível para remover.\n");
        botao_menu();
        return;
    }

    char chave[20];
    printf("Digite o modelo do carro que deseja remover: ");
    fgets(chave, 20, stdin);
    remove_linha(chave);

    int pos = -1;
    for (int i = 0; i < *count; i++)
    {
        if (strcmp((*cadastros)[i].automovel.nomeModelo, chave) == 0)
        {
            pos = i;
            break;
        }
    }

    if (pos == -1)
    {
        printf("Modelo não encontrado.\n");
        botao_menu();
        return;
    }

    for (int i = pos; i < *count - 1; i++)
    {
        (*cadastros)[i] = (*cadastros)[i + 1];
    }

    CadastroAutomovel *novoCadastros = realloc(*cadastros, (*count - 1) * sizeof(CadastroAutomovel));
    if (novoCadastros != NULL || *count - 1 == 0)
    {
        *cadastros = novoCadastros;
    }

    (*count)--; 
    printf("Automóvel removido com sucesso!\n");
    botao_menu();
}

void exibirCadastros(CadastroAutomovel *cadastros, int count)
{
    if (count == 0)
    {
        printf("Nenhum cadastro disponível.\n");
        botao_menu();
        return;
    }
    
    int modoVizualizacao = 0;
    while(modoVizualizacao != 1 && modoVizualizacao != 2){
        printf("Deseja mostrar no console[1] ou em arquivo [2]?");
        scanf("%d", &modoVizualizacao);
    }

    if(modoVizualizacao == 1){
        printf("\n%-20s %-20s %-20s %-10s %-10s %-15s %-15s\n",
                "Modelo", "Marca", "País de Origem", "Ano", "Cor", "Valor Estimado", "Data");
        printf("--------------------------------------------------------------------------------------\n");

        for (int i = 0; i < count; i++)
        {
            printf("%-20s %-20s %-20s %-10d %-10s R$%-15.2f %-02d/%02d/%04d\n",
                cadastros[i].automovel.nomeModelo,
                cadastros[i].marca.nomeMarca,
                cadastros[i].marca.paisOrigem,
                cadastros[i].automovel.anoFabricacao,
                cadastros[i].automovel.cor,
                cadastros[i].automovel.valorEstimado,
                cadastros[i].data.dia,
                cadastros[i].data.mes,
                cadastros[i].data.ano);
        }
        botao_menu();
    } else if(modoVizualizacao == 2){

        FILE *vizualizacao = fopen("vizualizacao.txt", "w");

        fprintf( vizualizacao, "\n%-20s %-20s %-20s %-10s %-10s %-15s %-15s\n",
                "Modelo", "Marca", "País de Origem", "Ano", "Cor", "Valor Estimado", "Data");
        fprintf(vizualizacao, "--------------------------------------------------------------------------------------\n");

        for (int i = 0; i < count; i++)
        {
            fprintf(vizualizacao, "%-20s %-20s %-20s %-10d %-10s R$%-15.2f %-02d/%02d/%04d\n",
                cadastros[i].automovel.nomeModelo,
                cadastros[i].marca.nomeMarca,
                cadastros[i].marca.paisOrigem,
                cadastros[i].automovel.anoFabricacao,
                cadastros[i].automovel.cor,
                cadastros[i].automovel.valorEstimado,
                cadastros[i].data.dia,
                cadastros[i].data.mes,
                cadastros[i].data.ano);
        }

        fclose(vizualizacao);
        botao_menu();
    }

    
}

void consultarAutomoveis(CadastroAutomovel *cadastros, int count)
{
    if (count == 0)
    {
        printf("Nenhum cadastro disponível para consulta.\n");
        botao_menu();
        return;
    }

    char chave[20];
    printf("Digite o modelo do carro que deseja consultar: ");
    fgets(chave, 20, stdin);
    remove_linha(chave);

    int encontrado = 0;
    for (int i = 0; i < count; i++)
    {
        if (strcmp(cadastros[i].automovel.nomeModelo, chave) == 0)
        {
            encontrado = 1;
            printf("Modelo: %s | Marca: %s | País: %s | Ano: %d | Cor: %s | Valor: R$%.2f | Data: %02d/%02d/%04d\n",
                   cadastros[i].automovel.nomeModelo,
                   cadastros[i].marca.nomeMarca,
                   cadastros[i].marca.paisOrigem,
                   cadastros[i].automovel.anoFabricacao,
                   cadastros[i].automovel.cor,
                   cadastros[i].automovel.valorEstimado,
                   cadastros[i].data.dia,
                   cadastros[i].data.mes,
                   cadastros[i].data.ano);
        }
    }

    if (!encontrado)
    {
        printf("Modelo não encontrado.\n");
    }
    botao_menu();
}

void remove_linha(char str[])
{
    str[strcspn(str, "\n")] = '\0';
}

int validarModelo(char nomeModelo[], CadastroAutomovel **cadastros, int *count){

    for(int i = 0; i < *count; i++){
        if(strcmp(*cadastros[*count]->automovel.nomeModelo, nomeModelo)){
            return 0;
        }
    }
    
    return 1;
}

int contarCadastros(FILE *file, CadastroAutomovel **cadastros)
{
    int count = 0;
    CadastroAutomovel *temp = malloc(sizeof(CadastroAutomovel) * 10);
    if (!temp)
    {
        printf("Erro ao alocar memória.\n");
        botao_menu();
        return 0;
    }

    while (fscanf(file, "%s %s %d %d %d %s %d %s %f",
                  temp[count].marca.nomeMarca,
                  temp[count].marca.paisOrigem,
                  &temp[count].data.dia,
                  &temp[count].data.mes,
                  &temp[count].data.ano,
                  temp[count].automovel.nomeModelo,
                  &temp[count].automovel.anoFabricacao,
                  temp[count].automovel.cor,
                  &temp[count].automovel.valorEstimado) == 9)
    {
        count++;
        if (count % 10 == 0)
        {
            temp = realloc(temp, sizeof(CadastroAutomovel) * (count + 10));
            if (!temp)
            {
                printf("Erro ao alocar memória.\n");
                botao_menu();
                return count;
            }
        }
    }

    *cadastros = temp;
    return count;
}

int inserirDadosArquivo(FILE *file, CadastroAutomovel *cadastros, int count) {
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n", cadastros[i].marca.nomeMarca);
        fprintf(file, "%s\n", cadastros[i].marca.paisOrigem);
        fprintf(file, "%d\n", cadastros[i].data.dia);
        fprintf(file, "%d\n", cadastros[i].data.mes);
        fprintf(file, "%d\n", cadastros[i].data.ano);
        fprintf(file, "%s\n", cadastros[i].automovel.nomeModelo);
        fprintf(file, "%d\n", cadastros[i].automovel.anoFabricacao);
        fprintf(file, "%s\n", cadastros[i].automovel.cor);
        fprintf(file, "%f\n", cadastros[i].automovel.valorEstimado);
    }
    return 0;
}

void botao_menu()
{
    int bot = 1;
    while (bot)
    {
        printf("Digite 0 para voltar ao menu\n");
        scanf("%i", &bot);
    }
}
