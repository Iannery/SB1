#ifndef TRABALHO1_ASSEMBLER_H
#define TRABALHO1_ASSEMBLER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TabelaDeSimbolos.h"

using namespace std;

class Assembler {
public:
    Assembler(string caminho_do_arquivo);     // Construtor
    ~Assembler();                  // Destrutor
    void assemble();               // Funcao que monta o programa

private:
    void passagemZero();     // Funcao para leitura do codigo fonte
    void padrao();           // Funcao para retirar comentarios e ignorar letras maiusculas/minusculas
    void criaListas();		 // Funcao para criar listas de OPcode e Diretivas de maneira vector de strings
    void passagemUm();       // Funcao que separa inicia a primeira passagem de fato
    void checaMneumonico(int *posicaotabela);     // Funcao para tratamento de erros na linha
    void trataErros(int *posicao_da_palavra, int n_operandos);     // Funcao que trata dos erros presentes do codigo pre-processado
	string hexad(string);
    vector<string> lista_de_opcode;
    vector<string> lista_de_diretivas;
    vector<string> lista_temp;
    vector<string> lista_temp_valor;
    vector<string> vetor_palavras;
    vector<string> opcodes;

    TabelaDeSimbolos* tabela_de_simbolos;
    string file_path, file_pre, file_obj, line, saida, apoio;
    int text_field_start, data_field_start, pc_codigo_fonte, pc_pre_processado,
            flag_salva_linha, linha_coluna_contador;
    bool section_text_present = false;
};


#endif //TRABALHO1_ASSEMBLER_H