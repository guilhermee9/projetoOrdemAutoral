/*
Autor:Guilherme Gonçalves da Silva
Descrição: Sistema para cadastro de ordens de serviço, com indice sequencial, 
instruções para criar uma ordem com um codigo, nome, problema, solucao e alguma observacao quando aplicavel.
No projeto foi utilizado structs para manipular as informações, manipulacao de arquivos
para manipular as informacoes em txt, recursividade na função "listarOrdens".
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

// struct armazenar dados
struct OrdemServico {
    int codigo;
    string nome;
    string problema;
    string solucao;
    string observacoes;
};

// gerar codigo, verificar e armazenar, como se fosse uma identidade no banco de dados
int gerarCodigo() {
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
void salvarOrdens(const vector<OrdemServico*>& ordens, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    if (arquivo.is_open()) {
        //organiza os itens e separa por ;
        for (const auto& ordem : ordens) {
            arquivo << ordem->codigo << ";" << ordem->nome << ";" << ordem->problema << ";" << ordem->solucao << ";" << ordem->observacoes << "\n";
        }
        arquivo.close();
    } else {
        cerr << "Erro ao abrir arquivo para salvar.\n";
    }
}

// funcao ler arquivo, separa cada campo por ; e verifica, faz a alocao dinamica
void carregarOrdens(vector<OrdemServico*>& ordens, const string& nomeArquivo) {
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
                OrdemServico* ordem = new OrdemServico;
                ordem->codigo = stoi(dados[0]);
                ordem->nome = dados[1];
                ordem->problema = dados[2];
                ordem->solucao = dados[3];
                ordem->observacoes = dados[4];
                ordens.push_back(ordem);
            }
        }
        arquivo.close();
    } else {
        cerr << "Erro ao abrir arquivo para carregar.\n";
    }
}

// lista as ordens de servico
void listarOrdens(const vector<OrdemServico*>& ordens) {
     if (ordens.empty()) {
        cout << "Nenhuma ordem cadastrada.\n";
        return;
    }

    cout << "\n--- Ordens de Serviço ---\n";
    for (const auto& ordem : ordens) {
        cout << "----------------------\n";
        cout << "Código: " << ordem->codigo << "\n";
        cout << "Nome: " << ordem->nome << "\n";
        cout << "Problema: " << ordem->problema << "\n";
        cout << "Solução: " << ordem->solucao << "\n";
        cout << "Observações: " << ordem->observacoes << "\n";
    }
    cout << "----------------------\n";
}

// busca as os por nome, chama indice
vector<int> busca_recursiva_nome(const vector<OrdemServico*>& ordens, const string& termoBusca, int indice = 0, vector<int> resultados = {}) {
    if (indice >= ordens.size()) {
        return resultados;
    }

    if (ordens[indice]->nome.compare(termoBusca) == 0) {
        resultados.push_back(indice);
    }
    return busca_recursiva_nome(ordens, termoBusca, indice + 1, resultados);
}

// cadastra nova ordem
void cadastrarOrdem(vector<OrdemServico*>& ordens) {
    OrdemServico* novaOrdem = new OrdemServico;
    novaOrdem->codigo = gerarCodigo();

    cout << "Nome: ";
    getline(cin >> ws, novaOrdem->nome);
    cout << "Problema: ";
    getline(cin >> ws, novaOrdem->problema);
    cout << "Solução: ";
    getline(cin >> ws, novaOrdem->solucao);

    cout << "Observações (pressione Enter para pular): ";
    getline(cin, novaOrdem->observacoes);

    ordens.push_back(novaOrdem);
    cout << "Ordem cadastrada com código: " << novaOrdem->codigo << endl;
}

int main() {
    // usa ponteiros para menipular
    vector<OrdemServico*> ordens; 
    const string nomeArquivo = "ordens.txt";

    carregarOrdens(ordens, nomeArquivo);// carrega as ordens do arquivo

    // array limita 100 resultados
    const int MAX_RESULTADOS = 100;
    int indicesEncontradosArray[MAX_RESULTADOS];
    int numResultados = 0;

    int opcao;
    do {
        cout << "\n--- Menu ---\n";
        cout << "1. Cadastrar Ordem\n";
        cout << "2. Listar Ordens\n";
        cout << "3. Buscar Ordem por Nome\n";
        cout << "0. Sair\n";
        cout << "Opção: ";
        cin >> opcao;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');//limpa memoria

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

                numResultados = min((int)indicesEncontrados.size(), MAX_RESULTADOS);
                for (int i = 0; i < numResultados; ++i) {
                    indicesEncontradosArray[i] = indicesEncontrados[i];
                }
                
                if (numResultados == 0) {
                    cout << "Nenhuma ordem encontrada com esse nome." << endl;
                } else {
                    cout << "Ordens encontradas:\n";
                    for (int i = 0; i < numResultados; ++i) { 
                        int indice = indicesEncontradosArray[i];
                        cout << "----------------------\n";
                        cout << "Índice: " << indice << endl;
                        cout << "Código: " << ordens[indice]->codigo << "\n";
                        cout << "Nome: " << ordens[indice]->nome << "\n";
                        cout << "Problema: " << ordens[indice]->problema << "\n";
                        cout << "Solução: " << ordens[indice]->solucao << "\n";
                        cout << "Observações: " << ordens[indice]->observacoes << "\n";
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
        salvarOrdens(ordens, nomeArquivo);

    } while (opcao != 0);

     for (OrdemServico* ordem : ordens) {
        delete ordem;
    }
    ordens.clear();

    return 0;
}