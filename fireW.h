#pragma once
#include "resource1.h"
#include<easyx.h>
#include<mmsystem.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include "winres.h"
#pragma comment(lib,"winmm.lib")

//�̻�����
#define FIRE_NUM 13

//����
#define W_WIDTH 960
#define W_HEIGHT 640

//�̻��ṹ
struct FIRE;

//�ӵ��ṹ
struct JET;

//
void Init(int);
void Load();
void Shoot(int, DWORD* pMenu);
extern void load_Resource(const char* dir, const char* file, const char* type, int nameId);
extern int Fmain();