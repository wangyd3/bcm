#include "public.h"

static void ClockSetTest(void)
{
	uchar ucRet,tmps[32], xstr[32], NewTime[7];

	while(1)
	{
		ScrClrLine(2,7);
		ScrDispStr(0,2,1,"调准时钟","Set Clock");
		ScrDispStr(0,4,1,"当前时间:","Current Time:");

		GetTime(xstr);
		sprintf((char*)tmps, "%02x/%02x/%02x %02x:%02x:%02x",
			(int)xstr[0], (uint)xstr[1], (uint)xstr[2],
			(int)xstr[3], (uint)xstr[4], (uint)xstr[5]);
		ScrPrint(0,6,0,(char*)tmps);
		if(getkey()==KEYCANCEL) return;
		ScrClrLine(2,7);
		ScrPrint(32,2,1,"YYMMDDHHMMSS");
		ScrDispStr(0,4,1,"请输入新时间: ","Reset Time:   ");
		ScrGotoxy(8,6);
		if(GetString(tmps,0x61,12,12)){
			ScrDispStr(0,6,1,"已取消设置时间","Set Time Pause");
			DelayMs(1000);
			return;
		}
		NewTime[0] = (tmps[1] - 0x30) * 16 + (tmps[2] - 0x30);
		NewTime[1] = (tmps[3] - 0x30) * 16 + (tmps[4] - 0x30);
		NewTime[2] = (tmps[5] - 0x30) * 16 + (tmps[6] - 0x30);
		NewTime[3] = (tmps[7] - 0x30) * 16 + (tmps[8] - 0x30);
		NewTime[4] = (tmps[9] - 0x30) * 16 + (tmps[10] - 0x30);
		NewTime[5] = (tmps[11] - 0x30) * 16 + (tmps[12] - 0x30);

		ucRet = SetTime(NewTime);
		ScrClrLine(2,7);
		if(ucRet)
		{
			ScrDispStr(0,4,1,"时间设置错误:%d","SetTime Err:%d",ucRet);
			BeepFail();
			DelayMs(1000);
			return;
		}
		ScrDispStr(0,4,1,"新时间OK??","New Time OK??");
		GetTime(xstr);
		sprintf((char*)tmps, "%02x/%02x/%02x %02x:%02x:%02x",
			(uint)xstr[0], (uint)xstr[1], (uint)xstr[2],
			(uint)xstr[3], (uint)xstr[4], (uint)xstr[5]);
		ScrPrint(0,6,0,(char*)tmps);
		if(getkey()==KEYENTER)break;
		else continue;
	}
}

static void  ClkTest1(void)
{
	ClkTest(1);
}

void ClkTestAll(void)
{
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",	"1-Single",		ClkTest1,	 },
		{"2-调时",	"2-SetTime",	ClockSetTest,},
	};
	uchar TestNum;
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"时钟单项测试","CLOCK TEST",3);
}



/************************************************************************
1.录当前时间old_time
2.设置时间 \x99\x12\x31\x23\x59\x58 马上读取，比较读取的时间与设置的时间
3.设置接近2000年的时间 \x99\x12\x31\x23\x59\x59  10000ms内，看时间是否走到200年。
4.把old_time写回
一次时钟测试，致使时钟误差约为1~2s
flag 0- 整机  1-单次
/************************************************************************/

uchar ClkTest(uchar flag)
{
	uchar i,ucRet,ucBuf[20],old_time[7];

	ScrClrLine(2,7);
	ScrDispStr(0,2,1,"时钟测试...","Clock Test...");
	GetTime(old_time); 
	
	if(old_time[0] < 0x15 || old_time[0]>0x20){//时间不是在15年～20年之间报错
		sprintf(gErrnoMsg,"It's %02x Year",old_time[0]);
		ScrDispStr(0,4,1,"时间是 %02x 年!!","It's %02x Year!!",old_time[0]);
		BeepFail(); 
		getkey();
		goto ENDERR;
	}
	  
	ucRet=SetTime((unsigned char *)"\x99\x12\x31\x23\x59\x58");//此时关机，时钟会置为2000年.
	if(ucRet){
		sprintf(gErrnoMsg,"SetTime=%d",ucRet);
		ScrDispStr(0,4,1,"时钟设置错误:%d","SetTime Err:%d",ucRet);
		BeepFail(); 
		getkey(); 
		goto ENDERR;
	}
	memset(ucBuf,0,sizeof(ucBuf));
	GetTime(ucBuf);
	if(memcmp(ucBuf,"\x99\x12\x31\x23\x59\x58",5)){
		sprintf(gErrnoMsg,"time compare 1999 error");
		ScrDispStr(0,4,1,"时钟对比错误","Clock CMP ERR");	
		BeepFail(); 
		getkey();
		goto ENDERR;
	}
	SetTime((unsigned char *)"\x99\x12\x31\x23\x59\x59");
	for(i=0;i<100;i++){
		memset(ucBuf,0,sizeof(ucBuf));
		GetTime(ucBuf);
		if(!memcmp(ucBuf,"\x00\x01\x01\x00\x00\x00\x06",7)) break;
		DelayMs(100);
	}
	if(i==100){
		sprintf(gErrnoMsg,"time compare 2000 error");
		ScrDispStr(0,4,1,"时钟测试错误3","Clock Test ERR3");
		BeepFail(); 
		getkey();
		goto ENDERR;
	}else{
		if(old_time[5]>0x56)old_time[5]=0x59;
		else if((old_time[5]&0x0f) < 8)old_time[5]=old_time[5]+2;
		else old_time[5]=old_time[5]+16-10+2;//if((old_time[5]&0x0f) >= 8)old_time[5]=old_time[5]+8;//
		SetTime(old_time);
		BeepOK();
		ScrDispStr(0,4,1,"时钟测试OK","Clock Test OK!");
		DelayMs(500);
		goto ENDOK;
	}
	
ENDOK:
	return 0;
ENDERR:
	return 1;
}
