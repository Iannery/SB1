/************************************
 * Trabalho 1 - Software Basico     *
 *                                  *
 * Ian Nery Bandeira                *
 * 170144739                        *
 *                                  *
 * Compilado em:                    *
 *                                  *
 ************************************/

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Montador.h"

using namespace std;

int main(int argc, char* argv[]) {
    string command = argv[1];
    string file = argv[2];
    // passa para o construtor do montador o nome do arquivo .asm
    Montador* montador = new Montador(file); 
    // passa para a inicializacao do montador o comando "-p" ou "-o" 
    montador->inicializar_processo(command);
	return 0;
}