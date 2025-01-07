/*
BY: nicolasjoannas14@gmail.com

El siguiente código responde a la siguiente consigna:
    "Una computadora comienza imprimiendo los números 2023, 2024 y 2025.
    Luego continúa imprimiendo sin parar la suma de los últimos 3 números que imprimió: 6072, 10121, 18218, …
    ¿Cuáles son los últimos 4 dígitos del número impreso en la posición 2023202320232023?"

Este problema es similar a un clásico desafío de programación que se utiliza para aprender recursión: la sucesión de Fibonacci. Sin embargo, tiene la sutileza
de que se suman los 3 números anteriores, no 2, y que no se nos pregunta sobre el resultado final concretamente, sino sobre sus últimos 4 dígitos. Un detalle no menor,
es que el número a ser calculado es increíblemente grande (2023202320232023, número que llamaré "N", y que es mayor a 10^16).
Esto puede resultar problemático si quisiéramos, por ejemplo, memorizar todos los valores calculados en cada recursión (El array podría ser demasiado grande).

Podemos definir la función del enunciado como: f(n) = f(n-1) + f(n-2) + f(n-3). Se me pide calcular f(N) mod 10000 (Pues sólo necesito los últimos 4 dígitos).

Analizando la función f, se deduce lo siguiente:

f(n-1) = f(n-2) + f(n-3) + f(n-4) por lo que f(n) = 2f(n-2) + 2f(n-3) + f(n-4)

f(n-2) = f(n-3) + f(n-4) + f(n-5) por lo que f(n) = 4f(n-3) + 3f(n-4) + 2f(n-5)

f(n-3) = f(n-4) + f(n-5) + f(n-6) por lo que f(n) = 7f(n-4) + 6f(n-5) + 4f(n-6)

f(n-4) = f(n-5) + f(n-6) + f(n-7) por lo que f(n) = 13f(n-5) + 11f(n-6) + 7f(n-7)

Eventualmente, operando de manera análoga, llegaremos a una expresión como esta:

f(n) = xf(1) + yf(2) + zf(3) = x2023 + y2024 + z2025 donde x,y,z son números naturales

Es decir, existen 3 coeficientes que al multiplicarlos por los 3 casos base, nos permiten llegar rápidamente al resultado final. Sin embargo, primero veamos qué
exponente se necesita en cada término de f(n) para calcular f(n), f(n-1) y f(n-2):
*   f(n)   = f(n-1) + f(n-2) + f(n-3) sólo si uso los 3 términos ([1, 1, 1])
*   f(n-1) = f(n-1) + f(n-2) + f(n-3) sólo si uso el primer término ([1, 0, 0])
*   f(n-2) = f(n-1) + f(n-2) + f(n-3) sólo si uso el segundo término ([0, 1, 0])

Al rearmarlos como una matriz de 3x3, obtengo que:
matriz = [[1, 1, 1], [1, 0, 0], [0, 1, 0]]
¿Para qué hice esto? Para encontrar los exponentes anteriormente mencionados usando exponenciación de matrices:

matriz = [[1, 1, 1], [1, 0, 0], [0, 1, 0]]
ultimos3 = [f(n), f(n-1), f(n-2)]
anteriores3 = [f(n-1), f(n-2), f(n-3)]

Se define entonces que:
    ultimos3 = matriz * anteriores3

Sin embargo, como se dijo anteriormente, para cualquier n se deduce que f(n) resulta una suma de 2023, 2024 y 2025 habiendo multiplicado por sus coeficientes correspondientes.
Psuedo entonces, pasar esta "carga" a matriz y dejar anteriores3 como [2023, 2024, 2025]. Para hacerlo, matriz ahora debe elevarse a n-1 (Para saber cuántas veces tuve que
realizar las operaciones como las de las linea 21, 23 o 25 en el presente comentario). renombro anteriores3 como: bases = [2023, 2024, 2025]

ultimos3 = matriz^(n-1) * bases

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M 10000
#define mod %

void multiplicoMatrices(int A[3][3], int B[3][3]){  //  Esta función multiplica matrices de 3x3 y a sus resultados los deja entre 0 y 9999
    int temporal[3][3] = {0};

    for (__uint8_t i = 0; i < 3; i++){          //  Itero por columnas de A
        for (size_t j = 0; j < 3; j++){         //  Itero por filas de B
            for (__uint8_t k = 0; k < 3; k++){  //  Iterador "dummy" que sirve para moverme sobre columnas (A) y filas (B) sin cambiar la posición apuntada de temporal
                temporal[j][i] += (A[k][i] * B[j][k]) % 10000;  // Sobre A, me muevo por columnas. Sobre B, por filas (tal y como funciona la multiplicación de matrices)
            }
        }
    }

    for (__uint8_t i = 0; i < 3; i++){          //  Sobre la matriz A se devolverá el resultado
        for (__uint8_t j = 0; j < 3; j++){
            A[j][i] = temporal[j][i];
        }        
    }
}

void exponenciacion(int matriz[3][3], int potencia, int resMatriz[3][3]){            //  Esta función, en esencia, calcula matriz^n-1 de la que se habló antes
    int matrizBase[3][3] = {{1,1,1},{1,0,0},{0,1,0}};                                //  HAY QUE CORREGIR EL CÁLCULO. multiplicoMatrices FUNCIONA BIEN
    while (potencia){
        if(potencia % 2 == 1){
            multiplicoMatrices(matriz, matrizBase);
            potencia--;
        }
        else{
            multiplicoMatrices(matriz, matriz);
            potencia = potencia / 2;
        }
        //multiplicoMatrices(resMatriz, matriz);
    }
}

void exponenciacionSobreMatriz(int m[3][3], int potencia){
    int logPotencia = (int)fmax((int)floor(log2(potencia)), 0);
    int exponenciadora[3][3] = {{1,1,1},{1,0,0},{0,1,0}};

    for (int i = 0; i < logPotencia; i++){
        multiplicoMatrices(exponenciadora, exponenciadora);
    }
    
    multiplicoMatrices(m, exponenciadora);

    if (potencia - (int)pow(2, logPotencia) > 0){
        exponenciacionSobreMatriz(m, potencia - (int)pow(2, logPotencia));
    }
}

int main(){
    long long unsigned posiciónABuscar;                 //  Dado que el valor a buscar es muy grande (mayor a 10^16) declaro un long long int (entero de 64 bits)

    printf("¿Qué valor debe ser buscado? \n");
    scanf("%llu", &posiciónABuscar);               //  Leo el número

    if (posiciónABuscar <= 3){
        printf("Los últimos 4 dígitos de la posición solicitada son: %llu\n", (posiciónABuscar+2022));   //  Si es uno de los casos base, devuelvo el valor sin necesidad de computar más
    }
    else{
        int matriz[3][3] = {{1,1,1},{1,0,0},{0,1,0}};   //  La matriz explicada en el bloque de comentarios
        int bases[3] = {2025, 2024, 2023};              //  Los 3 casos base
        int res = 0;

        if (posiciónABuscar != 4){
            exponenciacionSobreMatriz(matriz, posiciónABuscar - 4);
        }

        res = ((matriz[0][0] * 2025) % M + (matriz[0][1] * 2024) % M + (matriz[0][2] * 2023) % M) % 10000;

        printf("Los últimos 4 dígitos de la suma recursiva de los valores en la posición solicitada es: %i\n", res);
    }
}