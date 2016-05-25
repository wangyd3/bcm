#include "public.h"

#ifdef WIN32
int ScanOpen(void)
{
	return 0;
}
int  ScanRead (uchar *buf,  uint size)
{
	return 0;
}
void ScanClose (void)
{

}
#endif

void ScanTest1(void)
{
	ScanTest(1);
}

void ScanTest2(void)
{
	ScanTest(2);
}

void ScanTest3(void)
{
	ScanTest(3);
}

void ScanTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",			"1-Single",			ScanTest1,},
		{"2-循环",			"2-Cycle",			ScanTest2,},
		{"3-普通",			"3-Common",			ScanTest3,}
	};

	if(TestModule[T_SCAN]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"本机无扫描模块", "No Scan Module");
		getkey();
		return;
	}
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"扫描单项测试","Scan Test",3);
	
}


uchar ScanTest(uchar flag)
{
	int iRet;
	uchar ucBuf[500];
	uchar BarCodeStr1[]="001234567895";
	uchar BarCodeStr2[]="hello pax!!";
	uchar BarCodeStr3[]="1234567890005";
	int TotalNum=0,FailNum=0;
	
	ScrCls();
	iRet=ScanOpen();
	if(iRet)
	{
		if(iRet==-2)return 0xff;
		else 
		{
			sprintf(gErrnoMsg,"ScanOpen=%d",iRet);
			ScrPrint(0,4,1,"ScanOpen Fail %d ",iRet);
			ScanClose();
			getkey();
			return 1;
		}
	}
	if(flag==0)ScrDispStr(0,2,1,"扫描测试","Scan Test  ");
	if(flag==1)ScrDispStr(0,0,1,"单次测试","Single Test");
	if(flag==2)ScrDispStr(0,0,1,"循环测试","Cycle Test ");


	while(1)
	{
		while(2)
		{
			
			if(flag==2)
			{
				if(CancelCheck())break;
				ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
			}
			ScrClrLine(2,7);
			memset(ucBuf,0,sizeof(ucBuf));
			TimerSet(2,100);
			ScrDispStr(0,4,1,"请扫描条码...","Pls Scan Code...");
			while(TimerCheck(2))
			{
				if(CancelCheck())break;
				iRet = ScanRead(ucBuf,sizeof(ucBuf)-1);
				if (iRet > 0) break;
			}
			ScrClrLine(2,7);
			ScrPrint(0,4,1,"%d,%s",iRet,ucBuf);
			sprintf(gErrnoMsg,"%d,%s",iRet,ucBuf);
			if(flag==3)
			{
				ScrPrint(0,2,1,"ScanRead=%d",iRet);
				getkey();
				goto ENDOK;
			}
			if(ucBuf[iRet-2]=='\r' || ucBuf[iRet-2]=='\n' )//把\r\n部分省略
			{
				ucBuf[iRet-2]=0;
				ucBuf[iRet-1]=0;
				iRet=iRet-2;
			}

			if( iRet==(int)strlen(ucBuf) && 
				(strcmp(ucBuf,BarCodeStr1)==0 || strcmp(ucBuf,BarCodeStr2)==0 || strcmp(ucBuf,BarCodeStr3)==0) )
			{
				ScrPrint(0,2,1,"Test OK");
				BeepOK();
				DelayMs(500);
				if(flag==0 || flag==1)goto ENDOK;
				else continue;
			}
			else
			{
				ScrPrint(0,2,1,"Test Fail %d ",iRet);
				BeepFail();
				DelayMs(500);
				FailNum++;
				if(flag==0 || flag==1)goto ENDERR;
				else continue;
			}
		}
		ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",TotalNum);
		ShowSuccessRate(TotalNum,FailNum);
		if(kbflush(),getkey()==KEYCANCEL)break;
	}
	

ENDOK:
	ScanClose();
	return 0;
ENDERR:
	ScanClose();
	return 1;
	
}

uchar ScanTestCyc(ulong Loops)
{
	int iRet;
	uchar ucBuf[100];

	ScrCls();
	ScrDispStr(0,0,0x81,"扫描煲机  %6d","SCAN LOOP %6d",Loops);
	ScanOpen();
	TimerSet(2,30);
	while(TimerCheck(2))
	{
		memset(ucBuf,0,sizeof(ucBuf));
		ScrDispStr(0,2,0x01,"启动扫描...","Start Scan...");
		iRet = ScanRead(ucBuf,sizeof(ucBuf)-1);
	}
	ScanClose();
	return 0;
}


