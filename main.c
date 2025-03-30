/*Beatriz Di Palma Carvalho, RA: 10439477
Luiza Marinho de Mesquita, RA: 10438045
Marina Cantarelli Barroca, RA: 10740412
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1000    
// Define o número máximo de alunos que podem ser armazenados no nosso array

struct estudantes{     // armazena informações dos estudantes
    int semestre;
    char turma;
    char periodo;
    char nome[200];
    char disciplina[200];
    float media;
};

// Função para carregar dados de um arquivo CSV para o array dos estudantes
int carregar_csv(const char *nomeArquivo, estudantes alunos[]) {
    FILE *arquivo = fopen(nomeArquivo, "r");    // Abre um arquivo de leitura
    
    if (!arquivo) {     // Verifica se o arquivo foi aberto sem erros
        printf("Erro ao abrir o arquivo\n");
        return 0;
    }
    
    int num_alunos = 0;
    char linha[MAX];     // Buffer que armazena cada linha por linha do arquivo
    
    while (fgets(linha, sizeof(linha), arquivo)) {
        estudantes aluno;
        
        // Lê os valores do CSV e o armazena na estrutura
        if (sscanf(linha, "%d,%c,%c,%199[^,],%199[^,],%f", //le um int, caractere, 199 caracteres ate virgula e float
                   &aluno.semestre, &aluno.turma, &aluno.periodo, 
                   aluno.nome, aluno.disciplina, &aluno.media) == 6) {
            alunos[num_alunos++] = aluno;    // Adiciona o aluno ao array
        }
    }
    fclose(arquivo);    // Fecha o arquivo
    return num_alunos;  // Retorna o número de alunos lidos
}

// Função que salva os dados dos alunos em um arquivo CSV com o que foi pedido
void salvar_csv(const char *nomeArquivo, estudantes alunos[], int num_alunos, int opcao) { 
    FILE *arquivo = fopen(nomeArquivo, "w");    // Abre um arquivo de escrita
    
    if (!arquivo) {        
        printf("Erro ao criar o arquivo de saída: %s\n", nomeArquivo);        
        exit(1);
    }

    // Escreve os dados dos alunos no arquivo CSV dependendo da opção que foi selecionada
    for (int i = 0; i < num_alunos; i++) {
        switch (opcao) {
            case 1: // Ordenar por nome
                fprintf(arquivo, "%s,%d,%c,%c,%s,%.2f\n", alunos[i].nome, alunos[i].semestre, alunos[i].turma, alunos[i].periodo, alunos[i].disciplina, alunos[i].media);  
                break;
            
            case 2: // Ordenar por semestre
                fprintf(arquivo, "%s,%d,%c,%c,%s,%.2f\n", alunos[i].nome, alunos[i].semestre, alunos[i].turma, alunos[i].periodo, alunos[i].disciplina, alunos[i].media); 
                break;
            
            case 3: // Ordenar por semestre, turma, período, disciplina e nome
                fprintf(arquivo, "%d,%c,%c,%s,%s\n", alunos[i].semestre, alunos[i].turma, alunos[i].periodo, alunos[i].nome, alunos[i].disciplina);  
                break;
            
            case 4: // Ordenar por disciplina e média final
                fprintf(arquivo, "%s,%.2f,%s\n", alunos[i].disciplina, alunos[i].media, alunos[i].nome);  
                break;
            
            case 5: // Ordenar por período, semestre, turma, disciplina e nome
                fprintf(arquivo, "%c,%d,%c,%s,%s\n", alunos[i].periodo, alunos[i].semestre, alunos[i].turma, alunos[i].nome, alunos[i].disciplina); 
                break;
            
            default: // else..
                printf("Opção inválida para salvar!\n");
                break;
        }
    }   
    fclose(arquivo);
    printf("Arquivo '%s' gerado com sucesso!\n", nomeArquivo);
}

// Função que troca dois elementos no array para que auxilia a ordenação (bubble sort)
void troca(estudantes *a, estudantes *b) {
    estudantes temp = *a;
    *a = *b;
    *b = temp;
}

// Implementação do algoritmo Bubble Sort para que ocorra a ordenação dos alunos
void bubble_sort(estudantes alunos[], int num_alunos, int (*compara)(estudantes, estudantes)) {
    for (int i = 0; i < num_alunos - 1; i++) {
        for (int j = 0; j < num_alunos - i - 1; j++) {
            if (compara(alunos[j], alunos[j + 1]) > 0) {
                troca(&alunos[j], &alunos[j + 1]);
            }
        }
    }
}

// Funções de comparação para os diferentes critérios de ordenação 
int compara_nome(estudantes a, estudantes b) 
{
    for (int i = 0; a.nome[i] != '\0' && b.nome[i] != '\0'; i++) {
        if (a.nome[i] != b.nome[i]) {
            return a.nome[i] - b.nome[i];
        }
    }
    return 0;
}

int compara_semestre(estudantes a, estudantes b) 
{
    return a.semestre - b.semestre;
}

int compara_item3(estudantes a, estudantes b) {
    if (a.semestre != b.semestre) return a.semestre - b.semestre;
    if (a.turma != b.turma) return a.turma - b.turma;
    if (a.periodo != b.periodo) return a.periodo - b.periodo;
    if (strcmp(a.disciplina, b.disciplina) != 0) return strcmp(a.disciplina, b.disciplina);
    return strcmp(a.nome, b.nome);
}

// Função de comparação para ordenação DECRESCENTE por media final e disciplina]

int compara_disciplina_media(estudantes a, estudantes b) {
    int compara = strcmp(b.disciplina, a.disciplina);
    
    if (compara == 0) {
        if (a.media < b.media) return 1;     
        if (a.media > b.media) return -1;    
        return 0;
    }
    return compara;
}

int compara_item5(estudantes a, estudantes b) {
    if (a.periodo != b.periodo) return a.periodo - b.periodo;
    if (a.semestre != b.semestre) return a.semestre - b.semestre;
    if (a.turma != b.turma) return a.turma - b.turma;
    int disciplina_cmp = strcmp(a.disciplina, b.disciplina);
    if (disciplina_cmp != 0) return disciplina_cmp;
    return strcmp(a.nome, b.nome);
}

// Função que exibe o menu e executa a opção escolhida pelo usuário
void menu(estudantes alunos[], int num_alunos){
    int opcao;
    printf("\nMENU: \n");
    printf("1. Ordenar por nome\n");
    printf("2. Ordenar por semestre\n");
    printf("3. Ordenar por semestre, turma, período, disciplina e nome\n");
    printf("4. Ordenar por disciplina e média final (decrescente)\n");
    printf("5. Ordenar por período, semestre, turma, disciplina e nome\n");
    printf("Opção desejada: ");
    (void)scanf("%d", &opcao);    
    // Armazena a opcao em uma variavel e realiza os seus cases respectivos

    switch (opcao) {
        case 1:
            bubble_sort(alunos, num_alunos, compara_nome);
            salvar_csv("saida_nomes.csv", alunos, num_alunos, opcao);
            break;
        
        case 2:
            bubble_sort(alunos, num_alunos, compara_semestre);
            salvar_csv("saida_semestre.csv", alunos, num_alunos, opcao);
            break;
        
        case 3:
            bubble_sort(alunos, num_alunos, compara_item3); 
            salvar_csv("saida_item_tres.csv", alunos, num_alunos, opcao);
            break;
        
        case 4:
            bubble_sort(alunos, num_alunos, compara_disciplina_media); 
            salvar_csv("saida_disciplinaMedia.csv", alunos, num_alunos, opcao);
            break;
        
        case 5:
            bubble_sort(alunos, num_alunos, compara_item5); 
            salvar_csv("saida_item5.csv", alunos, num_alunos, opcao);
            break;
        
        default:
            printf("Opção inválida! Tente novamente...\n");
    }
}

int main() {
    estudantes alunos[MAX];                                 // Declara um array para armazenar os alunos
    int num_alunos = carregar_csv("entrada.txt", alunos);   // Carrega o CSV e finaliza o programa
     //int num_alunos = carregar_csv("entrada.csv", alunos); // Caso queira carregar o arquivo em csv é só descomentar o código e comentar o acima
    menu(alunos, num_alunos);
    return 0;
}