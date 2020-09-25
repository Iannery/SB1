#ifndef MONTADOR_BIB
#define MONTADOR_BIB

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Montador {
public:
    Montador(string asm_path_to_file);
    ~Montador();
    void inicializar_processo(string command);

private:
    void preprocess();
    void mount();
    void macro_handler();
    string asm_path, preprocessed_path, mounted_path, line;
};


#endif //MONTADOR_BIB