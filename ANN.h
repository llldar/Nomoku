#ifndef _ANN_H
#define _ANN_H
#include <vector>

#ifndef MAX_BOARD
#define MAX_BOARD 100
#endif // !MAX_BOARD

extern int mutex_ans;											//0:δʹ�ã�1�����ڱ�ռ��
extern int semaphore_weight;									//��ʾ�Ƿ��п���������0����ʾû�У�1��ʾ��
extern int ans;

//��������¿�ʹ��MAX_BOARD����width��height������
void init_ANN(int *neure, short *weight);					//��ʼ��ANN���������Ȩֵ��������Ԫ��ֵ���м估�����Ԫ��0
int Update_ANN(int *neure, short *weight);					//����Ȩֵ�����м䣬�����Ԫֵ
void init_ANN_N(int(*board)[MAX_BOARD], int *neure);		//��ʼ����Ԫ
void init_ANN_W(short *weight);								//��ʼ������Ȩֵ
int get_result();											//���һ������ֽ�� 0���䣬1��Ӯ
double winrateCalc(std::vector<short> *nowWeight);			//��������ʤ��
int outputevaluation();										//��AB��֦������������ֵ
void FINANN(std::vector<short> *nowWeight);					//������Ȩֵ��width_height.txt����
void FOutANN(std::vector<short> *nowWeight);				//������Ȩֵ�����width_height.txt��

#endif