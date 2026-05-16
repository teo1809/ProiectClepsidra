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
static GLfloat rotireInversare = 0.0;  // Unghiul adițional pentru rotația la 180°
static GLfloat tintaRotire = 0.0;      // Unghiul la care vrem să ajungem (0, 180, 360...)
static BOOL seInvarte = FALSE;

static GLfloat inaltimeSus = 28.0;   // Inaltimea initiala a conului de sus
static GLfloat inaltimeJos = 0.01;   // Inaltimea initiala a cilindrului de jos (aproape 0)
static BOOL seScurge = FALSE;        // Indicator daca animatia este pornita
static GLfloat cadereParticule = 0.0; // Controlează poziția particulelor pe verticală

static GLfloat rotireLumina = 0.0; // Unghiul curent pentru rotirea luminii

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
	// 1. Logica pentru rotirea clepsidrei la 180 grade
	if (seInvarte) {
		if (rotireInversare < tintaRotire) {
			rotireInversare += 1.5; // Viteza de rotație fluidă
			if (rotireInversare >= tintaRotire) {
				rotireInversare = tintaRotire;
				seInvarte = FALSE;

				// --- LOGICĂ CORECTATĂ ACUM ---
				// Indiferent de cât nisip mai rămăsese, la o rotire completă
				// vrem ca nisipul să reînceapă plin de sus (28.0) și gol jos (0.01)
				inaltimeSus = 28.0;
				inaltimeJos = 0.01;
				// -----------------------------

				seScurge = TRUE; // Pornește automat scurgerea după ce s-a învârtit
			}
		}
		display();
		return; // Cât timp se învârte, sub nicio formă NU curge nisipul
	}

	// 2. Logica pentru scurgerea nisipului
	if (seScurge) {
		if (inaltimeSus > 0.0) {
			inaltimeSus -= 0.002; // Ajustat pentru o viteză vizuală optimă
			if (inaltimeSus < 0.01) inaltimeSus = 0.01;
		}
		else {
			seScurge = FALSE;
		}

		if (inaltimeJos < 28.0) {
			inaltimeJos += 0.002; // Să crească în același ritm în care scade cel de sus
			if (inaltimeJos > 28.0) inaltimeJos = 28.0;
		}

		cadereParticule += 1.5; // Viteza bobițelor în cădere
		if (cadereParticule > 66.0) {
			cadereParticule = 0.0;
		}
		display();
	}
}

void CALLBACK ReseteazaAnimatia(void) {
	if (seInvarte) return; // Nu facem nimic dacă deja se învârte clepsidra

	seScurge = FALSE; // Oprim scurgerea pe durata rotației mecanice

	// --- LOGICA DE INVERSARE ÎNTRE CUPE ---
	// Salvăm temporar valoarea din cupa de sus
	GLfloat cantitateTemporara = inaltimeSus;

	// Nisipul care s-a strâns jos devine noul nisip care va curge de sus
	inaltimeSus = inaltimeJos;

	// Cupa de jos devine goală (sau primește ce mai rămăsese sus, dacă ai întors-o parțial)
	inaltimeJos = cantitateTemporara;

	// Resetăm animația particulelor de cădere (să înceapă iar de sus)
	cadereParticule = 0.0;

	// Setăm noua țintă de rotație cu încă 180 de grade
	tintaRotire += 180.0;
	seInvarte = TRUE;

	display(); // Cerem redesenarea ecranului
}


void CALLBACK PornesteAnimatia(void) {
	seScurge = !seScurge; // Togglem animatia (Apasă o tastă să pornească/oprească)
}



void CALLBACK RotesteLumina(void) {
	rotireLumina = rotireLumina + 15.0; // Crește unghiul cu 15 grade la fiecare apăsare
	if (rotireLumina >= 360.0) {
		rotireLumina -= 360.0;
	}
	display(); // Cere redesenarea scenei cu noua poziție a luminii
}


void CALLBACK display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	obj = gluNewQuadric();
	glLoadIdentity();



	GLfloat pozitieLumina[] = { 0.0, 0.0, 150.0, 1.0 };

	glPushMatrix(); // Începem grupul pentru sistemul de iluminat
	// Sincronizăm cu mișcarea camerei scenei
	glTranslatef(x, y, 0.0);
	glRotatef(alfaX, 1.0, 0.0, 0.0);
	glRotatef(alfaY, 0.0, 0.0, 1.0);

	// Rotim întregul ansamblu (lumina + becul geometric) pe axa Y
	glRotatef(rotireLumina, 0.0, 1.0, 0.0);

	// Înregistrăm poziția luminii exacte în OpenGL
	glLightfv(GL_LIGHT0, GL_POSITION, pozitieLumina);

	// ========================================================
	// CONSTRUCȚIA BECULUI GEOMETRIC
	// ========================================================
	// Ne mutăm în spațiu exact în punctul unde am setat lumina invizibilă
	glTranslatef(pozitieLumina[0], pozitieLumina[1], pozitieLumina[2]);

	// Rotim becul ca să fie orientat spre clepsidră (Z negativ)
	glRotatef(180, 0, 1, 0);

	// Dezactivăm iluminarea pe becul însuși pentru ca el să "strălucească"
	glDisable(GL_LIGHTING);

	gluQuadricNormals(obj, GLU_SMOOTH);

	// --- PART I: CAPĂTUL METALIC (Cilindru Mic Gri) ---
	glColor3f(0.6f, 0.6f, 0.6f); // Gri metalic
	glPushMatrix();
	// Desenăm baza metalică (pe axa Z, în sens negativ)
	glTranslatef(0.0, 0.0, -2.0);
	gluCylinder(obj, 2.5, 2.5, 6.0, 16, 4);
	// Capacul de jos
	gluDisk(obj, 0.0, 2.5, 16, 1);
	glPopMatrix();

	// --- PART II: CORPUL GALBEN (Cilindru Galben) ---
	glColor3f(1.0f, 0.9f, 0.2f); // Galben aprins, "warm light"
	glPushMatrix();
	// Cilindrul continuă din baza metalică
	glTranslatef(0.0, 0.0, 4.0);
	// Un cilindru care se evazează puțin spre vârf (crește raza)
	gluCylinder(obj, 2.5, 5.0, 8.0, 32, 8);
	glPopMatrix();

	// --- PART III: VÂRFUL BECULUI (Semisferă Galbenă) ---
	glColor3f(1.0f, 0.95f, 0.5f); // Galben puțin mai deschis la vârf (centrul luminii)
	glPushMatrix();
	// Ne mutăm la capătul cilindrului galben
	glTranslatef(0.0, 0.0, 12.0);
	// Desenăm o semisferă. Din păcate, GLU nu are semisferă nativă,
	// dar o simulăm rotind o sferă completă, sperând că Depth Test va ascunde spatele.
	// Pentru rezultate "frumoase", creștem subdiviziunile (32x32).
	gluSphere(obj, 5.0, 32, 32);
	glPopMatrix();

	glEnable(GL_LIGHTING);  // Reactivăm iluminarea pentru restul obiectelor (clepsidră, nisip)
	// ========================================================

	glPopMatrix(); // Închidem grupul pentru sistemul de iluminat, revenim la camera globală




	glTranslatef(x, y, 0.0);
	glRotatef(alfaX, 1.0, 0.0, 0.0);
	glRotatef(alfaY, 0.0, 0.0, 1.0);

	// glRotatef(rotireInversare, 1.0, 0.0, 0.0);

	gluQuadricDrawStyle(obj, GLU_FILL);
	gluQuadricNormals(obj, GLU_SMOOTH);

	// ==========================================
	// PASUL 1: OBIECTELE OPACE (Se randează primele)
	// ==========================================

		// --- NISIPUL OPAC (De sus, firul și cel de jos) ---
		// 1. Nisip Sus (Conul care scade)
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

	// 2. Firul central și bobițele (Doar când curge)
	if (seScurge && inaltimeSus > 0.1) {
		// Firul central de nisip - acum pornește de la Z=-5 direct prin gât până jos
		glPushMatrix();
		glColor3f(0.9f, 0.7f, 0.2f);
		glTranslatef(0, 0, -5);
		GLfloat lungimeFir = 67.0 - inaltimeJos;
		if (lungimeFir > 0) {
			gluCylinder(obj, 0.7, 0.7, lungimeFir, 12, 1);
		}
		glPopMatrix();

		// Particule individuale
		glColor3f(0.95f, 0.75f, 0.25f);

		// Particula 1
		glPushMatrix();
		GLfloat pozitieZ1 = -5 + cadereParticule;
		glTranslatef(0.3, 0.3, pozitieZ1);
		if (pozitieZ1 < 62 - inaltimeJos) {
			gluSphere(obj, 0.5, 8, 8);
		}
		glPopMatrix();

		// Particula 2
		glPushMatrix();
		GLfloat pozitieZ2 = -5 + (cadereParticule - 20);
		if (pozitieZ2 < -5) pozitieZ2 += 55.0;
		glTranslatef(-0.4, -0.2, pozitieZ2);
		if (pozitieZ2 < 62 - inaltimeJos) {
			gluSphere(obj, 0.4, 8, 8);
		}
		glPopMatrix();

		// Particula 3
		glPushMatrix();
		GLfloat pozitieZ3 = -5 + (cadereParticule - 40);
		if (pozitieZ3 < -5) pozitieZ3 += 55.0;
		glTranslatef(0.1, -0.4, pozitieZ3);
		if (pozitieZ3 < 62 - inaltimeJos) {
			gluSphere(obj, 0.6, 8, 8);
		}
		glPopMatrix();
	}

	// 3. Nisip Jos (Cilindrul care crește)
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

	// --- COMPONENTELE DIN LEMN ȘI TIJELE (Opace) ---

	glPushMatrix(); // <-- ADAUGĂ ACEASTĂ LINIE AICI
	glRotatef(rotireInversare, 1.0, 0.0, 0.0);


	glColor3f(0.4f, 0.2f, 0.0f);

	// Baza de sus
	glPushMatrix();
	glTranslatef(0, 0, -67);
	gluDisk(obj, 0, 45, 64, 1);
	gluCylinder(obj, 45, 45, 5, 64, 1);
	glTranslatef(0, 0, 5);
	gluDisk(obj, 0, 45, 64, 1);
	glPopMatrix();

	// Baza de jos
	glPushMatrix();
	glTranslatef(0, 0, 62);
	gluDisk(obj, 0, 45, 64, 1);
	gluCylinder(obj, 45, 45, 5, 64, 1);
	glTranslatef(0, 0, 5);
	gluDisk(obj, 0, 45, 64, 1);
	glPopMatrix();

	// TIJELE LATERALE
	// Tija 1
	glPushMatrix();
	glTranslatef(-40, 0, -65);
	gluCylinder(obj, 2, 2, 130, 20, 1);
	gluSphere(obj, 5, 20, 20);
	glTranslatef(0, 0, 130);
	gluSphere(obj, 5, 20, 20);
	glPopMatrix();

	// Tija 2
	glPushMatrix();
	glTranslatef(40, 0, -65);
	gluCylinder(obj, 2, 2, 130, 20, 1);
	gluSphere(obj, 5, 20, 20);
	glTranslatef(0, 0, 130);
	gluSphere(obj, 5, 20, 20);
	glPopMatrix();

	// Tija 3
	glPushMatrix();
	glTranslatef(0, 40, -65);
	gluCylinder(obj, 2, 2, 130, 20, 1);
	gluSphere(obj, 5, 20, 20);
	glTranslatef(0, 0, 130);
	gluSphere(obj, 5, 20, 20);
	glPopMatrix();

	// Tija 4
	glPushMatrix();
	glTranslatef(0, -40, -65);
	gluCylinder(obj, 2, 2, 130, 20, 1);
	gluSphere(obj, 5, 20, 20);
	glTranslatef(0, 0, 130);
	gluSphere(obj, 5, 20, 20);
	glPopMatrix();


	// ==========================================
	// PASUL 2: OBIECTELE TRANSPARENTE (Se randează la sfârșit)
	// ==========================================
	glDepthMask(GL_FALSE);

	// Setăm culoarea sticlei cu Alpha transparent (0.4f)
	glColor4f(0.7f, 0.7f, 0.8f, 0.4f);

	// 1. CORPUL DE SUS (STICLA)
	glPushMatrix();
	glTranslatef(0, 0, -62);
	gluCylinder(obj, 30, 30, 30, 64, 10);
	glTranslatef(0, 0, 30);
	gluCylinder(obj, 30, 2.0, 30, 64, 10);
	glTranslatef(0, 0, -30);
	gluDisk(obj, 0, 30, 64, 1);
	glPopMatrix();

	// 2. GATUL CLEPSIDREI (CILINDRUL CENTRAL)
	glPushMatrix();
	glTranslatef(0, 0, -2);
	gluCylinder(obj, 2.5, 2.5, 5, 32, 5);
	glPopMatrix();

	// 3. CORPUL DE JOS (STICLA)
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

	auxKeyFunc(AUX_s, PornesteAnimatia); // Tasta 'S' porneste/opreste
	auxKeyFunc(AUX_S, PornesteAnimatia);

	auxIdleFunc(AnimaNisip);

	auxKeyFunc(AUX_r, ReseteazaAnimatia);

	auxKeyFunc(AUX_l, RotesteLumina); // Pentru tasta 'l' mic



	auxReshapeFunc(myReshape);
	auxMainLoop(display);
	return(0);
}