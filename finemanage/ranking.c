#include<stdio.h>
#include<stdlib.h>
#include<string.h> 

#define MAXLENGTH 1000 
extern double  winwidth,winheight;
static int place[10]={0,1,2,3,4,5,6,7,8,9}; 
struct rank{
	

	int score;
	int time;
	char name[200];
	struct rank* next;
	
}usernow,Head;


/*	函数名：InitRankings
*	
*	功能：初始化链表，从当前目录下读取排行榜文件（若无记录则新建）
*
*	用法：直接调用InitRankings()
*/
void InitRankings(){
	int flag_first=0;
	FILE *fr;
	//头节点初始化，便于DEBUG 
	Head.score=-1;
	Head.time=-1;
	Head.next=NULL; 
	usernow.score=0;
	usernow.time=0;
//	initName("name");
	fr=fopen("rankings.txt","a+");	//从当前目录读取文件信息 
	rewind(fr);
	struct rank *rk,*node;
	node=&Head;
	//逐行读取数据，建立链表 
	while(!feof(fr)){
		rk = (struct rank*)malloc(sizeof(Head));
		fscanf(fr,"%s%d%d",&rk->name,&rk->score,&rk->time);
		printf("%s\n",rk->name);
		node->next=rk;
		node=node->next;
		node->next=NULL;
	}
	close(fr);
}

/*	函数名：drawRanking
*
*	功能：展示排行榜
*
*	用法： showMeRankings()
* 
*/


void drawRanking(){
	struct rank* node;
	node=Head.next;
	double string_width=1;
	char a[MAXLENGTH];
	char b[MAXLENGTH];
	int i=0;
	int j=0; 
	
	 
	drawBox(0.4*winwidth,0.9*winheight,0.2*winwidth,0.07*winheight, 0, "排行榜", "M", "BLACK");
	
	
	while(node->next!=NULL&&j<20){
		
		sprintf(a,"%2d                                %-15s       ",++j,node->name);
		sprintf(b,"%3d                                           %3d      \n",node->score,node->time);
			
		string_width = TextStringWidth(a);
		
		MovePen(0.20*winwidth,0.04*(20-j)*winheight+0.07*winheight);
	
		if (j>=1&&j<=3) SetPenColor("red"); 
		DrawTextString("排名："); 
		DrawTextString(a);
		MovePen(0.334*winwidth,0.04*(20-j)*winheight+0.07*winheight);
		DrawTextString("昵称：");		 
		MovePen(0.55*winwidth,0.04*(20-j)*winheight+0.07*winheight);
		DrawTextString("得分：");
		DrawTextString(b);
		MovePen(0.7*winwidth,0.04*(20-j)*winheight+0.07*winheight);
		DrawTextString("所用时间：           秒");	
		node=node->next;
		SetPenColor("black");
			
	}
}

/*	函数名：score
*
*	功能：后台进行分数的累积
*
*	用法： score(score_change)
*
*		
*	score_change  		该玩家分数变动时修改后台记录的分数变动 
*/

void changeScore(int score_change){
	usernow.score = score_change;
} 

void changeTime(int score_time){
	usernow.time = score_time;
} 

/*	函数名：name
*
*	功能：新建并输入玩家姓名 
*/
void initName(char a[])
{
	strcpy(usernow.name,a);
} 


/*
*	函数名：rankChange
*
*	功能：	在排行榜对应的链表中插入当前玩家的记录，并产生一个ranking.txt文档保存
*
*	用法：直接调用rankChange()即可 
* 
*/
void rankChange(){
	FILE* fp;
	int flag=0;
	char a[MAXLENGTH];
	struct rank *node;
	fp=fopen("rankings.txt","w");
	node=&Head;
	
	//查找节点位置 
	while((node->next)!=NULL){		
		if (Head.next==NULL){
			sprintf(a,"%s %d %d\n",usernow.name,usernow.score,usernow.time);
			fputs(a,fp);
			flag=1;
			break;
		}
		if (usernow.score<(node->next)->score){
			node=node->next;
			continue;
		}
		if(usernow.score==(node->next)->score&&usernow.time>=(node->next)->time){
			node=node->next;
			continue;
		}		
		break;
	}
	struct rank* NODE;
	NODE = (struct rank*)malloc(sizeof(Head));
	NODE->score=usernow.score;
	NODE->time=usernow.time;
	strcpy(NODE->name,usernow.name);
	//将本次记录的节点插入链表中 
	if (flag==0){
		
		NODE->next=node->next;
		node->next=NODE;

	}
	if (flag==1){
		Head.next=NODE;
		NODE->next=NULL;
		fclose(fp);
		return;
	}
	node=Head.next;
	while(node->next!=NULL){
		sprintf(a,"%s %d %d\n",node->name,node->score,node->time); 
		fputs(a,fp);
		node=node->next;
	}
	fclose(fp);
}






