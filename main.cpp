#include"main.h"

/*************************************************/
int main() {

	char buff[2048];
	//初始化窗口
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	settextstyle(SIZE_FONT,0, STYLE_FONT);
	settextcolor(COLOR_FONT);
	setbkmode(TRANSPARENT);
	//写入加载资源文件
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
		if (map[4][15] == 我 && map[3][15] ==深) {
			map[3][8] = 海;
			map[3][9] = 面;
			map[3][10] = 太;
			map[3][11] = 深;
			map[3][12] = 了;
		}
		if (map[3][11] == 浅) {
			for (int i = 1; i < 10; i++) {
				map[i][16] = 空;
				map[i][15] = 空;
				map[i][17] = 空;
				map[i][18] = 空;
				map[i][19] = 空;
				map[i][20] = 空;
			}
			map[3][10] = 下;
			map[3][11] = 降;
		}
		if (map[13][24] == 关) {
			map[13][24] = 空;
			map[14][24] = 空;
			map[15][24] = 空;

		}
		if (map[15][24] == 空 && map[12][26] == 箱 && c > 22 && r > 12) {
			mciSendString("close bk", 0, 0, 0);
			break;
		} 
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				outtextxy(j*textwidth(text[map[i][j]]), i * textheight(text[map[i][j]]), text[map[i][j]]);
				outtextxy(600, 30, "SHIFT：拆");
				outtextxy(600, 60, "CTRL：拉");
				outtextxy(600, 90, "记得关灯");
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
	//拆字
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000 && map[r - vr][c - vc] == 空) {
		if (map[r + vr][c + vc] == 波) {
			map[r + vr][c + vc] = 皮;
			map[r][c] = 水;
			map[r - vr][c - vc] = 我;
			r -= vr;
			c -= vc;
		}
		if (map[r + vr][c + vc] == 钱) {
			map[r + vr][c + vc] = 戋;
			map[r][c] = 金;
			map[r - vr][c - vc] = 我;
			r -= vr;
			c -= vc;
		}

	}
	//拉字
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && map[r + vr][c + vc] == 空 && (
			map[r - vr][c - vc] == 箱 ||
			map[r - vr][c - vc] == 桌 ||
			map[r - vr][c - vc] == 床 ||
			map[r - vr][c - vc] == 石 ||
			map[r - vr][c - vc] == 戋 ||
			map[r - vr][c - vc] == 水 ||
			map[r - vr][c - vc] == 皮 ||
			map[r - vr][c - vc] == 浅 ||
			map[r - vr][c - vc] == 深 ||
			map[r - vr][c - vc] == 破 ||
			map[r - vr][c - vc] == 关

			)) {
		map[r][c] = map[r - vr][c - vc];
		map[r - vr][c - vc] = 空;
		map[r + vr][c + vc] = 我;
		r += vr;
		c += vc;
		mciSendString("play move from 0", 0, 0, 0);
	}
	else
	if (map[r + vr][c + vc] == 空) {
		map[r + vr][c + vc] = 我;
		map[r][c] -= 我;
		r += vr;
		c += vc;
		mciSendString("play move from 0", 0, 0, 0);
	}
	else
	if (map[r + vr][c + vc] == 门) {
		map[r + vr][c + vc] = 我;
		map[r][c] -= 我;
		r += vr;
		c += vc;
		mciSendString("play door from 0", 0, 0, 0);
	}
	else//推字
	if ( 
			map[r + vr][c + vc] == 桌 || 
			map[r + vr][c + vc] == 箱 || 
			map[r + vr][c + vc] == 皮 ||
			map[r + vr][c + vc] == 水 || 
			map[r + vr][c + vc] == 戋 ||
			map[r + vr][c + vc] == 浅 ||
			map[r + vr][c + vc] == 石 ||
			map[r + vr][c + vc] == 床 ||
			map[r + vr][c + vc] == 深 ||
			map[r + vr][c + vc] == 破 ||
			map[r + vr][c + vc] == 关

			) {
		if (map[r + vr * 2][c + vc * 2] == 空) {
			map[r + vr * 2][c + vc * 2] = map[r + vr][c + vc];
			map[r + vr][c + vc] = 我;
			map[r][c] -= 我;
			r += vr;
			c += vc;
			mciSendString("play move from 0", 0, 0, 0);
		}
		//破墙
		if (map[r + vr][c + vc] == 破 && map[r + vr * 2][c + vc * 2] == 墙) {
			map[r + vr * 2][c + vc * 2] = 空;
			map[r + vr][c + vc] = 我;
			map[r][c] -= 我;
			r += vr;
			c += vc;
			mciSendString("play bomb from 0", 0, 0, 0);
		}
		//合字
		if (map[r + vr][c + vc] == 水 && map[r + vr * 2][c + vc * 2] == 戋) {
			map[r + vr * 2][c + vc * 2] = 浅;
			map[r + vr][c + vc] = 我;
			map[r][c] -= 我;
			r += vr;
			c += vc;
		}
		if (map[r + vr][c + vc] == 石 && map[r + vr * 2][c + vc * 2] == 皮) {
			map[r + vr * 2][c + vc * 2] = 破;
			map[r + vr][c + vc] = 我;
			map[r][c] -= 我;
			r += vr;
			c += vc;
		}
	}
	//包
	if (map[r + vr][c + vc] == 包) {
		map[6][2] = 要;
		map[6][3] = 钱;
		map[6][4] = 不;
		map[6][5] = 要;
		map[6][6] = 命;
	}
	//视
	if (map[r + vr][c + vc] == 视) {
		map[2][2] = 一;
		map[2][3] = 起;
		map[2][4] = 看;
		map[2][5] = 烟;
		map[2][6] = 花;
		map[2][7] = 吧;

	}

	return 0;
}

