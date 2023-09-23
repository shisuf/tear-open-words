#include"fireW.h"

//烟花结构
struct FIRE {
	int r, max_r;//烟花半径
	int x, y;//中心点坐标
	int height, width;//图片宽高
	int xy[240][240];//像素
	int cen_x, cen_y;
	bool show;
	bool draw;
	DWORD t1, t2, dt;
	IMAGE img;
}Fire[FIRE_NUM];

//子弹结构
struct JET {
	int hx, hy;//最高的
	int x, y;
	int xx, yy;
	int height;
	bool shoot;
	DWORD t1, t2, dt;
	int bright[50][20], dark[50][20];
	int n;
	IMAGE img[2];
}Jet[FIRE_NUM];

int Fmain() {

	initgraph(W_WIDTH, W_HEIGHT);
	//写入素材
	load_Resource("./fire", "./fire/flower.jpg", "JPG", IDB_BITMAP1);
	load_Resource("./fire", "./fire/shoot.jpg", "JPG", IDB_BITMAP2);
	load_Resource("./fire", "./fire/bomb.wav", "WAVE", IDR_WAVE1);
	//加载图片
	Load();
	for (int i = 0; i < 240; i++) Fire[4].xy[i][0] = 0;
	for (int i = 0; i < FIRE_NUM; i++) Init(i);

	DWORD* pMem = GetImageBuffer();		// 获取窗口显存指针
	BeginBatchDraw();
	mciSendString("open ./fire/bomb.wav alias b", 0, 0, 0);

	
	while (1) {		
		FlushBatchDraw();
		cleardevice();
		Shoot(rand() % 13, pMem);
		//Shoot(4, pMem);
		for (int i = 0; i < FIRE_NUM; i++)if (Fire[i].show || Fire[i].draw || Jet[i].shoot) Shoot(i, pMem);
		Sleep(70);
	};

	EndBatchDraw();
	closegraph();
	return 0;
}

//初始化
void Init(int i)
{
	Fire[i].width = 240;
	Fire[i].height = 240;
	Fire[i].x = 0;
	Fire[i].y = 0;
	Fire[i].r = 120;
	Fire[i].show = false;
	Fire[i].draw = false;
	Fire[i].cen_x = rand() % 20 + 100;
	Fire[i].cen_y = rand() % 20 + 100;
	Fire[i].max_r = rand() % 20 + 100;
	Fire[i].dt = 20;
	Fire[i].t1 = timeGetTime();

	Jet[i].x = 0;
	Jet[i].y = 0;
	Jet[i].hx = 0;
	Jet[i].hy = 0;
	Jet[i].height = 0;
	Jet[i].t1 = timeGetTime();
	Jet[i].dt = rand() % 10;
	Jet[i].shoot = false;
	Jet[i].n = 0;
}

void load_Resource(const char* dir,const char* file,const char* type,int nameId) {
	HMODULE gmodule = GetModuleHandle(NULL);
	if (gmodule != NULL) {
		HRSRC hrsrc = FindResource(gmodule, MAKEINTRESOURCE(nameId), type);
		if (hrsrc != NULL) {
			DWORD dwSize = SizeofResource(gmodule, hrsrc);
			HGLOBAL g = LoadResource(gmodule, hrsrc);
			if (g != NULL && dwSize != 0) {
				LPVOID lpResData = LockResource(g);
				if (lpResData != NULL) {
					
					CreateDirectory(dir, NULL);
					HANDLE hFile = CreateFile(file, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
					DWORD dwSizeW = 0;
					WriteFile(hFile, lpResData, dwSize, &dwSizeW, NULL);
					FreeResource(g);
					CloseHandle(hFile);
				}
			}
		}
	}
}
//加载图片
void Load()
{
	//加载像素点
	IMAGE fm, gm;
	loadimage(&fm, "./fire/flower.jpg");
	for (int i = 0; i < FIRE_NUM; i++) {
		SetWorkingImage(&fm);
		getimage(&gm, i * 240, 0, 240, 240);
		Fire[i].img = gm;
		SetWorkingImage(&gm);
		for (int j = 0; j < 240; j++)
			for (int k = 0; k < 240; k++)
				Fire[i].xy[j][k] = getpixel(j, k);	
	}
	//加载烟花弹
	IMAGE sm;
	loadimage(&sm, "./fire/shoot.jpg");
	for (int i = 0; i < FIRE_NUM; i++) {
		SetWorkingImage(&sm);
		int n = rand() % 5;
		getimage(&Jet[i].img[0], n * 20, 0, 20, 50);//暗
		getimage(&Jet[i].img[1], (n + 5) * 20, 0, 20, 50);

		SetWorkingImage(&Jet[i].img[0]);//暗
		for (int j = 0; j < 50; j++)
			for (int k = 0; k < 20; k++)
				Jet[i].dark[j][k] = getpixel(k, j);

		SetWorkingImage(&Jet[i].img[1]);//亮
		for (int j = 0; j < 50; j++)
			for (int k = 0; k < 20; k++)
				Jet[i].bright[j][k] = getpixel(k, j);
	}
	SetWorkingImage();
}

void Shoot(int i, DWORD* pMem)
{
	//初始化
	if (!Fire[i].show && !Fire[i].draw && !Jet[i].shoot) {
		Jet[i].x = (i)*60 + 120;
		Jet[i].y = W_HEIGHT -240;
		Jet[i].hy = rand() % 200 + 130;
		Jet[i].hx = Jet[i].x;
		Jet[i].t2 = timeGetTime();
		Jet[i].shoot = true;
	}
	//初始化后
	if (!Fire[i].show && !Fire[i].draw && Jet[i].shoot) {
		if (Jet[i].shoot) {
			//putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n]);//画图
			Jet[i].xx = Jet[i].x;
			Jet[i].yy = Jet[i].y;
			for (int j = 0; j < 50; j++) {
				for (int k = 0; k < 20; k++) {
					if (Jet[i].n == 1) {
						int b = Jet[i].bright[j][k] & 0xff;
						int g = (Jet[i].bright[j][k] >> 8) & 0xff;
						int r = (Jet[i].bright[j][k] >> 16);
						if (r > 0x3 && g > 0x3 && b > 0x3)
							pMem[Jet[i].yy * W_WIDTH + Jet[i].xx] = BGR(Jet[i].bright[j][k]);	
					}
					else 
					{
						int b = Jet[i].dark[j][k] & 0xff;
						int g = (Jet[i].dark[j][k] >> 8) & 0xff;
						int r = (Jet[i].dark[j][k] >> 16);
						if (r > 0x3 && g > 0x3 && b > 0x3)
							pMem[Jet[i].yy * W_WIDTH + Jet[i].xx] = BGR(Jet[i].dark[j][k]);
					}
					Jet[i].xx++;
				}
				Jet[i].xx = Jet[i].x;
				Jet[i].yy++;
			}

			if (Jet[i].y > Jet[i].hy) {
				Jet[i].y -= 10;
				Jet[i].n = Jet[i].n == 1 ? 0 : 1;
			}
			else {
				Fire[i].x = Jet[i].x - 120;
				Fire[i].y = Jet[i].y - 120;
				Jet[i].shoot = false;
				Fire[i].draw = true;
			}
		}
	}
	//烟花爆炸
	if (Fire[i].draw) {
		if(!Fire[i].show)
		mciSendString("play b from 0", 0, 0, 0);
		Fire[i].show = true;

		Fire[i].cen_x = Fire[i].x;
		Fire[i].cen_y = Fire[i].y;
		for (int j = 0; j < 240; j++) {
			for (int k = 0; k < 240; k++) {
				int b = Fire[i].xy[j][k] & 0xff;
				int g = (Fire[i].xy[j][k] >> 8) & 0xff;
				int r = (Fire[i].xy[j][k] >> 16);
				if (r > 0x20 && g > 0x20 && b > 0x20){
					pMem[Fire[i].cen_y*W_WIDTH+Fire[i].cen_x] = BGR(Fire[i].xy[j][k]);	// 显存操作绘制烟花
					}
				Fire[i].cen_x++;
			}
			Fire[i].cen_x = Fire[i].x;
			Fire[i].cen_y++;
		}

		if (Fire[i].dt > 0)
			Fire[i].dt--;
		else {
			Fire[i].dt = 20;
			Fire[i].show = false;
			Fire[i].draw = false;
		}

	}

}
