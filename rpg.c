#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <time.h>


/*			FILE NAMES			*/
/*------------------------------*/
#define LAUNCHTEXT "LaunchTitle.txt"
/*------------------------------*/

typedef struct Player
{
	int xPosition;
	int yPosition;
	int health;
}Player;

/*			Function Prototypes			*/
/*--------------------------------------*/
void setupScreenSize(void);
void printLaunchScreen(void);
/*--------------------------------------*/


int main (void)
{	
	setupScreenSize();
	system("cls");
	
	printLaunchScreen();

	/*TEST FUNCTION*/
	
	/*END TEST FUNCTION*/
}



/* Sets up the dimensions for CMD prompt. */
void setupScreenSize(void)
{
  	system("MODE 135, 40");
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
			putchar(c);
	fclose(fpt);
	}
}



