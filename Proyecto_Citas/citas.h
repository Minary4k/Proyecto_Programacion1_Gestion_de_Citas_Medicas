#ifndef CITAS_H
#define CITAS_H

typedef struct {
	int id;
	char cedula[20];
	int codigoMedico;
	char fecha[20];
	char hora[20];
	char estado[20];           // Activa, Cancelada, No asistio, Reprogramada
	char diagnostico[100];
	char cuadroClinico[200];
	char tratamiento[200];
} Cita;

//Funciones de citas
void asignarCita();
int validarFecha(char fecha[]);
int validarHora(char hora[]);
int medicoDisponible(int codigoMedico, char fecha[], char hora[]);

void listarTodasLasCitas();
void listarCitasPorPaciente(char cedula[]);
void listarYGestionarCitasMedico(int codigoMedico);
void menuPaciente(char cedulaPaciente[]);
void limpiarBuffer();
int idCitaExiste(int id);

#endif
