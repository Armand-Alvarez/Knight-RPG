/*
Armand Alvarez & Luke Terhall
Project Started 8/11/2016

KnightRPG.c
*/



/*			CURRENTLY IN PROGRESS			*/
/*PUT ANY FUNCTIONS THAT ARE NOT FINISHED HERE*/
/*--------------------------------------------*/
void mainMenu(void);
void setUpGame(void);
/*--------------------------------------------*/


/*			Function Prototypes			*/
/*	ONLY PUT FINISHED FUNCTIONS HERE	*/
/*--------------------------------------*/
void setupScreenSize(void);
void printLaunchScreen(void);
void printHeader(int style);
/*--------------------------------------*/



#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>


/*			FILE NAMES			*/
/*------------------------------*/
#define LAUNCHTEXT "LaunchTitle.txt"
/*------------------------------*/

#define MAXNAMELENGTH 12

typedef struct Player
{
	int xPosition;
	int yPosition;
	int health;
	char *name;
}Player;

Player PLAYER;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int main (void)
{	
	setUpGame();
	
	printLaunchScreen();

	/*TEST FUNCTION*/
	printHeader(2);
	/*END TEST FUNCTION*/
}


/* Sets up the game settings to default. */
void setUpGame(void)
{
	setupScreenSize();
	PLAYER.name="";
}


/* Sets up the dimensions for CMD prompt. */
void setupScreenSize(void)
{
  	system("MODE 105, 25");
	system("cls");
}


/* Prints the launch screen in ASCII art. */
void printLaunchScreen(void)
{
	FILE *fpt = fopen(LAUNCHTEXT, "r");
	char c;
	
	if (fpt)
	{
		while ((c=getc(fpt)) != EOF)
		{
			putchar(c);
			usleep(6000);
		}
	fclose(fpt);
	getch();
	}
}



/* 	Opens Main Menu. 
	Includes Start Game.
	Includes Credits. */
void mainMenu(void)
{
	printHeader(1);
	
	
}


/*	Clears the screen.
	Prints the header on top.
	1 for basic header (name of game only).
	2 for full header (DEFINE FULL HEADER). */
void printHeader(int style)
{
	system("cls");
	
	if (style==1)
	{
		printf("\t\t\t\t\t\tKNIGHT RPG\n");
		printf("---------------------------------------------------------------------------------------------------------\n");
		getch();
	}
	
	else if (style==2)
	{
		/* Use this failsafe if the character name has not been created yet. */
		if(PLAYER.name=="")
		{
			printf("\t\t\t\t\t\tKNIGHT RPG\n");
			printf("---------------------------------------------------------------------------------------------------------\n");
			getch();
		}
	}
}


