/*
Lukas Terhall
11/10/2014

Reads in chat lines from a chat file given by the user and then processes
these in order to move a character around a map (read in from a file given
from by the user). In anarchy mode, the next line will be the button processed
if it is a valid command. In democracy mode, the most common button out of
the next 30 valid button presses is the one that is processed. A comment
that say "anarchy" or "democracy" will process a vote for either mode.
Once the number of votes for a certain mode (they start at 50 each, and can
reach no more than 100) reaches 75, the game processes that button and then
switches modes. Prints the current move, mode, votes, step number, and game
screen or each move to a file named "sim.txt." The shell for this program
(all of the functions and structs) was created by Matt Fontaine.


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines for various maximum sizes
#define MAX_SIZE 500
#define MAX_FILENAME_SIZE 50
#define MAX_CHAT_LINE_SIZE 100

// Defines for display constants
#define DISPLAY_WIDTH 31
#define DISPLAY_HEIGHT 21
#define DISPLAY_CENTER_X DISPLAY_WIDTH/2
#define DISPLAY_CENTER_Y DISPLAY_HEIGHT/2

// An enum that keeps track of a direction
typedef enum
{
   LEFT=0,
   DOWN,
   RIGHT,
   UP,
   NUM_DIRECTIONS
} direction_t;

// An enum that keeps track of types of buttons
// The POWER_BUTTON is what is returned when the
// file has been exhausted of button presses.
typedef enum
{
   LEFT_BUTTON=0,
   UP_BUTTON,
   DOWN_BUTTON,
   RIGHT_BUTTON,
   A_BUTTON,
   B_BUTTON,
   POWER_BUTTON,
   NUM_BUTTONS,
   INVALID_BUTTON
} button_t;

typedef enum
{
   ANARCHY_MODE=0,
   DEMOCRACY_MODE,
   NUM_VOTING_MODES
} voting_mode_t;

// A structure that keeps track of a point on the map.
typedef struct
{
   int x, y;
} point_t;

// A structure that keeps track of the current map information
typedef struct
{
   int width, height;
   char grid[MAX_SIZE][MAX_SIZE];
} map_t;

// A structure that keeps track of the player
typedef struct
{
   point_t loc;
   direction_t dir;
} player_t;

// A structure that keeps track of the switch
typedef struct
{
   point_t door_loc;
   int is_open;
} switch_t;

typedef struct
{
   voting_mode_t mode;
   int democracy_votes;
   int anarchy_votes;
} vote_t;

// A structure to keep track of game state information
typedef struct
{
   map_t map;
   player_t player;

   // Make a switch for each letter in the alphabet
   switch_t switches[27];
   int switchCount;
   vote_t vote;

   FILE * sim_file;
   FILE * chat_file;
} game_t;

// A function to read in the game's map
void read_map(FILE * map_file, game_t * game);


// A function to display the game centered at the character's location
void display_map(FILE * sim_file, game_t * game);


// This function reads a line from the chat file and returns if the end
// of file was reached when it tried to read the chat file. The characters
// are read into the buffer that is passed to this function
int read_chat_line(FILE * chat_file, char * buffer);

// The following function reads the buffer passed and determines if the
// chat line was a button. If the chat line was a vote, it updates the
// vote_t struct with the appropriate vote. Returns INVALID_BUTTON if
// no button was pressed by the line.
button_t process_chat_line(char * buffer, vote_t * vote);

// The following function determines if the string in the buffer is a
// button press and returns the button associated with that string.
// Returns INVALID_BUTTON if the button found was invalid.
button_t get_button(char * buffer);

// A function that displays the given button.
void button_display(FILE * sim_file, button_t button);

// These functions process a vote in favor of either anarchy or democracy mode.
void vote_anarchy(vote_t * vote);
void vote_democracy(vote_t * vote);

// A function that helps display the votes.
void vote_display(FILE * sim_file, vote_t * vote);

// A function that changes the mode if the votes are appropriate for a mode
// change. The function checks the current votes and changes the mode in the
// struct.
void vote_change_mode(FILE * sim_file, vote_t * vote);


// A function that changes the current game state based on the button
// that was pressed.
void process_button_press(game_t * game, button_t button);

int main()
{


    FILE * sim_file=fopen("sim.txt", "w");


    game_t game;
    map_t map;
    player_t player;
    button_t button;
    switch_t switches[27];
    vote_t vote;

    //A variable for keeping track of which
    //number switch the program encounters when
    //reading in the map.
    int switchCount;


    game.switchCount=1;
    game.player.dir=DOWN;
    int screenCount=1;


    int i;


    //Gets the file name of the map file from the user.
    char mapName[MAX_FILENAME_SIZE], chatName[MAX_FILENAME_SIZE];
    printf("\nEnter the file containing the map:\n");
    scanf("%s", &mapName);

    //Opens the map and chat files.
    FILE * map_file=fopen(mapName, "r");


    //Reads the initial map and prints it to the sim
    //file.
    read_map(map_file, &game);
    fprintf(sim_file, "Initial Screen:\n");
    display_map(sim_file, &game);
    fprintf(sim_file, "\n");


    //Until the end of the file has been reached, reads in the
    //commands from the chat file, updates the map, and displays everything
    //accordingly to the sim file.

    do
    {


                char choice = getch();
                system("cls");
                if(choice =='w')
                    button=UP_BUTTON;
                else if(choice=='a')
                    button=LEFT_BUTTON;
                else if(choice=='s')
                    button=DOWN_BUTTON;
                else if(choice=='d')
                    button=RIGHT_BUTTON;
                else if(choice=='e')
                    button=A_BUTTON;

                //Enters the loop if the line contains a valid button press.
                if(button!=INVALID_BUTTON)
                {


                    process_button_press(&game,button);

                    display_map(sim_file, &game);



                }

    }while(1>0);

    fprintf(sim_file, "Game over!\n");


    return 0;
}




void read_map(FILE * map_file, game_t * game)
{
    //Gets the height and width of the map from the map file.
    fscanf(map_file, "%d %d", &game->map.height, &game->map.width);

    //Loops through the map and turns it into a 2D array.
    int i, j;
    for(i=0; i<(game->map.height); i++)
    {
        fscanf(map_file, "%s", &game->map.grid[i]);
    }

    //Finds the starting location of the player, and sets the x and y
    //values in the player location struct to that location. Replaces the
    //@ sign in the array with a . so that it will be treated as an empty space.
    for(i=0; i<(game->map.height); i++)
    {
        for(j=0; j<(game->map.width); j++)
        {
            if(game->map.grid[i][j]=='@')
            {
                game->player.loc.x = j;
                game->player.loc.y = i;
                game->map.grid[i][j]='.';
            }

            //Finds all uppercases letters in the map(gates) and puts them in the switches
            //array, along with their location. Sets them to be closed by default.
            if(isalpha(game->map.grid[i][j]) && (toupper(game->map.grid[i][j])==game->map.grid[i][j]))
            {
                game->switches[game->switchCount].door_loc.x=j;
                game->switches[game->switchCount].door_loc.y=i;
                game->switches[game->switchCount].is_open=0;
                game->switchCount++;

            }
        }
    }
}

void display_map(FILE * sim_file, game_t * game)
{
    int i, j, k, l;

    for(i=-1; i<=DISPLAY_HEIGHT; i++)
    {
        for(j=-1; j<=DISPLAY_WIDTH; j++)
        {
            //Prints the boarder from the map.
            if((i==-1 && j==-1) || (i==-1 && j==DISPLAY_WIDTH) || (i==DISPLAY_HEIGHT && j==-1) || (i==DISPLAY_HEIGHT && j==DISPLAY_WIDTH))
               printf("+");
            else if(i==-1 || i==DISPLAY_HEIGHT)
                printf("-");
            else if(j==-1 || j==DISPLAY_WIDTH)
                printf("|");


            else if (i>=(DISPLAY_CENTER_Y-(game->player.loc.y)) && i<(DISPLAY_CENTER_Y-game->player.loc.y+game->map.height) && j>=(DISPLAY_CENTER_X-game->player.loc.x) && j<(game->map.width+DISPLAY_CENTER_X-game->player.loc.x))
            {

                //Prints the player, depending on which direction he is facing.
                if((i==DISPLAY_CENTER_Y) && (j==DISPLAY_CENTER_X))
                {
                    if(game->player.dir==DOWN)
                        printf("v");
                    else if(game->player.dir==UP)
                        printf("^");
                    else if(game->player.dir==RIGHT)
                        printf(">");
                    else
                        printf("<");

                }

                //If the location is empty(a .) prints a space.
                else if (game->map.grid[i-DISPLAY_CENTER_Y+game->player.loc.y][j-DISPLAY_CENTER_X+game->player.loc.x]=='.')
                    printf(" ");

                //If the location is a gate, prints the gate letter if it is closed. If it is open, prints a space.
                else if(isalpha(game->map.grid[i-DISPLAY_CENTER_Y+game->player.loc.y][j-DISPLAY_CENTER_X+game->player.loc.x]) && (toupper(game->map.grid[i-DISPLAY_CENTER_Y+game->player.loc.y][j-DISPLAY_CENTER_X+game->player.loc.x])==game->map.grid[i-DISPLAY_CENTER_Y+game->player.loc.y][j-DISPLAY_CENTER_X+game->player.loc.x]))
                {
                    int k;
                    for(k=1; k<=26; k++)
                    {
                        if (game->switches[k].door_loc.y==(i-DISPLAY_CENTER_Y+game->player.loc.y) && game->switches[k].door_loc.x==(j-DISPLAY_CENTER_X+game->player.loc.x))
                        {

                            if(game->switches[k].is_open==1)
                                printf(" ");
                            else
                                printf("%c", game->map.grid[i-DISPLAY_CENTER_Y+game->player.loc.y][j-DISPLAY_CENTER_X+game->player.loc.x]);

                        }
                    }

                }

                //If the location is not the player, a gate, or empty, prints whatever is in the 2D array at that point (a fence, switch, etc.)
                else
                    printf("%c", game->map.grid[i-DISPLAY_CENTER_Y+game->player.loc.y][j-DISPLAY_CENTER_X+game->player.loc.x]);
            }

            //Prints a # everywhere else on the display screen where there is not a map.
            else
                printf("#");
        }

    //Creates a new line after the display.
    printf("\n");


    }
}


button_t get_button(char * buffer)
{
   // Given a string, extract what the string means as a button_t variable.
   if (strcmp(buffer, "left") == 0)
      return LEFT_BUTTON;
   if (strcmp(buffer, "up") == 0)
      return UP_BUTTON;
   if (strcmp(buffer, "down") == 0)
      return DOWN_BUTTON;
   if (strcmp(buffer, "right") == 0)
      return RIGHT_BUTTON;
   if (strcmp(buffer, "a") == 0)
      return A_BUTTON;
   if (strcmp(buffer, "b") == 0)
      return B_BUTTON;
   return INVALID_BUTTON;
}






void process_button_press(game_t * game, button_t button)
{
    //Processes the up button.
    if(button==UP_BUTTON)
    {
        //Checks to see if the space above the player is open. If it is, moves
        //the player up one and faces him up.
        if ((game->map.grid[game->player.loc.y-1][game->player.loc.x] == '.'))
        {
            game->player.dir=UP;
            game->player.loc.y--;

        }

        //Checks to see if the space above the player is a gate. If it is, and the gate
        //is open, the player moves as if it were an open space. If the gate is closed,
        //the player does not move, but still faces up.
        else if (isalpha(game->map.grid[game->player.loc.y-1][game->player.loc.x]) && toupper(game->map.grid[game->player.loc.y-1][game->player.loc.x])==(game->map.grid[game->player.loc.y-1][game->player.loc.x]))
        {
            //Find the correct gate at the location, and checks if it's open.
            int k;
            for(k=1; k<=26; k++)
            {
                if (game->switches[k].door_loc.y==(game->player.loc.y-1) && game->switches[k].door_loc.x==(game->player.loc.x))
                {

                    if(game->switches[k].is_open==1)
                    {
                        game->player.dir=UP;
                        game->player.loc.y--;
                    }
                    else
                        game->player.dir=UP;

                }
            }
        }

        //If the player cannot move, simply faces him up instead.
        else
            game->player.dir=UP;

    }

    //Behaves the same as the up button, but checks the space to the player's
    //left and acts as one would expect.
    if(button==LEFT_BUTTON)
    {

        if ((game->map.grid[game->player.loc.y][game->player.loc.x-1] == '.'))
        {
            game->player.dir=LEFT;
            game->player.loc.x--;

        }

        else if (isalpha(game->map.grid[game->player.loc.y][game->player.loc.x-1]) && toupper(game->map.grid[game->player.loc.y][game->player.loc.x-1])==(game->map.grid[game->player.loc.y][game->player.loc.x-1]))
        {
            int k;
            for(k=1; k<=26; k++)
            {
                if (game->switches[k].door_loc.y==(game->player.loc.y) && game->switches[k].door_loc.x==(game->player.loc.x-1))
                {

                    if(game->switches[k].is_open==1)
                    {
                        game->player.dir=LEFT;
                        game->player.loc.x--;
                    }
                    else
                        game->player.dir=LEFT;

                }
            }
        }

        else
            game->player.dir=LEFT;

    }

    //Acts the same as the previous two buttons, but also accounts for fences.
    if(button==DOWN_BUTTON)
    {

        if ((game->map.grid[game->player.loc.y+1][game->player.loc.x] == '.'))
        {
            game->player.dir=DOWN;
            game->player.loc.y++;

        }

        //If the space below the fence is open, the player moves to that space.
        //If it is not, the player simply faces down.
        else if ((game->map.grid[game->player.loc.y+1][game->player.loc.x] == '='))
        {
            if(game->map.grid[game->player.loc.y+2][game->player.loc.x] == '.')
            {
                game->player.dir=DOWN;
                game->player.loc.y+=2;
            }

            else if(isalpha(game->map.grid[game->player.loc.y+2][game->player.loc.x]) && toupper(game->map.grid[game->player.loc.y+2][game->player.loc.x])==(game->map.grid[game->player.loc.y+2][game->player.loc.x]))
            {
                int k;
                for(k=1; k<=26; k++)
                {
                    if (game->switches[k].door_loc.y==(game->player.loc.y+2) && game->switches[k].door_loc.x==(game->player.loc.x))
                    {
                        if(game->switches[k].is_open==1)
                        {
                            game->player.dir=DOWN;
                            game->player.loc.y+=2;
                        }
                        else
                            game->player.dir=DOWN;

                    }
                }
            }

            else
                game->player.dir=DOWN;
        }
        else if (isalpha(game->map.grid[game->player.loc.y+1][game->player.loc.x]) && toupper(game->map.grid[game->player.loc.y+1][game->player.loc.x])==(game->map.grid[game->player.loc.y+1][game->player.loc.x]))
        {
            int k;
            for(k=1; k<=26; k++)
            {
                if (game->switches[k].door_loc.y==(game->player.loc.y+1) && game->switches[k].door_loc.x==(game->player.loc.x))
                {

                    if(game->switches[k].is_open==1)
                    {
                        game->player.dir=DOWN;
                        game->player.loc.y++;
                    }

                    else
                        game->player.dir=DOWN;

                }
            }
        }

        else
            game->player.dir=DOWN;

    }

    //Behave the same as up and left buttons, but for the space to the right
    //of the player.
    if(button==RIGHT_BUTTON)
    {

        if ((game->map.grid[game->player.loc.y][game->player.loc.x+1] == '.'))
        {
            game->player.dir=RIGHT;
            game->player.loc.x++;

        }

        else if (isalpha(game->map.grid[game->player.loc.y][game->player.loc.x+1]) && toupper(game->map.grid[game->player.loc.y][game->player.loc.x+1])==(game->map.grid[game->player.loc.y][game->player.loc.x+1]))
        {
            int k;
            for(k=1; k<=26; k++)
            {
                if (game->switches[k].door_loc.y==(game->player.loc.y) && game->switches[k].door_loc.x==(game->player.loc.x+1))
                {

                    if(game->switches[k].is_open==1)
                    {
                        game->player.dir=RIGHT;
                        game->player.loc.x++;
                    }

                    else
                        game->player.dir=RIGHT;

                }
            }
        }

        else
            game->player.dir=RIGHT;

    }

    //Checks the space in front of the player. If the space is a switch, changes the
    //value of the switch's gate from either open to closed or closed to open.
    if(button==A_BUTTON)
    {
        //Works if the player is facing right.
        if (game->player.dir==RIGHT)
        {
            if (isalpha(game->map.grid[game->player.loc.y][game->player.loc.x+1]) && (toupper(game->map.grid[game->player.loc.y][game->player.loc.x+1]) != game->map.grid[game->player.loc.y][game->player.loc.x+1]))
            {
                //Searches through the map to find the location of the uppercase version of the switch.
                //Uses that location to find the switch value in the switches array, and then changes the
                //value of the switch from either open to closed or closed to open.
                char tmp;
                tmp = toupper(game->map.grid[game->player.loc.y][game->player.loc.x+1]);
                int i, j;
                for(i=0; i<(game->map.height); i++)
                {
                    for(j=0; j<(game->map.width); j++)
                    {
                        if(game->map.grid[i][j]==tmp)
                        {
                            int k;
                            for(k=1; k<=26; k++)
                            {
                                if (game->switches[k].door_loc.x==j && game->switches[k].door_loc.y==i)
                                {

                                    if(game->switches[k].is_open==0)
                                        game->switches[k].is_open=1;
                                    else
                                        game->switches[k].is_open=0;

                                }
                            }
                        }
                    }
                }
            }
        }

        //Behaves the same as previous, but if the player is facing left.
        if (game->player.dir==LEFT)
        {
            if (isalpha(game->map.grid[game->player.loc.y][game->player.loc.x-1]) && (toupper(game->map.grid[game->player.loc.y][game->player.loc.x-1]) != game->map.grid[game->player.loc.y][game->player.loc.x-1]))
            {
                char tmp;
                tmp = toupper(game->map.grid[game->player.loc.y][game->player.loc.x-1]);

                int i, j;
                for(i=0; i<(game->map.height); i++)
                {
                    for(j=0; j<(game->map.width); j++)
                    {
                        if(game->map.grid[i][j]==tmp)
                        {
                            int k;
                            for(k=1; k<=26; k++)
                            {

                                if (game->switches[k].door_loc.x==j && game->switches[k].door_loc.y==i)
                                {

                                    if(game->switches[k].is_open==0)
                                        game->switches[k].is_open=1;
                                    else
                                        game->switches[k].is_open=0;

                                }
                            }
                        }
                    }
                }
            }
        }

        //Behaves the same as previous, but if the player is facing up.
        if (game->player.dir==UP)
        {
            if (isalpha(game->map.grid[game->player.loc.y-1][game->player.loc.x]) && (toupper(game->map.grid[game->player.loc.y-1][game->player.loc.x]) != game->map.grid[game->player.loc.y-1][game->player.loc.x]))
            {
                char tmp;
                tmp = toupper(game->map.grid[game->player.loc.y-1][game->player.loc.x]);
                int i, j;
                for(i=0; i<(game->map.height); i++)
                {
                    for(j=0; j<(game->map.width); j++)
                    {
                        if(game->map.grid[i][j]==tmp)
                        {
                            int k;
                            for(k=1; k<=26; k++)
                            {
                                if (game->switches[k].door_loc.x==j && game->switches[k].door_loc.y==i)
                                {

                                    if(game->switches[k].is_open==0)
                                        game->switches[k].is_open=1;
                                    else
                                        game->switches[k].is_open=0;

                                }
                            }
                        }
                    }
                }
            }
        }

        //Behaves the same as previous, but if the player is facing down.
        if (game->player.dir==DOWN)
        {
            if (isalpha(game->map.grid[game->player.loc.y+1][game->player.loc.x]) && (toupper(game->map.grid[game->player.loc.y+1][game->player.loc.x]) != game->map.grid[game->player.loc.y+1][game->player.loc.x]))
            {
                char tmp;
                tmp = toupper(game->map.grid[game->player.loc.y+1][game->player.loc.x]);
                int i, j;
                for(i=0; i<(game->map.height); i++)
                {
                    for(j=0; j<(game->map.width); j++)
                    {
                        if(game->map.grid[i][j]==tmp)
                        {
                            int k;
                            for(k=1; k<=26; k++)
                            {
                                if (game->switches[k].door_loc.x==j && game->switches[k].door_loc.y==i)
                                {

                                    if(game->switches[k].is_open==0)
                                        game->switches[k].is_open=1;
                                    else
                                        game->switches[k].is_open=0;

                                }
                            }
                        }
                    }
                }
            }
        }
    }


}

