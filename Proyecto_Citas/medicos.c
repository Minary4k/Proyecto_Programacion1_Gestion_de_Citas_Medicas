#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "medicos.h"
#include "citas.h"
#include "usuarios.h"

int validarDia(char dia[]) {
	int i;
	if (strlen(dia) == 0) return 0;
	
	for (i = 0; dia[i] != '\0'; i++) {
		if (!((dia[i] >= 'A' && dia[i] <= 'Z') ||
			(dia[i] >= 'a' && dia[i] <= 'z') ||
			dia[i] == ' ')) {
			return 0;
		}
	}
	return 1;
}

int validarHoraMedico(char hora[]) {
	int h, m;
	if (sscanf(hora, "%d:%d", &h, &m) != 2)
		return 0;
	
	if (h < 0 || h > 23 || m < 0 || m > 59)
		return 0;
	
	return 1;
}


void mostrarEspecialidades() {
	printf("\nSeleccione la especialidad:\n");
	printf("1. Medicina General\n");
	printf("2. Pediatria\n");
	printf("3. Cardiologia\n");
	printf("4. Dermatologia\n");
}

//MENU MEDICO

void menuMedico() {
	int opcion;
	int codigoMedico;
	
	printf("\nIngrese su codigo de medico: ");
	scanf("%d", &codigoMedico);
	limpiarBuffer();
	
	do {
		limpiarPantalla();
		printf("=============================");
		printf("\n        MENU MEDICO        \n");
		printf("=============================\n");
		printf("1. Listar y gestionar mis citas\n");
		printf("2. Salir\n");
		printf("Opcion: ");
		scanf("%d", &opcion);
		limpiarBuffer();
		
		switch (opcion) {
		case 1:
			listarYGestionarCitasMedico(codigoMedico);
			pausar();
			limpiarPantalla();
			break;
		case 2:
			printf("Usted acaba de salir del menu medico.\n");
			limpiarPantalla();
			break;
		default:
			printf("Opcion invalida.\n");
		}
	} while (opcion != 2);
}

int codigoRepetidoMedico(int codigo) {
	FILE *archivo = fopen("medicos.txt", "r");
	
	if (archivo == NULL) {
		return 0;
	}
	
	Medico medico;
	
	while (fscanf(archivo, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", &medico.codigo, medico.nombre, medico.especialidad, medico.dia, medico.horaInicio, medico.horaFin) == 6) {
		if (medico.codigo == codigo) {
			fclose(archivo);
			return 1;
		}
	}
	
	fclose(archivo);
	return 0;
}

void registrarMedico() {
	Medico medico;
	int opcionEspecialidad;
	
	limpiarPantalla();
	printf("=============================");
	printf("\n     Registrar Medico      \n");
	printf("=============================\n");
	
	printf("Codigo: ");
	scanf("%d", &medico.codigo);
	limpiarBuffer();
	
	if (codigoRepetidoMedico(medico.codigo)) {
		printf("El codigo ya esta registrado.\n");
		return;
	}
	
	printf("Nombre: ");
	fgets(medico.nombre, 50, stdin);
	medico.nombre[strcspn(medico.nombre, "\n")] = 0;

	mostrarEspecialidades();
	printf("Opcion: ");
	scanf("%d", &opcionEspecialidad);
	limpiarBuffer();
	
	switch (opcionEspecialidad) {
	case 1: strcpy(medico.especialidad, "Medicina General"); break;
	case 2: strcpy(medico.especialidad, "Pediatria"); break;
	case 3: strcpy(medico.especialidad, "Cardiologia"); break;
	case 4: strcpy(medico.especialidad, "Dermatologia"); break;
	default:
		printf("Opcion invalida.\n");
		return;
	}
	
	do {
		printf("Dia de atencion (ej: Lunes): ");
		fgets(medico.dia, 15, stdin);
		medico.dia[strcspn(medico.dia, "\n")] = 0;
		
		if (!validarDia(medico.dia))
			printf("Dia invalido. Use solo letras.\n");
		
	} while (!validarDia(medico.dia));
	
	do {
		printf("Hora de inicio (hh:mm): ");
		fgets(medico.horaInicio, 10, stdin);
		medico.horaInicio[strcspn(medico.horaInicio, "\n")] = 0;
		
		if (!validarHoraMedico(medico.horaInicio))
			printf("Formato invalido. Ej: 08:30\n");
		
	} while (!validarHoraMedico(medico.horaInicio));
	
	do {
		printf("Hora de fin (hh:mm): ");
		fgets(medico.horaFin, 10, stdin);
		medico.horaFin[strcspn(medico.horaFin, "\n")] = 0;
		
		if (!validarHoraMedico(medico.horaFin))
			printf("Formato invalido. Ej: 17:00\n");
		
	} while (!validarHoraMedico(medico.horaFin));
	
	FILE *archivo = fopen("medicos.txt", "a");
	
	if (archivo == NULL) {
		printf("No se pudo abrir el archivo.\n");
		return;
	}
	
	fprintf(archivo, "%d|%s|%s|%s|%s|%s\n", medico.codigo, medico.nombre, medico.especialidad, medico.dia, medico.horaInicio, medico.horaFin);
	
	fclose(archivo);
	printf("Medico registrado correctamente.\n");
}

void listarMedicos() {
	FILE *archivo = fopen("medicos.txt", "r");
	
	if (archivo == NULL) {
		printf("No hay medicos registrados.\n");
		return;
	}
	
	Medico medico;
	
	limpiarPantalla();
	printf("=============================");
	printf("\n     Lista de Medicos      \n");
	printf("=============================\n");
	
	while (fscanf(archivo, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", &medico.codigo, medico.nombre, medico.especialidad, medico.dia, medico.horaInicio, medico.horaFin) == 6) {
		
		printf("\nCodigo: %d\n", medico.codigo);
		printf("Nombre: %s\n", medico.nombre);
		printf("Especialidad: %s\n", medico.especialidad);
		printf("Dia: %s\n", medico.dia);
		printf("Horario: %s - %s\n", medico.horaInicio, medico.horaFin);
		printf("--------------------------\n");
	}
	
	fclose(archivo);
}

int medicoExiste(int codigo) {
	FILE *archivo = fopen("medicos.txt", "r");
	Medico medico;
	
	if (archivo == NULL) {
		return 0;
	}
	
	while (fscanf(archivo, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", &medico.codigo, medico.nombre, medico.especialidad, medico.dia, medico.horaInicio, medico.horaFin) == 6) {
		
		if (medico.codigo == codigo) {
			fclose(archivo);
			return 1;
		}
	}
	
	fclose(archivo);
	return 0;
}
