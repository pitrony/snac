#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define MAX_X 30
#define MAX_Y 30
typedef struct tail_t{
	int x;
	int y;
	} tail_t;
	
typedef struct snake_t{
	int x;
	int y;
	int direction;
	int preddirection;
	struct control_buttons* controls;
	struct tail_t * tail;
	size_t tsize;
	}  snake_t;
	
	struct control_buttons
{
 int down;
 int up;
 int left;
 int right;
}control_buttons;
	
//enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_F(10), CONTROLS=3};
enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME='Q', CONTROLS=3};
//struct control_buttons default_controls[CONTROLS]= {{KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT},
//{'s','w','a','d'} , {'S','W','A','D'}};	
struct control_buttons default_controls[CONTROLS]={{'s','w','a','d'} , {'S','W','A','D'}}; 

// --> x
// || Y
// \/
// @**
struct snake_t initSnake(int x, int y, size_t tsize){
	struct snake_t snake;
	snake.x = x;
	snake.y = y;
	snake.tsize = tsize;
	snake.tail = (tail_t *) malloc (sizeof(tail_t) * 100);
	for (int i =0; i < tsize; ++i){
		snake.tail[i].x = x + i +1;
		snake.tail[i].y = y;
		}
		snake.preddirection=LEFT;
		snake.direction=LEFT;
	return snake;
}

	
// @**
void printSnake(struct snake_t snake){
		char matrix[MAX_X][MAX_Y];
		for (int i = 0; i < MAX_X; ++i){
			for (int j = 0; j < MAX_Y; ++j)
			{
				matrix[i][j] = ' ';
				}}
		
		matrix[snake.x][snake.y] = '@';
		for (int i = 0; i < snake.tsize; ++i){
			matrix[snake.tail[i].x][snake.tail[i].y] = '*';
			}
		
		for (int j = 0; j < MAX_Y; ++j){
			for (int i = 0; i < MAX_X; ++i)
			{
				printf("%c", matrix[i][j]);
				}
				printf("\n");
				}
	}
	
// <--  @** tsize = 2
//     @**

//  @**      @***
//    * <--     *
//  ***        **


snake_t moveDir(snake_t snake, int32_t dir){
	for (int i = snake.tsize - 1; i > 0; i--)
		{
		snake.tail[i] = snake.tail[i-1];
		}
	snake.tail[0].x = snake.x;
	snake.tail[0].y = snake.y;
	
	switch (dir)
	{
	case RIGHT: //right
	snake.x = snake.x + 1;	
	if (snake.x >MAX_X-1){
		snake.x =  1;
		}
	return snake;
	break;
	case LEFT: //left
	snake.x = snake.x - 1;	
	if (snake.x < 0)
		{
		snake.x = MAX_X - 1;
		}
	return snake;
	break;
	case UP: //up
	snake.y = snake.y - 1;	
	if (snake.y < 0){
		snake.y = MAX_Y - 1;
		}
	return snake;
	break;
case DOWN: //down
	snake.y = snake.y + 1;	
	if (snake.y >MAX_Y-1){
		snake.y =  1;
		}
	return snake;
	break;
	}
	return snake;
}	
		


void chageDirection(struct snake_t* snake, const int32_t key)
{
for(int i=0; i<CONTROLS; i++)
	{
	
	if(key==DOWN)//if(key==snake->controls[i].down)
		{
			snake->preddirection=snake->direction;
			snake->direction=DOWN;
		}
	else if(key==UP)//(key==snake->controls[i].up)
		{
			snake->preddirection=snake->direction;
			snake->direction=UP;
			}
	else if(key==RIGHT)//(key==snake->controls[i].right)
		{
			snake->preddirection=snake->direction;
			snake->direction=RIGHT;
			}
	else if(key==LEFT)//(key==snake->controls[i].left)
		{
			snake->preddirection=snake->direction;
			snake->direction=LEFT;
			}
	}
}
int checkDirection(snake_t* snake, const int32_t key)
{
//for(int i=0; 1<CONTROLS; i++)
//if((snake->controls[i].down==key && snake->direction==UP) || (snake->controls[i].up==key && snake->direction==DOWN)||
//(snake->controls[i].left==key && snake->direction==LEFT) || (snake->controls[i].right==key && snake->direction==RIGHT))
if ((snake->preddirection==LEFT && key!=RIGHT) || (snake->preddirection==RIGHT && key!=LEFT) ||
(snake->preddirection==DOWN && key!=UP) || (snake->preddirection==UP && key!=DOWN))
	{
	return 1;
	}
return 0;
}
	
int main(){
	
setlocale(LC_CTYPE, "");
	struct snake_t snake = initSnake( 10, 5, 2);
	printSnake(snake);
	int32_t key;
	
	while(1)
	{
		 if(_kbhit())
		 {
        char tmp=(_getch());    
		switch (tmp) 
			{
		case -28: // ф
		case -108:// Ф
		case 'a'://97 
		case 'A'://65 
		key=LEFT;
		if(checkDirection(&snake, key))
		{chageDirection(&snake, key);
		snake = moveDir(snake,key);}
		break;	
		case -21://ы
		case -101: //Ы
		case 's'://115 's'
		case 'S'://83 'S'
		key=DOWN;
		if(checkDirection(&snake, key))
		{chageDirection(&snake, key);
		snake = moveDir(snake,key);}
		break;	
		case -94: //в
		case -126://В
		case 'd': //100 'd'
		case 'D'://68 'D'
		key=RIGHT;
		if(checkDirection(&snake, key))
		{chageDirection(&snake, key);
		snake = moveDir(snake,key);}
		break;	
		case -26://ц
		case -106: //Ц
		case 'w'://119 'w'
		case 'W'://87 'W'
		key=UP;
		if(checkDirection(&snake, key))
		{chageDirection(&snake, key);
		snake = moveDir(snake,key);}
		break;	
		case -87: //й
		case -119://Й
		case 'Q'://81 'Q'
		case 'q'://113 'q'
		printf("Bay!");
		return 0;
		break;
		
		//case -87: //й
		//case -119://Й
		case 'P':// 'P'
		case 'p':// 'p'
		printf("Game in PAUSE\n\n");
		system("pause");
		break;
		
		default:
		printf("w a s d - for control Q - for exit");
		break;	
			}
		}
		else snake = moveDir(snake,key=snake.preddirection);
		sleep(1);
		system("cls");
		printSnake(snake);
	}
	return 0;
	}
