
// 宏 
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>

#include <stdlib.h>
#include <stddef.h>
#include <math.h> 

#include <olectl.h>
#include <windows.h>
#include <mmsystem.h>	//导入声音头文件
#pragma comment(lib,"Winmm.lib")
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include "imgui.h"
#include <time.h>


#define PI  3.1415926535
#define theta (hookangle*PI/180)
#define MY_ROTATE_TIMER  1                   // 定时器 

//为每个抓取物体赋分 
#define CANDY_SCORE 6
#define BEER_SCORE 4
#define WATERMELON_SCORE 3
#define RICE_SCORE  1
#define MILK_SCORE 4
#define BUBBLE_SCORE 1
#define HEART_SCORE 3
#define APPLE_SCORE 5

enum GAMESTATE                               // 游戏界面 
{
	BEGIN, GAME, RANK, HELP, TXT, END, USER, BOOT, SETTING
};
enum HOOKSTATE                               // 钩子状态 
{
	SWING, SHRINKGET, SHRINKFALL, STRETCH
};
enum GAMEMODE								//游戏情景模式 
{
	FOREST, OCEAN, DESERT
};
enum OBJECT									//物体种类 
{
	CANDY, BEER, WATERMELON, RICE, MILK, BUBBLE, HEART, APPLE
};
enum KINEJUDGE								//抓取种类判断 
{
	 GET_OBJECT, GET_MEDICINE, NONE1
};
enum MEDICINE								//药水种类判断 
{
	SPEEDUP, SLOWDOWN, NONE2
}; 
enum LEVELMODE								//游戏难度模式 
{
	EASY, NORMAL, DIFFICULT
};
enum MUSICCONDITION							//背景音乐播放模式 
{
	MUSICOFF,MUSICON
}; 
enum HELPCONDITION							//游戏帮助页面选择 
{
	PAGE1,PAGE2,PAGE3,PAGE4
}; 
enum TIMERCONDITION							//定时器状态管理 
{
	TIMEROFF,TIMERON
}; 

// 全局变量
// 界面 
double winwidth, winheight;           		// 窗口尺寸
static int    enable_game = 1;				//游戏运行 
static int    show_more_buttons = 0;         // 显示更多按钮
static int    Interface = BOOT;             // 界面 0 BEGIN,1 GAME,2 RANK,3 HELP
static int    Gamemode = DESERT;			//默认游戏情景模式 
static int    Levelmode = NORMAL;			//默认游戏难度模式 
static int    getWhat = NONE1;				//抓取置空 
static int    help_condition=0;				//游戏帮助初始化 
static int    timer_condition=0;			//计时器状态初始化 
// 开机动画 
static double bootlength = 0;
static double bootsize = 0;
static double bootx, booty;
// 钩子
static double hookx, hooky;                  // 钩子的初始位置 

// 形状
static double head = 0.2;                    // 头 
static double eye = 0.02;                    // 眼睛 
// 旋转 
static double hookangle = 0 ;                // 钩子角度
static double angle = 1;                     // 摆动的角度 
static int    flag = 1;                      // 转向 
// 伸缩 
static double hooklength = 0.5;              // 钩子长度 
static double slength = 0.1;                 // 钩子伸缩长度 
static double maxlength = 8;                 // 钩子最大长度 
static double minlength = 0.5;               // 钩子最小长度 

// 物体 
struct object{
	double x,y;                              // 物体坐标 
	double r;                                // 抓取有效范围 
	int    flag;                             // 抓取状态  
    int    score;                            // 分数 
    int    kind;                             // 种类 
    
}object[100];
static int objectNum = 10; 
static int nowNum;                  // 物体数目 
static int getObjectIndex;          // 被抓到的物体编号 
static int getMedicineIndex; 		//被抓到的药水编号 

struct medicine{
	double x,y;
	int    flag;
	int    kind; 
}medicine[100];
double mediciner = 0.2;
static int medicineNum = 2;

struct hookmode{
	double angle;
	double slength;
}hook[3]={
	{1  , 0.08  },
	{1.5  , 0.1 },
	{2.5  , 0.15  }
};
struct colormode{
	char light[20], deep[20];
	char font[20];
	char frame[20],label[20],hotFrame[20],hotLabel[20]; 
}color[3]={
	{
		"Madang", "LightGreen","Shark",
		"Salem","Salem","Shark","Shark"
	},
	{
		"AliceBlue", "JordyBlue","Shark",
		"JacksonPurple","JacksonPurple","Shark","Shark"
	},
	{
		"CREAM", "Dolly","Shark",
		"BurntOrange","BurntOrange","Shark","Shark"
	}
};

//用户名默认随机化处理 
static char *all_name[10]={
	"Dr.Strange","TonyStark","peppapig","Luckydog","ZJUer",
	"Dortmund","MarcoReus","Klose","Kroos","PeterParker"
};

// 初始化 
static int isNormal = 1;                     // 保证在非键盘，鼠标操作时进行状态的更新，否则保持原状态不变 
static int hookState = SWING;                // 钩子的状态，SWING,SHRINKGET,SHRINKFALL,STRETCH 
static int score = 0;                        // 总分 
char stringscore[20] = {'s','c','o','r','e',' ','0','\0'};
static char name[80] = "user";
static int force = 0;						//判断是否强制收缩 
static int win_flag=0; 						//是否播放通关音乐 
static int music_condition=1;				//背景音乐播放状态 
//游戏时间 
clock_t start, end, stop;
clock_t bootstart, bootend;
static double scoretime;		
static double finaltime;
static double boottime;						//开机动画用时 
char stringscoretime[32] = {'t','i','m','e',' ',' ','0','\0'};

//按钮
static int id = 0 ; 
//地图
static int map[30][20]; 

// 函数声明

// 游戏初始化
void init_game();

// 游戏界面
void drawMenu();                             // 菜单 
void drawButtons();                          // 按钮 
void drawRank();                             // 排行榜 
void drawHelp();                             // 帮助 
void drawSetting();							//游戏设置 
void drawBoard();                            // 计分板 
void drawTimeBorad(); 						// 计时板 
void drawStage(); 							// 状态栏 
void GetDoubleStr(double value);			 // 将时间转为可输出字符串 
void drawEditText();						//用户名输入 
void drawUser();							// 游戏设定 

// 钩子
void stretchHook();                          // 伸长钩子 
void shrinkHook();                           // 缩回钩子 
void swingHook();                            // 旋转钩子 
void rot_hook();                             // 改变钩子角度 
void draw_hook();                            // 画钩子                             

// 物体 
void init_object();                          // 初始化物体 
void DrawAllObject();                        // 筛选未被抓取的物体 
void DrawObject(int index);                  // 画物体 
void judgeGet();                             // 判断抓取 
void DrawMedicine(int index);

void drawWatermelon(double x,double y,double r);
void drawApple(double x,double y,double r);
void drawBeer(double x,double y,double r);
void drawHeart(double x,double y,double r);

//背景
//游戏各界面背景管理 
void drawBackground(); 
void drawGameBackground();
void drawBeginBackground();
void drawUserBackground();
void drawHelpBackground();
void drawTxtBackground();
void drawRankBackground();
//开机动画管理 
void stretchBoot();
void biggerBoot();
void drawBootBackground();
void drawBootBackground1();
void drawBootBackground2();

//收缩判断 
void judgeEnd();
void drawEnd();

//排行榜
extern changeScore();
extern initName(); 

// 用户的显示函数
void display(void);

// 用户的字符事件响应函数
void CharEventProcess(char ch)
{
	isNormal = 0 ;
	uiGetChar(ch);    // GUI字符输入
	display();        // 刷新显示
}

// 用户的键盘事件响应函数
void KeyboardEventProcess(int key, int event)
{
	isNormal = 0;
	uiGetKeyboard(key,event);                      // GUI获取键盘
	
	switch(event){
		case KEY_DOWN:
			switch(key){
				case VK_DOWN:                      //向下放钩 
					if(hookState == SWING) 
						hookState = STRETCH;	
					break;
			    case VK_UP:                        //向上缩钩            
			    	if(hookState == STRETCH && score>0) {
			    		score -=1;					//缩钩成功，消耗1点积分 
			    		force = 1;
			    		hookState = SHRINKFALL;
					}
					if(hookState == STRETCH && score<=0) {
			    		force = 2;					//积分不够，缩钩失败 
					}
					break; 
				case VK_RETURN:
					if(Interface == TXT)
					{
						Interface = GAME;
				    	initName(name);
				    	start = clock();
		                init_game();
					}
					break;
			} 
	} 
	display();                                     // 刷新显示
}

// 用户的鼠标事件响应函数
void MouseEventProcess(int x, int y, int button, int event)
{
	isNormal = 0;
	uiGetMouse(x, y, button, event);   // GUI获取鼠标
	display();                         // 刷新显示
}

// 用户的计时器时间响应函数
void TimerEventProcess(int timerID)
{
	isNormal = 1;
	if(timerID == MY_ROTATE_TIMER && enable_game ){
		display();                     // 刷新显示
	}
}

void init_color()
{
	DefineColor("CREAM", 1, 1, 0.8);
	
	//BLUE
	DefineColor("AliceBlue", 0.89,0.95,0.99);
	DefineColor("HummingBird",0.77, 0.937, 0.969);
	DefineColor("JordyBlue",0.54,0.77,0.96);	
	DefineColor("Spray",0.5059,0.8117,0.8784);
		
	DefineColor("Shark",0.004, 0.196, 0.263);
	DefineColor("JacksonPurple",0.122, 0.227, 0.576);


	
	//GREEN
	DefineColor("Madang",0.784,0.968,0.773);
	DefineColor("LightGreen",0.482,0.937,0.698);
	DefineColor("Salem",0.118,0.5098,0.298);

	
	//YELLOW
	DefineColor("WitchHaze",1,0.9647,0.5608);
	DefineColor("Dolly",1,1,0.494);
	DefineColor("BurntOrange",0.827,  0.3294  ,0 );

}

// 用户主程序入口 
// 仅初始化执行一次
void Main() 
{
	PlaySound (TEXT("stage_01.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP|SND_NODEFAULT );
	// 初始化窗口和图形系统
	SetWindowTitle("Pac-Miner");
    InitGraphics();
    //  InitConsole();
    
	// 获得窗口尺寸
    winwidth = GetWindowWidth();
    winheight = GetWindowHeight();
    
    init_color();   //define own colors 
    SetPenSize(2);

	// 注册时间响应函数
	registerCharEvent(CharEventProcess);           // 字符
	registerKeyboardEvent(KeyboardEventProcess);   // 键盘
	registerMouseEvent(MouseEventProcess);         // 鼠标
	registerTimerEvent(TimerEventProcess);         // 定时器
	
	// 初始化物体 
	
	InitRankings();
	bootlength = 0;
	bootx = 0;
	booty = winheight/2;
	bootsize = winheight/10;
	
	setMenuColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);
	setButtonColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0); 
	setTextBoxColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);

	
	// 开启定时器
	startTimer(MY_ROTATE_TIMER, 25);

	//全局变量，界面选择
	Interface = BOOT;
    
	// 打开控制台，方便输出变量信息，便于调试
	 //InitConsole(); 
}

void drawMenu()
{ 
	static char * menuListFile[] = {"设置",  
		 "播放BGM   |  Ctrl-M", 
		 "暂停BGM   |  Ctrl-I" ,
		"退出  | Ctrl-E"};
	static char * menuListGame[] = {"游戏",
		"暂停 | Ctrl-T",
		"重新开始  | Ctrl-O"
		};
	static char * menuListHelp[] = {"查看",
		"帮助  | Ctrl-A"};
	static char * selectedLabel = NULL;

	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH*1.5; // 控件高度
	double w = TextStringWidth(menuListHelp[0])*3; // 控件宽度
	double wlist = TextStringWidth(menuListGame[2])*1.2;
	double xindent = winheight/20; // 缩进
	int    selection;
	
	// menu bar
	drawMenuBar(0,y-h,winwidth,h);
	// File 菜单
	selection = menuList(GenUIID(0), x, y-h, w, wlist, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
	if( selection>0 ) selectedLabel = menuListFile[selection];
	if (selection==1){
		PlaySound (TEXT("stage_01.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP |SND_NODEFAULT);
	}
	if (selection==2){
		PlaySound (NULL,NULL,SND_FILENAME | SND_ASYNC|SND_LOOP |SND_NODEFAULT);
	}
	if( selection==3 )
		exit(-1); // 退出游戏 
	
	// Tool 菜单
	menuListGame[1] = enable_game ? "暂停 | Ctrl-T" : "继续 | Ctrl-T";
	selection = menuList(GenUIID(0),x+w,  y-h, w, wlist,h, menuListGame,sizeof(menuListGame)/sizeof(menuListGame[0]));
	if( selection>0 ) selectedLabel = menuListGame[selection];
	if( selection==1 )
		enable_game = ! enable_game;
	if( selection==2 )
	{
		Interface = BEGIN ;
		init_game();	
	}

	
	// Help 菜单
	selection = menuList(GenUIID(0),x+2*w,y-h, w, wlist, h, menuListHelp,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if( selection>0 ) selectedLabel = menuListHelp[selection];
	if( selection==1 )
		Interface = HELP ;
}

void drawButtons()
{
	double fH = GetFontHeight();
	double h = fH*2;                   // 控件高度
	double w = winwidth/5;             // 控件宽度
	double x = winwidth/2-w/2;  
	double y = winheight/2+2*h; 
	
	if( button(GenUIID(0), x, y, w, h, "游戏开始") ){

	    Interface = USER;
    }
    
	if( button(GenUIID(0), x, y-1.5*h, w, h, "排行榜") )
	{	
		Interface = RANK ;
	}

	if( button(GenUIID(0), x, y-3*h, w, h, "游戏帮助") )
	{	
		Interface = HELP ;
	}

	if( button(GenUIID(0), x, y-6*h, w, h, "退出游戏") )
	{
		exit(-1);
	}

    
	if( button(GenUIID(0), x, y-4.5*h, w, h, "游戏设置") ){
	    Interface = SETTING;
    }	
	
	if(Interface==GAME)
	{	
		initName(name);
		init_game();
	} 
	else if (Interface==RANK)
	{
		drawRank();
	}
	

}

void drawRank()
{
	double fH = GetFontHeight();
	double h = fH*1.5;                // 控件高度
	double w = winwidth/10;           // 控件宽度
	double x = w/4;  
	double y = winheight-1.5*h;
	
	if (button(GenUIID(0), x, y, w, h, "返回"))
	{
		Interface = BEGIN ;
	}
	drawRanking();
}

void drawHelp() 
{
	
	double h = GetFontHeight()*1.5;                // 控件高度
	double w = winwidth/10;           // 控件宽度
	double x = w/4;  
	double y = winheight-1.5*h; 
	double dx=winwidth/10;
	double dy=winheight/13;
	int pointsize;
	pointsize=GetPointSize();
	if (button(GenUIID(0), x, y, w, h, "返回主页面"))
	{
		switch (timer_condition){   //彩蛋会cancel计时器，所以当玩家返回时，重新设定计时器 
			case TIMEROFF:{
					startTimer(MY_ROTATE_TIMER, 25);
					timer_condition=TIMERON;
				break;
			}
		}
		help_condition=PAGE1;
		Interface = BEGIN ;
	}
	x = winwidth/15;
	y = winheight/8*7;
	switch (help_condition){  //判断玩家所处的帮助的界面进行绘制 
		case PAGE1:{
			if (button(GenUIID(0), 0.8*winwidth, 0.1*y, w, h, "下一页")){
				help_condition=PAGE2; 
			}
			SetPointSize(30);
			SetPenColor(color[Gamemode].frame);
			SetStyle(2);
			drawLabel(4*dx,12*dy, "游戏背景");
			SetStyle(0);
			SetPointSize(20);
			SetPenColor(color[Gamemode].font);
			drawLabel(1*dx,11*dy, "在阿拉德大陆的文明之光闪耀之前，世界是一个无边的宇宙，孕育着各种形态的生命。");
			drawLabel(1*dx,10*dy, "这些生命分散在各种虚空异界和地下城堡里，用它们的勤劳和智慧，创建着世界的繁荣。");
			drawLabel(1*dx,9*dy, "在各种虚空异界里，存在一种叫做【生命结晶】的物体，可以使获得者拥有无限的生命。");
			drawLabel(1*dx,8*dy, "生命结晶的出现，引起了异界和天界生命的争夺。那是一场天地变色、伤亡惨重的毁灭之战！"); 
			drawLabel(1*dx,7*dy, "异界进入了强者统治的黑暗期，而阿拉德大陆的繁荣和辉煌，似乎也开始毁灭。");
			drawLabel(1*dx,6*dy, "这是诸神的降罪还是人类毁灭的预兆？未知的天界和魔界是否是这些邪恶力量的诞生之地？") ;
			drawLabel(1*dx,5*dy, "事到如今，唯有冒险家们能为这混乱的世界找到一个解救之道！谁将是对抗邪恶力量的神谕之人？");
			drawLabel(1*dx,4*dy, "尊敬的勇士，请发挥神奇精灵【豆豆】的无上力量，搜集残存的生命结晶，拯救阿拉德大陆吧！"); 
			drawLabel(2*dx,3*dy, "抓准时机按↓来激活豆豆的强劲冲力");
			drawLabel(2*dx,2*dy, "当豆豆吸收所有能量体后即可通关");
			drawLabel(2*dx,1*dy, "魔法世界会随机出现神秘药水，可以激发豆豆的活力！");
			break;
		}
		case PAGE2:{
			if (button(GenUIID(0), 0.8*winwidth, 0.16*y, w, h, "上一页")){
			help_condition=PAGE1; 
			}
			if (button(GenUIID(0), 0.8*winwidth, 0.1*y, w, h, "下一页")){
			help_condition=PAGE3; 
			}
			SetPointSize(30);
			SetPenColor(color[Gamemode].frame);
			SetStyle(2);
			drawLabel(4*dx,12*dy, "游戏说明");
			SetStyle(0);
			SetPointSize(20);
			SetPenColor(color[Gamemode].font);
			drawLabel(1*dx,11*dy, "游戏界面可以分为三大部分：菜单栏、计时计分板;游戏主体与游戏物块;状态栏");
			drawLabel(1*dx,10*dy, "游戏获胜条件是操纵神奇精灵【豆豆】尽快吃掉所有的【生命结晶】");
			drawLabel(1*dx,8*dy, "游戏操作方法：在合适的时机按    ↓  键可以放开【豆豆】(键盘的 向下 按键)");
			drawLabel(1*dx,7*dy, "不同难度模式下，每种【生命结晶】蕴涵的能量不同，计算玩家得分的方式也不同"); 
			drawLabel(1*dx,6*dy, "游戏中会出现特殊道具【狂化药水】与【镇静药水】，可以改变【豆豆】的速度");
			drawLabel(2.5*dx,4.9*dy,"【狂化药水】");
			drawLabel(6.5*dx,4.9*dy,"【镇静药水】");
			drawLabel(1*dx,3.8*dy, "由于药水中含有的神秘能量会被【豆豆】吸收，所以玩家不需要搜集药水，取得药水后也不会加分");
			drawLabel(1*dx,2.8*dy, "玩家也可以通过按下   ↑  (向上按键)燃烧一个积分点数，获取能量，将已经出发的【豆豆】召回"); 
			drawLabel(2*dx,1.8*dy, "当所有的【生命结晶】被获取后，关卡即可结束");
			drawLabel(2*dx,0.8*dy, "时间是绝对流逝的，所以即便玩家选择暂停，计时也不会中断");
			drawWatermelon(1.3*dx,9.4*dy,head);
			drawMilk(2.6*dx,9*dy ,head);
			drawBeer(3.9*dx,9.06*dy,head);
			drawCandy(5.2*dx,9.4*dy,head);
			drawRice(6.5*dx,8.97*dy,head);
			drawMedicine(1.8*dx,5*dy,SPEEDUP);
			drawMedicine(5.7*dx,5*dy,SLOWDOWN);	
			break;
		}
		case PAGE3:{
			if (button(GenUIID(0), 0.8*winwidth, 0.16*y, w, h, "上一页")){
				help_condition=PAGE2; 
			}
			if (button(GenUIID(0), 0.8*winwidth, 0.1*y, w, h, "下一页")){
			help_condition=PAGE4;
			}			
			SetPointSize(30);
			SetPenColor(color[Gamemode].frame);
			SetStyle(2);
			drawLabel(4*dx,12*dy, "游戏秘笈");
			SetStyle(0);
			SetPointSize(22);
			SetPenColor(color[Gamemode].font);
			drawLabel(1*dx,11*dy, "速度过快未必是件好事，因为【豆豆】转动过快会忽视一些角度");
			drawLabel(1*dx,9*dy, "别看收回【豆豆】只需要一个能量点，但是想要取得好名次的话还是不容易的！");
			drawLabel(1*dx,7*dy, "【生命结晶】之所以是现在的样子，是因为某位大能施加法力改变了它的表现形式");
			drawLabel(1*dx,5*dy, "因为人们就不会过多关注这些平凡的物体，所以也大大减小了斗争的可能性"); 
			drawLabel(1*dx,3*dy, "何时服用怎样的药水，是一件值得斟酌的事情");
			break;
		}
		case PAGE4:{
			if (button(GenUIID(0), 0.8*winwidth, 0.16*y, w, h, "上一页")){  //彩蛋会cancel计时器，所以当玩家返回时，重新设定计时器 
				if (timer_condition==TIMEROFF){
					startTimer(MY_ROTATE_TIMER, 25);
					timer_condition=TIMERON;
				}
				
				help_condition=PAGE3; 
			}
			//彩蛋部分：团队成员照片及留言介绍,为了防止卡顿，暂时关闭了定时器。 
			if (button(GenUIID(0), 0.5*winwidth-3*w,0.23*y, w, h, "扶澈.(焦笑然)")){
				timer_condition=TIMEROFF;
				cancelTimer(MY_ROTATE_TIMER);
				DrawPic("jxr.txt",0.1*winwidth,0.85*winheight);
				//个性留言 
				SetPointSize(24);
				SetPenColor(color[Gamemode].frame);
				SetStyle(2);
				drawLabel(0.55*winwidth,0.65*winheight,"\"  我爱知宇和常老师！\"");
			    SetStyle(0);
				SetPointSize(pointsize);
			}
			if (button(GenUIID(0), 0.5*winwidth-0.5*w,0.23*y, w, h, "萝北.(常博宇)")){
				timer_condition=TIMEROFF;
				cancelTimer(MY_ROTATE_TIMER);
				DrawPic("cby.txt",0.06*winwidth,0.8*winheight);
				//个性留言 
				SetPointSize(24);
				SetPenColor(color[Gamemode].frame);
				SetStyle(2);
				drawLabel(0.56*winwidth,0.7*winheight,"\" 当你凝视深渊时");
				drawLabel(0.65*winwidth,0.56*winheight,"深渊也在凝视着你······\"");
			    SetStyle(0);
				SetPointSize(pointsize);	
			}
			if (button(GenUIID(0), 0.5*winwidth+2*w,0.23*y, w, h, "BIU.(张知宇)")){
				timer_condition=TIMEROFF;
				cancelTimer(MY_ROTATE_TIMER);
				//个性留言 
				SetPointSize(24);
				SetPenColor(color[Gamemode].frame);
				SetStyle(2);
				drawLabel(0.55*winwidth,0.65*winheight,"\"  我爱常老师和焦哥哥！\"");
			    SetStyle(0);
				SetPointSize(pointsize);
				DrawPic("zzy.txt",0.1*winwidth,0.86*winheight);
			}
			SetPointSize(30);
			SetPenColor(color[Gamemode].frame);
			SetStyle(2);
			drawLabel(4*dx,12*dy, "关于我们");
			SetStyle(0);
			SetPointSize(22);
			SetPenColor(color[Gamemode].font);
			drawLabel(0.3*winwidth,0.16*y,"凝聚魔法画像会消耗大量时间，请耐心等待");
			drawLabel(0.3*winwidth,0.08*y,"按钮下沉时，请不要触碰键盘和鼠标  若无反应，请等待弹起后重试");
			}
	}
	
	SetPointSize(pointsize);
}
	


void drawUser()
{
	
	double fH1 = GetFontHeight();
	double h1 = fH1*1.5;                // 控件高度
	double w1 = winwidth/10;           // 控件宽度
	double x1 = w1/4;  
	double y1 = winheight-1.5*h1;
	
	if (button(GenUIID(0), x1, y1, w1, h1, "返回"))
	{
		Interface = BEGIN ;
	}
	
	double fH = GetFontHeight();
	double h = fH*2;                   // 控件高度
	double w = winwidth/5;             // 控件宽度
	double x = winwidth/2-w/2;  
	double y = winheight/2+2*h;
	
	SetPenColor("Black");
	drawLabel(x/3, y+1.5*h, "当前选择难度为：");
	switch (Levelmode){
		case NORMAL:{
			drawLabel(x/3+x/4+x/11,y+1.5*h, "普通模式");
			break;
		}
		case DIFFICULT:{
			drawLabel(x/3+x/4+x/11,y+1.5*h, "地狱模式");
			break;
		} 
		case EASY:{
			drawLabel(x/3+x/4+x/11,y+1.5*h, "小白模式");
			break;
		}
	}
		
	if (button(GenUIID(0), x/3, y, w, h, "地狱模式")){
		Levelmode = DIFFICULT;
	}
	
	if (button(GenUIID(0), x/3, y-1.5*h, w, h, "普通模式")){
		Levelmode = NORMAL;
	}
	
	if (button(GenUIID(0), x/3, y-3*h, w, h, "小白模式")){
		Levelmode = EASY;
	}
	
	SetPenColor("Black");
	drawLabel(x*4/3, y+1.5*h, "当前选择模式为：");
	switch (Gamemode){
		case FOREST:{
			drawLabel(x*4/3+x/4+x/11, y+1.5*h, "森林模式");
			break;
		}
		case OCEAN:{
			drawLabel(x*4/3+x/4+x/11, y+1.5*h, "海洋模式");
			break;
		}
		case DESERT:{
			drawLabel(x*4/3+x/4+x/11, y+1.5*h, "沙漠模式");
			break;
		} 
	} 
	//修改各个按钮的颜色配置 
	if (button(GenUIID(0), x*4/3, y, w, h, "森林模式")){
		Gamemode = FOREST;
		setButtonColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0); 
		setTextBoxColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);
		setMenuColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);

	}
	
	if (button(GenUIID(0), x*4/3, y-1.5*h, w, h, "海洋模式")){
		Gamemode = OCEAN;
		setButtonColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0); 
		setTextBoxColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);
		setMenuColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);

	}
	
	if (button(GenUIID(0), x*4/3, y-3*h, w, h, "沙漠模式")){
		Gamemode = DESERT;
		setButtonColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0); 
		setTextBoxColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);
		setMenuColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);

	}
	
	if (button(GenUIID(0), x*4/3, y-7*h, w, h, "游戏开始")){
		Interface = TXT;
		strcpy(name,all_name[rand()%10]); 
		init_object();
		enable_game = 1;
		init_game();
	}
	
}

void drawSetting()
{
	int pointsize;
	pointsize=GetPointSize();
	double fH1 = GetFontHeight();
	double h1 = fH1*1.5;                // 控件高度
	double w1 = winwidth/10;           // 控件宽度
	double x1 = w1/4;  
	double y1 = winheight-1.5*h1;
	
	if (button(GenUIID(0), x1, y1, w1, h1, "返回"))
	{
		Interface = BEGIN ;
	}
	
	double fH = GetFontHeight();
	double h = fH*2;                   // 控件高度
	double w = winwidth/5;             // 控件宽度
	double x = winwidth/2-w/2;  
	double y = winheight/2+2*h;
	
	switch (music_condition){
		case MUSICOFF:{
			if( button(GenUIID(0), x, y-1.5*h, w, h, "播放背景音乐") ){
				PlaySound (TEXT("stage_01.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP |SND_NODEFAULT);
				music_condition=1;
			}
			break;
		}
		case MUSICON:{
			if( button(GenUIID(0), x, y-1.5*h, w, h, "暂停背景音乐") ){
				PlaySound (NULL,NULL,SND_FILENAME | SND_ASYNC|SND_LOOP |SND_NODEFAULT);
				music_condition=0;
			break;
			}
		}
		
	
	}
} 

void display()
{
	// 清屏
	DisplayClear();
	
    switch(Interface){
    	case BEGIN:{
	   		drawBackground();
    		drawButtons();
    		break;
    	} 
    	case GAME:{ 
    		drawBackground();
    		drawMenu();
    		drawBoard();
    		switch(hookState){
    			case SWING:{ 
    				DrawAllObject();
    				swingHook();
    				break;
    			} 
    			case STRETCH:{ 
    				DrawAllObject();
    				judgeGet();
    				stretchHook();
    				draw_hook();
    				break;
    			}	
    			case SHRINKGET:{ 
    				shrinkHook();
    				draw_hook();
    				DrawAllObject();
    				break;
    			}	
    			case SHRINKFALL:{ 
    				DrawAllObject();
    				shrinkHook();
    				draw_hook();
    				break;
    			}	
    			default:
    				break;
			}
			drawStage();
			break;
		} 
			
		case HELP:{ 
			drawBackground();
			drawHelp();
			break;
		} 
		case RANK:{ 
			drawBackground();
			drawRank();
			break;
		} 
		case TXT:{ 
			drawBackground();
			drawEditText();
			break;
		} 
		case USER:{ 
			drawBackground();
			drawUser();
			break;
		} 
		case SETTING:{ 
			drawBackground();
			drawSetting();
			break;
		} 
		case END:{ 
			//游戏结束以后可以添加一个短暂的庆祝界面 
			drawBackground();
			if (win_flag==0){
				PlaySound (TEXT("win.wav"),NULL,SND_FILENAME |  SND_ASYNC |SND_NODEFAULT );
				win_flag=1;
			}
	
			drawEnd();
			Pause(3);
			PlaySound (TEXT("stage_01.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP |SND_NODEFAULT);
			
			changeScore(score);
			changeTime((int)finaltime);
			rankChange();
			Interface = RANK;
			break;
		} 
		case BOOT:{ 
			drawBackground();
			if(bootlength<=winwidth) stretchBoot();
			else biggerBoot();
			if(bootsize>=winheight/2) Interface = BEGIN; 
			break;
		} 
		default:
			break;
	}

}
//游戏中，底部状态栏绘制 
void drawStage()
{
	double _pensize;
	string ccolor;
	ccolor=GetPenColor();
	_pensize=GetPenSize();
	SetPenSize(1);	
	MovePen(0,0.045*winheight);
	
	DrawLine(winwidth,0);
	SetPenColor("white");
	drawBox(-1,0,1.2*winwidth,0.043*winheight,1,"","L","white");
	SetPenColor(ccolor);
	switch(hookState){
		case SHRINKGET:{
			drawLabel(0.025*winwidth,0.015*winheight,"当前状态：成功抓到了");
			switch (getWhat){
				case GET_OBJECT:{
					switch(getObjectIndex){
						case  BEER:{
	 						drawLabel(0.19*winwidth,0.015*winheight,"【啤酒】！");
							break;
							}
						case WATERMELON:{
							drawLabel(0.19*winwidth,0.015*winheight,"【西瓜】！");
							break;
							}
						case RICE:{
							drawLabel(0.19*winwidth,0.015*winheight,"【饭团】！");
							break;
							} 
						case MILK:{
							drawLabel(0.19*winwidth,0.015*winheight,"【牛奶】！");
							break;
							}	
						case APPLE:{
							drawLabel(0.19*winwidth,0.015*winheight,"【苹果】！");
							break;
							}
						case CANDY:{
							drawLabel(0.19*winwidth,0.015*winheight,"【冰糖葫芦】！");
							break;
							}
			 			} 
			 		case GET_MEDICINE:{
					 	switch(getMedicineIndex){
							case SPEEDUP:{
								drawLabel(0.19*winwidth,0.015*winheight,"【狂化药水】！行动速度提升！");
								break;
								}
							case SLOWDOWN:{
								drawLabel(0.19*winwidth,0.015*winheight,"【镇静药水】！行动速度下降！");
								break;
								} 
						}break;
					 }
				 }
			}
		}	
		case SHRINKFALL:{
			if (getWhat==NONE1){
				drawLabel(0.025*winwidth,0.015*winheight,"当前状态：未抓取"); 
				if (force==1){
					drawLabel(0.4*winwidth,0.015*winheight,"您燃烧一个积分收回豆豆"); 
				}
				
			}
			
			break;
		} 
		case SWING:{
			drawLabel(0.025*winwidth,0.015*winheight,"当前状态：摆动中"); 
			
			break;
		}
		case STRETCH:{
			drawLabel(0.025*winwidth,0.015*winheight,"当前状态：正在尝试抓取"); 
			if (force==2){
					SetPenColor("red");
					drawLabel(0.4*winwidth,0.015*winheight,"积分不足，无法回收豆豆");
					SetPenColor(ccolor);	
				}
			break;
		}
	}	
	switch (Levelmode){
		case EASY:{
			drawLabel(0.8*winwidth,0.015*winheight,"当前难度：小白级");
			break;
		}
		case NORMAL:{
			drawLabel(0.8*winwidth,0.015*winheight,"当前难度：普通级");
			break;
		}
		case DIFFICULT:{
			drawLabel(0.8*winwidth,0.015*winheight,"当前难度：地狱级");
			break;
		}
	}
	SetPenSize(_pensize);
	SetPenColor(ccolor);
}

//各个不同的背景绘制 
void drawBackground()
{
	switch (Interface)
	{
		case BEGIN:
			drawBeginBackground();
			break;
		case GAME:
			drawGameBackground();
			break;
		case USER:
			drawUserBackground();
			break;
		case RANK: case END:
			drawRankBackground();
			break;
		case HELP:
			drawHelpBackground();
			break;
		case SETTING:
			drawHelpBackground();
		case TXT:
			drawTxtBackground();
			break;
		case BOOT:
			drawBootBackground();
			break;
		default:
			break;
	}
}
//绘制游戏背景 
void drawGameBackground()
{
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(0, 0); 
	DrawLine(winwidth, 0);
	DrawLine(0, winheight);
	DrawLine(-winwidth, 0);
	DrawLine(0, -winheight);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(winwidth/2-winheight/2, winheight);
	DrawArc(winheight/2, 180, 180);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(winwidth/2-winheight/4+winheight/16, winheight-winheight/4);
	DrawArc(winheight/16, 0, 360);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(winwidth/2+winheight/4+winheight/16, winheight-winheight/4);
	DrawArc(winheight/16, 0, 360);
	EndFilledRegion();
}
//绘制开始界面背景 
void drawBeginBackground()
{
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(0, 0); 
	DrawLine(winwidth, 0);
	DrawLine(0, winheight);
	DrawLine(-winwidth, 0);
	DrawLine(0, -winheight);
	EndFilledRegion();	
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(winheight*(0.5+pow(2, 0.5)/4), winheight*(0.5+pow(2, 0.5)/4)); 
	DrawArc(winheight/2, 45, 270);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(winheight/2, winheight/2); 
	DrawLine(winheight/pow(8, 0.5), winheight/pow(8, 0.5));
	DrawLine(0, -winheight/pow(2, 0.5));
	DrawLine(-winheight/pow(8, 0.5), winheight/pow(8, 0.5));
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(winheight/4+winheight/16, winheight/2); 
	DrawArc(winheight/16, 0, 360);
	EndFilledRegion();
}
//绘制设置界面背景 
void drawUserBackground()
{
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(0, 0); 
	DrawLine(winwidth, 0);
	DrawLine(0, winheight);
	DrawLine(-winwidth, 0);
	DrawLine(0, -winheight);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(winwidth, winheight/2);
	DrawArc(winheight/2, 0, 360);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(winwidth-winheight/4+winheight/16, winheight/2);
	DrawArc(winheight/16, 0, 360);
	EndFilledRegion();
	
	SetPenSize(10);
	MovePen(winwidth-winheight, winheight/2);
	DrawLine(winheight*3/8, 0);
}
//设置排行榜界面背景 
void drawRankBackground()
{
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(0, 0); 
	DrawLine(winwidth, 0);
	DrawLine(0, winheight);
	DrawLine(-winwidth, 0);
	DrawLine(0, -winheight);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(winwidth/2+winheight/2, 0);
	DrawArc(winheight/2, 0, 180);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(winwidth/2+winheight/2/pow(2, 0.5), winheight/2/pow(2, 0.5));
	DrawLine(winheight/3/pow(2, 0.5), winheight/3/pow(2, 0.5));
	DrawLine(-winheight/2/pow(2, 0.5)-winheight/3/pow(2, 0.5)+winheight/6, 0);
	DrawLine(-winheight/6, winheight/6*pow(3, 0.5));
	DrawLine(-winheight/6, -winheight/6*pow(3, 0.5));
	DrawLine(-winheight/2/pow(2, 0.5)-winheight/3/pow(2, 0.5)+winheight/6, 0);
	DrawLine(winheight/3/pow(2, 0.5), -winheight/3/pow(2, 0.5));
	DrawLine(winheight/pow(2, 0.5), 0);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(winwidth/2+winheight/2/pow(2, 0.5)+winheight/3/pow(2, 0.5)+winheight/32+winheight/15, winheight/2/pow(2, 0.5)+winheight/3/pow(2, 0.5));
	DrawArc(winheight/30, 0, 360);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(winwidth/2-winheight/2/pow(2, 0.5)-winheight/3/pow(2, 0.5)-winheight/32, winheight/2/pow(2, 0.5)+winheight/3/pow(2, 0.5));
	DrawArc(winheight/30, 0, 360);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(winwidth/2+winheight/30, winheight/2/pow(2, 0.5)+winheight/3/pow(2, 0.5)+winheight/6*pow(3, 0.5)+winheight/16);
	DrawArc(winheight/30, 0, 360);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(winwidth/2-winheight/4+winheight/16, winheight/4);
	DrawArc(winheight/16, 0, 360);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(winwidth/2+winheight/4+winheight/16, winheight/4);
	DrawArc(winheight/16, 0, 360);
	EndFilledRegion();
}
//绘制游戏帮助界面背景 
void drawHelpBackground()
{
	int i;
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(0, 0); 
	DrawLine(winwidth, 0);
	DrawLine(0, winheight);
	DrawLine(-winwidth, 0);
	DrawLine(0, -winheight);
	EndFilledRegion();	
	
	for(i=0; i<4; i++){
		
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(i*winwidth/4+winheight/3*(0.5+pow(2, 0.5)/4), i*winheight/4+winheight/3*(0.5+pow(2, 0.5)/4)); 
	DrawArc(winheight/6, 45, 270);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(i*winwidth/4+winheight/3/2, i*winheight/4+winheight/3/2); 
	DrawLine(winheight/3/pow(8, 0.5), winheight/3/pow(8, 0.5));
	DrawLine(0, -winheight/3/pow(2, 0.5));
	DrawLine(-winheight/3/pow(8, 0.5), winheight/3/pow(8, 0.5));
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(i*winwidth/4+winheight/3/4+winheight/3/16, i*winheight/4+winheight/3/2); 
	DrawArc(winheight/3/16, 0, 360);
	EndFilledRegion();
	}
}
//绘制文本输入背景 
void drawTxtBackground()
{
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(0, 0); 
	DrawLine(winwidth, 0);
	DrawLine(0, winheight);
	DrawLine(-winwidth, 0);
	DrawLine(0, -winheight);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(winwidth-winheight*(0.5+pow(2, 0.5)/4), winheight*(0.5-pow(2, 0.5)/4)); 
	DrawArc(winheight/2, -135, 270);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(winwidth/4, winheight/4);
	DrawLine(winheight/16, 0);
	DrawLine(0, winheight*2/3);
	DrawLine(-winheight/16, 0);
	DrawLine(0, -winheight*2/3);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(winwidth/4, winheight/4);
	DrawLine(winheight/32, -winheight/32);
    DrawLine(winheight/32, winheight/32);
    DrawLine(-winheight/16, 0);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(winwidth-winheight*(0.5+pow(2, 0.5)/4), winheight*(0.5-pow(2, 0.5)/4)); 
	DrawLine(winheight/pow(8, 0.5), winheight/pow(8, 0.5));
	DrawLine(-winheight/pow(8, 0.5), winheight/pow(8, 0.5));
	DrawLine(0, -winheight/pow(2, 0.5));
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(winwidth-winheight/4+winheight/16, winheight/2); 
	DrawArc(winheight/16, 0, 360);
	EndFilledRegion();
	
	
}
//绘制开场动画 
void stretchBoot()
{
	if(isNormal)bootlength += winwidth/100;
}

void biggerBoot()
{
	if(isNormal) bootsize += winheight/100;
}

void drawBootBackground()
{
	if(bootlength<=winwidth) drawBootBackground1();
	else drawBootBackground2();
}

void drawBootBackground1()
{
	double r = winheight/10;
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(0, 0); 
	DrawLine(winwidth, 0);
	DrawLine(0, winheight);
	DrawLine(-winwidth, 0);
	DrawLine(0, -winheight);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(bootx+bootlength+r/pow(2, 0.5), booty+r/pow(2, 0.5)); 
	DrawArc(r, 45, 270);
    EndFilledRegion();
	
    SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(bootx+bootlength, booty);
	DrawLine(r/pow(2, 0.5), r/pow(2, 0.5));
	DrawLine(0, -r*pow(2, 0.5));
	DrawLine(-r/pow(2, 0.5), r/pow(2, 0.5));
	EndFilledRegion();
	
	StartFilledRegion(1);
	MovePen(bootx+bootlength-r/2+r/5, booty); 
	DrawArc(r/5, 0, 360);
	EndFilledRegion();
}

void drawBootBackground2()
{
	double dx = winheight/20;
	bootx = winwidth/2;
	booty = winheight/2;
	
	SetPenColor(color[Gamemode].deep);
	StartFilledRegion(1);
	MovePen(0, 0); 
	DrawLine(winwidth, 0);
	DrawLine(0, winheight);
	DrawLine(-winwidth, 0);
	DrawLine(0, -winheight);
	EndFilledRegion();
	
	SetPenColor(color[Gamemode].light);
	StartFilledRegion(1);
	MovePen(bootx+bootsize, booty); 
	DrawArc(bootsize, 0, 360);
    EndFilledRegion();
	
	SetPenSize(15);
    SetPenColor(color[Gamemode].deep);
	MovePen(bootx-bootsize*2/3-bootsize/6+dx, booty+bootsize/6*pow(3, 0.5));
	DrawLine(bootsize/3, 0);
	DrawLine(-bootsize/3, -bootsize/3*pow(3, 0.5));
	DrawLine(bootsize/3, 0);
	
	MovePen(bootx-bootsize/4+dx, booty+bootsize/6*pow(3, 0.5));
	DrawLine(bootsize*1.7/4, 0);
	MovePen(bootx+dx, booty+bootsize/6*pow(3, 0.5));
	DrawLine(0, -bootsize/3*pow(3, 0.5)+bootsize/6);
	MovePen(bootx-bootsize/3+dx, booty+bootsize/6*pow(3, 0.5)-bootsize/3*pow(3, 0.5)+bootsize/6);
	DrawArc(bootsize/6, 180, 180);
	
	MovePen(bootx+bootsize/3+dx, booty+bootsize/6*pow(3, 0.5));
	DrawLine(0, -bootsize/3*pow(3, 0.5)+bootsize/5);
	DrawArc(bootsize/6, 180, 180);
	DrawLine(0, bootsize/3*pow(3, 0.5)-bootsize/5);
}
//初始化游戏参数 
void init_game()
{
	enable_game = 1;
	hookangle = 0;
	flag = 1;
	hookState = 0;
	hooklength = minlength;
	score = 0;
	slength = hook[Levelmode].slength; 
	angle = hook[Levelmode].angle;
	init_object(); 
	win_flag=0;	
}


//初始化物块属性 
void init_object()
{
	int i,nx,ny;
	int total;
	switch (Levelmode){
		case EASY:{
			total=5;
			break;
		}
		case NORMAL:{
			total = 7;
			break;
		}
		case DIFFICULT:{
			total = 9;
			break;
		}
	}
	
	memset(map,0,sizeof(map));
	
	for(i=0; i<total; i++){
		nx = rand()%(int)(0.8*winwidth*2.2);
		ny = rand()%(int)(0.72*winheight*2.2);
		
		while( map[nx][ny] || ny<=1 )
		{
			nx = rand()%(int)(0.8*winwidth*2.2);
			ny = rand()%(int)(0.72*winheight*2.2);
		}
		map[nx][ny]=1;
		object[i].r = head;
		object[i].x = (nx+1)*0.5+rand()%8*0.04;
		object[i].y = (ny)*0.5+rand()%7*0.04;
		object[i].flag = 1;      // 表示物体均未被抓走 
		object[i].kind = i%5;

	}
	objectNum = i;
	nowNum = i; 
	for(i=0; i<3; i++){
		medicine[i].flag=1;
		nx = rand()%(int)(0.8*winwidth*2.2);
		ny = rand()%(int)(0.72*winheight*2.2);
		
		while(map[nx][ny]||ny<=1)
		{
			nx = rand()%(int)(0.8*winwidth*2.2);
			ny = rand()%(int)(0.72*winheight*2.2);
		}
		map[nx][ny]=1;
		medicine[i].x = (1+nx)*0.5+rand()%8*0.04;
		medicine[i].y = (ny)*0.5+rand()%7*0.04;
		
		if(i<medicineNum/2){
			medicine[i].kind = i%2;
		}
		else{
			medicine[i].kind = i%2;
		}
	}
	medicineNum = i;


}
//核心函数：抓取判断处理 
void judgeGet()
{
	double x, y;
	int i;
	x = hookx+cos(hookangle*PI/180)*hooklength;       //钩子目前坐标 
	y = hooky+sin(hookangle*PI/180)*hooklength;
	
	double distance;
	
	for (i=0; i<objectNum; i++)
	{
		if( !object[i].flag ){
			continue;
		}
		distance = sqrt( pow(x-object[i].x, 2) + pow(y-object[i].y, 2));       //计算钩子与物体重心距离 
		if(distance <= object[i].r+head){
			hookState = SHRINKGET;
			getWhat=GET_OBJECT;
			getObjectIndex = object[i].kind;
			switch (Levelmode){
				case EASY:{
					score += 3;
					break;
				}
				case NORMAL:{
					score += object[i].score+2;
					break;
				}
				case DIFFICULT:{
					score += 2*object[i].score;
					break;
				}
			}
			
			
			object[i].flag = 0;
			nowNum--;
			return ;
		}
	}
	
	for (i=0; i<medicineNum; i++)
	{
		if( !medicine[i].flag ){
			continue;
		}
		distance = sqrt( pow(x-medicine[i].x, 2) + pow(y-medicine[i].y, 2));       //计算钩子与物体重心距离 
		if(distance <= mediciner+head){
			getWhat=GET_MEDICINE;
			hookState = SHRINKGET;
			getMedicineIndex = medicine[i].kind;
			if(medicine[i].kind==SLOWDOWN){
				if(slength >= 1)slength -= 0.05;
				if(angle >= 1)angle -= 0.5; 
			}
			else{
				if(slength) slength += 0.05;
				if(angle) angle += 1; 
			}
			medicine[i].flag = 0;
			//nowmeNum -- ;
		}
	}

}

//动画：钩子伸长 
void stretchHook()
{
	if(!isNormal){
		return;
	}
	if(hooklength >= maxlength){
		hookState = SHRINKFALL;
		hooklength = maxlength; 
	}
	else{
		hooklength += slength;
	}
}
//动画：钩子缩回 
void shrinkHook()
{
	if(!isNormal){
		return;
	}
	if(hooklength <= minlength){
		hookState = SWING;
		hooklength = minlength; 
		getWhat=NONE1;
		getObjectIndex=NONE2;
		getMedicineIndex=NONE2;
		judgeEnd();
	}
	else{
		hooklength -= slength;
	}
	
}

//游戏结束判断 
void judgeEnd()
{
	force = 0;
	if( nowNum <=0 ) 
		
		Interface = END ; 
}
//修改摆动角度 
void rot_hook()
{
	if(hookangle <= -180 || hookangle >= 0){      //0与-180度时进行转向 
		flag *= -1;
	}
    hookangle = hookangle + flag*angle;
}

//绘画：钩子 
void draw_hook()
{
	double dx, dy;
	double centrex, centrey;
	hookx = winwidth/2;
	hooky = 0.9*winheight-0.1-head;
	dx = hooklength*cos(theta);
	dy = hooklength*sin(theta);
	centrex = hookx+dx;
	centrey = hooky+dy;
	SetPenSize(1);
	
	switch(hookState){
		case SWING:{ 
			SetPenColor("Yellow");
			dx = (hooklength+head)*cos(theta);
	        dy = (hooklength+head)*sin(theta);
			MovePen(hookx+dx, hooky+dy);
			StartFilledRegion(0.7);
			DrawArc(head, hookangle, 360);
			EndFilledRegion();
			
			SetPenColor("Black");
    		dx = minlength*cos(theta);
			dy = minlength*sin(theta);
			MovePen(hookx+dx+head/2*sin(theta), hooky+dy-head/2*cos(theta));
			StartFilledRegion(1);
			DrawArc(eye, hookangle, 360);
			EndFilledRegion();
			MovePen(hookx+dx-head/2*sin(theta), hooky+dy+head/2*cos(theta));
			StartFilledRegion(1);
			DrawArc(eye, hookangle, 360);
			EndFilledRegion();
	
            SetPenSize(2);
	        dx = (minlength+head)*cos(theta);
	        dy = (minlength+head)*sin(theta);
	        MovePen(hookx+dx, hooky+dy);
	        DrawArc(head, hookangle, 360);
	        
			break;
		} 
        case STRETCH:{ 
        	SetPenColor("Green");
	    	StartFilledRegion(0.65);
	    	MovePen(centrex+head*cos((hookangle+60)*PI/180), centrey+head*sin((hookangle+60)*PI/180));
	    	DrawArc(head, hookangle+60, 240);
	    	EndFilledRegion();
	    	
	    	if(hooklength>=winheight/2-0.1*winheight-0.1-head){
	    		SetPenColor(color[Gamemode].deep);
			}
	    	else{
	    		SetPenColor(color[Gamemode].light);
			}
	    	StartFilledRegion(1);
	    	DrawLine(-head*cos((hookangle-60)*PI/180), -head*sin((hookangle-60)*PI/180));
	        DrawLine(head*cos((hookangle+60)*PI/180), head*sin((hookangle+60)*PI/180));
         	DrawLine(head*(cos((hookangle+60)*PI/180)-cos((hookangle-60)*PI/180)), head*(sin((hookangle+60)*PI/180)-head*sin((hookangle-60)*PI/180)));
	    	EndFilledRegion();
	
    	    SetPenColor("Black");
	    	StartFilledRegion(1);
	    	MovePen(centrex-head*cos(theta)/2, centrey-head*sin(theta)/2);
	    	DrawArc(eye, hookangle, 360);
	    	EndFilledRegion();

	    	SetPenSize(2);
	    	MovePen(centrex+head*cos((hookangle+60)*PI/180), centrey+head*sin((hookangle+60)*PI/180));
	    	DrawArc(head, hookangle+60, 240);
	    	DrawLine(-head*cos((hookangle-60)*PI/180), -head*sin((hookangle-60)*PI/180));
	    	DrawLine(head*cos((hookangle+60)*PI/180), head*sin((hookangle+60)*PI/180));

            break;
        } 
        case SHRINKGET:{ 
	    	SetPenColor("Red");
	    	StartFilledRegion(0.6);
	    	MovePen(centrex+head*cos((hookangle-120)*PI/180), centrey+head*sin((hookangle-120)*PI/180));
	    	DrawArc(head, hookangle-120, 240);
	    	EndFilledRegion();
	
	    	if(hooklength<=winheight/2-0.1*winheight-0.1-head){
	    		SetPenColor(color[Gamemode].light);
			}
	    	else{
	    		SetPenColor(color[Gamemode].deep);
			}
	    	StartFilledRegion(1);
	    	DrawLine(-head*cos((hookangle+120)*PI/180), -head*sin((hookangle+120)*PI/180));
	    	DrawLine(head*cos((hookangle-120)*PI/180), head*sin((hookangle-120)*PI/180));
	    	DrawLine(head*(cos((hookangle+120)*PI/180)-cos((hookangle-120)*PI/180)), head*(sin((hookangle+120)*PI/180)-head*sin((hookangle-120)*PI/180)));
	    	EndFilledRegion();
	
	    	SetPenColor("Black");
	    	StartFilledRegion(1);
	    	MovePen(centrex+head*cos(theta)/2, centrey+head*sin(theta)/2);
	    	DrawArc(eye, hookangle, 360);
	    	EndFilledRegion();
	
	    	SetPenColor("Black");
	    	StartFilledRegion(1);
	    	MovePen(centrex-head*cos(theta)/3, centrey-head*sin(theta)/3);
	    	DrawArc(2.5*eye, hookangle, 360);
	    	EndFilledRegion();
	
	    	SetPenSize(2);
	    	MovePen(centrex+head*cos((hookangle-120)*PI/180), centrey+head*sin((hookangle-120)*PI/180));
	    	DrawArc(head, hookangle-120, 240);
	    	DrawLine(-head*cos((hookangle+120)*PI/180), -head*sin((hookangle+120)*PI/180));
	    	DrawLine(head*cos((hookangle-120)*PI/180), head*sin((hookangle-120)*PI/180));

	    	break;
		} 
	    case SHRINKFALL:{ 
	    	SetPenColor("Blue");
	    	StartFilledRegion(0.6);
	    	MovePen(centrex+head*cos(theta), centrey+head*sin(theta));
	    	DrawArc(head, hookangle, 360);
	    	EndFilledRegion();
	
	    	SetPenColor("Black");
	    	StartFilledRegion(1);
	    	MovePen(centrex+head*cos(theta)/2, centrey+head*sin(theta)/2);
	    	DrawArc(eye, hookangle, 360);
	    	EndFilledRegion();
	
	    	SetPenSize(2);
	    	MovePen(centrex+head*cos(theta), centrey+head*sin(theta));
	    	DrawArc(head, hookangle, 360);
	    	MovePen(centrex-head*cos(theta), centrey-head*sin(theta));
	    	DrawLine(head*cos(theta)*2/3, head*sin(theta)*2/3);

            break;
        } 
        default:
        	break;
	}
}
//动画：钩子摆动 
void swingHook()
{
	if(isNormal){
		rot_hook();
	}
	draw_hook();
}
//绘画：展示未抓取的全部物体 
void DrawAllObject()
{
	int i;
	for(i=0; i<objectNum; i++ ){
		if( object[i].flag ){       //判断物体是否已被抓走 
			DrawObject(i);
		}
	}
	for(i=0; i<medicineNum; i++ ){
	if( medicine[i].flag ){       //判断物体是否已被抓走 
			DrawMedicine(i);
		}
	}
}

void DrawMedicine(int index)
{
	double x = medicine[index].x;
	double y = medicine[index].y;
	int    i = medicine[index].kind;
	drawMedicine(x,y,i);
	
}
//绘画：单个物体 
void DrawObject(int index)
{
	double x = object[index].x;
	double y = object[index].y;
	double r = object[index].r;
	
	switch (object[index].kind)	{
		case APPLE:{
			object[index].score=APPLE_SCORE;
			drawApple(x,y,r);
			break;
		}
			
		case WATERMELON:{
			object[index].score=WATERMELON_SCORE;
			drawWatermelon(x,y,r);
			break;
		}
		
		case BEER:{
			object[index].score=BEER_SCORE;
			drawBeer(x,y,r);
			break;
		}
	
		case HEART:{
			object[index].score=HEART_SCORE;
			drawHeart(x,y,r);	
			break;
		}
		
		case BUBBLE:{
			object[index].score=BUBBLE_SCORE;
			drawBubble(x,y,r);
			break;
		}
		
		case MILK:{
			object[index].score=MILK_SCORE;
			drawMilk(x,y,r);
			break;
		}
		
		case CANDY:{
			object[index].score=CANDY_SCORE;
			drawCandy(x,y,r);	
			break;
		}

		case RICE:{
			object[index].score=RICE_SCORE;
			drawRice(x,y,r);
			break;
		}
		
			
		default:
			
			break; 
		
	}
}

//结束界面展示 
void drawEnd()
{
	int ___pensize; 
	___pensize=GetPointSize();
	SetPointSize(20);
	SetPenColor(color[Gamemode].frame);
	drawLabel(0.45*winwidth, 0.6*winheight,"You win!");
	drawLabel(0.45*winwidth, 0.5*winheight,stringscore);
	drawLabel(0.45*winwidth, 0.45*winheight,stringscoretime);
	SetPointSize(___pensize);
	finaltime = scoretime; 
	Interface = RANK;
}

//绘制：用户名输入界面 
void drawEditText()
{
	double fH1 = GetFontHeight();
	double h1 = fH1*1.5;                // 控件高度
	double w1 = winwidth/10;           // 控件宽度
	double x1 = w1/4;  
	double y1 = winheight-1.5*h1;
	

	
	if (button(GenUIID(0), x1, y1, w1, h1, "返回"))
	{
		Interface = BEGIN ;
	}

	static char results[200] = "";
	static char memo[80]="Welcome! Please enter your name.";
	double fH = GetFontHeight();
	double h = fH*2; // 控件高度
	double w = winwidth/4; // 控件宽度
	double x = winwidth/2.5;
	double y = winheight/1.7;

	drawLabel(x, y, memo);

	SetPenColor("Brown");
	drawLabel(x-fH/2-TextStringWidth("Enter here:"), (y-=h*1.2)+fH*0.7, "Enter here:");
	if(textbox(GenUIID(0), x, y, w, h, name, sizeof(name)))
		sprintf(results,"Text edit result is: %s", name);

	SetPenColor("Gray"); 
	drawLabel(x, y-=fH*1.2, results);

	enable_game = 1;
	
}
//计时板、计分板绘制 
void drawBoard()
{
	
	stringscore[6] = score/100+'0';
    stringscore[7] = score%100/10+'0';
    stringscore[8] = score%10+'0';
    
	end = clock();
	scoretime = (double)(end-start)/CLK_TCK;
	
	stringscoretime[6] = (int)scoretime/100+'0';
	stringscoretime[7] = (int)scoretime%100/10+'0';
	stringscoretime[8] = (int)scoretime%10+'0';

	drawBox(0.9*winwidth, 0.9*winheight, 0.1*winwidth, 0.05*winheight, 0, stringscore, "M", color[Gamemode].frame);
	drawBox(0.9*winwidth, 0.85*winheight, 0.1*winwidth, 0.05*winheight, 0, stringscoretime, "M", color[Gamemode].frame);

}
//将double型时间转化为字符串格式 
void GetDoubleStr(double value)
{
	int i, index = 0;
	sprintf(stringscoretime, "%.8f", value);
	int len = strlen(stringscoretime);
	for(i=5+len-1; i>5; i--){
		if(stringscoretime[i] == '0')
		    continue;
		else{
			if(stringscoretime[i] == '.') index = i;
			else index = i+1;
			break;
		}
	}
	stringscoretime[index] = '\0';
}

