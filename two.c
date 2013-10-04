#include "two.h"

/*! \file two.c
 * \author JM Moreau
 * \date 2004
 */

/*! Generation d'un nombre aleatoire entre deux bornes.
 * \arg a : borne inferieure
 * \arg b : borne superieure.
 * \warning verifier que random() retourne bien un entier long entre 0 et RAND_MAX inclus.
 */
double myRandom (double a, double b)
{
  double tmp = random(); /* long int in [0, RAND_MAX] */

  return a+tmp*((b-a)/RAND_MAX);
}

/*! Incantation d'ouverture de fenetre OpenGL */
void winInit()
{
  gluOrtho2D(minX, maxX, minY, maxY);
}

/*! Generations des sites */
void selectPoints (void)
{
  int n = nbPoints;

  while (--n >= 0)
   {
     T[n].coords[0] = myRandom(minX+10, maxX-10);
     T[n].coords[1] = myRandom(minY+10, maxY-10);
   }
}

/*! Affichage.
 *\warning Particularite: "callback function", ie pas d'argument transmis... Tout en global, yeurk!
 */
void display(void)
{
  int n = nbPoints;

  glColor3f(0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
		
  glBegin(GL_POINTS);

  while (--n >= 0)
   glVertex2f(T[n].coords[0], T[n].coords[1]);
  glEnd();

  glFlush();
}


/*! \brief Fonction principale: on peut choisir le nombre de points
 * en utilisant l'option "-nX" où X est un entier strictement
 * positif.
 * \remark Mettre opterr a 0 equivaut a supprimer volontairement les messages d'erreur renvoyes par getopt 
 * lors de la lecture d'options non prevus sur la ligne de commande. Dans ces cas, l'erreur est traitee grace au
 * fait que getopt retourne la valeur '?', qui est donc traitee a part
 * \remark "n:" signifie que l'option n s'accompagne de la lecture immediate d'une valeur. Il ne faut pas
 * laisser d'espace entre -n et le nombre tape. Exemple: two -n1000.\par
 * La lecture est effectuee par un sscanf (lecture d'une chaine supposee formattee) sur l'argument ad hoc, optarg,
 * declare dans les routines de lecture de la ligne de commande.
 */
int main(int argc, char **argv)  
{  
  int c;

  opterr = 0;
  while ((c = getopt(argc, argv, "n:")) != EOF)
  {
	switch (c)
	{
		case 'n': if ((sscanf(optarg, "%d", &nbPoints) != 1) || nbPoints <= 0)
					nbPoints = 50;
				  break;
		case '?': printf("use option -nX (no space), with 0 < X.\n");
				  break;
		default : printf("Shouldn't be here, really...\n");
				  break;
	}
  }

  assert(nbPoints > 0);
  T = (vertex *) malloc(sizeof(vertex)*nbPoints);
  assert(T != NULL);

  glutInit(&argc, argv);  
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  
  glutInitWindowPosition(5,5);  
  glutInitWindowSize(300,300);  
  glutCreateWindow("My first OpenGL window...");  
  
  winInit();
  selectPoints();

  glutDisplayFunc(display);  
  glutMainLoop();  
  
  return EXIT_SUCCESS;  
}  
