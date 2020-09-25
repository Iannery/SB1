#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include "Montador.h"
using namespace std;

#define PC_START 1

Montador::Montador(string asm_path_to_file){
    this->asm_path = asm_path_to_file;
    this->preprocessed_path = this->asm_path.substr(0, this->asm_path.find("asm"))+"pre";
    this->mounted_path = this->asm_path.substr(0, this->asm_path.find("asm"))+"obj";
}

void Montador::inicializar_processo(string command){
    if(command == "-o"){
        this->mount();
    }
    else if(command == "-p"){
        this->preprocess();
    }
    else{
        cerr << "Erro no comando!" << endl;
    }
}

void Montador::preprocess(){
    ifstream asm_file;
    ofstream preprocessed_file;

    asm_file.open(this->asm_path);
    preprocessed_file.open(this->preprocessed_path);
    if(!asm_file.is_open()){
        cerr << "Erro na abertura do arquivo .asm";
    } 
    else if(!preprocessed_file.is_open()){
        cerr << "Erro na abertura do arquivo .pre";
    }
    else{
        while(asm_file.good()){
            //PEGA LINHA
            getline(asm_file, this->line);
            //RETIRA COMENTÁRIOS
            this->line = this->line.substr(0, this->line.find(";"));
            //COLOCA TODOS OS CHAR COM MAIUSCULA
            transform(this->line.begin(), 
            this->line.end(), 
            this->line.begin(), 
            ::toupper);
            if(!this->line.empty()){
                preprocessed_file << this->line + "\n";
            }
        }
        asm_file.close();
        preprocessed_file.close();
        macro_handler();
    }
}

void Montador::macro_handler(){
    int macro_count = 0;
    ifstream preprocessed_file;
    string macro_label1, macro_label2, macro_command;
    vector<string> macro_command_list1, macro_command_list2;

    preprocessed_file.open(this->preprocessed_path);
    if(preprocessed_file.is_open()){
        while(preprocessed_file.good()){
            getline(preprocessed_file, this->line);
            // ROTINA PARA CASO ACHE A(S) DIRETIVA(S) DE MACRO
            if(this->line.find("MACRO") != std::string::npos){
                macro_count++;
                if(macro_count == 1){
                    macro_label1 = this->line.substr(0, this->line.find(":"));
                    while(this->line.find("ENDMACRO") == std::string::npos){
                        getline(preprocessed_file, macro_command);
                        macro_command_list1.push_back(macro_command);
                    }
                }
                else if(macro_count == 2){
                    macro_label2 = this->line.substr(0, this->line.find(":"));
                    while(this->line.find("ENDMACRO") == std::string::npos){
                        getline(preprocessed_file, macro_command);
                        macro_command_list2.push_back(macro_command);
                    }
                }
                else{
                    cerr << "Existem mais macros do que o programa consegue lidar" << endl;
                }
            }
        }
    }
}

void Montador::mount(){
    cout << "SoonTM" << endl;
}