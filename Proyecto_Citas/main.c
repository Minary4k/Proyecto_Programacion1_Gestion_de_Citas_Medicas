#include <stdio.h>
#include <stdlib.h>
#include "usuarios.h"
#include "medicos.h"
#include "pacientes.h"

char cedulaLogueada[20];

int main() {
	int rolLogueado = 0;
	
	while (1) {
		
		rolLogueado = 0;
		menuLogin(&rolLogueado);
		
		if (rolLogueado == -1) {
			break;
		}
		else if (rolLogueado == 1) {
			menuOperador();
		}
		else if (rolLogueado == 2) {
			menuMedico();
		}
		else if (rolLogueado == 3) {
			menuPaciente(cedulaLogueada);
		}
	}
	
	printf("Saliendo del sistema...\n");
	return 0;
}
