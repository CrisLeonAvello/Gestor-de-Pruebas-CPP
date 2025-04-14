#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <ctime>

using namespace std;

// Función que elimina tildes de texto
string eliminarTildes(string texto) {
    for (size_t i = 0; i < texto.length(); i++) {
        if (texto[i] == 'á' || texto[i] == 'Á') texto[i] = 'a';
        else if (texto[i] == 'é' || texto[i] == 'É') texto[i] = 'e';
        else if (texto[i] == 'í' || texto[i] == 'Í') texto[i] = 'i';
        else if (texto[i] == 'ó' || texto[i] == 'Ó') texto[i] = 'o';
        else if (texto[i] == 'ú' || texto[i] == 'Ú') texto[i] = 'u';
    }
    return texto;
}

class Respuesta {
private:
    string texto;

public:
    Respuesta(string texto) : texto(eliminarTildes(move(texto))) {}
    string getTexto() const { return texto; }

    // Destructor
    ~Respuesta() {}
};

class ItemPregunta {
private:
    string enunciado;
    vector<Respuesta> respuestas;

public:
    ItemPregunta(string enunciado) : enunciado(eliminarTildes(move(enunciado))) {}

    void agregarRespuesta(const string& texto) { respuestas.emplace_back(eliminarTildes(texto)); }

    void mostrar() const {
        cout << "Pregunta: " << enunciado << endl;
        for (size_t i = 0; i < respuestas.size(); ++i) {
            cout << "  - Respuesta " << i + 1 << ": " << respuestas[i].getTexto() << endl;
        }
    }

    string getEnunciado() const { return enunciado; }
    vector<Respuesta>& getRespuestas() { return respuestas; }
    void setEnunciado(const string& nuevoEnunciado) { enunciado = eliminarTildes(nuevoEnunciado); }

    // Destructor
    ~ItemPregunta() {
        respuestas.clear(); // Limpia todas las respuestas almacenadas
    }
};

class Item {
private:
    string tipo;
    string taxonomia;
    vector<ItemPregunta> preguntas;

public:
    Item(string tipo, string taxonomia)
        : tipo(eliminarTildes(move(tipo))), taxonomia(eliminarTildes(move(taxonomia))) {}

    void agregarPregunta(const ItemPregunta& pregunta) { preguntas.push_back(pregunta); }

    void eliminarPregunta(int indice) {
        if (indice >= 0 && indice < preguntas.size()) {
            preguntas.erase(preguntas.begin() + indice);
            cout << "Pregunta eliminada exitosamente.\n";
        }
    }

    void mostrar(int numeroItem) const {
        cout << "Item #" << numeroItem << " (" << tipo << ", " << taxonomia << "):\n";
        for (size_t i = 0; i < preguntas.size(); ++i) {
            cout << "  " << i + 1 << ") ";
            preguntas[i].mostrar();
        }
    }

    string getTaxonomia() const { return taxonomia; }
    string getTipo() const { return tipo; }
    vector<ItemPregunta>& getPreguntas() { return preguntas; }

    // Destructor
    ~Item() {
        preguntas.clear(); // Limpia las preguntas del item al destruirlo
    }
};

class Profesor {
private:
    string nombre;

public:
    Profesor(string nombre) : nombre(eliminarTildes(move(nombre))) {}

    string getNombre() const { return nombre; }

    // Destructor
    ~Profesor() {}
};

class Materia {
private:
    string nombre;
    float ponderacion;

public:
    Materia(string nombre, float ponderacion)
        : nombre(eliminarTildes(move(nombre))), ponderacion(ponderacion) {}

    string getNombre() const { return nombre; }
    float getPonderacion() const { return ponderacion; }
    void setPonderacion(float nuevaPonderacion) { ponderacion = nuevaPonderacion; }

    // Destructor
    ~Materia() {}
};

class Prueba {
private:
    string titulo;
    string fechaCreacion;
    Profesor profesor;
    Materia materia;
    vector<Item> items;

    string obtenerFechaActual() {
        time_t t = time(nullptr);
        tm* fechaLocal = localtime(&t);

        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", fechaLocal);

        return string(buffer);
    }

public:
    Prueba(string titulo, const Profesor& profesor, const Materia& materia)
        : titulo(eliminarTildes(move(titulo))), profesor(profesor), materia(materia), fechaCreacion(obtenerFechaActual()) {}

    void agregarItem(const Item& item) { items.push_back(item); }

    const vector<Item>& getItems() const { return items; }
    vector<Item>& getEditableItems() { return items; }
    const string& getTitulo() const { return titulo; }

    void mostrarPrueba() const {
        cout << "Título: " << titulo << endl;
        cout << "Fecha: " << fechaCreacion << endl;
        cout << "Profesor: " << profesor.getNombre() << endl;
        cout << "Materia: " << materia.getNombre() << endl;
        cout << "Ponderación: " << materia.getPonderacion() << endl;

        for (size_t i = 0; i < items.size(); ++i) {
            cout << "\nItem #" << i + 1 << ":" << endl;
            items[i].mostrar(i + 1);
        }
    }

    // Destructor
    ~Prueba() {
        items.clear(); // Limpia los items al liberar la prueba
    }
};

class Menu {
private:
    vector<Prueba> pruebas;

    int obtenerEntradaValida(int min, int max) {
        int opcion;
        while (true) {
            cin >> opcion;
            if (cin.fail() || opcion < min || opcion > max) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ingrese un número válido: ";
            } else {
                return opcion;
            }
        }
    }

    void pausaMenu() const {
        cout << "\nPresione ENTER para regresar al menú principal...";
        cin.ignore();
        cin.get();
    }

public:
    void mostrarMenu() const {
        cout << "\n===== Menú Principal =====\n";
        cout << "1. Crear Evaluación.\n";
        cout << "2. Actualizar Item.\n";
        cout << "3. Borrar Items.\n";
        cout << "4. Información de los Items.\n";
        cout << "5. Buscar según Taxonomía.\n";
        cout << "6. Salir\n";
        cout << "Seleccione una opción: ";
    }

    void lanzar() {
        int opcion;
        do {
            mostrarMenu();
            opcion = obtenerEntradaValida(1, 6);
            switch (opcion) {
                case 1: crearPrueba(); break;
                case 2: actualizarItem(); break;
                case 3: borrarItem(); break;
                case 4: mostrarPruebas(); break;
                case 5: consultarItemsPorTaxonomia(); break;
                case 6: cout << "Saliendo... ¡Adiós!\n"; break;
            }
        } while (opcion != 6);
    }

    void crearPrueba() {
        string tituloPrueba;
        cout << "\n===== Crear Prueba =====\n";
        cout << "Ingrese el título de la prueba: ";
        cin.ignore();
        getline(cin, tituloPrueba);

        cout << "Ingrese el nombre del profesor: ";
        string nombreProfesor;
        getline(cin, nombreProfesor);
        Profesor profesor(nombreProfesor);

        cout << "Ingrese el nombre de la materia: ";
        string nombreMateria;
        getline(cin, nombreMateria);

        cout << "Ingrese la ponderación de la materia: ";
        float ponderacion;
        cin >> ponderacion;

        Materia materia(nombreMateria, ponderacion);
        Prueba nuevaPrueba(tituloPrueba, profesor, materia);

        while (true) {
            cout << "\n=== Nuevo Item ===\n";
            cout << "Seleccione qué Tipo de Pregunta desea en el Item.\n";
            cout << "1) Verdadero/Falso\n";
            cout << "2) Opción Múltiple\n------------\n";
            cout << "3) Salir\n";

            int tipoItem = obtenerEntradaValida(1, 3);

            if (tipoItem == 3) {
                pruebas.push_back(nuevaPrueba);
                cout << "Saliendo de la creación de la prueba...\n";
                return;
            }

            string tipo = (tipoItem == 1) ? "Verdadero/Falso" : "Opción Múltiple";

            cout << "Ingrese la Taxonomía del item.\n";
            cout << "1) Crear\n2) Evaluar\n3) Analizar\n4) Aplicar\n5) Entender\n6) Recordar\n------------\n";
            int taxonomiaItem = obtenerEntradaValida(1, 6);
            string taxonomia;
            switch (taxonomiaItem) {
                case 1: taxonomia = "crear"; break;
                case 2: taxonomia = "evaluar"; break;
                case 3: taxonomia = "analizar"; break;
                case 4: taxonomia = "aplicar"; break;
                case 5: taxonomia = "entender"; break;
                case 6: taxonomia = "recordar"; break;
            }

            Item nuevoItem(tipo, taxonomia);

            cout << "Ingrese la cantidad de preguntas que desea agregar al item: ";
            int numPreguntas = obtenerEntradaValida(1, 50);

            for (int j = 0; j < numPreguntas; ++j) {
                cout << "Ingrese la pregunta #" << j + 1 << ": ";
                cin.ignore();
                string enunciadoPregunta;
                getline(cin, enunciadoPregunta);

                ItemPregunta nuevaPregunta(enunciadoPregunta);

                if (tipo == "Opción Múltiple") {
                    cout << "¿Cuántas Respuestas quiere ingresar?: ";
                    int numRespuestas = obtenerEntradaValida(1, 4);

                    for (int r = 0; r < numRespuestas; ++r) {
                        cout << "Respuesta #" << r + 1 << ": ";
                        cin.ignore();
                        string respuestaTexto;
                        getline(cin, respuestaTexto);
                        nuevaPregunta.agregarRespuesta(respuestaTexto);
                    }
                } else {
                    nuevaPregunta.agregarRespuesta("Verdadero");
                    nuevaPregunta.agregarRespuesta("Falso");
                }

                nuevoItem.agregarPregunta(nuevaPregunta);
            }

            nuevaPrueba.agregarItem(nuevoItem);

            cout << "¿Desea agregar otro item?\n1) Sí\n2) No\n";
            int continuar = obtenerEntradaValida(1, 2);
            if (continuar == 2) {
                pruebas.push_back(nuevaPrueba);
                cout << "Prueba creada y guardada exitosamente.\n";
                return;
            }
        }
    }

    void mostrarPruebas() const {
        if (pruebas.empty()) {
            cout << "No se han creado pruebas aún.\n";
        } else {
            for (size_t i = 0; i < pruebas.size(); ++i) {
                cout << "\nPrueba #" << i + 1 << ":\n";
                pruebas[i].mostrarPrueba();
            }
        }
        pausaMenu();
    }

    void consultarItemsPorTaxonomia() const {
        if (pruebas.empty()) {
            cout << "No se han creado pruebas aún.\n";
        } else {
            for (size_t i = 0; i < pruebas.size(); ++i) {
                cout << "\nTítulo de la prueba: " << pruebas[i].getTitulo() << endl;
                const auto& items = pruebas[i].getItems();
                for (size_t j = 0; j < items.size(); ++j) {
                    cout << "  Item #" << j + 1 << " - Taxonomía: " << items[j].getTaxonomia() << endl;
                }
            }
        }
        pausaMenu();
    }

    void borrarItem() {
        if (pruebas.empty()) {
            cout << "No se han creado pruebas aún.\n";
            return;
        }

        while (true) {
            cout << "Seleccione la prueba donde desea borrar un item:\n";
            for (size_t i = 0; i < pruebas.size(); ++i) {
                cout << i + 1 << ") " << pruebas[i].getTitulo() << endl;
            }

            int indicePrueba = obtenerEntradaValida(1, pruebas.size()) - 1;
            auto& pruebaSeleccionada = pruebas[indicePrueba];
            auto& items = pruebaSeleccionada.getEditableItems();

            if (items.empty()) {
                cout << "La prueba seleccionada no tiene items.\n";
                break;
            }

            cout << "Prueba seleccionada: " << pruebaSeleccionada.getTitulo() << "\nItems disponibles:\n";
            for (size_t i = 0; i < items.size(); ++i) {
                cout << "  Item #" << i + 1 << " - Taxonomía: " << items[i].getTaxonomia() << endl;
            }

            cout << "Ingrese el número del item que desea borrar: ";
            int numeroItem = obtenerEntradaValida(1, items.size());
            cout << "¿Está seguro que quiere eliminar este item?\n1) Sí\n2) No\n";
            int confirmacion = obtenerEntradaValida(1, 2);

            if (confirmacion == 1) {
                items.erase(items.begin() + (numeroItem - 1));
                cout << "Item eliminado exitosamente.\n";

                if (items.empty()) {
                    cout << "La prueba \"" << pruebaSeleccionada.getTitulo() << "\" ya no tiene items. Se eliminará automáticamente.\n";
                    pruebas.erase(pruebas.begin() + indicePrueba);
                    break;
                }
            } else {
                cout << "Acción cancelada. El item no se eliminó.\n";
                continue;
            }

            cout << "¿Desea hacer algo más?\n";
            cout << "1) Borrar otro item\n";
            cout << "2) Seleccionar otra prueba\n";
            cout << "3) Regresar al menú principal\n";
            int opcion = obtenerEntradaValida(1, 3);

            if (opcion == 1) continue;
            if (opcion == 2) return;
            break;
        }
        pausaMenu();
    }

    void actualizarItem() {
        if (pruebas.empty()) {
            cout << "No se han creado pruebas aún.\n";
            return;
        }

        cout << "Seleccione la prueba donde desea actualizar un item:\n";
        for (size_t i = 0; i < pruebas.size(); ++i) {
            cout << i + 1 << ") " << pruebas[i].getTitulo() << endl;
        }

        int indicePrueba = obtenerEntradaValida(1, pruebas.size()) - 1;
        auto& pruebaSeleccionada = pruebas[indicePrueba];

        auto& items = pruebaSeleccionada.getEditableItems();
        if (items.empty()) {
            cout << "La prueba seleccionada no tiene items.\n";
            pausaMenu();
            return;
        }

        cout << "Seleccione el item que desea actualizar:\n";
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ") Item #" << i + 1 << " - Tipo: " << items[i].getTipo() << endl;
        }

        int indiceItem = obtenerEntradaValida(1, items.size()) - 1;
        auto& itemSeleccionado = items[indiceItem];

        cout << "\nSeleccione una acción:\n";
        cout << "1) Agregar Pregunta\n";
        cout << "2) Salir\n";
        int accion = obtenerEntradaValida(1, 2);

        switch (accion) {
            case 1: {
                cout << "Ingrese el enunciado de la nueva pregunta: ";
                cin.ignore();
                string preguntaTexto;
                getline(cin, preguntaTexto);

                ItemPregunta nuevaPregunta(preguntaTexto);

                if (itemSeleccionado.getTipo() == "Verdadero/Falso") {
                    nuevaPregunta.agregarRespuesta("Verdadero");
                    nuevaPregunta.agregarRespuesta("Falso");
                    cout << "Respuestas 'Verdadero' y 'Falso' añadidas automáticamente.\n";
                } else if (itemSeleccionado.getTipo() == "Opción Múltiple") {
                    cout << "¿Cuántas respuestas desea agregar?: ";
                    int numRespuestas = obtenerEntradaValida(1, 10);

                    for (int i = 0; i < numRespuestas; ++i) {
                        cout << "Ingrese la respuesta #" << i + 1 << ": ";
                        string respuestaTexto;
                        cin.ignore();
                        getline(cin, respuestaTexto);
                        nuevaPregunta.agregarRespuesta(respuestaTexto);
                    }
                } else {
                    cout << "Tipo de item desconocido. No se pudo agregar la pregunta.\n";
                    break;
                }

                itemSeleccionado.agregarPregunta(nuevaPregunta);
                cout << "Pregunta agregada exitosamente.\n";
                break;
            }
            case 2:
                cout << "Saliendo de la actualización...\n";
                return;
        }
        pausaMenu();
    }

    // Destructor
    ~Menu() {
        pruebas.clear(); // Libera la memoria asociada a las pruebas
    }
};

int main() {
    Menu menu;
    menu.lanzar();
    return 0;
}