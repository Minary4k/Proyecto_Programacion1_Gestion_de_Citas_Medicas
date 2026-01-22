#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pacientes.h"
#include "citas.h"
#include "usuarios.h"

int validarCedula(char cedula[]) {
	int i;
	
	if (strlen(cedula) != 10)
		return 0;
	
	for (i = 0; i < 10; i++) {
		if (cedula[i] < '0' || cedula[i] > '9') {
			return 0;
		}
	}
	
	return 1;
}

int cedulaExiste(char cedula[]) {
	FILE *archivo = fopen("pacientes.txt", "r");
	
	if (archivo == NULL) {
		return 0;
	}
	
	Paciente p;
	while (fscanf(archivo, "%[^|]|%[^|]|%d|%[^|]|%[^\n]\n", p.nombre, p.cedula, &p.edad, p.telefono, p.correo) == 5) {
		
		if (strcmp(p.cedula, cedula) == 0) {
			fclose(archivo);
			return 1;
		}
	}
	
	fclose(archivo);
	return 0;
}

void registrarPaciente() {
	Paciente p;
	
	limpiarPantalla();
	printf("=============================");
	printf("\n     Registrar Paciente    \n");
	printf("=============================\n");
	
	printf("Nombre: ");
	fgets(p.nombre, 50, stdin);
	p.nombre[strcspn(p.nombre, "\n")] = 0;
	
	printf("Cedula: ");
	fgets(p.cedula, 20, stdin);
	p.cedula[strcspn(p.cedula, "\n")] = 0;
	
	if (validarCedula(p.cedula) == 0) {
		printf("Cedula invalida.\n");
		return;
	}
	
	if (cedulaExiste(p.cedula)) {
		printf("La cedula ya esta registrada.\n");
		return;
	}
	
	printf("Edad: ");
	scanf("%d", &p.edad);
	limpiarBuffer();
	
	printf("Telefono: ");
	fgets(p.telefono, 20, stdin);
	p.telefono[strcspn(p.telefono, "\n")] = 0;
	
	printf("Correo: ");
	fgets(p.correo, 50, stdin);
	p.correo[strcspn(p.correo, "\n")] = 0;
	
	FILE *archivo = fopen("pacientes.txt", "a");
	
	if (archivo == NULL) {
		printf("No se pudo abrir el archivo.\n");
		return;
	}
	
	fprintf(archivo, "%s|%s|%d|%s|%s\n", p.nombre, p.cedula, p.edad, p.telefono, p.correo);
	
	fclose(archivo);
	
	printf("Paciente registrado correctamente.\n");
}

void listarPacientes() {
	FILE *archivo = fopen("pacientes.txt", "r");
	Paciente p;
	
	limpiarPantalla();
	printf("=============================");
	printf("\n     Lista de Pacientes    \n");
	printf("=============================\n");
	
	if (archivo == NULL) {
		printf("No hay pacientes registrados.\n");
		return;
	}
	
	while (fscanf(archivo, "%[^|]|%[^|]|%d|%[^|]|%[^\n]\n", p.nombre, p.cedula, &p.edad, p.telefono, p.correo) == 5) {
		
		printf("\nNombre: %s\n", p.nombre);
		printf("Cedula: %s\n", p.cedula);
		printf("Edad: %d\n", p.edad);
		printf("Telefono: %s\n", p.telefono);
		printf("Correo: %s\n", p.correo);
		printf("--------------------------\n");
	}
	
	fclose(archivo);
}
