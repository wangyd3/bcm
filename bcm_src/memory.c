#include "public.h"

#define BLOCKNUM  (1024*10)

void FlashTest(void)
{
	uchar fid,i;
	ulong  num, ii,jj,FreeSize,removenum=0;
	uint  allnum=0,filenum=0,failnum=0;
	int ret;
	uchar dat1[BLOCKNUM];
	uchar dat2[BLOCKNUM];
	ScrClrLine(0,7);
	ScrPrint(0,0,1,"Flash Test");
	remove("TestFlash.dat");
	fid=open("TestFlash.dat",O_CREATE|O_RDWR);
	FreeSize=freesize();
	num = 0;
	while(1)
	{	
		for(ii=0;ii<BLOCKNUM;ii++)
			dat1[ii]=rand()%0xff;
		
		if((num+3)*BLOCKNUM >=FreeSize)
		{
			num=0;
			seek(fid,0,SEEK_SET);
			removenum++;
			ScrPrint(0,4,0,"removenum=%d    ",removenum);
		}
		
		jj=write(fid,dat1,BLOCKNUM);
		if(jj != BLOCKNUM)
		{
			failnum++;
		}
		else
		{
			ScrPrint(0,0,1,"Flash Test %d",jj);
			DelayMs(10);
			ret = seek(fid,-BLOCKNUM,SEEK_CUR);
			if(ret != 0)
			{
				failnum++;
			}
			else
			{
				memset(dat2, 0, BLOCKNUM);
				ret = read(fid,dat2,BLOCKNUM);
				if(ret != BLOCKNUM)
				{
					failnum++;
				}
				else
				{
					
					i=rand()%0xff;
					ScrPrint(0,2,0,"%d,%d,%d,%d  ",dat1[i],dat1[i+1],dat1[i+2],dat1[i+3]);
					ScrPrint(0,3,0,"%d,%d,%d,%d  ",dat2[i],dat2[i+1],dat2[i+2],dat2[i+3]);
					if(memcmp(dat1,dat2,BLOCKNUM) != 0 )
					{
						failnum++;
					}
				}
			}
		}
		
		allnum++;
		num++;

		ScrPrint(0,5,0,"all=%d",allnum);
		ScrPrint(0,6,0,"fail=%d",failnum);
		ScrPrint(0,7,0,"rate=%d%%",100*(allnum-failnum)/allnum);
		if(CancelCheck()) break;
		DelayMs(10);
	}
	close(fid);
}

void FlashTestAll(void)
{
	FlashTest();
}


/************************************************************************/
/* �ڴ���ԣ���Ӧ�ó����������洢��Ԫ���ж�д���������裺
	1.���������浥Ԫԭ����
    2.д��0��Ȼ�����ֵ��0�Ƚ� 
	3.д��0xffffffff��Ȼ�����ֵ��0xffffffff�Ƚ� 
	4.�ٻָ���1������ĵ�Ԫԭ����                                                                */
/************************************************************************/
// flag: 0-�ֻ� 1-�һ� 2-���� 3-ѭ�� 4-�ɹ���
#if 0
uchar  RamTest(uchar flag)
{
	register unsigned long i,*Addr;
	unsigned long j,k,l;
	unsigned short oldRet,ret;
	long TotalNum,FailNum;

	kbflush();
	ScrClrLine(2,7);
	oldRet=1000;
	TimerSet(0,oldRet);

	if(flag==1) ScrDispStr(0,0,1,"���β���","Single Test");
	if(flag==2) ScrDispStr(0,0,1,"ѭ������","Cycle Test ");
				
	TotalNum=0; 
	FailNum=0;
	while(1)
	{
		while(2)
		{
			if(CancelCheck()) break;
			if(flag==2 && TotalNum==1000) break;
			if(flag==2)	ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",++TotalNum);
			j=0; k=0; l=0;
#ifdef SSERIES_POS
	    	Addr=(unsigned long *)0x28400000;//S80 0x28400000 - 0x28500000 ��Ӧ�ù�������1M��
#else 
	    	Addr=(unsigned long *)0x11000000;//SP30 0x10300000 - 0x103FFFFF Ӧ�ó����������1M��
#endif
			ScrClrLine(2,7);
			
			while(k<1024*1024L){
				i=*Addr;
				*Addr=0;
				if(*Addr!=0) 
					break;
				*Addr=0xffffffff;
				if(*Addr!=0xffffffff) 
					break;
				*Addr=i;
				j++;
				Addr++;
				if(j>2621){
					j=0;
					ScrDispStr(0,4,1,"SDRAM����...%d%%","SDRAM Test..%d%%",++l);
					DelayMs(5);
					if(flag==1)
					{
						ret=TimerCheck(0);
						if(!ret) 
							break;
						if(ret!=oldRet){
							DispTimer(7);
							oldRet=ret;
						}
					}
				}
				k=k+4;
			}
			if(k<1024*1024L)
			{
				ScrDispStr(0,6,1,"SDRAM���Գ���","SDRAM Test ERROR");
				BeepFail();  FailNum++;
				if(flag<=2)
				{
					WaitEsc(2,0);
					return 1;
				}
				else if(flag==3)
				{
					kbflush();
					if(getkey()==KEYCANCEL) break;
					else continue;
				}
				else
				{
					DelayMs(200); continue;
				}
			}
			if(flag<=2)
			{
				if(flag==2 || !flag) BeepOK();
				if(WaitEsc(0,1)) return 1;
				else return 0;
			}
			else
			{
				Beep();
				DelayMs(200);
			}
		}// 2
		if(!TotalNum) break;
		ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",TotalNum);
		ShowSuccessRate(TotalNum,FailNum);
		if(getkey()==KEYCANCEL) break;
		
	}// 1
   return 0;
}
#endif


// flag: 0-����/����  1-���� 2-ѭ�� 3-�ɹ���
uchar  SDCardTest(uchar flag)
{
	long TotalNum,FailNum;
	int fd;
	FS_W_STR  sd_card;
	uchar ucFileName[30];

	kbflush();
	ScrClrLine(2,7);
	if(flag==0) ScrDispStr(0,2,1,"SDCARD����","SDCARD Test");
	if(flag==1) ScrDispStr(0,0,1,"���β���","Single Test");
	if(flag==2) ScrDispStr(0,0,1,"ѭ������","Cycle Test ");

									
	TotalNum=0; FailNum=0;
	while(1)
	{
		sd_card.fmt = NAME_FMT_ASCII;
		sd_card.str = ucFileName;
		sd_card.size = sprintf(sd_card.str,"/sd_card/test");
		while(2)
		{
			
			if(flag==2)
			{
				ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",++TotalNum);
				if(CancelCheck()) break;
				//if(TotalNum==1000)break;
			}
			fd = FsOpen(&sd_card, FS_ATTR_C|FS_ATTR_R);
			if(fd >= 0)
			{
				ScrClrLine(2,7);
				ScrDispStr(0,4,1, "SD�����Գɹ�", "SDCard Disk Test OK");
				FsClose(fd);
				BeepOK();
				DelayMs(500);
				if(flag <=1)
					return 0;
				else
					continue;
			}
			else
			{
				sprintf(gErrnoMsg,"FsOpen=%d",fd);
				ScrClrLine(2,7);
				ScrDispStr(0,4,1,"SD������ʧ��%d", "SDCard Test Fail%d",fd);
				FsClose(fd);
				BeepFail();
				DelayMs(500);
				FailNum ++;
				if(flag <= 1)
					return 1;
				else
					continue;
			}
		}//while(2)
		if(!TotalNum) break;
		ShowSuccessRate(TotalNum,FailNum);
		if(kbflush(),getkey()==KEYCANCEL) break;
	}
    return 0;
}

void SDCardTest1(void)
{
	SDCardTest(1);
}
void SDCardTest2(void)
{
	SDCardTest(2);
}

void SDCardTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-����",	"1-Single",SDCardTest1},
		{"2-ѭ��",	"2-Cycle" ,SDCardTest2},
	};

	if(TestModule[T_SDCARD]=='N')
	{		
		ScrClrLine(2,7);
		ScrDispStr(0,4,1, "��SDCARD�豸", "No SDCARD Module");
		getkey();
		return ;
	}

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"SD���������","SDCard Test",3);

}
