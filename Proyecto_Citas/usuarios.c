#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usuarios.h"
#include "pacientes.h"
#include "medicos.h"
#include "citas.h"

void menuOperador() {
	int opcion;
	
	do {
		limpiarPantalla();
		printf("=============================");
		printf("\n       MENU OPERADOR       \n");
		printf("=============================\n");
		printf("1. Registrar Paciente\n");
		printf("2. Listar Pacientes\n");
		printf("3. Registrar Medico\n");
		printf("4. Listar Medicos\n");
		printf("5. Asignar Cita\n");
		printf("6. Listar Citas\n");
		printf("7. Salir\n");
		printf("Opcion: ");
		scanf("%d", &opcion);
		limpiarBuffer();
		
		switch(opcion) {
		case 1:
			registrarPaciente();
			pausar();
			limpiarPantalla();
			break;
		case 2:
			listarPacientes();
			pausar();
			limpiarPantalla();
			break;
		case 3:
			registrarMedico();
			pausar();
			limpiarPantalla();
			break;
		case 4:
			listarMedicos();
			pausar();
			limpiarPantalla();
			break;
		case 5:
			asignarCita();
			pausar();
			limpiarPantalla();
			break;
		case 6:
			listarTodasLasCitas();
			pausar();
			limpiarPantalla();
			break;
		case 7:
			printf("\nUsted acaba de salir del menu operador.\n");
			break;
		default:
			printf("Opcion invalida.\n");
		}
		
	} while(opcion != 7);
}

int usuarioExiste(char nombre[]) {
	FILE *archivo = fopen("usuarios.txt", "r");
	if (!archivo) return 0;
	
	struct Usuario usuario;
	while (fscanf(archivo, "%[^|]|%[^|]|%d|%[^\n]\n", usuario.nombreUsuario, usuario.contrasena, &usuario.rol, usuario.cedula) == 4) {
		
		if (strcmp(usuario.nombreUsuario, nombre) == 0) {
			fclose(archivo);
			return 1;
		}
	}
	
	fclose(archivo);
	return 0;
}

void registrarUsuario() {
	struct Usuario usuario;
	FILE *archivo;
	
	archivo = fopen("usuarios.txt", "a");
	if (archivo == NULL) {
		printf("Error al abrir archivo de usuarios.\n");
		return;
	}
	
	limpiarPantalla();
	printf("=============================");
	printf("\n    REGISTRO DE USUARIO    \n");
	printf("=============================\n");
	
	printf("Nombre de usuario: ");
	fgets(usuario.nombreUsuario, 30, stdin);
	usuario.nombreUsuario[strcspn(usuario.nombreUsuario, "\n")] = 0;
	
	if (usuarioExiste(usuario.nombreUsuario)) {
		printf("Ese usuario ya existe.\n");
		fclose(archivo);
		return;
	}
	
	printf("Contrasena: ");
	fgets(usuario.contrasena, 30, stdin);
	usuario.contrasena[strcspn(usuario.contrasena, "\n")] = 0;
	
	do {
		printf("Seleccione rol:\n");
		printf("1. Operador\n");
		printf("2. Medico\n");
		printf("3. Paciente\n");
		printf("Opcion: ");
		scanf("%d", &usuario.rol);
		limpiarBuffer();
	} while (usuario.rol < 1 || usuario.rol > 3);
	
	if (usuario.rol == 3) {
		printf("Ingrese su cedula: ");
		fgets(usuario.cedula, 15, stdin);
		usuario.cedula[strcspn(usuario.cedula, "\n")] = 0;
	} else {
		strcpy(usuario.cedula, "N/A");
	}
	
	fprintf(archivo, "%s|%s|%d|%s\n", usuario.nombreUsuario, usuario.contrasena, usuario.rol, usuario.cedula);
	
	fclose(archivo);
	
	printf("Usuario registrado correctamente.\n");
}

int iniciarSesion(int *rolLogueado) {
	struct Usuario usuario;
	char usuarioIngresado[30];
	char contrasenaIngresada[30];
	FILE *archivo;
	
	archivo = fopen("usuarios.txt", "r");
	
	if (archivo == NULL) {
		printf("No existen usuarios registrados.\n");
		return 0;
	}

	limpiarPantalla();
	printf("=============================");
	printf("\n       INICIAR SESION       \n");
	printf("=============================\n");
	
	printf("Usuario: ");
	fgets(usuarioIngresado, 30, stdin);
	usuarioIngresado[strcspn(usuarioIngresado, "\n")] = 0;
	
	printf("Contrasena: ");
	fgets(contrasenaIngresada, 30, stdin);
	contrasenaIngresada[strcspn(contrasenaIngresada, "\n")] = 0;
	
	while (fscanf(archivo, "%[^|]|%[^|]|%d|%[^\n]\n", usuario.nombreUsuario, usuario.contrasena, &usuario.rol, usuario.cedula) == 4) {
		
		if (strcmp(usuarioIngresado, usuario.nombreUsuario) == 0 &&
			strcmp(contrasenaIngresada, usuario.contrasena) == 0) {
			
			*rolLogueado = usuario.rol;
			strcpy(cedulaLogueada, usuario.cedula);
			
			fclose(archivo);
			return 1;
		}
	}
	
	fclose(archivo);
	printf("Usuario o contrasena incorrectos.\n");
	return 0;
}

void menuLogin(int *rolLogueado) {
	int opcion;
	int sesionIniciada = 0;
	
	do {
		limpiarPantalla();
		printf("=============================");
		printf("\n       MENU LOGIN       \n");
		printf("=============================\n");
		printf("1. Registrarse\n");
		printf("2. Iniciar sesion\n");
		printf("3. Salir\n");
		printf("Opcion: ");
		scanf("%d", &opcion);
		limpiarBuffer();
		
		switch (opcion) {
		case 1:
			registrarUsuario();
			pausar();
			limpiarPantalla();
			break;
		case 2:
			sesionIniciada = iniciarSesion(rolLogueado);
			if (!sesionIniciada) pausar();
			limpiarPantalla();
			break;
		case 3:
			printf("\nUsted acaba de salir del sistema de gestion de citas medicas.\n");
			*rolLogueado = -1;
			break;
		default:
			printf("Opcion invalida.\n");
		}
		
	} while (!sesionIniciada && opcion != 3);
}

void limpiarPantalla() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void pausar() {
	printf("\nPresione ENTER para continuar...");
	getchar();
}
