#include<stdio.h>
#include<easyx.h>
#include<time.h>

/**************函数原型**************/ 
void init();//初始化
void DrawMap();//绘制地图
void initmap();	//赢了之后初始化游戏数据 更新地图
void Play();//核心玩法函数
void Judg();//判断是否踩到雷或者赢了比赛
void Expand(int row,int col);//展开雷区
void copyright();//版权信息

/**************全局变量**************/
int map_row, map_col;	//地图大小
int times;	//翻开的次数
int mine;	//雷的数量
int isfail;	//判断是否失败
int pass;	//雷的数量达到10% 就扩大游戏区域
HWND hWnd;	//当前窗口
int map[30][30] = { 0 };	//地图	用map_row map_col控制地图大小
IMAGE imgs[16];	//15张图片


int main()
{

	copyright();//显示版权信息，5s后自动进入游戏

	init();//初始化游戏界面
	DrawMap();//绘图
	while (1)//游戏主心跳
	{
		Play();
		Judg();
	}
	getchar();
	closegraph();
	return 0;
}

void init()//初始化 加载图片
{
	srand((unsigned)time(NULL));//撒随机数种子
	map_row = 10;	//地图行
	map_col = 10;	//地图列
	isfail = 0;
	pass = 0;		
	//加载图片
	loadimage(&imgs[0], L"素材/0.bmp", 25, 25);
	loadimage(&imgs[1], L"素材/1.bmp", 25, 25);
	loadimage(&imgs[2], L"素材/2.bmp", 25, 25);
	loadimage(&imgs[3], L"素材/3.bmp", 25, 25);
	loadimage(&imgs[4], L"素材/4.bmp", 25, 25);
	loadimage(&imgs[5], L"素材/5.bmp", 25, 25);
	loadimage(&imgs[6], L"素材/6.bmp", 25, 25);
	loadimage(&imgs[7], L"素材/7.bmp", 25, 25);
	loadimage(&imgs[8], L"素材/8.bmp", 25, 25);
	loadimage(&imgs[9], L"素材/标记.bmp", 25, 25);
	loadimage(&imgs[10], L"素材/空.bmp", 25, 25);
	loadimage(&imgs[11], L"素材/雷0.bmp", 25, 25);
	loadimage(&imgs[12], L"素材/雷1.bmp", 25, 25);
	loadimage(&imgs[13], L"素材/雷2.bmp", 25, 25);

	initmap();
}
void initmap()//对地图进行初始化 实现对雷区的赋值 和 对雷区周围格子统计周围雷数的赋值
{


	int mine_r, mine_c;
	ZeroMemory(map, sizeof(int) * 30 * 30);		//初始化地图为0 作用相当于memset
	times = 0;	//翻开次数
	mine += 5;	//每次循环 加大难度 雷的数量 + 5
	
	float x = mine / (float)(map_row * map_col);//计算雷的密度
	if ( x > 0.15)
	{
		//当雷的密度大于15%时扩大游戏区域	先横向扩大5个后纵向扩大5个
		if (pass++ % 2)		//使 pass 在 0 和 1之间变换		首先是0 ，然后0 1 之间循环
		{
			map_row += 5;	//纵向扩大5个区域
		}
		else
		{
			map_col += 5;	//横向扩大5个区域
		}
	}

	hWnd = initgraph(map_col * 25, map_row * 25);		//初始化地图大小，获取窗口句柄

	
	for (int i = 0; i < mine;)
	{
		mine_r = rand() % map_row + 1;	//雷所在的行数和列数
		mine_c = rand() % map_col + 1;
		if (map[mine_r][mine_c] == 0)	//该处没有雷
		{
			map[mine_r][mine_c] = -1;	//在数组中放雷 该处值为-1
			i++;						//放入雷后雷的数量加一
		}
	}
	//给雷附近的数加一
	for (int i = 1; i <= map_row; i++)
	{
		for (int j = 1; j <= map_col; j++)
		{
			if (map[i][j] != -1)
			{
				//检查周围雷的个数
				for (int m = i - 1; m <= i + 1; m++)
				{
					for (int n = j - 1; n <= j + 1; n++)
					{
						if (map[m][n] == -1)
						{	//附近有雷，数字加一
							map[i][j]++;
						}
					}
				}
			}
		}
	}

	wchar_t arr[40];
	wsprintf(arr, L"当前雷数:%d  Powered by GaoLiZhangQiu", mine);
	SetWindowText(hWnd, arr);
}
void DrawMap()
{
	BeginBatchDraw();//开始批量画图
	for (int y = 1; y <= map_row; y++)
	{
		for (int x = 1; x <= map_col; x++)
		{
			if (map[y][x] > 18)
			{
				//标记部分额外处理
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[9]);
				continue;
			}
			switch (map[y][x])
			{
			
			/*
			case -1:	//该处是雷
				putimage(x * 25, y * 25, &imgs[12]);
				break;
			*/
			case 9:		// -1 + 10 = 9 踩到雷了
				putimage((x - 1) *25, (y - 1) *25, &imgs[13]);
				isfail = 1;//失败
				break;
			case 10:	//空白
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[0]);
				break;
			case 11:	//有一个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[1]);
				break;
			case 12:	//有两个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[2]);
				break;
			case 13:	//有三个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[3]);
				break;
			case 14:	//有四个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[4]);
				break;
			case 15:	//有五个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[5]);
				break;
			case 16:	//有六个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[6]);
				break;
			case 17:	//有七个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[7]);
				break;
			case 18:	//有八个雷
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[8]);
				break;
			default:
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[10]);//未翻开 加载对应图象
				break;
			}
		}
	}
	EndBatchDraw();
}



void Play()
{
	MOUSEMSG msg;//鼠标消息
	int row, col;//鼠标点击的行和列
	msg = GetMouseMsg();//获取鼠标点击信息
	row = msg.y / 25 + 1;//精确到鼠标的点击位置
	col = msg.x / 25 + 1;
	switch (msg.uMsg)
	{
	case WM_LBUTTONDOWN://左键按下
		if (map[row][col] >= 9)//翻开过的 和标记的 不进行操作
		{
			break;
		}
		if (map[row][col] == 0)//翻开 0项 和 周围的图片
		{
			Expand(row,col);
		}
		else//翻开点击的图片
		{
			map[row][col] += 10;//翻开 采取的标记是对其+ 10
			times++;	//翻开次数 +1
		}
		break;
	case WM_RBUTTONDOWN://右键按下 WM_RBUTTONDOWN 即对其做标记
		if (map[row][col] >= 9 && map[row][col]<19)//翻开的不进行操作
		{
			break;
		}
		if (map[row][col] >= 19)//标记的取消标记
		{
			map[row][col] -= 20;
		}
		else//标记的方块 采取的记录方式是 +20
		{
			map[row][col] += 20;	//20是随意的数字，只要足够大就行
		}
		break;
	}
	DrawMap();
}

void Judg()
{
	//先查找是否点开了雷
	if (isfail)
	{
		if (MessageBox(GetHWnd(), L"你踩到雷了,是否重新开始本关", L"失败", MB_YESNO) == IDYES)		//GetHWnd()是获取一个顶置窗口
		{
			mine -= 5;
			isfail = 0;
			initmap();
			DrawMap();	//渲染地图
		}
		else
		{
			_exit(0);
		}
	}

	if (times == map_row * map_col - mine)	//通过判断点击的方式确定是否赢得本关游戏
	{
		if (MessageBox(GetHWnd(), L"是否进入下一关", L"成功", MB_YESNO) == IDYES) {
			closegraph();
			//重新开始游戏
			initmap();
			DrawMap();	//渲染地图
		}
		else
			_exit(0);
	}
}

void Expand(int row, int col)
{
	//递归实现以 0 为 九宫格的区域 自动翻开
	map[row][col] += 10;	//翻开当前位置
	times++;				//翻开次数+1
	for (int i = row - 1; i <= row + 1; i++)
	{
		for (int j = col - 1; j <= col + 1; j++)
		{
			//外圈空白区域不操作	仅对可以看到的区域进行递归调用
			if (i >= 1 && i <= map_row && j >= 1 && j <= map_col)
			{
				if (map[i][j] < 9)			//小于9 是未操作，未翻开的区域
				{
					if (map[i][j] == 0)		//0的周围九宫格内无雷，翻开
					{
						Expand(i, j);
					}
					else
					{
						map[i][j] += 10;	//有雷的不翻，让玩家思考是否需要翻开
						times++;
					}
				}
			}
		}
	}
}


/*函数功能 ： 显示版权信息*/
void copyright()
{
	int width,height;
	wchar_t arr[50];
	wsprintf(arr, L"扫雷2.0 GUI Powered by GaoLiZhangQiu");

	loadimage(&imgs[14], _T("D:\\daily\\Project\\素材\\background.png"));
	loadimage(&imgs[15], _T("D:\\daily\\Project\\素材\\cpyright.png"));//加载背景图片
	width = imgs[14].getwidth();
	height = imgs[14].getheight();//获取背景图片的长宽高
	hWnd = initgraph(width,height);//图形界面初始化
	SetWindowText(hWnd, arr);//输出标题信息
	
	BeginBatchDraw();//开始批量画图
	putimage(0, 0, &imgs[14]);
	putimage(350, 0, &imgs[15]);
	EndBatchDraw();



	Sleep(5000);//延时5s进入游戏
	



}

