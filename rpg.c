/*
Armand Alvarez & Luke Terhall
Project Started 8/11/2016

KnightRPG.c
*/



/*					CURRENTLY IN PROGRESS				*/
/*------------------------------------------------------*/
int mainMenu(void);
/*------------------------------------------------------*/


/*					Function Prototypes					*/
/*	ONLY PUT FUNCTIONS THAT ARE NOT IN PROGRESS HERE	*/
/*------------------------------------------------------*/
void setUpGame(void);
void printLaunchScreen(void);
void displayFileText(char* fileName, int buffer);
void setupScreenSize(void);
void printHeader(int style);
/*------------------------------------------------------*/



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
#define MAINMENUTEXT "MainMenu.txt"
#define CREDITSTEXT "credits.txt"
#define QUITTEXT "QuitText.txt"
/*------------------------------*/

#define MAXNAMELENGTH 12
#define TEXTSPEED 13500

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
	
	if (!mainMenu())
		return 0;

	/*TEST FUNCTION*/
	
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



/* 	This function displays all the text in a text file.
	Buffer is the time delay for each character.  */
void displayFileText(char* fileName, int buffer)
{
	FILE *fpt = fopen(fileName, "r");
	char c;
	
	if (fpt)
	{
		while ((c=getc(fpt))!=EOF)
		{
			putchar(c);
			usleep(buffer);
		}
		fclose(fpt);
	}
}



/* Prints the launch screen in ASCII art. */
void printLaunchScreen(void)
{
	displayFileText(LAUNCHTEXT, 6000);
	getch();
	
}



/* 	Opens Main Menu. 
	Includes Start Game.
	Includes Credits.
	Includes Quit option. */
int mainMenu(void)
{
	int option;
	int breaker = 1;
	
	while (breaker)
	{
		printHeader(1);
		
		displayFileText(MAINMENUTEXT, TEXTSPEED);
		scanf("%d", &option);
		
		switch(option)
		{
			case 1:
			{
				system("cls");
				printf("WARNING: NO GAME BEGIN SEQUENCE YET");
				getch();
				/* NO GAME BEGIN SEQUENCE YET */
				return 1;
			}
			case 2:
			{
				system("cls");
				displayFileText(CREDITSTEXT, TEXTSPEED);
				getch();
				break;
			}
			case 3:
			{
				system("cls");
				displayFileText(QUITTEXT, TEXTSPEED);
				getch();
				return 0;
			}
			default:
			{
				system("cls");
				printf("Please input valid option.\n\n");
				system("pause");		
			}	
			
		}
	}
	
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
	}
	
	else if (style==2)
	{
		/* Use this failsafe if the character name has not been created yet. */
		if(PLAYER.name=="")
		{
			printf("\t\t\t\t\t\tKNIGHT RPG\n");
			printf("---------------------------------------------------------------------------------------------------------\n");
		}
	}
}


