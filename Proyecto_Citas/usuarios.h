#ifndef USUARIOS_H
#define USUARIOS_H

struct Usuario {
	char nombreUsuario[30];
	char contrasena[30];
	int rol; // 1=Operador, 2=Medico, 3=Paciente
	char cedula[15]; 
};

extern char cedulaLogueada[20];

void limpiarPantalla();
void pausar();

//Funciones del sistema de login
void menuLogin(int *rolLogueado);
void registrarUsuario();
int iniciarSesion(int *rolLogueado);

//Menus por rol
void menuOperador();
void menuMedico();
void menuPaciente(char cedulaPaciente[]);

#endif
