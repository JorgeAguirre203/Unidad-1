#include <iostream>
#include <vector>
#include <ctime>
#include <limits>
#include <algorithm>
#include <map>
#include <cmath>
#include <fstream>

using namespace std;

template<typename T>
using vector2 = std::vector<T>;

class Fecha {
public:
    int dia;
    int mes;
    int año;
};

struct Paciente {
    string nombre;
    string apellidoP;
    string apellidoM;
    string sexo;
    char opcsex;
    int edad;
    Fecha fechaCita;
    string doctor;
};
struct Consultorio {
    string doctor;
    int horarioInicio;
    int horarioFin;
    string Turno;
};
using ConsultorioMap = std::map<string, string>;

bool esLetra(const std::string &str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isalpha(c); });
}


void mostrarHoraActual() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    ltm->tm_hour -= 7;
    if (ltm->tm_hour < 0) {
        ltm->tm_hour += 24;
        ltm->tm_mday -= 1;
    }

    cout << "Hora actual: "
         << (ltm->tm_hour < 10 ? "0" : "") << ltm->tm_hour << ":"
         << (ltm->tm_min < 10 ? "0" : "") << ltm->tm_min << ":"
         << (ltm->tm_sec < 10 ? "0" : "") << ltm->tm_sec << std::endl;
}

void mostrardiferencia(const Fecha &fechaEvento) {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    tm fechaEventoTM = {0, 0, 0, fechaEvento.dia, fechaEvento.mes - 1, fechaEvento.año - 1900};

    time_t ahora = mktime(ltm);
    time_t tiempoEvento = mktime(&fechaEventoTM);

    // Calcular la diferencia en segundos
    double diferencia = difftime(tiempoEvento, ahora);

    if (diferencia < 0) {
        cout << "El evento ya ha ocurrido." << endl;
    } else {
        int dias = static_cast<int>(diferencia / (24 * 3600));
        int horas = static_cast<int>((fmod(diferencia, (24 * 3600))) / 3600);
        int minutos = static_cast<int>((fmod(diferencia, 3600)) / 60);

        cout << " - Tiempo restante para el evento: " << dias << " días, " << horas << " horas y " << minutos << " minutos." << endl;
    }
}

void registrarDoctores(ConsultorioMap &doctores) {
    int cantidadDoctores;
    string especialidad;
int obtenerHorarioFin(const std::string &horario);
    cout << "Ingrese la cantidad de doctores: ";
    while (!(cin >> cantidadDoctores) || cantidadDoctores <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ingrese una cantidad válida de doctores (número entero mayor que 0): ";
    }

    int numeroConsultorio = 1;

    for (int i = 0; i < cantidadDoctores; ++i) {
        string nombreDoctor;
        cout << "Ingrese el nombre del doctor " << i + 1 << ": ";
        do {
            cin >> nombreDoctor;
            if (esLetra(nombreDoctor)) {
                break;
            } else {
                cout << "Escribe solo letras no números: ";
            }
        } while (true);

        cout << "Ingrese la especialidad del doctor " << nombreDoctor << ": ";
        do {
            cin >> especialidad;
            if (esLetra(especialidad)) {
                break;
            } else {
                cout << "Escribe solo letras no números: ";
            }
        } while (true);

        string nuevonombredoctor;
        string espacio = " Con especialidad en: ";

        nuevonombredoctor = nombreDoctor + espacio + especialidad;

        // Forma el nombre del consultorio con el número asignado
        string consultorio = "consultorio " + to_string(numeroConsultorio);

        // Solicita la hora de inicio
        int horarioInicio;
        int horarioFin;

        if (doctores.find(consultorio) != doctores.end()) {
            // Si ya hay un doctor en este consultorio, obtenemos el horario de fin del último doctor
            auto ultimoDoctor = doctores.find(consultorio);
            horarioFin = obtenerHorarioFin(ultimoDoctor->second);
            cout << "Ingrese la hora de inicio (en formato de 12 horas) para " << nuevonombredoctor << ": ";
            while (!(cin >> horarioInicio) || horarioInicio < horarioFin + 1 || horarioInicio > 12) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ingresa una hora válida (después de " << horarioFin << " y antes de las 12): ";
            }
        } else {
            // Si es el primer doctor en este consultorio, solicita la hora de inicio normalmente
            cout << "Ingrese la hora de inicio (en formato de 12 horas) para " << nuevonombredoctor << ": ";
            while (!(cin >> horarioInicio) || horarioInicio < 1 || horarioInicio > 12) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ingresa una hora válida (entre 1 y 12): ";
            }
        }

        // Calcula la hora de fin del turno
        horarioFin = (horarioInicio + 8) % 12;
        string ampm = (horarioInicio + 8) < 12 ? "AM" : "PM";

        // Almacena el horario en el ConsultorioMap
        doctores[nuevonombredoctor] = consultorio + " de " + to_string(horarioInicio) + ":00 " + ampm +
                                       " a " + to_string(horarioFin) + ":00 " + ampm;

        // Incrementa el número de consultorio si ya se han asignado dos doctores al actual
        if (i % 2 == 1) {
            numeroConsultorio++;
        }
    }
}

int obtenerHorarioFin(const string &horario) {
    // La función obtiene el horario de fin del último doctor en un consultorio
    size_t inicio = horario.find(" a ") + 3;
    size_t fin = horario.find(":00");
    return stoi(horario.substr(inicio, fin - inicio));
}

void registrardoctor(vector2<Consultorio> &consultorios, ConsultorioMap &doctores) {
    int opcion3;

    cout << "Doctores disponibles: \n";
    int i = 1;
    for (const auto &doctor : doctores) {
        cout << "(" << i << ") " << doctor.first << " en consultorio " << doctor.second << endl;
        i++;
    }

    cout << "Elije el número del doctor que deseas para tu cita: \n";
    while (!(cin >> opcion3) || opcion3 < 1 || opcion3 > doctores.size()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Elije una opción válida: ";
    }

    Consultorio nuevoConsultorio;
    auto it = doctores.begin();
    advance(it, opcion3 - 1);
    nuevoConsultorio.doctor = it->first;
    consultorios.push_back(nuevoConsultorio);
}

void registrarPaciente(vector<Paciente> &pacientes, const ConsultorioMap &doctores) {
    Paciente nuevoPaciente;

    cout << "Ingrese el nombre del paciente al que se hará la cita (máximo 15 caracteres): ";
    do {
        cin >> nuevoPaciente.nombre;
        if (nuevoPaciente.nombre.length() <= 15 && esLetra(nuevoPaciente.nombre)) {
            break;
        } else {
            std::cout << "Ingresa un nombre válido (máximo 15 caracteres y solo letras): ";
        }
    } while (true);

    cout << "Ingrese el apellido paterno del paciente (máximo 15 caracteres): ";
    do {
        cin >> nuevoPaciente.apellidoP;
        if (nuevoPaciente.apellidoP.length() <= 15 && esLetra(nuevoPaciente.apellidoP)) {
            break;
        } else {
            std::cout << "Ingresa un apellido paterno válido (máximo 15 caracteres y solo letras): ";
        }
    } while (true);

    cout << "Ingrese el apellido materno del paciente (máximo 15 caracteres): ";
    do {
        cin >> nuevoPaciente.apellidoM;
        if (nuevoPaciente.apellidoM.length() <= 15 && esLetra(nuevoPaciente.apellidoM)) {
            break;
        } else {
            cout << "Ingresa un apellido materno válido (máximo 15 caracteres y solo letras): ";
        }
    } while (true);

    cout << "Sexo del paciente:\n";
    cout<<"M = Mujer - H = Hombre - O = Otro:\n";
    char opcsex;
    do {
       
         cin >> opcsex;
         opcsex = std::tolower(opcsex);
   switch (opcsex){
       
       case 'm':
       {
          nuevoPaciente.sexo = "Mujer";
          break;
       }
       case 'h':
       {
           nuevoPaciente.sexo = "Hombre";
           break;
       }
       case 'o':
       {
           nuevoPaciente.sexo = "Otro";
           break;
       }
       default:
       {
           cout<<"Elije una opcion valida";
           break;
       }
                 }
     
    }while  (opcsex != 'h' && opcsex != 'm' && opcsex != 'o');
  
    cout << "Edad del paciente:";
    do {
        if (cin >> nuevoPaciente.edad) {
            if (nuevoPaciente.edad >= 0 && nuevoPaciente.edad <= 125) {
                break;
            } else {
                cout << "Ingresa una edad válida (número entero positivo entre 0 y 125): ";
            }
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Usa numeros no letras: ";
        }
    } while (true);
  time_t now = time(0);
    tm *ltm = localtime(&now);
    int mesActual = ltm->tm_mon + 1;  // tm_mon devuelve el mes de 0 a 11, por lo que se suma 1

    cout << "A continuación, añade la fecha a la que se registrará la cita:\n";
    cout << "Mes: ";

    while (!(cin >> nuevoPaciente.fechaCita.mes) || nuevoPaciente.fechaCita.mes < mesActual || nuevoPaciente.fechaCita.mes > 12) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ingresa un mes válido  y actual (igual o mayor al actual y entre 1 y 12): ";
    }

int diaactual = ltm->tm_mday;
    cout << "Día: ";
    while (true) {
        while (!(cin >> nuevoPaciente.fechaCita.dia)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Usa números no letras: ";
        }

       if ((nuevoPaciente.fechaCita.dia >= 1 && nuevoPaciente.fechaCita.dia <= 31) && (nuevoPaciente.fechaCita.dia >= diaactual)) {
        break;
    } else {
        cout << "Ingresa un día válido que sea igual o mayor al actual (entre 1 y 31): ";
    }
    }

  
    cout << "Año: ";
    do {
        if (cin >> nuevoPaciente.fechaCita.año) {
    if (nuevoPaciente.fechaCita.año >= 2023) {
        // Año válido
        
    } else {
        cout << "Ingresa un año válido (Fecha mayor o igual a 2023): ";
    }
        } else {
         // La entrada no es un número
         cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ingresa un año válido (número entero): ";
        }
        }while(nuevoPaciente.fechaCita.año < 2023);

  
    auto it = doctores.begin();
   
    nuevoPaciente.doctor = it->first;
    pacientes.push_back(nuevoPaciente);
}

void verPacientes(const vector<Paciente> &pacientes, const vector<Consultorio> &consultorios) {
    cout << "Tus citas registradas son:" << endl;
    for (const Paciente &paciente : pacientes) {
        cout << "Doctor a cargo: " << paciente.doctor << "\n"
             << "Nombre: " << paciente.nombre << ", Apellido Paterno: " << paciente.apellidoP
             << ", Apellido Materno: " << paciente.apellidoM << ", Sexo del paciente: " << paciente.sexo
             << ", Edad del paciente: " << paciente.edad << "\n"
             << "Fecha: \n" << paciente.fechaCita.dia << "/" << paciente.fechaCita.mes << "/"
             << paciente.fechaCita.año << endl;
    }
}

void verdoctores(vector2<Consultorio> &consultorios, ConsultorioMap &doctores) {
    cout << "Tus doctores registrados son:" << endl;

    int i = 1;
    for (const auto &doctor : doctores) {
        cout << "(" << i << ") " << doctor.first << " En " << doctor.second << endl;
        i++;
    }
}

void eliminar(vector<Paciente> &pacientes) {
    cout << "Estas son tus citas registradas: " << '\n';
    for (int i = 0; i < pacientes.size(); ++i) {
        cout << i + 1 << ". "
             << "Cita a nombre de: \n"
             << pacientes[i].nombre << "\n Doctor:\n  " << pacientes[i].doctor << "\n"
             << "Fecha:\n"
             << pacientes[i].fechaCita.dia << "/" << pacientes[i].fechaCita.mes << "/"
             << pacientes[i].fechaCita.año << endl;
        cout << '\n';
    }

    cout << "Ingrese el número del evento que desea eliminar: ";
    int indiceEliminar;
    cin >> indiceEliminar;

    if (indiceEliminar >= 1 && indiceEliminar <= pacientes.size()) {
        pacientes.erase(pacientes.begin() + indiceEliminar - 1);
        cout << "Cita eliminada con éxito." << '\n';
    } else {
        cout << "Número de Cita no válido." << '\n';
    }
}

void editarcita(vector<Paciente> &pacientes, const vector<Consultorio> &consultorio) {
    cout << "Elije la cita a editar: \n";
    cout << "Estas son tus citas registradas: " << '\n';
    for (int i = 0; i < pacientes.size(); ++i) {
        cout << i + 1 << ". "
             << "Cita a nombre de: \n"
             << pacientes[i].nombre << "\n Doctor:\n  " << pacientes[i].doctor << "\n"
             << "Fecha:\n"
             << pacientes[i].fechaCita.dia << "/" << pacientes[i].fechaCita.mes << "/"
             << pacientes[i].fechaCita.año << endl;
        cout << '\n';
    }

    cout << "Ingrese el número de la cita que desea editar: ";
    int indiceEditar;
    cin >> indiceEditar;

    if (indiceEditar >= 1 && indiceEditar <= pacientes.size()) {
        // Opciones de edición
        cout << "Selecciona el dato a editar: \n";
        cout << "(1) Nombre\n";
        cout << "(2) Apellido Paterno\n";
        cout << "(3) Apellido Materno\n";
        cout << "(4) Sexo\n";
        cout << "(5) Edad\n";
        cout << "(6) Fecha\n";

        int opcionEditar;
        while (!(cin >> opcionEditar)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Usa números no letras: ";
        }

        switch (opcionEditar) {
            case 1:
                cout << "Nuevo nombre: ";
                cin >> pacientes[indiceEditar - 1].nombre;
                break;
            case 2:
                cout << "Nuevo apellido paterno: ";
                cin >> pacientes[indiceEditar - 1].apellidoP;
                break;
            case 3:
                cout << "Nuevo apellido materno: ";
                cin >> pacientes[indiceEditar - 1].apellidoM;
                break;
            case 4:
                cout << "Nuevo sexo: ";
               
               
    cout<<"M = Mujer - H = Hombre - O = Otro:\n";
    char opcsex;
    do {
       
         cin >> opcsex;
         opcsex = std::tolower(opcsex);
   switch (opcsex){
       
       case 'm':
       {
           pacientes[indiceEditar - 1].sexo = "Mujer";
          break;
       }
       case 'h':
       {
            pacientes[indiceEditar - 1].sexo= "Hombre";
           break;
       }
       case 'o':
       {
            pacientes[indiceEditar - 1].sexo = "Otro";
           break;
       }
       default:
       {
           cout<<"Elije una opcion valida";
           break;
       }
                 }
     
    }while  (opcsex != 'h' && opcsex != 'm' && opcsex != 'o');
                break;
            case 5:
                cout << "Nueva edad: ";
                while (!(cin >> pacientes[indiceEditar - 1].edad)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Usa números no letras: ";
                }
                break;
            case 6:
                cout << "Nueva fecha:\n";
                cout << "Mes: ";
                while (!(cin >> pacientes[indiceEditar - 1].fechaCita.mes) || pacientes[indiceEditar - 1].fechaCita.mes < 1 || pacientes[indiceEditar - 1].fechaCita.mes > 12) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Ingresa un mes válido (entre 1 y 12): ";
                }
                cout << "Día: ";
                while (true) {
                    while (!(cin >> pacientes[indiceEditar - 1].fechaCita.dia)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Usa números no letras: ";
                    }

                    if (pacientes[indiceEditar - 1].fechaCita.dia >= 1 && pacientes[indiceEditar - 1].fechaCita.dia <= 31) {
                        break;
                    } else {
                        cout << "Ingresa un día válido (entre 1 y 31): ";
                    }
                }
                cout << "Año: ";
                while (!(cin >> pacientes[indiceEditar - 1].fechaCita.año)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Usa números no letras: ";
                }
                break;
            default:
                cout << "Opción no válida." << endl;
        }

        cout << "Cita editada con éxito." << '\n';
    } else {
        cout << "Número de Cita no válido." << '\n';
    }
}
void guardarDatos(const string &nombreArchivo, const ConsultorioMap &doctores) {
    ofstream archivo(nombreArchivo, ios::app);

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo para escritura." << endl;
        return;
    }

    // Guardar información de los doctores y consultorios
    for (const auto &doctor : doctores) {
        archivo << "Doctor: " << doctor.first << " Consultorio: " << doctor.second << "\n";
    }

    archivo.close();
}

void guardarDatosPacientes(const string &nombreArchivo, const vector<Paciente> &pacientes) {
    ofstream archivo(nombreArchivo, ios::app);

    // Guardar información de los pacientes
    archivo << "Pacientes:\n";
    for (const Paciente &paciente : pacientes) {
        archivo << "Nombre: " << paciente.nombre << " Apellido Paterno: " << paciente.apellidoP
                << " Apellido Materno: " << paciente.apellidoM << " Sexo: " << paciente.sexo
                << " Edad: " << paciente.edad << " Doctor: " << paciente.doctor
                << " Fecha: " << paciente.fechaCita.dia << "/" << paciente.fechaCita.mes
                << "/" << paciente.fechaCita.año << "\n";
    }

    archivo.close();
}

void leerDatosGuardados() {
    ifstream archivo("datos_pacientes.txt");

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo para lectura." << endl;
        return;
    }

    string linea;
    cout << "Datos almacenados en el archivo:\n";
    while (getline(archivo, linea)) {
        cout << linea << endl;
    }

    archivo.close();
}
void leerDatosGuardados2() {
    ifstream archivo2("datos_doctores.txt");

    if (!archivo2.is_open()) {
        cout << "No se pudo abrir el archivo para lectura." << endl;
        return;
    }

    string linea;
    cout << "Datos almacenados en el archivo:\n";
    while (getline(archivo2, linea)) {
        cout << linea << endl;
    }

    archivo2.close();
}


int main() {
    vector2<Consultorio> consultorios;
    vector<Paciente> pacientes;
    ConsultorioMap doctores;
    int opc;

    do {
          
        mostrarHoraActual();

        cout << "--------------------------------------------------------------------\n";
        cout << "----Agenda De Médica ----" << '\n';
        cout << "---Elige la opción de tu interés:----" << '\n';
        cout << "(1) Ingresar doctores" << '\n';
        cout << "(2) Registrar una cita" << '\n';
        cout << "(3) Ver citas" << '\n';
        cout << "(4) Eliminar citas  " << '\n';
        cout << "(5) Editar citas  " << '\n';
        cout << "(6) Ver doctores registrados" << '\n';
        cout << "(7) Vaciar archivos txt" << '\n';
        cout << "(8) Salir" << '\n';

        while (!(std::cin >> opc)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Usa números, no letras, vuelve a intentar: ";
        }

        switch (opc) {
            case 1: {
            
                         registrarDoctores(doctores);
                         break;
            }
            case 2: {
                if (doctores.empty()) {
                    cout << "Primero debes registrar al menos un doctor." << endl;
                    
                } else {
                    registrardoctor(consultorios, doctores);
                    registrarPaciente(pacientes, doctores);
                   
                }
                break;
            }
            case 3: {
               if (pacientes.empty()) {
             cout << "No has registrado citas recientemente..." << endl;
                } else {
             verPacientes(pacientes, consultorios);
             mostrardiferencia(pacientes.back().fechaCita);
                }

                int ver;
                cout << "Deseas ver las citas registradas en el archivo txt? \n";
                cout << "(1)-Si (2)-No \n";

                    // Bucle para obtener una entrada válida
                while (!(cin >> ver) || (ver != 1 && ver != 2)) {
                 cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                 cout << "Ingresa una opción válida (1 o 2): ";
                    }       

                 if (ver == 1) {
                 leerDatosGuardados();
                    } else {
                     cout << "Regresando al menú..." << endl;
                    }
                    break;
            }
            case 4: {
                if (pacientes.empty()) {
                    cout << "No hay citas registradas." << endl;
                } else {
                    eliminar(pacientes);
                }
                break;
            }
            case 5: {
                if (pacientes.empty()) {
                    cout << "No hay citas registradas recientemente, buscando archivo txt..." << endl;
                  
                } else {
                    editarcita(pacientes, consultorios);
                }
                 leerDatosGuardados();
                break;
            }

            case 6: {
                if (doctores.empty()) {
                    cout << "No hay doctores registrados." << endl;
                } else {
                    verdoctores(consultorios, doctores);
                    
                }
                 leerDatosGuardados2();
            }
            break;
              case 7: {
                  
                  int borrar;
                  cout<<"Que archivo se vaciara?: \n";
                  cout<<"(1)..Doctores\n (2)..Pacientes \n (3)..Ambos\n";
                  cin>>borrar;
                  if(borrar ==1 ){
                         const std::string nombreArchivo = "datos_doctores.txt";
                std::ofstream archivo(nombreArchivo);
                 archivo.close();
                  std::cout << "El archivo '" << nombreArchivo << "' se ha vaciado." << std::endl;
                  }if (borrar == 2)
                  {
                       
                  const std::string nombreArchivo2 = "datos_pacientes.txt";
                std::ofstream archivo2(nombreArchivo2);
                 archivo2.close();
                  std::cout << "El archivo '" << nombreArchivo2 << "' se ha vaciado." << std::endl; 
                  }if (borrar == 3 ){
                      
                        const std::string nombreArchivo = "datos_doctores.txt";
                std::ofstream archivo(nombreArchivo);
                 archivo.close();
                  std::cout << "El archivo '" << nombreArchivo << "' se ha vaciado." << std::endl;
                  
                     const std::string nombreArchivo2 = "datos_pacientes.txt";
                std::ofstream archivo2(nombreArchivo2);
                 archivo2.close();
                      
                  }else {
                      cout<<"opcion invalida\n";
                  }
                      
                
                    }
            break;
            
            case 8: {
                 guardarDatos("datos_doctores.txt", doctores);
    guardarDatosPacientes("datos_pacientes.txt", pacientes);

                return 0;
            }
            default: {
                cout << "Opción no válida" << endl;
            }
        }

    } while (opc != 8);

    return 0;
}
