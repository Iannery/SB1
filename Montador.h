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
    void macro_identifier();
    void macro_expander();
    string asm_path, preprocessed_path, mounted_path, line;

    vector<string> macro_command_list1, macro_command_list2;
    string macro_label1, macro_label2, macro_command;
};


#endif //MONTADOR_BIB