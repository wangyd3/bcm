#include "public.h"

// flag: 0-整机/主板  1-单次 2-循环 3-成功率
uchar  SDCardTest(uchar flag)
{
	long TotalNum,FailNum;
	int fd;
	FS_W_STR  sd_card;
	uchar ucFileName[30];

	kbflush();
	ScrClrLine(2,7);
	if(flag==0) ScrDispStr(0,2,1,"SDCARD测试","SDCARD Test");
	if(flag==1) ScrDispStr(0,0,1,"单次测试","Single Test");
	if(flag==2) ScrDispStr(0,0,1,"循环测试","Cycle Test ");

									
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
				ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
				if(CancelCheck()) break;
				//if(TotalNum==1000)break;
			}
			fd = FsOpen(&sd_card, FS_ATTR_C|FS_ATTR_R);
			if(fd >= 0)
			{
				ScrClrLine(2,7);
				ScrDispStr(0,4,1, "SD卡测试成功", "SDCard Disk Test OK");
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
				ScrDispStr(0,4,1,"SD卡测试失败%d", "SDCard Test Fail%d",fd);
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
		{"1-单次",	"1-Single",SDCardTest1},
		{"2-循环",	"2-Cycle" ,SDCardTest2},
	};

	if(TestModule[T_SDCARD]=='N')
	{		
		ScrClrLine(2,7);
		ScrDispStr(0,4,1, "无SDCARD设备", "No SDCARD Module");
		getkey();
		return ;
	}

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"SD卡单项测试","SDCard Test",3);

}
