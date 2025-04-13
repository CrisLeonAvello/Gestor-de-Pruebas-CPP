#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Clase Profesor
class Profesor {
private:
    string NombreProfesor;
    string Asignatura;

public:
    Profesor(string NombreProfesor, string Asignatura) : NombreProfesor(NombreProfesor), Asignatura(Asignatura) {}

    string getNombreProfesor() const {
        return NombreProfesor;
    }

    string getAsignatura() const {
        return Asignatura;
    }
};

// Clase Respuesta
class Respuesta {
private:
    string texto;
    bool esCorrecta;

public:
    Respuesta(string texto, bool esCorrecta) : texto(texto), esCorrecta(esCorrecta) {}

    string getTexto() const {
        return texto;
    }

    bool getEsCorrecta() const {
        return esCorrecta;
    }
};

// Clase ItemPregunta
class ItemPregunta {
private:
    string enunciado;
    string tipoPregunta;
    vector<Respuesta> respuestas;

public:
    ItemPregunta(string enunciado, string tipoPregunta) : enunciado(enunciado), tipoPregunta(tipoPregunta) {}

    void agregarRespuesta(string texto, bool esCorrecta) {
        respuestas.push_back(Respuesta(texto, esCorrecta));
    }

    void mostrarRespuestas() const {
        cout << "Pregunta: " << enunciado << endl;
        cout << "Tipo: " << tipoPregunta << endl;
        for (size_t i = 0; i < respuestas.size(); ++i) {
            cout << "[" << i + 1 << "] " << respuestas[i].getTexto() << endl;
        }
    }
};

// Clase ItemManager (gestiona la cantidad de preguntas)
class ItemManager {
private:
    int cantidadItems;

public:
    ItemManager() : cantidadItems(0) {}

    // Configurar la cantidad de ítems
    void configurarCantidadItems() {
        cout << "Ingrese la cantidad de ítems (preguntas) que desea añadir: ";
        cin >> cantidadItems;
    }

    // Obtener el número de ítems
    int getCantidadItems() const {
        return cantidadItems;
    }
};

// Clase Prueba
class Prueba : public Profesor {
private:
    string tituloPrueba;
    string fechaCreacion;
    vector<ItemPregunta> preguntas;

public:
    Prueba(string NombreProfesor, string Asignatura, string titulo, string fecha)
        : Profesor(NombreProfesor, Asignatura), tituloPrueba(titulo), fechaCreacion(fecha) {}

    void CrearPrueba(ItemManager& gestorItems) {
        int cantidadPreguntas = gestorItems.getCantidadItems();
        if (cantidadPreguntas <= 0) {
            cout << "Primero debe configurar una cantidad válida de ítems." << endl;
            return;
        }

        for (int i = 0; i < cantidadPreguntas; ++i) {
            cout << "\nIngrese el enunciado de la pregunta " << i + 1 << ": ";
            string enunciado;
            cin.ignore(); // Limpiar el buffer
            getline(cin, enunciado);

            string tipoPregunta;
            cout << "Ingrese el tipo de pregunta: [1] Opción Múltiple [2] Verdadero / Falso: ";
            cin >> tipoPregunta;

            tipoPregunta = (tipoPregunta == "1") ? "Opción Múltiple" : "Verdadero / Falso";
            ItemPregunta nuevaPregunta(enunciado, tipoPregunta);

            if (tipoPregunta == "Opción Múltiple") {
                cout << "Ingrese la cantidad de respuestas: ";
                int cantidadRespuestas;
                cin >> cantidadRespuestas;

                for (int j = 0; j < cantidadRespuestas; ++j) {
                    cout << "Ingrese el texto de la respuesta " << j + 1 << ": ";
                    string textoRespuesta;
                    cin.ignore();
                    getline(cin, textoRespuesta);

                    cout << "¿Es la respuesta correcta? (1 para Sí, 0 para No): ";
                    bool esCorrecta;
                    cin >> esCorrecta;

                    nuevaPregunta.agregarRespuesta(textoRespuesta, esCorrecta);
                }
            } else {
                // Pregunta de tipo "Verdadero / Falso"
                nuevaPregunta.agregarRespuesta("Verdadero", true);
                nuevaPregunta.agregarRespuesta("Falso", false);
            }

            preguntas.push_back(nuevaPregunta);
        }
    }

    void mostrarPrueba() const {
        cout << "\nPrueba: " << tituloPrueba << endl;
        cout << "Fecha de creación: " << fechaCreacion << endl;
        cout << "Profesor: " << getNombreProfesor() << endl;
        cout << "Asignatura: " << getAsignatura() << endl;

        for (size_t i = 0; i < preguntas.size(); ++i) {
            cout << "\nPregunta " << i + 1 << ":" << endl;
            preguntas[i].mostrarRespuestas();
        }
    }
};

// Clase Menu
class Menu {
private:
    Prueba* prueba;
    ItemManager gestorItems;

public:
    Menu(string NombreProfesor, string Asignatura, string tituloPrueba, string fechaCreacion) {
        prueba = new Prueba(NombreProfesor, Asignatura, tituloPrueba, fechaCreacion);
    }

    ~Menu() {
        delete prueba;
    }

    void mostrarMenu() const {
        cout << "\n--- Menú Principal ---" << endl;
        cout << "1. Configurar cantidad de preguntas" << endl;
        cout << "2. Crear Prueba" << endl;
        cout << "3. Mostrar Prueba" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccione una opción: ";
    }

    void ejecutarOpcion(int opcion) {
        switch (opcion) {
            case 1:
                gestorItems.configurarCantidadItems();
                break;
            case 2:
                prueba->CrearPrueba(gestorItems);
                break;
            case 3:
                prueba->mostrarPrueba();
                break;
            case 4:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opción no válida. Inténtelo de nuevo." << endl;
        }
    }

    void lanzarMenu() {
        int opcion;
        do {
            mostrarMenu();
            cin >> opcion;
            ejecutarOpcion(opcion);
        } while (opcion != 4);
    }
};

// Función principal
int main() {
    // Crear el menú con profesor, asignatura, título y fecha
    Menu menu("Juan Pérez", "Matemáticas", "Examen Final", "20/11/2023");

    // Lanzar el menú
    menu.lanzarMenu();

    return 0;
}