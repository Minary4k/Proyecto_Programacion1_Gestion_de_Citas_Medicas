#ifndef MEDICOS_H
#define MEDICOS_H

typedef struct {
	int codigo;
	char nombre[50];
	char especialidad[30];
	char dia[15];
	char horaInicio[10];
	char horaFin[10];
} Medico;

void registrarMedico();
void listarMedicos();
int codigoRepetidoMedico(int codigo);
void menuMedico();
void mostrarEspecialidades();
int medicoExiste(int codigo);
int validarDia(char dia[]);
int validarHoraMedico(char hora[]);

#endif
