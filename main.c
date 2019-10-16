#include <stdio.h>
#include <stdlib.h>

#define GRID_W	(40)
#define GRID_H	(25)

enum
{
	PLAYER,
	ROBOT
} E_TYPE;

typedef struct
{
	int x;
	int y;
	int alive;

} entity;

entity player;
entity cursor;
entity* robots;
int robotCount;
int aliveCount;
int input;

void clearScreen()
{
	printf("\033[2J");
}

void drawCharacter(int x, int y, char c)
{
	printf("\033[%i;%iH%c", y, x, c);
	cursor.x += (x - cursor.x);
	cursor.y += (y - cursor.y);
}

void moveCursor(int x, int y)
{
	printf("\033[%i;%iH", y, x);
}

void setRandom(entity* e)
{
	e->x = (GRID_W * random() / RAND_MAX);
	e->y = (GRID_H * random() / RAND_MAX);
}

void move(entity* e, int x, int y)
{
	e->x += x;
	e->y += y;

	if(e->x < 1) e->x = 1;
	if(e->y < 1) e->y = 1;
	if(e->x > GRID_W) e->x = GRID_W;
	if(e->y > GRID_H) e->y = GRID_H;
}

void initGame()
{
	printf("Initialising with %i robots\n", robotCount);

	player.x = (GRID_W * random() / RAND_MAX);
	player.y = (GRID_H * random() / RAND_MAX);
	player.alive = 1;

	cursor.x = cursor.y = 0;

	robots = calloc(robotCount, sizeof(entity));

	for(int i = 0; i < robotCount; i++)
	{
		setRandom(&robots[i]);
		robots[i].alive = 1;
	}

	aliveCount = robotCount;
}

void update()
{
	for(int i = 0; i < robotCount; i++)
	{
		if(robots[i].alive)
		{
			move(&robots[i], robots[i].x > player.x ? -1 : (robots[i].x < player.x  ? 1 : 0),
					robots[i].y > player.y ? -1 : (robots[i].y < player.y  ? 1 : 0));
		}
	}

	for(int i = 0; i < robotCount; i++)
	{
		if(robots[i].alive)
		{
			for(int j = 0; j < robotCount; j++)
			{
				if(i == j)
					continue;

				if(robots[i].x == robots[j].x && robots[i].y == robots[j].y)
				{
					if(robots[i].alive)
					{
						robots[i].alive = 0;
						aliveCount --;
					}

					if(robots[j].alive)
					{
						robots[j].alive = 0;
						aliveCount --;
					}
				}
			}
		}

		if(robots[i].x == player.x && robots[i].y == player.y)
		{
			player.alive = 0;
		}
	}
}

void drawMap()
{
	clearScreen();
	moveCursor(0, 0);

	for(int i = 0; i < GRID_H; i++)
	{
		for(int j = 0; j < GRID_W; j++)
		{
			putchar('.');
		}

		printf("\n");
	}

	for(int  k = 0; k < robotCount; k++)
	{
		drawCharacter(robots[k].x, robots[k].y, robots[k].alive ? '*' : '#');
	}

	drawCharacter(player.x, player.y, player.alive ? '@' : 'X');
}

void cleanUp()
{
	free(robots);
	printf("Thanks for playing!\n\n");
}

int main(int argc, char **argv)
{
	char message[255];
	robotCount = 10;

	if(argc > 1)
	{
		sscanf(argv[1], "%i", &robotCount);
	}

	initGame();
	clearScreen();
	drawMap();
	moveCursor(0, 26);
	
	while((char)input != 'q')
	{
		int doUpdate = 1;
		input = getchar();

		switch((char)input)
		{
			case 'h': move(&player, -1, 0); break;
			case 'j': move(&player, 0, 1); break;
			case 'k': move(&player, 0, -1); break;
			case 'l': move(&player, 1, 0); break;
			case 'u': move(&player, -1, -1); break;
			case 'i': move(&player, 1, -1); break;
			case 'n': move(&player, -1, 1); break;
			case 'm': move(&player, 1, 1); break;
			case 't': setRandom(&player); sprintf(message, "Teleport!"); break;
			default: doUpdate = 0;
		}

		if(doUpdate)
		{
			update();
			drawMap();
			moveCursor(0, 26);
		}

		printf("%s", message);
		sprintf(message, "\n");

		if(aliveCount == 0)
		{
			printf("You won!\n");
			break;
		}

		if(player.alive == 0)
		{
			printf("You died a horrible death :(\n");
			break;
		}
	}
	
	cleanUp();

	return 0;
}

