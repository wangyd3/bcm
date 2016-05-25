#include "public.h"


void BeepTest1(void)
{
	BeepTest(1);
}
void BeepTest2(void)
{
	BeepTest(2);
}

void BeepTestAll(void)
{
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",	"1-Single",		BeepTest1,},
		{"2-循环",	"2-Cycle ",		BeepTest2,}
	};
	uchar TestNum;
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"蜂鸣器测试","BUZZER TEST",3);
}


// flag: 0-整机 1-单次 2-循环 
uchar BeepTest(uchar flag)
{
	uchar i,ucRet;
	ushort TotalNum=0;
	
	ScrCls();
	if(flag == 1) ScrDispStr(0,0,1,"单次测试", "Single Test");
	if(flag == 2) ScrDispStr(0,0,1,"循环测试", "Cycle Test");
	TotalNum=0;
	while(flag)
	{
		if(CancelCheck()) break;
		if(flag==2)	ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
		ScrDispStr(0,2,1,"蜂鸣器测试", "  BUZZER TEST   ");
		for(i=0;i<7;i++)
		{
			ScrDispStr(0,4,1,"  鸣声%d...","  Beep%d...",i+1);
			Beef(i,100);
			DelayMs(100);
		}
		if(flag==1) break;
		DelayMs(200);
	}
	
	if(flag==0 || flag==1)
	{
		ucRet=JudgeState("蜂鸣器","Beep");
		if(ucRet==KEYENTER)
			return 0;
		else
			return 1;
	}
	return 0;

}


