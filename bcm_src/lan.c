#include "public.h"
#define TXDLEN			512


void LanServer(void);
void LanServer2(void);
uchar LanPing(uchar flag);
void LanTest1(void)
{
	LanTest(1);
}
void LanTest2(void)
{
	LanTest(2);
}

void LanTest3(void)
{
	LanTest(3);
}


void LanTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",	"1-Single",	LanTest1},
		{"2-循环",	"2-Cycle ",	LanTest2},
		{"3-服务器","3-Server",	LanTest3},
		{"1-PING",	"1-PING",	LanPing},
	};
	
	if(TestModule[T_LAN] == 'N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"本机无LAN模块", "No LAN Module");
		getkey();
		return;
	}
	
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"以太网测试","LAN Test",3);

}


// flag:  0-整机 1-单次 2-循环
uchar LanTest(uchar flag)
{

	uchar TxBuf[1024], RxBuf[1024], temp[30];
	ushort usPortID;
	char *strEnglish,*strChinese;
	uint TotalNum, FailNum,i;
	uchar ucIp, ucRet,AutoSetIP=0;
	int error;
	int TxLen, RxLen,iRet,fid,socket;
	char host_mask[16], gw_ip[16];
	struct net_sockaddr addr;
		
	memset(host_mask, 0x00, sizeof(host_mask));
	memset(gw_ip, 0x00, sizeof(gw_ip));
	
	TotalNum=0;
	FailNum=0;
	RouteSetDefault(ROUTE_LAN);//配置系统缺省路由lan
	
    ScrClrLine(2,7);
	if(flag==0) ScrDispStr(0,2,1,"LAN模块测试", "LAN Test");
	if(flag==1) ScrDispStr(0,0,1,"单次测试","Single Test");
	if(flag==2) ScrDispStr(0,0,1,"循环测试","Cycle Test");
	

	if(flag == 0) //整机测试
	{
		AutoSetIP = 1;
	}
	else
	{
		ScrDispStr(0,2,1, "[1]-自动分配IP", "[1]-Auto SetIP");
		ScrDispStr(0,4,1, "[2]-手动分配IP", "[2]-Manual SetIP");
		kbflush();
		ucRet = getkey();
		ScrClrLine(2,7);
		if (ucRet == KEY1) AutoSetIP = 1;
		else if(ucRet==KEY2) AutoSetIP = 0;
		else return 1;
	}

	if (AutoSetIP == 0)
	{
		uchar *pstr;
		memset(temp, 0x00, sizeof(temp));
		strEnglish="Pls Set Local IP";
		strChinese="请设置本地IP";
		ScrDispStr(0,2,1,strChinese, strEnglish);
		ucIp=(rand()%250)+1;
		if(ucIp==231)  ucIp=230;          //如果取得的随机本地IP与工厂服务器IP一样，则本地IP取为230
		pstr = LANPara[0];
		i=0;
		while(*pstr!=0)
		{
			if(*pstr++=='.')
			{
				i++;
			}
			if(i == 3) break;
		}
		sprintf(pstr, "%d", ucIp);
		ScrPrint(0,4,1,"%s",LANPara[0]);
		ScrGotoxy(0,6);
		memcpy(temp, LANPara[0], 20);
		ucRet = GetString(temp, 0xF7, 7, 15);
		if(ucRet!=0xff && temp[0]!=0)
		{
			memset(LANPara[0], 0x00, 20);
			memcpy(LANPara[0], temp+1, temp[0]);
			fid = open(F_LANPARA,O_RDWR);
			if(fid < 0)
			{
				ShowFileCtrlErr(__LINE__);//获取文件操作出错错误码
			}
			else
			{
				iRet = seek(fid,0*20,SEEK_SET);
				if(iRet < 0)
				{
					ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
				}
				iRet = write(fid,LANPara[0],20);
				if(iRet != 20)
				{
					ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
				}
			}
			close(fid);
		}
	}

	{//设置服务器IP
        if(flag==0)//PORT接口板测试的时候不在进入设置服务器界面，直接使用默认的
        {
            memset(temp, 0x00, sizeof(temp));
            memcpy(temp, LANPara[1], 20);
        }
		else
        {
            ScrClrLine(2, 7);
            memset(temp, 0x00, sizeof(temp));
            strEnglish="LAN Remote IP";
            strChinese="设置LAN服务器IP";
            ScrDispStr(0,2,1,strChinese, strEnglish);
            ScrPrint(0,4,1,"%s",LANPara[1]);
            ScrGotoxy(0,6);
            memcpy(temp, LANPara[1], 20);
			ucRet = GetString(temp, 0xF5, 7, 15);
            if(ucRet!=0xff && temp[0]!=0)
            {
                memset(LANPara[1], 0x00, 20);
                memcpy(LANPara[1], temp+1, temp[0]);
                fid = open(F_LANPARA,O_RDWR);
                if(fid < 0)
                {
                    ShowFileCtrlErr(__LINE__); //获取文件操作出错错误码
                }
                else
                {
                    iRet = seek(fid,1*20,SEEK_SET);
                    if(iRet < 0)
                    {
                        ShowFileCtrlErr(__LINE__); //获取文件操作出错错误码
            
                    }
                    iRet = write(fid,LANPara[1],20);
                    if(iRet != 20)
                    {
                        ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
                    }
                 }
                 close(fid);
             }
        }
        usPortID=60180;//60180
	}
	
	while(1)
	{
		while(2)
		{
				
			
			if(flag>=2)
			{
				if(CancelCheck())break;	
				if(flag==2)
					ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
				else
					ScrDispStr(0,0,0x81,"网络服务器%6d","LAN SERVER%6d",++TotalNum);
				//if(TotalNum==10000)break;
			}
			while(3)
			{
				error = 0;	
				ScrClrLine(2,7);
				if(AutoSetIP==1)
				{	
					strEnglish="Start DHCP...";
					strChinese="启动DHCP...";
					ScrDispStr(0,2,1,strChinese, strEnglish);
					DhcpStart();
					TimerSet(0,50);
					do
					{
						iRet = DhcpCheck();
						DelayMs(100);
					}while(iRet < 0 && TimerCheck(0) > 0);
					if (iRet <0)
					{
						error = 1;
						sprintf(gErrnoMsg,"DhcpCheck=%d",iRet);
						ScrDispStr(0,4,1,"启动DHCP失败:%d","Start DHCP Err:%d", iRet);
						break;
					}
				}
				else
				{
					uchar gw_ip[20],i;
					memcpy(gw_ip,LANPara[0],20);
					for(i=19;i>4;i--){
						if(gw_ip[i]=='.'){
							gw_ip[i+1]='1';
							gw_ip[i+2]=0;
							break;
						}
					}

					iRet = EthSet(LANPara[0], "255.255.255.0", gw_ip, "192.168.0.111");
					if(iRet<0)
					{
						error = 1;
						sprintf(gErrnoMsg,"EthSet=%d",iRet);
						ScrDispStr(0,4,1,"设置网络参数失败:%d", "Set NetPara Err:%d",iRet);
						break;

					}
				}
				
				//查询当前IP
				{
					char host_ip_get[30]; 
					char host_mask_get[30];
					char gw_ip_get[30];
					char dns_ip_get[30];
					long state;

					ScrClrLine(2,7);	
					strEnglish="Get Current IP...";
					strChinese="获取当前IP...";
					ScrDispStr(0,2,1,strChinese, strEnglish);
					iRet = EthGet(host_ip_get,host_mask_get,gw_ip_get,dns_ip_get,&state);
					if(iRet<0)
					{
						error = 1;
						sprintf(gErrnoMsg,"EthGet=%d",iRet);
						ScrDispStr(0,4,1,"查询IP地址失败:%d", "Get IP Err:%d",  iRet);
						break;
					}
					if(flag==1)//单项单次测试时显示各种信息
					{
						ScrClrLine(2,7);
						ScrPrint(0,3,0,"IP:%s",host_ip_get);
						ScrPrint(0,4,0,"MASK:%s",host_mask_get);
						ScrPrint(0,5,0,"GATE:%s",gw_ip_get);
						ScrPrint(0,6,0,"DNS:%s",dns_ip_get);
						ScrPrint(0,7,0,"Port:%d",usPortID);	
						getkey();	
					}
				}

				ScrClrLine(2,7);
				strEnglish="Connect Server...";
				strChinese="连接服务器...";
				ScrDispStr(0,2,1,strChinese, strEnglish);
				ScrPrint(0,4,1, LANPara[1]);
				iRet = SockAddrSet(&addr, LANPara[1], usPortID);
				if(iRet<0)
				{
					error = 1;
					sprintf(gErrnoMsg,"SockAddrSet=%d",iRet);
					ScrDispStr(0,4,1, "设置套接字地址失败:%d","Set SocketAddr Err:%d", iRet);
					break;
				}

				socket = NetSocket(NET_AF_INET,NET_SOCK_STREAM,0);
				if (socket<0)
				{
					error = 1;
					sprintf(gErrnoMsg,"NetSocket=%d",socket);
					ScrDispStr(0,4,1, "创建网络套接字失败:%d", "Create Socket Err:%d", socket);
					break;

				}

				iRet = NetConnect(socket, &addr, sizeof(addr));
				if (iRet <0)
				{
					error = 1;
					sprintf(gErrnoMsg,"NetConnect=%d",iRet);
					ScrDispStr(0,4,1,"连接服务器失败:%d",  "Connect Server Err:%d",iRet);
					NetCloseSocket(socket);
					break;
				}

				ScrClrLine(3,7);
				strEnglish="Sending Data...";
				strChinese="发送数据....";
				ScrDispStr(0,2,1,strChinese, strEnglish);
				memset(TxBuf, 0x00, sizeof(TxBuf));
				for (i=0; i<TXDLEN; i++)
				{
					TxBuf[i] = rand()%0xff;
				}
				TxLen = 0;
				TxLen = NetSend(socket, TxBuf, TXDLEN, 0);
				if((TxLen < 0) || (TxLen != TXDLEN))
				{
					error = 1;
					sprintf(gErrnoMsg,"NetSend=%d",TxLen);
					ScrDispStr(0,4,1, "发送数据出错:%d", "Sending Data Err:%d", TxLen);
					NetCloseSocket(socket);
					break;
				}

				ScrClrLine(2, 7);
				strEnglish="Receiving Data...";
				strChinese="接收数据...";
				ScrDispStr(0,2,1,strChinese, strEnglish);
				TimerSet(0, 100);
				memset(RxBuf, 0x00, sizeof(RxBuf));
				RxLen=0;
				while((RxLen <TxLen) && TimerCheck(0))
				{
				
					iRet = NetRecv(socket,&RxBuf[RxLen],sizeof(RxBuf)-RxLen,0);
                    if (iRet > 0)
                    {
						  RxLen += iRet;
                    }
					if(RxLen)
					{
						break;
					}
				}
				if( RxLen!=TXDLEN || memcmp(TxBuf, RxBuf, TXDLEN) )
				{
					error = 1;
					sprintf(gErrnoMsg,"NetRecv=%d",RxLen);
					ScrDispStr(0,4,1,"接收数据出错:%d", "Receiving Data Err:%d", RxLen);
					break;
				}
				break;
			}//while(3)
			NetCloseSocket(socket);
			DhcpStop();
			if(!error)
			{
				BeepOK();
				ScrClrLine(2, 7);
				strEnglish = "LAN Test OK!";
				strChinese = "LAN通讯成功!";
				ScrDispStr(0,4,1,strChinese, strEnglish);
				if(flag==1 || flag==0)
				{
					DelayMs(500);
					goto ENDOK;
				}
								
				while(flag==3)
				{
					long state;
					if(CancelCheck())return 0;
					if(EthGet(NULL, NULL, NULL, NULL, &state)==NET_OK && state&0x1)
						DelayMs(1000);					
					else 
						break;
				}
				continue;
			}
			else
			{
				FailNum ++;
				BeepFail();
				if(flag==1 || flag==0)
				{
					DelayMs(500);
					goto ENDERR;
				}
				else
				{
					continue;
				}
			}
		}//while(2)
		ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",TotalNum);
		ShowSuccessRate(TotalNum,FailNum);
		if((kbflush(),getkey())==KEYCANCEL)break;
	}//while(1)
ENDOK:
	return 0;
ENDERR:
	return 1;
}

uchar LanPing(uchar flag)
{
	long state;
	int iRet = -1;
	ScrPrint(0,2,1,"Ping...");
	RouteSetDefault(ROUTE_LAN);//配置系统缺省路由lan
	iRet = EthGet(NULL, NULL, NULL, NULL, &state);
	ScrPrint(0,4,1,"%d,state:%d,%d",iRet,state,state&0x01);
	if(iRet==NET_OK && state&0x1)
	{
		iRet = NetPing("192.168.0.2",5000,512);
		ScrPrint(0,6,1,"NetPing = %d",iRet);
	}
	getkey();
	return iRet>0?0:1;
}


void LanServer(void)
{
	int iRet,i,index;
	char key;
	char ip[][20] = {
		"192.168.0.10",
		"192.168.0.11",
		"192.168.0.12",
		"192.168.0.13",
		"192.168.0.14",
		"192.168.0.15",	
	};

	ScrCls();
	ScrDispStr(0,0,1," LAN服务器"," LAN SERVER");
	for(i=0;i<(sizeof(ip)/sizeof(ip[0]));i++)
		ScrPrint(0,i+2,0,"%d-%s",i+1,ip[i]);
	key = getkey();
	if(key>=KEY1 && key<=KEY6)
		index = key-KEY1;
	else if(key==KEYCANCEL || key==KEYENTER)
		return;
	else 
		index = 0;
	
	ScrClrLine(2,7);
	RouteSetDefault(ROUTE_LAN);//配置系统缺省路由lan
	iRet = EthSet(ip[index], "255.255.255.0", "192.168.0.1", "192.168.0.111");
	if(iRet<0)
	{
		ScrDispStr(0,4,1,"设置网络参数失败:%d", "Set NetPara Err:%d",iRet);
		getkey();
	}

	while(1)
	{
		char host_ip_get[30]; 
		char host_mask_get[30];
		char gw_ip_get[30];
		char dns_ip_get[30];
		long state;
		if(CancelCheck())break;
		ScrDispStr(0,2,1,"获取网络状态","Get Net State");
		iRet = EthGet(host_ip_get,host_mask_get,gw_ip_get,dns_ip_get,&state);
		if(iRet<0)
		{
			ScrDispStr(0,2,1,"EthGet = %d", "EthGet = %d",  iRet);
			DelayMs(2000);
		}
		else
		{
			ScrClrLine(2,7);
			ScrPrint(0,3,0,"state:%d,%d",state,state&0x01);
			ScrPrint(0,4,0,"IP:%s",host_ip_get);
			ScrPrint(0,5,0,"MASK:%s",host_mask_get);
			ScrPrint(0,6,0,"GATE:%s",gw_ip_get);
			ScrPrint(0,7,0,"DNS:%s",dns_ip_get);
			DelayMs(2000);	
		}
	}
}



void LanServer2(void)
{
	int iRet;
	char host_ip_get[30]; 
	char host_mask_get[30];
	char gw_ip_get[30];
	char dns_ip_get[30];
	long state;

	ScrCls();
	ScrDispStr(0,0,1," LAN服务器"," LAN SERVER");
	RouteSetDefault(ROUTE_LAN);//配置系统缺省路由lan
	DhcpStart();
	while(1)
	{
		if(CancelCheck())break;
		/*
		ScrClrLine(2,5);
		ScrDispStr(0,2,1,"Start DHCP...", "启动DHCP...");
		DhcpStart();
		do
		{
			iRet = DhcpCheck();
			ScrDispStr(0,4,1,"DhcpCheck=%d   ","DhcpCheck=%d   ", iRet);
			DelayMs(1000);
		}while(iRet);	
		*/

		ScrClrLine(2,7);
		ScrDispStr(0,2,1,"获取网络状态","Get Net State");
		iRet = EthGet(host_ip_get,host_mask_get,gw_ip_get,dns_ip_get,&state);
		if(iRet<0)
		{
			ScrDispStr(0,2,1,"EthGet = %d", "EthGet = %d",  iRet);
			DelayMs(2000);
			break;
		}

		if(state&0x01)
		{
			ScrClrLine(2,7);
			ScrPrint(0,3,0,"state:%d,%d",state,state&0x01);
			ScrPrint(0,4,0,"IP:%s",host_ip_get);
			ScrPrint(0,5,0,"MASK:%s",host_mask_get);
			if(strcmp(host_ip_get,"0.0.0.0"))
				ScrDispStr(0,6,1,"网络正常","LAN IS OK");
			else
				ScrDispStr(0,6,1,"网络不正常","LAN IS FAIL");
			//ScrPrint(0,6,0,"GATE:%s",gw_ip_get);
			//ScrPrint(0,7,0,"DNS:%s",dns_ip_get);
			DelayMs(2000);
		}
		else
		{
			ScrDispStr(0,6,1,"网络不正常","LAN IS FAIL");
			DelayMs(1000);
		}
	}


}

uchar Lan2Test(uchar flag)
{
	ScrCls();
	ScrDispStr(0,0,1,"服务器测试结果","server result");
	ScrDispStr(0,2,1,"[确认]-成功","[ENTER]-OK");
	ScrDispStr(0,4,1,"[其他]-失败","[OTHER]-FAIL");
	if(getkey()==KEYENTER)
		return 0;
	else
		return 1;
}


void LanHubTest(void)
{
    uchar ucRet,ucKey;
	uchar OKNum,TestNum,AllNum,Times;
    int i,j,k,iRet,step=0;
	uint TTT1;

	TEST_ITEM TestItem[]={
		{'Y',	0,	"LAN",		LanTest		},
		{'Y',	0,	"COMA",		ComATest	},
		{'Y',	0,	"USBHOST",	UDiskTest	},
		{'Y',	0,	"USBDEV",	UsbDevTest	},
		{'Y',	0,	"LAN2",		Lan2Test	},
		{'N',	0,	"VER",		VerTest     },//TestModule[T_VER]='N',开始必须是N
	};

	TTT1=GetTimerCount();
	AllNum = sizeof(TestItem)/sizeof(TEST_ITEM);//AllNum = sizeof(TestItem)/sizeof(TestItem[0]);
	OKNum=TestNum=0;
    

    DeleteLog(F_BASIC_INFO);
	RECORD(0,".FTEST=%s",SOFT_VERSION); //记录FTest版本
    RECORD(1, "INTEGRATION TEST BEGIN");
    RECORD(1, "TestStart=%s",DispTimer(7));
    RECORD(1, "\n");
    RECORD(1, "..TestFinished=0");
    //step=GetStep();
    RECORD(1, "..TestStep=%d",step);

	for(i=0;i<AllNum;i++)
	{
		if(TestItem[i].flag=='Y')
		{
			ScrCls();
			ScrDispStr(0,0,0x81,"    整机测试    ", "Integration Test");
			Times=0;
			TestNum++;
			while(1)
			{
				Times++;
				memset(gErrnoMsg,0,sizeof(gErrnoMsg));
				ucRet=TestItem[i].TestFun(0);
				RECORD(1,"..%s_TEST_TIME=%d",TestItem[i].itemStr,GetTimerCount()-TTT1);//记录每项目测试的时间
				if(ucRet==0)
				{
					RECORD(1,"%s=1\r\n",TestItem[i].itemStr);
					TestItem[i].result=1;
					OKNum++;
					break;
				}
				else
				{
					RECORD(1,"%s=0\r\n",TestItem[i].itemStr);
					RECORD(1,"..%s.err=%s\r\n",TestItem[i].itemStr,gErrnoMsg);
					if(Times<2)
					{  
						ucRet=Select();
						if(ucRet==KEY1 || ucRet==KEYUP) continue;
						if(ucRet==KEYCANCEL) goto RESULT;
					}
				}
				break;
			}//1			
		}//if(TestItem[i].flag='Y')
	}//for

RESULT:
    RECORD(1, "INT.TOTAL.cnt=%d", TestNum);
    RECORD(1, "INT.PASS.cnt=%d", OKNum);
    RECORD(1, "INT.FAIL.cnt=%d", TestNum-OKNum);
	for(i=0,j=0;i<AllNum;i++)
	{
		if(TestItem[i].flag=='Y')
		{
			if(TestItem[i].result==0)
			{
				j++;
				RECORD(1, "#FAIL%d=%s",j,TestItem[i].itemStr);
			}
		}
	}
    RECORD(1, "\n");
    RECORD(1, "INTEGRATION TEST END\n");
    RECORD(1, "\n");

#if 0
    ScrClrLine(2,7);
    TimerSet(0, 200);
	ucKey=KEYCANCEL;
	ScrDispStr(0,2,1, "[确认]-上传结果", "[ENTER]-Upload");
	ScrDispStr(0,4,1, "[其他]-跳过上传", "[OTHER]-Skip  ");
    while(i=TimerCheck(0))
    {
        ScrPrint(60,6,1," %3d ",i);
        if(!kbhit())
        {
            ucKey=getkey();
            break;
        }
    }

    if(ucKey == KEYENTER)
    {
		while(1)
        {			
			iRet = UploadFile();
            if(iRet == PC_SAVEOK || iRet == PC_VER_CHECK_OK) {
				break;
			}else if(iRet == POS_NO_RECORD || iRet == OPEN_PORT_ERR || iRet == OPEN_RECORD_ERR){
                getkey();
                break;
            }else  if(CancelCheck())break;

        }
        if(iRet == PC_SAVEOK)  //版本信息不符
        {
            TestNum++;
            TestItem[AllNum-1].flag='Y';//最后一项是VER项
            TestItem[AllNum-1].result=0;
        }
        else if(iRet == PC_VER_CHECK_OK)  //版本信息正确
        {
            TestNum++;
			OKNum++;
            TestItem[AllNum-1].flag='Y';
            TestItem[AllNum-1].result=1;
            
        }
    }
#endif
	
    kbflush();
	ScrCls();
    ScrDispStr(0,0,0x81,"    整机测试    ", "Integration Test");
	ScrDispStr(0,2,1,"共有%d项测试",  "%dtests in total",TestNum);
    if(OKNum>=TestNum)
    {
		BeepOK();
		ScrDispStr(0,4,1,"%d项测试全部通过","%d tests all pass",TestNum);
    }
    else
    {
        BeepFail(); 
        ScrDispStr(0,4,1,"整机测试仅过%d项",	"%dtest pass",OKNum);
        ScrDispStr(0,6,1,"请按键查看失败项",	"CHECK FAIL ITEM");
        getkey();
        ScrClrLine(2,7);
		for(i=0,j=0,k=0;i<AllNum;i++)
		{
			if(TestItem[i].flag=='Y')
			{
				if(TestItem[i].result==0)
				{
					if(j%2==0)
						ScrPrint(0,(uchar)(j/2+2),0,"%s ",TestItem[i].itemStr);
					else
						ScrPrint(64,(uchar)(j/2+2),0,"%s ",TestItem[i].itemStr);
					j++;
				}
				if(++k >=TestNum)break;//可能中途退出，这时候TestNum并不是需要测试的项目合，应该是测试过的项目和
			}
		}

    }
    getkey();
}



void LanHubTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-LAN_HUB","1-LAN_HUB",	LanHubTest},
		{"2-LAN单次","2-LAN_ONE",	LanTest1  },
		{"3-LAN循环","3-LAN_CYC",	LanTest2  },
		{"1-USB主","2-USBH",	UsbDevTestAll  },
		{"2-USB从","3-USBD",	UDiskTestAll},
		{"3-串口","3-COM",		PortTestAll  },
	};
	
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"LANHUB TEST V1.0","LANHUB TEST V1.0",3);
}

