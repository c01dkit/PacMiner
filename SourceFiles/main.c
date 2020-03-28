
// �� 
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
#include <mmsystem.h>	//��������ͷ�ļ�
#pragma comment(lib,"Winmm.lib")
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include "imgui.h"
#include <time.h>


#define PI  3.1415926535
#define theta (hookangle*PI/180)
#define MY_ROTATE_TIMER  1                   // ��ʱ�� 

//Ϊÿ��ץȡ���帳�� 
#define CANDY_SCORE 6
#define BEER_SCORE 4
#define WATERMELON_SCORE 3
#define RICE_SCORE  1
#define MILK_SCORE 4
#define BUBBLE_SCORE 1
#define HEART_SCORE 3
#define APPLE_SCORE 5

enum GAMESTATE                               // ��Ϸ���� 
{
	BEGIN, GAME, RANK, HELP, TXT, END, USER, BOOT, SETTING
};
enum HOOKSTATE                               // ����״̬ 
{
	SWING, SHRINKGET, SHRINKFALL, STRETCH
};
enum GAMEMODE								//��Ϸ�龰ģʽ 
{
	FOREST, OCEAN, DESERT
};
enum OBJECT									//�������� 
{
	CANDY, BEER, WATERMELON, RICE, MILK, BUBBLE, HEART, APPLE
};
enum KINEJUDGE								//ץȡ�����ж� 
{
	 GET_OBJECT, GET_MEDICINE, NONE1
};
enum MEDICINE								//ҩˮ�����ж� 
{
	SPEEDUP, SLOWDOWN, NONE2
}; 
enum LEVELMODE								//��Ϸ�Ѷ�ģʽ 
{
	EASY, NORMAL, DIFFICULT
};
enum MUSICCONDITION							//�������ֲ���ģʽ 
{
	MUSICOFF,MUSICON
}; 
enum HELPCONDITION							//��Ϸ����ҳ��ѡ�� 
{
	PAGE1,PAGE2,PAGE3,PAGE4
}; 
enum TIMERCONDITION							//��ʱ��״̬���� 
{
	TIMEROFF,TIMERON
}; 

// ȫ�ֱ���
// ���� 
double winwidth, winheight;           		// ���ڳߴ�
static int    enable_game = 1;				//��Ϸ���� 
static int    show_more_buttons = 0;         // ��ʾ���ఴť
static int    Interface = BOOT;             // ���� 0 BEGIN,1 GAME,2 RANK,3 HELP
static int    Gamemode = DESERT;			//Ĭ����Ϸ�龰ģʽ 
static int    Levelmode = NORMAL;			//Ĭ����Ϸ�Ѷ�ģʽ 
static int    getWhat = NONE1;				//ץȡ�ÿ� 
static int    help_condition=0;				//��Ϸ������ʼ�� 
static int    timer_condition=0;			//��ʱ��״̬��ʼ�� 
// �������� 
static double bootlength = 0;
static double bootsize = 0;
static double bootx, booty;
// ����
static double hookx, hooky;                  // ���ӵĳ�ʼλ�� 

// ��״
static double head = 0.2;                    // ͷ 
static double eye = 0.02;                    // �۾� 
// ��ת 
static double hookangle = 0 ;                // ���ӽǶ�
static double angle = 1;                     // �ڶ��ĽǶ� 
static int    flag = 1;                      // ת�� 
// ���� 
static double hooklength = 0.5;              // ���ӳ��� 
static double slength = 0.1;                 // ������������ 
static double maxlength = 8;                 // ������󳤶� 
static double minlength = 0.5;               // ������С���� 

// ���� 
struct object{
	double x,y;                              // �������� 
	double r;                                // ץȡ��Ч��Χ 
	int    flag;                             // ץȡ״̬  
    int    score;                            // ���� 
    int    kind;                             // ���� 
    
}object[100];
static int objectNum = 10; 
static int nowNum;                  // ������Ŀ 
static int getObjectIndex;          // ��ץ���������� 
static int getMedicineIndex; 		//��ץ����ҩˮ��� 

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

//�û���Ĭ����������� 
static char *all_name[10]={
	"Dr.Strange","TonyStark","peppapig","Luckydog","ZJUer",
	"Dortmund","MarcoReus","Klose","Kroos","PeterParker"
};

// ��ʼ�� 
static int isNormal = 1;                     // ��֤�ڷǼ��̣�������ʱ����״̬�ĸ��£����򱣳�ԭ״̬���� 
static int hookState = SWING;                // ���ӵ�״̬��SWING,SHRINKGET,SHRINKFALL,STRETCH 
static int score = 0;                        // �ܷ� 
char stringscore[20] = {'s','c','o','r','e',' ','0','\0'};
static char name[80] = "user";
static int force = 0;						//�ж��Ƿ�ǿ������ 
static int win_flag=0; 						//�Ƿ񲥷�ͨ������ 
static int music_condition=1;				//�������ֲ���״̬ 
//��Ϸʱ�� 
clock_t start, end, stop;
clock_t bootstart, bootend;
static double scoretime;		
static double finaltime;
static double boottime;						//����������ʱ 
char stringscoretime[32] = {'t','i','m','e',' ',' ','0','\0'};

//��ť
static int id = 0 ; 
//��ͼ
static int map[30][20]; 

// ��������

// ��Ϸ��ʼ��
void init_game();

// ��Ϸ����
void drawMenu();                             // �˵� 
void drawButtons();                          // ��ť 
void drawRank();                             // ���а� 
void drawHelp();                             // ���� 
void drawSetting();							//��Ϸ���� 
void drawBoard();                            // �Ʒְ� 
void drawTimeBorad(); 						// ��ʱ�� 
void drawStage(); 							// ״̬�� 
void GetDoubleStr(double value);			 // ��ʱ��תΪ������ַ��� 
void drawEditText();						//�û������� 
void drawUser();							// ��Ϸ�趨 

// ����
void stretchHook();                          // �쳤���� 
void shrinkHook();                           // ���ع��� 
void swingHook();                            // ��ת���� 
void rot_hook();                             // �ı乳�ӽǶ� 
void draw_hook();                            // ������                             

// ���� 
void init_object();                          // ��ʼ������ 
void DrawAllObject();                        // ɸѡδ��ץȡ������ 
void DrawObject(int index);                  // ������ 
void judgeGet();                             // �ж�ץȡ 
void DrawMedicine(int index);

void drawWatermelon(double x,double y,double r);
void drawApple(double x,double y,double r);
void drawBeer(double x,double y,double r);
void drawHeart(double x,double y,double r);

//����
//��Ϸ�����汳������ 
void drawBackground(); 
void drawGameBackground();
void drawBeginBackground();
void drawUserBackground();
void drawHelpBackground();
void drawTxtBackground();
void drawRankBackground();
//������������ 
void stretchBoot();
void biggerBoot();
void drawBootBackground();
void drawBootBackground1();
void drawBootBackground2();

//�����ж� 
void judgeEnd();
void drawEnd();

//���а�
extern changeScore();
extern initName(); 

// �û�����ʾ����
void display(void);

// �û����ַ��¼���Ӧ����
void CharEventProcess(char ch)
{
	isNormal = 0 ;
	uiGetChar(ch);    // GUI�ַ�����
	display();        // ˢ����ʾ
}

// �û��ļ����¼���Ӧ����
void KeyboardEventProcess(int key, int event)
{
	isNormal = 0;
	uiGetKeyboard(key,event);                      // GUI��ȡ����
	
	switch(event){
		case KEY_DOWN:
			switch(key){
				case VK_DOWN:                      //���·Ź� 
					if(hookState == SWING) 
						hookState = STRETCH;	
					break;
			    case VK_UP:                        //��������            
			    	if(hookState == STRETCH && score>0) {
			    		score -=1;					//�����ɹ�������1����� 
			    		force = 1;
			    		hookState = SHRINKFALL;
					}
					if(hookState == STRETCH && score<=0) {
			    		force = 2;					//���ֲ���������ʧ�� 
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
	display();                                     // ˢ����ʾ
}

// �û�������¼���Ӧ����
void MouseEventProcess(int x, int y, int button, int event)
{
	isNormal = 0;
	uiGetMouse(x, y, button, event);   // GUI��ȡ���
	display();                         // ˢ����ʾ
}

// �û��ļ�ʱ��ʱ����Ӧ����
void TimerEventProcess(int timerID)
{
	isNormal = 1;
	if(timerID == MY_ROTATE_TIMER && enable_game ){
		display();                     // ˢ����ʾ
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

// �û���������� 
// ����ʼ��ִ��һ��
void Main() 
{
	PlaySound (TEXT("stage_01.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP|SND_NODEFAULT );
	// ��ʼ�����ں�ͼ��ϵͳ
	SetWindowTitle("Pac-Miner");
    InitGraphics();
    //  InitConsole();
    
	// ��ô��ڳߴ�
    winwidth = GetWindowWidth();
    winheight = GetWindowHeight();
    
    init_color();   //define own colors 
    SetPenSize(2);

	// ע��ʱ����Ӧ����
	registerCharEvent(CharEventProcess);           // �ַ�
	registerKeyboardEvent(KeyboardEventProcess);   // ����
	registerMouseEvent(MouseEventProcess);         // ���
	registerTimerEvent(TimerEventProcess);         // ��ʱ��
	
	// ��ʼ������ 
	
	InitRankings();
	bootlength = 0;
	bootx = 0;
	booty = winheight/2;
	bootsize = winheight/10;
	
	setMenuColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);
	setButtonColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0); 
	setTextBoxColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);

	
	// ������ʱ��
	startTimer(MY_ROTATE_TIMER, 25);

	//ȫ�ֱ���������ѡ��
	Interface = BOOT;
    
	// �򿪿���̨���������������Ϣ�����ڵ���
	 //InitConsole(); 
}

void drawMenu()
{ 
	static char * menuListFile[] = {"����",  
		 "����BGM   |  Ctrl-M", 
		 "��ͣBGM   |  Ctrl-I" ,
		"�˳�  | Ctrl-E"};
	static char * menuListGame[] = {"��Ϸ",
		"��ͣ | Ctrl-T",
		"���¿�ʼ  | Ctrl-O"
		};
	static char * menuListHelp[] = {"�鿴",
		"����  | Ctrl-A"};
	static char * selectedLabel = NULL;

	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH*1.5; // �ؼ��߶�
	double w = TextStringWidth(menuListHelp[0])*3; // �ؼ����
	double wlist = TextStringWidth(menuListGame[2])*1.2;
	double xindent = winheight/20; // ����
	int    selection;
	
	// menu bar
	drawMenuBar(0,y-h,winwidth,h);
	// File �˵�
	selection = menuList(GenUIID(0), x, y-h, w, wlist, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
	if( selection>0 ) selectedLabel = menuListFile[selection];
	if (selection==1){
		PlaySound (TEXT("stage_01.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP |SND_NODEFAULT);
	}
	if (selection==2){
		PlaySound (NULL,NULL,SND_FILENAME | SND_ASYNC|SND_LOOP |SND_NODEFAULT);
	}
	if( selection==3 )
		exit(-1); // �˳���Ϸ 
	
	// Tool �˵�
	menuListGame[1] = enable_game ? "��ͣ | Ctrl-T" : "���� | Ctrl-T";
	selection = menuList(GenUIID(0),x+w,  y-h, w, wlist,h, menuListGame,sizeof(menuListGame)/sizeof(menuListGame[0]));
	if( selection>0 ) selectedLabel = menuListGame[selection];
	if( selection==1 )
		enable_game = ! enable_game;
	if( selection==2 )
	{
		Interface = BEGIN ;
		init_game();	
	}

	
	// Help �˵�
	selection = menuList(GenUIID(0),x+2*w,y-h, w, wlist, h, menuListHelp,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if( selection>0 ) selectedLabel = menuListHelp[selection];
	if( selection==1 )
		Interface = HELP ;
}

void drawButtons()
{
	double fH = GetFontHeight();
	double h = fH*2;                   // �ؼ��߶�
	double w = winwidth/5;             // �ؼ����
	double x = winwidth/2-w/2;  
	double y = winheight/2+2*h; 
	
	if( button(GenUIID(0), x, y, w, h, "��Ϸ��ʼ") ){

	    Interface = USER;
    }
    
	if( button(GenUIID(0), x, y-1.5*h, w, h, "���а�") )
	{	
		Interface = RANK ;
	}

	if( button(GenUIID(0), x, y-3*h, w, h, "��Ϸ����") )
	{	
		Interface = HELP ;
	}

	if( button(GenUIID(0), x, y-6*h, w, h, "�˳���Ϸ") )
	{
		exit(-1);
	}

    
	if( button(GenUIID(0), x, y-4.5*h, w, h, "��Ϸ����") ){
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
	double h = fH*1.5;                // �ؼ��߶�
	double w = winwidth/10;           // �ؼ����
	double x = w/4;  
	double y = winheight-1.5*h;
	
	if (button(GenUIID(0), x, y, w, h, "����"))
	{
		Interface = BEGIN ;
	}
	drawRanking();
}

void drawHelp() 
{
	
	double h = GetFontHeight()*1.5;                // �ؼ��߶�
	double w = winwidth/10;           // �ؼ����
	double x = w/4;  
	double y = winheight-1.5*h; 
	double dx=winwidth/10;
	double dy=winheight/13;
	int pointsize;
	pointsize=GetPointSize();
	if (button(GenUIID(0), x, y, w, h, "������ҳ��"))
	{
		switch (timer_condition){   //�ʵ���cancel��ʱ�������Ե���ҷ���ʱ�������趨��ʱ�� 
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
	switch (help_condition){  //�ж���������İ����Ľ�����л��� 
		case PAGE1:{
			if (button(GenUIID(0), 0.8*winwidth, 0.1*y, w, h, "��һҳ")){
				help_condition=PAGE2; 
			}
			SetPointSize(30);
			SetPenColor(color[Gamemode].frame);
			SetStyle(2);
			drawLabel(4*dx,12*dy, "��Ϸ����");
			SetStyle(0);
			SetPointSize(20);
			SetPenColor(color[Gamemode].font);
			drawLabel(1*dx,11*dy, "�ڰ����´�½������֮����ҫ֮ǰ��������һ���ޱߵ����棬�����Ÿ�����̬��������");
			drawLabel(1*dx,10*dy, "��Щ������ɢ�ڸ���������͵��³Ǳ�������ǵ����ͺ��ǻۣ�����������ķ��١�");
			drawLabel(1*dx,9*dy, "�ڸ��������������һ�ֽ����������ᾧ�������壬����ʹ�����ӵ�����޵�������");
			drawLabel(1*dx,8*dy, "�����ᾧ�ĳ��֣�����������������������ᡣ����һ����ر�ɫ���������صĻ���֮ս��"); 
			drawLabel(1*dx,7*dy, "��������ǿ��ͳ�εĺڰ��ڣ��������´�½�ķ��ٺͻԻͣ��ƺ�Ҳ��ʼ����");
			drawLabel(1*dx,6*dy, "��������Ľ��ﻹ����������Ԥ�ף�δ֪������ħ���Ƿ�����Ща�������ĵ���֮�أ�") ;
			drawLabel(1*dx,5*dy, "�µ����Ψ��ð�ռ�����Ϊ����ҵ������ҵ�һ�����֮����˭���ǶԿ�а������������֮�ˣ�");
			drawLabel(1*dx,4*dy, "�𾴵���ʿ���뷢�����澫�顾�������������������Ѽ��д�������ᾧ�����Ȱ����´�½�ɣ�"); 
			drawLabel(2*dx,3*dy, "ץ׼ʱ���������������ǿ������");
			drawLabel(2*dx,2*dy, "��������������������󼴿�ͨ��");
			drawLabel(2*dx,1*dy, "ħ������������������ҩˮ�����Լ��������Ļ�����");
			break;
		}
		case PAGE2:{
			if (button(GenUIID(0), 0.8*winwidth, 0.16*y, w, h, "��һҳ")){
			help_condition=PAGE1; 
			}
			if (button(GenUIID(0), 0.8*winwidth, 0.1*y, w, h, "��һҳ")){
			help_condition=PAGE3; 
			}
			SetPointSize(30);
			SetPenColor(color[Gamemode].frame);
			SetStyle(2);
			drawLabel(4*dx,12*dy, "��Ϸ˵��");
			SetStyle(0);
			SetPointSize(20);
			SetPenColor(color[Gamemode].font);
			drawLabel(1*dx,11*dy, "��Ϸ������Է�Ϊ���󲿷֣��˵�������ʱ�Ʒְ�;��Ϸ��������Ϸ���;״̬��");
			drawLabel(1*dx,10*dy, "��Ϸ��ʤ�����ǲ������澫�顾����������Ե����еġ������ᾧ��");
			drawLabel(1*dx,8*dy, "��Ϸ�����������ں��ʵ�ʱ����    ��  �����Էſ���������(���̵� ���� ����)");
			drawLabel(1*dx,7*dy, "��ͬ�Ѷ�ģʽ�£�ÿ�֡������ᾧ���̺���������ͬ��������ҵ÷ֵķ�ʽҲ��ͬ"); 
			drawLabel(1*dx,6*dy, "��Ϸ�л����������ߡ���ҩˮ���롾��ҩˮ�������Ըı䡾���������ٶ�");
			drawLabel(2.5*dx,4.9*dy,"����ҩˮ��");
			drawLabel(6.5*dx,4.9*dy,"����ҩˮ��");
			drawLabel(1*dx,3.8*dy, "����ҩˮ�к��е����������ᱻ�����������գ�������Ҳ���Ҫ�Ѽ�ҩˮ��ȡ��ҩˮ��Ҳ����ӷ�");
			drawLabel(1*dx,2.8*dy, "���Ҳ����ͨ������   ��  (���ϰ���)ȼ��һ�����ֵ�������ȡ���������Ѿ������ġ��������ٻ�"); 
			drawLabel(2*dx,1.8*dy, "�����еġ������ᾧ������ȡ�󣬹ؿ����ɽ���");
			drawLabel(2*dx,0.8*dy, "ʱ���Ǿ������ŵģ����Լ������ѡ����ͣ����ʱҲ�����ж�");
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
			if (button(GenUIID(0), 0.8*winwidth, 0.16*y, w, h, "��һҳ")){
				help_condition=PAGE2; 
			}
			if (button(GenUIID(0), 0.8*winwidth, 0.1*y, w, h, "��һҳ")){
			help_condition=PAGE4;
			}			
			SetPointSize(30);
			SetPenColor(color[Gamemode].frame);
			SetStyle(2);
			drawLabel(4*dx,12*dy, "��Ϸ����");
			SetStyle(0);
			SetPointSize(22);
			SetPenColor(color[Gamemode].font);
			drawLabel(1*dx,11*dy, "�ٶȹ���δ���Ǽ����£���Ϊ��������ת����������һЩ�Ƕ�");
			drawLabel(1*dx,9*dy, "���ջء�������ֻ��Ҫһ�������㣬������Ҫȡ�ú����εĻ����ǲ����׵ģ�");
			drawLabel(1*dx,7*dy, "�������ᾧ��֮���������ڵ����ӣ�����Ϊĳλ����ʩ�ӷ����ı������ı�����ʽ");
			drawLabel(1*dx,5*dy, "��Ϊ���ǾͲ�������ע��Щƽ�������壬����Ҳ����С�˶����Ŀ�����"); 
			drawLabel(1*dx,3*dy, "��ʱ����������ҩˮ����һ��ֵ�����õ�����");
			break;
		}
		case PAGE4:{
			if (button(GenUIID(0), 0.8*winwidth, 0.16*y, w, h, "��һҳ")){  //�ʵ���cancel��ʱ�������Ե���ҷ���ʱ�������趨��ʱ�� 
				if (timer_condition==TIMEROFF){
					startTimer(MY_ROTATE_TIMER, 25);
					timer_condition=TIMERON;
				}
				
				help_condition=PAGE3; 
			}
			//�ʵ����֣��Ŷӳ�Ա��Ƭ�����Խ���,Ϊ�˷�ֹ���٣���ʱ�ر��˶�ʱ���� 
			if (button(GenUIID(0), 0.5*winwidth-3*w,0.23*y, w, h, "����.(��ЦȻ)")){
				timer_condition=TIMEROFF;
				cancelTimer(MY_ROTATE_TIMER);
				DrawPic("jxr.txt",0.1*winwidth,0.85*winheight);
				//�������� 
				SetPointSize(24);
				SetPenColor(color[Gamemode].frame);
				SetStyle(2);
				drawLabel(0.55*winwidth,0.65*winheight,"\"  �Ұ�֪��ͳ���ʦ��\"");
			    SetStyle(0);
				SetPointSize(pointsize);
			}
			if (button(GenUIID(0), 0.5*winwidth-0.5*w,0.23*y, w, h, "�ܱ�.(������)")){
				timer_condition=TIMEROFF;
				cancelTimer(MY_ROTATE_TIMER);
				DrawPic("cby.txt",0.06*winwidth,0.8*winheight);
				//�������� 
				SetPointSize(24);
				SetPenColor(color[Gamemode].frame);
				SetStyle(2);
				drawLabel(0.56*winwidth,0.7*winheight,"\" ����������Ԩʱ");
				drawLabel(0.65*winwidth,0.56*winheight,"��ԨҲ���������㡤����������\"");
			    SetStyle(0);
				SetPointSize(pointsize);	
			}
			if (button(GenUIID(0), 0.5*winwidth+2*w,0.23*y, w, h, "BIU.(��֪��)")){
				timer_condition=TIMEROFF;
				cancelTimer(MY_ROTATE_TIMER);
				//�������� 
				SetPointSize(24);
				SetPenColor(color[Gamemode].frame);
				SetStyle(2);
				drawLabel(0.55*winwidth,0.65*winheight,"\"  �Ұ�����ʦ�ͽ���磡\"");
			    SetStyle(0);
				SetPointSize(pointsize);
				DrawPic("zzy.txt",0.1*winwidth,0.86*winheight);
			}
			SetPointSize(30);
			SetPenColor(color[Gamemode].frame);
			SetStyle(2);
			drawLabel(4*dx,12*dy, "��������");
			SetStyle(0);
			SetPointSize(22);
			SetPenColor(color[Gamemode].font);
			drawLabel(0.3*winwidth,0.16*y,"����ħ����������Ĵ���ʱ�䣬�����ĵȴ�");
			drawLabel(0.3*winwidth,0.08*y,"��ť�³�ʱ���벻Ҫ�������̺����  ���޷�Ӧ����ȴ����������");
			}
	}
	
	SetPointSize(pointsize);
}
	


void drawUser()
{
	
	double fH1 = GetFontHeight();
	double h1 = fH1*1.5;                // �ؼ��߶�
	double w1 = winwidth/10;           // �ؼ����
	double x1 = w1/4;  
	double y1 = winheight-1.5*h1;
	
	if (button(GenUIID(0), x1, y1, w1, h1, "����"))
	{
		Interface = BEGIN ;
	}
	
	double fH = GetFontHeight();
	double h = fH*2;                   // �ؼ��߶�
	double w = winwidth/5;             // �ؼ����
	double x = winwidth/2-w/2;  
	double y = winheight/2+2*h;
	
	SetPenColor("Black");
	drawLabel(x/3, y+1.5*h, "��ǰѡ���Ѷ�Ϊ��");
	switch (Levelmode){
		case NORMAL:{
			drawLabel(x/3+x/4+x/11,y+1.5*h, "��ͨģʽ");
			break;
		}
		case DIFFICULT:{
			drawLabel(x/3+x/4+x/11,y+1.5*h, "����ģʽ");
			break;
		} 
		case EASY:{
			drawLabel(x/3+x/4+x/11,y+1.5*h, "С��ģʽ");
			break;
		}
	}
		
	if (button(GenUIID(0), x/3, y, w, h, "����ģʽ")){
		Levelmode = DIFFICULT;
	}
	
	if (button(GenUIID(0), x/3, y-1.5*h, w, h, "��ͨģʽ")){
		Levelmode = NORMAL;
	}
	
	if (button(GenUIID(0), x/3, y-3*h, w, h, "С��ģʽ")){
		Levelmode = EASY;
	}
	
	SetPenColor("Black");
	drawLabel(x*4/3, y+1.5*h, "��ǰѡ��ģʽΪ��");
	switch (Gamemode){
		case FOREST:{
			drawLabel(x*4/3+x/4+x/11, y+1.5*h, "ɭ��ģʽ");
			break;
		}
		case OCEAN:{
			drawLabel(x*4/3+x/4+x/11, y+1.5*h, "����ģʽ");
			break;
		}
		case DESERT:{
			drawLabel(x*4/3+x/4+x/11, y+1.5*h, "ɳĮģʽ");
			break;
		} 
	} 
	//�޸ĸ�����ť����ɫ���� 
	if (button(GenUIID(0), x*4/3, y, w, h, "ɭ��ģʽ")){
		Gamemode = FOREST;
		setButtonColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0); 
		setTextBoxColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);
		setMenuColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);

	}
	
	if (button(GenUIID(0), x*4/3, y-1.5*h, w, h, "����ģʽ")){
		Gamemode = OCEAN;
		setButtonColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0); 
		setTextBoxColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);
		setMenuColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);

	}
	
	if (button(GenUIID(0), x*4/3, y-3*h, w, h, "ɳĮģʽ")){
		Gamemode = DESERT;
		setButtonColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0); 
		setTextBoxColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);
		setMenuColors(color[Gamemode].frame,color[Gamemode].label,color[Gamemode].hotFrame,color[Gamemode].hotLabel,0);

	}
	
	if (button(GenUIID(0), x*4/3, y-7*h, w, h, "��Ϸ��ʼ")){
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
	double h1 = fH1*1.5;                // �ؼ��߶�
	double w1 = winwidth/10;           // �ؼ����
	double x1 = w1/4;  
	double y1 = winheight-1.5*h1;
	
	if (button(GenUIID(0), x1, y1, w1, h1, "����"))
	{
		Interface = BEGIN ;
	}
	
	double fH = GetFontHeight();
	double h = fH*2;                   // �ؼ��߶�
	double w = winwidth/5;             // �ؼ����
	double x = winwidth/2-w/2;  
	double y = winheight/2+2*h;
	
	switch (music_condition){
		case MUSICOFF:{
			if( button(GenUIID(0), x, y-1.5*h, w, h, "���ű�������") ){
				PlaySound (TEXT("stage_01.wav"),NULL,SND_FILENAME | SND_ASYNC|SND_LOOP |SND_NODEFAULT);
				music_condition=1;
			}
			break;
		}
		case MUSICON:{
			if( button(GenUIID(0), x, y-1.5*h, w, h, "��ͣ��������") ){
				PlaySound (NULL,NULL,SND_FILENAME | SND_ASYNC|SND_LOOP |SND_NODEFAULT);
				music_condition=0;
			break;
			}
		}
		
	
	}
} 

void display()
{
	// ����
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
			//��Ϸ�����Ժ�������һ�����ݵ���ף���� 
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
//��Ϸ�У��ײ�״̬������ 
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
			drawLabel(0.025*winwidth,0.015*winheight,"��ǰ״̬���ɹ�ץ����");
			switch (getWhat){
				case GET_OBJECT:{
					switch(getObjectIndex){
						case  BEER:{
	 						drawLabel(0.19*winwidth,0.015*winheight,"��ơ�ơ���");
							break;
							}
						case WATERMELON:{
							drawLabel(0.19*winwidth,0.015*winheight,"�����ϡ���");
							break;
							}
						case RICE:{
							drawLabel(0.19*winwidth,0.015*winheight,"�����š���");
							break;
							} 
						case MILK:{
							drawLabel(0.19*winwidth,0.015*winheight,"��ţ�̡���");
							break;
							}	
						case APPLE:{
							drawLabel(0.19*winwidth,0.015*winheight,"��ƻ������");
							break;
							}
						case CANDY:{
							drawLabel(0.19*winwidth,0.015*winheight,"�����Ǻ�«����");
							break;
							}
			 			} 
			 		case GET_MEDICINE:{
					 	switch(getMedicineIndex){
							case SPEEDUP:{
								drawLabel(0.19*winwidth,0.015*winheight,"����ҩˮ�����ж��ٶ�������");
								break;
								}
							case SLOWDOWN:{
								drawLabel(0.19*winwidth,0.015*winheight,"����ҩˮ�����ж��ٶ��½���");
								break;
								} 
						}break;
					 }
				 }
			}
		}	
		case SHRINKFALL:{
			if (getWhat==NONE1){
				drawLabel(0.025*winwidth,0.015*winheight,"��ǰ״̬��δץȡ"); 
				if (force==1){
					drawLabel(0.4*winwidth,0.015*winheight,"��ȼ��һ�������ջض���"); 
				}
				
			}
			
			break;
		} 
		case SWING:{
			drawLabel(0.025*winwidth,0.015*winheight,"��ǰ״̬���ڶ���"); 
			
			break;
		}
		case STRETCH:{
			drawLabel(0.025*winwidth,0.015*winheight,"��ǰ״̬�����ڳ���ץȡ"); 
			if (force==2){
					SetPenColor("red");
					drawLabel(0.4*winwidth,0.015*winheight,"���ֲ��㣬�޷����ն���");
					SetPenColor(ccolor);	
				}
			break;
		}
	}	
	switch (Levelmode){
		case EASY:{
			drawLabel(0.8*winwidth,0.015*winheight,"��ǰ�Ѷȣ�С�׼�");
			break;
		}
		case NORMAL:{
			drawLabel(0.8*winwidth,0.015*winheight,"��ǰ�Ѷȣ���ͨ��");
			break;
		}
		case DIFFICULT:{
			drawLabel(0.8*winwidth,0.015*winheight,"��ǰ�Ѷȣ�������");
			break;
		}
	}
	SetPenSize(_pensize);
	SetPenColor(ccolor);
}

//������ͬ�ı������� 
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
//������Ϸ���� 
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
//���ƿ�ʼ���汳�� 
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
//�������ý��汳�� 
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
//�������а���汳�� 
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
//������Ϸ�������汳�� 
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
//�����ı����뱳�� 
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
//���ƿ������� 
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
//��ʼ����Ϸ���� 
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


//��ʼ��������� 
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
		object[i].flag = 1;      // ��ʾ�����δ��ץ�� 
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
//���ĺ�����ץȡ�жϴ��� 
void judgeGet()
{
	double x, y;
	int i;
	x = hookx+cos(hookangle*PI/180)*hooklength;       //����Ŀǰ���� 
	y = hooky+sin(hookangle*PI/180)*hooklength;
	
	double distance;
	
	for (i=0; i<objectNum; i++)
	{
		if( !object[i].flag ){
			continue;
		}
		distance = sqrt( pow(x-object[i].x, 2) + pow(y-object[i].y, 2));       //���㹳�����������ľ��� 
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
		distance = sqrt( pow(x-medicine[i].x, 2) + pow(y-medicine[i].y, 2));       //���㹳�����������ľ��� 
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

//�����������쳤 
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
//�������������� 
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

//��Ϸ�����ж� 
void judgeEnd()
{
	force = 0;
	if( nowNum <=0 ) 
		
		Interface = END ; 
}
//�޸İڶ��Ƕ� 
void rot_hook()
{
	if(hookangle <= -180 || hookangle >= 0){      //0��-180��ʱ����ת�� 
		flag *= -1;
	}
    hookangle = hookangle + flag*angle;
}

//�滭������ 
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
//���������Ӱڶ� 
void swingHook()
{
	if(isNormal){
		rot_hook();
	}
	draw_hook();
}
//�滭��չʾδץȡ��ȫ������ 
void DrawAllObject()
{
	int i;
	for(i=0; i<objectNum; i++ ){
		if( object[i].flag ){       //�ж������Ƿ��ѱ�ץ�� 
			DrawObject(i);
		}
	}
	for(i=0; i<medicineNum; i++ ){
	if( medicine[i].flag ){       //�ж������Ƿ��ѱ�ץ�� 
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
//�滭���������� 
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

//��������չʾ 
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

//���ƣ��û���������� 
void drawEditText()
{
	double fH1 = GetFontHeight();
	double h1 = fH1*1.5;                // �ؼ��߶�
	double w1 = winwidth/10;           // �ؼ����
	double x1 = w1/4;  
	double y1 = winheight-1.5*h1;
	

	
	if (button(GenUIID(0), x1, y1, w1, h1, "����"))
	{
		Interface = BEGIN ;
	}

	static char results[200] = "";
	static char memo[80]="Welcome! Please enter your name.";
	double fH = GetFontHeight();
	double h = fH*2; // �ؼ��߶�
	double w = winwidth/4; // �ؼ����
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
//��ʱ�塢�Ʒְ���� 
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
//��double��ʱ��ת��Ϊ�ַ�����ʽ 
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

