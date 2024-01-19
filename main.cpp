// ReSharper disable All
#include <GL/freeglut.h>
#include <cmath>
#include <chrono>


GLfloat katX = 0.0f;  // Kat obrotu wokol osi X
GLfloat katY = 0.0f;  // Kat obrotu wokol osi Y
GLfloat odlegloscKamery = 10.0f;  // Poczatkowa odleglosc kamery

GLfloat rotacja = 0.0f;  // Kat obrotu obiekt�w
std::chrono::high_resolution_clock::time_point lastUpdateTime;  // Ostatnia aktualizacja czasu

GLfloat przesuniecieX = 0.0f, przesuniecieY = 0.0f, przesuniecieZ = 0.0f;  // Przesuni�cia dla obiekt�w
GLfloat skalowanie = 1.0f;  // Skalowanie obiekt�w

int myszX = 0;  // Pozycja X kursora myszy
int myszY = 0;  // Pozycja Y kursora myszy
bool naciscniecieMyszy = false;  // Flaga informujaca o nacisnieciu przycisku myszy



class Obiekt3D {
public:
    // Rysowanie szescianu
    static void szescian() {
        glPushMatrix();
        // Obr�t szesciana wok�l osi X i Y
        glRotatef(rotacja, 0.0f, 1.0f, 0.0f);

        glBegin(GL_QUADS);
        // Dolna sciana
        glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f); glVertex3f(0.5f, -0.5f, 0.5f); glVertex3f(-0.5f, -0.5f, 0.5f);

        // G�rna sciana
        glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f); glVertex3f(0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);

        // Przednia sciana
        glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f); glVertex3f(0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);

        // Tylna sciana
        glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f); glVertex3f(0.5f, 0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, -0.5f);

        // Lewa sciana
        glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, -0.5f);

        // Prawa sciana
        glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f); glVertex3f(0.5f, 0.5f, 0.5f); glVertex3f(0.5f, 0.5f, -0.5f);

        glEnd();
        glPopMatrix();
    }

    // Rysowanie stozka
    static void stozek() {
        glPushMatrix();
        glColor3f(1.0f, 1.0f, 0.0f);  // zolty
        glTranslatef(2.5f, -0.5f, 0.0f);
        glRotatef(180.0f, 0.0f, 1.0f, 1.0f);
        glutSolidCone(0.5, 1.0, 20, 20);  // Rysowanie stozka
        glPopMatrix();
    }

    // Rysowanie walca
    static void walec() {
        glPushMatrix();
        glColor3f(0.0f, 1.0f, 1.0f);  // Jasnoniebieski
        glTranslatef(-2.5f, 0.0f, 0.0f);  // Przesuniecie walca na bok
        glutSolidCylinder(0.5, 2.0, 20, 20);  // Rysowanie walca
        glPopMatrix();
    }
};

class Scena{
public:
    // Aktualizacja sceny
    static void update() {
        // Aktualizacja animacji, logika gry, itp.
        auto currentTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastUpdateTime).count() / 1000000.0;

        rotacja += 60.0f * deltaTime;  // Predkosc obrotu r�wna 60 stopni na sekunde

        lastUpdateTime = currentTime;

        glutPostRedisplay();  // Wymuszenie ponownego rysowania sceny
    }

    // Renderowanie sceny
    static void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
        glTranslatef(0.0f, 0.0f, -odlegloscKamery);
        glRotatef(katX, 1.0f, 0.0f, 0.0f);
        glRotatef(katY, 0.0f, 1.0f, 0.0f);

        glTranslatef(przesuniecieX, przesuniecieY, przesuniecieZ);
        glScalef(skalowanie, skalowanie, skalowanie);

        // Rysowanie obiekt�w 3D
        Obiekt3D::szescian();
        Obiekt3D::stozek();
        Obiekt3D::walec();

        glutSwapBuffers();
    }
};
class Klawiatura {
public:
    // Obsluga klawiatury
    static void klawiatura(unsigned char klawisz, int x, int y) {
        switch (klawisz) {
        case 27:  // Esc - Wyj�cie z programu
            exit(0);
            break;
        case 'w':  // W - Przesuni�cie obiekt�w w g�r�
            przesuniecieY += 0.1f;
            break;
        case 's':  // S - Przesuni�cie obiekt�w w d�
            przesuniecieY -= 0.1f;
            break;
        case 'a':  // A - Przesuni�cie obiekt�w w lewo
            przesuniecieX -= 0.1f;
            break;
        case 'd':  // D - Przesuni�cie obiekt�w w prawo
            przesuniecieX += 0.1f;
            break;
        case 'q':  // Q - Przesuni�cie obiekt�w w prz�d
            przesuniecieZ += 0.1f;
            break;
        case 'e':  // E - Przesuni�cie obiekt�w w ty�
            przesuniecieZ -= 0.1f;
            break;
        case '+':  // + - Powi�kszenie obiekt�w
            skalowanie += 0.1f;
            break;
        case '-':  // - - Zmniejszenie obiekt�w
            if (skalowanie > 0.1f) {
                skalowanie -= 0.1f;
            }
            break;
        }
        glutPostRedisplay();
    }

    // Obsluga ruchu z klawiatury
    static void klawiaturaRuch(int klawisz, int x, int y) {
        switch (klawisz) {
        case GLUT_KEY_UP:
            katX += 5.0f;
            break;
        case GLUT_KEY_DOWN:
            katX -= 5.0f;
            break;
        case GLUT_KEY_LEFT:
            katY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            katY += 5.0f;
            break;
        }

        glutPostRedisplay();
    }
};
class Mysz {
public:
    // Obsluga myszy
    static void mysz(int przycisk, int stan, int x, int y) {
        if (przycisk == GLUT_LEFT_BUTTON) {
            if (stan == GLUT_DOWN) {
                naciscniecieMyszy = true;
                myszX = x;
                myszY = y;
            }
            else if (stan == GLUT_UP) {
                naciscniecieMyszy = false;
            }
        }
    }

    // Obsluga ruchu myszy
    static void ruchMysz(int x, int y) {
        if (naciscniecieMyszy) {
            int deltaX = x - myszX;
            int deltaY = y - myszY;

            katY += deltaX * 0.1f;
            katX += deltaY * 0.1f;

            myszX = x;
            myszY = y;

            glutPostRedisplay();
        }
    }

    // Obsluga scrolla myszy
    static void myszScroll(int przycisk, int kierunek, int x, int y) {
        if (kierunek > 0) {
            // Scroll w g�re, przyblizanie kamery
            odlegloscKamery -= 0.5f;
        }
        else if (kierunek < 0) {
            // Scroll w d�l, oddalanie kamery
            odlegloscKamery += 0.5f;
        }

        glutPostRedisplay();
    }
};

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("PGK");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(Scena::render);
    glutKeyboardFunc(Klawiatura::klawiatura);
    glutSpecialFunc(Klawiatura::klawiaturaRuch);
    glutMouseFunc(Mysz::mysz);
    glutMotionFunc(Mysz::ruchMysz);
    glutMouseWheelFunc(Mysz::myszScroll);


    glClearColor(0.0, 0.0, 0.0, 1.0);

    lastUpdateTime = std::chrono::high_resolution_clock::now();

    
    glutIdleFunc(Scena::update);  //  funkcja do aktualizacji 

    glutMainLoop();

    return 0;
}
