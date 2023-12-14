#include <iostream>
#include <vector>
#include <ctime>
#include <limits>
#include <algorithm>
#include <map>
#include <cmath>
#include <fstream>

using namespace std;

struct Fecha
{
    int dia;
    int mes;
    int año;
};

bool esLetra(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isalpha(c); });
}

void mostrarTiempoRestante(const Fecha &fechaEvento)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    tm fechaEventoTM = {0, 0, 0, fechaEvento.dia, fechaEvento.mes - 1, fechaEvento.año - 1900};

    time_t ahora = mktime(ltm);
    time_t tiempoEvento = mktime(&fechaEventoTM);

    // Calcular la diferencia en segundos
    double diferencia = difftime(tiempoEvento, ahora);

    if (diferencia < 0)
    {
        cout << "El evento ya ha ocurrido." << endl;
    }
    else
    {
        int dias = static_cast<int>(diferencia / (24 * 3600));
        int horas = static_cast<int>((fmod(diferencia, (24 * 3600))) / 3600);
        int minutos = static_cast<int>((fmod(diferencia, 3600)) / 60);

        cout << " - Tiempo restante para el evento: " << dias << " días, " << horas << " horas y " << minutos << " minutos." << endl;
    }
}
void guardarDatos(const string &nombreArchivo, const vector<string> &eventos, const vector<Fecha> &fechas)
{
    ofstream archivo(nombreArchivo, ios::app);

    if (!archivo.is_open())
    {
        cout << "Error al abrir el archivo para escritura." << endl;
        return;
    }

    for (size_t i = 0; i < eventos.size(); ++i)
    {
        archivo << "Evento: " << eventos[i] << " - Fecha: " << fechas[i].dia << '/' << fechas[i].mes << '/' << fechas[i].año << "\n";
    }

    archivo.close();
}

void leerDatosGuardados(const string &nombreArchivo)
{
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open())
    {
        cout << "No se pudo abrir el archivo para lectura." << endl;
        return;
    }

    string linea;
    cout << "Datos almacenados en el archivo:\n";
    while (getline(archivo, linea))
    {
        cout << linea << endl;
    }

    archivo.close();
}
int main()
{
    string evento;
    int dia;
    int mes;
    int año;
    int opc;
    vector<string> eventos;
    vector<Fecha> fechas;
    int indiceEditar;

    do
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        ltm->tm_hour -= 7;
        if (ltm->tm_hour < 0)
        {
            ltm->tm_hour += 24;
            ltm->tm_mday -= 1;
        }

        cout << "Hora actual: ";
        cout << (ltm->tm_hour < 10 ? "0" : "") << ltm->tm_hour << ":";
        cout << (ltm->tm_min < 10 ? "0" : "") << ltm->tm_min << ":";
        cout << (ltm->tm_sec < 10 ? "0" : "") << ltm->tm_sec << std::endl;

        cout << "--------------------------------------------------------------------\n";

        cout << "----Agenda De Eventos ----" << '\n';
        cout << "---Elije la opcion de tu interes:----" << '\n';
        cout << "(1) Registrar un evento" << '\n';
        cout << "(2) Ver eventos" << '\n';
        cout << "(3) Eliminar eventos existentes" << '\n';
        cout << "(4) Editar eventos" << '\n';
         cout << "(5) Leer datos guardados en archivo txt" << '\n';
        cout << "(6) Salir" << '\n';
        cout << "Elige una opción: ";

        while (!(cin >> opc))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Usa números, no letras, vuelve a intentar: ";
        }

        if (opc == 1)
        {
            cout << "En que mes desea registrar este evento?: " << '\n';

            while (!(cin >> mes) || mes > 12)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ingresa un mes válido (igual o mayor al actual y entre 1 y 12): ";
            }

            cout << "En que dia desea registrar este evento?: " << '\n';
            while (true)
            {
                while (!(cin >> dia))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Usa números no letras: ";
                }
                if (dia >= 1 && dia <= 31)
                {
                    break;
                }
                else
                {
                    cout << "Ingresa un día válido que sea igual o mayor al actual (entre 1 y 31): ";
                }
            }
            cout << "En que año desea registrar este evento?: " << '\n';
            cin >> año;
            
            cin.ignore();
            cout << "Describe el evento a registrar:";
            
             do {
    cin>>evento;
        if ( esLetra(evento)) {
            break;
        } else {
            std::cout << "Ingresa un apellido paterno válido (máximo 15 caracteres y solo letras): ";
        }
    } while (true);

            eventos.push_back(evento);

            Fecha fechaActual = {dia, mes, año};
            fechas.push_back(fechaActual);
        }
        else if (opc == 2)
        {
            cout << "Estos son tus eventos registrados: " << '\n';
            for (int i = 0; i < eventos.size(); ++i)
            {
                cout << i + 1 << ". " << eventos[i] << " - Fecha: " << fechas[i].dia << '/' << fechas[i].mes << '/' << fechas[i].año;
                mostrarTiempoRestante(fechas[i]);
                cout << '\n';
            }
        }
        else if (opc == 3)
        {
            cout << "Estos son tus eventos registrados: " << '\n';
            for (int i = 0; i < eventos.size(); ++i)
            {
                cout << i + 1 << ". " << eventos[i] << " - Fecha: " << fechas[i].dia << '/' << fechas[i].mes << '/' << fechas[i].año << " ";
                mostrarTiempoRestante(fechas[i]);
                cout << '\n';
            }

            cout << "Ingrese el número del evento que desea eliminar: ";
            int indiceEliminar;
             while (!(cin >> indiceEliminar))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Usa números, no letras, vuelve a intentar: ";
        }

            if (indiceEliminar >= 1 && indiceEliminar <= eventos.size())
            {
                eventos.erase(eventos.begin() + indiceEliminar - 1);
                fechas.erase(fechas.begin() + indiceEliminar - 1);
                cout << "Evento eliminado con éxito." << '\n';
            }
            else
            {
                cout << "Número de evento no válido." << '\n';
            }
        }
        else if (opc == 4)
        {
            for (int i = 0; i < eventos.size(); ++i)
            {
                cout << i + 1 << ". " << eventos[i] << " - Fecha: " << fechas[i].dia << '/' << fechas[i].mes << '/' << fechas[i].año;
                mostrarTiempoRestante(fechas[i]);
                cout << '\n';
            }

            cout << "Ingrese el número del evento que desea editar: ";
           
             while (!(cin >> indiceEditar))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Usa números, no letras, vuelve a intentar: ";
        }
            if (indiceEditar >= 1 && indiceEditar <= eventos.size())
            {
                cin.ignore();
                cout << "Sobre escriba el nuevo evento:" << '\n';
                getline(cin, evento);
                eventos[indiceEditar - 1] = evento;

                cout << "Nueva fecha para el evento: \n";
                cout << "Mes: ";
                cin >> fechas[indiceEditar - 1].mes;
                cout << "Día: ";
                cin >> fechas[indiceEditar - 1].dia;
                cout << "Año: ";
                cin >> fechas[indiceEditar - 1].año;
            }
            else
            {
                cout << "Número de evento no válido." << '\n';
                break;
            }

            // Mostrar el tiempo restante para el evento editado
            mostrarTiempoRestante(fechas[indiceEditar - 1]);
            
        }
          else if (opc == 5)
{
    cout << "Datos guardados: " << '\n';
    leerDatosGuardados("eventos.txt");
    cout << "Datos leídos correctamente." << '\n';
   
}

        else if (opc == 6)
        {
            cout << "Gracias por usar la agenda" << '\n';
            guardarDatos("eventos.txt", eventos, fechas);
            return 0;
        }
        else
        {
            cout << "Opción no válida" << '\n';
        }

       

    } while (opc != 6);

    return 0;
}
