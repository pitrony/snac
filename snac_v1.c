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
#define FOOD_EXPIRE_SECONDS 30
#define SEED_NUMBER 4
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

struct food
{
int x;
int y;
time_t put_time;
char point;
uint8_t enable;
} food[MAX_FOOD_SIZE];
	


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
//{
  //  COORD cd;
    //cd.X=x;
    //cd.Y=y;
    //SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cd);
//}

void putFoodSeed(struct food *fp)
{
char spoint[2];
gotoxy(fp->y, fp->x);
printf(" ");
fp->x=rand()%(MAX_X-1);
fp->y=rand()%(MAX_Y-2)+1;
fp->put_time=time(NULL);
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
	if(f[i].put_time)
		{
		if(!f[i].enable || ((time(NULL)-f[i].put_time)>FOOD_EXPIRE_SECONDS))
			{
			putFoodSeed(&f[i]);
			}
			//else printFood(&f[i]);
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
	struct food f[MAX_FOOD_SIZE]; //food  f;
	initFood(f, MAX_FOOD_SIZE);
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
		case -87: //й
		case -119://Й
		case 'Q'://81 'Q'
		case 'q'://113 'q'
		printf("\nBay!");
		return 0;
		break;
		case 'P':// 'P'
		case 'p':// 'p'
		printf("\nGame in PAUSE\n\n");
		system("pause");
		break;
		
		default:
		printf("w a s d - for control Q - for exit");
		break;	
			}
		snake = moveDir(snake,key);
		}
		else snake = moveDir(snake,key=snake.preddirection);
		sleep(1);
		system("cls");
		printSnake(snake);
		delay(&snake);
        putFood(f, SEED_NUMBER);
        refreshFood(f, SEED_NUMBER);
        printLevel(&snake);
		}
 printExit(&snake);
	return 0;
	}
