#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "citas.h"
#include "usuarios.h"
#include "pacientes.h"
#include "medicos.h"

void limpiarBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

//FUNCIONES DE VALIDACION
int idCitaExiste(int id) {
	FILE *archivo = fopen("citas.txt", "r");
	Cita cita;
	
	if (archivo == NULL) {
		return 0;
	}
	
	while (fscanf(archivo, "%d|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", &cita.id, cita.cedula, &cita.codigoMedico, cita.fecha, cita.hora, cita.estado, cita.diagnostico, cita.cuadroClinico, cita.tratamiento) == 9) {
		
		if (cita.id == id) {
			fclose(archivo);
			return 1;
		}
	}
	
	fclose(archivo);
	return 0;
}

int validarHora(char hora[]) {
	int h, m;
	if (sscanf(hora, "%d:%d", &h, &m) != 2) {
		return 0;
	}
	if (h < 0 || h > 23 || m < 0 || m > 59) {
		return 0;
	}
	return 1;
}

int validarFecha(char fecha[]) {
	int d, m, a;
	if (sscanf(fecha, "%d/%d/%d", &d, &m, &a) != 3) {
		return 0;
	}
	if (d < 1 || d > 31 || m < 1 || m > 12 || a < 2024) {
		return 0;
	}
	return 1;
}

//VERIFICAR DISPONIBILIDAD
int medicoDisponible(int codigoMedico, char fecha[], char hora[]) {
	FILE *archivo = fopen("citas.txt", "r");
	Cita cita;
	
	if (archivo == NULL) {
		return 1;
	}
	
	while (fscanf(archivo, "%d|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", &cita.id, cita.cedula, &cita.codigoMedico, cita.fecha, cita.hora, cita.estado, cita.diagnostico, cita.cuadroClinico, cita.tratamiento) == 9) {
		
		if (cita.codigoMedico == codigoMedico &&
			strcmp(cita.fecha, fecha) == 0 &&
			strcmp(cita.hora, hora) == 0 &&
			strcmp(cita.estado, "ACTIVA") == 0) {
			
			fclose(archivo);
			return 0;
		}
	}
	
	fclose(archivo);
	return 1;
}

//ASIGNAR CITA
void asignarCita() {
	Cita cita;
	FILE *archivo;
	int existe;
	
	limpiarPantalla();
	printf("=============================");
	printf("\n       ASIGNAR CITA       \n");
	printf("=============================\n");
	
	do {
		printf("ID de la cita: ");
		scanf("%d", &cita.id);
		limpiarBuffer();
		
		existe = idCitaExiste(cita.id);
		
		if (existe)
			printf("Ese ID ya existe. Intente otro.\n");
		
		if (cita.id <= 0)
			printf("El ID debe ser mayor a 0.\n");
		
	} while (existe || cita.id <= 0);
	
	printf("Cedula del paciente: ");
	fgets(cita.cedula, 20, stdin);
	cita.cedula[strcspn(cita.cedula, "\n")] = 0;
	
	if (!validarCedula(cita.cedula)) {
		printf("Cedula invalida.\n");
		return;
	}
	
	if (!cedulaExiste(cita.cedula)) {
		printf("El paciente no existe.\n");
		return;
	}
	
	printf("Codigo del medico: ");
	scanf("%d", &cita.codigoMedico);
	limpiarBuffer();
	
	if (!medicoExiste(cita.codigoMedico)) {
		printf("El medico no existe.\n");
		return;
	}
	
	do {
		printf("Fecha (dd/mm/aaaa): ");
		fgets(cita.fecha, 20, stdin);
		cita.fecha[strcspn(cita.fecha, "\n")] = 0;
		
		if (!validarFecha(cita.fecha)) {
			printf("Formato de fecha incorrecto.\n");
			continue;
		}
		
		printf("Hora (hh:mm): ");
		fgets(cita.hora, 20, stdin);
		cita.hora[strcspn(cita.hora, "\n")] = 0;
		
		if (!validarHora(cita.hora)) {
			printf("Formato de hora incorrecto.\n");
			continue;
		}
		
		if (!medicoDisponible(cita.codigoMedico, cita.fecha, cita.hora)) {
			printf("El medico no esta disponible en ese horario.\n");
			continue;
		}
		
		break;
		
	} while (1);
	
	strcpy(cita.estado, "ACTIVA");
	strcpy(cita.diagnostico, "Pendiente");
	strcpy(cita.cuadroClinico, "Pendiente");
	strcpy(cita.tratamiento, "Pendiente");
	
	archivo = fopen("citas.txt", "a");
	if (archivo == NULL) {
		printf("Error al abrir archivo.\n");
		return;
	}
	
	fprintf(archivo, "%d|%s|%d|%s|%s|%s|%s|%s|%s\n", cita.id, cita.cedula, cita.codigoMedico, cita.fecha, cita.hora, cita.estado, cita.diagnostico, cita.cuadroClinico, cita.tratamiento);
	
	fclose(archivo);
	printf("Cita asignada correctamente.\n");
	
}

//LISTAR TODAS
void listarTodasLasCitas() {
	FILE *archivo = fopen("citas.txt", "r");
	Cita cita;
	
	if (archivo == NULL) {
		printf("No hay citas registradas.\n");
		return;
	}
	
	limpiarPantalla();
	printf("==============================");
	printf("\n  LISTA DE TODAS LAS CITAS  \n");
	printf("==============================\n");
	
	while (fscanf(archivo, "%d|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", &cita.id, cita.cedula, &cita.codigoMedico, cita.fecha, cita.hora, cita.estado, cita.diagnostico, cita.cuadroClinico, cita.tratamiento) == 9){
		
		printf("ID Cita: %d\n", cita.id);
		printf("Cedula Paciente: %s\n", cita.cedula);
		printf("Codigo Medico: %d\n", cita.codigoMedico);
		printf("Fecha: %s\n", cita.fecha);
		printf("Hora: %s\n", cita.hora);
		printf("Estado: %s\n", cita.estado);
		printf("--------------------------\n");
	}
	
	fclose(archivo);
}

//PACIENTE
void listarCitasPorPaciente(char cedula[]) {
	FILE *archivo = fopen("citas.txt", "r");
	Cita cita;
	int encontrado = 0;
	
	if (archivo == NULL) {
		printf("No hay citas registradas.\n");
		return;
	}
	
	limpiarPantalla();
	printf("=============================");
	printf("\n    CITAS DEL PACIENTE     \n");
	printf("=============================\n");
	
	while (fscanf(archivo, "%d|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", &cita.id, cita.cedula, &cita.codigoMedico, cita.fecha, cita.hora, cita.estado, cita.diagnostico, cita.cuadroClinico, cita.tratamiento) == 9) {
		
		if (strcmp(cita.cedula, cedula) == 0) {
			encontrado = 1;
			printf("ID Cita: %d\n", cita.id);
			printf("Fecha: %s\n", cita.fecha);
			printf("Hora: %s\n", cita.hora);
			printf("Estado: %s\n", cita.estado);
			printf("--------------------------\n");
		}
	}
	
	if (!encontrado) {
		printf("No tiene citas registradas.\n");
	}
	
	fclose(archivo);
}

//MEDICO
void listarYGestionarCitasMedico(int codigoMedico) {
	FILE *archivo = fopen("citas.txt", "r");
	FILE *temporal;
	Cita cita;
	int opcion, idBuscar, encontrada = 0;
	int hayCitas = 0;
	
	if (archivo == NULL) {
		printf("No existen citas registradas.\n");
		return;
	}
	
	limpiarPantalla();
	printf("=============================");
	printf("\n     CITAS DEL MEDICO     \n");
	printf("=============================\n");
	
	while (fscanf(archivo, "%d|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", &cita.id, cita.cedula, &cita.codigoMedico, cita.fecha, cita.hora, cita.estado, cita.diagnostico, cita.cuadroClinico, cita.tratamiento) == 9) {
		
		if (cita.codigoMedico == codigoMedico) {
			hayCitas = 1;
			
			printf("\nID Cita: %d\n", cita.id);
			printf("Cedula Paciente: %s\n", cita.cedula);
			printf("Fecha: %s\n", cita.fecha);
			printf("Hora: %s\n", cita.hora);
			printf("Estado: %s\n", cita.estado);
			printf("Diagnostico: %s\n", cita.diagnostico);
			printf("Cuadro Clinico: %s\n", cita.cuadroClinico);
			printf("Tratamiento: %s\n", cita.tratamiento);
			printf("--------------------------\n");
		}
	}
	
	if (!hayCitas) {
		printf("No tiene citas asignadas.\n");
		fclose(archivo);
		return;
	}
	
	fclose(archivo);
	
	printf("\n¿Desea cambiar el estado de alguna cita?\n");
	printf("1. Si\n");
	printf("2. No\n");
	printf("Seleccione una opcion: ");
	scanf("%d", &opcion);
	limpiarBuffer();
	
	if (opcion != 1) {
		return;
	}
	
	printf("Ingrese el ID de la cita: ");
	scanf("%d", &idBuscar);
	limpiarBuffer();
	
	archivo = fopen("citas.txt", "r");
	temporal = fopen("temp.txt", "w");
	
	if (archivo == NULL || temporal == NULL) {
		printf("Error al procesar archivos.\n");
		return;
	}
	
	while (fscanf(archivo, "%d|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", &cita.id, cita.cedula, &cita.codigoMedico, cita.fecha, cita.hora, cita.estado, cita.diagnostico, cita.cuadroClinico, cita.tratamiento) == 9) {
		
		if (cita.id == idBuscar && cita.codigoMedico == codigoMedico) {
			encontrada = 1;
			
			int opcionEstado;
			
			printf("\nSeleccione el nuevo estado:\n");
			printf("1. No asistio\n");
			printf("2. Cancelada\n");
			printf("3. Reprogramada\n");
			printf("4. Atendida\n");
			printf("Opcion: ");
			scanf("%d", &opcionEstado);
			limpiarBuffer();
			
			if (opcionEstado == 1) {
				strcpy(cita.estado, "No asistio");
			}
			else if (opcionEstado == 2) {
				strcpy(cita.estado, "Cancelada");
			}
			else if (opcionEstado == 3) {
				do {
					printf("Nueva fecha (dd/mm/aaaa): ");
					fgets(cita.fecha, 20, stdin);
					cita.fecha[strcspn(cita.fecha, "\n")] = 0;
				} while (!validarFecha(cita.fecha));
				
				do {
					printf("Nueva hora (hh:mm): ");
					fgets(cita.hora, 20, stdin);
					cita.hora[strcspn(cita.hora, "\n")] = 0;
				} while (!validarHora(cita.hora));
				
				strcpy(cita.estado, "Reprogramada");
			}
			else if (opcionEstado == 4) {
				printf("Nuevo diagnostico: ");
				fgets(cita.diagnostico, 100, stdin);
				cita.diagnostico[strcspn(cita.diagnostico, "\n")] = 0;
				
				printf("Cuadro clinico: ");
				fgets(cita.cuadroClinico, 200, stdin);
				cita.cuadroClinico[strcspn(cita.cuadroClinico, "\n")] = 0;
				
				printf("Tratamiento: ");
				fgets(cita.tratamiento, 200, stdin);
				cita.tratamiento[strcspn(cita.tratamiento, "\n")] = 0;
				
				strcpy(cita.estado, "Atendida");
			}
		}
		
		fprintf(temporal, "%d|%s|%d|%s|%s|%s|%s|%s|%s\n", cita.id, cita.cedula, cita.codigoMedico, cita.fecha, cita.hora, cita.estado, cita.diagnostico, cita.cuadroClinico, cita.tratamiento);
	}
	
	fclose(archivo);
	fclose(temporal);
	
	remove("citas.txt");
	rename("temp.txt", "citas.txt");
	
	if (encontrada) {
		printf("Estado de la cita actualizado correctamente.\n");
	} else {
		printf("No se encontro la cita o no pertenece a este medico.\n");
	}
}

//MENU DEL PACIENTE
void menuPaciente(char cedulaPaciente[]) {
	FILE *archivo, *temporal;
	Cita cita;
	int opcion, idBuscar, encontrada;

	
	do {
		limpiarPantalla();
		printf("=============================");
		printf("\n       MENU PACIENTE       \n");
		printf("=============================\n");
		printf("1. Listar mis citas\n");
		printf("2. Cancelar una cita\n");
		printf("3. Reprogramar una cita\n");
		printf("4. Salir\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		limpiarBuffer();
		
		if (opcion == 1) {
			listarCitasPorPaciente(cedulaPaciente);
			pausar();
			limpiarPantalla();
		}
		
		else if (opcion == 2 || opcion == 3) {
			printf("Ingrese el ID de la cita: ");
			scanf("%d", &idBuscar);
			limpiarBuffer();
			
			archivo = fopen("citas.txt", "r");
			temporal = fopen("temp.txt", "w");
			encontrada = 0;
			
			if (archivo == NULL || temporal == NULL) {
				printf("Error al abrir archivos.\n");
				continue;
			}
			
			while (fscanf(archivo, "%d|%[^|]|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", &cita.id, cita.cedula, &cita.codigoMedico, cita.fecha, cita.hora, cita.estado, cita.diagnostico, cita.cuadroClinico, cita.tratamiento) == 9) {
				
				if (cita.id == idBuscar &&
					strcmp(cita.cedula, cedulaPaciente) == 0) {
					
					encontrada = 1;
					
					if (opcion == 2) {
						strcpy(cita.estado, "Cancelada");
					} else {
						do {
							printf("Nueva fecha (dd/mm/aaaa): ");
							fgets(cita.fecha, 20, stdin);
							cita.fecha[strcspn(cita.fecha, "\n")] = 0;
						} while (!validarFecha(cita.fecha));
						
						do {
							printf("Nueva hora (hh:mm): ");
							fgets(cita.hora, 20, stdin);
							cita.hora[strcspn(cita.hora, "\n")] = 0;
						} while (!validarHora(cita.hora));
						
						strcpy(cita.estado, "Reprogramada");
					}
				}
				
				fprintf(temporal, "%d|%s|%d|%s|%s|%s|%s|%s|%s\n", cita.id, cita.cedula, cita.codigoMedico, cita.fecha, cita.hora, cita.estado, cita.diagnostico, cita.cuadroClinico, cita.tratamiento);
			}
			
			fclose(archivo);
			fclose(temporal);
			
			remove("citas.txt");
			rename("temp.txt", "citas.txt");
			
			if (encontrada)
				printf("Operacion realizada correctamente.\n");
			else
				printf("No se encontro la cita o no le pertenece.\n");
			
			pausar();
			limpiarPantalla();
		}
		
	} while (opcion != 4);
}
