#include <windows.h>
#include <GL/glut.h>
#include <cstdio>
#include <cstring>

// ---------------- Window ----------------
int windowWidth = 800;
int windowHeight = 600;

// ---------------- Chicken ----------------
float chickenX = 400.0f;
float chickenY = 520.0f;
float chickenSpeed = 2.0f;
int chickenDirection = 1; // 1 =right , -1 = left

// bamboo movement limit
float chickenLeftLimit = 120.0f;
float chickenRightLimit = 680.0f;

// ----------------  falling object ----------------
float objectX = 400.0f;
float objectY = 480.0f;
float fallSpeed = 2.0f;

// 0 = normal egg
// 1 = blue egg
// 2 = golden egg
// 3 = poop
// 4 = perk

int objectType = 0;
bool objectActive = false;

// if objectType == 4 , then perktype decides which perk
// 0 = bigger basket
// 1 = slow fall
// 2 = extra time

int perkType = 0;

// ---------------- Timer ----------------
int totalGameTime = 60;
int timeLeft = 60;
bool gameRunning = true;

// ---------------- Perk states ----------------
bool slowFallActive = false;
bool bigBasketPerkActive = false;
int slowFallTimer = 0;
int bigBasketTimer = 0;

float basketWidth = 60.0f;

// ---------------- Function declarations ----------------
void drawChicken();
void moveChicken();

void spawnObjects();
void updateFallingObjects();

void updateTimer(int value);
void applyPerk(int perkType);

void drawTimeText(float x, float y);
void drawText(float x, float y, const char *text);

void display();
void update(int value);
void init();

// ---------------- Helper: draw text ----------------

void drawText(float x, float y, const char *text)
{
   glRasterPos2f(x, y);

   for (int i = 0; text[i] != '\0'; i++)
   {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
   }
}

//--------------draw Time Text()------------------

void drawTimeText(float x, float y)
{
   char buffer[50];
   sprintf(buffer, "Time : %d", timeLeft);

   glColor3f(1.0f, 1.0f, 1.0f);

   drawText(x, y, buffer);
}

//-----------------Function Implement-----------

void drawChicken()
{
   // body structure

   glColor3f(0.85f, 0.45f, 0.15f);
   glBegin(GL_POLYGON);
   glVertex2f(chickenX - 25, chickenY);
   glVertex2f(chickenX + 25, chickenY);
   glVertex2f(chickenX + 20, chickenY + 25);
   glVertex2f(chickenX - 20, chickenY + 25);
   glEnd();

   // head structure
   glColor3f(0.95f, 0.75f, 0.45f);
   glBegin(GL_POLYGON);
   glVertex2f(chickenX + 15, chickenY + 22);
   glVertex2f(chickenX + 35, chickenY + 22);
   glVertex2f(chickenX + 35, chickenY + 42);
   glVertex2f(chickenX + 15, chickenY + 42);
   glEnd();

   // beak
   glColor3f(1.0f, 0.8f, 0.0f);
   glBegin(GL_TRIANGLES);

   glVertex2f(chickenX + 35, chickenY + 30);
   glVertex2f(chickenX + 48, chickenY + 34);
   glVertex2f(chickenX + 35, chickenY + 38);
   glEnd();

   // comb
   glColor3f(1.0f, 0.0f, 0.0f);
   glBegin(GL_TRIANGLES);

   glVertex2f(chickenX + 18, chickenY + 42);
   glVertex2f(chickenX + 24, chickenY + 52);
   glVertex2f(chickenX + 30, chickenY + 42);
   glEnd();

   // tail structure

   glColor3f(0.6f, 0.2f, 0.1f);
   glBegin(GL_TRIANGLES);
   glVertex2f(chickenX - 25, chickenY + 18);
   glVertex2f(chickenX - 45, chickenY + 40);
   glVertex2f(chickenX - 20, chickenY + 30);
   glEnd();

   // left leg

   glColor3f(1.0f, 0.8f, 0.0f);
   glBegin(GL_LINES);
   glVertex2f(chickenX - 8, chickenY);
   glVertex2f(chickenX - 8, chickenY - 15);
   glEnd();

   // right leg

   glBegin(GL_LINES);
   glVertex2f(chickenX + 8, chickenY);
   glVertex2f(chickenX + 8, chickenY - 15);
   glEnd();
}

void moveChicken()
{
   // stop movement if game is over or paused
   if (gameRunning == false)
   {
      return;
   }

   // move chicken horizontally
   chickenX = chickenX + chickenSpeed * chickenDirection;

   // right boundary
   if (chickenX >= chickenRightLimit)
   {
      chickenX = chickenRightLimit;
      chickenDirection = -1;
   }

   // left boundary
   if (chickenX <= chickenLeftLimit)
   {
      chickenX = chickenLeftLimit;
      chickenDirection = 1;
   }
}

void spawnObjects()
{
   // if one object is already falling , do nothing
   if (objectActive == true)
   {
      return;
   }

   // start new object below chicken
   objectX = chickenX;
   objectY = chickenY - 25;

   // random type
   int r = rand() % 100;

   if (r < 50)
   {
      objectType = 0; // normal egg
   }

   else if (r < 70)
   {
      objectType = 1; // blue egg
   }

   else if (r < 80)
   {
      objectType = 2; // golden egg
   }
   else if (r < 90)
   {
      objectType = 3; // poop
   }

   else
   {
      objectType = 4;        // perk
      perkType = rand() % 3; // 0,1,2
   }
   objectActive = true;
}

//--------------Display---------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT);

   // bamboo stick
   glColor3f(0.8f, 0.7f, 0.3f);
   glBegin(GL_QUADS);
   glVertex2f(80, 500);
   glVertex2f(720, 500);
   glVertex2f(720, 510);
   glVertex2f(80, 510);
   glEnd();

   // chicken
   drawChicken();

   // time text
   drawTimeText(20, 560);

   // temporary object drawing for test
   if (objectActive == true)
   {
      // normal egg = white
      if (objectType == 0)
         glColor3f(1.0f, 1.0f, 1.0f);

      // blue egg = blue
      else if (objectType == 1)
      {
         glColor3f(0.2f, 0.4f, 1.0f);
      }

      // golden egg = yellow
      else if (objectType == 2)
      {
         glColor3f(1.0f, 0.8f, 0.0f);
      }

      // poop = brown
      else if (objectType == 3)
      {
         glColor3f(0.4f, 0.2f, 0.0f);
      }

      // perk =green
      else
      {
         glColor3f(0.0f, 1.0f, 0.0f);
      }

      glBegin(GL_QUADS);
      glVertex2f(objectX - 8, objectY);
      glVertex2f(objectX + 8, objectY);
      glVertex2f(objectX + 8, objectY + 16);
      glVertex2f(objectX - 8, objectY + 16);
      glEnd();
   }

   glFlush();
}

void update(int value)
{
   // move chicken
   moveChicken();

   // try to create one object if none exists
   spawnObjects();

   // redraw the  screen
   glutPostRedisplay();

   // call update function again after 16 ms
   glutTimerFunc(16, update, 0);
}

//------------Init-----------
void init()
{
   glClearColor(0.4f, 0.8f, 1.0f, 1.0f);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0, windowWidth, 0, windowHeight);
}

//-----------------Main------------------

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

   glutInitWindowSize(windowWidth, windowHeight);

   glutCreateWindow("Catch The Eggs - Test");

   init();

   glutDisplayFunc(display);

   glutTimerFunc(16, update, 0);

   glutMainLoop();
   return 0;
}
