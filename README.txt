/****************************************
 * Trabalho 1 - Software Basico         *
 *                                      *
 * Ian Nery Bandeira                    *
 * 170144739                            *
 *                                      *
 * Versao do compilador:                *
 * g++ (Ubuntu 9.3.0-10ubuntu2) 9.3.0   *
 *                                      *
 ****************************************/

O trabalho consiste de um Programa que simula um montador, fazendo tanto o pré-processamento quanto a montagem do arquivo .asm.
O resultado da execução do pré-processamento e montagem é visto nos arquivos .pre e .obj, respectivamente.
Os comandos necessários são:

- Para compilar o programa:
g++ main.cpp Montador.cpp -o montador

- Para pré-processar o arquivo .asm com o programa, gerando o arquivo .pre:
./montador -p myprogram.asm

- Para montar o arquivo .pre com o programa, gerando o arquivo .obj:
./montador -o myprogram.pre