#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_MAX_SENHA 50

void salvarSenha(char *senha) {
  FILE *arquivo = fopen("senha.bin", "wb+");
  if (arquivo != NULL) {
    fwrite(senha, sizeof(char), strlen(senha) + 1, arquivo);
    fclose(arquivo);
    printf("\nSenha salva com sucesso!\n");
  } else {
    printf("\nErro ao salvar a senha.\n");
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
  printf("\nDigite a senha antiga: ");
  scanf("%s", senhaAntiga);

  // Verificar se a senha antiga está correta
  if (strcmp(senhaAntiga, senhaCorreta) == 0) {
    // Solicita a nova senha ao usuário
    printf("\nDigite a nova senha: ");
    scanf("%s", novaSenha);

    // Salva a nova senha no arquivo
    salvarSenha(novaSenha);

    printf("\nSenha atualizada com sucesso!\n");
  } else {
    printf("\nSenha antiga incorreta. Acesso negado.\n");
  }
}