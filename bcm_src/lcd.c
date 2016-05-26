#include "public.h"

// flag: 0-整机 1-单次 2-循环
uchar LcdTest(uchar flag)
{
	uint TotalNum=0;
	uchar ucRet,i;
	ScrCls();
	
	while(1)
	{
		
		if(flag==0)	ScrDispStr(0,2,1,"LCD测试",	"LCD Test");
		if(flag==1)	ScrDispStr(0,0,1,"单次测试",	"Single Test");
		if(flag==2) 
		{
			if(CancelCheck()) return 1;
			ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
		}
		kbflush();
		
		//彩屏
		if(gIsColorLcd==1 || TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900 || InfoOut[C_LCD][0]!=0)
		{
			CLcdGetInfo(&stLcdInfo);
			ScrBackLight(0);
			for(i=0; i<=8; i++)
			{
				if(i==2) ScrSetIcon(i,6);
				else ScrSetIcon(i,1);
			}
			ScrCls();
			CLcdBgDrawBox(0,0,stLcdInfo.width-1,stLcdInfo.height-1,C_BLUE);
			DelayMs(1000);
			CLcdBgDrawBox(0,0,stLcdInfo.width-1,stLcdInfo.height-1,C_GREEN);
			DelayMs(1000);
			CLcdBgDrawBox(0,0,stLcdInfo.width-1,stLcdInfo.height-1,C_RED);
			DelayMs(1000);

			ScrBackLight(1);
			for(i=0; i<=8; i++)
			{
				ScrSetIcon(i,0);
			}
			CLcdBgDrawBox(0,0,stLcdInfo.width-1,stLcdInfo.height-1,C_BLACK);
			DelayMs(1000);
			CLcdBgDrawBox(0,0,stLcdInfo.width-1,stLcdInfo.height-1,C_WHITE);
			DelayMs(1000);
		}
		else//非彩屏
		{
			//关背光,开图标,黑屏
			for(i=1; i<=8; i++)
			{
				if(i==2) ScrSetIcon(i,6);
				else ScrSetIcon(i,1);
				ScrPrint(0,i-1,0x81,"                ");
			}			
			ScrBackLight(0);
			DelayMs(1000);

			//开背光,关图标,白屏
			for(i=1; i<=8; i++)
			{
				ScrSetIcon(i,0);
				ScrPrint(0,i-1,0x01,"                ");
			}
			ScrBackLight(1);
			DelayMs(1000);
		}
		if(flag==3)return 0;//煲机
		if(flag <= 1)
		{
			ucRet=JudgeState("屏幕","LCD");
			if(ucRet== KEYENTER)
				return 0;
			else
				return 1;
		}
	}
	return 1;
	
}

void LcdBackTest(void)
{
	while(1)
	{
		if(CancelCheck())break;
		ScrCls();
		ScrDispStr(0,4,1,"开背光","Back Light On");
		ScrBackLight(2);
		DelayMs(1500);

		ScrCls();
		ScrDispStr(0,4,1,"关背光","Back Light Off");
		ScrBackLight(0);
		DelayMs(1500);
	}
	ScrBackLight(1);
}

void LcdTest1(void)
{
	LcdTest(1);
}
void LcdTest2(void)
{
	LcdTest(2);
}

void LcdTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",	"1-Single",		LcdTest1},
		{"2-循环",	"2-Cycle",		LcdTest2},
		{"3-背光",	"3-BackLED",	LcdBackTest}
	};

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"屏幕单项测试","LCD Test",3);

}


uchar LedTest(uchar flag)
{
	uint TotalNum=0;
	uchar ucRet;
	kbflush();
	ScrClrLine(2,7);
	while(1)
	{
		if(flag==2)
		{
			if(CancelCheck())break;
			ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);		
		}
		ScrDispStr(0,2,1,"LED测试","LED Test");	
		ScrClrLine(4,7);
		ScrDispStr(0,4,1, "  蓝灯亮 ", " BLUE Light ");			
		PiccLight(PICC_LED_BLUE, 1);
		if(flag<=1)
			WaitEsc(0,1);
		else
			DelayMs(1000);
		PiccLight(PICC_LED_BLUE, 0);

		ScrClrLine(4,7);
		ScrDispStr(0,4,1,"  黄灯亮 ", " YELLOW Light ");			
		PiccLight(PICC_LED_YELLOW, 1);
		if(flag<=1)
			WaitEsc(0,1);
		else
			DelayMs(1000);
		PiccLight(PICC_LED_YELLOW, 0);
		
		ScrClrLine(4,7);
		ScrDispStr(0,4,1,"  绿灯亮 ", " GREEN Light ");			
		PiccLight(PICC_LED_GREEN, 1);
		if(flag<=1)
			WaitEsc(0,1);
		else
			DelayMs(1000);
		PiccLight(PICC_LED_GREEN, 0);
		
		ScrClrLine(4,7);
		ScrDispStr(0,4,1,"  红灯亮 ", " RED Light ");			
		PiccLight(PICC_LED_RED, 1);
		if(flag<=1)
			WaitEsc(0,1);
		else
			DelayMs(1000);
		PiccLight(PICC_LED_RED, 0);

		ScrClrLine(4,7);
		ScrDispStr(0,4,1,"  全部点亮 ", " Light All ");
		PiccLight(PICC_LED_ALL, 1);

		if(flag <= 1)
		{
			ucRet=JudgeState("蓝黄绿红","  LED  ");
			if(ucRet == KEYENTER)
			{			
				PiccLight(PICC_LED_ALL, 0);
				return 0;
			}
			else
			{
				PiccLight(PICC_LED_ALL, 0);
				return 1;
			}
		}
		else if(flag == 2)
		{
			DelayMs(1000);
			PiccLight(PICC_LED_ALL, 0);
			DelayMs(1000);
			continue;
		}
	}
	return 0;
}

void LedTest1(void)
{
	LedTest(1);
}
void LedTest2(void)
{
	LedTest(2);
}

void LedTestAll(void)
{
   	
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",	"1-Single",	LedTest1},
		{"2-循环",	"2-Cycle",	LedTest2}
	};

	if(TestModule[T_LED]=='N') 
    {
		ScrClrLine(2,7);
        ScrDispStr(0,4,1,"无射频LED灯", "No LED Module");
		getkey();
        return ;        
    }
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"LED灯测试","LED Test",3);
	
}
