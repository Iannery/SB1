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

    Montador* montador = new Montador(file);
    montador->inicializar_processo(command);
	return 0;
}