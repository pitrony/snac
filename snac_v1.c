#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>
#include <windows.h>

#define MAX_X 30
#define MAX_Y 30
#define MAX_FOOD_SIZE 20
double DELAY=0.1;
#define MAX_TAIL_SIZE 10
#define FOOD_EXPIRE_SECONDS 10
#define SEED_NUMBER 1
typedef struct tail_t{
	int x;
	int y;
	} tail_t;
	
typedef struct snake_t{
	int x;
	int y;
	int direction;
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

struct food
{
int x;
int y;
time_t put_time;
char point;
uint8_t enable;
} food[MAX_FOOD_SIZE];
	
enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME='Q', CONTROLS=3};
struct control_buttons default_controls[CONTROLS]={{'s','w','a','d'} , {'S','W','A','D'}}; 

struct colors {
int color_snake;
int color_snake2;
int color_level;
int color_food;
int color_message;
};
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
		//snake.preddirection=LEFT;
		snake.direction=LEFT;
	return snake;
}
struct snake_t initSnake2(int x, int y, size_t tsize){
	struct snake_t snake2;
	snake2.x = x;
	snake2.y = y;
	snake2.tsize = tsize;
	snake2.tail = (tail_t *) malloc (sizeof(tail_t) * 100);
	for (int i =0; i < tsize; ++i){
		snake2.tail[i].x = x + i +1;
		snake2.tail[i].y = y;
		}
		//snake.preddirection=LEFT;
		snake2.direction=RIGHT;
	return snake2;
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
	
void printSnake2(struct snake_t snake2){
		char matrix2[MAX_X][MAX_Y];

		for (int i = 0; i < MAX_X; ++i){
			for (int j = 0; j < MAX_Y; ++j)
			{
				matrix2[i][j] = ' ';
				}}
		
		matrix2[snake2.x][snake2.y] = '#';

		for (int i = 0; i < snake2.tsize; ++i){
			matrix2[snake2.tail[i].x][snake2.tail[i].y] = '+';
			}	
		
		for (int j = 0; j < MAX_Y; ++j){
			for (int i = 0; i < MAX_X; ++i)
			{
				printf("%c", matrix2[i][j]);
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

snake_t moveDir2(snake_t snake2, int32_t dir){
	for (int i = snake2.tsize - 1; i > 0; i--)
		{
		snake2.tail[i] = snake2.tail[i-1];
		}
	snake2.tail[0].x = snake2.x;
	snake2.tail[0].y = snake2.y;
	
	switch (dir)
	{
	case RIGHT: //right
	snake2.x = snake2.x + 1;	
	if (snake2.x >MAX_X-1){
		snake2.x =  1;
		}
	return snake2;
	break;
	case LEFT: //left
	snake2.x = snake2.x - 1;	
	if (snake2.x < 0)
		{
		snake2.x = MAX_X - 1;
		}
	return snake2;
	break;
	case UP: //up
	snake2.y = snake2.y - 1;	
	if (snake2.y < 0){
		snake2.y = MAX_Y - 1;
		}
	return snake2;
	break;
case DOWN: //down
	snake2.y = snake2.y + 1;	
	if (snake2.y >MAX_Y-1){
		snake2.y =  1;
		}
	return snake2;
	break;
	}
	return snake2;
}	

void initFood(struct food f[], size_t size)
{
	struct food init={0,0,0,0,0};
	for(size_t i=0; i<size; i++)
	{
	f[i]=init;
	}
}		
void gotoxy(int x,int y)
{
    HANDLE OutputHandle;
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufInfo;
    OutputHandle=GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(OutputHandle,&ScreenBufInfo);
    ScreenBufInfo.dwCursorPosition.X=x;
    ScreenBufInfo.dwCursorPosition.Y=y;
    SetConsoleCursorPosition(OutputHandle,ScreenBufInfo.dwCursorPosition);
}

void putFoodSeed(struct food *fp)
{
char spoint[2];
gotoxy(fp->y, fp->x);
printf(" ");
fp->x=rand()%(MAX_X-1);
fp->y=rand()%(MAX_Y-2)+1;
fp->put_time = time(0);
fp->point='$';
fp->enable=1;
spoint[1]=fp->point;
gotoxy(fp->y, fp->x);
printf("%c", spoint[1]);
}
void printFood(struct food *fp)
{
if(fp->enable==1)
{gotoxy(fp->y, fp->x);
printf("%c",fp->point);}
else {gotoxy(fp->y, fp->x); 
	printf(" "); }
}


void putFood(struct food f[], size_t number_seeds)
{
for(size_t i=0; i<number_seeds; i++)
	{
	putFoodSeed(&f[i]);
	}	
	
}


_Bool haveEat(struct snake_t *head, struct food f[])
{
	for(size_t i=0; i<MAX_FOOD_SIZE; i++)
	if(f[i].enable && head->x==f[i].x && head->y==f[i].y)
	{
		f[i].enable=0;
		return 1;
		}
		return 0;
}
void addTail(struct snake_t *head)
{
if(head==NULL||head->tsize>MAX_TAIL_SIZE)
{printf("can't add tail");

	return;
	}
	head->tsize++;
}

void refreshFood(struct food f[], int nfood)//обновление еды
{ for(size_t i=0; i<nfood; i++)
	{
	if(f[i].put_time!=0)
		{
			if( !(f[i].enable) || ((time(NULL) - f[i].put_time) >= FOOD_EXPIRE_SECONDS))
			{
			putFoodSeed(&f[i]);
			}
			//else printFood(&f[i]);printf(" if enabl");
		}	
	}
	
}
void increaseSpeed(struct snake_t* head) {
    // delay  snake grows set minimum delay
    if (head->tsize > 2 && DELAY > 0.01) {
        DELAY = 0.1 - (head->tsize * 0.009); // Speed up as size increases
    }
}
void printLevel(struct snake_t *head)
{
 
gotoxy(0,MAX_X+2);//mvprintw(0, 10, "LEVEL: %d", head->tsize);
printf("LEVEL: %lld", head->tsize);
}

void printExit(struct snake_t *head)
{
gotoxy(MAX_Y /2, MAX_X /2 - 3);
printf("\nYour LEVEL is %lld", head->tsize);
system("pause");
system("cls");
}

// Adjust delay after eating food
void delay(struct snake_t* head) {
    if (haveEat(head, food)) {
        addTail(head);
        printLevel(head);
        increaseSpeed(head); // Adjust speed based on new size
    }
    usleep(DELAY * 1000000); // Adjusted delay for each frame
}

void chageDirection(struct snake_t* snake, const int32_t key)
{
for(int i=0; i<CONTROLS; i++)
	{
	
	if(key==DOWN)//if(key==snake->controls[i].down)
		{
			//snake->preddirection=snake->direction;
			snake->direction=DOWN;
		}
	else if(key==UP)//(key==snake->controls[i].up)
		{
			//snake->preddirection=snake->direction;
			snake->direction=UP;
			}
	else if(key==RIGHT)//(key==snake->controls[i].right)
		{
			//snake->preddirection=snake->direction;
			snake->direction=RIGHT;
			}
	else if(key==LEFT)//(key==snake->controls[i].left)
		{
			//snake->preddirection=snake->direction;
			snake->direction=LEFT;
			}
	}
}
void chageDirection2(struct snake_t* snake2, const int32_t key)
{
for(int i=0; i<CONTROLS; i++)
	{
	
	if(key==DOWN)//if(key==snake->controls[i].down)
		{
			//snake->preddirection=snake->direction;
			snake2->direction=DOWN;
		}
	else if(key==UP)//(key==snake->controls[i].up)
		{
			//snake->preddirection=snake->direction;
			snake2->direction=UP;
			}
	else if(key==RIGHT)//(key==snake->controls[i].right)
		{
			//snake->preddirection=snake->direction;
			snake2->direction=RIGHT;
			}
	else if(key==LEFT)//(key==snake->controls[i].left)
		{
			//snake->preddirection=snake->direction;
			snake2->direction=LEFT;
			}
	}
}




int checkDirection(snake_t* snake, const int32_t key)
{
if ((snake->direction==LEFT && key!=RIGHT) || (snake->direction==RIGHT && key!=LEFT) ||
(snake->direction==DOWN && key!=UP) || (snake->direction==UP && key!=DOWN))
	{
	return 1;
	}
return 0;
}
int checkDirection2(snake_t* snake2, const int32_t key)
{
if ((snake2->direction==LEFT && key!=RIGHT) || (snake2->direction==RIGHT && key!=LEFT) ||
(snake2->direction==DOWN && key!=UP) || (snake2->direction==UP && key!=DOWN))
	{
	return 1;
	}
return 0;
}


//void setColor(int objectType){}
struct colors startMenu(struct colors *now_colors){
	int ex=9;
	int color_1=9;
	int color_2=6;
	int color_3=4; 
	int color_4=5;
	int color_5=7;
	printf("********************************************\n");
	printf("*********** Game snake *********************\n");
	while(ex!=5)
	{
	printf("\nGame - 5\n");
	printf("\nChange colors - 6\n");
	scanf("%d", &ex);
	if(ex==6){
	printf("\nchoice for snake1 - 1\n");
	printf("choice for snake2 - 2\n");
	printf("choice for food - 3\n");
	printf("choice for text - 4\n");
	printf("choice for exit - 0\n");
	scanf("%d", &ex);
	switch (ex)
	{
	case 1:
	printf("Choice color for snake1 1-15: ");
	scanf("%d", &color_1);
	break;
	case 2:
	printf("\nChoice color for snske2 1-15: ");
	scanf("%d", &color_2);
	break;
	case 3:
	printf("\nChoice color for food 1-15: ");
	scanf("%d", &color_3);
	break;
	case 4:
	printf("\nChoice color for level 1-15: ");
	scanf("%d", &color_4);
	break;
	case 5:
	printf("\nChoice color for text 1-15: ");
	scanf("%d", &color_5);
	break;
	case 0:
	break ;
	default:
	printf("choice 1-5 or 0 \n");
	break;
	}	//switch
			}//if
		}//while
	
	now_colors->color_snake = color_1;
	now_colors->color_snake2= color_2;
	now_colors->color_food= color_3;
	now_colors->color_level= color_4;
	now_colors->color_message= color_5;
	return *now_colors;
}

int main(){

setlocale(LC_CTYPE, "");
	struct snake_t snake = initSnake( 10, 5, 2);
	struct snake_t snake2 = initSnake( 2, 2, 2);

	HANDLE  hConsole;
    struct colors now_colors = {9,6,4,5,7};
    startMenu(&now_colors);
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, now_colors.color_snake);
	printSnake(snake);
	SetConsoleTextAttribute(hConsole, now_colors.color_snake2);
	printSnake2(snake2);
	int32_t key, key2;
	struct food f[MAX_FOOD_SIZE]; //food  f;
	initFood(f, MAX_FOOD_SIZE);
	SetConsoleTextAttribute(hConsole,now_colors.color_food);
	putFood(food, SEED_NUMBER);
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
		{chageDirection(&snake, key);}
		
		break;	
		case -21://ы
		case -101: //Ы
		case 's'://115 's'
		case 'S'://83 'S'
		key=DOWN;
		if(checkDirection(&snake, key))
		{chageDirection(&snake, key);}
		
		break;	
		case -94: //в
		case -126://В
		case 'd': //100 'd'
		case 'D'://68 'D'
		key=RIGHT;
		if(checkDirection(&snake, key))
		{chageDirection(&snake, key);}
		
		break;	
		case -26://ц
		case -106: //Ц
		case 'w'://119 'w'
		case 'W'://87 'W'
		key=UP;
		if(checkDirection(&snake, key))
		{chageDirection(&snake, key);}
		break;	
		//snake2 ijkl
		case 'j': 
		case 'J': 
		key2=LEFT;
		if(checkDirection2(&snake2, key2))
		{chageDirection2(&snake2, key2);}
		break;	
		
		case 'k':
		case 'K':
		key2=DOWN;
		if(checkDirection2(&snake2, key2))
		{chageDirection2(&snake2, key2);}
		break;	
		
		case 'l': 
		case 'L':
		key2=RIGHT;
		if(checkDirection2(&snake2, key2))
		{chageDirection2(&snake2, key2);}
		break;	
		
		case 'i':
		case 'I':
		key2=UP;
		if(checkDirection2(&snake2, key2))
		{chageDirection2(&snake2, key2);}
		break;	
		
		case -87: //й
		case -119://Й
		case 'Q'://81 'Q'
		case 'q'://113 'q'
		SetConsoleTextAttribute(hConsole, now_colors.color_message);
		printf("\nBay!");
		return 0;
		break;
		case 'P':// 'P'
		case 'p':// 'p'
		SetConsoleTextAttribute(hConsole, now_colors.color_message);
		printf("\nGame in PAUSE\n\n");
		system("pause");
		break;
		
		default:
		printf("w a s d - for control Q - for exit");
		break;	
			}
		snake = moveDir(snake,key);
		snake2 = moveDir2(snake2,key2);
		}
		else {snake = moveDir(snake,key=snake.direction);
			snake2 = moveDir2(snake2,key2=snake2.direction);}
		sleep(1);
		system("cls");
		SetConsoleTextAttribute(hConsole, now_colors.color_snake);
		delay(&snake);
		printSnake(snake);
		SetConsoleTextAttribute(hConsole, now_colors.color_snake2);
		delay(&snake2);
		printSnake2(snake2);
		refreshFood(f, SEED_NUMBER);
        SetConsoleTextAttribute(hConsole, now_colors.color_food);
        putFood(f, SEED_NUMBER);
        //printFood(f);
        SetConsoleTextAttribute(hConsole, now_colors.color_level);
        printLevel(&snake);
		}//while
SetConsoleTextAttribute(hConsole, now_colors.color_message);
 printExit(&snake);
	return 0;
	}
