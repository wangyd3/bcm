#include "public.h"


void IcTest1(void)
{
	IcTest(1);
}	
void IcTest2(void)
{
	IcTest(2);
}


void IcTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",		"1-Single",	IcTest1,},
		{"2-循环",		"2-Cycle",	IcTest2,}
	};

	if(TestModule[T_IC]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"本机无IC模块", "No IC Module");
		getkey();
		return;
	}
	
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"IC卡单项测试","IC Card Test",3);

}

uchar IccSelectPSE(unsigned char slot)
{
    
	uchar ret = 0;
	APDU_SEND apdu_s;
	APDU_RESP apdu_r;

	apdu_s.Command[0] = 0x00;    
	apdu_s.Command[1] = 0xa4;
	apdu_s.Command[2] = 0x04;
	apdu_s.Command[3] = 0x00;
	apdu_s.Lc = 0x0e;
	apdu_s.Le = 256;
	memcpy ( apdu_s.DataIn, "1PAY.SYS.DDF01", 14 );
    
	ret = IccIsoCommand(slot, &apdu_s, &apdu_r);
	return ret;        
}



// flag: 0-整机 1-单次 2-循环 
uchar IcTest(uchar flag)
{
	uint TotalNum,FailNum;
	uchar count,slot,ucRet,ucRet1,BuffAtr[40];
	ScrClrLine(2,7);
	
	if(flag==1) ScrDispStr(0,0,1, "单次测试", "Single Test");
	if(flag==2) ScrDispStr(0,0,1, "循环测试", "Cycle Test ");

	TotalNum=0;
	FailNum=0; 
	count=0;
	while(1)
	{

		while(2)
		{
			
			if(flag==2)
			{
				if(TotalNum==10000)break;
				if(CancelCheck())break;
				ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
				ScrClrLine(2,7);
			}
			
			count=0;
			for(slot=0; slot<=(gSamNum+1); slot++)// 有3张卡/0通道/2通道/3通道 或者 2/3/4通道  所以需要+1
			{
				if(gIcNum==0 && slot==0)continue;
				if(slot==1) continue;
				ucRet=IccInit(slot,BuffAtr);
				ucRet1 = 255;
				if(!ucRet)ucRet1=IccSelectPSE(slot);
				IccClose(slot);
				if(ucRet || ucRet1){
					ScrPrint(0,(uchar)(6-gSamNum+slot),0,"SLOT%d: ERROR %02x,%02x",slot,ucRet,ucRet1);
					RECORD(1, "IC.slot%d.err=%02x,%02x",slot,ucRet,ucRet1);
					continue;
				}
				else{
					ScrPrint(0,(uchar)(6-gSamNum+slot),0,"SLOT%d: PASSED",slot);
				}				
				count++;
			}
			
			if( count<(gIcNum+gSamNum) )//
			{
				FailNum++; 
				BeepFail();
				DelayMs(200);
				if(flag<=1)return 1;
			}
			else
			{
				BeepOK();
				DelayMs(200);
				if(flag<=1)return 0;				
			}
		} //2
		ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",TotalNum);
		ShowSuccessRate(TotalNum,FailNum);	
		kbflush();
		if(getkey()==KEYCANCEL)break;
	}
	return 0;
}

