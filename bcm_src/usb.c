#include "public.h"


void UDiskTest1(void)
{
	UDiskTest(1);
}
void UDiskTest2(void)
{
	UDiskTest(2);
}

void UDiskATest1(void)
{
	UDiskATest(1);
}
void UDiskATest2(void)
{
	UDiskATest(2);
}


void UDiskTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",	"1-Single",	UDiskTest1},
		{"2-循环",	"2-Cycle ",	UDiskTest2},
		{"3-A单次",	"3-ASingle",UDiskATest1},
		{"1-A循环",	"1-ACycle ",UDiskATest2},
	};

	if(TestModule[T_USBHOST]=='N')
	{		
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"无USB主机接口", "No USB Host Module");
		getkey();
		return ;
	}

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"U盘单项测试","U_Disk Test",3);
}


//USB扩展端口文件目录是udisk_a, sprintf(Udisk.str,"/udisk_a/test");
//12端口，U盘文件访问目录是udisk
//14端口，U盘文件访问目录是udisk_a
// flag: 0-整机/主板  1-单次 2-循环 3-成功率 
uchar  UDiskATest(uchar flag)
{
	uint TotalNum,FailNum;
	int fd;
	FS_W_STR  Udisk;
	uchar ucFileName[30];
	uchar ucRet;

	kbflush();
	ScrClrLine(2,7);
	
	if(flag==0) ScrDispStr(0,2,1,"U盘A测试","UDISKA Test");
	if(flag==1) ScrDispStr(0,0,1,"   单次测试","Single Test");
	if(flag==2) ScrDispStr(0,0,1,"   循环测试","Cycle Test ");
	PortClose(P_USB_HOST);
							
	TotalNum=0;
	FailNum=0;
	while(1)
	{
		ucRet = PortOpen(P_USB_HOST_A,"UDISK");
		if(ucRet!= 0)
		{
			sprintf(gErrnoMsg,"PortOpen(14)=0x%02x",ucRet);
			ScrPrint(0,4,1,"PortOpen(14) fail %d ",ucRet);
			goto ENDERR;
		}
		while(2)
		{
			if (flag==2)
			{
				ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
				if(CancelCheck()) break;
				if(TotalNum==10000) break;
			}
			/*
			iRet=FsUdiskIn();
			if(iRet)
			{
				ScrClrLine(2,7);
				ScrPrint(0,4,1,"No UDISKA,FsUdiskIn=%d",iRet);
				if(flag<=1)goto ENDERR;
			}
			*/
			Udisk.fmt = NAME_FMT_ASCII;
			Udisk.str = ucFileName;
			Udisk.size = sprintf(Udisk.str,"/udisk_a/test");
			fd = FsOpen(&Udisk, FS_ATTR_C|FS_ATTR_R);
			if(fd<0)
			{
				ScrClrLine(2,7);
				sprintf(gErrnoMsg,"FsOpen=%d",fd);
				ScrDispStr(0,4,1,"U盘A测试失败FsOpen=%d", "UDiskA Fail,FsOpen=%d",fd);
				FsClose(fd);
				BeepFail();
				FailNum ++;
				if(flag<=1)	goto ENDERR;
			}
			else
			{
				ScrClrLine(2,7);
				ScrDispStr(0,4,1, "U盘A测试成功%d", "U DiskA Test OK%d",fd);
				FsClose(fd);
				if(flag<=1)	goto ENDOK;
			}
		
		}//while(2)
		ShowSuccessRate(TotalNum,FailNum);
		kbflush();
		if(getkey()==KEYCANCEL)break;
	}

ENDERR:
	BeepFail();
	DelayMs(500);
	PortClose(P_USB_HOST_A);
    return 1;
ENDOK:
	BeepOK();
	DelayMs(500);
	PortClose(P_USB_HOST_A);
    return 0;
}




// flag: 0-整机/主板  1-单次 2-循环 3-成功率
uchar  UDiskTest(uchar flag)
{
	uint TotalNum,FailNum;
	int iRet,fd;
	FS_W_STR  Udisk;
	uchar ucFileName[30];
	uchar ucRet;

	kbflush();
	ScrClrLine(2,7);
	
	if(flag==0) ScrDispStr(0,2,1,"U盘测试","UDISK Test");
	if(flag==1) ScrDispStr(0,0,1,"   单次测试","Single Test");
	if(flag==2) ScrDispStr(0,0,1,"   循环测试","Cycle Test ");

	PortClose(P_USB_DEV);//防止中途退出而没有关闭Device
	PortClose(P_USB_HOST);//
	DelayMs(250);
							
	TotalNum=0;
	FailNum=0;
	while(1)
	{
		ucRet = PortOpen(12,"UDISK");
		if(ucRet!= 0)
		{
			sprintf(gErrnoMsg,"PortOpen(12)=0x%02x",ucRet);
			ScrPrint(0,4,1,"PortOpen(12) fail %d ",ucRet);
			goto ENDERR;
		}
		while(2)
		{
			if (flag==2)
			{
				ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
				if(CancelCheck()) break;
				if(TotalNum==10000) break;
			}
			iRet=FsUdiskIn();
			if(iRet)
			{
				ScrClrLine(2,7);
				sprintf(gErrnoMsg,"FsUdiskIn=%d",iRet);
				ScrPrint(0,4,1,"No UDISK,FsUdiskIn=%d",iRet);
				if(flag<=1)goto ENDERR;
			}
			
			Udisk.fmt = NAME_FMT_ASCII;
			Udisk.str = ucFileName;
			Udisk.size = sprintf(Udisk.str,"/udisk/test");
			fd = FsOpen(&Udisk, FS_ATTR_C|FS_ATTR_R);
			if(fd<0)
			{
				
				ScrClrLine(2,7);
				sprintf(gErrnoMsg,"FsOpen=%d",fd);
				ScrDispStr(0,4,1,"U盘测试失败FsOpen=%d", "UDisk Fail,FsOpen=%d",fd);
				FsClose(fd);
				FailNum ++;
				BeepFail();
				if(flag<=1)	goto ENDERR;
			}
			else
			{
				ScrClrLine(2,7);
				ScrDispStr(0,4,1, "U盘测试成功", "U Disk Test OK");
				FsClose(fd);
				if(flag<=1)	goto ENDOK;
			}
		
		}//while(2)
		ShowSuccessRate(TotalNum,FailNum);
		kbflush();
		if(getkey()==KEYCANCEL)break;
	}

ENDERR:
	BeepFail();
	DelayMs(500);
	PortClose(P_USB_HOST);
    return 1;
ENDOK:
	BeepOK();
	DelayMs(500);
	PortClose(P_USB_HOST);
    return 0;
}


char open_picc(void)
{
	uchar SerialNo[11],CardType[10],ucRet;
	
	if(TestModule[T_RF]!='Y')
		return 0;

	ScrClrLine(2,7);
	ucRet=PiccOpen();
    if(ucRet) 
    {
		sprintf(gErrnoMsg,"PiccOpen=0x%02x",ucRet);
		ScrDispStr(0,4,1, "射频打开失败", "PiccOpenErr",ucRet);
        getkey();
        return 1;
    }
	ucRet=PiccDetect(0, CardType, SerialNo, NULL, NULL);
	return 0;
}

void UsbDevTest0(void)
{
	UsbDevTest(0);
}

void UsbDevTest1(void)
{
	UsbDevTest(1);
}

void UsbDevTest2(void)
{
	UsbDevTest(2);
}

void UsbDevTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",	"1-Single",	UsbDevTest1},
		{"2-循环",	"2-Cycle ",	UsbDevTest2},
		{"3-抽检",	"3-Sampling",UsbDevTest0},

	};

	if(TestModule[T_USBDEV]=='N')
	{		
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"无USB设备接口", "No USBDEV Module");
		getkey();
		return ;
	}

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"U设备单项测试","UDevice Test",3);
}

uchar UsbDevTest(uchar flag)//S78 USB Device 测试
{
	uint TotalNum,FailNum;
	uchar ucRet;
	uint  recCnt,totle;
	uchar wrBuf[65535],rdBuf[65535];
	uint i;

	if(flag==0)
	{
		if(open_picc())goto ENDERR;
	}

	PortClose(P_USB_DEV); //
	PortClose(P_USB_HOST);//防止中途退出而没有关闭host

	ScrClrLine(2,7);
	memset(wrBuf,0,sizeof(wrBuf));
	memset(rdBuf,0,sizeof(rdBuf));
	if(flag==0) ScrDispStr(0,2,1,"USB设备测试","USBDEV TEST");
	if(flag==1) ScrDispStr(0,0,1,"单次测试","Single Test");
	if(flag==2) ScrDispStr(0,0,1,"循环测试","Cycle Test ");
	DelayMs(250);

	if(flag==0 && TermInfo[0]==S78)
	{
		PortOpen(P_USB_DEV,"9600,8,n,1");
		ScrClrLine(2,7);
		ScrDispStr(0, 2, 0x01, "请拔掉U盘，接好USB线，按键继续", "PLS Take Udisk,Connect Device,Any Key Go On");
		kbflush();
		getkey();
	}


	TotalNum=0; 
	FailNum=0;
	while(1)
	{
		while(2)
		{
			ScrClrLine(4,7);
			if (flag==2)
			{
				ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
				if(CancelCheck()) break;
				if(TotalNum==10000) break;
			}
			ScrDispStr(0,4,1,"USB设备测试中...","USB Device testing...");
			ucRet=PortOpen(P_USB_DEV,"9600,8,n,1");
			if(ucRet!=0)
			{
				ScrClrLine(2,7);
				sprintf(gErrnoMsg,"PortOpen(11)=0x%02x",ucRet);
				ScrDispStr(0,4,1,"打开设备错误%d", "DEVICE OPEN ERROR%d",ucRet);
				BeepFail();
				DelayMs(500);
				FailNum++;
				if(flag==0 || flag==1)
					goto ENDERR;
				else 
					continue;
			}

			//发送数据
			for(i=0;i<8192;i++) wrBuf[i]=i+1;
			ucRet=PortSends(P_USB_DEV,wrBuf,8192);	
			if(ucRet!=0)
			{				
				ScrClrLine(2,7);
				sprintf(gErrnoMsg,"PortSends(11)=0x%02x",ucRet);
				ScrDispStr(0,4,1,"发送数据错误%d", "DEVICE SEND ERROR%d",ucRet);
				BeepFail();
				DelayMs(500);
				FailNum++;
				if(flag==0 || flag==1)
					goto ENDERR;
				else
					continue;
			}
			
			//接受数据					
			memset(rdBuf,0,sizeof(rdBuf));
			TimerSet(1,100);
			totle=0;
			recCnt=0;
			//PortReset(11);//刚刚加上去试了也一样
			while(TimerCheck(1))
			{
				if(totle>=8192 || CancelCheck()) break;
				recCnt=PortRecvs(P_USB_DEV,&rdBuf[totle],8192,0);
				if(recCnt>0) totle+=recCnt;
			}			
			if(totle!=8192 || memcmp(rdBuf,wrBuf,8192))
			{
				ScrClrLine(2,7);
				sprintf(gErrnoMsg,"DATA ERROR%d",totle);
				ScrDispStr(0,4,1,"接收数据错误%d", "DEVICE RECEIVE ERROR%d",totle);
				BeepFail();
				DelayMs(500);
				FailNum++;
				if(flag==0 || flag==1)
					goto ENDERR;
				else 
					continue;
			}
			
			BeepOK();
			ScrClrLine(2,7);
			ScrDispStr(0,4,1,"USB设备测试成功", "USB Device Test OK");
			DelayMs(400);
			if(flag==0 || flag==1)
				goto ENDOK;
		}//while(2)
		ShowSuccessRate(TotalNum,FailNum);
		kbflush();	
		if(getkey()==KEYCANCEL) break;
	}//while(1)

ENDOK:
	PiccClose();
	PortClose(P_USB_DEV);
	return 0;
ENDERR:
	PiccClose();
	PortClose(P_USB_DEV);
	return 1;

}

typedef struct{
	char portname[10];
	char portno;
}PORT_PARA;

PORT_PARA ports[]={
	{"RS232A",P_RS232A},
	{"RS232B",P_RS232B},
	{"PINPAD",P_PINPAD},
	{"USBH",P_USB_HOST},
	{"USBD",P_USB_DEV},
	{"BASE_A",P_BASE_A},
	{"BASE_B",P_BASE_B},
	{"BASE_HOST",P_BASE_HOST},
	{"BASE_DEV",P_BASE_DEV},
};

#define LEN 256
void send_recv(int portno,char *attr)
{
	int ret;
	uchar chs[LEN+1];
	while(1)
	{
		if(CancelCheck())break;
		ret=PortOpen(portno,attr);
		//if(ret)
		//{
		//	DelayMs(100); //要配合好，需要D180打开USB口成功后延时至少100ms(就1s吧)才发数据
		//	continue;			
		//}
		PortReset(portno);
		while(2)
		{   
			if(CancelCheck())return;
			ret = PortRecvs(portno,chs,LEN,0);
			if(ret<0)
			{
				PortClose(portno);//增加这个的话不需要从新插拔
				break;
			}
			PortSends(portno,chs,ret);
		}
	}
}

void ComServer(void)
{
	uchar ret,i,index,portno,ch;
	char *attr;
	
	ScrCls();
	ScrDispStr(0,0,1,"选择端口号:","Select PortNo:");
	for(i=0;i < sizeof(ports)/sizeof(PORT_PARA); i++)
	{
		ScrPrint(64*(i%2),i/2+2,0,"%d-%s",i+1,ports[i].portname);
	}
	ret = getkey();
	if(ret>=KEY1 && ret<= sizeof(ports)/sizeof(PORT_PARA) + KEY0)
	{
		index = ret - KEY1;
		portno = ports[index].portno;
	}
	else return;
	
	ScrCls();
	ScrPrint(0,0,1,"   COM SERVER");
	ScrPrint(0,2,1,"%s = %d",ports[index].portname,ports[index].portno);

	if(portno==P_BASE_HOST || portno==P_USB_HOST)
		attr = "PAXDEV";
	else attr = "115200,8,n,1";
	send_recv(portno,attr);
}




void ComClient(int flag)
{
	int i,index,ret;
	int timeOut = 2000;
	int n = 0;
	int success = 0;
	int totle,recCnt,sendLen;

	uchar sendBuff[8192];
	uchar recvBuff[8192];
	char *attr;
	char portno = 0;
	char tmp[20];

	ScrCls();
	ScrDispStr(0,0,1,"选择端口号:","Select PortNo:");
	for(i=0;i < sizeof(ports)/sizeof(PORT_PARA); i++)
	{
		ScrPrint(64*(i%2),i/2+2,0,"%d-%s",i+1,ports[i].portname);
	}
	ret = getkey();
	if(ret>=KEY1 && ret<= sizeof(ports)/sizeof(PORT_PARA) + KEY0)
	{
		index = ret - KEY1;
		portno = ports[index].portno;
	}
	else return;
	
	ScrCls();
	ScrPrint(0,0,1,"%s = %d",ports[index].portname,ports[index].portno);

	ScrClrLine(2,7);
	ScrDispStr(0,2,1,"发送数据长度", "Send Length");
	strcpy(tmp, "8192");
	ScrGotoxy(0,6);
    if(ret=GetString(tmp, 0xF2, 0, 20))
    {
    	return;
    }
    sendLen = atoi(tmp+1);

	for(i=0;i<sizeof(sendBuff);i++)
		sendBuff[i] = i;
    		
	if(portno==P_BASE_HOST || portno==P_USB_HOST)
		attr = "PAXDEV";
	else attr = "115200,8,n,1";


	if(ret = PortOpen(portno, attr))
	{
		ScrClrLine(2,7);
		ScrPrint(0,4,1,"PortOpen(%d) = %d",portno,ret);
		getkey();
		PortClose(portno);
		return; 
	}
			
	
	kbflush();
	ScrClrLine(2,7);

	while(1)
	{
		if(CancelCheck()) break;
		if(flag)
		{	
			ScrClrLine(2,7);
			ScrPrint(0,2,0,"getkey begin");
			getkey();
		}
		ScrPrint(0,3,0,"Len = %d",sendLen);
		if(!flag)
		{
			ScrPrint(0,4,0,"Total = %d", n++);
			ScrPrint(0,5,0,"Success = %d", success);
		}
		PortReset(portno);
    	if(ret = PortSends(portno, sendBuff, sendLen))
		{
    		ScrPrint(0,3,1,"sendLen=%d ret=%d", sendLen, ret);
			ScrPrint(0,4,1,"TEST FAIL ");
			DelayMs(1000);
			BeepFail();
			continue;
    	}		
		totle=0;
		recCnt=0;
		TimerSet(1,100);
		memset(recvBuff,0,sizeof(recvBuff));
		while(TimerCheck(1))
		{
			if(totle>=sendLen) break;
			recCnt = PortRecvs(portno, &recvBuff[totle], sendLen, 0);
			if(recCnt>0) totle+=recCnt;				
		}	

		ScrPrint(0,6,0,"recv=%d time=%d",totle,100-TimerCheck(1));
		if(memcmp(sendBuff, recvBuff, sendLen) == 0)
		{
    		success++;
			ScrPrint(0,6,1,"TEST OK  ");
			BeepOK();
			if(flag)DelayMs(1000);
		}
		else
		{
			ScrPrint(0,4,1,"TEST FAIL ");
			BeepFail();
			DelayMs(1000);
		    continue;
		}
	}
		
	PortClose(portno);
}

void ComClient0(void)
{
	ComClient(0);	
}

void ComClient1(void)
{
	ComClient(1);	
}

void ComTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-通讯服务器",	"1-PORT SERVER",ComServer},
		{"2-通讯口测试",	"2-PORT CLIENT",ComClient0},
		{"3-通讯口测试",	"3-PORT CLIENT",ComClient1},

	};

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"通讯口单项测试","PORT TEST",3);
}
