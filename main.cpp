#include <windows.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>

const int W = 800, H = 600;
const float PI = 3.1415926f;
const int BAR_THICK = 16;

// Moving quad (new animation)
const float XR1 = 728.0f, YR1 = 0.0f;
const float XR2 = 454.6f, YR2 = 313.7f;
const float XL1_INIT = 720.0f, YL1 = 0.0f;
const float XL2_INIT = 440.0f, YL2 = 300.0f;
float shift = 0.0f;
float maxShift = 25.0f;
float speed = 140.0f / 60.0f;

// ===================== ANIMATION =====================
float roadOffset = 0.0f;
float sunPhase   = 0.0f;
float balloonX = (0.78f + 1.0f) * (W * 0.5f);
float balloonSpeed = 0.0045f * (W * 0.5f);
float birdX = (-1.30f + 1.0f) * (W * 0.5f);
float birdSpeed = 0.0060f * (W * 0.5f);

// Red ball (simple movement)
float redBallX = 120.0f;
float redBallY = 300.0f;
float redBallSpeed = 2.0f;
// ===================== PIXEL CIRCLE/ELLIPSE =====================
void drawEllipsePx(float x, float y, float rx, float ry, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 60; i++) {
            float a = 2.0f * 3.14159f * i / 60.0f;
            glVertex2f(x + cos(a) * rx, y + sin(a) * ry);
        }
    glEnd();
}

void drawFilledCircle(float cx, float cy, float r)
{
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= 360; i++)
        {
            float a = i * PI / 180.0f;
            glVertex2f(cx + cos(a) * r, cy + sin(a) * r);
        }
    glEnd();
}

void wave()
{
    float xL1 = XL1_INIT - shift;
    float xL2 = XL2_INIT - shift;

    glColor3f(0.15f, 0.60f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(xL1, 0.0f);
        glVertex2f(xL2, 300.0f);
        glVertex2f(454.6f, 313.7f);
        glVertex2f(728.0f, 0.0f);
    glEnd();
}

// ===================== SUN =====================
void drawSunPulsing()
{
    float sunCX = (0.78f + 1.0f) * (W * 0.5f);
    float sunCY = (0.78f + 1.0f) * (H * 0.5f);
    float sunBaseRx = 0.11f * (W * 0.5f);
    float sunBaseRy = 0.11f * (H * 0.5f);
    float sunPulseAmp = 0.12f;

    // 1) pulse factor using sine wave
    float pulse = 1.0f + sunPulseAmp * sinf(sunPhase);

    // 2) current radius (changes smoothly)
    float rx = sunBaseRx * pulse;
    float ry = sunBaseRy * pulse;

    // 3) sun disk
    drawEllipsePx(sunCX, sunCY, rx, ry, 0.95f, 0.78f, 0.10f);

    // Simple pulsing sun (no rays)
}

// ===================== HANDLEBAR =====================
void drawHandlebar()
{
    int baseBarY = 420;
    int handH    = 170;

    int povMargin = -20;
    int handDrop  = 0;

    int barBot0  = baseBarY - BAR_THICK / 2;
    int lowest0  = barBot0 - handDrop - handH;
    int sceneDy = povMargin - lowest0;

    int barY   = baseBarY + sceneDy;
    int barTop = barY + BAR_THICK / 2;
    int barBot = barY - BAR_THICK / 2;

    int barL = 185;
    int barR = 615;

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
        glVertex2i(barL, barTop);
        glVertex2i(barR, barTop);
        glVertex2i(barR, barBot);
        glVertex2i(barL, barBot);
    glEnd();

    int dropBottomY = 340 + sceneDy;
    int lean = 10;

    glBegin(GL_QUADS);
        glVertex2i(barL, barBot);
        glVertex2i(barL + BAR_THICK, barBot);
        glVertex2i(barL + BAR_THICK + lean, dropBottomY);
        glVertex2i(barL + lean, dropBottomY);

        glVertex2i(barR - BAR_THICK, barBot);
        glVertex2i(barR, barBot);
        glVertex2i(barR - lean, dropBottomY);
        glVertex2i(barR - BAR_THICK - lean, dropBottomY);
    glEnd();

    glColor3f(0.55f, 0.35f, 0.15f);
    int gripHalfW = 30;
    int gripHalfH = 14;

    int leftGripCX  = 270;
    int rightGripCX = 530;

    glBegin(GL_QUADS);
        glVertex2i(leftGripCX - gripHalfW, barY + gripHalfH);
        glVertex2i(leftGripCX + gripHalfW, barY + gripHalfH);
        glVertex2i(leftGripCX + gripHalfW, barY - gripHalfH);
        glVertex2i(leftGripCX - gripHalfW, barY - gripHalfH);

        glVertex2i(rightGripCX - gripHalfW, barY + gripHalfH);
        glVertex2i(rightGripCX + gripHalfW, barY + gripHalfH);
        glVertex2i(rightGripCX + gripHalfW, barY - gripHalfH);
        glVertex2i(rightGripCX - gripHalfW, barY - gripHalfH);
    glEnd();

    glColor3f(0.55f, 0.35f, 0.15f);

    int handTopY = barBot - handDrop;

    glBegin(GL_QUADS);
        glVertex2i(230, handTopY);
        glVertex2i(260, handTopY);
        glVertex2i(260, handTopY - handH);
        glVertex2i(230, handTopY - handH);

        glVertex2i(540, handTopY);
        glVertex2i(570, handTopY);
        glVertex2i(570, handTopY - handH);
        glVertex2i(540, handTopY - handH);
    glEnd();

    glColor3f(0.12f, 0.12f, 0.12f);
    glBegin(GL_POLYGON);
        glVertex2i(365, barY);
        glVertex2i(435, barY);
        glVertex2i(415, 260 + sceneDy);
        glVertex2i(385, 260 + sceneDy);
    glEnd();

    glColor3f(0.75f, 0.75f, 0.75f);
    drawFilledCircle((barL + BAR_THICK / 2.0f) + lean, dropBottomY, 8);
    drawFilledCircle((barR - BAR_THICK / 2.0f) - lean, dropBottomY, 8);
}
// ===================== BALLOON =====================
void drawBalloon(float x, float y)
{
    drawEllipsePx(x, y, 34, 34, 0.95f, 0.15f, 0.15f);

    glColor3f(0.40f, 0.22f, 0.08f);
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2f(x - 16, y - 16);
        glVertex2f(x - 7, y - 48);

        glVertex2f(x + 16, y - 16);
        glVertex2f(x + 7, y - 48);
    glEnd();
    glLineWidth(1);

    glColor3f(0.55f, 0.30f, 0.10f);
    glBegin(GL_QUADS);
        glVertex2f(x - 12, y - 48);
        glVertex2f(x + 12, y - 48);
        glVertex2f(x + 12, y - 70);
        glVertex2f(x - 12, y - 70);
    glEnd();
}

// ===================== BIRD =====================
void drawBird(float x, float y)
{

    glColor3f(0, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x + 14, y + 5);

        glVertex2f(x + 14, y + 5);
        glVertex2f(x + 28, y);
    glEnd();
    glLineWidth(1);
}

// ===================== TREE =====================
void drawTree()
{
    glColor3f(0.35f, 0.20f, 0.08f);
    glBegin(GL_QUADS);
        glVertex2f(32, 282);
        glVertex2f(48, 282);
        glVertex2f(48, 362);
        glVertex2f(32, 362);
    glEnd();

    glColor3f(0.05f, 0.45f, 0.10f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(40, 392);
        for (int i = 0; i <= 60; i++) {
            float a = 2.0f * 3.14159f * i / 60.0f;
            glVertex2f(40 + cos(a) * 28, 392 + sin(a) * 28);
        }
    glEnd();

    glColor3f(0.35f, 0.20f, 0.08f);
    glBegin(GL_QUADS);
        glVertex2f(72, 282);
        glVertex2f(88, 282);
        glVertex2f(88, 362);
        glVertex2f(72, 362);
    glEnd();

    glColor3f(0.05f, 0.45f, 0.10f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(80, 392);
        for (int i = 0; i <= 60; i++) {
            float a = 2.0f * 3.14159f * i / 60.0f;
            glVertex2f(80 + cos(a) * 28, 392 + sin(a) * 28);
        }
    glEnd();

    glColor3f(0.05f, 0.45f, 0.10f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(60, 402);
        for (int i = 0; i <= 60; i++) {
            float a = 2.0f * 3.14159f * i / 60.0f;
            glVertex2f(60 + cos(a) * 40, 402 + sin(a) * 40);
        }
    glEnd();
}

void drawHorizonTrees()
{
     // closer

    drawTree();
}

// ===================== BACKGROUND =====================
void drawBackground()
{
    float birdGap = 48.0f;
    float birdY  = 426.0f;
    float bird2Y = 414.0f;
    float balloonY = 510.0f;

    glColor3f(0.95f, 0.85f, 0.10f);
    glBegin(GL_QUADS);
        glVertex2f((-1.0f + 1.0f) * (W * 0.5f), (-1.0f + 1.0f) * (H * 0.5f));
        glVertex2f(( 0.0f + 1.0f) * (W * 0.5f), (-1.0f + 1.0f) * (H * 0.5f));
        glVertex2f(( 0.0f + 1.0f) * (W * 0.5f), ( 0.0f + 1.0f) * (H * 0.5f));
        glVertex2f((-1.0f + 1.0f) * (W * 0.5f), ( 0.0f + 1.0f) * (H * 0.5f));
    glEnd();

    glColor3f(0.15f, 0.60f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(( 0.0f + 1.0f) * (W * 0.5f), (-1.0f + 1.0f) * (H * 0.5f));
        glVertex2f(( 1.0f + 1.0f) * (W * 0.5f), (-1.0f + 1.0f) * (H * 0.5f));
        glVertex2f(( 1.0f + 1.0f) * (W * 0.5f), ( 0.0f + 1.0f) * (H * 0.5f));
        glVertex2f(( 0.0f + 1.0f) * (W * 0.5f), ( 0.0f + 1.0f) * (H * 0.5f));
    glEnd();

    // Removed horizon line (no black line)

    drawSunPulsing();
    drawBalloon(balloonX, balloonY);

    drawBird(birdX,           birdY);
    drawBird(birdX + birdGap, bird2Y);
}

// ===================== ROAD =====================
void drawRoad()
{

    glColor3f(0.90f, 0.80f, 0.15f);
    glBegin(GL_QUADS);
        glVertex2f((-1.0f + 1.0f) * (W * 0.5f), (-1.0f + 1.0f) * (H * 0.5f));
        glVertex2f(( 1.0f + 1.0f) * (W * 0.5f), (-1.0f + 1.0f) * (H * 0.5f));
        glVertex2f(( 0.1f + 1.0f) * (W * 0.5f), ( 0.0f + 1.0f) * (H * 0.5f));
        glVertex2f((-0.1f + 1.0f) * (W * 0.5f), ( 0.0f + 1.0f) * (H * 0.5f));
    glEnd();

    // center dashes (stop before horizon)
    glColor3f(0.30f, 0.20f, 0.05f);
    for (float i = -1.0f; i <= -0.20f; i += 0.2f)
    {
        float pos = i + roadOffset;
        if (pos > -0.05f) continue;

        glLineWidth(2.0f);
        glBegin(GL_LINES);
            glVertex2f((pos * 0.1f + 1.0f) * (W * 0.5f), (pos + 1.0f) * (H * 0.5f));
            glVertex2f(((pos + 0.1f) * 0.1f + 1.0f) * (W * 0.5f), ((pos + 0.1f) + 1.0f) * (H * 0.5f));
        glEnd();
    }
    glLineWidth(1);
}

// ===================== RED BALL =====================
void drawRedBallLeft()
{
    float yNdc = (redBallY / (H * 0.5f)) - 1.0f;
    float xNdc = -1.0f + 0.9f * (yNdc + 1.0f);
    float xPix = (xNdc + 1.0f) * (W * 0.5f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(xPix, redBallY);
        for (int i = 0; i <= 60; i++) {
            float a = 2.0f * 3.14159f * i / 60.0f;
            glVertex2f(xPix + cos(a) * 14, redBallY + sin(a) * 14);
        }
    glEnd();
}

// ===================== GLUT =====================
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawBackground();
    drawHorizonTrees();
    drawRoad();
    wave();
    drawRedBallLeft();
    drawHandlebar();
    glutSwapBuffers();
}

void update(int)
{
    roadOffset += 0.015f;
    if (roadOffset > 0.2f) roadOffset = 0.0f;

    sunPhase += 0.06f;

    balloonX -= balloonSpeed;
    if (balloonX < 0.0f) balloonX = W;

    birdX += birdSpeed;
    if (birdX > W) birdX = 0.0f;

    redBallY -= redBallSpeed;
    if (redBallY < 0.0f) redBallY = 300.0f;

    shift += speed;
    if (shift >= maxShift) shift = 0.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void initGL()
{
    glClearColor(0.4f, 0.7f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, W, 0, H);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(W, H);
    glutCreateWindow("No Horizon Line");

    initGL();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
