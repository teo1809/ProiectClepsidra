#include <windows.h>  
#include "glos.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "GLAux.h"
#include <math.h>

void myinit(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK MutaStanga(void);
void CALLBACK MutaDreapta(void);
void CALLBACK MutaSus(void);
void CALLBACK MutaJos(void);
void CALLBACK RotireSus(AUX_EVENTREC* event);
void CALLBACK RotireJos(AUX_EVENTREC* event);

GLUquadricObj* obj;
static GLfloat x = 0;
static GLfloat y = 0;
static GLfloat alfaX = 45.0;
static GLfloat alfaY = 0.0;
static GLfloat rotireInversare = 0.0;  // unghiul aditional pentru rotatia la 180 de grade
static GLfloat tintaRotire = 0.0;      // unghiul la care vrem sa ajungem (0, 180, 360...)
static BOOL seInvarte = FALSE;

static GLfloat inaltimeSus = 28.0;   // inaltimea initiala a conului de sus
static GLfloat inaltimeJos = 0.01;   // inaltimea initiala a cilindrului de jos (aproape 0)
static BOOL seScurge = FALSE;        // indicator daca animatia este pornita
static GLfloat cadereParticule = 0.0; // controlează pozitia particulelor pe verticală

static GLfloat rotireLumina = 0.0; // unghiul curent pentru rotirea luminii

void myinit(void) {
	//glClearColor(0.1, 0.1, 0.1, 0.1);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
}
void CALLBACK MutaStanga(void) { x = x - 10; }
void CALLBACK MutaDreapta(void) { x = x + 10; }
void CALLBACK MutaSus(void) { y = y + 10; }
void CALLBACK MutaJos(void) { y = y - 10; }
void CALLBACK RotireSus(AUX_EVENTREC* event) { alfaX = alfaX + 10; }
void CALLBACK RotireJos(AUX_EVENTREC* event) { alfaX = alfaX - 10; }
void CALLBACK RotireDreaptaY(void) { alfaY = alfaY + 10; }
void CALLBACK RotireStangaY(void) { alfaY = alfaY - 10; }

void CALLBACK AnimaNisip(void) {
	// rotire clepsidrei la 180 grade
	if (seInvarte) {
		if (rotireInversare < tintaRotire) {
			rotireInversare += 1.5; 
			if (rotireInversare >= tintaRotire) {
				rotireInversare = tintaRotire;
				seInvarte = FALSE;		
				inaltimeSus = 28.0;
				inaltimeJos = 0.01;				
				seScurge = TRUE; // porneste automat scurgerea după ce s-a invartit
			}
		}
		display();
		return; // cat timp se invarte, sub nicio forma nu curge nisipul
	}

	// scurgere nisip
	if (seScurge) {
		if (inaltimeSus > 0.0) {
			inaltimeSus -= 0.002; 
			if (inaltimeSus < 0.01) inaltimeSus = 0.01;
		}
		else {
			seScurge = FALSE;
		}

		if (inaltimeJos < 28.0) {
			inaltimeJos += 0.002; 
			if (inaltimeJos > 28.0) inaltimeJos = 28.0;
		}

		cadereParticule += 1.5; 
		if (cadereParticule > 66.0) {
			cadereParticule = 0.0;
		}
		display();
	}
}

void CALLBACK ReseteazaAnimatia(void) {
	if (seInvarte) return; // nu fac nimic daca nu se invarte

	seScurge = FALSE; // pe durata roatiei

	// inversarea nisipului
	GLfloat cantitateTemporara = inaltimeSus;   //salvare pt cupa de sus

	inaltimeSus = inaltimeJos;

	inaltimeJos = cantitateTemporara; //cupa de jos trebuie sa ia 0 sau nisipul ramas

	cadereParticule = 0.0;

	tintaRotire += 180.0;
	seInvarte = TRUE;

	display();
}


void CALLBACK PornesteAnimatia(void) {
	seScurge = !seScurge; 
}



void CALLBACK RotesteLumina(void) {
	rotireLumina = rotireLumina + 15.0;
	if (rotireLumina >= 360.0) {
		rotireLumina -= 360.0;
	}
	display(); 
}


void CALLBACK display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	obj = gluNewQuadric();
	glLoadIdentity();



	GLfloat pozitieLumina[] = { 0.0, 0.0, 150.0, 1.0 };


	glPushMatrix(); 
	//sincronizare cu miscarea camerei
	glTranslatef(x, y, 0.0);
	glRotatef(alfaX, 1.0, 0.0, 0.0);
	glRotatef(alfaY, 0.0, 0.0, 1.0);

	// rotire bec si lumina pe y
	glRotatef(rotireLumina, 0.0, 1.0, 0.0);

	// inregistrare pozitie lumina exacta in opengl 
	glLightfv(GL_LIGHT0, GL_POSITION, pozitieLumina);



	//becul
	glTranslatef(pozitieLumina[0], pozitieLumina[1], pozitieLumina[2]);
	glRotatef(180, 0, 1, 0);

	glDisable(GL_LIGHTING);//sa straluceasca becul

	gluQuadricNormals(obj, GLU_SMOOTH);

	// partea gri
	glColor3f(0.6f, 0.6f, 0.6f); // Gri metalic
	glPushMatrix();
	glTranslatef(0.0, 0.0, -2.0);
	gluCylinder(obj, 2.5, 2.5, 6.0, 16, 4);
	gluDisk(obj, 0.0, 2.5, 16, 1);
	glPopMatrix();

	// partea galbena
	glColor3f(1.0f, 0.9f, 0.2f);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 4.0);
	gluCylinder(obj, 2.5, 5.0, 8.0, 32, 8);
	glPopMatrix();

	// varf parte galbena
	glColor3f(1.0f, 0.95f, 0.5f); 
	glPushMatrix();
	glTranslatef(0.0, 0.0, 12.0);
	gluSphere(obj, 5.0, 32, 32);
	glPopMatrix();

	glEnable(GL_LIGHTING);  // reactivare lumina pentru restul de obiecte
	glPopMatrix(); 



	glTranslatef(x, y, 0.0);
	glRotatef(alfaX, 1.0, 0.0, 0.0);
	glRotatef(alfaY, 0.0, 0.0, 1.0);

	// glRotatef(rotireInversare, 1.0, 0.0, 0.0);

	gluQuadricDrawStyle(obj, GLU_FILL);
	gluQuadricNormals(obj, GLU_SMOOTH);

//obiecte opace

		//nisip sus
	glPushMatrix();
	glColor3f(0.9f, 0.7f, 0.2f);
	if (seInvarte) {
		glRotatef(rotireInversare, 1.0, 0.0, 0.0);
	}
	glTranslatef(0, 0, -4);
	glRotatef(180, 1, 0, 0);
	gluCylinder(obj, 0, (inaltimeSus * 1.0), inaltimeSus, 64, 1);
	glTranslatef(0, 0, inaltimeSus);
	gluDisk(obj, 0, (inaltimeSus * 1.0), 64, 1);
	glPopMatrix();

	//  firul central si bobitele
	if (seScurge && inaltimeSus > 0.1) {
		glPushMatrix();
		glColor3f(0.9f, 0.7f, 0.2f);
		glTranslatef(0, 0, -5);
		GLfloat lungimeFir = 67.0 - inaltimeJos;
		if (lungimeFir > 0) {
			gluCylinder(obj, 0.7, 0.7, lungimeFir, 12, 1);
		}
		glPopMatrix();

		// particule individuale
		glColor3f(0.95f, 0.75f, 0.25f);

		// particula 1
		glPushMatrix();
		GLfloat pozitieZ1 = -5 + cadereParticule;
		glTranslatef(0.3, 0.3, pozitieZ1);
		if (pozitieZ1 < 62 - inaltimeJos) {
			gluSphere(obj, 0.5, 8, 8);
		}
		glPopMatrix();

		// particula 2
		glPushMatrix();
		GLfloat pozitieZ2 = -5 + (cadereParticule - 20);
		if (pozitieZ2 < -5) pozitieZ2 += 55.0;
		glTranslatef(-0.4, -0.2, pozitieZ2);
		if (pozitieZ2 < 62 - inaltimeJos) {
			gluSphere(obj, 0.4, 8, 8);
		}
		glPopMatrix();

		// particula 3
		glPushMatrix();
		GLfloat pozitieZ3 = -5 + (cadereParticule - 40);
		if (pozitieZ3 < -5) pozitieZ3 += 55.0;
		glTranslatef(0.1, -0.4, pozitieZ3);
		if (pozitieZ3 < 62 - inaltimeJos) {
			gluSphere(obj, 0.6, 8, 8);
		}
		glPopMatrix();
	}

	// nisip jos 
	glPushMatrix();
	glColor3f(0.9f, 0.7f, 0.2f);
	if (seInvarte) {
		glRotatef(rotireInversare, 1.0, 0.0, 0.0);
	}
	glTranslatef(0, 0, 62 - inaltimeJos);
	gluCylinder(obj, 27, 27, inaltimeJos, 64, 1);
	gluDisk(obj, 0, 27, 64, 1);
	glTranslatef(0, 0, inaltimeJos);
	gluDisk(obj, 0, 27, 64, 1);
	glPopMatrix();



	glPushMatrix(); 
	glRotatef(rotireInversare, 1.0, 0.0, 0.0);
	glColor3f(0.4f, 0.2f, 0.0f);

	// baza de sus
	glPushMatrix();
	glTranslatef(0, 0, -67);
	gluDisk(obj, 0, 45, 64, 1);
	gluCylinder(obj, 45, 45, 5, 64, 1);
	glTranslatef(0, 0, 5);
	gluDisk(obj, 0, 45, 64, 1);
	glPopMatrix();

	// baza de jos
	glPushMatrix();
	glTranslatef(0, 0, 62);
	gluDisk(obj, 0, 45, 64, 1);
	gluCylinder(obj, 45, 45, 5, 64, 1);
	glTranslatef(0, 0, 5);
	gluDisk(obj, 0, 45, 64, 1);
	glPopMatrix();

	// tija 1
	glPushMatrix();
	glTranslatef(-40, 0, -65);
	gluCylinder(obj, 2, 2, 130, 20, 1);
	gluSphere(obj, 5, 20, 20);
	glTranslatef(0, 0, 130);
	gluSphere(obj, 5, 20, 20);
	glPopMatrix();

	// tija 2
	glPushMatrix();
	glTranslatef(40, 0, -65);
	gluCylinder(obj, 2, 2, 130, 20, 1);
	gluSphere(obj, 5, 20, 20);
	glTranslatef(0, 0, 130);
	gluSphere(obj, 5, 20, 20);
	glPopMatrix();

	// tija 3
	glPushMatrix();
	glTranslatef(0, 40, -65);
	gluCylinder(obj, 2, 2, 130, 20, 1);
	gluSphere(obj, 5, 20, 20);
	glTranslatef(0, 0, 130);
	gluSphere(obj, 5, 20, 20);
	glPopMatrix();

	// tija 4
	glPushMatrix();
	glTranslatef(0, -40, -65);
	gluCylinder(obj, 2, 2, 130, 20, 1);
	gluSphere(obj, 5, 20, 20);
	glTranslatef(0, 0, 130);
	gluSphere(obj, 5, 20, 20);
	glPopMatrix();


//obiecte transparente
	glDepthMask(GL_FALSE);
	glColor4f(0.7f, 0.7f, 0.8f, 0.4f);

	// corp de sus
	glPushMatrix();
	glTranslatef(0, 0, -62);
	gluCylinder(obj, 30, 30, 30, 64, 10);
	glTranslatef(0, 0, 30);
	gluCylinder(obj, 30, 2.0, 30, 64, 10);
	glTranslatef(0, 0, -30);
	gluDisk(obj, 0, 30, 64, 1);
	glPopMatrix();

	// gatul clepsidrei
	glPushMatrix();
	glTranslatef(0, 0, -2);
	gluCylinder(obj, 2.5, 2.5, 5, 32, 5);
	glPopMatrix();

	// corp de jos
	glPushMatrix();
	glTranslatef(0, 0, 2);
	gluCylinder(obj, 2.0, 30, 30, 64, 10);
	glTranslatef(0, 0, 30);
	gluCylinder(obj, 30, 30, 30, 64, 10);
	glTranslatef(0, 0, 30);
	gluDisk(obj, 0, 30, 64, 1);
	glPopMatrix();

	glDepthMask(GL_TRUE);
	glPopMatrix();

	auxSwapBuffers();
	gluDeleteQuadric(obj);
}
void CALLBACK myReshape(GLsizei w, GLsizei h)
{
	if (!h) return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-160.0, 160.0, -160.0 * (GLfloat)h / (GLfloat)w,
			160.0 * (GLfloat)h / (GLfloat)w, -160.0, 160.0);
	else
		glOrtho(-160.0 * (GLfloat)w / (GLfloat)h,
			160.0 * (GLfloat)w / (GLfloat)h, -160.0, 160.0, -160.0, 160.0);

	glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv)
{
	auxInitDisplayMode(AUX_DOUBLE | AUX_RGB | AUX_DEPTH);
	auxInitPosition(0, 0, 300, 200);
	auxInitWindow("Clepsidra");
	myinit();

	auxKeyFunc(AUX_LEFT, MutaStanga);
	auxKeyFunc(AUX_RIGHT, MutaDreapta);
	auxKeyFunc(AUX_UP, MutaSus);
	auxKeyFunc(AUX_DOWN, MutaJos);

	auxMouseFunc(AUX_LEFTBUTTON, AUX_MOUSEDOWN, RotireSus);
	auxMouseFunc(AUX_RIGHTBUTTON, AUX_MOUSEDOWN, RotireJos);

	auxKeyFunc(AUX_a, RotireStangaY);
	auxKeyFunc(AUX_d, RotireDreaptaY);

	auxKeyFunc(AUX_s, PornesteAnimatia);

	auxIdleFunc(AnimaNisip);

	auxKeyFunc(AUX_r, ReseteazaAnimatia);

	auxKeyFunc(AUX_l, RotesteLumina); 

	auxReshapeFunc(myReshape);
	auxMainLoop(display);
	return(0);
}