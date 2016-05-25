#include "public.h"

static void ClockSetTest(void)
{
	uchar ucRet,tmps[32], xstr[32], NewTime[7];

	while(1)
	{
		ScrClrLine(2,7);
		ScrDispStr(0,2,1,"��׼ʱ��","Set Clock");
		ScrDispStr(0,4,1,"��ǰʱ��:","Current Time:");

		GetTime(xstr);
		sprintf((char*)tmps, "%02x/%02x/%02x %02x:%02x:%02x",
			(int)xstr[0], (uint)xstr[1], (uint)xstr[2],
			(int)xstr[3], (uint)xstr[4], (uint)xstr[5]);
		ScrPrint(0,6,0,(char*)tmps);
		if(getkey()==KEYCANCEL) return;
		ScrClrLine(2,7);
		ScrPrint(32,2,1,"YYMMDDHHMMSS");
		ScrDispStr(0,4,1,"��������ʱ��: ","Reset Time:   ");
		ScrGotoxy(8,6);
		if(GetString(tmps,0x61,12,12)){
			ScrDispStr(0,6,1,"��ȡ������ʱ��","Set Time Pause");
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
			ScrDispStr(0,4,1,"ʱ�����ô���:%d","SetTime Err:%d",ucRet);
			BeepFail();
			DelayMs(1000);
			return;
		}
		ScrDispStr(0,4,1,"��ʱ��OK??","New Time OK??");
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
		{"1-����",	"1-Single",		ClkTest1,	 },
		{"2-��ʱ",	"2-SetTime",	ClockSetTest,},
	};
	uchar TestNum;
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"ʱ�ӵ������","CLOCK TEST",3);
}



/************************************************************************
1.¼��ǰʱ��old_time
2.����ʱ�� \x99\x12\x31\x23\x59\x58 ���϶�ȡ���Ƚ϶�ȡ��ʱ�������õ�ʱ��
3.���ýӽ�2000���ʱ�� \x99\x12\x31\x23\x59\x59  10000ms�ڣ���ʱ���Ƿ��ߵ�200�ꡣ
4.��old_timeд��
һ��ʱ�Ӳ��ԣ���ʹʱ�����ԼΪ1~2s
flag 0- ����  1-����
/************************************************************************/

uchar ClkTest(uchar flag)
{
	uchar i,ucRet,ucBuf[20],old_time[7];

	ScrClrLine(2,7);
	ScrDispStr(0,2,1,"ʱ�Ӳ���...","Clock Test...");
	GetTime(old_time); 
	
	if(old_time[0] < 0x15 || old_time[0]>0x20){//ʱ�䲻����15�ꡫ20��֮�䱨��
		sprintf(gErrnoMsg,"It's %02x Year",old_time[0]);
		ScrDispStr(0,4,1,"ʱ���� %02x ��!!","It's %02x Year!!",old_time[0]);
		BeepFail(); 
		getkey();
		goto ENDERR;
	}
	  
	ucRet=SetTime((unsigned char *)"\x99\x12\x31\x23\x59\x58");//��ʱ�ػ���ʱ�ӻ���Ϊ2000��.
	if(ucRet){
		sprintf(gErrnoMsg,"SetTime=%d",ucRet);
		ScrDispStr(0,4,1,"ʱ�����ô���:%d","SetTime Err:%d",ucRet);
		BeepFail(); 
		getkey(); 
		goto ENDERR;
	}
	memset(ucBuf,0,sizeof(ucBuf));
	GetTime(ucBuf);
	if(memcmp(ucBuf,"\x99\x12\x31\x23\x59\x58",5)){
		sprintf(gErrnoMsg,"time compare 1999 error");
		ScrDispStr(0,4,1,"ʱ�ӶԱȴ���","Clock CMP ERR");	
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
		ScrDispStr(0,4,1,"ʱ�Ӳ��Դ���3","Clock Test ERR3");
		BeepFail(); 
		getkey();
		goto ENDERR;
	}else{
		if(old_time[5]>0x56)old_time[5]=0x59;
		else if((old_time[5]&0x0f) < 8)old_time[5]=old_time[5]+2;
		else old_time[5]=old_time[5]+16-10+2;//if((old_time[5]&0x0f) >= 8)old_time[5]=old_time[5]+8;//
		SetTime(old_time);
		BeepOK();
		ScrDispStr(0,4,1,"ʱ�Ӳ���OK","Clock Test OK!");
		DelayMs(500);
		goto ENDOK;
	}
	
ENDOK:
	return 0;
ENDERR:
	return 1;
}
