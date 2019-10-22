#include <stdio.h>
#include <stdlib.h>

int dump(char *nom) {
	FILE *fitxer;
	char linia[256];
	printf("Obrint el fitxer : %s\n",nom);
	fitxer = fopen(nom,"r");
	while(fgets(linia,sizeof(linia),fitxer)) {
		printf("%s",linia);
	}
	fclose(fitxer);
	return 0;
}
int main(int estado, char **contenido) {
	if (estado!=2) {
		printf("Ajuda: lector 'fitxer'\n\n");
		return 1;
	}
	int tt = dump(contenido[1]);
	return tt;
}
