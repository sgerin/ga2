#include "two.h"

#define AGAUCHE 0
#define ADROITE 1
#define ALIGNE 2
#define ARRAY_SIZE 3
#define DEDANS 0
#define SUR 1
#define DEHORS 2

int puissance(vertex*, vertex*, vertex*);
int in_triangle(vertex*, vertex*, vertex*, vertex*);
void find_line_equation(vertex*, vertex*, double*);
double find_signed_distance(vertex*, double*);
