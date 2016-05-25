#include "public.h"

uchar GetLeanAngleTest(uchar flag)
{
	int iXLeanAngle, iYLeanAngle, iZLeanAngle;
	int count = 0;
	int iResult = 0;
	
	ScrClrLine(2, 7);
	ScrDispStr(0,2,1,"Sensor测试", "Sensor Test");
	if(flag==1)ScrDispStr(0,4,1,"正常放置", "Put Normal");
	if(flag==2)ScrDispStr(0,4,1,"倒置","Upside");
	if(flag==3)ScrDispStr(0,4,1,"左倾","Lean Left");
	if(flag==4)ScrDispStr(0,4,1,"右倾","Lean Right");
	if(flag==5)ScrDispStr(0,4,1,"后倾","BackForward");
	if(flag==6)ScrDispStr(0,4,1,"前倾","HeadForward");
	

	TimerSet(1,100);
	while ( TimerCheck(1) ) 
    {

		  GetLeanAngle(&iXLeanAngle, &iYLeanAngle, &iZLeanAngle);
		  ScrClrLine(6,7);	
		  ScrPrint(0,6,0, "XAngle=%d", iXLeanAngle);
		  ScrPrint(64,6,0, "YAngle=%d", iYLeanAngle);
		  ScrPrint(0,7,0, "ZAngle=%d", iZLeanAngle);

		  if (!kbhit()) break;
		  
		  if (flag == 1) 
		  {
			  if (abs(iXLeanAngle)<30 && abs(iXLeanAngle)<30 && iZLeanAngle > 60)
			  {
				  iResult++; 
			  }
		  }
		  else if(flag == 2)
		  {
			  if (abs(iXLeanAngle)<30 && abs(iXLeanAngle)<30 && iZLeanAngle < -40)//倒置的情况下，不利于人眼观察屏幕，Z轴与水平面的夹角较难达到-70度以上，故将Z轴夹角的阈值调整为-40，则认为是倒置
			  {
				  iResult++;
			  }
		  }
		  else if(flag == 3)
		  {
			  if (abs(iYLeanAngle)<30 && abs(iZLeanAngle)<30 && iXLeanAngle < -60)
			  {
				  iResult++;
			  }

		  }
		  else if(flag == 4)
		  {
			  if (abs(iYLeanAngle)<30 && abs(iZLeanAngle)<30 && iXLeanAngle > 60)
			  {
				  iResult++;
			  }
		  }
		  else if(flag == 5)
		  {
			  if (abs(iXLeanAngle)<30 && abs(iZLeanAngle)<30 && iYLeanAngle < -60)
			  {
				  iResult++;
			  }
		  }
		  else if(flag == 6)
		  {
			  if (abs(iXLeanAngle)<30 && abs(iZLeanAngle)<30 && iYLeanAngle > 60)
			  {
				  iResult++;
			  }
		  }
		  else return 0xff;
		  if (iResult >=3) break;
    }

	ScrClrLine(6,7);
    if (iResult >=3) 
	{
		ScrDispStr(0,6,1,"测试成功!","TEST OK!");
		BeepOK();
		DelayMs(500);
		return 0;
	}
	else
	{
		ScrDispStr(0,6,1,"测试失败!","TEST FAIL!");
		BeepFail();
		DelayMs(500);
		return 1;
	}   
	return 1;
    
	
}

uchar GSensorTest(uchar flag)
{
	uchar ucRet;
	ucRet=GetLeanAngleTest(1);
	if (ucRet) goto ENDERR;

	ucRet=GetLeanAngleTest(3);
	if (ucRet) goto ENDERR;
	
	ucRet=GetLeanAngleTest(6);
	if (ucRet) goto ENDERR;
	return 0;
ENDERR:
	return 1;
}



void GetLeanAngleTest1(void)
{
	GetLeanAngleTest(1);
}
void GetLeanAngleTest2(void)
{
	GetLeanAngleTest(2);
}
void GetLeanAngleTest3(void)
{
	GetLeanAngleTest(3);
}
void GetLeanAngleTest4(void)
{
	GetLeanAngleTest(4);
}
void GetLeanAngleTest5(void)
{
	GetLeanAngleTest(5);
}
void GetLeanAngleTest6(void)
{
	GetLeanAngleTest(6);
}





void GSensorTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-正常",		"1-Normal",		GetLeanAngleTest1,},
		{"2-倒置",		"2-Upside",		GetLeanAngleTest2,},
		{"3-左倾",		"3-Left",		GetLeanAngleTest3,},
		{"1-右倾",		"1-Right",		GetLeanAngleTest4,},
		{"2-后倾",		"2-Back",		GetLeanAngleTest5,},
		{"3-前倾",		"3-Head",		GetLeanAngleTest6,},
	};

	if(TestModule[T_GSENSOR]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1, "无Sensor模块", "No Sensor Module");
		getkey();
		return ;
	}
	
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"重力感应单项测试","gSensor Test",3);

}

