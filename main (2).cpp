#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>  
using namespace std;

struct Fecha
{
    int dia;
    int mes;
    int año;
};


void mostrarTiempoRestante(const Fecha &fechaEvento)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

   
    ltm->tm_hour -= 7;
    if (ltm->tm_hour < 0)
    {
        ltm->tm_hour += 24;
        ltm->tm_mday -= 1;
    }

    
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

int main()
{
    string evento;
    int dia;
    int mes;
    int año;
    int opc;
    int intento = 1;
    int editar;
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
        cout << "(5) Salir" << '\n';
        cout << "Elige una opción: ";
        cin >> opc;

        if (opc == 1)
        {
            cout << "En que mes desea registrar este evento?: " << '\n';
            cin >> mes;

            cout << "En que dia desea registrar este evento?: " << '\n';
            cin >> dia;

            cout << "En que año desea registrar este evento?: " << '\n';
            cin >> año;
            cin.ignore();
            cout << "Describe el evento a registrar:";
            getline(cin, evento);

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
                cout << i + 1 << ". " << eventos[i] << " - Fecha: " << fechas[i].dia << '/' << fechas[i].mes << '/' << fechas[i].año;
                mostrarTiempoRestante(fechas[i]);
                cout << '\n';
            }

            cout << "Ingrese el número del evento que desea eliminar: ";
            int indiceEliminar;
            cin >> indiceEliminar;

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
            cin >> indiceEditar;

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
            }

            // Mostrar el tiempo restante para el evento editado
            mostrarTiempoRestante(fechas[indiceEditar - 1]);
        }
        else if (opc == 5)
        {
            cout << "Gracias por usar la agenda" << '\n';
            break;
        }
        else
        {
            cout << "Opción no válida" << '\n';
        }

        cout << "¿Deseas volver a usar el programa? \n ";
        cout << "(1) Si\n";
        cout << "(2) No\n";
        cin >> intento;

    } while (intento == 1);

    return 0;
}



