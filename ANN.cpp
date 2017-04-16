//
//	ANN.cpp
//	Nomoku
//
//	�����(width*height)����Ԫ
//	�м��1/2*(width*height)����Ԫ
//	�����1����Ԫ
//һ��3/2(width*height)+1����Ԫ
//	һ��(1 / 2)*width*height*width*height+(1 / 2)*width*height��Ȩֵ
//	itoa������һ����׼��C����������Windows���еģ����Ҫд��ƽ̨�ĳ�������sprintf����Windowsƽ̨����չ�ģ���׼������sprintf�����ܱ������ǿ���÷���printf���ƣ�
//	
//	char str[255];
//	sprintf(str, "%x", 100); //��100תΪ16���Ʊ�ʾ���ַ���
//
//

#include "ANN.h"
#include "pisqpipe.h"
#include "AI_Wrapper.h"
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>

int temp_board[MAX_BOARD][MAX_BOARD];						//ȫ�ֱ�������״̬

int mutex_ans = 0;											//0:δʹ�ã�1�����ڱ�ռ��
int semaphore_weight = 0;									//��ʾ�Ƿ��п���������0����ʾû�У�1��ʾ��
int ans = 0;												//0��û�н����1��ʤ��-1�䡣

char file_path[1000] = "E:/";							//�ļ�·����Ĭ��E��
int input_number = width*height;						//�������Ԫ����
int mid_number = (1.0 / 2)*width*height;				//�м����Ԫ����
int output_number = 1;								//�������Ԫ����
int weight_number = input_number*mid_number + mid_number*output_number;//����Ȩֵ���� ����*�м�+�м�*���
int neure_number = input_number + mid_number + output_number;//������Ԫ���� ����+�м�+���
int winp_denominator = 10;							//ʤ�ʵķ�ĸ

int *neure = new int[neure_number];							//��Ԫ����
short *weight = new short[weight_number];					//Ȩֵ����

int get_result(){
	int temp = 0;
	for (;;){
		if (mutex_ans == 0){
			mutex_ans = 1;
			if (ans == 0);
			else if (ans == 1)temp = ans;
			else if (ans == -1) temp = ans;
			mutex_ans = 0;
		}
		if (temp != 0) break;
	}
	return temp;
};

void init_ANN_N(int(*board)[MAX_BOARD], int *neure){//��ʼ����Ԫ
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			neure[i*height + j] = board[i][j];
		}
	}

	memset(&(neure[width*height]), 0, sizeof(int)*(mid_number + output_number));
}

void init_ANN_W(short *weight){//��ʼ������Ȩֵ
	srand((unsigned)time(NULL));
	for (int i = 0; i < weight_number; i++){
		weight[i] = rand() % 0xffff;
	}
}

//����������ڴ潻��GA�����㷨����
void init_ANN(int *neure, short *weight){//��ʼ��ANN���������Ȩֵ��������Ԫ��ֵ���м估�����Ԫ��0

	//��ʼ��ǰinput_number����ԪֵΪ����״̬
	init_ANN_N(temp_board, neure);
	init_ANN_W(weight);
}

int Update_ANN(int *neure, short *weight){//����Ȩֵ�����м䣬�����Ԫֵ
	//���Ϻ���=��Ȩ���g(x)=sum(wi*ai)
	//�����=���Ժ���f(x)=x
	for (int i = 0; i < mid_number; i++){//�����м����Ԫֵ
		//neure[input_number+i]=sum(wi*ai)
		for (int j = 0; j < input_number; j++){
			//ÿһ��������ԪJ*���ϵ��м���ԪI��Ȩ��   
			neure[input_number + i] += neure[j] * weight[j*mid_number + i];
		}
	}
	for (int i = 0; i <= mid_number; i++){//�����������Ԫֵ
		//nneure[input_number+mid_number] =sum(wi*ai)
		neure[input_number + mid_number] += neure[input_number + i] * weight[input_number*mid_number + i];
	}
	return neure[input_number + mid_number];
}
//
double winrateCalc(std::vector<short> *nowWeight){//��������ʤ��
	semaphore_weight = 1;
	for (int i = 0; i < weight_number; i++){
		weight[i] = (*nowWeight)[i];
	}
	//����һ�ֵĺ���
	int counts;
	int win_times = 0;
	for (counts = 0; counts < winp_denominator; counts++){
		if (get_result() == 1){
			win_times++;
		}
	}
	semaphore_weight = 0;
	return (win_times*1.0) / counts;
}

int outputevaluation(){//��AB��֦������������ֵ
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			temp_board[i][j] = bc->board[i][j];
		}
	}
	init_ANN_N(temp_board, neure);
	return Update_ANN(neure, weight);
}




void FOutANN(std::vector<short> *nowWeight){//������Ȩֵ�����width_height.txt��
	for (int i = 0; i < weight_number; i++){
		weight[i] = (*nowWeight)[i];
	}
	//����һ�ֵĺ���
	
	char swidth[10], sheight[10];
	char underline[2] = "_";
	char filename[20];
	sprintf(swidth, "%d", width);
	sprintf(sheight, "%d", height);
	strcpy(filename, file_path);
	strcat(filename, swidth);
	strcat(filename, "_");
	strcat(filename, sheight);
	strcat(filename, ".txt");
	std::ofstream file;

	file.open(filename);
	for (int i = 0; i < weight_number; i++){
		file << weight[i] << '\n';
	}
	file.close();
}
void FINANN(std::vector<short> *nowWeight){//������Ȩֵ��width_height.txt����
	char swidth[10], sheight[10];
	char underline[2] = "_";
	char filename[20];
	sprintf(swidth, "%d", width);
	sprintf(sheight, "%d", height);
	strcpy(filename, file_path);
	strcat(filename, swidth);
	strcat(filename, "_");
	strcat(filename, sheight);
	strcat(filename, ".txt");
	std::ifstream file;

	file.open(filename);
	if (file.is_open() == 0){
		init_ANN_W(weight);
		for (int i = 0; i < weight_number; i++){
			(*nowWeight)[i] = weight[i];
		}
	}
	else {
		for (int i = 0; i < weight_number; i++){
			file >> weight[i];
			(*nowWeight)[i] = weight[i];
		}
	}

	file.close();
	//����һ�ֵĺ���
}
