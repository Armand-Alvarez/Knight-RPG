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
	
	/*TEST FUNCTION*/
	printLaunchScreen();
	/*END TEST FUNCTION*/
}



/* Sets up the dimensions for CMD prompt. */
void setupScreenSize(void)
{
	system("mode 120,40");   //Set mode to ensure window does not exceed buffer size
  	SMALL_RECT WinRect = {0, 0, 80, 25};   //New dimensions for window in 8x12 pixel chars
  	SMALL_RECT* WinSize = &WinRect;
  	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), 1, WinSize);   //Set new size for window

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



