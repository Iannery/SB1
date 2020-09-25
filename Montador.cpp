#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include "Montador.h"
using namespace std;

#define PC_START 1

Montador::Montador(string asm_path_to_file)
{
    this->asm_path = asm_path_to_file;
    this->preprocessed_path = this->asm_path.substr(0, this->asm_path.find("asm")) + "pre";
    this->mounted_path = this->asm_path.substr(0, this->asm_path.find("asm")) + "obj";
}

void Montador::inicializar_processo(string command)
{
    if (command == "-o")
    {
        this->mount();
    }
    else if (command == "-p")
    {
        this->preprocess();
    }
    else
    {
        cerr << "Erro no comando!" << endl;
    }
}

void Montador::preprocess()
{
    ifstream asm_file;
    ofstream preprocessed_file;

    asm_file.open(this->asm_path);
    preprocessed_file.open(this->preprocessed_path);
    if (!asm_file.is_open())
    {
        cerr << "Erro na abertura do arquivo .asm";
    }
    else if (!preprocessed_file.is_open())
    {
        cerr << "Erro na abertura do arquivo .pre";
    }
    else
    {
        while (asm_file.good())
        {
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
            if (!this->line.empty())
            {
                preprocessed_file << this->line + "\n";
            }
        }
        asm_file.close();
        preprocessed_file.close();
        macro_handler();
    }
}

void Montador::macro_identifier()
{
    int macro_count = 0;
    ifstream preprocessed_file;

    preprocessed_file.open(this->preprocessed_path);
    if (preprocessed_file.is_open())
    {
        while (preprocessed_file.good())
        {
            getline(preprocessed_file, this->line);
            // ROTINA PARA CASO ACHE A(S) DIRETIVA(S) DE MACRO
            // ta feio e mal optimizado, mas funciona!!
            // o ian do futuro vai arrumar, eu tenho certeza
            if (this->line.find("MACRO") != std::string::npos)
            {
                macro_count++;
                if (macro_count == 1)
                {
                    this->macro_label1 = this->line.substr(0, this->line.find(":"));

                    while (this->line.find("ENDMACRO") == std::string::npos)
                    {
                        getline(preprocessed_file, this->line);
                        this->macro_command = this->line;
                        this->macro_command_list1.push_back(this->macro_command);
                    }
                    this->macro_command_list1.pop_back();
                }
                else if (macro_count == 2)
                {
                    this->macro_label2 = this->line.substr(0, this->line.find(":"));
                    while (this->line.find("ENDMACRO") == std::string::npos)
                    {
                        getline(preprocessed_file, this->line);
                        this->macro_command = this->line;
                        this->macro_command_list2.push_back(this->macro_command);
                    }
                    this->macro_command_list2.pop_back();
                }
                else
                {
                    cerr << "Existem mais do que dois macros no programa." << endl;
                }
            }
        }
    }
    preprocessed_file.close();
}

void Montador::macro_expander()
{
    int macro_position_begin, macro_position_end, macro_begin_flag = 0;
    ifstream preprocessed_file_in;
    ofstream preprocessed_file_out;
    string command_line;
    vector<string> command_list;
    preprocessed_file_in.open(this->preprocessed_path);
    if (preprocessed_file_in.is_open())
    {
        while (preprocessed_file_in.good())
        {
            getline(preprocessed_file_in, this->line);
            if(!this->line.empty()){
                command_list.push_back(this->line);
            }
        }
        preprocessed_file_in.close();
        for (int i = 0; i < command_list.size(); i++)
        {
            command_line = command_list.at(i);
            // RETIRA AS DIRETIVAS DE MACRO DO VETOR PARA PASSAR PARA O ARQUIVO
            if (command_line.find("MACRO") != std::string::npos && !macro_begin_flag)
            {
                macro_position_begin = i;
                macro_begin_flag = 1;
            }
            else if (command_line.find("ENDMACRO") != std::string::npos)
            {
                macro_position_end = i;
                command_list.erase(
                    command_list.begin() + macro_position_begin,
                    command_list.begin() + macro_position_end + 1);
                macro_begin_flag = 0;
            }
        }
        for (int i = 0; i < command_list.size(); i++)
        {
            command_line = command_list.at(i);
            // EXPANDE AS MACROS DA LABEL 1
            if (command_line.find(this->macro_label1) != std::string::npos && command_line.find(":") == std::string::npos)
            {
                command_list.erase(command_list.begin() + i);
                command_list.insert(
                    command_list.begin() + i,
                    this->macro_command_list1.begin(),
                    this->macro_command_list1.end());
            }
        }

        remove(this->preprocessed_path.c_str());
        preprocessed_file_out.open(this->preprocessed_path);

        for (int i = 0; i < command_list.size(); i++){
            preprocessed_file_out << command_list.at(i) << "\n";
        }
        preprocessed_file_out.close();
    }
}

void Montador::macro_handler()
{
    macro_identifier();
    macro_expander();
}

void Montador::mount()
{
    cout << "SoonTM" << endl;
}