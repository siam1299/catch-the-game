#include <windows.h>
#include <GL/glut.h>
#include <cstdio>
#include <cstring>
#include <math.h>

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

void drawFilledCircle(float cx, float cy, float r)
{
   glBegin(GL_TRIANGLE_FAN);
   glVertex2f(cx, cy);

   for (int i = 0; i <= 100; i++)
   {
      float angle = 2.0f * 3.1416f * i / 100;
      float x = r * cos(angle);
      float y = r * sin(angle);
      glVertex2f(cx + x, cy + y);
   }
   glEnd();
}

void drawSingleCloud(float x, float y)
{
   // cloud color = white
   glColor3f(1.0f, 1.0f, 1.0f);

   // 4 overlapping circles = 1 cloud

   drawFilledCircle(x, y, 18);
   drawFilledCircle(x + 20, y + 10, 20);
   drawFilledCircle(x + 40, y, 18);
   drawFilledCircle(x + 20, y - 5, 22);
}

void drawClouds()
{
   drawSingleCloud(80, 540);
   drawSingleCloud(220, 470);
   drawSingleCloud(380, 520);
   drawSingleCloud(560, 480);
   drawSingleCloud(650, 530);
}

void drawField()
{
   // green ground
   glColor3f(0.2f, 0.7f, 0.2f);
   glBegin(GL_QUADS);
   glVertex2f(0, 0);
   glVertex2f(windowWidth, 0);
   glVertex2f(windowWidth, 80);
   glVertex2f(0, 80);
   glEnd();

   glColor3f(0.1f, 0.6f, 0.1f);
   glBegin(GL_POLYGON);
   glVertex2f(0, 80);
   glVertex2f(100, 70);
   glVertex2f(220, 78);
   glVertex2f(340, 72);
   glVertex2f(460, 82);
   glVertex2f(580, 74);
   glVertex2f(700, 79);
   glVertex2f(800, 72);
   glVertex2f(800, 0);
   glVertex2f(0, 0);
   glEnd();
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

void updateTimer(int value)
{
   // if game already stopped, do nothing
   if (gameRunning == false)
   {
      return;
   }

   // reduce remaining time by 1 second
   timeLeft--;

   // stop game when time becomes 0
   if (timeLeft <= 0)
   {
      timeLeft = 0;
      gameRunning = false;
      return;
   }

   // call this function again after 1 second

   glutTimerFunc(1000, updateTimer, 0);
}

//--------------Display---------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT);

   // draw clouds first
   drawClouds();

   // draw green field
   drawField();

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

      // perk block
      else if (objectType == 4)
      {
         // bigger basket perk
         if (perkType == 0)
         {
            glColor3f(0.0f, 1.0f, 0.0f);
         }

         // slow fall perk
         else if (perkType == 1)
         {
            glColor3f(1.0f, 0.5f, 0.0f);
         }

         // extra time perk

         else if (perkType == 2)
         {
            glColor3f(1.0f, 0.0f, 1.0f);
         }
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
   // move chicken only if game is running
   moveChicken();

   // handle spawning + falling
   updateFallingObjects();

   // if bigger basket perk is active, reduce it's remaining frames
   if (bigBasketActive == true)
   {
      bigBasketFrames--;

      // when perk time ends, return basket to normal size
      if (bigBasketFrames <= 0)
      {
         bigBasketActive = false;
         basketWidth = 60.0f;
      }
   }

   // if slow fall perk is active , reduce its remaining frames

   if (slowFallActive == true)
   {
      slowFallFrames--;

      // when perk time ends, return fall speed to normal
      if (slowFallFrames <= 0)
      {
         slowFallActive = false;
         fallSpeed = 2.0f;
      }
   }
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
   // start glut
   glutInit(&argc, argv);

   // set display mode
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

   // set window size
   glutInitWindowSize(windowWidth, windowHeight);

   // create window
   glutCreateWindow("Catch The Eggs - Test");
   // OpenGL setup
   init();

   // display callback
   glutDisplayFunc(display);

   // frame update loop
   glutTimerFunc(16, update, 0);

   // game timer loop
   glutTimerFunc(1000, updateTimer, 0);

   // keep programming running
   glutMainLoop();
   return 0;
}
