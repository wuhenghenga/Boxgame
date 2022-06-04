
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define LAST		10	//游戲種關卡數目

//地圖的列數和行數
#define MAP_ROW		16
#define MAP_COL		16

//定義按鍵值
#define KEY_UP		72	//向上
#define KEY_DOWN	80	//向下
#define KEY_LEFT	75	//向左
#define KEY_RIGHT	77	//向右
#define KEY_ESC		27	//按ESC撤銷上一步操作

//定義游戲進行時的操作
#define KEY_RETURN	1	//返回主界面
#define KEY_NEXT	2	//進入下一關
#define KEY_RETRY	3	//重啓本關卡

//定義地圖中的狀態
#define SPACE		0	//空地
#define WALL		1	//墻壁
#define TARGET		2	//目標位置
#define BOX			3	//箱子
#define PLAYER		4	//小人
#define TARGET_IN	5	//箱子已經推到的目標位置


//定義玩家輸入選擇
#define STARTGAME	'1'	//開始游戲第一關
#define STARTGAMEN	'2'	//選擇游戲第N關
#define EXITGAME	'3'	//退出游戲

//定義玩家操作控制方向
#define UP			1	//↑
#define RIGHT		2	//→
#define DOWN		3	//↓
#define LEFT		4	//←


//改變控制台窗口大小為width寬，height高
void set_size(int width, int height)
{
	HANDLE hOut, hIn;
	COORD size = {width, height};
	SMALL_RECT rc = {0, 0, width - 1, height - 1};
	CONSOLE_CURSOR_INFO cursor = {1, FALSE};

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleWindowInfo(hOut, TRUE, &rc);
	SetConsoleScreenBufferSize(hOut, size);
	SetConsoleCursorInfo(hOut, &cursor);
}
//播放wav格式的聲音
void play_sound(const char *sndfile)
{
	PlaySound(sndfile, NULL, SND_FILENAME | SND_ASYNC);
}

//設置控制台窗口的標題為字符串title
void set_title(char *title)
{
	SetConsoleTitle(title);
}

//移動光標到坐標(x, y)處
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//隱藏光標
void hide_cursor(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = 0;    //賦1為顯示，賦0為隱藏
    SetConsoleCursorInfo(hOut, &cci);
}


//顯示光標
void show_cursor(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = 1;     //賦1為顯示，賦0為隱藏
    SetConsoleCursorInfo(hOut, &cci);
}


//設置顏色函數： bcolor背景色，fcolor前景色
void set_color(int bcolor, int fcolor)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bcolor << 4) + fcolor);
}

//延時函數，單位毫秒
void delay(int dtime)
{
    Sleep(dtime);
}



//清除屏幕
void cls(void)
{
	system("cls");
}


//獲取按鍵
int getkey(void)
{
	int key = getch();

	if ((key == 0) || (key == 224))
	{
		key = getch();
	}

	return key;
}


//啓動游戲
void StartGame();

//游戲主界面每次通關后返回到主界面
int ShowMainMenu();

//進入游戲第n關
void StartLevel(int n);

//玩家遊戲過程，map為當前遊戲地圖，initmap為初始遊戲地圖，
//MAP_COL為遊戲地圖的行數
int PlayGame(int map[][MAP_COL], int initmap[][MAP_COL], int nboxes, int n);


//顯示遊戲地圖，地圖為當前遊戲地圖，當前為第n關，MAP_COL為遊戲地圖的列數
int PrintMap(int map[][MAP_COL], int n);


/*
 * map為當前遊戲地圖，initmap為初始遊戲地圖，MAP_COL為遊戲地圖的列數
 * playerx，playery為玩家當前坐標，direction為移動方向（上UP，下DOWN，
 *左LEFT，右RIGHT）
 * nsteps代表當前玩家本關已經移動的步數
 * ismusicon代表是否播放音效（1播放0關閉）
 */
void MoveBox(int map[][MAP_COL], int playerx, int playery, int direction, \
int initmap[][MAP_COL], int *nsteps, int ismusicon);

int main(void)
{
	StartGame();
}


void StartGame()
{
	int choice;		//選擇的主操作：1-進入遊戲第1關2-選指定的關卡3-退出遊戲
	int nchoice;	//選擇關卡數n

	do
	{
		choice = ShowMainMenu();		//顯示遊戲主菜單界面，返回玩家選擇

		switch (choice)
		{
			case STARTGAME:				 //鍵盤輸入1，開始第1關遊戲
				StartLevel(1);
				break;
			case STARTGAMEN:			//鍵盤輸入2，選擇遊戲的1到8關之一
				printf("\n\n\t\t\t\t請選擇游戲關數（1-%d）：", LAST);
				while (1)
				{
					gotoxy(57, 16);
					scanf("%d", &nchoice);
					fflush(stdin);
					//清除輸入的關數
					gotoxy(57, 16);
					printf("    ");
					if ((nchoice >= 1) && (nchoice <= LAST))
					{
						StartLevel(nchoice);
						break;
					}
					else
					{
						gotoxy(57, 14);
						printf("    ");	  //刪除錯誤的輸入
						gotoxy(57, 14);
					}
				}
				break;
			case EXITGAME:				//輸入3退出游戲
				cls();
				gotoxy(30, 11);
				printf("歡迎下次再來玩本游戲！");
				delay(2000);
				set_color(0, 7);		//恢復黑白底字
				cls();

				exit(0);
		}
	}
	while (1);
}


//顯示遊戲主菜單界面，返回玩家輸入的選擇
int ShowMainMenu()
{
	int choice = 0;

	cls();

	set_color(0, 0xe);			//設置遊戲主菜單的顏色

	set_title("推箱子游戲");	//設置窗口標題
	set_size(80, 26);

	//显示游戏主菜单
	printf("\n\n\n\n\n\n\t\t\t\t歡迎光臨推箱子游戲!\n\n");
	printf("\t\t\t\t1-----開始新游戲\n\n");
	printf("\t\t\t\t2-----選擇指定關數\n\n");
	printf("\t\t\t\t3-----退出游戲\n\n");
	printf("\t\t\t\t請輸入您的選擇（1-3）: ");

	show_cursor();				//顯示光標

	while (1)
	{
		choice = getche();

		if ((choice >= '1') && (choice <= '3'))
		{
			printf("\b");
			return choice;
		}
		else
		{
			printf("\b \b");	//刪除錯誤的輸入
		}
	}
}

//顯示遊戲地圖，參數n代表遊戲第n關
int PrintMap(int map[][MAP_COL], int n)
{
	int nsucc = 0;	    //已正確加入預定位置的箱子數
	int x, y;		    //繪製地圖的左上角的起點位置坐標

	hide_cursor();	//隱藏光標


	x = 25;			//初始點為第1列第25行
	y = 1;

	for (int i = 0; i < MAP_ROW; i++)
	{
		for (int j = 0; j < MAP_COL; j++)
		{
			gotoxy(j * 2 + x, i + y);	//圖形字符▓等都佔2個字符的寬度，所以乘2
			switch (map[i][j])
			{
				case 0:					//墻外空地
					printf("  ");
					break;
				case 1:
					set_color(0, 14);	//墻
					printf("▓");
					break;
				case 2:
					set_color(0, 11);	//目的地
					printf("○");
					break;
				case 3:					//箱子
					set_color(0, 11);
					printf("●");
					break;
				case 4:					//小人
					set_color(0, 10);
					printf("♀");
					break;
				case 5:					//箱子推到目的地後的顯示圖形
					nsucc++;
					set_color(0, 9);
					printf("☆");
					break;
			}
		}
	}
	printf("\n");
	printf("\n");

	set_color(0, 14);
	gotoxy(0, 17);
	printf("\t\t您當前正在第%d關！\n\n", n);
	printf("\t\t玩家:♀ 墻壁:▓ 箱子:● 目標:○ 到達目標:☆\n\n");
	printf("\t\t上:↑ 下:↓ 左:← 右:→ 返回主界面: Q 切換音效：O\n\n");
	printf("\t\t下一关: N  重啓本關: R  悔一步（不能連續）: ESC\n\n");

	return nsucc;						//返回已正確加入預定位置的箱子數
}

//開始進行游戲第n關
void  StartLevel(int n)
{
	int initmap[MAP_ROW][MAP_COL];		//表示原始地圖
	int nboxes;							// nboxes為統計本關中箱子的數量
	int i, j;
	FILE *fp;
	char line[80];						//讀取地圖壹行的緩沖區
	char *token;						//解析地圖每壹行的token
	int map[MAP_ROW][MAP_COL];			//第n關的遊戲地圖
	int nlines;							//用於連續讀取遊戲地圖MAP_ROW行的計數器
	int tn;								//用於讀取第n關地圖的計數器

	i = 0;								//地圖二維數列的行下標
	j = 0;								//地圖二維數列的列下標
	tn = n;
	nboxes = 0;
	nlines = 0;
    cls();
	//打開當前目錄下的遊戲地圖文件map.dat（壹個文本文件）
	if ((fp = fopen("map.dat", "rt")) == NULL)
	{
		printf("游戲地圖文件map.dat沒有找到！");

		delay(2000);

		exit(0); //退出
	}

	//printf("讀取第%d關地圖：\n", n);
	while (!feof(fp))
	{
		//跳過前面的n-1關的地圖
		while (tn > 0)
		{
			//讀取每關地圖前的導引兼分隔行=====
			fgets(line, 80, fp);

			//本行含有===則為引導兼分隔行
			if (strstr(line, "="))
			{
				tn--;
			}
		}

		//開始讀取地圖行
		fgets(line, 80, fp);
		if (!strstr(line, "="))
		{
			nlines++;
			if (nlines > MAP_ROW)
			{
				break;
			}

			//將地圖行用逗號，空格和換行符隔開的數值存入對應的遊戲地圖數列中
			token = strtok(line, ", \n");
			while (token != NULL)
			{
				map[i][j] = atoi(token);
				j++;
				token = strtok(NULL, ", \n");
			}

			//行數加1
			i++;

			//列下標回到行首
			j = 0;
		}
	}

	fclose(fp);	//地圖文件讀完，關閉文件

	for (i = 0; i < MAP_ROW; i++)
	{
		for (j = 0; j < MAP_COL; j++)
		{
			initmap[i][j] = map[i][j];
			if (map[i][j] == TARGET || map[i][j] == TARGET_IN)	//統計箱子個數
			{
				nboxes++;
			}
		}
	}

	//如果不是最後壹關，則進入下壹關，否則提示返回遊戲主菜單界面
	int ret = PlayGame(map, initmap, nboxes, n);

	switch (ret)
	{
		case KEY_NEXT:
			if (n != LAST)
			{
				StartLevel(n + 1);	//不是最後壹關則進入下壹關
			}
			else					//是最後壹關則返回主界面
			{
				cls();
				gotoxy(20, 11);
				printf("已經是最後一關了，2秒后將返回游戲主界面！");
				delay(2000);
			}
			break;
		case KEY_RETRY:
			StartLevel(n);			//重啟本關
			return;
		default:					//返回遊戲主界面
			return;
	}
}

//遊戲操作過程，nboxes代表箱子個數，當前遊戲位於第n關
//返回值：KEY_NEXT-直接進入下壹關，KEY_RETURN-返回遊戲主菜單界面，
//KEY_RETRY-重啟本關
int PlayGame(int map[][MAP_COL], int initmap[][MAP_COL], int nboxes, int n)
{
	int i, j;
	int playerx, playery;		//玩家的x，y位置坐標
	char op;					//遊戲操作所按下的方向鍵
	int nsucc = 0;              //成功推達目的地的箱子個數

	//用於撤消上壹步操作的相關臨時存儲變量
	int lastpx, lastpy;			//上壹步玩家的x，y位置坐標
	char oplast;				//上壹步的操作方向鍵
	int nsucclast;				//上壹步時已經成功推達目的地的箱子個數
	int cur, next, nnext;		//上壹步的當前、下壹步、下下壹步的map地圖元素值
	int isFirst = 1;			//按ESC键之前是否有其它操作

	long starttime, endtime;	//用於統計玩家本關用時的起止時間
	int nsteps = 0;				//用於統計玩家移動的步數
	int ismusicon = 1;			//是否打開音效，1打開，0關閉

	starttime = time(NULL);
	while (1)
	{
		//循環遍歷遊戲地圖，尋找玩家的位置
		for (i = 0; i < MAP_ROW; i++)
		{
			for (j = 0; j < MAP_COL; j++)
			{
				//地圖中元素數值為6的代表了玩家的位置
				if (map[i][j] == PLAYER)
				{
					break;
				}
			}
			if (j < MAP_COL)
			{
				break;
			}
		}

		//將地圖數列中的玩家的x，y坐標賦值給playerx和playery
		playerx = i;
		playery = j;

		//保存用於撤消上壹步時已經成功推達的箱子的個數
		nsucclast = nsucc;

		//打印當前遊戲第n關的遊戲地圖
		nsucc = PrintMap(map, n);

		//已經成功將所有箱子都推達目的地，則闖關成功
		if (nsucc == nboxes)
		{
			endtime = time(NULL);
			gotoxy(34, 17);
			printf("恭喜您移動%d步用時%d秒第%d关闖關成功！！\n", nsteps, \
				(endtime - starttime), n);
			if (ismusicon)
			{
				play_sound("over.wav"); //播放過關的聲音
			}
			delay(3000);				//延時3秒進入下一關

			return KEY_NEXT;
		}
		else//還有箱子沒有推達目的地
		{
			//如果玩家沒有按鍵
			while (!kbhit())
			{
				delay(100);
				gotoxy(34, 17);
				endtime = time(NULL);
				printf("您已移動%d步，用時%d秒！                       \n", \
					nsteps, (endtime - starttime));
			}

			op = getch();	//繼續讀取玩家鍵盤輸入

			//輸入大寫N或小寫n都直接進入遊戲下一關
			if ((op == 'n') || (op == 'N'))
			{
				return KEY_NEXT;
			}
			else if ((op == 'o') || (op == 'O'))
			{
				ismusicon = !ismusicon;//切換音效
			}
			else if ((op == 'q') || (op == 'Q'))
			{
				//輸入大寫Q或小寫q則返回主菜單界面
				return KEY_RETURN;
			}
			else if ((op == 'R') || (op == 'r'))
			{
				//輸入大寫R或小寫r則重啟本關
				return KEY_RETRY;
			}
		}

		//玩家操作輸入鍵
		switch (op)
		{
			case KEY_UP:							//向上的光標鍵
				cur = map[playerx][playery];		//保存當前位置值
				next  = map[playerx - 1][playery];	//保存下一個位置值
				nnext = map[playerx - 2][playery];	//保存下一個一個位置值
				oplast = op;						//保存當前方向值
				lastpx = playerx;					//保存當前玩家的x坐標
				lastpy = playery;					//保存當前玩家的y坐標
				MoveBox(map, playerx, playery, UP, initmap, &nsteps, ismusicon);
				isFirst = 0;						//按ESC前已有別的操作，可以撤消
				break;
			case KEY_RIGHT:							//向右的光標鍵和上麵類似的處理
				cur = map[playerx][playery];		//保存當前位置值
				next  = map[playerx][playery + 1];
				nnext = map[playerx][playery + 2];
				oplast = op;
				lastpx = playerx;
				lastpy = playery;
				MoveBox(map, playerx, playery, RIGHT, initmap, &nsteps, ismusicon);
				isFirst = 0;
				break;
			case KEY_DOWN:							//向下的光標鍵和上麵類似的處理
				cur = map[playerx][playery];
				next  = map[playerx + 1][playery];
				nnext = map[playerx + 2][playery];
				oplast = op;
				lastpx = playerx;
				lastpy = playery;
				MoveBox(map, playerx, playery, DOWN, initmap, &nsteps, ismusicon);
				isFirst = 0;
				break;
			case KEY_LEFT:							//向左的光標鍵和上麵類似的處理
				cur = map[playerx][playery];
				next  = map[playerx][playery - 1];
				nnext = map[playerx][playery - 2];
				oplast = op;
				lastpx = playerx;
				lastpy = playery;
				MoveBox(map, playerx, playery, LEFT, initmap, &nsteps, ismusicon);
				isFirst = 0;
				break;
			case KEY_ESC:							//如果前面有操作，按ESC鍵則撤消上一步操作
				if (!isFirst)						//否則前面沒有操作，按ESC鍵無效果
				{
					isFirst = 1;
					nsteps--;						//玩家移動步數-1
					gotoxy(34, 17);
					endtime = time(NULL);
					printf("您已移动%d步，用时%d秒！                  \n", \
						nsteps, (endtime - starttime));

                    //撤消上一步操作，恢復相關的變量值（
                    //根據上一步操作進行反向恢復）
					nsucc = nsucclast;
					playerx = lastpx;
					playery = lastpy;
					map[playerx][playery] = cur;

					switch (oplast)
					{
						case KEY_UP:
							map[playerx - 1][playery] = next;
							map[playerx - 2][playery] = nnext;
							break;
						case KEY_DOWN:
							map[playerx + 1][playery] = next;
							map[playerx + 2][playery] = nnext;
							break;
						case KEY_LEFT:
							map[playerx][playery - 1] = next;
							map[playerx][playery - 2] = nnext;
							break;
						case KEY_RIGHT:
							map[playerx][playery + 1] = next;
							map[playerx][playery + 2] = nnext;
							break;
					}
				}
		}
	}
}

//玩家方向移動，處理對應的遊戲地圖數組
void MoveBox(int map[][MAP_COL], int playerx, int playery, int direction, \
		int initmap[][MAP_COL], int *nsteps, int ismusicon)
{
	int nextplayerx, nextplayery;		//玩家下一步要走的位置坐標
	int nnextplayerx, nnextplayery;		//玩家下下一要走的位置坐標

	switch (direction)
	{
		case UP:	//向上移動
			nextplayerx = playerx - 1;
			nextplayery = playery;
			nnextplayerx = playerx - 2;
			nnextplayery = playery;
			break;
		case RIGHT:	//向右移動
			nextplayerx = playerx;
			nextplayery = playery + 1;
			nnextplayerx = playerx;
			nnextplayery = playery + 2;
			break;
		case DOWN:	//向下移動
			nextplayerx = playerx + 1;
			nextplayery = playery;
			nnextplayerx = playerx + 2;
			nnextplayery = playery;
			break;
		case LEFT:	//向左移動
			nextplayerx = playerx;
			nextplayery = playery - 1;
			nnextplayerx = playerx;
			nnextplayery = playery - 2;
	}

	//對遊戲地圖進行相應處理
	//判斷玩家接下來要走的位置
	switch (map[nextplayerx][nextplayery])
	{
		case WALL:			//下一位置是牆，不能移動
			break;
		case SPACE:			//下一位置為牆內空地
		case TARGET:		//下一位置為空的目的地
			//玩家移動步數+1
			(*nsteps)++;
			if (ismusicon)
			{
				play_sound("go.wav");				//播放移動的聲音
			}
			map[nextplayerx][nextplayery] = PLAYER;	//將玩家移動到下一位置
			if ((initmap[playerx][playery] == TARGET)
				|| (initmap[playerx][playery] == TARGET_IN))
			{
				//將玩家所在位置初始為空目的地或滿目的地，
				//玩家移動後此處恢復為空目的地
				map[playerx][playery] = TARGET;
			}
			else
			{
                //玩家所在位置初始為牆內空地，箱子或者玩家，
                //玩家移動後此處恢復為牆內空地
				map[playerx][playery] = SPACE;
			}
			break;
		case BOX:	//下一位置是箱子和下一位置是已放好箱子的目的地
		case TARGET_IN:
			if (map[nnextplayerx][nnextplayery] == TARGET)
			{
				//下下位置為空目的地，箱子和玩家一起移動，箱子推達目的地
				map[nnextplayerx][nnextplayery] = TARGET_IN;
				map[nextplayerx][nextplayery] = PLAYER;
				//玩家移動步數+1
				(*nsteps)++;
				if (ismusicon)
				{
					play_sound("go.wav");		//播放移動的聲音
				}
			}
			else if (map[nnextplayerx][nnextplayery] == SPACE)
			{
				//下下位置為空地，箱子和玩家一起移動，箱子落入空地
				map[nnextplayerx][nnextplayery] = BOX;
				map[nextplayerx][nextplayery] = PLAYER;
				//玩家移動步數+1
				(*nsteps)++;
				if (ismusicon)
				{
					play_sound("go.wav");		//播放移動的聲音
				}
			}
			else
			{	//下一個位置為牆，箱子或滿目的地，不能移動，直接退出
				break;
			}
			if ((initmap[playerx][playery] == TARGET)
				|| (initmap[playerx][playery] == TARGET_IN))
			{

            //玩家所在位置初始為空目的地或滿目的地，
            //玩家移動後此處恢復為空目的地
				map[playerx][playery] = TARGET;
			}
			else
			{

                //玩家所在位置初始為牆內空地，箱子或者玩家，
				//玩家移動後此處恢復為牆內空地
				map[playerx][playery] = SPACE;
			}
	}
}
