/*
*===================================
*
*
*	���а������趨
*
*
*=================================== 
*/

#ifndef ____ranking_h____
#define ____ranking_h____
void rankChange(); 
/*
*	��������InitRankings
*	
*	���ܣ���ʼ�������ӵ�ǰĿ¼�¶�ȡ���а��ļ������޼�¼���½���
*
*	�÷���ֱ�ӵ���InitRankings()
* 
*/
void InitRankings();

void drawRanking();
/*	��������showMeRankings 
*
*	���ܣ�չʾ���а�
*
*	�÷��� showMeRankings()
* 
*/

//void shouMeRankings();



/*	��������score
*
*	���ܣ���̨���з������ۻ�
*
*	�÷��� score(score_change)
*
*	score_change  		����ҷ����䶯ʱ�޸ĺ�̨��¼�ķ��� 
*/

void score(int score_change);

#endif
