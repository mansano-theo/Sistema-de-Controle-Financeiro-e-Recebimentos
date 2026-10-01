//Joao Pedro Diniz Nacur e THEO LOPES MANSANO
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define LISTA 20
#define MAX_PARCELAS 10
#define LIMITE_PARCELAS 1000

typedef struct Data
{
    int dia;
    int mes;
    int ano;
}data;

typedef struct Clientes
{
    char CPF[20];
    char nome[50];
    char telefone[15];
    data dataNascimento;
    char numeroCartao[20];
    char chavePix[30];
}Cliente;

typedef struct Vendas
{
    int idVenda;
    Cliente cliente;
    float valorTotalVenda;
    int formaPagamento;
    int qtdParcelas;
    data dataVenda;
    char observacao[50];
}vendas;

typedef struct Parcelas
{
    int idParcela;
    int idVenda;
    int numeroDaParcela;
    float valorDaParcela;
    data dataVencimento;
    data dataRecebimento;
    char situacaoDaParcela;
}Parcela;

Parcela listaParcelas[1000];
int qtdParcelas = 0;
int proximoIdParcela = 1001;

Cliente listaClientes[LISTA];
int qtdClientes = 0;

vendas listaVendas[100];
int qtdVendas = 0;
int proximoIdVenda = 101;

float recibos();
float aindaReceber();
float parcelasVencidas();
float debitosPendentes();
float faturamentoTotal();
int validarCPF(char cpf[]);
int validarData(data d);
int anoBissexto(int ano);
int validarTelefone(char telefone[]);
int validarCartao(char numeroCartao[]);
int validarEmail (char email[]);
int registroVendas();
void cadastroClientes();
void buscarClientes();
int consultarVendas();
void geradorParcelas(int idVenda, float valorTotal, int qtd, data dataVenda);
void quitarParcela();
void liquidarDivida();
void identificarParcelasAtraso();
void relatorioDiario();
void relatorioMensal();
void dashboardFinanceiro();


int main(){
int op;
    do{
        printf("\nMENU\n");
        printf("1) Cadastrar Cliente\n");
        printf("2) Buscar Cliente\n");
        printf("3) Registrar Venda\n");
        printf("4) Consultar Vendas\n");
        printf("5) Atualizar Parcelas em Atraso\n");
        printf("6) Quitar Parcela\n");
        printf("7) Liquidar Divida Total\n");
        printf("8) Relatorios Financeiros (Totais e Debitos)\n");
        printf("9) Relatorio Diario\n");
        printf("10) Relatorio Mensal\n");
        printf("11) Dashboard Financeiro\n");
        printf("0) Sair\n");
        printf("\n");
        printf("Escolha: ");
        scanf("%d", &op);
        getchar();

        switch(op) {
            case 1:
                cadastroClientes();
                break;
            case 2:
                buscarClientes();
                break;
            case 3:
                registroVendas();
                break;
            case 4:
                consultarVendas();
                break;
            case 5:
                identificarParcelasAtraso();
                break;
            case 6:
                quitarParcela();
                break;
            case 7:
                liquidarDivida();
                break;
            case 8:
                printf("\nFaturamento Total: R$ %.2f", faturamentoTotal());
                printf("\nDebitos Pendentes: R$ %.2f", debitosPendentes());
                printf("\nRecebido: R$ %.2f", recibos());
                printf("\nAinda a Receber: R$ %.2f", aindaReceber());
                printf("\nParcelas Vencidas: R$ %.2f\n", parcelasVencidas());
                break;
            case 9:
                relatorioDiario();
                break;

            case 10:
                relatorioMensal();
                break;
            case 11:
                dashboardFinanceiro();
                break;
            case 0:
                printf("\nSaindo...");
                break;
            default:
                printf("\nErro");
                break;
        }
    }while(op != 0);

    return 0;
}

float recibos() {
    float total = 0;
    int i;
    for (i = 0; i < qtdParcelas; i++) {
        if (listaParcelas[i].situacaoDaParcela == 'R') {
            total += listaParcelas[i].valorDaParcela;
        }
    }
    return total;
}

float aindaReceber() {
    float total = 0;
    int i;
    for (i = 0; i < qtdParcelas; i++) {
        if (listaParcelas[i].situacaoDaParcela == 'A') {
            total += listaParcelas[i].valorDaParcela;
        }
    }
    return total;
}

float parcelasVencidas() {
    float total = 0;
    int i;
    for (i = 0; i < qtdParcelas; i++) {
        if (listaParcelas[i].situacaoDaParcela == 'V') {
            total += listaParcelas[i].valorDaParcela;
        }
    }
    return total;
}

float debitosPendentes() {
    float total = 0;
    int i = 0;

    for (i = 0; i < qtdParcelas; i++) {
        if (listaParcelas[i].situacaoDaParcela == 'A' || listaParcelas[i].situacaoDaParcela == 'V') {
            total += listaParcelas[i].valorDaParcela;
        }
    }
    return total;
}

float faturamentoTotal(){
    int i = 0;
    float somaTotal = 0.0;

    for(i = 0; i < qtdVendas; i++){
        somaTotal += listaVendas[i].valorTotalVenda;
    }
    return somaTotal;
}

int validarCPF(char cpf[]){
	int tam = strlen(cpf), i = 0, soma = 0,todosIguais = 1;
    int peso = 10, resto = 0, atualNumero = 0, digitosIniciais, digitosFinais;

    for(i = 0; i < 11; i++){
        if(cpf[i] < '0' || cpf[i] > '9'){
            return 0;
        }
    }

	if(tam != 11){
		return 0;
	}

    for(i = 0; i < 11; i++){
        if(cpf[i] != cpf[0]){
            todosIguais = 0;
            break;
        }
    }

    if(todosIguais == 1){
        return 0;
    }

    for(i = 0; i < 9; i++){
        atualNumero = cpf[i] - '0';
        soma += (atualNumero * peso);
        peso--;
    }

    resto = soma % 11;

    if(resto < 2){
        digitosIniciais = 0;
    } else {
        digitosIniciais = 11 - resto;
    }

    if(digitosIniciais != (cpf[9] - '0')){
        return 0;
    }

    soma = 0;
    peso = 11;

    for(i = 0; i < 10; i++){
        atualNumero = cpf[i] - '0';
        soma += (atualNumero * peso);
        peso--;
    }

     resto = soma % 11;

    if(resto < 2){
        digitosFinais = 0; 
    } else {
        digitosFinais = 11 - resto; 
    }

    if(digitosFinais != (cpf[10] - '0')){
        return 0;
    }
	return 1;
}

int validarData(data d) {
    if(d.ano < 1900 || d.ano > 2026){
        return 0;
    }

    if(d.mes < 1 || d.mes > 12){
        return 0;
    }

    int diasPorMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(d.mes == 2 && anoBissexto(d.ano)){
        diasPorMes[2] = 29;
    }

    if(d.dia < 1 || d.dia > diasPorMes[d.mes]){
        return 0;
    }
    return 1;
}

int anoBissexto(int ano){
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        return 1;
    }
    return 0;
}

int validarTelefone(char telefone[]) {
    int TAM = strlen(telefone);
    int i;
    
    if (TAM != 11 && TAM != 12){
		return 0;
	}
	
    if (telefone[0] != '0'){
		return 0;
	}

    for(i = 0; i < TAM; i++) {
        if (telefone[i] < '0' || telefone[i] > '9'){
            return 0;
        }
    }
    return 1;  
}

int validarCartao(char numeroCartao[]){
	int soma = 0, i = 0, tam = strlen(numeroCartao), casa = 0, digito;
	
	if(tam < 13 || tam > 16){
        return 0;
    }

    for(i = 0; i < tam; i++){
        if(numeroCartao[i] < '0' || numeroCartao[i] > '9'){
            return 0;
        }
    }
	for(i = tam - 1; i >= 0; i--){
		digito = numeroCartao[i] - '0';

		if(casa == 1){
			digito *= 2;
		} if(digito > 9){
			digito -= 9;
		}

		soma += digito;

		if(casa == 0){
			casa = 1;
		} else {
			casa = 0; 
		}
	}

	if(soma % 10 == 0){
		return 1;
	} else {
		return 0;
	}
}

int validarEmail (char email[]) {
    int TAM = strlen(email);
    int psAR = -1,psPonto = -1, qtdAR = 0; 
	int i = 0;
	
    for (i = 0; i < TAM; i++) {
        if (email[i] == '@') {
            qtdAR++;
            psAR = i;
        }
    }

    if (qtdAR != 1 || psAR < 2) { return 0; }
    for (i = psAR + 1; i < TAM; i++) {
        if (email[i] == '.') {
        psPonto = i;
        break;
        }
    }

    if (psPonto == -1 || (psPonto - psAR - 1) < 2) { return 0; }
    if ((TAM - 1 - psPonto) < 2) { return 0; }

    return 1;
}

int registroVendas(){
    vendas venda;
    char cpfBuscar[20];
    int i = 0, indiceCliente = -1;

    if(qtdVendas >= 100){
        return 0;
    }

    printf("\nDigite o CPF: ");
    fgets(cpfBuscar, 20, stdin);

    cpfBuscar[strlen(cpfBuscar) - 1] = '\0';

    for (i = 0; i < qtdClientes; i++){
        if(strcmp(cpfBuscar, listaClientes[i].CPF) == 0){
            indiceCliente = i;
            break;
        }
    }
        
    if(indiceCliente == -1){
        printf("\nCliente nao encontrado");
        return 0;
    }

    venda.cliente = listaClientes[indiceCliente];
    venda.idVenda = proximoIdVenda++;
    
    printf("\nDigite o valor total da venda: ");
    scanf("%f", &venda.valorTotalVenda);

    printf("\nDigite a data da venda");
    
    do{
        printf("\nDia: ");
        scanf("%d", &venda.dataVenda.dia);
        printf("\nMes: ");
        scanf("%d", &venda.dataVenda.mes);
        printf("\nAno: ");
        scanf("%d", &venda.dataVenda.ano);
        
        if (validarData(venda.dataVenda) == 1) {
            break;
        }
        
        printf("\nData invalida.\n");
    }while(validarData(venda.dataVenda) == 0);
            
    printf("\nDigite qual foi a forma de pagamento: \n1) Dinheiro\n2) PIX\n3) Debito\n4) Parcelado\n");
    scanf("%d", &venda.formaPagamento);
    getchar();

    printf("\nDigite uma observacao: ");
    fgets(venda.observacao, 50, stdin);
    venda.observacao[strlen(venda.observacao) - 1] = '\0';

    if (venda.formaPagamento == 4) {
        int qtdValida = 0;
        do {
            printf("\nQuantidade de parcelas (maximo 10): ");
            scanf("%d", &venda.qtdParcelas);
            if (venda.qtdParcelas >= 1 && venda.qtdParcelas <= 10) {
                qtdValida = 1;
            } else {
                printf("Quantidade invalida.\n");
            }
        } while (qtdValida == 0);
    } else {
        venda.qtdParcelas = 1;
    }

    listaVendas[qtdVendas] = venda;
    qtdVendas++;

    geradorParcelas(venda.idVenda, venda.valorTotalVenda, venda.qtdParcelas, venda.dataVenda);

    printf("\nVenda %d registrada.\n", venda.idVenda);
    return 1;
}

void cadastroClientes(){
    int cpfValido = 0;
    int telefoneValido = 0;
    int dataNasciValido = 0;
    int cartaoValido = 0;
    int emailValido = 0;
    
    if (qtdClientes >= LISTA) {
        printf("\nLimite de clientes cadastrados atingido!\n");
        return;
    }

    Cliente novoCliente;

    do{
        printf("\nDigite o CPF: ");
        fgets(novoCliente.CPF, 20, stdin);

        novoCliente.CPF[strlen(novoCliente.CPF) - 1] = '\0';

        if(validarCPF(novoCliente.CPF) == 1){
            printf("\nCPF valido!\n");
            cpfValido = 1;
        } else {
            printf("\nCPF invalido!\n");
        }
    } while(cpfValido == 0);

    printf("\nDigite o nome: ");
    fgets(novoCliente.nome, 50, stdin);

    novoCliente.nome[strlen(novoCliente.nome) - 1] = '\0';

    do{
        printf("\nDigite o telefone: ");
        fgets(novoCliente.telefone, 15, stdin);

        novoCliente.telefone[strlen(novoCliente.telefone) - 1] = '\0';

        if(validarTelefone(novoCliente.telefone) == 1){
            printf("\nTelefone valido!\n");
            telefoneValido = 1;
        } else {
            printf("\nTelefone invalido!\n");
        }
    }while(telefoneValido == 0);

    do{
        printf("\nDigite o dia de seu nascimento: ");
        scanf("%d", &novoCliente.dataNascimento.dia);

        printf("\nDigite o mes de seu nascimento: ");
        scanf("%d", &novoCliente.dataNascimento.mes);

        printf("\nDigite o ano de seu nascimento: ");
        scanf("%d", &novoCliente.dataNascimento.ano);
        getchar();

        if(validarData(novoCliente.dataNascimento) == 1){
            printf("\nData valida!\n");
            dataNasciValido = 1;
        } else {
            printf("\nData invalida!\n");
        }
    }while(dataNasciValido == 0);

    do{
        printf("\nDigite o numero do cartao: ");
        fgets(novoCliente.numeroCartao, 20, stdin);

        novoCliente.numeroCartao[strlen(novoCliente.numeroCartao) - 1] = '\0';

        if(validarCartao(novoCliente.numeroCartao) == 1){
            printf("\nNumero do cartao valido!\n");
            cartaoValido = 1;
        } else {
            printf("\nNumero do cartao invalido!\n");
        }
    }while(cartaoValido == 0);

    do{
        printf("\nDigite a sua chave Pix (somente email): ");
        fgets(novoCliente.chavePix, 30, stdin);

        novoCliente.chavePix[strlen(novoCliente.chavePix) - 1] = '\0';

        if(validarEmail(novoCliente.chavePix) == 1){
            printf("\nEmail/Pix valido!\n");
            emailValido = 1;
        } else {
            printf("\nEmail invalido!\n");
        }
    }while(emailValido == 0);
	
	listaClientes[qtdClientes] = novoCliente;
    qtdClientes++;

    printf("\nCliente cadastrado.\n");
}

void buscarClientes(){
    int op, i = 0;
    char cpfBuscar[20];
    char nomeBuscar[50];
    char telefoneBuscar[15];
    int encontrado = 0;

    do{
        printf("\nDeseja buscar por qual meio?\n");
        printf("1) CPF\n");
        printf("2) Nome\n");
        printf("3) Telefone\n");
        printf("0) Retornar\n");

        scanf("%d", &op);
        getchar();

        switch(op)
        {
        case 1:
            printf("\nDigite o CPF: ");
            fgets(cpfBuscar, 20, stdin);

            cpfBuscar[strlen(cpfBuscar) - 1] = '\0';

            for(i = 0; i < qtdClientes; i++){
                if(strcmp(cpfBuscar, listaClientes[i].CPF) == 0){
                    encontrado = 1;
                    
                    printf("\nDados encontrados\n");
                    printf("Nome: %s\n", listaClientes[i].nome);
                    printf("CPF: %s\n", listaClientes[i].CPF);
                    printf("Telefone: %s\n", listaClientes[i].telefone);
                    
                    printf("Data de Nascimento: %02d/%02d/%d\n", 
                            listaClientes[i].dataNascimento.dia, 
                            listaClientes[i].dataNascimento.mes, 
                            listaClientes[i].dataNascimento.ano);
                            
                    printf("Numero do Cartao: %s\n", listaClientes[i].numeroCartao);
                    printf("Chave Pix / Email: %s\n", listaClientes[i].chavePix);
                }
            } 

            if(encontrado == 0){
                printf("\nCliente nao encontrado.");
            }

            encontrado = 0;
            break;

            case 2:
            printf("\nDigite o nome: ");
            fgets(nomeBuscar, 50, stdin);

            nomeBuscar[strlen(nomeBuscar) - 1] = '\0';

            for(i = 0; i < qtdClientes; i++){
                if(strcmp(nomeBuscar, listaClientes[i].nome) == 0){
                    encontrado = 1;
                    
                    printf("\nDados encontrados\n");
                    printf("Nome: %s\n", listaClientes[i].nome);
                    printf("CPF: %s\n", listaClientes[i].CPF);
                    printf("Telefone: %s\n", listaClientes[i].telefone);
                    
                    printf("Data de Nascimento: %02d/%02d/%d\n", 
                            listaClientes[i].dataNascimento.dia, 
                            listaClientes[i].dataNascimento.mes, 
                            listaClientes[i].dataNascimento.ano);
                            
                    printf("Numero do Cartao: %s\n", listaClientes[i].numeroCartao);
                    printf("Chave Pix / Email: %s\n", listaClientes[i].chavePix);
                }
            } 

            if(encontrado == 0){
                printf("\nCliente nÃƒÂ£o encontrado.");
            }
            
            encontrado = 0;
            break;

            case 3:
            printf("\nDigite o telefone: ");
            fgets(telefoneBuscar, 15, stdin);

            telefoneBuscar[strlen(telefoneBuscar) - 1] = '\0';

            for(i = 0; i < qtdClientes; i++){
                if(strcmp(telefoneBuscar, listaClientes[i].telefone) == 0){
                    encontrado = 1;
                    
                    printf("\nDados encontrados\n");
                    printf("Nome: %s\n", listaClientes[i].nome);
                    printf("CPF: %s\n", listaClientes[i].CPF);
                    printf("Telefone: %s\n", listaClientes[i].telefone);
                    
                    printf("Data de Nascimento: %02d/%02d/%d\n", 
                            listaClientes[i].dataNascimento.dia, 
                            listaClientes[i].dataNascimento.mes, 
                            listaClientes[i].dataNascimento.ano);
                            
                    printf("Numero do Cartao: %s\n", listaClientes[i].numeroCartao);
                    printf("Chave Pix / Email: %s\n", listaClientes[i].chavePix);
                }
            } 

            if(encontrado == 0){
                printf("\nCliente nao encontrado.");
            }
            
            encontrado = 0;
            break;

        default:
            break;
        }

    }while (op != 0);
}

data diasPorMes(data dataAtual) {
    dataAtual.mes += 1;
    if (dataAtual.mes > 12) {
        dataAtual.mes = 1;
        dataAtual.ano += 1;
    }
    return dataAtual;
}

int consultarVendas() {
    int i = 0, j = 0, op, idVendaBusca;
    char cpfBuscar[20];
    int indiceVendas = -1;

    do {
        printf("\nConsultar vendas:\n");
        printf("1) Buscar por CPF\n");
        printf("2) Buscar por ID da venda\n");
        printf("0) Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);
        getchar();

        switch (op) {
            case 1:
                printf("\nDigite o CPF: ");
                fgets(cpfBuscar, 20, stdin);
                cpfBuscar[strlen(cpfBuscar) - 1] = '\0';

                for (i = 0; i < qtdVendas; i++) {
                    if (strcmp(cpfBuscar, listaVendas[i].cliente.CPF) == 0) {
                        indiceVendas = i;

                        printf("\nDados da venda: \n");
                        printf("ID da Venda: %d\n", listaVendas[i].idVenda);
                        printf("Valor Total: R$ %.2f\n", listaVendas[i].valorTotalVenda);
                        printf("Forma de pagamento (1-Din, 2-Pix, 3-Deb, 4-Parc): %d\n", listaVendas[i].formaPagamento);
                        printf("Data da venda: %02d/%02d/%d\n", listaVendas[i].dataVenda.dia, listaVendas[i].dataVenda.mes, listaVendas[i].dataVenda.ano);

                        printf("\nParcelas da venda: %d\n", listaVendas[i].idVenda);
                        for (j = 0; j < qtdParcelas; j++) {
                            if (listaParcelas[j].idVenda == listaVendas[i].idVenda) {
                                printf("Parcela %d | Valor: R$ %.2f | Vencimento: %02d/%02d/%d | Recebimento: %02d/%02d/%d | Situacao: %c\n", 
                                    listaParcelas[j].numeroDaParcela,
                                    listaParcelas[j].valorDaParcela,
                                    listaParcelas[j].dataVencimento.dia, listaParcelas[j].dataVencimento.mes, listaParcelas[j].dataVencimento.ano,
                                    listaParcelas[j].dataRecebimento.dia, listaParcelas[j].dataRecebimento.mes, listaParcelas[j].dataRecebimento.ano,
                                    listaParcelas[j].situacaoDaParcela);
                            }
                        }
                    }
                }
                
                if (indiceVendas == -1) {
                    printf("\nNenhuma venda encontrada para este CPF.\n");
                }
                indiceVendas = -1;
                break;

            case 2:
                printf("\nDigite o ID da Venda: ");
                scanf("%d", &idVendaBusca);

                for (i = 0; i < qtdVendas; i++) {
                    if (idVendaBusca == listaVendas[i].idVenda) {
                        indiceVendas = i;

                        printf("\nDados da venda: \n");
                        printf("ID da Venda: %d\n", listaVendas[i].idVenda);
                        printf("Valor Total: R$ %.2f\n", listaVendas[i].valorTotalVenda);
                        printf("Forma de pagamento (1-Din, 2-Pix, 3-Deb, 4-Parc): %d\n", listaVendas[i].formaPagamento);
                        printf("Data da venda: %02d/%02d/%d\n", listaVendas[i].dataVenda.dia, listaVendas[i].dataVenda.mes, listaVendas[i].dataVenda.ano);

                        printf("\nParcelas da venda: %d\n", listaVendas[i].idVenda);
                        for (j = 0; j < qtdParcelas; j++) {
                            if (listaParcelas[j].idVenda == listaVendas[i].idVenda) {
                                printf("Parcela %d | Valor: R$ %.2f | Vencimento: %02d/%02d/%d | Recebimento: %02d/%02d/%d | Situacao: %c\n", 
                                    listaParcelas[j].numeroDaParcela,
                                    listaParcelas[j].valorDaParcela,
                                    listaParcelas[j].dataVencimento.dia, listaParcelas[j].dataVencimento.mes, listaParcelas[j].dataVencimento.ano,
                                    listaParcelas[j].dataRecebimento.dia, listaParcelas[j].dataRecebimento.mes, listaParcelas[j].dataRecebimento.ano,
                                    listaParcelas[j].situacaoDaParcela);
                            }
                        }
                        break;
                    }
                }
                
                if (indiceVendas == -1) {
                    printf("\nVenda nao encontrada.\n");
                }
                indiceVendas = -1;
                break;

            default:
                if (op != 0) {
                    printf("\nOpcao invalida!\n");
                }
                break;
        }

    } while (op != 0);
    
    return 1;
}

void geradorParcelas(int idVenda, float valorTotal, int qtd, data dataVenda) {
	Parcela p;
    data vencimento;
    float valorParcela;
    int i = 0;
    float valorBase;

	valorParcela = valorTotal / qtd;
	valorBase = valorParcela;
    vencimento = dataVenda;
	for (i = 0; i < qtd; i++) {
        if (qtdParcelas >= LIMITE_PARCELAS) {
            printf("Limite de parcelas atingido\n");
            return;
        	}
		
	vencimento = diasPorMes(vencimento);
        p.valorDaParcela = valorBase * pow(1.01, i + 1);
 		p.idParcela = proximoIdParcela++;
        p.idVenda  = idVenda;
        p.numeroDaParcela = i + 1;
        p.dataVencimento =  vencimento;
        p.dataRecebimento.dia = 0;
        p.dataRecebimento.mes = 0;
        p.dataRecebimento.ano = 0;
        p.situacaoDaParcela = 'A';
 			listaParcelas[qtdParcelas] = p;
        	qtdParcelas++;
    }
}


int dataAnterior(data d1, data d2) {
    if (d1.ano < d2.ano) return 1;
    if (d1.ano > d2.ano) return 0;
    if (d1.mes < d2.mes) return 1;
    if (d1.mes > d2.mes) return 0;
    if (d1.dia < d2.dia) return 1;
    return 0;
}

int dataParaDias(data d) {
    int i, dias = d.dia;   
    int meses[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    for (i = 1; i < d.ano; i++) {
        if ((i % 4 == 0 && i % 100 != 0) || (i % 400 == 0)) {
            dias += 366;
        } else {
            dias += 365;
        }
    }
    for (i = 1; i < d.mes; i++) {
        if (i == 2 && ((d.ano % 4 == 0 && d.ano % 100 != 0) || (d.ano % 400 == 0))) {
            dias += 29;
        } else {
            dias += meses[i];
        }
    }
    return dias;
}
int calcularDiferencaDias(data dInicio, data dFim) {     
    return dataParaDias(dFim) - dataParaDias(dInicio);
}


void identificarParcelasAtraso() {
    data dataAtual;
    int i, contador = 0;

    do {
        printf("\nDigite a data atual: ");
        scanf("%d %d %d", &dataAtual.dia, &dataAtual.mes, &dataAtual.ano);
        getchar();

        if (validarData(dataAtual) == 1) 
        break;
        
        printf("\nData invalida!\n");
        
    } while(1);

    for (i = 0; i < qtdParcelas; i++) {
        if (listaParcelas[i].situacaoDaParcela == 'A') {
            if (dataAnterior(listaParcelas[i].dataVencimento, dataAtual)) {
                listaParcelas[i].situacaoDaParcela = 'V';
                contador++;
            }
        }
    }
    printf("%d parcela(s) alterada(s) para 'Vencida' (V).\n", contador);
}

void quitarParcela() {
    int idVendaBuscar, i, proximaIdx = -1;
    float juros = 0.0;

    printf("\nQuitar a proxima parcela pendente\n");
    printf("Digite o ID da Venda: ");
    scanf("%d", &idVendaBuscar);
    getchar();

    int menorNumero = 1000;
    for (i = 0; i < qtdParcelas; i++) {
        if (listaParcelas[i].idVenda == idVendaBuscar && 
            (listaParcelas[i].situacaoDaParcela == 'A' || listaParcelas[i].situacaoDaParcela == 'V')) {
            if (listaParcelas[i].numeroDaParcela < menorNumero) {
                menorNumero = listaParcelas[i].numeroDaParcela;
                proximaIdx = i;
            }
        }
    }

    if (proximaIdx == -1) {
        printf("Nenhuma parcela pendente encontrada.\n");
        return;
    }

    printf("Parcela N: %d | Valor Base: %.2f | Status: %c\n", 
       listaParcelas[proximaIdx].numeroDaParcela, 
       listaParcelas[proximaIdx].valorDaParcela, 
       listaParcelas[proximaIdx].situacaoDaParcela);

    data dataPagamento;
    do {

        printf("Digite a data de pagamento: ");
        scanf("%d %d %d", &dataPagamento.dia, &dataPagamento.mes, &dataPagamento.ano);
        getchar();

        if (validarData(dataPagamento) == 1) 
        break;

        printf("Data invalida!\n");

    } while (1);

    float valorCobrado = listaParcelas[proximaIdx].valorDaParcela;
    
    if (listaParcelas[proximaIdx].situacaoDaParcela == 'V' || dataAnterior(listaParcelas[proximaIdx].dataVencimento, dataPagamento)) {
        
        int diasAtraso = calcularDiferencaDias(listaParcelas[proximaIdx].dataVencimento, dataPagamento);
        
        if (diasAtraso > 0) {
            juros = (listaParcelas[proximaIdx].valorDaParcela * 0.02) + (listaParcelas[proximaIdx].valorDaParcela * 0.0008 * diasAtraso);
            valorCobrado += juros;
            printf("Parcela com %d dia(s) de atraso.\n", diasAtraso);
            printf("Juros aplicados (2%% + 0.08%% ao dia): %.2f\n", juros);
        }
    }
    
    int conf;
    printf("Valor final a pagar: %.2f\n", valorCobrado);

    printf("Confirmar recebimento? (1-Sim / 0-Nao): ");
    scanf("%d", &conf);
    getchar();

    if (conf == 1) {
        listaParcelas[proximaIdx].situacaoDaParcela = 'R';
        listaParcelas[proximaIdx].dataRecebimento = dataPagamento;
        printf("Pagamento registrado.\n");
    } else {
        printf("Operacao cancelada.\n");
    }
}

void liquidarDivida(){
	char cpfBuscar[20];
	int k = 0, j = 0, i = 0, indiceCliente = -1, compraIdentificada = 0;
	int idVendaBusca, parcelasPendentes = 0, diasAtraso;
	float totalDivida = 0.0, juros = 0.0;
	int escolha;
	data dataPagamento;
	
	printf("\nDigite o CPF: ");
    fgets(cpfBuscar, 20, stdin);

    cpfBuscar[strlen(cpfBuscar) - 1] = '\0';
    
    for(i = 0; i < qtdClientes; i++){
		if(strcmp(cpfBuscar, listaClientes[i].CPF) == 0){
			indiceCliente = i;
			break;
		}
	}
	
  	if(indiceCliente == -1){
		printf("\nCliente nao encontrado");
		return;
	}
	
	printf("\nCompras vinculadas ao CPF");
	
	for(j = 0; j < qtdVendas; j++){
		if(strcmp(cpfBuscar, listaVendas[j].cliente.CPF) == 0){
			printf("\nID da Venda: %d | Valor: R$ %.2f | Data: %02d/%02d/%d", 
               listaVendas[j].idVenda, 
               listaVendas[j].valorTotalVenda,
               listaVendas[j].dataVenda.dia,
               listaVendas[j].dataVenda.mes,
               listaVendas[j].dataVenda.ano);
               
        	compraIdentificada = 1;	
		}
	}
	if(compraIdentificada == 0) {
    	printf("\nO cliente nao tem registro de compras");
    	return; 
	}
	
	printf("\nQual venda deseja liquidar?\n");
	scanf("%d", &idVendaBusca);
	getchar();
	
	identificarParcelasAtraso();
	
	printf("\nDigite a data de pagamento: ");
	scanf("%d %d %d", &dataPagamento.dia, &dataPagamento.mes, &dataPagamento.ano);
	getchar();

	
	for(k = 0; k < qtdParcelas; k++){
		if(listaParcelas[k].idVenda == idVendaBusca && (listaParcelas[k].situacaoDaParcela == 'A' || listaParcelas[k].situacaoDaParcela == 'V')){
			parcelasPendentes = 1;
			if(listaParcelas[k].situacaoDaParcela == 'V'){
				diasAtraso = calcularDiferencaDias(listaParcelas[k].dataVencimento, dataPagamento);
				
				if (diasAtraso > 0) {
					juros = (listaParcelas[k].valorDaParcela * 0.02) + (listaParcelas[k].valorDaParcela * 0.0008 * diasAtraso);
				}
				
				totalDivida += (listaParcelas[k].valorDaParcela + juros);
			}
			
			if(listaParcelas[k].situacaoDaParcela == 'A')
			{
				totalDivida += listaParcelas[k].valorDaParcela;
			}
		}
	}
	
	if(parcelasPendentes == 0){
		printf("\nSem parcelas pendentes");
		return;
	}
	
	printf("\nValor liquidado %d: R$ %.2f", idVendaBusca, totalDivida);
	printf("\nConfirmar o recibo?\n1) Sim\n2)Nao");
	scanf("%d", &escolha);
	getchar();

	if(escolha == 1){
        for(k = 0; k < qtdParcelas; k++){
            if(listaParcelas[k].idVenda == idVendaBusca && 
              (listaParcelas[k].situacaoDaParcela == 'A' || listaParcelas[k].situacaoDaParcela == 'V')){
                listaParcelas[k].situacaoDaParcela = 'R';
                listaParcelas[k].dataRecebimento = dataPagamento;
            }
        }
        
        printf("\nDivida liquidada.\n");
    } else {
        printf("\nOperacaoo cancelada.\n");
    }

}

void relatorioDiario(){
    data dataBusca;
    float totalVendido = 0.0;
    float totalRecebido = 0.0;
    float totalReceber = 0.0;
    float totalPendente = 0.0;
    int i = 0, j = 0;

    do{
        printf("\nDigite o dia: ");
        scanf("%d", &dataBusca.dia);
        printf("\nDigite o mes: ");
        scanf("%d", &dataBusca.mes);
        printf("\nDigite o ano: ");
        scanf("%d", &dataBusca.ano);


        if(validarData(dataBusca) == 1){
            break;
        }
        printf("\nData invalida.\n");

    }while(1);

    for(i = 0; i < qtdVendas; i++){
        if(dataBusca.dia == listaVendas[i].dataVenda.dia && 
           dataBusca.mes == listaVendas[i].dataVenda.mes && 
           dataBusca.ano == listaVendas[i].dataVenda.ano){
            
            totalVendido += listaVendas[i].valorTotalVenda;
        }
    }

    for(j = 0; j < qtdParcelas; j++){
        if(listaParcelas[j].situacaoDaParcela == 'R' && 
            listaParcelas[j].dataRecebimento.dia == dataBusca.dia && 
            listaParcelas[j].dataRecebimento.mes == dataBusca.mes && 
            listaParcelas[j].dataRecebimento.ano == dataBusca.ano){
            
                totalRecebido += listaParcelas[j].valorDaParcela;
        }

        if(listaParcelas[j].situacaoDaParcela == 'A' && 
            listaParcelas[j].dataVencimento.dia == dataBusca.dia && 
            listaParcelas[j].dataVencimento.mes == dataBusca.mes && 
            listaParcelas[j].dataVencimento.ano == dataBusca.ano){
            
                totalReceber += listaParcelas[j].valorDaParcela;
        }

        if(listaParcelas[j].situacaoDaParcela == 'V' && 
            listaParcelas[j].dataVencimento.dia == dataBusca.dia && 
            listaParcelas[j].dataVencimento.mes == dataBusca.mes && 
            listaParcelas[j].dataVencimento.ano == dataBusca.ano){
            
                totalPendente += listaParcelas[j].valorDaParcela;
        }
    }

    printf("Relatorio diario = %02d/%02d/%d\n", dataBusca.dia, dataBusca.mes, dataBusca.ano);
    printf("Vendas Realizadas: R$ %.2f\n", totalVendido);
    printf("Valores Recebidos: R$ %.2f\n", totalRecebido);
    printf("Valores a Receber: R$ %.2f\n", totalReceber);
    printf("Valores Pendentes: R$ %.2f\n", totalPendente);
}

void relatorioMensal(){
    data dataBusca;
    float totalVendido = 0.0;
    float totalRecebido = 0.0;
    float totalReceber = 0.0;
    float totalPendente = 0.0;
    int i = 0, j = 0;

    do{
        printf("\nDigite o mes: ");
        scanf("%d", &dataBusca.mes);
        printf("\nDigite o ano: ");
        scanf("%d", &dataBusca.ano);

        dataBusca.dia = 1;

        if(validarData(dataBusca) == 1){
            break;
        }

        printf("\nData invalida.\n");

    }while(1);

    for(i = 0; i < qtdVendas; i++){
        if(dataBusca.mes == listaVendas[i].dataVenda.mes && 
           dataBusca.ano == listaVendas[i].dataVenda.ano){
            
            totalVendido += listaVendas[i].valorTotalVenda;
        }
    }

    for(j = 0; j < qtdParcelas; j++){
        if(listaParcelas[j].situacaoDaParcela == 'R' && 
            listaParcelas[j].dataRecebimento.mes == dataBusca.mes && 
            listaParcelas[j].dataRecebimento.ano == dataBusca.ano){
            
                totalRecebido += listaParcelas[j].valorDaParcela;
        }

        if(listaParcelas[j].situacaoDaParcela == 'A' && 
            listaParcelas[j].dataVencimento.mes == dataBusca.mes && 
            listaParcelas[j].dataVencimento.ano == dataBusca.ano){
            
                totalReceber += listaParcelas[j].valorDaParcela;
        }

        if(listaParcelas[j].situacaoDaParcela == 'V' && 
            listaParcelas[j].dataVencimento.mes == dataBusca.mes && 
            listaParcelas[j].dataVencimento.ano == dataBusca.ano){
            
                totalPendente += listaParcelas[j].valorDaParcela;
        }
    }

    printf("\nRelatorio Mensal = %02d/%d", dataBusca.mes, dataBusca.ano);
    printf("\nVendas Realizadas: R$ %.2f", totalVendido);
    printf("\nValores Recebidos: R$ %.2f", totalRecebido);
    printf("\nValores a Receber: R$ %.2f", totalReceber);
    printf("\nValores Pendentes: R$ %.2f", totalPendente);
}

void dashboardFinanceiro(){
    int anoBusca, mes, i, j;
    float recebidoMes, aReceberMes, pendenteMes, vendidoMes;

    printf("\nDigite o ano para gerar o painel financeiro: ");
    scanf("%d", &anoBusca);
    getchar(); // Limpa o buffer do teclado (padrão do seu código)

    printf("\n==========================================================================\n");
    printf("                 DASHBOARD FINANCEIRO - ANO: %d\n", anoBusca);
    printf("==========================================================================\n");
    printf(" MÊS |   VENDIDO   |   RECEBIDO  |  A RECEBER  |  PENDENTE/ATRASADO \n");
    printf("==========================================================================\n");

    for (mes = 1; mes <= 12; mes++) {
        
        vendidoMes = 0.0;
        recebidoMes = 0.0;
        aReceberMes = 0.0;
        pendenteMes = 0.0;

        for (i = 0; i < qtdVendas; i++) {
            if (listaVendas[i].dataVenda.ano == anoBusca && listaVendas[i].dataVenda.mes == mes) {
                vendidoMes += listaVendas[i].valorTotalVenda;
            }
        }

        for (j = 0; j < qtdParcelas; j++) {
            
            if (listaParcelas[j].situacaoDaParcela == 'R' && 
                listaParcelas[j].dataRecebimento.ano == anoBusca && listaParcelas[j].dataRecebimento.mes == mes) {
                recebidoMes += listaParcelas[j].valorDaParcela;
            }

            if (listaParcelas[j].situacaoDaParcela == 'A' && 
                listaParcelas[j].dataVencimento.ano == anoBusca && listaParcelas[j].dataVencimento.mes == mes) {
                aReceberMes += listaParcelas[j].valorDaParcela;
            }

            if (listaParcelas[j].situacaoDaParcela == 'V' && 
                listaParcelas[j].dataVencimento.ano == anoBusca && listaParcelas[j].dataVencimento.mes == mes) {
                pendenteMes += listaParcelas[j].valorDaParcela;
            }
        }

        printf(" %02d  | R$ %9.2f | R$ %9.2f | R$ %9.2f | R$ %9.2f\n", mes, vendidoMes, recebidoMes, aReceberMes, pendenteMes);
    }
    
    printf("==========================================================================\n");
}
