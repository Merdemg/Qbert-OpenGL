
#include "Texture.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector3.h"
#include <math.h>
#include <iostream>
#include <time.h>
#include <string.h>
#include <fstream>
#include <rpcsal.h>
#include <string>
#include <fstream>
#include "Wave.h"
//#include "mmsystem.h"

/*  Create checkerboard texture  */
#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;

Texture backgroundTex;
Texture helpTex;
Texture quitTex;
Texture playTex;
Texture logoTex;
Texture helpScreenTex;


std::string playername;
std::ifstream HiScoreFile;
std::ifstream NameFile;
int top1, top2, top3;

std::string top1name, top2name, top3name;

float timePrev = glutGet(GLUT_ELAPSED_TIME);

float winX = 0;
float winZ = 40;

using namespace std;

CVector3 cameraPos = CVector3(0.0, 0.0, 0.0);
CVector3 newCamPos = CVector3(0.0, 0.0, 0.0);
CVector3 lookAt = CVector3(0.0, 0.0, 10.0);
float rotationAngle = 0.0;


int deltaTime = 0;
int oldTime = 0;
int initial_time = 0;
int elapsed_time = 0;
int timeSinceLastSpawn = 3000;
int spawnRate = 5000;
int moveRate = 500;
int timeSinceLastAIMove = 0;

int highlightedMenuItem = 1;
float highlightedAngle = 0;
float rotationSpeed = 0.4f;

int myScore = 0;
int numOfCubesChanged = 0;
int gameState = 0;
int numOfLives = 3;
int levelsBeaten = 0;

int a = 0;
int b = 0;

Wave menuSound("menu.wav");
Wave levelSound("level.wav");
Wave hurtSound("hurt.wav");
Wave powerup("powerup.wav");

void backgroundTextureRunTime();
void playTextureRunTime();
void helpTextureRunTime();
void quitTextureRunTime();
void logoTextureRunTime();
void helpScreenTextureRunTime();

int colorArray[9][9];
int objectArray[9][9];

int playerX;
int playerY;

char scoreBoard[25];

bool weHaveASnake = false;

bool willStartNewLevel = false;

int timeBeforeNewLevel = 500;

void reshape(int w, int h)
{

	//cout<<"w = " << w << endl << "h = " << h << endl;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (gameState == 0)
	{
		gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.1, 3000.0);
	} else if (gameState == 1)
	{
		glOrtho(-12, 12, -14, 10, 0.1, 3000);
	}

	glMatrixMode(GL_MODELVIEW);


}

void printScore(){
	cout << "SCORE: " << myScore << endl;
}

void init(void)
{
	srand (time(NULL));


	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			colorArray[i][j] = 0;
			objectArray[i][j] = 0;
		}
	}

	//add disk
	int myTemp = rand() % 6;
	myTemp += 2;
	objectArray[0][myTemp] = 6;
	//add disk2
	myTemp = rand() % 6;
	myTemp += 2;
	objectArray[myTemp][9-myTemp] = 6;
	//add player
	objectArray[1][7] = 1;
	playerX = 1;
	playerY = 7;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	//Light for milestone 2 & 3 maybe?
	GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};


	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { -10.0, -10.0, -20.0, 0.0 };
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat light_position2[] = { -10.0, -10.0, -20.0, 0.0 };

	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//glLightfv(GL_LIGHT1, GL_POSITION, light_position2);


	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

}

void gameOver(){

	if (myScore > top3)
	{
		cout << "New high score: " << myScore << endl;

		top3 = myScore;
		top3name = playername;

		if (top3 > top2)
		{
			top3 = top2;
			top3name = top2name;
			top2 = myScore;
			top2name = playername;

			if (top2 > top1)
			{
				top2 = top1;
				top2name = top1name;
				top1 = myScore;
				top1name = playername;
			}

		}

		cout << "New TOP 3 scores: " << myScore << endl;
		cout << top1name << " " << top1 << endl;
		cout << top2name << " " << top2 << endl;
		cout << top3name << " " << top3 << endl;


		ofstream NameFile("names.txt");
		if (NameFile.is_open())
		{
			NameFile << top1name << "\n";
		NameFile << top2name << "\n";
		NameFile << top3name << "\n";

		}else
		{
			cout << "Can't open name file" << endl;
		}
		ofstream HiScoreFile("hiscore.txt");

		if (HiScoreFile.is_open())
		{
					HiScoreFile << top1 << "\n";
		HiScoreFile << top2 << "\n";
		HiScoreFile << top3 << "\n";
		}else
		{
			cout << "Can't open hi score file" << endl;
		}

	}



	gameState = 0;



	timeSinceLastSpawn = 3000;
 timeSinceLastAIMove = 0;

 highlightedMenuItem = 1;
 highlightedAngle = 0;

 myScore = 0;
 numOfCubesChanged = 0;
 numOfLives = 3;
 levelsBeaten = 0;

 a = 0;
 b = 0;

		//init();

 for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			colorArray[i][j] = 0;
			objectArray[i][j] = 0;
		}
	}

	objectArray[1][7] = 1;

	reshape(500,500);
	glutPostRedisplay();
}

void SpawnBall(){
	int temp;
	if (weHaveASnake)
	{
		 temp = rand() % 2;
	}else
	{
		 temp = rand() % 3;
	}

	if (objectArray[1][7] == 1 && temp == 0)
	{
		powerup.play(1);
		myScore += 100;
		printScore();
	} else if (objectArray[1][7] == 1 && temp == 1)
	{
		hurtSound.play(1);
		numOfLives--;
	} else if (temp == 0)
	{
		objectArray[1][7] = 2;
	}else if (temp == 1)
	{
		objectArray[1][7] = 3;
	}else if (objectArray[1][7] == 1 && temp == 2)
	{
		powerup.play(1);
		myScore += 50;
		printScore();
	}else
	{
		objectArray[1][7] = 4;
	}


	if (numOfLives < 1)
	{
		gameOver();
	}

}

void moveAI(){

	bool AIwillMove = true;
	bool diskCanMove = true;
	bool snakeCanMove = true;


	if (objectArray[1][8] == 7)
	{
		objectArray[1][8] = 0;
		objectArray[1][7] = 1;
		playerX = 1;
		playerY = 7;
	}else if (objectArray[0][8] == 7)
	{
		objectArray[0][8] = 0;
		objectArray[1][7] = 1;
		playerX = 1;
		playerY = 7;
	}


	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (objectArray[i][j] == 2 && AIwillMove)
			{
				int temp = rand() % 2;

				if (j == 1)
				{
					objectArray[i][j] = 0;

				}else if (temp == 0 && i < 6 && j > 0)
				{
					if (objectArray[i+1][j-1] == 0)
					{
						objectArray[i+1][j-1] = 2;
					}else
					{
						powerup.play(1);
						myScore+=100;
						printScore();
					}
					objectArray[i][j] = 0;
					AIwillMove = false;

				} else{
					if (objectArray[i][j-1] == 0)
					{
						objectArray[i][j-1] = 2;
					}else
					{
						powerup.play(1);
							myScore+=100;
							printScore();

					}

					objectArray[i][j] = 0;
					AIwillMove = false;
				}

			} else if (objectArray[i][j] == 3 && AIwillMove)
			{
				int temp2 = rand() % 2;


				if (j == 1)
				{
					objectArray[i][j] = 0;
				} else if (temp2 == 0 && i < 6 && j > 0)
				{
					if (objectArray[i+1][j-1] == 0)
					{
						objectArray[i+1][j-1] = 3;
					}else
					{
						hurtSound.play(1);
						numOfLives--;					
					}
					objectArray[i][j] = 0;
					AIwillMove = false;

				} else{
					if (objectArray[i][j-1] == 0)
					{
						objectArray[i][j-1] = 3;
					}else
					{
						hurtSound.play(1);
						numOfLives--;
					}
					objectArray[i][j] = 0;
					AIwillMove = false;
				}
			}else if (objectArray[i][j] == 4 && AIwillMove)
			{
				int temp = rand() % 2;

				if (j == 1)
				{
					weHaveASnake = true;
					objectArray[i][j] = 5;

				}else if (temp == 0 && i < 6 && j > 0)
				{
					if (objectArray[i+1][j-1] == 0)
					{
						objectArray[i+1][j-1] = 4;
					}else
					{
						powerup.play(1);
						myScore+=50;
						printScore();
					}
					objectArray[i][j] = 0;
					AIwillMove = false;

				} else{
					if (objectArray[i][j-1] == 0)
					{
						objectArray[i][j-1] = 4;
					}else
					{
						powerup.play(1);
							myScore+=50;
							printScore();

					}

					objectArray[i][j] = 0;
					AIwillMove = false;
				}
			}
			if (objectArray[i][j] == 7 && diskCanMove)
			{
				if (i == 0)
				{
					objectArray[i][j] = 0;
					objectArray[i][j+1]  = 7;
				}else
				{
					objectArray[i][j] = 0;
					objectArray[i-1][j+1]  = 7;
				}
				diskCanMove = false;
			}
			
			if (objectArray[i][j] == 5 && snakeCanMove)
			{
				//left down
				if (playerX > i && playerY <= j && i != 7 && j != 1)
				{
					objectArray[i][j] = 0;
						if (objectArray[i+1][j-1] != 1)
						{
							objectArray[i+1][j-1]  = 5;
						}else
						{
							hurtSound.play(1);
							weHaveASnake = false;
							numOfLives--;
						}



				}else if (playerX <= i && playerY >= j && i !=1 && j != 7)
				{
					objectArray[i][j] = 0;
					if (objectArray[i-1][j+1] != 1)
						{
							objectArray[i-1][j+1]  = 5;
						}else
						{
							hurtSound.play(1);
							weHaveASnake = false;
							numOfLives--;
						}
				}else if (playerY < j && j != 1)
				{
					objectArray[i][j] = 0;
					if (objectArray[i][j-1] != 1)
						{
							objectArray[i][j-1]  = 5;
						}else
						{
							hurtSound.play(1);
							weHaveASnake = false;
							numOfLives--;
						}
				} else if(i != 7)
				{
					objectArray[i][j] = 0;
					if (objectArray[i][j+1] != 1)
						{
							objectArray[i][j+1]  = 5;
						}else
						{
							hurtSound.play(1);
							weHaveASnake = false;
							numOfLives--;
						}
				}


				snakeCanMove = false;
			}
		}
	}



	if (numOfLives <1)
	{
		gameOver();
	}

}


void newLevel(){
levelSound.play(1);
levelsBeaten++;
myScore+=1000;
printScore();

if (levelsBeaten % 5 == 0 && numOfLives < 3)
{
	numOfLives++;
}


timeSinceLastSpawn = 3000;
timeSinceLastAIMove = 0;

numOfCubesChanged = 0;

 a = 0;
 b = 0;

 for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			colorArray[i][j] = 0;
			objectArray[i][j] = 0;
		}
	}


	//add disk
	int myTemp = rand() % 6;
	myTemp += 2;
	objectArray[0][myTemp] = 6;
	//add disk2
	myTemp = rand() % 6;
	myTemp += 2;
	objectArray[myTemp][9-myTemp] = 6;


	objectArray[1][7] = 1;
}


void calculateTime(){

	elapsed_time = glutGet(GLUT_ELAPSED_TIME) - initial_time;
	
	deltaTime = elapsed_time - oldTime;
	//cout << deltaTime << endl;
	oldTime = elapsed_time;

	if (gameState == 1)
	{
		if (timeSinceLastAIMove > moveRate)
		{
			moveAI();
			timeSinceLastAIMove = 0;
		} else
		{
			timeSinceLastAIMove += deltaTime;
		}


		if (timeSinceLastSpawn > spawnRate)
		{
			SpawnBall();
			timeSinceLastSpawn = 0;
		} else
		{
			timeSinceLastSpawn += deltaTime;
		}
	}

	if (willStartNewLevel)
	{
		timeBeforeNewLevel -= deltaTime;

		if (timeBeforeNewLevel < 0)
		{
			willStartNewLevel = false;
			timeBeforeNewLevel = 500;
			newLevel();
		}

	}

	//cout << elapsed_time << endl;
}

void backgroundTexture(){
	glGenTextures(1, &backgroundTex.texID);

	glBindTexture(GL_TEXTURE_2D, backgroundTex.texID);



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, backgroundTex.bpp / 8, backgroundTex.width, backgroundTex.height, 0, backgroundTex.type, GL_UNSIGNED_BYTE, backgroundTex.imageData);
}

void helpTexture(){
	glGenTextures(1, &helpTex.texID);

	glBindTexture(GL_TEXTURE_2D, helpTex.texID);



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, helpTex.bpp / 8, helpTex.width, helpTex.height, 0, helpTex.type, GL_UNSIGNED_BYTE, helpTex.imageData);

}

void playTexture(){


	glGenTextures(1, &playTex.texID);

	glBindTexture(GL_TEXTURE_2D, playTex.texID);



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, playTex.bpp / 8, playTex.width, playTex.height, 0, playTex.type, GL_UNSIGNED_BYTE, playTex.imageData);

}

void quitTexture(){
	glGenTextures(1, &quitTex.texID);

	glBindTexture(GL_TEXTURE_2D, quitTex.texID);



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, quitTex.bpp / 8, quitTex.width, quitTex.height, 0, quitTex.type, GL_UNSIGNED_BYTE, quitTex.imageData);

}

void logoTexture(){
	glGenTextures(1, &logoTex.texID);
	
	glBindTexture(GL_TEXTURE_2D, logoTex.texID);



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, logoTex.bpp / 8, logoTex.width, logoTex.height, 0, logoTex.type, GL_UNSIGNED_BYTE, logoTex.imageData);

}

void helpScreenTexture(){
	glGenTextures(1, &helpScreenTex.texID);

	glBindTexture(GL_TEXTURE_2D, helpScreenTex.texID);



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, helpScreenTex.bpp / 8, helpScreenTex.width, helpScreenTex.height, 0, helpScreenTex.type, GL_UNSIGNED_BYTE, helpScreenTex.imageData);
}

void drawDisk(){
	//gluDisk(
	glRotatef(45, 0, 1, 0);
	glRotatef(90,1,0,0);
	glRotatef(180, 0,0,1);
	glTranslatef(0,0,1.0f);
	//glColor4f(0,1,1,0.8f);
	glColor3f(0,1,1);
	GLUquadricObj *disk;
	disk = gluNewQuadric();
	gluDisk(disk, 0, 1.1f, 30, 10); 
	//gluCylinder(disk, 0.5f, 0.5f, 0.2f, 500, 100);
	glTranslatef(0,0,-1.0f);
	glRotatef(-180, 0,0,1);
	glRotatef(-90,1,0,0);
	glRotatef(-45, 0, 1, 0);
}


void drawCube(){
	glBegin(GL_QUADS);


	/*
	if (a == 1)
	{
					glColor4f(1,0,0,0.8);

	} else
	{
					glColor4f(0,1,0,0.8);

	}
	*/



	
	if (colorArray[a][b] == 0)
	{
			glColor4f(0,0,1,0.8);

	} else if (colorArray[a][b] == 1)
	{
			glColor4f(1,0.5,0,0.8);

	}else
	{
		glColor4f(1,0.5,1,0.8);
	}
	
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
   glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);



   glColor4f(0.5,0.5,0.5,1);
	//glColor3f(0, 1, 0);

   glVertex3f(-1.0, -1.0, 1.0);
   glVertex3f(1.0, -1.0, 1.0);
   glVertex3f(1.0, 1.0, 1.0);
   glVertex3f(-1.0, 1.0, 1.0);
   //glColor4f(0, 1, 0, 1);

   //glColor4f(1,1,1,1);
   glVertex3f(-1.0, -1.0, -1.0);
   glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
	//glColor4f(0, 1, 0, 1);

   glColor4f(0.3,0.3,0.3,1);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
   glVertex3f(-1.0, 1.0, 1.0);
   glVertex3f(-1.0, -1.0, 1.0);
   //glColor4f(0, 1, 0, 1);

   glColor4f(0,1,0,1);
     glVertex3f(1.0, -1.0, -1.0);
   glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
	//glColor4f(0, 1, 0, 1);

	glColor4f(1,1,1,1);
   glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);
	//glColor4f(0, 1, 0, 1);

	

	 glEnd();
}

void drawCube2(){
	
	glBegin(GL_QUADS);


	//glColor4f(1, 0, 0, 1);
	//glColor3f(1, 0, 0);
	glTexCoord4f(1.0, 0.0, 0, 1);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord4f(0.0, 1.0, 0, 1);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord4f(1.0, 1.0, 0, 1);
	glVertex3f(-1.0, 1.0, 1.0);
	glEnd();

	//glColor3f(1, 0, 0);

	glBegin(GL_QUADS);

	//glColor4f(1,1,1,1);
	glTexCoord4f(1.0, 0.0, 0, 1);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord4f(0.0, 1.0, 0, 1);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord4f(1.0, 1.0, 0, 1);
	glVertex3f(-1.0, 1.0, -1.0);
	//glColor3f(1, 0, 0);
	glEnd();

	glBegin(GL_QUADS);

	//glColor4f(0,1,0,1);
	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord4f(1.0, 0.0, 0, 1);
	glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord4f(1.0, 1.0, 0, 1);
	glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord4f(0.0, 1.0, 0, 1);
	glVertex3f(-1.0, -1.0, 1.0);
	//glColor3f(1, 0, 0);
	////glColor4f(0,1,1,1);
	glBegin(GL_QUADS);

	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord4f(1.0, 0.0, 0, 1);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord4f(1.0, 1.0, 0, 1);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord4f(0.0, 1.0, 0, 1);
	glVertex3f(1.0, -1.0, 1.0);
	//glColor3f(1, 0, 0);
	glEnd();

	glBegin(GL_QUADS);

	//glColor4f(0,0,1,1);
	glTexCoord4f(0.0, 0.0, 0, 1);

	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord4f(1.0, 0.0, 0, 1);
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord4f(0.0, 1.0, 0, 1);
	glVertex3f(-1.0, -1.0, 1.0);
	//glColor3f(1, 0, 0);
	glEnd();

	glBegin(GL_QUADS);

	//glColor4f(1,0,0,0.8);
	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord4f(1.0, 0.0, 0, 1);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord4f(1.0, 1.0, 0, 1);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord4f(0.0, 1.0, 0, 1);
	glVertex3f(-1.0, 1.0, 1.0);

	glEnd();
}

void drawSnake(){
	glBegin(GL_QUADS);


	glColor4f(1, 0, 1, 1);
	//glColor3f(1, 0, 0);
	glTexCoord4f(0.75, 0.0, 0, 1);
	glVertex3f(-0.75, -0.75, 0.75);
	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(0.75, -0.75, 0.75);
	glTexCoord4f(0.0, 0.75, 0, 1);
	glVertex3f(0.75, 0.75, 0.75);
	glTexCoord4f(0.75, 0.75, 0, 1);
	glVertex3f(-0.75, 0.75, 0.75);
	glEnd();

	//glColor3f(1, 0, 0);

	glBegin(GL_QUADS);

	//glColor4f(1,1,1,1);
	glTexCoord4f(0.75, 0.0, 0, 1);
	glVertex3f(-0.75, -0.75, -0.75);
	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(0.75, -0.75, -0.75);
	glTexCoord4f(0.0, 0.75, 0, 1);
	glVertex3f(0.75, 0.75, -0.75);
	glTexCoord4f(0.75, 0.75, 0, 1);
	glVertex3f(-0.75, 0.75, -0.75);
	//glColor3f(1, 0, 0);
	glEnd();

	glBegin(GL_QUADS);

	//glColor4f(0,1,0,1);
	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(-0.75, -0.75, -0.75);
	glTexCoord4f(0.75, 0.0, 0, 1);
	glVertex3f(-0.75, 0.75, -0.75);
	glTexCoord4f(0.75, 0.75, 0, 1);
	glVertex3f(-0.75, 0.75, 0.75);
	glTexCoord4f(0.0, 0.75, 0, 1);
	glVertex3f(-0.75, -0.75, 0.75);
	//glColor3f(1, 0, 0);
	////glColor4f(0,1,1,1);
	glBegin(GL_QUADS);

	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(0.75, -0.75, -0.75);
	glTexCoord4f(0.75, 0.0, 0, 1);
	glVertex3f(0.75, 0.75, -0.75);
	glTexCoord4f(0.75, 0.75, 0, 1);
	glVertex3f(0.75, 0.75, 0.75);
	glTexCoord4f(0.0, 0.75, 0, 1);
	glVertex3f(0.75, -0.75, 0.75);
	//glColor3f(1, 0, 0);
	glEnd();

	glBegin(GL_QUADS);

	//glColor4f(0,0,1,1);
	glTexCoord4f(0.0, 0.0, 0, 1);

	glVertex3f(-0.75, -0.75, -0.75);
	glTexCoord4f(0.75, 0.0, 0, 1);
	glVertex3f(0.75, -0.75, -0.75);
	glVertex3f(0.75, 0.75, 0.75);
	glVertex3f(0.75, -0.75, 0.75);
	glTexCoord4f(0.0, 0.75, 0, 1);
	glVertex3f(-0.75, -0.75, 0.75);
	//glColor3f(1, 0, 0);
	glEnd();

	glBegin(GL_QUADS);

	//glColor4f(1,0,0,0.8);
	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(-0.75, 0.75, -0.75);
	glTexCoord4f(0.75, 0.0, 0, 1);
	glVertex3f(0.75, 0.75, -0.75);
	glTexCoord4f(0.75, 0.75, 0, 1);
	glVertex3f(0.75, 0.75, 0.75);
	glTexCoord4f(0.0, 0.75, 0, 1);
	glVertex3f(-0.75, 0.75, 0.75);

	glEnd();


}

void drawTitle(){
	glBegin(GL_QUADS);


	//glColor4f(1, 0, 0, 1);
	//glColor3f(1, 0, 0);
	glTexCoord4f(1.0, 0.0, 0, 1);
	glVertex3f(-2.0, -1.0, 0.0);
	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(2.0, -1.0, 0.0);
	glTexCoord4f(0.0, 1.0, 0, 1);
	glVertex3f(2.0, 1.0, 0.0);
	glTexCoord4f(1.0, 1.0, 0, 1);
	glVertex3f(-2.0, 1.0, 0.0);
	glEnd();

	//glColor3f(1, 0, 0);

}

void drawBackground(){
	glBegin(GL_QUADS);


	//glColor4f(1, 0, 0, 1);
	//glColor3f(1, 0, 0);
	glTexCoord4f(1.0, 0.0, 0, 1);
	glVertex3f(-10.0, -10.0, 0.0);
	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(10.0, -10.0, 0.0);
	glTexCoord4f(0.0, 1.0, 0, 1);
	glVertex3f(10.0, 10.0, 0.0);
	glTexCoord4f(1.0, 1.0, 0, 1);
	glVertex3f(-10.0, 10.0, 0.0);
	glEnd();

}

void drawHelp(){
	glBegin(GL_QUADS);


	//glColor4f(1, 0, 0, 1);
	//glColor3f(1, 0, 0);
	glTexCoord4f(1.0, 0.0, 0, 1);
	glVertex3f(-4.0, -4.0, 0.0);
	glTexCoord4f(0.0, 0.0, 0, 1);
	glVertex3f(4.0, -4.0, 0.0);
	glTexCoord4f(0.0, 1.0, 0, 1);
	glVertex3f(4.0, 4.0, 0.0);
	glTexCoord4f(1.0, 1.0, 0, 1);
	glVertex3f(-4.0, 4.0, 0.0);
	glEnd();

}

void drawGameArena(){
	a = 1;
	b = 1;

	//glRotatef(45, 0,1,0);

	//glLoadIdentity();
	//glTranslated(0,0,0);
	glTranslatef(-12, -12, 0);
	glColor3f(1,0,0);
	drawCube();

	for (int i = 0; i < 6; i++)
	{
		a++;
		glTranslatef(2, 0, -2);
		drawCube();
	}

	a--;
	b++;
	glTranslatef(0,2, 2);
	drawCube();
	for (int i = 0; i < 5; i++)
	{
		a--;
		glTranslatef(-2,0,2);
		drawCube();
	}

	//a++;
	b++;
	glTranslatef(2, 2,0);
	drawCube();
	for (int i = 0; i < 4; i++)
	{
		a++;
		glTranslatef(2, 0, -2);
		drawCube();
	}

	a--;
	b++;
	glTranslatef(0,2,2);
	drawCube();
	for (int i = 0; i < 3; i++)
	{
		a--;
		glTranslatef(-2,0,2);
		drawCube();
	}
	//a=0;
	b++;
	glTranslatef(2, 2,0);
	drawCube();
	for (int i = 0; i < 2; i++)
	{
		a++;
		glTranslatef(2, 0, -2);
		drawCube();
	}
	a--;
	//cout << "a is " << a << endl;
	b++;
	glTranslatef(0,2,2);
	drawCube();
	a--;
	//cout << "a is " << a << endl;

	glTranslatef(-2,0,2);
	drawCube();


	a=1;
	b++;
	glTranslatef(2, 2,0);
	drawCube();

}

void drawPlayer(){
	glColor4f(1,1,0,1);
	glutSolidSphere(1,100,20);
}

void drawGreen(){
	glColor4f(0,1,0,1);
	glutSolidSphere(0.5,100,20);
}

void drawRed(){
	glColor4f(1,0,0,1);
	glutSolidSphere(0.5,100,20);
}

void drawPurple(){
	glColor4f(1,0,1,1);
	glutSolidSphere(0.5,100,20);
}

void drawObjects(){
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (objectArray[i][j] == 1)
			{
				//cout << "Drawing player at " << i << " " << j << endl;
				glTranslatef((i*2) +(j*2)-16,(j*2) -12,((-2)*i) + 2);
				drawPlayer();
				//drawDisk();
				glTranslatef((-i*2) +(-j*2)+16,(-j*2) +12,((2)*i) + -2);

			} else if (objectArray[i][j] == 2)
			{
				glTranslatef((i*2) +(j*2)-16,(j*2) -12,((-2)*i) + 2);
				drawGreen();
				glTranslatef((-i*2) +(-j*2)+16,(-j*2) +12,((2)*i) -2);


			} else if (objectArray[i][j] == 3)
			{
				glTranslatef((i*2) +(j*2)-16,(j*2) -12,((-2)*i) + 2);
				drawRed();
				glTranslatef((-i*2) +(-j*2)+16,(-j*2) +12,((2)*i) -2);

			}else if (objectArray[i][j] == 4)
			{
				glTranslatef((i*2) +(j*2)-16,(j*2) -12,((-2)*i) + 2);
				drawPurple();
				glTranslatef((-i*2) +(-j*2)+16,(-j*2) +12,((2)*i) -2);
			}else if (objectArray[i][j] == 5)
			{
				glTranslatef((i*2) +(j*2)-16,(j*2) -12,((-2)*i) + 2);
				drawSnake();
				glTranslatef((-i*2) +(-j*2)+16,(-j*2) +12,((2)*i) -2);
			}
			else if (objectArray[i][j] == 6)
			{
				glTranslatef((i*2) +(j*2)-16,(j*2) -12,((-2)*i) + 2);
				drawDisk();
				glTranslatef((-i*2) +(-j*2)+16,(-j*2) +12,((2)*i) -2);
			}else if (objectArray[i][j] == 7)
			{
				glTranslatef((i*2) +(j*2)-16,(j*2) -12,((-2)*i) + 2);
				drawPlayer();
				drawDisk();
				glTranslatef((-i*2) +(-j*2)+16,(-j*2) +12,((2)*i) -2);
			}
		}
	}

}

void drawMainMenu(){
	
	glTranslatef(0,3,7);
	logoTextureRunTime();
	drawTitle();
	glTranslatef(0,-3,0);

	glPushMatrix();
	glTranslatef(0,-2,0);
	if (highlightedMenuItem == 1)
	{
		glRotatef(highlightedAngle, 0,1,0);
	}
	helpTextureRunTime();
	drawCube2();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3,-1,0);
	if (highlightedMenuItem == 2)
	{
		glRotatef(highlightedAngle, 0,1,0);
	}
	quitTextureRunTime();
	drawCube2();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3,-1,0);
	if (highlightedMenuItem == 0)
	{
		glRotatef(highlightedAngle, 0,1,0);
	}
	//playTexture();
	playTextureRunTime();
	drawCube2();
	glPopMatrix();


	//glLoadIdentity();
	
	glPushMatrix();
	glTranslatef(0,0,3);
	backgroundTextureRunTime();
	drawBackground();
	glPopMatrix();
	
}

void drawHelpScreen(){

	glTranslatef(0,3,7);
	logoTextureRunTime();
	drawTitle();
	glTranslatef(0,-3,0);


	//glPushMatrix();
	glTranslatef(0,-2,0);
	helpScreenTextureRunTime();
	drawHelp();
	//glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0,2,3);
	backgroundTextureRunTime();
	drawBackground();
	glPopMatrix();
	
}

void drawLives(){
	glTranslatef(-6,4,6);
	glColor4f(1,1,0,1);
	if (numOfLives > 0)
	{
		glutSolidSphere(1,100,20);
	}

	glTranslatef(0,-3,0);
	if (numOfLives > 1)
	{
		glutSolidSphere(1,100,20);
	}

	glTranslatef(0,-3,0);
	if (numOfLives > 2)
	{
		glutSolidSphere(1,100,20);
	}
}

void print(int x, int y, char *chars){
	int length = strlen(chars);
	glRasterPos3i(x,y,-1);
	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, chars[i]);
	}
}


void display(void)
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glLoadIdentity();
	//gluLookAt(0, 0, ffar, 0, 0, 0, 0, 1, 0);

	if (gameState == 0 || gameState == 2)
	{
		gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z , lookAt.x, lookAt.y, lookAt.z, 0,1,0);
	} else if (gameState == 1)
	{
		//glRotatef(45, 0, 0, 1);
		//glRotatef(45, 0, 1, 0);
		//glRotatef(45, 1, 0, 0);
		gluLookAt(-8,3,-8,0,-6,0, 0,1,0);
	}
	

	//glPushMatrix();
	//glLoadIdentity();
	//glDisable(GL_TEXTURE_2D);
	if (gameState == 0)
	{
		drawMainMenu();
	} else if (gameState == 1){
			//glPopMatrix();
		glPushMatrix();
		glDisable(GL_TEXTURE_2D);
		drawGameArena();
		glPopMatrix();
		glPushMatrix();
		drawObjects();
		glPopMatrix();
		glPushMatrix();
		drawLives();
		glPopMatrix();
		//sprintf(scoreBoard, "SCORE");
		//printf(scoreBoard);

		glPushMatrix();
	glTranslatef(7,4,-7);
		print(0,0,"");
		sprintf_s(scoreBoard, "SCORE: %i", myScore);
		print(0,0,scoreBoard);
		glPopMatrix();
	} else if (gameState == 2)
	{
		drawHelpScreen();
	}

	
	glLoadIdentity();


	glFlush();
	//glutSwapBuffers();
	
}

void changeColor(){
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (objectArray[i][j] == 1)
			{
				if (colorArray[i][j] == 0 && i != 0 && j != 0)
				{
					colorArray[i][j] = 1;
					//cout<< "colorarrey" << i << " " << j << " is changed"<<endl;
					myScore += 25;
					printScore();
					numOfCubesChanged++;
					if (levelsBeaten == 0 && numOfCubesChanged == 28)
					{
						//newLevel();
						willStartNewLevel = true;
					}
				}else if (colorArray[i][j] == 1 && i != 0 && j != 0 && levelsBeaten > 0)
				{
					colorArray[i][j] = 2;
					//cout<< "colorarrey" << i << " " << j << " is changed"<<endl;
					myScore += 25;
					printScore();
					numOfCubesChanged++;
					if (levelsBeaten > 0 && numOfCubesChanged == 56)
					{
						//newLevel();
						willStartNewLevel = true;
					}
				}
			}
		}
	}

}

void checkCollision(int myX, int myY, int i, int j){
	if (objectArray[i+myX][j+myY] == 2)
	{
		powerup.play(1);
		myScore+= 100;
		printScore();
	}else if (objectArray[i+myX][j+myY] == 3 || objectArray[i+myX][j+myY] == 5)
	{
		hurtSound.play(1);
		numOfLives--;
	}
}

void movePlayer(char myDir){
	bool willMove = true;
	//cout << "Moving Player " << myDir << endl;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (objectArray[i][j] == 1 && willMove)
			{
				if (myDir == 'w')
				{
					if (j < 8 && (i+j) < 8)
					{
						checkCollision(0, 1,i,j);
					objectArray[i][j] = 0;
					objectArray[i][j+1] = 1;
					playerY++;
					//cout<<"Player is at " << i << " " <<j+1 << endl;
					willMove = false;
					} else if (objectArray[i][j+1] == 6)

					{
						objectArray[i][j] = 0;
						objectArray[i][j+1] = 7;
						willMove = false;
					}

				} else if (myDir == 'e')
				{
					if (i > 1 && j < 8)
					{
						checkCollision(-1, 1,i,j);
					objectArray[i][j] = 0;
					objectArray[i-1][j+1] = 1;
					playerX--;
					playerY++;
					//cout<<"Player is at " << i-1 << " " <<j+1 << endl;
					}else if (objectArray[i-1][j+1] == 6)
					{
						objectArray[i][j] = 0;
						objectArray[i-1][j+1] = 7;
					}
					

					willMove = false;

				} else if (myDir == 's' && i < 8 && j > 1)
				{
					checkCollision(1, -1,i,j);
					objectArray[i][j] = 0;
					objectArray[i+1][j-1] = 1;
					playerX++;
					playerY--;
										//cout<<"Player is at " << i+1 << " " <<j-1 << endl;

					willMove = false;

				} else if (myDir == 'd' && j > 1)
				{
					checkCollision(0, -1,i,j);
					objectArray[i][j] = 0;
					objectArray[i][j-1] = 1;
					playerY--;
										//cout<<"Player is at " << i << " " <<j-1 << endl;

					willMove = false;

				}
			}
		}
	}

	if (willMove == false)
	{
		changeColor();
	}

	if (numOfLives < 1)
	{
			gameOver();
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		if (gameState == 2)
		{
			PlaySound(TEXT("menu.wav"), NULL, SND_FILENAME);
			gameState = 0;
		} else if (gameState == 1)
		{
			exit(0);
		}
		//reshape(500,500);
		glutPostRedisplay();
		break;
	case 13:
		if (gameState == 0)
		{
			if (highlightedMenuItem == 0)
			{
				//PlaySound(TEXT("menu.wav"), NULL, SND_FILENAME);
				
				gameState = 1;
				GLfloat light_position[] = { -20.0, 50.0, 80.0, 0.0 };
				glLightfv(GL_LIGHT0, GL_POSITION, light_position);
				glEnable(GL_LIGHT0);

				reshape(500,500);
			} else if (highlightedMenuItem == 1)
			{
				PlaySound(TEXT("menu.wav"), NULL, SND_FILENAME);
				gameState = 2;
			} else if (highlightedMenuItem == 2)
			{
				exit(0);
			}			
		}		
		glutPostRedisplay();
		break;

case 'w':
		if (gameState == 1)
		{
			movePlayer('w');
		}
		glutPostRedisplay();
		break;
case 'e':
		if (gameState == 1)
		{
			movePlayer('e');
		}
		glutPostRedisplay();
		break;

case 's':
		if (gameState == 1)
		{
			movePlayer('s');
		}
		glutPostRedisplay();
		break;

case 'd':
		if (gameState == 1)
		{
			movePlayer('d');
		}
		glutPostRedisplay();
		break;


	default:
		break;
	}


	glutPostRedisplay();
}



void moveMenuItem(){
	highlightedAngle += rotationSpeed * deltaTime;

	while (highlightedAngle > 360)
	{
		highlightedAngle = highlightedAngle - 360;
	}
}

void idle()
{
	if (gameState == 0)
	{
			calculateTime();
			moveMenuItem();
	} else if (gameState == 1)
	{
		calculateTime();
	}
	
	glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {
	switch (key)
	{
		case GLUT_KEY_RIGHT:
		if (gameState == 0 && highlightedMenuItem < 2)
		{
			highlightedMenuItem++;
		}
		break;
	case GLUT_KEY_LEFT:
		if (gameState == 0 && highlightedMenuItem > 0)
		{
			highlightedMenuItem--;
		}
		break;
	default:
		break;
	}

}

int main(int argc, char** argv)
{
	//Wave menuSound("menu.wav");
	menuSound.play(1);


	cout << "Please enter name:" << endl;
	cin >> playername;
	cout << "Welcome, " << playername << "!" << endl;

	HiScoreFile.open("hiscore.txt");
	if (HiScoreFile.is_open())
	{
		HiScoreFile >> top1 >> top2 >> top3;
		/*
		cout << "Current High Scores:" << endl;
		cout << top1 << endl;
		cout << top2 << endl;
		cout << top3 << endl;
		*/
	}else
	{
		cout<< "Missing hi score file" << endl;
	}

	NameFile.open("names.txt");
	if (NameFile.is_open())
	{
		//HiScoreFile >> top1name >> top2name >> top3name;
		getline(NameFile, top1name);
		getline(NameFile, top2name);
		getline(NameFile, top3name);



		cout << "Current High Scores:" << endl;
		cout << top1name << " " << top1 << endl;
		cout << top2name << " " << top2 << endl;
		cout << top3name << " " << top3 << endl;

	}else
	{
		cout<< "Missing hi score names file" << endl;
	}
	

	LoadTGA(&backgroundTex, "cubes.tga");
	LoadTGA(&helpTex, "helpYellow.tga");
	LoadTGA(&playTex, "playBlue.tga");
	LoadTGA(&quitTex, "quitRed.tga");
	LoadTGA(&logoTex, "logo.tga");
	LoadTGA(&helpScreenTex, "Help.tga");

	glutInit(&argc, argv);
	glEnable(GL_DEPTH_TEST);
	//glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	playTexture();
	helpTexture();
	quitTexture();
	logoTexture();
	helpScreenTexture();
	backgroundTexture();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(processSpecialKeys);
	glutMainLoop();
	return 0;
}

void playTextureRunTime(){

	glTexImage2D(GL_TEXTURE_2D, 0, playTex.bpp / 8, playTex.width, playTex.height, 0, playTex.type, GL_UNSIGNED_BYTE, playTex.imageData);

}

void backgroundTextureRunTime(){
		glTexImage2D(GL_TEXTURE_2D, 0, backgroundTex.bpp / 8, backgroundTex.width, backgroundTex.height, 0, backgroundTex.type, GL_UNSIGNED_BYTE, backgroundTex.imageData);
}

void helpTextureRunTime(){
		glTexImage2D(GL_TEXTURE_2D, 0, helpTex.bpp / 8, helpTex.width, helpTex.height, 0, helpTex.type, GL_UNSIGNED_BYTE, helpTex.imageData);

}

void quitTextureRunTime(){
		glTexImage2D(GL_TEXTURE_2D, 0, quitTex.bpp / 8, quitTex.width, quitTex.height, 0, quitTex.type, GL_UNSIGNED_BYTE, quitTex.imageData);

}

void logoTextureRunTime(){
			glTexImage2D(GL_TEXTURE_2D, 0, logoTex.bpp / 8, logoTex.width, logoTex.height, 0, logoTex.type, GL_UNSIGNED_BYTE, logoTex.imageData);

}

void helpScreenTextureRunTime(){
			glTexImage2D(GL_TEXTURE_2D, 0, helpScreenTex.bpp / 8, helpScreenTex.width, helpScreenTex.height, 0, helpScreenTex.type, GL_UNSIGNED_BYTE, helpScreenTex.imageData);

}