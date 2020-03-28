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


/*	��������InitRankings
*	
*	���ܣ���ʼ�������ӵ�ǰĿ¼�¶�ȡ���а��ļ������޼�¼���½���
*
*	�÷���ֱ�ӵ���InitRankings()
*/
void InitRankings(){
	int flag_first=0;
	FILE *fr;
	//ͷ�ڵ��ʼ��������DEBUG 
	Head.score=-1;
	Head.time=-1;
	Head.next=NULL; 
	usernow.score=0;
	usernow.time=0;
//	initName("name");
	fr=fopen("rankings.txt","a+");	//�ӵ�ǰĿ¼��ȡ�ļ���Ϣ 
	rewind(fr);
	struct rank *rk,*node;
	node=&Head;
	//���ж�ȡ���ݣ��������� 
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

/*	��������drawRanking
*
*	���ܣ�չʾ���а�
*
*	�÷��� showMeRankings()
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
	
	 
	drawBox(0.4*winwidth,0.9*winheight,0.2*winwidth,0.07*winheight, 0, "���а�", "M", "BLACK");
	
	
	while(node->next!=NULL&&j<20){
		
		sprintf(a,"%2d                                %-15s       ",++j,node->name);
		sprintf(b,"%3d                                           %3d      \n",node->score,node->time);
			
		string_width = TextStringWidth(a);
		
		MovePen(0.20*winwidth,0.04*(20-j)*winheight+0.07*winheight);
	
		if (j>=1&&j<=3) SetPenColor("red"); 
		DrawTextString("������"); 
		DrawTextString(a);
		MovePen(0.334*winwidth,0.04*(20-j)*winheight+0.07*winheight);
		DrawTextString("�ǳƣ�");		 
		MovePen(0.55*winwidth,0.04*(20-j)*winheight+0.07*winheight);
		DrawTextString("�÷֣�");
		DrawTextString(b);
		MovePen(0.7*winwidth,0.04*(20-j)*winheight+0.07*winheight);
		DrawTextString("����ʱ�䣺           ��");	
		node=node->next;
		SetPenColor("black");
			
	}
}

/*	��������score
*
*	���ܣ���̨���з������ۻ�
*
*	�÷��� score(score_change)
*
*		
*	score_change  		����ҷ����䶯ʱ�޸ĺ�̨��¼�ķ����䶯 
*/

void changeScore(int score_change){
	usernow.score = score_change;
} 

void changeTime(int score_time){
	usernow.time = score_time;
} 

/*	��������name
*
*	���ܣ��½�������������� 
*/
void initName(char a[])
{
	strcpy(usernow.name,a);
} 


/*
*	��������rankChange
*
*	���ܣ�	�����а��Ӧ�������в��뵱ǰ��ҵļ�¼��������һ��ranking.txt�ĵ�����
*
*	�÷���ֱ�ӵ���rankChange()���� 
* 
*/
void rankChange(){
	FILE* fp;
	int flag=0;
	char a[MAXLENGTH];
	struct rank *node;
	fp=fopen("rankings.txt","w");
	node=&Head;
	
	//���ҽڵ�λ�� 
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
	//�����μ�¼�Ľڵ���������� 
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






