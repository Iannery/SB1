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
    this->preprocessed_path = this->asm_path.substr(0, this->asm_path.find("asm")) + "pre";
    this->mounted_path = this->asm_path.substr(0, this->asm_path.find("asm")) + "obj";
    this->macro_label1  = "";
    this->macro_label2  = "";
    this->macro_command = "";
    this->macro1_arg1   = "";
    this->macro1_arg2   = "";
    this->macro2_arg1   = "";
    this->macro2_arg2   = "";
    
}

void Montador::inicializar_processo(string command){
    if (command == "-o"){
        this->mount();
    }
    else if (command == "-p"){
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
    if (!asm_file.is_open()){
        cerr << "Erro na abertura do arquivo .asm";
    }
    else if (!preprocessed_file.is_open()){
        cerr << "Erro na abertura do arquivo .pre";
    }
    else{
        while (asm_file.good()){
            //PEGA LINHA
            getline(asm_file, this->line);
            //RETIRA COMENTÁRIOS
            this->line = this->line.substr(0, this->line.find(";"));
            //COLOCA TODOS OS CHAR COM MAIUSCULA
            transform(
                this->line.begin(),
                this->line.end(),
                this->line.begin(),
                ::toupper);
            if (!this->line.empty()){
                preprocessed_file << this->line + "\n";
            }
        }
        asm_file.close();
        preprocessed_file.close();
        macro_handler();
    }
}

void Montador::macro_argument_finder(string declaration_line, int macro_count){
    int macro_arg1_pos = 0, 
        macro_arg2_pos = 0, 
        occurrences = 0;
    string arg_substr;
    arg_substr = declaration_line.substr(declaration_line.find("MACRO") + 5, declaration_line.length());
    if(arg_substr.length() > 1){
        for(size_t i = 1; i < arg_substr.length(); i++){
            if(arg_substr.at(i) == '&'){
                occurrences++;
                if(occurrences == 1){
                    macro_arg1_pos = i;
                }
                else if(occurrences == 2){
                    macro_arg2_pos = i;
                }
            }
        }
        if(macro_count == 1){
            switch(occurrences){
                case 1:
                    this->macro1_arg1 = arg_substr.substr(macro_arg1_pos, arg_substr.length());
                    break;
                case 2:
                    this->macro1_arg1 = arg_substr.substr(macro_arg1_pos, macro_arg2_pos - 2);
                    this->macro1_arg2 = arg_substr.substr(macro_arg2_pos, arg_substr.length());
                    break;
            }
            // cout << this->macro1_arg1 << "aaa\t" << this->macro1_arg2 << "bbb\t" << endl;
        }
        else if(macro_count == 2){

            switch(occurrences){
                case 1:
                    this->macro2_arg1 = arg_substr.substr(macro_arg1_pos, arg_substr.length());
                    break;
                case 2:
                    this->macro2_arg1 = arg_substr.substr(macro_arg1_pos, macro_arg2_pos - 2);
                    this->macro2_arg2 = arg_substr.substr(macro_arg2_pos, arg_substr.length());
                    break;
            }
            // cout << this->macro2_arg1 << "\t" << this->macro2_arg2 << endl;
        }
    }
}

void Montador::macro_identifier(){
    int macro_count = 0;
    ifstream preprocessed_file;
    preprocessed_file.open(this->preprocessed_path);
    if (preprocessed_file.is_open()){
        while (preprocessed_file.good()){
            getline(preprocessed_file, this->line);

            if (this->line.find("MACRO") != std::string::npos){
                macro_count++;
                if (macro_count == 1){
                    macro_argument_finder(this->line, macro_count);
                    this->macro_label1 = this->line.substr(0, this->line.find(":"));

                    while (this->line.find("ENDMACRO") == std::string::npos){
                        getline(preprocessed_file, this->line);
                        this->macro_command = this->line;
                        this->macro_command_list1.push_back(this->macro_command);
                    }
                    this->macro_command_list1.pop_back();
                }
                else if (macro_count == 2){
                    macro_argument_finder(this->line, macro_count);
                    this->macro_label2 = this->line.substr(0, this->line.find(":"));
                    while (this->line.find("ENDMACRO") == std::string::npos){
                        getline(preprocessed_file, this->line);
                        this->macro_command = this->line;
                        this->macro_command_list2.push_back(this->macro_command);
                    }
                    this->macro_command_list2.pop_back();
                }
                else{
                    cerr << "Existem mais do que dois macros no programa." << endl;
                }
            }
        }
    }
    preprocessed_file.close();
}

void Montador::macro_expander(){
    int macro_position_begin, macro_position_end, macro_begin_flag = 0, macro_label_position;
    ifstream preprocessed_file_in;
    ofstream preprocessed_file_out;
    string command_line, macro_label_arg1 = "", macro_label_arg2 = "";
    vector<string> command_list, macro_command_list1_local, macro_command_list2_local;
    preprocessed_file_in.open(this->preprocessed_path);
    if (preprocessed_file_in.is_open()){
        while (preprocessed_file_in.good()){
            getline(preprocessed_file_in, this->line);
            if(!this->line.empty()){
                command_list.push_back(this->line);
            }
        }
        preprocessed_file_in.close();
        for (size_t i = 0; i < command_list.size(); i++){
            command_line = command_list.at(i);
            // RETIRA AS DIRETIVAS DE MACRO DO VETOR PARA PASSAR PARA O ARQUIVO
            if (command_line.find("MACRO") != std::string::npos 
            && !macro_begin_flag){
                macro_position_begin = i;
                macro_begin_flag = 1;
            }
            else if (command_line.find("ENDMACRO") != std::string::npos){
                macro_position_end = i;
                command_list.erase(
                    command_list.begin() + macro_position_begin,
                    command_list.begin() + macro_position_end + 1);
                macro_begin_flag = 0;
                i = 0;
            }
        }
        for (size_t i = 0; i < command_list.size(); i++){
            command_line = command_list.at(i);
            // EXPANDE AS MACROS DA LABEL 1
            if (command_line.substr(0, command_line.find(" ")) == this->macro_label1
            && command_line.find(":") == std::string::npos
            && !this->macro_label1.empty()){
                macro_label_position = this->macro_label1.length() + 1;
                if(!macro1_arg2.empty()){
                    macro_label_arg1 = command_line.substr(macro_label_position, command_line.find(" "));
                    macro_label_arg2 = macro_label_arg1.substr(macro_label_arg1.find(" ") + 1, macro_label_arg1.length());
                    macro_label_arg1 = macro_label_arg1.substr(0, macro_label_arg1.find(" "));
                    macro_command_list1_local = this->macro_command_list1;
                    for(size_t j = 0; j < macro_command_list1_local.size(); j++){
                        if(macro_command_list1_local.at(j).find(macro1_arg1) != std::string::npos){
                            macro_command_list1_local.at(j).replace(
                                macro_command_list1_local.at(j).find(macro1_arg1),
                                macro1_arg1.length(),
                                macro_label_arg1
                            );
                        }
                        if(macro_command_list1_local.at(j).find(macro1_arg2) != std::string::npos){
                            macro_command_list1_local.at(j).replace(
                                macro_command_list1_local.at(j).find(macro1_arg2),
                                macro1_arg2.length(),
                                macro_label_arg2
                            );
                        }
                        // cout << macro_command_list1_local.at(j) << endl;
                    }
                }
                else if(!macro1_arg1.empty()){
                    macro_label_arg1 = command_line.substr(macro_label_position, command_line.length());
                    macro_command_list1_local = this->macro_command_list1;
                    for(size_t j = 0; j < macro_command_list1_local.size(); j++){
                        if(macro_command_list1_local.at(j).find(macro1_arg1) != std::string::npos){
                            macro_command_list1_local.at(j).replace(
                                macro_command_list1_local.at(j).find(macro1_arg1),
                                macro1_arg1.length(),
                                macro_label_arg1
                            );
                        }
                        // cout << macro_command_list1_local.at(j) << endl;
                    }
                }
                command_list.erase(command_list.begin() + i);
                command_list.insert(
                    command_list.begin() + i,
                    macro_command_list1_local.begin(),
                    macro_command_list1_local.end()
                );
            }

            //EXPANDE AS MACROS DA LABEL 2
            if (command_line.substr(0, command_line.find(" ")) == this->macro_label2
            && command_line.find(":") == std::string::npos
            && !this->macro_label2.empty()){
                macro_label_position = this->macro_label2.length() + 1;
                if(!macro2_arg2.empty()){
                    macro_label_arg1 = command_line.substr(macro_label_position, command_line.find(" "));
                    macro_label_arg2 = macro_label_arg1.substr(macro_label_arg1.find(" ") + 1, macro_label_arg1.length());
                    macro_label_arg1 = macro_label_arg1.substr(0, macro_label_arg1.find(" "));
                    macro_command_list2_local = this->macro_command_list2;
                    for(size_t j = 0; j < macro_command_list1_local.size(); j++){
                        if(macro_command_list2_local.at(j).find(macro2_arg1) != std::string::npos){
                            macro_command_list2_local.at(j).replace(
                                macro_command_list2_local.at(j).find(macro2_arg1),
                                macro2_arg1.length(),
                                macro_label_arg1
                            );
                        }
                        if(macro_command_list2_local.at(j).find(macro2_arg2) != std::string::npos){
                            macro_command_list2_local.at(j).replace(
                                macro_command_list2_local.at(j).find(macro2_arg2),
                                macro2_arg2.length(),
                                macro_label_arg2
                            );
                        }
                        cout << macro_command_list2_local.at(j) << endl;
                    }
                }
                else if(!macro2_arg1.empty()){
                    macro_label_arg1 = command_line.substr(macro_label_position, command_line.length());
                    macro_command_list2_local = this->macro_command_list2;
                    for(size_t j = 0; j < macro_command_list2_local.size(); j++){
                        if(macro_command_list2_local.at(j).find(macro2_arg1) != std::string::npos){
                            macro_command_list2_local.at(j).replace(
                                macro_command_list2_local.at(j).find(macro2_arg1),
                                macro2_arg1.length(),
                                macro_label_arg1
                            );
                        }
                        cout << macro_command_list2_local.at(j) << endl;
                    }
                }
                command_list.erase(command_list.begin() + i);
                command_list.insert(
                    command_list.begin() + i,
                    macro_command_list2_local.begin(),
                    macro_command_list2_local.end()
                );
            }
        }

        remove(this->preprocessed_path.c_str());
        preprocessed_file_out.open(this->preprocessed_path);

        for (size_t i = 0; i < command_list.size(); i++){
            preprocessed_file_out << command_list.at(i) << "\n";
        }
        preprocessed_file_out.close();
    }
}

void Montador::macro_handler(){
    macro_identifier();
    macro_expander();
}

void Montador::mount(){
    cout << "SoonTM" << endl;
}