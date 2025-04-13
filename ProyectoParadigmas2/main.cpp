#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Profesor {
private:
    string NombreProfesor;
    string Asignatura;

public:
    Profesor(string NombreProfesor, string Asignatura)
        : NombreProfesor(NombreProfesor), Asignatura(Asignatura) {}

    string getNombreProfesor() const { return NombreProfesor; }
    string getAsignatura() const { return Asignatura; }
};

// Clase Respuesta
class Respuesta {
private:
    string texto;
    bool esCorrecta;

public:
    Respuesta(string texto, bool esCorrecta)
        : texto(texto), esCorrecta(esCorrecta) {}

    string getTexto() const { return texto; }
    bool getEsCorrecta() const { return esCorrecta; }
};


// Clase ItemPregunta
class ItemPregunta {
private:
    string enunciado;
    string tipoPregunta;
    vector<Respuesta> respuestas;

public:
    ItemPregunta(string enunciado, string tipoPregunta)
        : enunciado(enunciado), tipoPregunta(tipoPregunta) {}

    void agregarRespuesta(string texto, bool esCorrecta) {
        Respuesta nuevaRespuesta(texto, esCorrecta);
        respuestas.push_back(nuevaRespuesta);
    }

    void mostrarRespuestas() const {
        cout << "Pregunta: " << enunciado << endl;
        for (size_t i = 0; i < respuestas.size(); ++i) {
            cout << i + 1 << ". " << respuestas[i].getTexto();
            if (respuestas[i].getEsCorrecta()) {
                cout << " (Correcta)";
            }
            cout << endl;
        }
    }
};

// Clase ItemManager (gestiona la cantidad de preguntas)
class ItemManager {
private:
    int cantidadItems;

public:
    ItemManager() : cantidadItems(0) {}

    void configurarCantidadItems() {
        cout << "Ingrese la cantidad de ítems para la prueba: ";
        cin >> cantidadItems;
        if (cantidadItems < 0) {
            cantidadItems = 0;
        }
    }

    int getCantidadItems() const { return cantidadItems; }
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

        string nombre, asignatura, titulo, fecha;
        cout << "Ingrese el nombre del profesor: ";
        getline(cin, nombre);
        cout << "Ingrese la asignatura: ";
        getline(cin, asignatura);
        cout << "Ingrese el título de la prueba: ";
        getline(cin, titulo);
        cout << "Ingrese la fecha de creación: ";
        getline(cin, fecha);

        // Configurar la cantidad de ítems
        gestorItems.configurarCantidadItems();
        int cantidadItems = gestorItems.getCantidadItems();

        if (cantidadItems <= 0) {
            cout << "Primero debe configurar una cantidad válida de ítems." << endl;
            return;
        }

        // Configurar ítems
        for (int i = 0; i < cantidadItems; ++i) {
            cout << "===== Configurando el Ítem #" << (i + 1) << " =====" << endl;

            // Capturar el enunciado del ítem
            string enunciado;
            cout << "Ingrese el enunciado de la pregunta: ";
            cin.ignore(); // Limpiar el buffer del teclado
            getline(cin, enunciado);

            // Capturar el tipo de pregunta
            string tipoPregunta;
            cout << "Ingrese el tipo de pregunta (ej. 'Verdadero/Falso', 'Opción Múltiple'): ";
            getline(cin, tipoPregunta);

            // Crear el ItemPregunta
            ItemPregunta nuevoItem(enunciado, tipoPregunta);

            // Agregar respuestas al ítem
            int numRespuestas;
            cout << "¿Cuántas respuestas tendrá este ítem? ";
            cin >> numRespuestas;

            cin.ignore(); // Limpiar el buffer del teclado
            for (int j = 0; j < numRespuestas; ++j) {
                cout << "Respuesta #" << (j + 1) << endl;

                // Capturar el texto de la respuesta
                string texto;
                cout << "Ingrese el texto de la respuesta: ";
                getline(cin, texto);

                // Capturar si la respuesta es correcta
                bool esCorrecta;
                cout << "¿Es esta respuesta correcta? (1 = Sí, 0 = No): ";
                cin >> esCorrecta;

                cin.ignore(); // Limpiar el buffer del teclado

                // Agregar la respuesta al ítem
                nuevoItem.agregarRespuesta(texto, esCorrecta);
            }

            // Agregar el ítem a la lista de preguntas de la prueba
            preguntas.push_back(nuevoItem);
        }

        cout << "¡Todos los ítems han sido configurados exitosamente!" << endl;

        // Aquí puedes continuar con el resto del proceso de creación de la prueba
        cout << "La prueba se ha creado con éxito." << endl;
    }

    void mostrarPrueba() const {
        cout << "Título de la Prueba: " << tituloPrueba << endl;
        cout << "Fecha de Creación: " << fechaCreacion << endl;
        for (size_t i = 0; i < preguntas.size(); ++i) {
            cout << "Pregunta #" << (i + 1) << ":" << endl;
            preguntas[i].mostrarRespuestas();
        }
    }
};

// Clase Menu
class Menu {
private:
    Prueba prueba;
    ItemManager gestorItems;

public:
    Menu(string NombreProfesor, string Asignatura, string tituloPrueba, string fechaCreacion)
        : prueba(NombreProfesor, Asignatura, tituloPrueba, fechaCreacion), gestorItems() {}

    void mostrarMenu() const {
        cout << "===== Menú Principal =====" << endl;
        cout << "1. Crear Prueba" << endl;
        cout << "2. Mostrar Prueba" << endl;
        cout << "3. Salir" << endl;
        cout << "==========================" << endl;
    }

    void ejecutarOpcion(int opcion) {
        switch (opcion) {
            case 1: // Crear prueba
                prueba.CrearPrueba(gestorItems);
            break;
            case 2: // Mostrar prueba
                prueba.mostrarPrueba();
            break;
            case 3: // Salir
                cout << "Saliendo del programa..." << endl;
            break;
            default:
                cout << "Opción no válida. Intente nuevamente." << endl;
            break;
        }
    }

    void lanzarMenu() {
        int opcion;
        do {
            mostrarMenu();
            cout << "Seleccione una opción: ";
            cin >> opcion;
            ejecutarOpcion(opcion);
        } while (opcion != 3);
    }
};

// Función principal
int main() {
    Menu menu("NombreProfesor", "Asignatura", "TituloPrueba", "FechaCreacion");
    menu.lanzarMenu();

    return 0;
}
