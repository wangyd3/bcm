#include "public.h"


// flag: 0-整机 1-单次测试
uchar TelTest(uchar flag)
{
	return 0;
}


//普通电话
void TelePhone(void)
{

}


void TelTest1(void)
{
	TelTest(0);
}

void TelTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次测试",	"1-Single Test",TelePhone},
		{"2-普通电话",	"2-Telephone  ",TelTest1 },

	};

	if(TestModule[T_TEL]=='N')
	{		
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"无电话模块", "NO PHONE Module");
		getkey();
		return ;
	}
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"电话单项测试","Phone Test",3);
}
