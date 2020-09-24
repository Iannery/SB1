#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#define PC_START 1

Montador::Montador(string path_to_file){
    this->path = path_to_file;
    this->path_preprocess = this->path.substr(0, this->path.find("asm"))+"pre";
    this->path_mounted = this->path.substr(0, this->path.find("asm"))+"obj";
}

Montador::~Montador();

void Montador::inicializar_processo(string command){
    if(command == "-o"){
        this->preprocess();
    }
    else if(command == "-p"){
        this->mount();
    }
    else{
        cerr << "Erro no comando!" << endl;
    }
}