#include "public.h"


uchar ShowMagErr(uchar Line,uchar ucRet,char *Track1,char *Track2,char *Track3)
{
	
	int iRet1,iRet2,iRet3;
	//2004-11-17�����ŵ�1��������

	uchar Str[2][10]={
		"OK",
		"Fail"
	};

	char add1_data_T1[]="1=ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890(0987654321)ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//2005-04-25�����ŵ�1��2��3��������
	char add2_data_T1[]="123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567899ABCDEFGHIJKLMNOPQRSTUVWXYZ1230";
	char add2_data_T2[]="2345678901234567890123456789012345671";
	char add2_data_T3[]="11111111122222222223333333333444444444455555555556666666666777777777788888888889999999999000000000012341";
	//
	char data_T1[]="1TRACK1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ<>_=\\:@^1234567890ABCDEFGHIJK[PAX]";//76
	char data_T2[]="2=1234567890:1234567890<9876543210>12";//37
	char data_T3[]="3=1234567890:1234567890<9876543210>3824176509=2465138709:6253149087<7216954803>1234567890=:=<=>=12345678";//104
 

	ScrClrLine(Line,7);

	if ( !strcmp(data_T1,Track1) || !strcmp(add1_data_T1,Track1) || !strcmp(add2_data_T1,Track1) ){
		iRet1 = 0;//��������
	}
	else {
		iRet1 = 1;//���ݴ���
	}


	if( !strcmp(data_T2,Track2) || !strcmp(add2_data_T2,Track2) ){
		iRet2 = 0;
	}
	else{
		iRet2 = 1;
	}

	if( !strcmp(data_T3,Track3) || !strcmp(add2_data_T3,Track3) ){
		iRet3 = 0;
	}
	else{
		iRet3 = 1;
	}

	ScrClrLine(Line,(uchar)(Line+1));
	if(ucRet==0x07 && iRet1==0 && iRet2==0 && iRet3==0)
	{
		ScrDispStr(0,Line,1,"���ſ���ȷ","Read Mag OK");
		return 0;//3���ŵ�������	
	}
	else
	{
		sprintf(gErrnoMsg,"MagRead=0x%02x,#1%s,#2%s,#3%s",ucRet,Str[iRet1],Str[iRet2],Str[iRet3]);
		ScrPrint(0,Line,0,"Read Mag Error0x%02x",ucRet);
		ScrPrint(0,(uchar)(Line+1),0,"#1%s,#2%s,#3%s",Str[iRet1],Str[iRet2],Str[iRet3]);
		return 1;	
	}

}


// flag: 0-���� 1-���� 2-ѭ��
uchar MagTest(uchar flag)
{

	uint SuccessCnt,FailCnt,TotalCnt;
	uchar ucRet;
	char Track1[256],Track2[256],Track3[256];

	memset(Track1, 0x00, sizeof(Track1));
    memset(Track2, 0x00, sizeof(Track2));
	memset(Track3, 0x00, sizeof(Track3));
	ScrClrLine(2,7);
	if(flag==0)ScrDispStr(0,2,1,"�ſ�����","MagCard Test");
	if(flag==1)ScrDispStr(0,0,1,"���β���","Single Test");
	if(flag==2)ScrDispStr(0,0,1,"ѭ������","Cycle Test ");
    MagOpen();
	SuccessCnt=FailCnt=TotalCnt=0;
	while(1)
	{
		
		while(2)
		{
		
			if(flag==2)
			{
				if(CancelCheck())break;
				ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",++TotalCnt);
			}
			ScrDispStr(0,4,1,"��ˢ�ſ�<<<"," SWIPE CARD<<<");

			while(3)
			{
				if(!MagSwiped()) break;
				if(CancelCheck()){
					sprintf(gErrnoMsg,"User Cancel");
					if(flag==2)goto ENDTEST;
					else goto ENDERR;
				}
			}//while 3
			ucRet=MagRead((unsigned char*)Track1,(unsigned char*)Track2,(unsigned char*)Track3);
			if( ShowMagErr(6,ucRet,Track1,Track2,Track3)==0 )//���ſ���ȷ
			{
				BeepOK();	
				SuccessCnt++;			
			}
			else
			{
				FailCnt++;
				MagReset();
				BeepFail();
				if(flag<=1 && FailCnt>=3)goto ENDERR;//5�λ���,��3��OK����OK
			}
			if(flag<=1)
			{
				ScrDispStr(0,6,1,"�ɹ�����:%d/3   ","Succeed:%d/3   ",SuccessCnt);	
				MagReset();
				if(SuccessCnt==3)
				{
					MagClose();
					ScrDispStr(0,6,1,"�ſ�����ͨ��","Magcard Test OK!");			
					DelayMs(500);
					goto ENDOK;
				}
				continue;
			}
		}
ENDTEST:	
		ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",TotalCnt);
		ShowSuccessRate(TotalCnt,FailCnt);
		kbflush();
		if(getkey()==KEYCANCEL) break;
	}//while 1 
	
	
ENDOK:
	MagClose();
	return 0;
ENDERR:
	MagClose();
	return 1;
}


void MagCommonTest(void)
{
	unsigned char ucRet;
	char Track1[256],Track2[256],Track3[256];
	MagOpen();
	while(1)
	{
		ScrClrLine(2,7);
		ScrDispStr(0,2,1,"��ͨ������","Common Card Test");
        ScrDispStr(0,4,1,"��ˢ��<<<", "SWIPE CARD<<<");								
		
		while(2)
		{
			if(!MagSwiped()) break;
			if(CancelCheck()){
				MagClose(); return;
			}
		}// 2
		ucRet=MagRead((unsigned char*)Track1,(unsigned char*)Track2,(unsigned char*)Track3);
		if(!ucRet)
		{
			ScrClrLine(4,7);
			ScrDispStr(0,6,1,"��������","Read MAG Error");			
			BeepFail(); MagReset(); continue;
		}
		switch(ucRet)
		{
			case 0x01:	ScrDispStr(0,6,1,"��1������","Read Track1");	    	break;
			case 0x02:	ScrDispStr(0,6,1,"��2������","Read Track2");	    	break;
			case 0x04:	ScrDispStr(0,6,1,"��3������","Read Track3");	       	break;
			case 0x03:	ScrDispStr(0,6,1,"��1,2������","Read Track1/2");		break;
			case 0x05:	ScrDispStr(0,6,1,"��1,3������","Read Track1/3");		break;
			case 0x06:	ScrDispStr(0,6,1,"��2,3������","Read Track2/3");		break;
			case 0x07:	ScrDispStr(0,6,1,"��123������","Read Track1/2/3");  	break;
		}
		BeepOK(); WaitEsc(0,1);
		if(ucRet & 0x01){ //��ʾ1������
			ScrClrLine(2,7);
			ScrPrint(0,2,0,"%s",Track1);
			kbflush();
			getkey();
		}
		if(ucRet & 0x02){ //��ʾ2������
			ScrClrLine(2,7);
			ScrPrint(0,2,0,"%s",Track2);
			kbflush();
			getkey();
		}
		if(ucRet & 0x04){ //��ʾ3������
			ScrClrLine(2,7);
			ScrPrint(0,2,0,"%s",Track3);
			kbflush();
			getkey();
		}
		return;
	}// 1
}


void MagCommonTestCyc(void)
{
	int FailNum,TotalNum;
	char Track1[256],Track2[256],Track3[256];
	char TempTrack1[256],TempTrack2[256],TempTrack3[256];
	uchar ErrorFlag,ucRet,ExitFlag;
	uchar Str[2][10]={
		"OK",
		"Fail"
	};
	memset(Track1, 0x00, sizeof(Track1));
    memset(Track2, 0x00, sizeof(Track2));
	memset(Track3, 0x00, sizeof(Track3));
	memset(TempTrack1, 0x00, sizeof(TempTrack1));
    memset(TempTrack2, 0x00, sizeof(TempTrack2));
	memset(TempTrack3, 0x00, sizeof(TempTrack3));


	TotalNum=0; FailNum=0;
	MagOpen();
	while(1)
	{
		
		while(2)
		{

			ScrCls();
			ScrDispStr(0,0,1,"��ͨ����  %6d","CommonTest%6d",++TotalNum);
			ErrorFlag=0;
			ExitFlag=0;
			if(CancelCheck())break;
			ScrDispStr(0,2,1,"��ˢ��<<<","SWIPE CARD<<<");
			MagReset();
			while(3)
			{
				if(!MagSwiped()) break;
				if(CancelCheck())
				{
					TotalNum--; 
					ExitFlag=1;
					break;
				}
			}// 3
			
			if(ExitFlag==1)break;

			ucRet=MagRead((unsigned char*)Track1,(unsigned char*)Track2,(unsigned char*)Track3);

			switch(ucRet)
			{
				case 0x01:ScrDispStr(0,4,1,"����1�� 0x%02x","Only Track1! 0x%02x",ucRet);break;
				case 0x02:ScrDispStr(0,4,1,"����2�� 0x%02x","Only Track2! 0x%02x",ucRet);break;
				case 0x04:ScrDispStr(0,4,1,"����3�� 0x%02x","Only Track3! 0x%02x",ucRet);break;
				case 0x03:ScrDispStr(0,4,1,"����1,2�� 0x%02x","Only Track1/2! 0x%02x",ucRet);break;
				case 0x05:ScrDispStr(0,4,1,"����1,3�� 0x%02x","Only Track1/3! 0x%02x",ucRet);break;
				case 0x06:ScrDispStr(0,4,1,"����2,3�� 0x%02x","Only Track2/3! 0x%02x",ucRet);break;
				case 0x07:ScrDispStr(0,4,1,"����1,2,3�� 0x%02x","Only Track1/2/3! 0x%02x",ucRet);break;
				default:ScrDispStr(0,4,1,"�������� 0x%02x","Read MAG Error 0x%02x", ucRet);ErrorFlag=1;break;
			}

			if(ErrorFlag==1)
			{
				BeepFail(); 
				getkey();
				FailNum++;
				if(TotalNum==1)return;
				continue;
			}

			if(TotalNum==1){
				getkey();
				ScrClrLine(2,7);
				ScrPrint(0,2,0,"Track1:   %s",Track1);
				getkey();
				
				ScrClrLine(2,7);
				ScrPrint(0,2,0,"Track2:   %s",Track2);
				getkey();

				ScrClrLine(2,7);
				ScrPrint(0,2,0,"Track3:   %s",Track3);
				getkey();

				ScrClrLine(2,7);
				ScrDispStr(0,4,1,"[ȡ��]-�˳�\n[ȷ��]-����","[CANCLE]-Quit\n[ENTER]-GoOn");
				if( getkey()==KEYENTER ){
					memcpy(TempTrack1,Track1,sizeof(Track1));
					memcpy(TempTrack2,Track2,sizeof(Track2));
					memcpy(TempTrack3,Track3,sizeof(Track3));
					continue;
				}
				else{
					return ;
				}
			
			}
			else{
					int iRet1,iRet2,iRet3;
					if( strcmp(TempTrack1,Track1)==0 )iRet1=0;
					else iRet1=1;
					if( strcmp(TempTrack2,Track2)==0 )iRet2=0;
					else iRet2=1;
					if( strcmp(TempTrack3,Track3)==0 )iRet3=0;
					else iRet3=1;
					ScrPrint(0,6,0,"#1%s,#2%s,#3%s",Str[iRet1],Str[iRet2],Str[iRet3]);
					if( iRet1==0 && iRet2==0 && iRet3==0 )ErrorFlag=0;
					else ErrorFlag=1;			
			}

			if(ErrorFlag){
				BeepFail(); 
				getkey();
				FailNum++;				
			}
			else{
				BeepOK();
			}

		}// 2
		ShowSuccessRate(TotalNum,FailNum);	
		if(kbflush(),getkey()==KEYCANCEL) break;
	}//1 
	MagClose();
}



void MagTest1(void)
{
	MagTest(1);
}
void MagTest2(void)
{
	MagTest(2);
}

void MagTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-����",			"1-Single",			MagTest1,},
		{"2-ѭ��",			"2-Cycle",			MagTest2,},
		{"3-��ͨ���ɹ���",	"3-Common Cycle",	MagCommonTestCyc,}
	};

	if(TestModule[T_MAG]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"�����޴ſ�ģ��", "No Magcard Module");
		getkey();
		return;
	}
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"�ſ��������","MagCard Test",3);
	
}
