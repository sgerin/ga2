#include "inclusion.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#ifdef __linux
#include <GL/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif
 
extern char *optarg;
extern int opterr;
 
int displayChoice = 0;
int g_dots = 0;
int nb_dots = 0; 
char *lineOption[] = { "GL_POINTS", "GL_LINES", "GL_LINE_STRIP", "GL_LINE_LOOP", "GL_POLYGON"};

/* La fonction puissance prenant trois points a, b, c en entrée 
permet de savoir si le point c est à "gauche" de la droite AB, 
fait partie de la droite ou s'y trouve à droite.
On calcule la distance signée de c par rapport à la droite AB.
Ainsi si le résultat de puissance(a, b, c) retourne AGAUCHE, 
le résultat de puissance(b, a, c) donnera ADROITE et vice versa. 
Un résultat aligné ne change pas.
*/
int puissance(vertex* a, vertex* b, vertex* c)
{
	double* array = malloc(ARRAY_SIZE*sizeof(double));
	find_line_equation(a, b, array);
	double distance = find_signed_distance(c, array);
	free(array);
	if(distance > 0)
		return AGAUCHE;
	else if(distance < 0)
		return ADROITE; 
	else if(distance == 0)
		return ALIGNE;
	else
		return -1;
	return 0;
}

int in_triangle(vertex* a, vertex* b, vertex* c, vertex* d)
{
	int ab, bc, ca;
	ab = puissance(a, b, d);
	bc = puissance(b, c, d);
	ca = puissance(c, a, d);
	//Si on est aligné on doit bien tester si on est dans les limites du segment formant le côté. 
	if((ab == ALIGNE) || (bc == ALIGNE) || (ca == ALIGNE))
	{
		if(ab == ALIGNE)
		{
			if( d->coords[0] > fmax(a->coords[0], b->coords[0]) || 
				d->coords[0] < fmin(a->coords[0], b->coords[0]) || 
				d->coords[1] > fmax(a->coords[1], b->coords[1]) ||
				d->coords[1] < fmin(a->coords[1], b->coords[1]))
					return DEHORS;
		}
		if(bc == ALIGNE)
		{
			if( d->coords[0] > fmax(c->coords[0], b->coords[0]) || 
				d->coords[0] < fmin(c->coords[0], b->coords[0]) || 
				d->coords[1] > fmax(c->coords[1], b->coords[1]) ||
				d->coords[1] < fmin(c->coords[1], b->coords[1]))
					return DEHORS;
		}
		if(ca == ALIGNE)
		{
			if( d->coords[0] > fmax(a->coords[0], c->coords[0]) || 
				d->coords[0] < fmin(a->coords[0], c->coords[0]) || 
				d->coords[1] > fmax(a->coords[1], c->coords[1]) ||
				d->coords[1] < fmin(a->coords[1], c->coords[1]))
					return DEHORS;
		}
		return SUR;
	}
	else if((ab == ADROITE) && (bc == ADROITE) && (ca == ADROITE))
		return DEDANS; 
	else
		return DEHORS; 
}

void find_line_equation(vertex* x, vertex* y, double* array)
{
	//array stores the equation parameters a, b and c in this order
	//line equation is the following : ax + by + c
	double a,b,c;
	a = x->coords[1] - y->coords[1];
	b = y->coords[0] - x->coords[0];
	c = (x->coords[0] - y->coords[0])*x->coords[1] + (y->coords[1] - x->coords[1])*x->coords[0];
	array[0] = a;
	array[1] = b;
	array[2] = c;
}

double find_signed_distance(vertex* c, double* array)
{
	double divisor = sqrt(array[0]*array[0] + array[1]*array[1]);
	return (c->coords[0]*array[0] + c->coords[1]*array[1] + array[2])/divisor;
}


void winInit (void)
{
  gluOrtho2D(0.0, 200, 0.0, 200);
}


void display ()
{
  glColor3f(0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.22, 0.45, 1.0);
  glEnable(GL_POINT_SMOOTH);

  switch (displayChoice)
  {
                  case 0: glBegin(GL_POINTS);
                                  break;
                  case 1: glBegin(GL_LINES);
                                  break;
                  case 2: glBegin(GL_LINE_STRIP);
                                  break;
                  case 3: glBegin(GL_LINE_LOOP);
                                  break;
                  default: glBegin(GL_POLYGON);
                                  break;
  }


  glVertex2f(T[1].coords[0], T[1].coords[1]);
  glVertex2f(T[2].coords[0], T[2].coords[1]);
  glVertex2f(T[3].coords[0], T[3].coords[1]);

  glEnd();

  glPointSize(4.25);

  glColor3f(0.8, 0.45, 0.22);  

  glBegin(GL_POINTS);
  int i; 
  //Pour chaque point la couleur peut changer.
  for(i=0; i<nb_dots; ++i)
  {
      switch(B[i])
      {
            case DEHORS:
				glColor3f(0.9, 0.1, 0.1);
			break;
            case SUR:
				glColor3f(1.0, 1.0, 1.0);
			break;
            case DEDANS:
				glColor3f(0.1, 0.9, 0.3);
			break;
		}
		glVertex2f(T[i].coords[0], T[i].coords[1]);
  } 
  glEnd();
  

  glFlush();
}


int main(int argc, char **argv)
{
  srand((unsigned)time(NULL));
	
  int option;
  opterr = 0; /* set off std error messages in case wrong option is chosen */
  while ((option = getopt(argc, argv, "c:d:")) != EOF)
  {
                  switch (option)
                  {
                        case 'd': if ((sscanf(optarg, "%d", &g_dots) == 1) && g_dots >= 0)
                                              break;
                        case 'c': if ((sscanf(optarg, "%d", &displayChoice) == 1) && displayChoice >= 0 && displayChoice <= 4)
                                                break;
                        case '?': /* value returned by getopt if invalid option chosen */
                        default : printf("Usage: %s -cX -dY, with X = 1, 2, 3 or 4; Y >= 0.\n", argv[0]);
                                          displayChoice = 3;
										  g_dots = 0; 
                                          break;
                  }
  }
  assert(displayChoice >= 0 && displayChoice <= 4);
  assert(g_dots >= 0);
  printf("Executing %s with line option %d = %s and %d generated points.\n", argv[0], displayChoice, lineOption[displayChoice], g_dots);
  
  nb_dots = 4+g_dots;
  
  T = (vertex *) malloc(sizeof(vertex)*nb_dots);
  T[0].coords[0] = 0;
  T[0].coords[1] = 0;
  T[1].coords[0] = 33;
  T[1].coords[1] = 130;
  T[2].coords[0] = 88;
  T[2].coords[1] = 122;
  T[3].coords[0] = 76;
  T[3].coords[1] = 36;  

  //On génère aléatoirement les x points. 
  //On fixe une limite de 170 afin de ne pas dessiner des points en dehors de l'écran.
  int i;
  double max = 170;
  double min = 0; 
  for(i=4; i<nb_dots; ++i)
  {
	  T[i].coords[0] = (double)rand()/RAND_MAX * (max - min) + min;
	  T[i].coords[1] = (double)rand()/RAND_MAX * (max - min) + min;
  }          
  
  //On stocke les états de tout les sommets pour ne faire le calcul qu'une seule fois
  B = (int *) malloc(sizeof(int)*nb_dots);
  for(i=0; i<nb_dots; ++i)
  {
	  B[i] = in_triangle(&T[1], &T[2], &T[3], &T[i]);
  } 
    
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

  glutInitWindowPosition(5,5);
  glutInitWindowSize(300,300);

  glutCreateWindow("My first OpenGL window...");

  winInit();

  glutDisplayFunc(display);

  glutMainLoop();
  
  free(T);
  T = NULL;
  free(B);
  B = NULL;

  return EXIT_SUCCESS;
}

