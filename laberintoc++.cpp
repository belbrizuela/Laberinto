#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <cstdlib>
#include <ctime>
using namespace std;

// Estructura que representa una celda en el laberinto
struct Celda {
    int x, y;          // Coordenadas de la celda (x, y)
    bool visitada;     // Indica si la celda ha sido visitada
    bool paredArriba, paredAbajo, paredIzquierda, paredDerecha; // Paredes de la celda
    bool camino;       // Indica si la celda es parte del camino solución
    Celda* padre;      // Puntero a la celda "padre" para trazar el camino de la solución

    // Constructor de la celda
    Celda(int _x, int _y) : x(_x), y(_y), visitada(false),
        paredArriba(true), paredAbajo(true),
        paredIzquierda(true), paredDerecha(true),
        camino(false), padre(nullptr) {}
};

// Clase que representa el laberinto
class Laberinto {
public:
    int ancho, alto;   // Dimensiones del laberinto
    vector<vector<Celda>> grilla; // Grilla de celdas

    // Constructor del laberinto
    Laberinto(int _ancho, int _alto);

    // Métodos del laberinto
    void generarLaberinto(); // Genera el laberinto
    void mostrarLaberinto(); // Muestra el laberinto
    vector<Celda*> obtenerVecinosNoVisitados(Celda& celda); // Obtiene los vecinos no visitados de una celda
    void removerParedes(Celda& actual, Celda& siguiente); // Remueve las paredes entre dos celdas
};

// Constructor del laberinto
Laberinto::Laberinto(int _ancho, int _alto) : ancho(_ancho), alto(_alto) {
    for (int y = 0; y < alto; ++y) {
        vector<Celda> fila;
        for (int x = 0; x < ancho; ++x) {
            fila.emplace_back(x, y); // Agrega una nueva celda a la fila con las coordenadas (x, y)
        }
        grilla.push_back(fila); // Agrega la fila a la grilla del laberinto
    }
}

// Genera el laberinto utilizando el algoritmo de retroceso (backtracking)
void Laberinto::generarLaberinto() {
    srand(time(0)); // Inicializa el generador de números aleatorios
    stack<Celda*> pila;
    Celda* actual = &grilla[0][0]; // Comienza desde la celda inicial
    actual->visitada = true; // Marca la celda actual como visitada
    int celdasVisitadas = 1;
    int totalCeldas = ancho * alto;

    while (celdasVisitadas < totalCeldas) {
        vector<Celda*> vecinos = obtenerVecinosNoVisitados(*actual); // Obtiene los vecinos no visitados de la celda actual
        if (!vecinos.empty()) { // Si hay vecinos no visitados
            Celda* siguiente = vecinos[rand() % vecinos.size()]; // Elige un vecino al azar
            siguiente->visitada = true; // Marca el vecino como visitado
            removerParedes(*actual, *siguiente); // Remueve las paredes entre la celda actual y el vecino
            pila.push(actual); // Guarda la celda actual en la pila
            actual = siguiente; // Mueve la celda actual al vecino
            ++celdasVisitadas; // Incrementa el contador de celdas visitadas
        }
        else if (!pila.empty()) { // Si no hay vecinos no visitados pero la pila no está vacía
            actual = pila.top(); // Retrocede a la celda anterior almacenada en la pila
            pila.pop(); // Saca la celda anterior de la pila
        }
    }
}

// Obtiene los vecinos no visitados de una celda
vector<Celda*> Laberinto::obtenerVecinosNoVisitados(Celda& celda) {
    vector<Celda*> vecinos;
    if (celda.y > 0 && !grilla[celda.y - 1][celda.x].visitada) // Vecino arriba
        vecinos.push_back(&grilla[celda.y - 1][celda.x]);
    if (celda.y < alto - 1 && !grilla[celda.y + 1][celda.x].visitada) // Vecino abajo
        vecinos.push_back(&grilla[celda.y + 1][celda.x]);
    if (celda.x > 0 && !grilla[celda.y][celda.x - 1].visitada) // Vecino izquierda
        vecinos.push_back(&grilla[celda.y][celda.x - 1]);
    if (celda.x < ancho - 1 && !grilla[celda.y][celda.x + 1].visitada) // Vecino derecha
        vecinos.push_back(&grilla[celda.y][celda.x + 1]);
    return vecinos;
}

// Remueve las paredes entre dos celdas adyacentes
void Laberinto::removerParedes(Celda& actual, Celda& siguiente) {
    if (actual.x == siguiente.x) { // Si están en la misma columna
        if (actual.y > siguiente.y) { // Si la celda siguiente está arriba
            actual.paredArriba = false; // Quita la pared de arriba de la celda actual
            siguiente.paredAbajo = false; // Quita la pared de abajo de la celda siguiente
        }
        else { // Si la celda siguiente está abajo
            actual.paredAbajo = false; // Quita la pared de abajo de la celda actual
            siguiente.paredArriba = false; // Quita la pared de arriba de la celda siguiente
        }
    }
    else { // Si están en la misma fila
        if (actual.x > siguiente.x) { // Si la celda siguiente está a la izquierda
            actual.paredIzquierda = false; // Quita la pared izquierda de la celda actual
            siguiente.paredDerecha = false; // Quita la pared derecha de la celda siguiente
        }
        else { // Si la celda siguiente está a la derecha
            actual.paredDerecha = false; // Quita la pared derecha de la celda actual
            siguiente.paredIzquierda = false; // Quita la pared izquierda de la celda siguiente
        }
    }
}

// Muestra el laberinto en la consola
void Laberinto::mostrarLaberinto() {
    for (int y = 0; y < alto; ++y) {
        for (int x = 0; x < ancho; ++x) {
            if (grilla[y][x].camino) { // Si la celda es parte del camino solución
                cout << " * ";
            }
            else { // Si no es parte del camino solución
                cout << (grilla[y][x].paredArriba ? "###" : "   "); // Muestra las paredes horizontales
            }
        }
        cout << endl;
        for (int x = 0; x < ancho; ++x) {
            if (grilla[y][x].paredIzquierda) { // Si hay pared izquierda
                cout << "#"; // Muestra la pared izquierda
            }
            else {
                cout << " "; // No hay pared izquierda, muestra un espacio
            }
            cout << " "; // Separador entre celdas
            if (x == ancho - 1 || grilla[y][x].paredDerecha) { // Si es la última columna o hay pared derecha
                cout << "#"; // Muestra la pared derecha
            }
            else {
                cout << " "; // No hay pared derecha, muestra un espacio
            }
        }
        cout << endl;
    }
}

// Verifica si el movimiento a una nueva celda es válido
bool esMovimientoValido(Laberinto& laberinto, Celda* celda, int nuevoX, int nuevoY) {
    if (nuevoX < 0 || nuevoX >= laberinto.ancho || nuevoY < 0 || nuevoY >= laberinto.alto)
        return false;

    Celda& actual = laberinto.grilla[celda->y][celda->x];
    Celda& siguiente = laberinto.grilla[nuevoY][nuevoX];

    if (nuevoX == celda->x + 1 && !actual.paredDerecha && !siguiente.paredIzquierda) return true;
    if (nuevoX == celda->x - 1 && !actual.paredIzquierda && !siguiente.paredDerecha) return true;
    if (nuevoY == celda->y + 1 && !actual.paredAbajo && !siguiente.paredArriba) return true;
    if (nuevoY == celda->y - 1 && !actual.paredArriba && !siguiente.paredAbajo) return true;

    return false;
}

// Resuelve el laberinto utilizando el algoritmo de búsqueda en anchura (BFS)
void resolverLaberinto(Laberinto& laberinto) {
    queue<Celda*> cola;
    cola.push(&laberinto.grilla[0][0]);
    vector<vector<bool>> visitado(laberinto.alto, vector<bool>(laberinto.ancho, false));
    visitado[0][0] = true;

    while (!cola.empty()) {
        Celda* actual = cola.front();
        cola.pop();

        // Si hemos llegado a la esquina inferior derecha, marcamos el camino
        if (actual->x == laberinto.ancho - 1 && actual->y == laberinto.alto - 1) {
            while (actual) {
                actual->camino = true;
                actual = actual->padre;
            }
            break;
        }

        // Movimientos posibles: derecha, izquierda, abajo, arriba
        int dx[] = { 1, -1, 0, 0 };
        int dy[] = { 0, 0, 1, -1 };

        for (int i = 0; i < 4; ++i) {
            int nuevoX = actual->x + dx[i];
            int nuevoY = actual->y + dy[i];

            // Si el movimiento es válido y la celda no ha sido visitada
            if (esMovimientoValido(laberinto, actual, nuevoX, nuevoY) && !visitado[nuevoY][nuevoX]) {
                visitado[nuevoY][nuevoX] = true;
                laberinto.grilla[nuevoY][nuevoX].padre = actual;
                cola.push(&laberinto.grilla[nuevoY][nuevoX]);
            }
        }
    }
}

// Función principal
int main() {
    int ancho, alto;
    cout << "Ingrese el ancho del laberinto: ";
    cin >> ancho;
    cout << "Ingrese el alto del laberinto: ";
    cin >> alto;

    Laberinto laberinto(ancho, alto); // Crea un objeto de tipo Laberinto
    laberinto.generarLaberinto(); // Genera el laberinto
    laberinto.mostrarLaberinto(); // Muestra el laberinto generado

    resolverLaberinto(laberinto); // Resuelve el laberinto
    cout << "\nSolución del laberinto:\n";
    laberinto.mostrarLaberinto(); // Muestra la solución del laberinto

    // Pausa al final para que la consola no se cierre inmediatamente
    system("pause");
    return 0;
}
