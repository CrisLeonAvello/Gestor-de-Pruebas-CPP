#include <iostream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Funcion que elimina tildes de texto
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
};

class Item {
private:
    string tipo;
    string taxonomia;
    vector<ItemPregunta> preguntas;

public:
    Item(string tipo, string taxonomia) : tipo(eliminarTildes(move(tipo))), taxonomia(eliminarTildes(move(taxonomia))) {}

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
};

class Prueba {
private:
    string titulo;
    string fechaCreacion;
    vector<Item> items;

    string obtenerFechaActual() { return "2023-10-25"; }

public:
    Prueba(string titulo) : titulo(eliminarTildes(move(titulo))), fechaCreacion(obtenerFechaActual()) {}
    void agregarItem(const Item& item) { items.push_back(item); }
    const vector<Item>& getItems() const { return items; }
    vector<Item>& getEditableItems() { return items; }
    const string& getTitulo() const { return titulo; }
    void mostrarPrueba() const {
        cout << "Prueba: " << titulo << " creada el " << fechaCreacion << endl;
        for (size_t i = 0; i < items.size(); ++i) {
            items[i].mostrar(i + 1);
        }
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
                cout << "Ingrese un numero valido: ";
            } else {
                return opcion;
            }
        }
    }

    void pausaMenu() const {
        cout << "\nPresione ENTER para regresar al menu principal...";
        cin.ignore();
        cin.get();
    }

    void actualizarItem() {
        if (pruebas.empty()) {
            cout << "No se han creado pruebas aun.\n";
            return;
        }

        cout << "Seleccione la prueba donde desea actualizar un item:\n";
        for (size_t i = 0; i < pruebas.size(); ++i) {
            cout << i + 1 << ") " << pruebas[i].getTitulo() << endl;
        }

        int indicePrueba = obtenerEntradaValida(1, pruebas.size()) - 1;
        auto& pruebaSeleccionada = pruebas[indicePrueba];

        const auto& items = pruebaSeleccionada.getItems();
        if (items.empty()) {
            cout << "La prueba seleccionada no tiene items.\n";
            pausaMenu();
            return;
        }

        cout << "Seleccione el item que desea actualizar:\n";
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ") Item #" << i + 1 << " - Taxonomia: " << items[i].getTaxonomia() << endl;
        }

        int indiceItem = obtenerEntradaValida(1, items.size()) - 1;
        auto& itemSeleccionado = pruebaSeleccionada.getEditableItems()[indiceItem];

        cout << "\nSeleccione una accion:\n";
        cout << "1) Agregar Pregunta\n";
        cout << "2) Eliminar Preguntas\n";
        cout << "3) Seleccionar y Modificar pregunta (Solo Opcion Multiple)\n";
        int accion = obtenerEntradaValida(1, 3);

        switch (accion) {
            case 1: { // Agregar pregunta
                cout << "Ingrese el enunciado de la nueva pregunta: ";
                cin.ignore();
                string preguntaTexto;
                getline(cin, preguntaTexto);
                ItemPregunta nuevaPregunta(preguntaTexto);
                itemSeleccionado.agregarPregunta(nuevaPregunta);
                cout << "Pregunta agregada exitosamente.\n";
                break;
            }
            case 2: { // Eliminar pregunta
                auto& preguntas = itemSeleccionado.getPreguntas();
                if (preguntas.empty()) {
                    cout << "El item no tiene preguntas.\n";
                } else {
                    cout << "Seleccione la pregunta a eliminar:\n";
                    for (size_t i = 0; i < preguntas.size(); ++i) {
                        cout << i + 1 << ") " << preguntas[i].getEnunciado() << endl;
                    }
                    int indicePregunta = obtenerEntradaValida(1, preguntas.size()) - 1;
                    itemSeleccionado.eliminarPregunta(indicePregunta);
                }
                break;
            }
            case 3: { // Modificar pregunta (solo para Opcion Multiple)
                if (itemSeleccionado.getTipo() != "Opcion Multiple") {
                    cout << "Esta opcion solo esta disponible para items de tipo Opcion Multiple.\n";
                } else {
                    auto& preguntas = itemSeleccionado.getPreguntas();
                    if (preguntas.empty()) {
                        cout << "El item no tiene preguntas para modificar.\n";
                    } else {
                        cout << "Seleccione la pregunta que desea modificar:\n";
                        for (size_t i = 0; i < preguntas.size(); ++i) {
                            cout << i + 1 << ") " << preguntas[i].getEnunciado() << endl;
                        }
                        int indicePregunta = obtenerEntradaValida(1, preguntas.size()) - 1;
                        ItemPregunta& preguntaSeleccionada = preguntas[indicePregunta];

                        cout << "Ingrese nuevamente el enunciado de la pregunta: ";
                        cin.ignore();
                        string nuevoEnunciado;
                        getline(cin, nuevoEnunciado);
                        preguntaSeleccionada.setEnunciado(nuevoEnunciado);

                        cout << "Cuantas respuestas desea ingresar? ";
                        int numRespuestas = obtenerEntradaValida(1, 5);
                        preguntaSeleccionada.getRespuestas().clear();
                        for (int i = 0; i < numRespuestas; ++i) {
                            cout << "Ingrese la respuesta #" << i + 1 << ": ";
                            string nuevaRespuesta;
                            getline(cin, nuevaRespuesta);
                            preguntaSeleccionada.agregarRespuesta(nuevaRespuesta);
                        }
                        cout << "Pregunta modificada exitosamente.\n";
                    }
                }
                break;
            }
        }
        pausaMenu();
    }

public:
    void mostrarMenu() const {
        cout << "\n===== Menu Principal =====\n";
        cout << "1. Crear Evaluacion.\n";
        cout << "2. Actualizar Item.\n";
        cout << "3. Borrar Items.\n";
        cout << "4. Informacion de los Items.\n";
        cout << "5. Buscar segun Taxonomia.\n";
        cout << "6. Salir\n";
        cout << "Seleccione una opcion: ";
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
                case 6: cout << "Saliendo... Adios!\n"; break;
            }
        } while (opcion != 6);
    }

    void crearPrueba() {
        string tituloPrueba;
        cout << "Ingrese el titulo de la prueba: ";
        cin.ignore();
        getline(cin, tituloPrueba);
        Prueba nuevaPrueba(tituloPrueba);

        cout << "Ingrese la cantidad de Items de su prueba(max20)\n ";
        int numItems = obtenerEntradaValida(1, 20);

        for (int i = 0; i < numItems; ++i) {
            cout << "\n===== Item #" << i + 1 << " =====\n";

            cout << "Seleccione que Tipo de Pregunta desea en el Item.\n";
            cout << "1) Verdadero/Falso\n";
            cout << "2) Opcion Multiple\n------------\n";
            int tipoItem = obtenerEntradaValida(1, 2);
            string tipo = (tipoItem == 1) ? "Verdadero/Falso" : "Opcion Multiple";

            cout << "Ingrese la Taxonomia del item.\n";
            cout << "1) crear\n2) Evaluar\n3) Analizar\n4) Aplicar\n5) Entender\n6) Recordar\n------------\n";
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

            cout << "Cuantas preguntas contiene este item\n------------\n ";
            int numPreguntas = obtenerEntradaValida(1, 5);

            for (int j = 0; j < numPreguntas; ++j) {
                cout << "Ingrese la pregunta #" << j + 1 << "\n------------\n ";
                string enunciado;
                cin.ignore();
                getline(cin, enunciado);
                ItemPregunta nuevaPregunta(enunciado);

                if (tipo == "Opcion Multiple") {
                    cout << "Cuantas respuestas tendra esta pregunta\n------------\n ";
                    int numRespuestas = obtenerEntradaValida(1, 4);

                    for (int k = 0; k < numRespuestas; ++k) {
                        cout << "Respuesta #" << k + 1 << ": ";
                        string respuesta;
                        getline(cin, respuesta);
                        nuevaPregunta.agregarRespuesta(respuesta);
                    }
                }
                nuevoItem.agregarPregunta(nuevaPregunta);
            }
            nuevaPrueba.agregarItem(nuevoItem);
        }

        pruebas.push_back(nuevaPrueba);
        cout << "Prueba creada exitosamente.\n";
    }

    void mostrarPruebas() const {
        if (pruebas.empty()) {
            cout << "No se han creado pruebas aun.\n";
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
            cout << "No se han creado pruebas aun.\n";
        } else {
            for (size_t i = 0; i < pruebas.size(); ++i) {
                cout << "\nTitulo de la prueba: " << pruebas[i].getTitulo() << endl;
                const auto& items = pruebas[i].getItems();
                for (size_t j = 0; j < items.size(); ++j) {
                    cout << "  Item #" << j + 1 << " - Taxonomia: " << items[j].getTaxonomia() << endl;
                }
            }
        }
        pausaMenu();
    }

    void borrarItem() {
        if (pruebas.empty()) {
            cout << "No se han creado pruebas aun.\n";
            return;
        }

        while (true) {
            cout << "Seleccione la prueba donde desea borrar un item:\n";
            for (size_t i = 0; i < pruebas.size(); ++i) {
                cout << i + 1 << ") " << pruebas[i].getTitulo() << endl;
            }

            int indicePrueba = obtenerEntradaValida(1, pruebas.size()) - 1;
            auto& pruebaSeleccionada = pruebas[indicePrueba];

            const auto& items = pruebaSeleccionada.getItems();
            if (items.empty()) {
                cout << "La prueba seleccionada no tiene items.\n";
                break;
            }

            cout << "Prueba seleccionada: " << pruebaSeleccionada.getTitulo() << "\nItems disponibles:\n";
            for (size_t i = 0; i < items.size(); ++i) {
                cout << "  Item #" << i + 1 << " - Taxonomia: " << items[i].getTaxonomia() << endl;
            }

            cout << "Ingrese el numero del item que desea borrar: ";
            int numeroItem = obtenerEntradaValida(1, items.size());
            cout << "Esta seguro que quiere eliminar este item?\n1) Si\n2) No\n";
            int confirmacion = obtenerEntradaValida(1, 2);
            if (confirmacion == 1) {
                pruebaSeleccionada.getEditableItems().erase(pruebaSeleccionada.getEditableItems().begin() + (numeroItem - 1));
                cout << "Item eliminado exitosamente.\n";
            } else {
                cout << "Accion cancelada. El item no se eliminara.\n";
                continue; // Repetimos la seleccion
            }

            cout << "¿Desea hacer algo mas?\n";
            cout << "1) Borrar otro item\n";
            cout << "2) Seleccionar otra prueba\n";
            cout << "3) Regresar al menu principal\n";
            int opcion = obtenerEntradaValida(1, 3);

            if (opcion == 1) continue;
            if (opcion == 2) return;
            break; // Salimos al menu principal
        }
        pausaMenu();
    }
};

// Main
int main() {
    Menu menu;
    menu.lanzar();
    return 0;
}