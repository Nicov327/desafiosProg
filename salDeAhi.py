#   BY: nicolasjoannas14@gmai.com
#   El siguiente código genera la letra de la canción infantil "Sal de ahí chivita" con animales aleatorios de una lista de 100. La consigna es:
# Un joven estaba elaborando un generador automático de una versión ampliada y aleatoria de la canción para niños "Sal de ahí chivita". Te mostramos el pseudocódigo para que busques la siguiente respuesta:
# ¿Cuál es el tiempo esperado de ejecución para N = 1, 10, 50 y 200? Tené en cuenta que los puntos dentro del pseudocódigo son para indentación.

#Pseudocódigo:
    # array animales = [“el lobo”, “el toro”, … ]; // 100 animales

    # map LlamarA = {}

    # print(“Sal de ahí chivita chivita, sal de ahí de ese lugar”)
    # actualmente = “la chiva”
    # Realizar N veces {
    #   prox = elemento_random(animales)
    #   LlamarA[actualmente] = prox
    #   print(“Hay que llamar a “ + prox + “ para que saque a “ + actualmente)
    #   actualmente = prox
    #   array remover = [ ]
    #   inspeccionar = “la chiva”
    #   repetir hasta que inspeccionar no esté en LlamarA {
    #       remover.insertar_primero(LlamarA[inspeccionar] + “ no quiere sacar a ” + inspeccionar)
    #       inspeccionar = LlamarA[inspeccionar]
    #   }
    #   for i in remover {
    #       print(i)
    #   }
    #   print(“La chiva no quiere salir de ahí. Sal de ahí chivita chivita, sal de ahí de ese lugar”)
    # }

    # Ejemplo de output para N = 3

    # Sal de ahí chivita chivita, sal de ahí de ese lugar
    # Hay que llamar a el lobo para que saque a la chiva
    # el lobo no quiere sacar a la chiva
    # La chiva no quiere salir de ahí. Sal de ahí chivita chivita, sal de ahí de ese lugar
    # Hay que llamar a el perro para que saque a el lobo
    # el perro no quiere sacar a el lobo
    # el lobo no quiere sacar a la chiva
    # La chiva no quiere salir de ahí. Sal de ahí chivita chivita, sal de ahí de ese lugar
    # Hay que llamar a el leon para que saque a el perro
    # el leon no quiere sacar a el perro
    # el perro no quiere sacar a el lobo
    # el lobo no quiere sacar a la chiva
    # La chiva no quiere salir de ahí. Sal de ahí chivita chivita, sal de ahí de ese lugar

# Si bien este problema podría abordarse con una lista enlazada, decidí optar por una resolución más modesta, reordenando la lista inicial de manera
# que pueda ser leída apropiadamente para aparentar encadenar cada linea con lo que corresponde. Al igual que en el pseudocódigo, el tiempo de ejecución es de O(1) (porque
# tenemos una cantidad máxima de 100 animales) pero se imprimen min(100, N(N+1)/2) lineas de "(animal) no quiere sacar a (animal)", min(100, N) lineas
# de "Hay que llamar a (animal) para que saque a (animal)" y N de "La chiva no quiere salir de ahí. Sal de ahí chivita chivita, sal de ahí de ese lugar" por lo que si la lista
# no tuviera un cantidad específica de animales, la complejidad resultaria O(N(N+1)/2)

import random

animales = [        #   100 animales + la chiva
    "el perro", "el gato", "la rata", "el ratón", "el caballo", "la vaca",
    "el toro", "el buey", "la oveja", "la cabra", "el cerdo", "el jabalí",
    "el burro", "la mula", "el gallo", "la gallina", "el pato", "el ganso",
    "el pavo", "el conejo", "la liebre", "el cuy", "el carpincho", "el coipo",
    "el armadillo", "el erizo", "el zorro", "el tejón", "el hurón", "la mofeta",
    "la ardilla", "el castor", "la nutria", "el topo", "la comadreja", "el lobo",
    "el aguará guazú", "el coatí", "el zorrino", "el gato montés", "el puma",
    "el jaguar", "el león", "el tigre", "el oso", "el oso hormiguero",
    "el tamandúa", "el perezoso", "el pangolín", "el dromedario", "el camello",
    "el yak", "el ciervo", "el venado", "el antílope", "la gacela", "el búfalo",
    "el bisonte", "el rinoceronte", "el hipopótamo", "la jirafa", "la cebra",
    "el ñandú", "el guanaco", "la llama", "la vicuña", "el tero", "el hornero",
    "el gorrión", "el chingolo", "el carpintero", "el chimango", "el carancho",
    "el caburé", "el lechuzón", "el picaflor", "el jilguero", "el benteveo",
    "el tordo", "el zorzal", "el chajá", "el cisne", "el flamenco",
    "el pato criollo", "el pato overo", "el pato cuchara", "el pato colorado",
    "la vizcacha", "el cuis", "el ñacurutú", "el peludo",
    "el tatú carreta", "el loro hablador", "el tucán",
    "el gato del monte", "el cormorán", "el pingüino", "el yacaré",
    "el sapo", "la rana", "la chiva"
]

def intercambioPosiciones(x: int, y: int):
    dummy: str = animales[x]
    animales[x] = animales[y]
    animales[y] = dummy
    
def imprimoNoQuiere(indice: int):
    for i in range(indice, 100):
        print(animales[i] + " no quiere sacar a " + animales[i+1])

N: int = int(input("Ingrese cantidad máxima de animales (N)"))
actualmente: str = "la chiva"
animalesTotales: int = 99   #   Si bien la lista contiene 100 animales (sin contar a la chiva), el índice más grande es 99. Por eso asigno este número

print("Sal de ahí chivita chivita, sal de ahí de ese lugar")

for i in range(min(N, 100)):
    prox: int = random.randint(0, animalesTotales)
    print("Hay que llamar a " + animales[prox] + " para que saque a " + actualmente)
    actualmente = animales[prox]
    
    intercambioPosiciones(prox, animalesTotales)
    imprimoNoQuiere(animalesTotales)
    print("La chiva no quiere salir de ahí. Sal de ahí chivita chivita, sal de ahí de ese lugar")
    
    animalesTotales -= 1