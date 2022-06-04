
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define LAST		10	//�Α�N�P����Ŀ

//�؈D���Д����Д�
#define MAP_ROW		16
#define MAP_COL		16

//���x���Iֵ
#define KEY_UP		72	//����
#define KEY_DOWN	80	//����
#define KEY_LEFT	75	//����
#define KEY_RIGHT	77	//����
#define KEY_ESC		27	//��ESC���N��һ������

//���x�Α��M�Еr�Ĳ���
#define KEY_RETURN	1	//����������
#define KEY_NEXT	2	//�M����һ�P
#define KEY_RETRY	3	//�؆����P��

//���x�؈D�еĠ�B
#define SPACE		0	//�յ�
#define WALL		1	//����
#define TARGET		2	//Ŀ��λ��
#define BOX			3	//����
#define PLAYER		4	//С��
#define TARGET_IN	5	//�����ѽ��Ƶ���Ŀ��λ��


//���x���ݔ���x��
#define STARTGAME	'1'	//�_ʼ�Α��һ�P
#define STARTGAMEN	'2'	//�x���Α��N�P
#define EXITGAME	'3'	//�˳��Α�

//���x��Ҳ������Ʒ���
#define UP			1	//��
#define RIGHT		2	//��
#define DOWN		3	//��
#define LEFT		4	//��


//��׃����̨���ڴ�С��width����height��
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
//����wav��ʽ����
void play_sound(const char *sndfile)
{
	PlaySound(sndfile, NULL, SND_FILENAME | SND_ASYNC);
}

//�O�ÿ���̨���ڵĘ��}���ַ���title
void set_title(char *title)
{
	SetConsoleTitle(title);
}

//�Ƅӹ�˵�����(x, y)̎
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//�[�ع��
void hide_cursor(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = 0;    //�x1���@ʾ���x0���[��
    SetConsoleCursorInfo(hOut, &cci);
}


//�@ʾ���
void show_cursor(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = 1;     //�x1���@ʾ���x0���[��
    SetConsoleCursorInfo(hOut, &cci);
}


//�O���ɫ������ bcolor����ɫ��fcolorǰ��ɫ
void set_color(int bcolor, int fcolor)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bcolor << 4) + fcolor);
}

//�ӕr��������λ����
void delay(int dtime)
{
    Sleep(dtime);
}



//�����Ļ
void cls(void)
{
	system("cls");
}


//�@ȡ���I
int getkey(void)
{
	int key = getch();

	if ((key == 0) || (key == 224))
	{
		key = getch();
	}

	return key;
}


//�����Α�
void StartGame();

//�Α�������ÿ��ͨ�P�󷵻ص�������
int ShowMainMenu();

//�M���Α��n�P
void StartLevel(int n);

//����[���^�̣�map�鮔ǰ�[��؈D��initmap���ʼ�[��؈D��
//MAP_COL���[��؈D���Д�
int PlayGame(int map[][MAP_COL], int initmap[][MAP_COL], int nboxes, int n);


//�@ʾ�[��؈D���؈D�鮔ǰ�[��؈D����ǰ���n�P��MAP_COL���[��؈D���Д�
int PrintMap(int map[][MAP_COL], int n);


/*
 * map�鮔ǰ�[��؈D��initmap���ʼ�[��؈D��MAP_COL���[��؈D���Д�
 * playerx��playery����Ү�ǰ���ˣ�direction���Ƅӷ�����UP����DOWN��
 *��LEFT����RIGHT��
 * nsteps����ǰ��ұ��P�ѽ��ƄӵĲ���
 * ismusicon�����Ƿ񲥷���Ч��1����0�P�]��
 */
void MoveBox(int map[][MAP_COL], int playerx, int playery, int direction, \
int initmap[][MAP_COL], int *nsteps, int ismusicon);

int main(void)
{
	StartGame();
}


void StartGame()
{
	int choice;		//�x�����������1-�M���[���1�P2-�xָ�����P��3-�˳��[��
	int nchoice;	//�x���P����n

	do
	{
		choice = ShowMainMenu();		//�@ʾ�[�����ˆν��棬��������x��

		switch (choice)
		{
			case STARTGAME:				 //�I�Pݔ��1���_ʼ��1�P�[��
				StartLevel(1);
				break;
			case STARTGAMEN:			//�I�Pݔ��2���x���[���1��8�P֮һ
				printf("\n\n\t\t\t\tՈ�x���Α��P����1-%d����", LAST);
				while (1)
				{
					gotoxy(57, 16);
					scanf("%d", &nchoice);
					fflush(stdin);
					//���ݔ����P��
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
						printf("    ");	  //�h���e�`��ݔ��
						gotoxy(57, 14);
					}
				}
				break;
			case EXITGAME:				//ݔ��3�˳��Α�
				cls();
				gotoxy(30, 11);
				printf("�gӭ�´��ف��汾�Α�");
				delay(2000);
				set_color(0, 7);		//�֏ͺڰ׵���
				cls();

				exit(0);
		}
	}
	while (1);
}


//�@ʾ�[�����ˆν��棬�������ݔ����x��
int ShowMainMenu()
{
	int choice = 0;

	cls();

	set_color(0, 0xe);			//�O���[�����ˆε��ɫ

	set_title("�������Α�");	//�O�ô��ژ��}
	set_size(80, 26);

	//��ʾ��Ϸ���˵�
	printf("\n\n\n\n\n\n\t\t\t\t�gӭ���R�������Α�!\n\n");
	printf("\t\t\t\t1-----�_ʼ���Α�\n\n");
	printf("\t\t\t\t2-----�x��ָ���P��\n\n");
	printf("\t\t\t\t3-----�˳��Α�\n\n");
	printf("\t\t\t\tՈݔ�������x��1-3��: ");

	show_cursor();				//�@ʾ���

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
			printf("\b \b");	//�h���e�`��ݔ��
		}
	}
}

//�@ʾ�[��؈D������n�����[���n�P
int PrintMap(int map[][MAP_COL], int n)
{
	int nsucc = 0;	    //�����_�����A��λ�õ����Ӕ�
	int x, y;		    //�L�u�؈D�����Ͻǵ����cλ������

	hide_cursor();	//�[�ع��


	x = 25;			//��ʼ�c���1�е�25��
	y = 1;

	for (int i = 0; i < MAP_ROW; i++)
	{
		for (int j = 0; j < MAP_COL; j++)
		{
			gotoxy(j * 2 + x, i + y);	//�D���ַ����ȶ���2���ַ��Č��ȣ����Գ�2
			switch (map[i][j])
			{
				case 0:					//����յ�
					printf("  ");
					break;
				case 1:
					set_color(0, 14);	//��
					printf("��");
					break;
				case 2:
					set_color(0, 11);	//Ŀ�ĵ�
					printf("��");
					break;
				case 3:					//����
					set_color(0, 11);
					printf("��");
					break;
				case 4:					//С��
					set_color(0, 10);
					printf("��");
					break;
				case 5:					//�����Ƶ�Ŀ�ĵ�����@ʾ�D��
					nsucc++;
					set_color(0, 9);
					printf("��");
					break;
			}
		}
	}
	printf("\n");
	printf("\n");

	set_color(0, 14);
	gotoxy(0, 17);
	printf("\t\t����ǰ���ڵ�%d�P��\n\n", n);
	printf("\t\t���:�� ����:�� ����:�� Ŀ��:�� ���_Ŀ��:��\n\n");
	printf("\t\t��:�� ��:�� ��:�� ��:�� ����������: Q �ГQ��Ч��O\n\n");
	printf("\t\t��һ��: N  �؆����P: R  ��һ���������B�m��: ESC\n\n");

	return nsucc;						//���������_�����A��λ�õ����Ӕ�
}

//�_ʼ�M���Α��n�P
void  StartLevel(int n)
{
	int initmap[MAP_ROW][MAP_COL];		//��ʾԭʼ�؈D
	int nboxes;							// nboxes��yӋ���P�����ӵĔ���
	int i, j;
	FILE *fp;
	char line[80];						//�xȡ�؈DҼ�еľ��_�^
	char *token;						//�����؈DÿҼ�е�token
	int map[MAP_ROW][MAP_COL];			//��n�P���[��؈D
	int nlines;							//����B�m�xȡ�[��؈DMAP_ROW�е�Ӌ����
	int tn;								//����xȡ��n�P�؈D��Ӌ����

	i = 0;								//�؈D���S���е�����
	j = 0;								//�؈D���S���е�����
	tn = n;
	nboxes = 0;
	nlines = 0;
    cls();
	//���_��ǰĿ��µ��[��؈D�ļ�map.dat��Ҽ���ı��ļ���
	if ((fp = fopen("map.dat", "rt")) == NULL)
	{
		printf("�Α�؈D�ļ�map.dat�]���ҵ���");

		delay(2000);

		exit(0); //�˳�
	}

	//printf("�xȡ��%d�P�؈D��\n", n);
	while (!feof(fp))
	{
		//���^ǰ���n-1�P�ĵ؈D
		while (tn > 0)
		{
			//�xȡÿ�P�؈Dǰ�Č�����ָ���=====
			fgets(line, 80, fp);

			//���к���===�t��������ָ���
			if (strstr(line, "="))
			{
				tn--;
			}
		}

		//�_ʼ�xȡ�؈D��
		fgets(line, 80, fp);
		if (!strstr(line, "="))
		{
			nlines++;
			if (nlines > MAP_ROW)
			{
				break;
			}

			//���؈D���ö�̖���ո�͓Q�з����_�Ĕ�ֵ���댦�����[��؈D������
			token = strtok(line, ", \n");
			while (token != NULL)
			{
				map[i][j] = atoi(token);
				j++;
				token = strtok(NULL, ", \n");
			}

			//�Д���1
			i++;

			//���˻ص�����
			j = 0;
		}
	}

	fclose(fp);	//�؈D�ļ��x�꣬�P�]�ļ�

	for (i = 0; i < MAP_ROW; i++)
	{
		for (j = 0; j < MAP_COL; j++)
		{
			initmap[i][j] = map[i][j];
			if (map[i][j] == TARGET || map[i][j] == TARGET_IN)	//�yӋ���ӂ���
			{
				nboxes++;
			}
		}
	}

	//�����������Ҽ�P���t�M����Ҽ�P����t��ʾ�����[�����ˆν���
	int ret = PlayGame(map, initmap, nboxes, n);

	switch (ret)
	{
		case KEY_NEXT:
			if (n != LAST)
			{
				StartLevel(n + 1);	//��������Ҽ�P�t�M����Ҽ�P
			}
			else					//������Ҽ�P�t����������
			{
				cls();
				gotoxy(20, 11);
				printf("�ѽ�������һ�P�ˣ�2��󌢷����Α������棡");
				delay(2000);
			}
			break;
		case KEY_RETRY:
			StartLevel(n);			//�؆����P
			return;
		default:					//�����[��������
			return;
	}
}

//�[������^�̣�nboxes�������ӂ�������ǰ�[��λ춵�n�P
//����ֵ��KEY_NEXT-ֱ���M����Ҽ�P��KEY_RETURN-�����[�����ˆν��棬
//KEY_RETRY-�؆����P
int PlayGame(int map[][MAP_COL], int initmap[][MAP_COL], int nboxes, int n)
{
	int i, j;
	int playerx, playery;		//��ҵ�x��yλ������
	char op;					//�[����������µķ����I
	int nsucc = 0;              //�ɹ����_Ŀ�ĵص����ӂ���

	//��춳�����Ҽ�����������P�R�r�惦׃��
	int lastpx, lastpy;			//��Ҽ����ҵ�x��yλ������
	char oplast;				//��Ҽ���Ĳ��������I
	int nsucclast;				//��Ҽ���r�ѽ��ɹ����_Ŀ�ĵص����ӂ���
	int cur, next, nnext;		//��Ҽ���Į�ǰ����Ҽ��������Ҽ����map�؈DԪ��ֵ
	int isFirst = 1;			//��ESC��֮ǰ�Ƿ�����������

	long starttime, endtime;	//��춽yӋ��ұ��P�Õr����ֹ�r�g
	int nsteps = 0;				//��춽yӋ����ƄӵĲ���
	int ismusicon = 1;			//�Ƿ���_��Ч��1���_��0�P�]

	starttime = time(NULL);
	while (1)
	{
		//ѭ�h��v�[��؈D��������ҵ�λ��
		for (i = 0; i < MAP_ROW; i++)
		{
			for (j = 0; j < MAP_COL; j++)
			{
				//�؈D��Ԫ�ؔ�ֵ��6�Ĵ�������ҵ�λ��
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

		//���؈D�����е���ҵ�x��y�����xֵ�oplayerx��playery
		playerx = i;
		playery = j;

		//������춳�����Ҽ���r�ѽ��ɹ����_�����ӵĂ���
		nsucclast = nsucc;

		//��ӡ��ǰ�[���n�P���[��؈D
		nsucc = PrintMap(map, n);

		//�ѽ��ɹ����������Ӷ����_Ŀ�ĵأ��t�J�P�ɹ�
		if (nsucc == nboxes)
		{
			endtime = time(NULL);
			gotoxy(34, 17);
			printf("��ϲ���Ƅ�%d���Õr%d���%d���J�P�ɹ�����\n", nsteps, \
				(endtime - starttime), n);
			if (ismusicon)
			{
				play_sound("over.wav"); //�����^�P����
			}
			delay(3000);				//�ӕr3���M����һ�P

			return KEY_NEXT;
		}
		else//߀�����ӛ]�����_Ŀ�ĵ�
		{
			//�����қ]�а��I
			while (!kbhit())
			{
				delay(100);
				gotoxy(34, 17);
				endtime = time(NULL);
				printf("�����Ƅ�%d�����Õr%d�룡                       \n", \
					nsteps, (endtime - starttime));
			}

			op = getch();	//�^�m�xȡ����I�Pݔ��

			//ݔ���N��С��n��ֱ���M���[����һ�P
			if ((op == 'n') || (op == 'N'))
			{
				return KEY_NEXT;
			}
			else if ((op == 'o') || (op == 'O'))
			{
				ismusicon = !ismusicon;//�ГQ��Ч
			}
			else if ((op == 'q') || (op == 'Q'))
			{
				//ݔ���Q��С��q�t�������ˆν���
				return KEY_RETURN;
			}
			else if ((op == 'R') || (op == 'r'))
			{
				//ݔ���R��С��r�t�؆����P
				return KEY_RETRY;
			}
		}

		//��Ҳ���ݔ���I
		switch (op)
		{
			case KEY_UP:							//���ϵĹ���I
				cur = map[playerx][playery];		//���殔ǰλ��ֵ
				next  = map[playerx - 1][playery];	//������һ��λ��ֵ
				nnext = map[playerx - 2][playery];	//������һ��һ��λ��ֵ
				oplast = op;						//���殔ǰ����ֵ
				lastpx = playerx;					//���殔ǰ��ҵ�x����
				lastpy = playery;					//���殔ǰ��ҵ�y����
				MoveBox(map, playerx, playery, UP, initmap, &nsteps, ismusicon);
				isFirst = 0;						//��ESCǰ���Єe�Ĳ��������Գ���
				break;
			case KEY_RIGHT:							//���ҵĹ���I�����I��Ƶ�̎��
				cur = map[playerx][playery];		//���殔ǰλ��ֵ
				next  = map[playerx][playery + 1];
				nnext = map[playerx][playery + 2];
				oplast = op;
				lastpx = playerx;
				lastpy = playery;
				MoveBox(map, playerx, playery, RIGHT, initmap, &nsteps, ismusicon);
				isFirst = 0;
				break;
			case KEY_DOWN:							//���µĹ���I�����I��Ƶ�̎��
				cur = map[playerx][playery];
				next  = map[playerx + 1][playery];
				nnext = map[playerx + 2][playery];
				oplast = op;
				lastpx = playerx;
				lastpy = playery;
				MoveBox(map, playerx, playery, DOWN, initmap, &nsteps, ismusicon);
				isFirst = 0;
				break;
			case KEY_LEFT:							//����Ĺ���I�����I��Ƶ�̎��
				cur = map[playerx][playery];
				next  = map[playerx][playery - 1];
				nnext = map[playerx][playery - 2];
				oplast = op;
				lastpx = playerx;
				lastpy = playery;
				MoveBox(map, playerx, playery, LEFT, initmap, &nsteps, ismusicon);
				isFirst = 0;
				break;
			case KEY_ESC:							//���ǰ���в�������ESC�I�t������һ������
				if (!isFirst)						//��tǰ��]�в�������ESC�I�oЧ��
				{
					isFirst = 1;
					nsteps--;						//����ƄӲ���-1
					gotoxy(34, 17);
					endtime = time(NULL);
					printf("�����ƶ�%d������ʱ%d�룡                  \n", \
						nsteps, (endtime - starttime));

                    //������һ���������֏����P��׃��ֵ��
                    //������һ�������M�з���֏ͣ�
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

//��ҷ����Ƅӣ�̎�팦�����[��؈D���M
void MoveBox(int map[][MAP_COL], int playerx, int playery, int direction, \
		int initmap[][MAP_COL], int *nsteps, int ismusicon)
{
	int nextplayerx, nextplayery;		//�����һ��Ҫ�ߵ�λ������
	int nnextplayerx, nnextplayery;		//�������һҪ�ߵ�λ������

	switch (direction)
	{
		case UP:	//�����Ƅ�
			nextplayerx = playerx - 1;
			nextplayery = playery;
			nnextplayerx = playerx - 2;
			nnextplayery = playery;
			break;
		case RIGHT:	//�����Ƅ�
			nextplayerx = playerx;
			nextplayery = playery + 1;
			nnextplayerx = playerx;
			nnextplayery = playery + 2;
			break;
		case DOWN:	//�����Ƅ�
			nextplayerx = playerx + 1;
			nextplayery = playery;
			nnextplayerx = playerx + 2;
			nnextplayery = playery;
			break;
		case LEFT:	//�����Ƅ�
			nextplayerx = playerx;
			nextplayery = playery - 1;
			nnextplayerx = playerx;
			nnextplayery = playery - 2;
	}

	//���[��؈D�M������̎��
	//�Д���ҽ���Ҫ�ߵ�λ��
	switch (map[nextplayerx][nextplayery])
	{
		case WALL:			//��һλ���Ǡ��������Ƅ�
			break;
		case SPACE:			//��һλ�Þ頝�ȿյ�
		case TARGET:		//��һλ�Þ�յ�Ŀ�ĵ�
			//����ƄӲ���+1
			(*nsteps)++;
			if (ismusicon)
			{
				play_sound("go.wav");				//�����Ƅӵ���
			}
			map[nextplayerx][nextplayery] = PLAYER;	//������Ƅӵ���һλ��
			if ((initmap[playerx][playery] == TARGET)
				|| (initmap[playerx][playery] == TARGET_IN))
			{
				//���������λ�ó�ʼ���Ŀ�ĵػ�MĿ�ĵأ�
				//����Ƅ����̎�֏͞��Ŀ�ĵ�
				map[playerx][playery] = TARGET;
			}
			else
			{
                //�������λ�ó�ʼ�頝�ȿյأ����ӻ�����ң�
                //����Ƅ����̎�֏͞頝�ȿյ�
				map[playerx][playery] = SPACE;
			}
			break;
		case BOX:	//��һλ�������Ӻ���һλ�����ѷź����ӵ�Ŀ�ĵ�
		case TARGET_IN:
			if (map[nnextplayerx][nnextplayery] == TARGET)
			{
				//����λ�Þ��Ŀ�ĵأ����Ӻ����һ���Ƅӣ��������_Ŀ�ĵ�
				map[nnextplayerx][nnextplayery] = TARGET_IN;
				map[nextplayerx][nextplayery] = PLAYER;
				//����ƄӲ���+1
				(*nsteps)++;
				if (ismusicon)
				{
					play_sound("go.wav");		//�����Ƅӵ���
				}
			}
			else if (map[nnextplayerx][nnextplayery] == SPACE)
			{
				//����λ�Þ�յأ����Ӻ����һ���Ƅӣ���������յ�
				map[nnextplayerx][nnextplayery] = BOX;
				map[nextplayerx][nextplayery] = PLAYER;
				//����ƄӲ���+1
				(*nsteps)++;
				if (ismusicon)
				{
					play_sound("go.wav");		//�����Ƅӵ���
				}
			}
			else
			{	//��һ��λ�Þ頝�����ӻ�MĿ�ĵأ������Ƅӣ�ֱ���˳�
				break;
			}
			if ((initmap[playerx][playery] == TARGET)
				|| (initmap[playerx][playery] == TARGET_IN))
			{

            //�������λ�ó�ʼ���Ŀ�ĵػ�MĿ�ĵأ�
            //����Ƅ����̎�֏͞��Ŀ�ĵ�
				map[playerx][playery] = TARGET;
			}
			else
			{

                //�������λ�ó�ʼ�頝�ȿյأ����ӻ�����ң�
				//����Ƅ����̎�֏͞頝�ȿյ�
				map[playerx][playery] = SPACE;
			}
	}
}
