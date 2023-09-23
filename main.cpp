#include"main.h"

/*************************************************/
int main() {

	char buff[2048];
	//��ʼ������
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	settextstyle(SIZE_FONT,0, STYLE_FONT);
	settextcolor(COLOR_FONT);
	setbkmode(TRANSPARENT);
	//д�������Դ�ļ�
	load_Resource("./music", "./music/bkMusic.wav", "WAVE", IDR_WAVE2);
	load_Resource("./music", "./music/move.wav", "WAVE", IDR_WAVE4);
	load_Resource("./music", "./music/door.wav", "WAVE", IDR_WAVE3);
	load_Resource("./music", "./music/bomb.wav", "WAVE", IDR_WAVE1);

	mciSendString("open ./music/bkMusic.wav alias bk", 0, 0, 0);
	mciSendString("open ./music/move.wav alias move",0,0,0);
	mciSendString("open ./music/door.wav alias door", 0, 0, 0);
	mciSendString("open ./music/bomb.wav alias bomb", 0, 0, 0);
	mciSendString("play bk repeat", 0, 0, 0);

	while (1) {
		int key = GamePlay();
		if (key != -1) {
			int vr = 0, vc = 0;
			switch (key) {
			case VK_UP: Move(-1, 0);break;
			case VK_DOWN:Move(1, 0);break;
			case VK_LEFT:Move(0, -1);break;
			case VK_RIGHT:Move(0, 1);break;
			}
		}
		cleardevice();
		BeginBatchDraw();
		//
		if (map[4][15] == �� && map[3][15] ==��) {
			map[3][8] = ��;
			map[3][9] = ��;
			map[3][10] = ̫;
			map[3][11] = ��;
			map[3][12] = ��;
		}
		if (map[3][11] == ǳ) {
			for (int i = 1; i < 10; i++) {
				map[i][16] = ��;
				map[i][15] = ��;
				map[i][17] = ��;
				map[i][18] = ��;
				map[i][19] = ��;
				map[i][20] = ��;
			}
			map[3][10] = ��;
			map[3][11] = ��;
		}
		if (map[13][24] == ��) {
			map[13][24] = ��;
			map[14][24] = ��;
			map[15][24] = ��;

		}
		if (map[15][24] == �� && map[12][26] == �� && c > 22 && r > 12) {
			mciSendString("close bk", 0, 0, 0);
			break;
		} 
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				outtextxy(j*textwidth(text[map[i][j]]), i * textheight(text[map[i][j]]), text[map[i][j]]);
				outtextxy(600, 30, "SHIFT����");
				outtextxy(600, 60, "CTRL����");
				outtextxy(600, 90, "�ǵùص�");
			}
		}
		FlushBatchDraw();

	}
	EndBatchDraw();
	closegraph();
	Fmain();
	return 0;
}

int GamePlay() {
	ExMessage m;
	peekmessage(&m, EX_KEY);
	if (m.message == WM_KEYDOWN)
		return m.vkcode;
	else return -1;
}


int Move(int vr, int vc)
{	
	//����
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000 && map[r - vr][c - vc] == ��) {
		if (map[r + vr][c + vc] == ��) {
			map[r + vr][c + vc] = Ƥ;
			map[r][c] = ˮ;
			map[r - vr][c - vc] = ��;
			r -= vr;
			c -= vc;
		}
		if (map[r + vr][c + vc] == Ǯ) {
			map[r + vr][c + vc] = �;
			map[r][c] = ��;
			map[r - vr][c - vc] = ��;
			r -= vr;
			c -= vc;
		}

	}
	//����
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && map[r + vr][c + vc] == �� && (
			map[r - vr][c - vc] == �� ||
			map[r - vr][c - vc] == �� ||
			map[r - vr][c - vc] == �� ||
			map[r - vr][c - vc] == ʯ ||
			map[r - vr][c - vc] == � ||
			map[r - vr][c - vc] == ˮ ||
			map[r - vr][c - vc] == Ƥ ||
			map[r - vr][c - vc] == ǳ ||
			map[r - vr][c - vc] == �� ||
			map[r - vr][c - vc] == �� ||
			map[r - vr][c - vc] == ��

			)) {
		map[r][c] = map[r - vr][c - vc];
		map[r - vr][c - vc] = ��;
		map[r + vr][c + vc] = ��;
		r += vr;
		c += vc;
		mciSendString("play move from 0", 0, 0, 0);
	}
	else
	if (map[r + vr][c + vc] == ��) {
		map[r + vr][c + vc] = ��;
		map[r][c] -= ��;
		r += vr;
		c += vc;
		mciSendString("play move from 0", 0, 0, 0);
	}
	else
	if (map[r + vr][c + vc] == ��) {
		map[r + vr][c + vc] = ��;
		map[r][c] -= ��;
		r += vr;
		c += vc;
		mciSendString("play door from 0", 0, 0, 0);
	}
	else//����
	if ( 
			map[r + vr][c + vc] == �� || 
			map[r + vr][c + vc] == �� || 
			map[r + vr][c + vc] == Ƥ ||
			map[r + vr][c + vc] == ˮ || 
			map[r + vr][c + vc] == � ||
			map[r + vr][c + vc] == ǳ ||
			map[r + vr][c + vc] == ʯ ||
			map[r + vr][c + vc] == �� ||
			map[r + vr][c + vc] == �� ||
			map[r + vr][c + vc] == �� ||
			map[r + vr][c + vc] == ��

			) {
		if (map[r + vr * 2][c + vc * 2] == ��) {
			map[r + vr * 2][c + vc * 2] = map[r + vr][c + vc];
			map[r + vr][c + vc] = ��;
			map[r][c] -= ��;
			r += vr;
			c += vc;
			mciSendString("play move from 0", 0, 0, 0);
		}
		//��ǽ
		if (map[r + vr][c + vc] == �� && map[r + vr * 2][c + vc * 2] == ǽ) {
			map[r + vr * 2][c + vc * 2] = ��;
			map[r + vr][c + vc] = ��;
			map[r][c] -= ��;
			r += vr;
			c += vc;
			mciSendString("play bomb from 0", 0, 0, 0);
		}
		//����
		if (map[r + vr][c + vc] == ˮ && map[r + vr * 2][c + vc * 2] == �) {
			map[r + vr * 2][c + vc * 2] = ǳ;
			map[r + vr][c + vc] = ��;
			map[r][c] -= ��;
			r += vr;
			c += vc;
		}
		if (map[r + vr][c + vc] == ʯ && map[r + vr * 2][c + vc * 2] == Ƥ) {
			map[r + vr * 2][c + vc * 2] = ��;
			map[r + vr][c + vc] = ��;
			map[r][c] -= ��;
			r += vr;
			c += vc;
		}
	}
	//��
	if (map[r + vr][c + vc] == ��) {
		map[6][2] = Ҫ;
		map[6][3] = Ǯ;
		map[6][4] = ��;
		map[6][5] = Ҫ;
		map[6][6] = ��;
	}
	//��
	if (map[r + vr][c + vc] == ��) {
		map[2][2] = һ;
		map[2][3] = ��;
		map[2][4] = ��;
		map[2][5] = ��;
		map[2][6] = ��;
		map[2][7] = ��;

	}

	return 0;
}

