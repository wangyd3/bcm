#include "public.h"


//��������
//flag=0 ������1 ���Σ� 2 ѭ��    3 ����    4 �һ�
uchar SpeechTest(uchar flag)
{

#ifdef SPEECH_ON
	uchar ucRet;
	int TotalNum,FailNum,iRet;
	ScrClrLine(2,7);

	if(flag==0)ScrDispStr(0,2,1,"��������...","Sound Test...");
	if(flag==1)ScrDispStr(0,0,1,"���β���","Single Test");
	TotalNum=0;FailNum=0;
	while(1)
	{
		while(2)
		{
			
			ScrClrLine(4,7);
			if(flag==2 || flag==3)
			{
				if(CancelCheck())break;
				ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",++TotalNum);
			}
			if(flag==3)iRet = SoundPlay("other.wav",1);
			else   iRet = SoundPlay("Confirm.wav",1);
			if(flag==4)return iRet;
			if(!iRet)
			{
				if(flag==0 || flag==1)
				{
					ucRet=JudgeState("����","Speech");
					if(ucRet == KEYENTER)
						return 0;
					else
						return 1;
				}
				ScrDispStr(0,4,1,"�������Գɹ�","Speech Test OK");
				DelayMs(1000);
			}
			else
			{
				sprintf(gErrnoMsg,"SoundPlay=%d",iRet);
				ScrDispStr(0,4,1,"��������ʧ��%d","Speech Test Fail%d",iRet);
				DelayMs(1000);
				if(flag==0 || flag==1)
				{
					 return 1;
				}
			}
			
		}
		ShowSuccessRate(TotalNum,FailNum);
		if(kbflush(),getkey()==KEYCANCEL) 
		break;
	}
#endif
	return 1;
}

void SpeechTest1(void)
{
	SpeechTest(1);
}
void SpeechTest2(void)
{
	SpeechTest(2);
}
void SpeechTest3(void)
{
	SpeechTest(3);
}
void SpeechTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-����",	"1-Single",SpeechTest1},
		{"2-ѭ��",	"2-Cycle ",SpeechTest2},
		{"3-����",	"3-Other" ,SpeechTest3},

	};

	if(TestModule[T_SPEECH]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1, "����������ģ��", "No Speech Module");
		getkey();
		return ;
	}
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"�����������","Speech Test",3);

}

