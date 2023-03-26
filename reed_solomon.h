//Marta Noguera Segura
//Eines informàtiques per a les matemàtiques
//Grau en Matemàtiques, UAB
//Curs 2021-22
//Treball final

#ifndef REED_SOLOMON_H
# define REED_SOLOMON_H
# include<stdio.h>
# include<stdlib.h>
# include<string.h>

void configurapik(char* []);
int comprovapik(int, int);
int primer(int);
void llegeixcadena(char*, int*, int*, int*, int*, int*);
int decidirtipusfitxer(char*, int);
int contarvalors(int, int, char*);
int primitiu(int);
int ordre(int, int);
int potencia(int, int, int);
void llegeixmatambtipus(int, int d2, int, int, int, int, int[][d2], char*);
void llegeixmatdat(int d1, int d2, int p, int mat[][d2], char*, int);
void llegeixmatbin(int d1, int d2, int p, int mat[][d2], char*, int);
void llegeixmattxt(int d1, int d2, int p, int mat[][d2], char*, int);
void codificar(int, int, int, int, int[], int[]);
void descodificar(int, int, int, int, int[], int[], int*);
int discussio(int, int d2, int, int[], int[], int[][d2]);
int arreglarerrors(int, int, int, int, int, int[], int[]);
int rang(int, int d2, int, int[][d2]);
void gauss(int d1, int d2, int, int[][d2]);
void intercanviarfiles(int dim, int, int, int [][dim]);
void filaxescalar(int dim, int, int, int, int [][dim]);
void sumamultfila(int dim, int, int, int, int, int [][dim]);
int invers(int, int);

#endif
