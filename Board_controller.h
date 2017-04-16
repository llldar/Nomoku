//
//  Board_controller.h
//  Nomoku
//
//  This is the controller of the board
//  Due to the strange piskvork developer's preference
//  We use height as x-axis
//         width  as y-axis (which is fortran style arrays)
//

#ifndef Board_controller_h
#define Board_controller_h

#ifndef MAX_BOARD_WIDTH
#define MAX_BOARD_WIDTH 100
#endif

#ifndef MAX_BOARD_HEIGHT
#define MAX_BOARD_HEIGHT 100
#endif

//���̱��
enum{ B_FREE, B_MY_MOVE, B_OP_MOVE, B_WIN_MOVE, B_TO_EXPAND};

class Board_controller 
{
	int width;
	int height;
	int count;
	bool i_w;
	bool h_r;

public:
	//�洢����
	int **board;

	//Constructor Destructor
	Board_controller(int w, int h);
	~Board_controller();

	//getter setter
	int getWidth();
	int getHeight();
	void setWidth(int w);
	void setHeight(int h);

	//�ж����������Ƿ�Ϊ��
	bool is_empty();
	//�ж�ָ����λ��xy �Ƿ�Ϊ��
	bool is_free(int x, int y);
	//�ж�ָ����λ��xy �Ƿ������̷�Χ��
	bool is_in_range(int x, int y);
	bool have_result();
	bool is_winner();

	//�ҷ��ߵ�һ��
	void move_self(int x, int y);
	//�Է��ߵ�һ��
	void move_oppo(int x, int y);
	//���ջ�ʤ�����趨
	void move_win(int x, int y);
	//����ĳһ���õĺ���
	int take_back(int x, int y);

	//����״̬���������̨
	void visualize();
	//����alpha-beta��������ǵ�ǰ����Ҫ�ߵĲ�
	void mark_edge_blocks();
	//��������������չ���
	void clear_expand_marks();
	//��x,y��һ������λ�ý�����չ ab������
	void expand(int x, int y);
	//����һ����֮����������
	void clear_board();

};

#endif /* Board_controller_h */
