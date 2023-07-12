#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

float A, B, C; //rotation angles

float cubeWidth = 20; 
const int screen_width = 160, screen_height = 44;
float zBuffer[160 * 44]; // zBuffer is an interesting concept I learned here
char buffer[160 * 44];

int background = ' '; // this can be changed to whatever you want the background to look like, I prefer empty.
int distanceFromCube = 100;
float K1 = 30; // distance to the projeted screen

float incrementSpeed = 0.9;

float x, y, z; // these are the x,y,z coordinates
float ooz; // stands for one over z.
int xp, yp; // these are the projected coordinates
int index;

// Below, I use Linear Algebra to apply the rotation matrices to each of the (x,y,z) coordinates.
float calculateX(int i, int j, int k) {
  return j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C) +  j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C);
}

float calculateY(int i, int j, int k) {
  return  k * cos(A) * sin(B) * sin(C) - i * cos(B) * sin(C) + j * cos(A) * cos(C) + k * sin(A) * cos(C) - j * sin(A) * sin(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
  return i * sin(B) + k * cos(A) * cos(B) - j * sin(A) * cos(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch) {
  x = calculateX(cubeX, cubeY, cubeZ);
  y = calculateY(cubeX, cubeY, cubeZ);
  z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCube;

  ooz = 1 / z;

  xp = (int)(screen_width / 2 + K1 * ooz * x * 2);
  yp = (int)(screen_height / 2 + K1 * ooz * y);

  index = xp + yp * screen_width;
  if (index >= 0 && index <screen_width *screen_height) {
    if (ooz > zBuffer[index]) {
      zBuffer[index] = ooz;
      buffer[index] = ch;
    }
  }
}

int main() {
  printf("\x1b[2J"); // reset
  while (1) { // we want the program to continuously display 
    memset(buffer, background,screen_width *screen_height);
    memset(zBuffer, 0, screen_width *screen_height * 4);
    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
      for (float cubeY = -cubeWidth; cubeY < cubeWidth;
           cubeY += incrementSpeed) {
        calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
        calculateForSurface(cubeWidth, cubeY, cubeX, '$');
        calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
        calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
        calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
        calculateForSurface(cubeX, cubeWidth, cubeY, '+');
      }
    }

    printf("\x1b[H"); //reset
    for (int k = 0; k <screen_width *screen_height; k++) {
      putchar(k %screen_width ? buffer[k] : 10);
    }

    A += 0.06;
    B += 0.06; // control how fast/slow you want the cube to spin
    C += 0.02; 
    usleep(2000 * 2);
  }
  
  return 0;
}
