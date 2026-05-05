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
bool bigBasketActive = false;
int slowFallFrames = 0;
int bigBasketFrames = 0;

float basketWidth = 60.0f;
float basketX = 300.0f;
float basketY = 40.0f;
int score = 0;
float basketHeight = 20.0f;

float caughtObjectX = 0.0f;
float caughtObjectY = 0.0f;
int caughtObjectPoints = 0;

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

void drawBasket();
void moveBasketKeyboard(int key, int x, int y);
void moveBasketMouse(int x, int y);
void setCaughtObjectData(float x, float y, int points);
bool checkCatch();
void updateScore(int points);
void drawScoreText();

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

void updateTimer(int value)
{
   if (gameRunning == true && timeLeft > 0)
   {
      timeLeft--;
   }
   else if (timeLeft <= 0)
   {
      gameRunning = false;
   }

   glutPostRedisplay();
   glutTimerFunc(1000, updateTimer, 0);
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

void drawBasket()
{
   glColor3f(1.0f, 1.0f, 0.0f);

   glBegin(GL_POLYGON);
   glVertex2f(basketX - basketWidth / 2, basketY);
   glVertex2f(basketX + basketWidth / 2, basketY);
   glVertex2f(basketX + basketWidth / 3, basketY + basketHeight);
   glVertex2f(basketX - basketWidth / 3, basketY + basketHeight);
   glEnd();
}

void moveBasketKeyboard(int key, int x, int y)
{
   if (gameRunning == false)
   {
      return;
   }

   if (key == GLUT_KEY_LEFT)
   {
      basketX -= 20.0f;
   }
   else if (key == GLUT_KEY_RIGHT)
   {
      basketX += 20.0f;
   }

   if (basketX < basketWidth / 2)
      basketX = basketWidth / 2;

   if (basketX > windowWidth - basketWidth / 2)
      basketX = windowWidth - basketWidth / 2;

   glutPostRedisplay();
}

void moveBasketMouse(int x, int y)
{
   if (gameRunning == false)
   {
      return;
   }

   basketX = (float)x;

   if (basketX < basketWidth / 2)
      basketX = basketWidth / 2;

   if (basketX > windowWidth - basketWidth / 2)
      basketX = windowWidth - basketWidth / 2;

   glutPostRedisplay();
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
   if (objectActive == true)
   {
      return;
   }

   objectX = chickenX;
   objectY = chickenY - 25;

   int r = rand() % 100;

   if (r < 60)
   {
      objectType = 0; // white egg
   }
   else if (r < 85)
   {
      objectType = 2; // golden egg
   }
   else
   {
      objectType = 3; // poop
   }

   objectActive = true;
}

void updateFallingObjects()
{
   // if game stopped, do nothing
   if (gameRunning == false)
   {
      return;
   }

   // if no object exists, create one
   if (objectActive == false)
   {
      spawnObjects();
      return;
   }
   // move object downward
   objectY = objectY - fallSpeed;

   int points = 0;

   if (objectType == 0)
      points = 1;
   else if (objectType == 2)
      points = 10;
   else if (objectType == 3)
      points = -10;

setCaughtObjectData(objectX, objectY, points);

   if (checkCatch())
   {
      objectActive = false;
   }

   // if object goes below screen, remove it
   if (objectY < 0)
   {
      objectActive = false;
   }
}

void applyPerk(int perkType)
{
   // bigger basket
   if (perkType == 0)
   {
      basketWidth = 100.0f; // default 60
      bigBasketActive = true;
      bigBasketFrames = 600; // about 10 sec if update runs ~ 60 FPS
   }

   // slower fall speed
   else if (perkType == 1)
   {
      fallSpeed = 1.0f;
      slowFallActive = true;
      slowFallFrames = 600;
   }

   // extra time
   else if (perkType == 2)
   {
      timeLeft = timeLeft + 5;

      // optional cap
      if (timeLeft > totalGameTime + 20)
      {
         timeLeft = totalGameTime + 20;
      }
   }
}

void drawScoreText()
{
   char buffer[50];
   sprintf(buffer, "Score : %d", score);

   glColor3f(1.0f, 1.0f, 1.0f);

   drawText(20, 530, buffer);
}

void setCaughtObjectData(float x, float y, int points)
{
   caughtObjectX = x;
   caughtObjectY = y;
   caughtObjectPoints = points;
}

bool checkCatch()
{
   if (caughtObjectY <= basketY + basketHeight &&
       caughtObjectX >= basketX - basketWidth / 2 &&
       caughtObjectX <= basketX + basketWidth / 2)
   {
      updateScore(caughtObjectPoints);
      return true;
   }
   return false;
}

void updateScore(int points)
{
   score += points;
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
   
   // basket
   drawBasket();


   // time text
   drawTimeText(20, 560);
   drawScoreText();

   // temporary object drawing for test
   if (objectActive == true)
   {
      if (objectType == 0)
      {
         glColor3f(1.0f, 1.0f, 1.0f); // white egg
      }
      else if (objectType == 2)
      {
         glColor3f(1.0f, 0.8f, 0.0f); // golden egg
      }
      else if (objectType == 3)
      {
         glColor3f(0.4f, 0.2f, 0.0f); // poop
      }

      // simple object shape

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

   // handle spawning + falling
   updateFallingObjects();
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

   glutSpecialFunc(moveBasketKeyboard);

   glutPassiveMotionFunc(moveBasketMouse);

   glutTimerFunc(16, update, 0);

   glutTimerFunc(1000, updateTimer, 0);

   glutMainLoop();
   return 0;
}
