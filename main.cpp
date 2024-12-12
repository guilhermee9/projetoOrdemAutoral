/*
Autor:Guilherme Gonçalves da Silva
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

using namespace std;

// struct armazenar dados
struct OrdemServico {
    int codigo;
    string nome;
    string problema;   
    string solucao;
    string observacoes;
};


int gerarCodigo() { // gerar codigo, verificar e armazenar, como se fosse uma identidade no banco de dados
    ifstream arquivoCodigo("codigo.txt");
    int ultimoCodigo = 0;
    if (arquivoCodigo.is_open()) {
        arquivoCodigo >> ultimoCodigo;
        arquivoCodigo.close();
    }
    // incrementa ultimo codigo
    int novoCodigo = ultimoCodigo + 1;
    // armazena o ultimo codigo
    ofstream arquivoSaida("codigo.txt");
    arquivoSaida << novoCodigo;
    arquivoSaida.close();
    return novoCodigo;
}

// salva em arquivo
void salvarOrdens(const vector<OrdemServico>& ordens, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    if (arquivo.is_open()) {
        //organiza os itens e separa por ;
        for (const auto& ordem : ordens) {
            arquivo << ordem.codigo << ";" << ordem.nome << ";" << ordem.problema << ";" << ordem.solucao << ";" << ordem.observacoes << "\n";
        }
        arquivo.close();
    } else {
        cerr << "Erro ao abrir arquivo para salvar.\n";
    }
}

// funcao ler arquivo, separa cada campo por ; e verifica
void carregarOrdens(vector<OrdemServico>& ordens, const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (arquivo.is_open()) {
        string linha;
        while (getline(arquivo, linha)) {
            stringstream ss(linha);
            string token;
            vector<string> dados;
            while (getline(ss, token, ';')) {
                dados.push_back(token);
            }
            if (dados.size() == 5) {
                OrdemServico ordem;
                ordem.codigo = stoi(dados[0]);
                ordem.nome = dados[1];
                ordem.problema = dados[2];
                ordem.solucao = dados[3];
                ordem.observacoes = dados[4];
                ordens.push_back(ordem);
            }
        }
        arquivo.close();
    } else {
        cerr << "Erro ao abrir arquivo para carregar.\n";
    }
}

// lista as ordens de servico
void listarOrdens(const vector<OrdemServico>& ordens) {
    if (ordens.empty()) {
        cout << "Nenhuma ordem cadastrada.\n";
        return;
    }

    cout << "\n--- Ordens de Serviço ---\n";
    for (const auto& ordem : ordens) {
         cout << "----------------------\n";
        cout << "Código: " << ordem.codigo << "\n";
        cout << "Nome: " << ordem.nome << "\n";
        cout << "Problema: " << ordem.problema << "\n";
        cout << "Solução: " << ordem.solucao << "\n";
        cout << "Observações: " << ordem.observacoes << "\n";
    }
    cout << "----------------------\n";
}

// busca as os por nome, chama indice
vector<int> busca_recursiva_nome(const vector<OrdemServico>& ordens, const string& termoBusca, int indice = 0, vector<int> resultados = {}) {

    if (indice >= ordens.size()) {
        return resultados;
    }

    if (ordens[indice].nome.compare(termoBusca) == 0) {
        resultados.push_back(indice);
    }
    return busca_recursiva_nome(ordens, termoBusca, indice + 1, resultados);
}

// cadastra nova ordem
void cadastrarOrdem(vector<OrdemServico>& ordens) {
    OrdemServico novaOrdem;
    novaOrdem.codigo = gerarCodigo();

    cout << "Nome: ";
    getline(cin >> ws, novaOrdem.nome);
    cout << "Problema: ";
    getline(cin >> ws, novaOrdem.problema);
    cout << "Solução: ";
    getline(cin >> ws, novaOrdem.solucao);

    cout << "Observações (pressione Enter para pular): ";
    getline(cin, novaOrdem.observacoes);

    ordens.push_back(novaOrdem);
    cout << "Ordem cadastrada com código: " << novaOrdem.codigo << endl;
}

int main() {
    vector<OrdemServico> ordens;
    const string nomeArquivo = "ordens.txt";

    carregarOrdens(ordens, nomeArquivo); // carrega as ordens do arquivo

    int opcao;
    do {
        cout << "\n--- Menu ---\n";
        cout << "1. Cadastrar Ordem\n";
        cout << "2. Listar Ordens\n";
        cout << "3. Buscar Ordem por Nome\n";
        cout << "0. Sair\n";
        cout << "Opção: ";
        cin >> opcao;

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpeza buffer

        switch (opcao) {
            case 1:
                cadastrarOrdem(ordens);
                break;
            case 2:
                listarOrdens(ordens);
                break;
            case 3: {
                string termoBusca;
                cout << "Digite o nome para buscar: ";
                getline(cin >> ws, termoBusca);

                vector<int> indicesEncontrados = busca_recursiva_nome(ordens, termoBusca);

                if (indicesEncontrados.empty()) {
                    cout << "Nenhuma ordem encontrada com esse nome." << endl;
                } else {
                    cout << "Ordens encontradas:\n";
                    for (int indice : indicesEncontrados) {
                        cout << "----------------------\n";
                        cout << "Índice: " << indice << endl;
                        cout << "Código: " << ordens[indice].codigo << "\n";
                        cout << "Nome: " << ordens[indice].nome << "\n";
                        cout << "Problema: " << ordens[indice].problema << "\n";
                        cout << "Solução: " << ordens[indice].solucao << "\n";
                        cout << "Observações: " << ordens[indice].observacoes << "\n";
                    }
                    cout << "----------------------\n";
                }

                break;
            }
            case 0:
                cout << "Saindo...\n";
                break;
            default:
                cout << "Opção inválida!\n";
        }
        salvarOrdens(ordens, nomeArquivo); // salva as os
    } while (opcao != 0);

    return 0;
}