/*
    MÓDULO 2701 DE UN NÚMERO GIGANTE
    By: nicolasjoannas14@gmail.com

    El enunciado me plantea lo siguiente:
        "¿Podrías decirnos cuál es el resultado de ejecutar el siguiente pseudocódigo? (suponiendo que la máquina tiene los recursos suficientes para completar su ejecución).

        const M = 2701

        funcion compute(n)
        string s = ""
        para i desde 1 hasta n
        s = s + n
        fin para
        return convertir_a_numero(s) % M
        fin funcion

        para cada n en 1, 2, 5, 10, 20, 371844285230994047
        imprimir(n + ": " + compute(n))
        fin para
        fin funcion

        Resultado parcial de la ejecución
        1: 1
        2: 22
        5: 1535
        10: 1083
        20: 1095
        371844285230994047: ???"

    En palabras más simples, lo que hace el pseudocódigo anterior es concatenar n veces (asumiendo n un número natural) el número n, y a ese resultado, calcularle el módulo 2701.
    Dado el tamaño de 371844285230994047, no es factible computarlo como lo dice el pseudocódigo, por lo que voy a encontrar el resultado de manera analítica.
    Sería interesante encontrar una función que describa el armado de un número, es decir, una función que haga lo siguiente:
    f(1) = 1    f(2) = 22    f(3) = 333    f(4) = 4444 ... etc
    ¿Existe una función que describa esto? De hecho, sí. Sea d = floor(log10(n))+1. Se observa que d "cuenta" la cantidad de dígitos de n (por ejemplo, si n = 5433, entonces
    d = floor(log10(5433))+1, lo que resulta en 4). ¿De qué nos sirve esto? Pues resulta que concatenar n veces n no es otra cosa que escribir n cada d lugares decimales. Vamos
    sumando las "partes" del resultado de multiplicar n * 10^d * el i-ésimo lugar. Visto a modo de fórmula:
        
        f(n) = Σn10^(d*i)   ->  f(n) = nΣ(10^(d))^i     (Esta sumatoria va desde i = 0 hasta i = n - 1)

    ¡Tenemos una fórmula que describe la generación del número cuyo mod 2701 queremos calcular!
    Sin embargo, computar tal sumatoria es computacionalmente imposible, al menos para la mayoría de los casos (Con un n>10, por ejemplo, el resultado de la sumatoria ya tendría
    más de 20 dígitos, siendo que es un n "chico"). Además, para un n demasiado grande, es posible que la complejidad de resolver esa sumatoria (que sería O(n)) tome demasiado
    tiempo real.

    Para sortear estos problemas, llamo t = 10^(d) quedando S (la sumatoria) como: nΣt^i
    Es decir, estamos ante una serie geométrica multiplicada por n:

        G(n) = n * (t^n - 1) / (t - 1)    ->    n * (t^n - 1) * (t - 1)^(-1)    ->    n * (10^(floor(log10(n)) + 1) - 1)^(n) * (10^(floor(log10(n)) +1) - 1)^(-1)

    Por lo tanto, G describe el número n concatenado n veces a través de una fórmula cerrada. G(n) mod 2701 me debería dar el resultado buscado. Tal operación, es
    igual a hacer mod 2701 sobre sus factores, multiplicarlos, y a ese resultado calcular su mod 2701. Sin embargo, si (10^(floor(log10(n)) +1) - 1) ≅ 0 mod 2701 esta
    función queda indefinida (Porque necesitaría calcular su inverso multiplicativo siendo que (10^(floor(log10(n)) +1) - 1) no es coprimo con 2701). Pareciera que
    tanto S como G tienen problemas para poder operarles el módulo 2701, ¿Qué se puede hacer?

    Teorema Chino del Resto al rescate:
    Resulta que 2701 = 37 * 73, así que voy a calcular las congruencias mod 37 y mod 73 con S o G según convenga y luego juntar los resultados con el Teorema Chino del Resto.
    Para hacerlo, primero averigüemos cuándo se anula (10^(floor(log10(n)) +1) - 1) (De ahora en más, me referiré a esta expresión como "b").
    *   Caso 37:    Resulta que b ≅ 0 mod 37 cuando 10^k ≅ 1 mod 37 siendo k un múltiplo de 3
    *   Caso 73:    Análogo al razonamiento anterior, pero cuando k es múltiplo de 8
    Ejemplifiquemos con 37 (aunque la lógica es la misma para 73): Si k es múltiplo de 3 (múltiplo de 8 en el caso del 73), S no sólo es computable, sino que es fácil de calcular
    pues: nΣ(10^(d))^i = nΣ(1)^i = n*n = n². Se calcula el módulo de esta expresión, y se combinará con TCR a lo que haya tenido que ser operado sobre 73 (Si k era múltiplo de 8,
    se calculó S mod 73, caso contrario, G mod 73)

    El resultado obtenido, es el pedido por el enunciado
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M 2701
#define mod %

//  Esta función calcula 10^n mod p, a través de las propiedades de la exponenciación. Todo resultado intermedio se calcula mod p para evitar desbordamientos por tamaño (overflow)
int binarioSobreExponente(unsigned long long int n, int p){
    if(n <= 8){                                                 // 10^(n<=8) se puede representar, por consiguiente, calcular. Se devuelve ese resultado
        return ((int)pow(10, n) mod p);
    }
    
    if (n % 2 == 1){                        // Si n impar, llamo nuevamente a la función pasándole el par anterior a n, lo multiplico por 10, y saco su módulo
        return ((10 * binarioSobreExponente(n-1, p)) mod p);
    }
    else{
        int dummy = binarioSobreExponente(n/2, p) mod p;    //  Utilizo una variable dummy para evitar calcular dos veces binarioSobreExponente(n/2, p) para un mismo n
        return ((dummy * dummy) mod p);     //  Si n par, el doble del resultado de binarioSobreExponente(n/2) mod p es lo que necesito
    }
}

//  La siguiente función es una implementación sencilla y "bruta" del Teorema Chino del Resto. Tiene como precondición a coprimo con b.
int TeoremaChinoDelResto(int a, int b, int aResto, int bResto){
    int res = 1;            //  Resultado "por defecto"
    while (1)    {          //  El Teorema Chino del Resto eventualmente termina, por lo que este loop se rompe
        int j = 0;
        while (j < 2){
            if ((res mod a) != aResto || (res mod b) != bResto){
                break;
            }
            j += 1;
        }
        if (j == 2){
            return res;
        }
        res += 1;
    }
    
}

//  La siguiente función es una implementación del inverso multiplicativo de n dado un p. La complejidad de esta función es O(n) y podría ser log(n), además, no chequea que n y M 
//  sean efectivamente coprimos. Sin embargo, para lo que le compete a este programa, es suficiente con la siguiente implementación
int inversoMultiplicativoModular(int n, int p){
    for (int i = 1; i < p; i++){
        if ((n*i) mod p == 1){
            return i;
        }
    }
}

//  Devuelve el n mod p, el primer factor de G. A pesar de ser sólo una linea, fue implementado así para mantener un orden y prolijidad
int calculoPrimerFactor(unsigned long long n, int p){
    return (n mod p);
}

//  Calcula el mod p del segundo factor de G, es decir, (10^(floor(log10(n)) + 1) - 1)^(n) mod p
int calculoSegundoFactor(unsigned long long n, int p){
    int nReducido;
    int lognReducido;
    int res;

    if (p == 37){
        nReducido = n mod 3;
        lognReducido = (int)(floor(log10(n))+1) mod 3;
        nReducido = (nReducido * lognReducido) mod 3;
    }
    else{
        nReducido = n mod 8;
        lognReducido = (int)(floor(log10(n))+1) mod 8;
        nReducido = (nReducido * lognReducido) mod 8;
    }

    res = binarioSobreExponente(nReducido, p);
    res = res - 1;
    res = res mod p;

    return res;
}

//  Similar a la función anterior, calculo el módulo p del tercer factor de G, es decir, (10^(floor(log10(n)) +1) - 1)^(-1) mod p
int calculoTercerFactor(unsigned long long n, int p){
    int lognReducido;
    int res;

    if (p == 37){
        lognReducido = ((int)(floor(log10(n)))+1) mod 3;
    }
    else{
        lognReducido = ((int)(floor(log10(n)))+1) mod 8;
    }

    res = binarioSobreExponente(lognReducido, p);
    res--;
    res = inversoMultiplicativoModular(res, p);

    return res;
}

//  Dado un número n no divisible por p, calcula n*geométrica mod p donde p ∈ {37, 73}.
int noEsDivisible(unsigned long long int n, int p){
    return (calculoPrimerFactor(n, p) * calculoSegundoFactor(n, p) * calculoTercerFactor(n, p)) mod p;
}

int restoDeMod37(unsigned long long int n){
    int cantidadDeDigitos = (int)(floor(log10(n))+1);
    int res;

    if (cantidadDeDigitos mod 3 == 0){      //  Si la cantidad de dígitos de n es múltiplo de 3, entonces 10^(floor(log(n) + 1) - 1 es divisible por 37,
        res = n mod 37;                     //  por lo que devuelvo n² mod 37
        return ((res*res) mod 37);
    }
    else{
        res = noEsDivisible(n, 37);
        return res;
    }
}

int restoDeMod73(unsigned long long int n){
    int cantidadDeDigitos = (int)(floor(log10(n))+1);
    int res;

    if (cantidadDeDigitos mod 8 == 0){      //  Si la cantidad de dígitos de n es múltiplo de 8, entonces 10^(floor(log(n) + 1) - 1 es divisible por 73,
        res = n mod 73;                     //  por lo que devuelvo n² mod 73
        return ((res*res) mod 73);
    }
    else{
        res = noEsDivisible(n, 73);
        return res;
    }
}

int main(){    
    unsigned long long int n;
    printf("Ingrese el número a ser concatenado su misma cantidad de veces y calculado su mod 2701");
    printf("\n");
    scanf("%llu", &n);

    int de37 = restoDeMod37(n);
    int de73 = restoDeMod73(n);

    int res = TeoremaChinoDelResto(37, 73, de37, de73);
    printf("%i", res);
    
    return 0;
}
