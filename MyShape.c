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

void myinit(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void CALLBACK MutaStanga(void) { x = x - 10; }
void CALLBACK MutaDreapta(void) { x = x + 10; }
void CALLBACK MutaSus(void) { y = y + 10; }
void CALLBACK MutaJos(void) { y = y - 10; }
void CALLBACK RotireSus(AUX_EVENTREC* event) { alfaX = alfaX + 10; }
void CALLBACK RotireJos(AUX_EVENTREC* event) { alfaX = alfaX - 10; }
void CALLBACK RotireDreaptaY(void) { alfaY = alfaY + 10; }
void CALLBACK RotireStangaY(void) { alfaY = alfaY - 10; }


//void CALLBACK display(void)
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    obj = gluNewQuadric();
//    glLoadIdentity();
//
//    glTranslatef(x, y, 0.0);
//    glRotatef(alfa, 1.0, 0.0, 0.0);
//
//    gluQuadricDrawStyle(obj, GLU_FILL);
//    gluQuadricNormals(obj, GLU_SMOOTH);
//
//
//    glPushMatrix();
//    glColor3f(0.7f, 0.7f, 0.8f); 
//    glTranslatef(0, 0, -60);
//    gluCylinder(obj, 30, 0, 60, 64, 10);
//    gluDisk(obj, 0, 30, 64, 1);
//
//    
//  
//    glColor3f(0.4f, 0.2f, 0.0f); 
//    glTranslatef(0, 0, -5);
//
//    gluDisk(obj, 0, 45, 64, 1);
//    gluCylinder(obj, 45, 45, 5, 64, 1);
//    glTranslatef(0, 0, 5);
//    gluDisk(obj, 0, 45, 64, 1);
//    glPopMatrix();
//
//    glPushMatrix();
//    glColor3f(0.7f, 0.7f, 0.8f); 
//    gluCylinder(obj, 0, 30, 60, 64, 10);
//
//    glTranslatef(0, 0, 60);
//    gluDisk(obj, 0, 30, 64, 1); 
//
//    glColor3f(0.4f, 0.2f, 0.0f);
//    gluDisk(obj, 0, 45, 64, 1);
//
//    gluCylinder(obj, 45, 45, 5, 64, 1);
//    glTranslatef(0, 0, 5);
//    gluDisk(obj, 0, 45, 64, 1);
//    glPopMatrix();
//
//
//    glColor3f(0.4f, 0.2f, 0.0f); 
//
//
//    glPushMatrix();
//    glTranslatef(-40, 0, -60);
//    gluCylinder(obj, 2, 2, 120, 20, 1);
//    glPopMatrix();
//
//    glPushMatrix();
//    glTranslatef(40, 0, -60);
//    gluCylinder(obj, 2, 2, 120, 20, 1);
//    glPopMatrix();
//
//    auxSwapBuffers();
//}


//void CALLBACK display(void)
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    obj = gluNewQuadric();
//    glLoadIdentity();
//
//    glTranslatef(x, y, 0.0);
//    glRotatef(alfa, 1.0, 0.0, 0.0);
//
//    gluQuadricDrawStyle(obj, GLU_FILL);
//    gluQuadricNormals(obj, GLU_SMOOTH);
//
//    glPushMatrix();
//    glColor3f(0.6f, 0.6f, 0.7f);
//    glTranslatef(0, 0, -2);
//    gluCylinder(obj, 1.5, 1.5, 4, 32, 1);
//    glPopMatrix();
//
//    glPushMatrix();
//    glColor3f(0.7f, 0.7f, 0.8f);
//    glTranslatef(0, 0, -62);
//    gluCylinder(obj, 30, 30, 10, 64, 10);
//    glTranslatef(0, 0, 10);
//    gluCylinder(obj, 30, 1.5, 50, 64, 10);
//    glTranslatef(0, 0, -10);
//    gluDisk(obj, 0, 30, 64, 1);
//
//    glColor3f(0.4f, 0.2f, 0.0f);
//    glTranslatef(0, 0, -5);
//    gluDisk(obj, 0, 45, 64, 1);
//    gluCylinder(obj, 45, 45, 5, 64, 1);
//    glTranslatef(0, 0, 5);
//    gluDisk(obj, 0, 45, 64, 1);
//    glPopMatrix();
//
//    glPushMatrix();
//    glColor3f(0.7f, 0.7f, 0.8f);
//    glTranslatef(0, 0, 2);
//    gluCylinder(obj, 1.5, 30, 50, 64, 10);
//    glTranslatef(0, 0, 50);
//    gluCylinder(obj, 30, 30, 10, 64, 10);
//    glTranslatef(0, 0, 10);
//    gluDisk(obj, 0, 30, 64, 1);
//
//    glColor3f(0.4f, 0.2f, 0.0f);
//    gluDisk(obj, 0, 45, 64, 1);
//    gluCylinder(obj, 45, 45, 5, 64, 1);
//    glTranslatef(0, 0, 5);
//    gluDisk(obj, 0, 45, 64, 1);
//    glPopMatrix();
//
//    glColor3f(0.4f, 0.2f, 0.0f);
//
//    glPushMatrix();
//    glTranslatef(-40, 0, -65);
//    gluCylinder(obj, 2, 2, 130, 20, 1);
//    glPushMatrix();
//    gluSphere(obj, 5, 20, 20);
//    glPopMatrix();
//    glPushMatrix();
//    glTranslatef(0, 0, 130);
//    gluSphere(obj, 5, 20, 20);
//    glPopMatrix();
//    glPopMatrix();
//
//    glPushMatrix();
//    glTranslatef(40, 0, -65);
//    gluCylinder(obj, 2, 2, 130, 20, 1);
//    glPushMatrix();
//    gluSphere(obj, 5, 20, 20);
//    glPopMatrix();
//    glPushMatrix();
//    glTranslatef(0, 0, 130);
//    gluSphere(obj, 5, 20, 20);
//    glPopMatrix();
//    glPopMatrix();
//
//
//    glPushMatrix();
//    glTranslatef(0, 40, -65);
//    gluCylinder(obj, 2, 2, 130, 20, 1);
//    gluSphere(obj, 5, 20, 20); 
//    glPushMatrix();
//    glTranslatef(0, 0, 130);
//    gluSphere(obj, 5, 20, 20);
//    glPopMatrix();
//    glPopMatrix();
//
//
//    glPushMatrix();
//    glTranslatef(0, -40, -65);
//    gluCylinder(obj, 2, 2, 130, 20, 1);
//    gluSphere(obj, 5, 20, 20);
//    glPushMatrix();
//    glTranslatef(0, 0, 130);
//    gluSphere(obj, 5, 20, 20); 
//    glPopMatrix();
//    glPopMatrix();
//
//    auxSwapBuffers();
//    gluDeleteQuadric(obj);
//}

void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    obj = gluNewQuadric();
    glLoadIdentity();

    glTranslatef(x, y, 0.0);
   // glRotatef(alfaY, 0.0, 1.0, 0.0);
    glRotatef(alfaX, 1.0, 0.0, 0.0);
    glRotatef(alfaY, 0.0, 0.0, 1.0);

    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricNormals(obj, GLU_SMOOTH);

    // --- FIRUL DE NISIP ---
    glPushMatrix();
    glColor3f(0.9f, 0.7f, 0.2f);
    glTranslatef(0, 0, -6);
    gluCylinder(obj, 1.0, 1.0, 30, 10, 3); 
    glPopMatrix();

    // GATUL CLEPSIDREI (CILINDRUL MIC CENTRAL)
    glPushMatrix();
    glColor4f(0.7f, 0.7f, 0.8f, 0.4f);
    glTranslatef(0, 0, -2);
    gluCylinder(obj, 2.5, 2.5, 5, 32, 5);
    glPopMatrix();


    // --- NISIP SUS ---
    glPushMatrix();
    glColor3f(0.9f, 0.7f, 0.2f);
    glTranslatef(0, 0, -4);
    glRotatef(180, 1, 0, 0); 
    gluCylinder(obj, 2.5, 20, 20, 64, 1);

    // Capacul de sus al nisipului (să pară plin)
    glTranslatef(0, 0, 20);
    gluDisk(obj, 0, 20, 64, 1);
    glPopMatrix();


    // CORPUL DE SUS (STICLA CONICA + BAZA LEMN SUS)
    glPushMatrix();
   // glColor3f(0.7f, 0.7f, 0.8f);
    glColor4f(0.7f, 0.7f, 0.8f, 0.4f);
    glTranslatef(0, 0, -62);
    gluCylinder(obj, 30, 30, 30, 64, 10);
    glTranslatef(0, 0, 30);
    gluCylinder(obj, 30, 2.5, 30, 64, 10);
    glTranslatef(0, 0, -30);
    gluDisk(obj, 0, 30, 64, 1);
    
    glColor3f(0.4f, 0.2f, 0.0f);
    glTranslatef(0, 0, -5);
    gluDisk(obj, 0, 45, 64, 1);
    gluCylinder(obj, 45, 45, 5, 64, 1);
    glTranslatef(0, 0, 5);
    gluDisk(obj, 0, 45, 64, 1);
    glPopMatrix();
    
    // --- NISIP JOS ---
    glPushMatrix();
    glColor3f(0.9f, 0.7f, 0.2f);
    // Ne mutăm la baza de jos (unde începe sticla cilindrică)
    // Sticla de jos începe la Z=2 și are un cilindru de 30 unități la final
    glTranslatef(0, 0, 40);

    // Desenăm stratul de nisip (cilindru cu raza 29, înălțime 10)
    gluCylinder(obj, 29, 29, 20, 64, 1);

    // "Capacul" nisipului de jos
    glTranslatef(0, 0, 10);
    gluDisk(obj, 0, 29, 64, 1);

    // Fundul nisipului (opțional, dar bun pentru rotații)
    glTranslatef(0, 0, -10);
    gluDisk(obj, 0, 29, 64, 1);
    glPopMatrix();


    // CORPUL DE JOS (STICLA CONICA + BAZA LEMN JOS)
    glPushMatrix();
   // glColor3f(0.7f, 0.7f, 0.8f);
    glColor4f(0.7f, 0.7f, 0.8f, 0.4f);
    glTranslatef(0, 0, 2);
    gluCylinder(obj, 2.5, 30, 30, 64, 10);
    glTranslatef(0, 0, 30);
    gluCylinder(obj, 30, 30, 30, 64, 10);
    glTranslatef(0, 0, 30);
    gluDisk(obj, 0, 30, 64, 1);

    glColor3f(0.4f, 0.2f, 0.0f);
    gluDisk(obj, 0, 45, 64, 1);
    gluCylinder(obj, 45, 45, 5, 64, 1);
    glTranslatef(0, 0, 5);
    gluDisk(obj, 0, 45, 64, 1);
    glPopMatrix();

    glColor3f(0.4f, 0.2f, 0.0f);

    // TIJA LATERALA 1 (STANGA) +bile
    glPushMatrix();
    glTranslatef(-40, 0, -65);
    gluCylinder(obj, 2, 2, 130, 20, 1);
    glPushMatrix();
    gluSphere(obj, 5, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, 130);
    gluSphere(obj, 5, 20, 20);
    glPopMatrix();
    glPopMatrix();

    // TIJA LATERALA 2 (DREAPTA) +bile
    glPushMatrix();
    glTranslatef(40, 0, -65);
    gluCylinder(obj, 2, 2, 130, 20, 1);
    glPushMatrix();
    gluSphere(obj, 5, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, 130);
    gluSphere(obj, 5, 20, 20);
    glPopMatrix();
    glPopMatrix();

    // TIJA LATERALA 3 (FATA) + bile
    glPushMatrix();
    glTranslatef(0, 40, -65);
    gluCylinder(obj, 2, 2, 130, 20, 1);
    gluSphere(obj, 5, 20, 20);
    glPushMatrix();
    glTranslatef(0, 0, 130);
    gluSphere(obj, 5, 20, 20);
    glPopMatrix();
    glPopMatrix();

    // TIJA LATERALA 4 (SPATE) + bile
    glPushMatrix();
    glTranslatef(0, -40, -65);
    gluCylinder(obj, 2, 2, 130, 20, 1);
    gluSphere(obj, 5, 20, 20);
    glPushMatrix();
    glTranslatef(0, 0, 130);
    gluSphere(obj, 5, 20, 20);
    glPopMatrix();
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

    auxReshapeFunc(myReshape);
    auxMainLoop(display);
    return(0);
}