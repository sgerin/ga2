#ifndef __TWO_H__
#define TWO_H__
/*! \file two.h
 * \author JM Moreau
 * \date 2004
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>  
#include <math.h>  
#include <unistd.h>  
#ifdef __linux
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif

/*! variable externe permettant de lire les parametres sur le ligne de commande.*/
extern char *optarg;

/*! bascule pour autoriser ou interdire (0) les messages d'erreurs envoyes par getopt. */
extern int opterr;

/*! Dimension de l'espace */
#define DIM 2

/*! Structure pour representer un point. */
typedef struct
{
  double coords[DIM];
} vertex;

/*! Tailles diverses du systemes de fenetrage .*/
const double minX = 0,
             maxX = 500,
	         minY = 0,
	         maxY = 500,
	         margin = 10;

/*! Nombre de "sites" dans le programme, modifiable par la ligne
 * de commande de la fonction main().
 */
int nbPoints = 50;

/*! Tableau gobal des sommets */
vertex *T = NULL;
/* Pour stocker la liste des états des sommets (dedans, dehors, dessus) */
int* B = NULL;

#endif



