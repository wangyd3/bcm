#include "public.h"


// flag: 0-���� 1-���β���
uchar TelTest(uchar flag)
{
	return 0;
}


//��ͨ�绰
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
		{"1-���β���",	"1-Single Test",TelePhone},
		{"2-��ͨ�绰",	"2-Telephone  ",TelTest1 },

	};

	if(TestModule[T_TEL]=='N')
	{		
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"�޵绰ģ��", "NO PHONE Module");
		getkey();
		return ;
	}
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"�绰�������","Phone Test",3);
}
