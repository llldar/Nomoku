#include <windows.h>
#include "pisqpipe.h"
#include "Board_controller.h"
#include "ANN.h"
#include "AI_Wrapper.h"
#include "Logger.h"
#include <fstream>
const char *infotext = "name=\"Nomoku\", author=\"Clark Piao & Howard Liu & Nathaniel Lin\", version=\"0.5 beta\", country=\"China\", www=\"\"";
AI_Nomoku *ai;
Board_controller *bc;
int lastwinsnumber = 0;

void brain_init()
{	
	//��Ҫ��������������ʱ��ʼ���ķ��ڴ˴�
	bc = new Board_controller(width,height);
	ai = new AI_Nomoku();
	brain_restart();
}

void brain_restart()
{
	//ÿһ���忪ʼǰ��ʼ�����ڴ˴�
	//��������
	bc->clear_board();
	//�ȴ��ź���
	while (semaphore_weight == 0){}
	pipeOut("OK");
	//ai->start();
}

void brain_my(int x, int y)
{
	//ϵͳͨ���˺���֪ͨAI�ҷ����µ���
	bc->move_self(x, y);
}

void brain_opponents(int x, int y)
{
	//ϵͳͨ���˺���֪ͨAI�������µ���
	bc->move_oppo(x, y);
}

void brain_block(int x, int y)
{
	//�����ڻ�ʤ���������
	bc->move_win(x,y);
}

int brain_takeback(int x, int y)
{
	//���ڻ���
	return bc->take_back(x,y);
}

void brain_turn()
{
	ai->alpha_beta_search();
}

void brain_end()
{
	std::ifstream result;
	result.open("_result.txt");
	if (result.is_open()){
		char resultstring[500];
		int nowwinsnamber = 0;
		for (int i = 0; i < 12; i++){
			result >> resultstring;
		}
		for (int i = 6; i < 500; i++){
			if (resultstring[i] == '='){
				resultstring[i] = 0;
			}
		}
		nowwinsnamber = atoi(&resultstring[6]);

		//pv����
		for (;;){
			if (mutex_ans == 0){
				mutex_ans = 1;
				if (nowwinsnamber>lastwinsnumber) ans = 1;
				else ans = -1;
				mutex_ans = 0;
				break;
			}
		}
		lastwinsnumber = nowwinsnamber;
	}
	delete ai;
	delete bc;
}

#ifdef DEBUG_EVAL
#include <windows.h>

void brain_eval(int x, int y)
{
	HDC dc;
	HWND wnd;
	RECT rc;
	char c;
	wnd = GetForegroundWindow();
	dc = GetDC(wnd);
	GetClientRect(wnd, &rc);
	c = (char)(board[x][y] + '0');
	TextOut(dc, rc.right - 15, 3, &c, 1);
	ReleaseDC(wnd, dc);
}

#endif
