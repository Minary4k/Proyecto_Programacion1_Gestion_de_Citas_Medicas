#ifndef PACIENTES_H
#define PACIENTES_H

typedef struct {
	char nombre[50];
	char cedula[20];
	int edad;
	char telefono[15];
	char correo[50];
} Paciente;

void registrarPaciente();
void listarPacientes();
int cedulaExiste(char cedula[]);
int validarCedula(char cedula[]);

#endif
