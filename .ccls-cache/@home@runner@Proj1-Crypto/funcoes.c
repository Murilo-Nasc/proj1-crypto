#include "funcoes.h"

// PÁGINA DO INVESTIDOR
void investidor() {
  // Declaração das variáveis
  Usuario lista_usuarios[MAX_USUARIOS];
  Cotacao cotacao;
  carregar_cotacao(&cotacao);
  int num_usuarios = carregar_usuarios(lista_usuarios);
  int opcao, login_efetuado, index_usuario;
  char lixo;

  // Código Principal para investidor
  printf("Bem-vindo à página do investidor!\n");
  while (1) {
    printf("Escolha uma opção:\n");
    printf("1 - Login\n");
    printf("2 - Cadastro\n");
    printf("3 - Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    scanf("%c", &lixo); // Limpa o buffer

    switch (opcao) {
      case 1: // Função Login
        login_efetuado = login(lista_usuarios, num_usuarios, &index_usuario);
        if (login_efetuado) {
          printf("Login efetuado com sucesso! Bem vindo, %s\n", lista_usuarios[index_usuario].nome);

          // Menu de opções do investidor
          while (1) {
            printf("\nEscolha uma opção:\n");
            printf("1 - Consultar saldo\n");
            printf("2 - Consultar extrato\n");
            printf("3 - Depositar\n");
            printf("4 - Sacar\n");
            printf("5 - Comprar criptomoedas\n");
            printf("6 - Vender criptomoedas\n");
            printf("7 - Atualizar cotação\n");
            printf("8 - Sair\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcao);
            scanf("%c", &lixo); // Limpa o buffer

            switch (opcao) {
              case 1: // Função Consultar Saldo
                mostrar_saldo(lista_usuarios, index_usuario); 
                continue;
              case 2: // Função Consultar Extrato
                carregar_extrato(lista_usuarios, index_usuario);
                continue;
              case 3: // Função Depositar
                depositar(lista_usuarios, index_usuario, num_usuarios);
                continue;
              case 4: // Função Sacar
                sacar(lista_usuarios, index_usuario, num_usuarios);
                continue;
              case 5: // Função Comprar Criptomoedas
                comprar_criptomoedas(lista_usuarios, index_usuario, num_usuarios, cotacao);
                continue;
              case 6: // Função Vender Criptomoedas
                vender_criptomoedas(lista_usuarios, index_usuario, num_usuarios, cotacao);
                continue;
              case 7: // Função Atualizar Cotação
                atualizar_cotacao(&cotacao);
                continue;
              case 8: // Função Sair
                printf("Adeus! Volte sempre!\n");
                break;
            }
            break;
          }
        } else {
          printf("Login não efetuado. (Senha ou CPF incorretos)\n");
          continue;
        }
        break;

      case 2: // Função Cadastro
        cadastro(lista_usuarios, &num_usuarios);
        continue;

      case 3: // Sair
        printf("Adeus! Volte sempre!\n");
        return;

      default: // Opção Inválida
        printf("\nOpção inválida. Tente novamente.\n");
        continue;
    }
  }
}

// Função Cadastro
void cadastro(Usuario lista_usuarios[], int *num_usuarios) {
  long long int cpf;
  int senha;

  if (*num_usuarios >= MAX_USUARIOS) {
    printf("Limite de usuários atingido.\n");
  } else {
    while (1) {
      printf("\nDigite o nome:\n");
      scanf("%s", lista_usuarios[*num_usuarios].nome);
      lista_usuarios[*num_usuarios].nome[strcspn(lista_usuarios[*num_usuarios].nome, "\n")] = 0;
      
      printf("\nDigite o CPF (11 caracteres):\n");
      if (scanf("%lld", &cpf) != 1) {
        printf("\nEntrada inválida. Tente novamente.\n");
        while (getchar() != '\n'); // Limpa o buffer
        continue;
      }

      if (cpf < 10000000000LL || cpf > 99999999999LL) {  // Verifica se o CPF tem 11 dígitos
        printf("\nCPF inválido\n");
        continue;
      }

      printf("\nDigite a senha (6 dígitos):\n");
      if (scanf("%d", &senha) != 1) {
        printf("\nEntrada inválida. Tente novamente.\n");
        while (getchar() != '\n'); // Limpa o buffer
        continue;
      }

      if (senha < 100000 || senha > 999999) {  // Verifica se a senha tem 6 dígitos
        printf("\nSenha inválida\n");
        continue;
      }

      // Verifica se o CPF já está cadastrado
      int cpf_existe = 0;
      for (int i = 0; i < *num_usuarios; i++) {
        if (lista_usuarios[i].cpf == cpf) {
          printf("CPF já cadastrado\n");
          cpf_existe = 1;
          break;
        }
      }

      if (cpf_existe) {
        continue;
      }

      // Armazenar o usuário no array
      lista_usuarios[*num_usuarios].cpf = cpf;
      lista_usuarios[*num_usuarios].senha = senha;
      lista_usuarios[*num_usuarios].reais = 0.0;
      lista_usuarios[*num_usuarios].bitcoin = 0.0;
      lista_usuarios[*num_usuarios].ethereum = 0.0;
      lista_usuarios[*num_usuarios].ripple = 0.0;
      (*num_usuarios)++;


      // Salvar os usuários no arquivo
      salvar_usuarios(lista_usuarios, *num_usuarios);

      printf("\nCadastro realizado com sucesso!\n");
      break;
    }
  }
}


// Função Salvar Usuários
void salvar_usuarios(Usuario lista_usuarios[], int num_usuarios) {
  FILE *file = fopen("usuarios.bin", "wb"); 
  if (file == NULL) {
    perror("Erro ao abrir o arquivo para salvar");
    return;
  }

  fwrite(lista_usuarios, sizeof(Usuario), num_usuarios, file);
  fclose(file);
}


// Função Carregar Usuários
int carregar_usuarios(Usuario lista_usuarios[]) {
  FILE *file = fopen("usuarios.bin", "rb"); // Abrir em modo binário
  if (file == NULL) {
    perror("Erro ao abrir o arquivo para carregar");
    return 0; // Retorna 0 se não houver usuários
  }

  int num_usuarios = fread(lista_usuarios, sizeof(Usuario), MAX_USUARIOS, file);  // Lê os usuários
  fclose(file);
  return num_usuarios; 
}


// Função Login
int login(Usuario lista_usuarios[], int num_usuarios, int *index_usuario) {
  long long cpf;
  int senha;

  while (1) {
    printf("\nDigite seu CPF (11 caracteres):\n");
    if (scanf("%lld", &cpf) != 1) {
      printf("\nEntrada inválida. Tente novamente.\n");
      while (getchar() != '\n'); // Limpa o buffer
      continue;
    }

    if (cpf < 10000000000LL || cpf > 99999999999LL) {  // Verifica se o CPF tem 11 dígitos
      printf("\nCPF inválido\n");
      continue;
    }

    printf("\nDigite sua senha (6 dígitos):\n");
    if (scanf("%d", &senha) != 1) {
      printf("\nEntrada inválida. Tente novamente.\n");
      while (getchar() != '\n'); // Limpa o buffer
      continue;
    }

    if (senha < 100000 || senha > 999999) {  // Verifica se a senha tem 6 dígitos
      printf("\nSenha inválida\n");
      continue;
    }

    // Verifica se o CPF e a senha correspondem a um usuário válido
    int usuario_encontrado = 0;
    for (int i = 0; i < num_usuarios; i++) {
      if (lista_usuarios[i].cpf == cpf && lista_usuarios[i].senha == senha) {
        usuario_encontrado = 1;
        *index_usuario = i;
        break;
      }
    }

    if (usuario_encontrado) {
      return 1;
    } else {
      return 0;
    }
  }
}


// Função Mostrar Saldo
void mostrar_saldo(Usuario lista_usuarios[], int index_usuario){
  printf("\nSeus dados:\n");
  printf("CPF: %lld\n", lista_usuarios[index_usuario].cpf);
  printf("Senha: %d\n", lista_usuarios[index_usuario].senha);
  printf("\nSeu saldo:\n");
  printf("Reais: R$ %.2lf\n", lista_usuarios[index_usuario].reais);
  printf("Bitcoin: %.8lf BTC\n", lista_usuarios[index_usuario].bitcoin);
  printf("Ethereum: %.8lf ETH\n", lista_usuarios[index_usuario].ethereum);
  printf("Ripple: %.8lf XRP\n", lista_usuarios[index_usuario].ripple);
}


// Função Depósito
void depositar(Usuario lista_usuarios[], int index_usuario, int num_usuarios){
  double valor;
  printf("\nDigite o valor que deseja depositar em reais:\n");
  scanf("%lf", &valor);
  if (valor > 0){
    lista_usuarios[index_usuario].reais += valor;
    printf("\nDepósito realizado com sucesso!\n");
    salvar_usuarios(lista_usuarios, num_usuarios);  // Salva as alterações do saldo no arquivo
    salvar_extrato(&lista_usuarios[index_usuario], "+", valor, "Reais", 0.0, 0.0);  // Registra a transação
  }
  else {
    printf("Valor inválido (deve ser maior que zero)\n");
  }
}


// Função Saque
void sacar(Usuario lista_usuarios[], int index_usuario, int num_usuarios){
  double valor;
  int senha;
  printf("\nDigite o valor que deseja sacar em reais:\n");
  scanf("%lf", &valor);
  if (valor > 0){
    if (valor > lista_usuarios[index_usuario].reais){
      printf("\nSaldo insuficiente\n");
    }
    else {
      printf("Insira sua senha:\n");
      if (scanf("%d", &senha) != 1) {
        printf("\nEntrada inválida. Tente novamente.\n");
        while (getchar() != '\n'); // Limpa o buffer
        return;
      }
      
      if (senha == lista_usuarios[index_usuario].senha){
        lista_usuarios[index_usuario].reais -= valor;
        printf("\nSaque realizado com sucesso!\n");
        salvar_usuarios(lista_usuarios, num_usuarios);  // Salva as alterações do saldo no arquivo
        salvar_extrato(&lista_usuarios[index_usuario], "-", valor, "Reais", 0.0, 0.0);  // Registra a transação
      }
      else {
        printf("\nSenha incorreta\n");
      }
    }
  }
  else {
    printf("Valor inválido (deve ser maior que zero)\n");
  }
}


// Função Carregar Cotação
void carregar_cotacao(Cotacao *cotacao){
  FILE *file = fopen("cotacao.bin", "rb");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo para carregar");
  }
  else {
    fread(cotacao, sizeof(Cotacao), 1, file);
  }
  fclose(file);
}

// Função Salvar Cotação
void salvar_cotacao(Cotacao cotacao){
  FILE *file = fopen("cotacao.bin", "wb");
  if (file == NULL) {// Função Salvar Cotação
    perror("Erro ao abrir o arquivo para salvar");
  }
  else {
    fwrite(&cotacao, sizeof(Cotacao), 1, file);
  }
  fclose(file);
}


// Função Comprar Criptomoedas
void comprar_criptomoedas(Usuario lista_usuarios[], int index_usuario, int num_usuarios, Cotacao cotacao) {
  int opcao, senha, opcao_confirmacao;
  double valor, valor_taxado, valor_cripto, taxa;
  const char *nomes[] = {"Bitcoin", "Ethereum", "Ripple"};
  double *cotas[] = {&cotacao.bitcoin, &cotacao.ethereum, &cotacao.ripple};

  printf("\nEscolha a criptomoeda que deseja comprar:\n");
  for (int i = 0; i < 3; i++) {
    printf("%d. %s\n", i + 1, nomes[i]);
  }
  scanf("%d", &opcao);
  opcao--; // Ajuste para índice de array

  if (opcao < 0 || opcao >= 3) {
    printf("Opção inválida\n");
    return;
  }

  printf("Insira o valor que você deseja comprar em Reais (Sem incluir taxas):\n");
  scanf("%lf", &valor);

  if (valor <= 0) {
    printf("Valor inválido (deve ser maior que zero)\n");
    return;
  }

  if (opcao == 0) {taxa = 1.02;}
  else {taxa = 1.01;}
  valor_taxado = valor * taxa;

  if (valor_taxado > lista_usuarios[index_usuario].reais) {
    printf("\nSaldo insuficiente\n");
    return;
  }

  printf("Insira sua senha para confirmação:\n");
  if (scanf("%d", &senha) != 1) {
    printf("\nEntrada inválida. Tente novamente.\n");
    while (getchar() != '\n'); // Limpa o buffer
    return;
  }
  if (lista_usuarios[index_usuario].senha != senha) {
    printf("\nSenha incorreta\n");
    return;
  }

  valor_cripto = valor / (*cotas[opcao]);
  printf("\nResumo da compra:\n");
  printf("Valor em Reais com taxas: R$ %.2lf\n", valor_taxado);
  printf("Valor em %s: %.8lf\n", nomes[opcao], valor_cripto);
  printf("\n1. Confirmar Compra\n2. Cancelar Compra\n");
  scanf("%d", &opcao_confirmacao);

  if (opcao_confirmacao == 1) {
    if (opcao == 0) {  // Se a opção for Bitcoin
      lista_usuarios[index_usuario].bitcoin += valor_cripto;
    } else if (opcao == 1) {  // Se a opção for Ethereum
      lista_usuarios[index_usuario].ethereum += valor_cripto;
    } else if (opcao == 2) {  // Se a opção for Ripple
      lista_usuarios[index_usuario].ripple += valor_cripto;
    }
    lista_usuarios[index_usuario].reais -= valor_taxado;
    printf("\nCompra realizada com sucesso!\n");
    salvar_usuarios(lista_usuarios, num_usuarios);  // Salva as alterações do saldo no arquivo
    salvar_extrato(&lista_usuarios[index_usuario], "+", valor_cripto, nomes[opcao], taxa, (*cotas[opcao]));  // Registra a transação
  } else if (opcao == 2) {
    printf("\nCompra cancelada\n");
  } else {
    printf("Opção inválida\n");
  }
}


// Função Vender Criptomoedas
void vender_criptomoedas(Usuario lista_usuarios[], int index_usuario, int num_usuarios, Cotacao cotacao) {
  int opcao, senha, opcao_confirmacao;
  double valor_cripto, valor_cripto_taxado, valor_reais, taxa;
  const char *nomes[] = {"Bitcoin", "Ethereum", "Ripple"};
  double *cotas[] = {&cotacao.bitcoin, &cotacao.ethereum, &cotacao.ripple};

  printf("\nEscolha a criptomoeda que deseja vender:\n");
  for (int i = 0; i < 3; i++) {
    printf("%d. %s\n", i + 1, nomes[i]);
  }
  scanf("%d", &opcao);
  opcao--; // Ajuste para índice de array

  if (opcao < 0 || opcao >= 3) {
    printf("Opção inválida\n");
    return;
  }

  printf("Insira o valor que você deseja vender em %s (Sem incluir taxas):\n", nomes[opcao]);
  scanf("%lf", &valor_cripto);

  if (valor_cripto <= 0) {
    printf("Valor inválido (deve ser maior que zero)\n");
    return;
  }

  if (opcao == 0) {
    taxa = 1.03;
    valor_cripto_taxado = valor_cripto * taxa;
    if (valor_cripto_taxado > lista_usuarios[index_usuario].bitcoin) {
      printf("\nSaldo insuficiente\n");
      return;
    }
  }
  else if (opcao == 1) {
    taxa = 1.02;
    valor_cripto_taxado = valor_cripto * taxa;
    if (valor_cripto_taxado > lista_usuarios[index_usuario].ethereum) {
      printf("\nSaldo insuficiente\n");
      return;
    }
  }
  else if (opcao == 2) {
    taxa = 1.01;
    valor_cripto_taxado = valor_cripto * taxa;
    if (valor_cripto_taxado > lista_usuarios[index_usuario].ripple) {
      printf("\nSaldo insuficiente\n");
      return;
    }
  }

  printf("Insira sua senha para confirmação:\n");;
  if (scanf("%d", &senha) != 1) {
    printf("\nEntrada inválida. Tente novamente.\n");
    while (getchar() != '\n'); // Limpa o buffer
    return;
  }
  if (lista_usuarios[index_usuario].senha != senha) {
    printf("\nSenha incorreta\n");
    return;
  }

  valor_reais = valor_cripto * (*cotas[opcao]);
  printf("\nResumo da venda:\n");
  printf("Valor em %s com taxas: %.8lf\n", nomes[opcao], valor_cripto_taxado);
  printf("Valor em Reais: R$ %.2lf\n", valor_reais);
  printf("\n1. Confirmar Venda\n2. Cancelar Venda\n");
  scanf("%d", &opcao_confirmacao);

  if (opcao_confirmacao == 1) {
    if (opcao == 0) {  // Se a opção for Bitcoin
      lista_usuarios[index_usuario].bitcoin -= valor_cripto_taxado;
    } else if (opcao == 1) {  // Se a opção for Ethereum
      lista_usuarios[index_usuario].ethereum -= valor_cripto_taxado;
    } else if (opcao == 2) {  // Se a opção for Ripple
      lista_usuarios[index_usuario].ripple -= valor_cripto_taxado;
    }
    lista_usuarios[index_usuario].reais += valor_reais;
    printf("\nVenda realizada com sucesso!\n");
    salvar_usuarios(lista_usuarios, num_usuarios);  // Salva as alterações do saldo no arquivo
    salvar_extrato(&lista_usuarios[index_usuario], "-", valor_cripto_taxado, nomes[opcao], taxa, (*cotas[opcao]));  // Registra a transação
  } else if (opcao == 2) {
    printf("\nVenda cancelada\n");
  } else {
    printf("Opção inválida\n");
  }
}


// Função Atualizar Cotação
void atualizar_cotacao(Cotacao *cotacao) {
  double valor_bitcoin, valor_ethereum, valor_ripple;
  valor_bitcoin = cotacao->bitcoin;
  valor_ethereum = cotacao->ethereum;
  valor_ripple = cotacao->ripple;

  cotacao->bitcoin *= (1 + ((double)rand() / RAND_MAX) * 0.1 - 0.05); 
  cotacao->ethereum *= (1 + ((double)rand() / RAND_MAX) * 0.1 - 0.05);
  cotacao->ripple *= (1 + ((double)rand() / RAND_MAX) * 0.1 - 0.05);

  printf("\nCotação Atualizada!\n");
  printf("Bitcoin: %.8lf BTC -> %.8lf BTC\n", valor_bitcoin, cotacao->bitcoin);
  printf("Ethereum: %.8lf ETH -> %.8lf ETH\n", valor_ethereum, cotacao->ethereum);
  printf("Ripple: %.8lf XRP -> %.8lf XRP\n", valor_ripple, cotacao->ripple);
}


// Função para salvar extrato de transações
void salvar_extrato(Usuario *usuario, char sinal[], double valor, const char moeda[], double taxa, double cotacao) {
  FILE *file;
  char filename[30];
  snprintf(filename, sizeof(filename), "extrato_%lld.txt", usuario->cpf); // Nome do arquivo baseado no CPF

  file = fopen(filename, "a"); // Abrir em modo append
  if (file == NULL) {
    perror("Erro ao abrir o arquivo para salvar o extrato");
    return;
  }

  // Obter o timestamp atual
  char timestamp[20];
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  strftime(timestamp, sizeof(timestamp), "%d-%m-%Y %H:%M:%S", tm_info);

  // Escreve a transação no arquivo
  fprintf(file, "%s %s %.4lf %s CT: %.2f TX: %.2f REAL: %.2f BTC: %.6f ETH: %.6f XRP: %.6f\n", 
  timestamp, sinal, valor, moeda, cotacao, taxa, usuario->reais, 
  usuario->bitcoin, usuario->ethereum, usuario->ripple);

  fclose(file);
}



// Função para carregar extrato (caso precise futuramente)
void carregar_extrato(Usuario *usuario, int index_usuario) {
  FILE *file;
  char filename[30];
  snprintf(filename, sizeof(filename), "extrato_%lld.txt", usuario[index_usuario].cpf);

  file = fopen(filename, "r");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo para carregar o extrato");
    return;
  }

  char linha[256];
  while (fgets(linha, sizeof(linha), file)) {
    printf("%s", linha);
  }

  fclose(file);
}


// PÁGINA DO ADM
void adm() {
  Admin dados_adm;
  int logado, opcao;
  char lixo;
  Usuario lista_usuarios[MAX_USUARIOS];
  int num_usuarios = carregar_usuarios(lista_usuarios);
  Cotacao cotacao;
  carregar_cotacao(&cotacao);
  
  carregar_adm(&dados_adm);
  login_adm(dados_adm);
  while (1) {
    printf("\nEscolha uma opção:\n");
    printf("1 - Cadastrar novo investidor\n");
    printf("2 - Excluir investidor\n");
    printf("3 - Cadastrar criptomoeda\n");
    printf("4 - Excluir criptomoeda\n");
    printf("5 - Consultar saldo de investidor\n");
    printf("6 - Consultar extrato de investidor\n");
    printf("7 - Atualizar cotação\n");
    printf("8 - Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    scanf("%c", &lixo); // Limpa o buffer

    switch (opcao) {
      case 1: // Função Cadastrar novo investidor
        cadastro(lista_usuarios, &num_usuarios);
        printf("Usuário %s cadastrado como investidor\n", lista_usuarios[num_usuarios - 1].nome);
        continue;
      case 2: // Função Excluir investidor
        excluir_investidor(lista_usuarios, &num_usuarios);
        continue;
      case 3: // Função Cadastrar criptomoeda

        continue;
      case 4: // Função Excluir criptomoeda

        continue;
      case 5: // Função Consultar saldo de investidor
        saldo_investidor(lista_usuarios, &num_usuarios);
        continue;
      case 6: // Consultar extrato de investidor
        extrato_investidor(lista_usuarios, &num_usuarios);
        continue;
      case 7: // Função Atualizar Cotação
        atualizar_cotacao(&cotacao);
        continue;
      case 8: // Função Sair
        printf("Adeus! Volte sempre!\n");
        break;
      default:
        printf("Opção inválida!\n");
        continue;
      }
    break;
  }
}


// Carregar ADM
void carregar_adm(Admin *dados_adm) {
  FILE *file = fopen("admin.bin", "rb"); 
  if (file == NULL) {
    perror("Erro ao abrir o arquivo para carregar");
  }
  int num_usuarios = fread(dados_adm, sizeof(Admin), 1, file);  
  
  fclose(file);
}

// Login do ADM
void login_adm(Admin dados_adm) {
  long long cpf;
  int senha;
  

  while (1) {
    printf("\nLOGIN DE ADMINISTRADOR\n");
    printf("\nDigite seu CPF (11 caracteres):\n");
    if (scanf("%lld", &cpf) != 1) {
      printf("\nEntrada inválida. Tente novamente.\n");
      while (getchar() != '\n'); // Limpa o buffer
      continue;
    }

    if (cpf < 10000000000LL || cpf > 99999999999LL) {  // Verifica se o CPF tem 11 dígitos
      printf("\nCPF inválido\n");
      continue;
    }

    printf("\nDigite sua senha (6 dígitos):\n");
    if (scanf("%d", &senha) != 1) {
      printf("\nEntrada inválida. Tente novamente.\n");
      while (getchar() != '\n'); // Limpa o buffer
      continue;
    }

    if (senha < 100000 || senha > 999999) {  // Verifica se a senha tem 6 dígitos
      printf("\nSenha inválida\n");
      continue;
    }

    if (cpf == dados_adm.cpf && senha == dados_adm.senha) {
      printf("\nLogin realizado com sucesso!\n");
      break;
    } else {
      printf("\nCPF ou senha incorretos\n");
    }
    
  }
}


// Exclusão de Investidor
void excluir_investidor(Usuario lista_usuarios[], int *num_usuarios) {
  long long cpf;
  int index, opcao;
  char lixo;
  
  while (1) {
    printf("\nInsira o CPF do investidor que deseja excluir:\n");
    if (scanf("%lld", &cpf) != 1) {
      printf("\nEntrada inválida. Tente novamente.\n");
      while (getchar() != '\n'); // Limpa o buffer
      continue;
    }

    if (cpf < 10000000000LL || cpf > 99999999999LL) {  // Verifica se o CPF tem 11 dígitos
      printf("\nCPF inválido\n");
      continue;
    }

    // Verifica se o CPF e a senha correspondem a um usuário válido
    int usuario_encontrado = 0;
    for (int i = 0; i < *num_usuarios; i++) {
      if (lista_usuarios[i].cpf == cpf) {
        usuario_encontrado = 1;
        index = i;
        break;
      }
    }
    
    if (!usuario_encontrado) {
      printf("\nCPF não encontrado\n");
      break;
    } else if (usuario_encontrado) {
      printf("Dados do investidor:\n");
      printf("Nome: %s\nCPF: %lld\nSenha: %d\n", lista_usuarios[index].nome, lista_usuarios[index].cpf, lista_usuarios[index].senha);
      printf("Deseja excluir este investidor? (1 - Sim, 2 - Não)\n");
      scanf("%d", &opcao);
      scanf("%c", &lixo);
      if (opcao) {
        for (int i = index; i < *num_usuarios - 1; i++) {
          lista_usuarios[i] = lista_usuarios[i + 1];
        }
        (*num_usuarios)--;
        salvar_usuarios(lista_usuarios, *num_usuarios);
        printf("Usuário excluido com sucesso\n");
        break;
      }
    }
  }
}


// Função Consultar saldo de investidor
void saldo_investidor(Usuario lista_usuarios[], int *num_usuarios){
long long cpf;
  int index, opcao;
  char lixo;

  while (1) {
    printf("\nInsira o CPF do investidor que deseja consultar:\n");
    if (scanf("%lld", &cpf) != 1) {
      printf("\nEntrada inválida. Tente novamente.\n");
      while (getchar() != '\n'); // Limpa o buffer
      continue;
    }

    if (cpf < 10000000000LL || cpf > 99999999999LL) {  // Verifica se o CPF tem 11 dígitos
      printf("\nCPF inválido\n");
      continue;
    }

    // Verifica se o CPF e a senha correspondem a um usuário válido
    int usuario_encontrado = 0;
    for (int i = 0; i < *num_usuarios; i++) {
      if (lista_usuarios[i].cpf == cpf) {
        usuario_encontrado = 1;
        index = i;
        break;
      }
    }

    if (!usuario_encontrado) {
      printf("\nCPF não encontrado\n");
      break;
    } else if (usuario_encontrado) {
      printf("\nDados do investidor:\n\n");
      printf("Nome: %s\nCPF: %lld\nSenha: %d\n", lista_usuarios[index].nome, lista_usuarios[index].cpf, lista_usuarios[index].senha);
      printf("\nSaldo do usuário:\n\n");
      printf("Reais: %.2lf\n", lista_usuarios[index].reais);
      printf("Bitcoin: %.8lf\n", lista_usuarios[index].bitcoin);
      printf("Ethereum: %.8lf\n", lista_usuarios[index].ethereum);
      printf("Ripple: %.8lf\n", lista_usuarios[index].ripple);
      printf("\nDeseja consultar o saldo de outro investidor? (1 - Sim, 2 - Não)\n");
      scanf("%d", &opcao);
      scanf("%c", &lixo);
      if (opcao == 1) {
        continue;
      }
      else {
        break;
      }
    }
  }
}


// Consultar extrato de investidor
void extrato_investidor(Usuario lista_usuarios[], int *num_usuarios){
long long cpf;
  int index, opcao;
  char lixo;

  while (1) {
    printf("\nInsira o CPF do investidor que deseja consultar:\n");
    if (scanf("%lld", &cpf) != 1) {
      printf("\nEntrada inválida. Tente novamente.\n");
      while (getchar() != '\n'); // Limpa o buffer
      continue;
    }

    if (cpf < 10000000000LL || cpf > 99999999999LL) {  // Verifica se o CPF tem 11 dígitos
      printf("\nCPF inválido\n");
      continue;
    }

    // Verifica se o CPF e a senha correspondem a um usuário válido
    int usuario_encontrado = 0;
    for (int i = 0; i < *num_usuarios; i++) {
      if (lista_usuarios[i].cpf == cpf) {
        usuario_encontrado = 1;
        index = i;
        break;
      }
    }

    if (!usuario_encontrado) {
      printf("\nCPF não encontrado\n");
      break;
    } else if (usuario_encontrado) {
      printf("\nDados do investidor:\n\n");
      printf("Nome: %s\nCPF: %lld\nSenha: %d\n", lista_usuarios[index].nome, lista_usuarios[index].cpf, lista_usuarios[index].senha);
      printf("\nExtrato do usuário:\n\n");
      carregar_extrato(lista_usuarios, index);
      printf("\nDeseja consultar o extrato de outro investidor? (1 - Sim, 2 - Não)\n");
      scanf("%d", &opcao);
      scanf("%c", &lixo);
      if (opcao == 1) {
        continue;
      }
      else {
        break;
      }
    }
  }
}