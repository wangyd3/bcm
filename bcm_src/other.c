#include "public.h"

uchar VerTest(uchar flag)
{
	return 0;
}

uchar PaperBackTest(uchar flag)
{
	ScrClrLine(2,7);
	ScrDispStr(0,2,1,"��ֽ���Ƿ�OK?", "PaperBack Is Ok?");
	ScrDispStr(0,4,1,"[ȷ��]--��", "[ENTER]--YES");
	ScrDispStr(0,6,1,"[ȡ��]--��", "[CANCEL]--NO");
	
	if(getkey()==KEYENTER)
		return 0;
	else
		return 1;
}


static uchar level_1;
static uchar level_2;
uchar BatteryTest(uchar flag)
{

	int n = 0;
	int on;
	
	ScrClrLine(2,7);
	if( TestModule[T_BATTERY]=='N' ) 
    {
        ScrDispStr(0,4,1,"�޳����", "No Battery");
		getkey();
        return 0xff;      
    }

	while(flag==1) //flag==1��ʱ����Ҫ������
	{
		if(CancelCheck())return 1;
		level_1 = BatteryCheck();
		if(level_1>4)
		{
			n = 0;
			ScrDispStr(0,2,1,"�벦���%d","PULL POWER%d",level_1);
			continue;
		}
		if(++n > 5)return 0;
		DelayMs(30);
	}
	
	ScrDispStr(0,2,1,"����ģ�����","Power Test");

	if(gBatChange==0 && flag==0)
	{
		level_2=BatteryCheck();
		ScrDispStr(0,4,1,"��ѹ�ȼ�:%d","Voltage :%d",level_2);
		if(level_2 <= 6)goto ENDOK;
		else goto ENDERR;
	}

	while(1)
	{
		if(CancelCheck())
		{
			sprintf(gErrnoMsg,"User Cancel");
			goto ENDERR;						
		}
		level_2= BatteryCheck();
		on = OnBase();
		if(level_2>4 && level_1<=4 && on!=0)
		{
			if(++n>5)goto ENDOK;
		}
		else
		{
			n = 0;
			sprintf(gErrnoMsg,"POWER = %d,%d,%d",level_1,level_2,on);
			ScrDispStr(0,4,1,"������%d,%d,%d","INSER POWER%d,%d,%d",level_1,level_2,!(!on));
		}
		DelayMs(50);
	}	

ENDERR:
	ScrDispStr(0,6,1,"    ����ʧ��    ","   Test Fail   ");
	BeepFail();
	DelayMs(500);
	return 1;

ENDOK:
	ScrDispStr(0,4,1,"    ���Գɹ�    ","    Test OK   ");
	BeepOK();
	DelayMs(500);
	return 0;
}

void check_battery(void)
{
	int level,n;
	ScrCls();
	ScrPrint(0,0,1,"check battery");
	
	if(TestModule[T_BATTERY]=='N' )
    {
        return;
    }

	while(1)
	{
		if(CancelCheck())break;
		level=BatteryCheck();
		ScrDispStr(0,2,1,"��ѹ�ȼ�:%d","Voltage :%d",level);
		if(level <= 2)
		{
			if(n++ > 5)BeepFail();
		}
		else
		{
			n = 0;
		}
		DelayMs(200);
	}

}

uchar BaseTest(uchar flag)
{
	uchar level,on,fid;
	int result = 0,n=0,n1=0;

	ScrCls();
	ScrDispStr(32,0,1,"��������","BASE TEST");
	while(1)
	{
		
		if(CancelCheck())return 1;
		ScrClrLine(2,7);
		level = BatteryCheck();
		on = OnBase();
		ScrPrint(0,2,1,"Battery=%d,%d",level,result);
		ScrPrint(0,4,1,"OnBase=%d",on);
		ScrDispStr(0,6,1,"ȥ���������","NoPower&OnBase");
		if(level<=4)
		{
			n1 = 0;
			if(n++ > 5)
				result |= 0x01;
		}
		
		if(level>4 && on==0)
		{
			n = 0;
			if(n1++ > 5)
				result |= 0x02;
		}

		if(result==3)
		{
			ScrClrLine(6,7);
			ScrPrint(0,6,1,"BASE TEST OK");
			gBaseChange = 'Y';
			fid = open(F_BASE_CHANGE,O_RDWR);
			if(fid<0)ShowFileCtrlErr(__LINE__);
			else if( write(fid,&gBaseChange,1)!=1 )
				ShowFileCtrlErr(__LINE__);
			close(fid);
			BeepOK();
			if(flag)getkey();
			return 0;
		}
		DelayMs(50);
	}

}

void reset_base_change(void)
{
	int fid;
	gBaseChange = 'N';
	fid = open(F_BASE_CHANGE,O_RDWR);
	if(fid<0)ShowFileCtrlErr(__LINE__);
	else if( write(fid,&gBaseChange,1)!=1 )
		ShowFileCtrlErr(__LINE__);
	close(fid);
}

void BatteryTest1(void)
{
	gBatChange = 1;
	BatteryTest(1);
	BatteryTest(0);
}

void BaseTest1(void)
{
	BaseTest(1);
}

void BatteryTestAll(void)
{
	TESTSINGLE_ITEM TestItem[]={
		{"1-ֱ��",	"1-InLine",		BatteryTest1,},
		{"2-����",	"2-BASE ",		BaseTest1,},
		{"3-��λ",	"3-RESET ",		reset_base_change,}
	};
	uchar TestNum;
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"������","Change Test",2);
}