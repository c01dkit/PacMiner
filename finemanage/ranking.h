/*
*===================================
*
*
*	排行榜的相关设定
*
*
*=================================== 
*/

#ifndef ____ranking_h____
#define ____ranking_h____
void rankChange(); 
/*
*	函数名：InitRankings
*	
*	功能：初始化链表，从当前目录下读取排行榜文件（若无记录则新建）
*
*	用法：直接调用InitRankings()
* 
*/
void InitRankings();

void drawRanking();
/*	函数名：showMeRankings 
*
*	功能：展示排行榜
*
*	用法： showMeRankings()
* 
*/

//void shouMeRankings();



/*	函数名：score
*
*	功能：后台进行分数的累积
*
*	用法： score(score_change)
*
*	score_change  		该玩家分数变动时修改后台记录的分数 
*/

void score(int score_change);

#endif
