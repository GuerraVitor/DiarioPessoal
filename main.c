#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRY_LENGTH 1000
#define TAMANHO_MAX_SENHA 50
#define MAX_TAMANHO 200

const char *nome_diretorio = "ArquivosUsuario";

// Estrutura de dados auxiliar
typedef struct {
  DATA data;
  char entry[MAX_ENTRY_LENGTH];
} DATAFORNECIDA;

int validarData(DATAFORNECIDA *data) {
  if (data->data.ano < 1000 || data->data.ano > 3000) {
    printf("\nData inválida, ano deve estar entre 1000 e 3000.\n");
    return 0;
  }
  if (data->data.mes < 1 || data->data.mes > 12) {
    printf("\nData inválida, mês deve estar entre 1 e 12.\n");
    return 0;
  }
  if (data->data.dia < 1 || data->data.dia > 31) {
    printf("\nData inválida, dia deve estar entre 1 e 31.\n");
    return 0;
  }

  // Validação de dias por mês
  if ((data->data.mes == 4 || data->data.mes == 6 || data->data.mes == 9 || data->data.mes == 11) &&
      data->data.dia > 30) {
    printf("\nData inválida, neste mês, o máximo de dias é 30.\n");
    return 0;
  }
  if (data->data.mes == 2) {
    if ((data->data.ano % 4 == 0 && data->data.ano % 100 != 0) || data->data.ano % 400 == 0) {
      if (data->data.dia > 29) {
        printf("\nData inválida, Fevereiro em ano bissexto tem no máximo 29 dias.\n");
        return 0;
      }
    } else {
      if (data->data.dia > 28) {
        printf("\nData inválida, Fevereiro em ano não bissexto tem no máximo 28 dias.\n");
        return 0;
      }
    }
  }
  return 1; // A data é válida
}

// Função que grava o texto no arquivo
void gravarTextoNoArquivo(char *texto, const char *nome_diretorio, DATA data) {
  FILE *arquivo;
  char nomeArquivo[MAX_TAMANHO];

  // Criando o nome do arquivo usando os valores da estrutura de data
  sprintf(nomeArquivo, "%d_%d_%d.txt", data.dia, data.mes, data.ano);

  // Abrindo o arquivo para escrita
  char caminhoArquivo[MAX_TAMANHO + strlen(nome_diretorio) +
                      2]; // +2 para o '/' e o caractere nulo
  sprintf(caminhoArquivo, "%s/%s", nome_diretorio, nomeArquivo);
  arquivo = fopen(caminhoArquivo, "w");

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  // Gravando o texto no arquivo
  fputs(texto, arquivo);

  // Fechando o arquivo
  fclose(arquivo);
}

// Função que adciona o texto no final arquivo
void adcionaTextoNoArquivo(char *texto, const char *nome_diretorio, DATA data) {
  FILE *arquivo;
  char nomeArquivo[MAX_TAMANHO];

  // Criando o nome do arquivo usando os valores da estrutura de data
  sprintf(nomeArquivo, "%d_%d_%d.txt", data.dia, data.mes, data.ano);

  // Abrindo o arquivo para escrita no final
  char caminhoArquivo[MAX_TAMANHO + strlen(nome_diretorio) +
                      2]; // +2 para o '/' e o caractere nulo
  sprintf(caminhoArquivo, "%s/%s", nome_diretorio, nomeArquivo);
  arquivo = fopen(caminhoArquivo, "a");

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  // Gravando o texto no arquivo
  fputs(texto, arquivo);

  // Fechando o arquivo
  fclose(arquivo);
}

PONT buscaBinaria(LISTA *lista, DATAFORNECIDA dataScaneada) {
  PONT inicio = lista->inicio;
  PONT fim = NULL;
  PONT meio;

  while (inicio != fim) {
    int distancia = 0;
    meio = inicio;

    // Calcula a distância entre os ponteiros
    while (meio != fim && distancia < 2) {
      meio = meio->prox;
      distancia++;
    }

    // Verifica se a data do meio é a procurada
    if (meio->reg.DATA.dia == dataScaneada.data.dia &&
        meio->reg.DATA.mes == dataScaneada.data.mes &&
        meio->reg.DATA.ano == dataScaneada.data.ano) {
      return meio;
    } else if (meio->reg.DATA.ano < dataScaneada.data.ano ||
               (meio->reg.DATA.ano == dataScaneada.data.ano &&
                meio->reg.DATA.mes < dataScaneada.data.mes) ||
               (meio->reg.DATA.ano == dataScaneada.data.ano &&
                meio->reg.DATA.mes == dataScaneada.data.mes &&
                meio->reg.DATA.dia < dataScaneada.data.dia)) {
      fim = meio;
    } else {
      inicio = meio->prox;
    }
  }

  return NULL;
}

// funcao que exclui elemento especifico da lista
void removerElemento(LISTA *lista, DATAFORNECIDA dataScaneada) {
  PONT atual = lista->inicio;
  PONT anterior = NULL;

  while (atual != NULL) {
    if (atual->reg.DATA.dia == dataScaneada.data.dia &&
        atual->reg.DATA.mes == dataScaneada.data.mes &&
        atual->reg.DATA.ano == dataScaneada.data.ano) {

      if (anterior == NULL) {
        lista->inicio =
            atual->prox; // O elemento a ser removido é o primeiro da lista
      } else {
        anterior->prox = atual->prox; // Elemento a ser removido está no meio ou
                                      // no final da lista
      }

      free(atual); // Libera memória do nó removido
      return;
    }

    anterior = atual;
    atual = atual->prox;
  }

  printf("\nElemento não encontrado na lista.\n");
}

void adicionarEntrada(LISTA *lista) {
  DATAFORNECIDA *pdataScaneada = (DATAFORNECIDA *)malloc(sizeof(DATAFORNECIDA));
  DATAFORNECIDA dataScaneada;
  pdataScaneada = &dataScaneada;

  printf("Digite a data (DD MM AAAA): ");

  //verificando se a data eh valida
  scanf("%d %d %d", &dataScaneada.data.dia, &dataScaneada.data.mes, &dataScaneada.data.ano);
  if((validarData(pdataScaneada)) == 0) return;

  // Verificando se existe entrada com esta data
  if (buscarData(lista, dataScaneada.data.dia, dataScaneada.data.mes, dataScaneada.data.ano)) {
    printf("\ndata já ocupada.\n");
  } else {
    inserirElemento(lista, dataScaneada.data); // Insere data na lista
    ordenarLista(lista);                       // Ordena a lista
    printf("Digite a entrada: ");
    getchar();                                 // Limpar o buffer do teclado
    fgets(dataScaneada.entry, MAX_ENTRY_LENGTH, stdin);
    gravarTextoNoArquivo(dataScaneada.entry, nome_diretorio, dataScaneada.data);

    printf("\nEntrada adicionada com sucesso!\n");
  }
}

void editarEntrada(LISTA *lista) {
  DATAFORNECIDA dataScaneada;
  printf("Digite a data (DD MM AAAA): ");

  // Verificando se existe entrada com esta data
  scanf("%d %d %d", &dataScaneada.data.dia, &dataScaneada.data.mes,
        &dataScaneada.data.ano);

  if (buscarData(lista, dataScaneada.data.dia, dataScaneada.data.mes,
                 dataScaneada.data.ano)) {
    printf("Digite a entrada (sera adcionado no final do arquivo): ");
    getchar();
    fgets(dataScaneada.entry, MAX_ENTRY_LENGTH, stdin);
    adcionaTextoNoArquivo(dataScaneada.entry, nome_diretorio,
                          dataScaneada.data);

    printf("\nEntrada adcionado ao registro anterior com sucesso!\n");
  } else {
    printf("\nNao existe arquivo com a data fornecida\n");
  }
}

void LerEntradaPorData(LISTA *lista) {
  DATAFORNECIDA dataScaneada;
  printf("Digite a data (DD MM AAAA): ");

  scanf("%d %d %d", &dataScaneada.data.dia, &dataScaneada.data.mes,
        &dataScaneada.data.ano);

  if (buscarData(lista, dataScaneada.data.dia, dataScaneada.data.mes,
                 dataScaneada.data.ano)) {
    FILE *arquivoBuscado;
    char caminhoArquivoBuscado[MAX_TAMANHO + strlen(nome_diretorio) + 15];
    sprintf(caminhoArquivoBuscado, "%s/%d_%d_%d.txt", nome_diretorio,
            dataScaneada.data.dia, dataScaneada.data.mes,
            dataScaneada.data.ano);

    arquivoBuscado = fopen(caminhoArquivoBuscado, "r");
    if (arquivoBuscado == NULL) {
      printf("Erro ao abrir o arquivo.\n");
      return;
    }

    printf("\nA entrada desse dia é:\n");
    char linha[1000];

    // Ler uma linha até que não haja mais linhas para ler
    while (fgets(linha, sizeof(linha), arquivoBuscado) != NULL) {
      printf("%s", linha);
    }

    fclose(arquivoBuscado);
  } else {
    printf("\nNão há entrada para esta data.\n");
  }
}

void listarEntradasPorDatas(LISTA *lista) { exibirLista(lista); }

void excluiArquivo(LISTA *lista) {
  DATAFORNECIDA dataScaneada;
  printf("Digite a data (DD MM AAAA): ");

  scanf("%d %d %d", &dataScaneada.data.dia, &dataScaneada.data.mes,
        &dataScaneada.data.ano);

  if (buscarData(lista, dataScaneada.data.dia, dataScaneada.data.mes,
                 dataScaneada.data.ano)) {
    char caminhoArquivoBuscado[MAX_TAMANHO + strlen(nome_diretorio) + 15];
    sprintf(caminhoArquivoBuscado, "%s/%d_%d_%d.txt", nome_diretorio,
            dataScaneada.data.dia, dataScaneada.data.mes,
            dataScaneada.data.ano);

    if (remove(caminhoArquivoBuscado) == 0) {
      removerElemento(lista, dataScaneada);
      printf("\nArquivo excluído com sucesso.\n");
    } else {
      printf("\nErro ao excluir o arquivo.\n");
    }
  } else {
    printf("\nNão há entrada para esta data.\n");
  }
}

// aqui termina o comando inicial
//
// aqui começa a implementacao da senha

void salvarSenha(const char *senha) {
  FILE *arquivo = fopen("senha.bin", "rb+");
  if (arquivo != NULL) {
    fwrite(senha, sizeof(char), strlen(senha) + 1, arquivo);
    fclose(arquivo);
    printf("Senha salva com sucesso!\n");
  } else {
    printf("Erro ao salvar a senha.\n");
  }
}

void lerSenha(char *senha) {
  FILE *arquivo = fopen("senha.bin", "rb+");
  if (arquivo != NULL) {
    fread(senha, sizeof(char), TAMANHO_MAX_SENHA, arquivo);
    fclose(arquivo);
  }
}

void atualizarSenha() {
  char senhaCorreta[TAMANHO_MAX_SENHA];
  char senhaAntiga[TAMANHO_MAX_SENHA];
  char novaSenha[TAMANHO_MAX_SENHA];

  // Lê a senha atual do arquivo
  lerSenha(senhaCorreta);

  // Solicita a senha antiga ao usuário
  printf("Digite a senha antiga: ");
  scanf("%s", senhaAntiga);

  // Verificar se a senha antiga está correta
  if (strcmp(senhaAntiga, senhaCorreta) == 0) {
    // Solicita a nova senha ao usuário
    printf("Digite a nova senha: ");
    scanf("%s", novaSenha);

    // Salva a nova senha no arquivo
    salvarSenha(novaSenha);

    printf("Senha atualizada com sucesso!\n");
  } else {
    printf("Senha antiga incorreta. Acesso negado.\n");
  }
}

// aqui termina a implementacao de senha

int main() {
  FILE *pa;
  pa = fopen("senha.bin", "rb");
  int choice, k = 1;
  char *senha = (char *)malloc(50 * sizeof(char));
  char *pssw = (char *)malloc(50 * sizeof(char));

  if (!pa) {
    printf("Primeira vez entrando? Digite uma senha, por favor: ");
    scanf("%s", senha);
    salvarSenha(senha);
  } else {
    while (k) {
      printf("1. Digite a senha\n");
      printf("2. Mudar senha\n");
      printf("3. Sair\n");
      scanf("%d", &choice);

      switch (choice) {
      case 1:
        printf("Senha: ");
        scanf("%s", senha);
        lerSenha(pssw);
        if (strcmp(senha, pssw) == 0) {
          printf("\n... Senha correta, caregando menu...\n");
          k--;
        } else {
          printf("\nSenha incorreta! Tente novamente!\n");
        }
        break;

      case 2:
        atualizarSenha();
        break;

      case 3:
        printf("Encerrando o programa...\n");
        exit(0);
      default:
        printf("Opção inválida. Tente novamente.\n");
      }
    }
  }

  LISTA *lista = (LISTA *)malloc(sizeof(LISTA));
  inicializarLista(lista);
  popularListaDeArquivo(lista);

  while (1) {
    printf(" \nMenu\n");
    printf("\n1. Adicionar entrada\n");
    printf("2. Editar entrada\n");
    printf("3. Ler entrada (seleciona por data)\n");
    printf("4. Listar entradas por data\n");
    printf("5. Excluir entrada (seleciona por data)\n");
    printf("6. Sair\n");
    printf("\nEscolha uma opção: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      adicionarEntrada(lista);
      break;
    case 2:
      editarEntrada(lista);
      break;
    case 3:
      LerEntradaPorData(lista);
      break;
    case 4:
      listarEntradasPorDatas(lista);
      break;
    case 5:
      excluiArquivo(lista);
      break;
    case 6:
      printf("Encerrando o programa...\n");
      salvarEmArquivo(lista);
      exit(0);
    default:
      printf("Opção inválida. Tente novamente.\n");
    }
  }

  return 0;
}
