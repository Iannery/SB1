#include <stdio.h>

float alo(float peso, float altura){
    float imc = peso / (altura*altura);
    return imc;
}

int main(){
    float altura = 0,  peso = 0;
    printf("Informe seu peso(em Kg): ");
    scanf("%f", &peso);
    printf("Informe sua altura(em m): ");
    scanf("%f", &altura);

    float imc = alo(peso, altura);

    printf("%f\n", &imc);
}