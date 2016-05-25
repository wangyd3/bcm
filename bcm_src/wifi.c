#include "public.h"

#define TXDLEN 512
uint gPort = 60180;
uchar TcpAddr[20]="124.172.162.188";


void SetPort(void)
{
	uchar ucRet;
	uchar temp[20]={0};
	ScrCls();
	ScrPrint(0,0,0x01,"Set TCP IP");
	ScrPrint(0,4,0,"TCP=%s",TcpAddr);
	ScrGotoxy(0,6);
	memcpy(temp,TcpAddr,sizeof(TcpAddr));
	ucRet = GetString(temp, 0xF5, 7, 15);
    if(ucRet!=0xff && temp[0]!=0)
    {
        memset(TcpAddr, 0x00, 20);
        memcpy(TcpAddr, temp+1, temp[0]);
	}

	ScrCls();
	ScrPrint(0,0,0x01,"Set TCP Port");
	ScrPrint(0,4,0,"Port=%d",gPort);
	memset(temp,0,sizeof(temp));
	ScrGotoxy(0,6);
	ucRet=GetString(temp,0xf7,1,14);
	if(ucRet!=0xff && temp[0]!=0)
	{
		temp[temp[0]+1]=0;
		gPort= atoi(&temp[1]);
	}

	ScrCls();
	ScrPrint(0,4,0,"TCPIP=%s",TcpAddr);
	ScrPrint(0,5,0,"Port=%d",gPort);
	ScrPrint(0,0,1,"AnyKey To Quit");
	getkey();
	
}

uchar TcpConnect(void)
{
	int iRet,Socket,TxLen,RxLen,i;
	uchar TxBuf[512],RxBuf[512];
	struct net_sockaddr addr;

	ScrClrLine(2,7);
	ScrDispStr(0,4,1,"连接服务器","Connect Server...");
	iRet = SockAddrSet(&addr,TcpAddr,gPort);
	if(iRet<0)
	{
		ScrClrLine(4,5);
		ScrPrint(0,4,0,"Set SocketAddr Err:%d", iRet);
		getkey();
		return 1;				
	}
	Socket = NetSocket(NET_AF_INET,NET_SOCK_STREAM,0);
	if (Socket<0)
	{
		ScrClrLine(4,5);
		ScrPrint(0,4,0, "Create Socket Err:%d", Socket);
		getkey();
		return 1;					
	}
	iRet = NetConnect(Socket, &addr, sizeof(addr));
	if (iRet <0)
	{
		ScrClrLine(4,5);
		ScrPrint(0,4,0,"Connect Server Err:%d", iRet);
		getkey();
		NetCloseSocket(Socket);
		return 1;
	}
	ScrClrLine(4,5);
	ScrPrint(0,4,1,"Sending Data...");
	memset(TxBuf, 0x00, sizeof(TxBuf));
	for (i=0; i<TXDLEN; i++)
	{
		TxBuf[i] = rand()%0xff;
	}
	TxLen = 0;
	TxLen = NetSend(Socket, TxBuf, TXDLEN, 0);
	if((TxLen < 0) || (TxLen != TXDLEN))
	{
		ScrClrLine(4,5);
		ScrPrint(0,4,0,"Sending Data Err:%d", TxLen);
		getkey();
		NetCloseSocket(Socket);
		return 1;
	}
	ScrClrLine(4,5);
	ScrPrint(0,4,0,"Receiving Data...");
	TimerSet(0, 100);
	memset(RxBuf, 0x00, sizeof(RxBuf));
	RxLen=0;
	while((RxLen <TxLen) && TimerCheck(0))
	{
		iRet = NetRecv(Socket,&RxBuf[RxLen],sizeof(RxBuf)-RxLen,0);
		if (iRet >= 0)
		{
			  RxLen += iRet;
		}
		else 
		{
			ScrPrint(0,4,0,"NetRecv  Err:%d", iRet);
			getkey();
		}
		if(RxLen)
		{
			break;
		}
	}
	if(RxLen != TXDLEN)
	{
		ScrClrLine(4,5);
		ScrPrint(0,4,0, "Receiving Data Err:%d", RxLen);
		getkey();
		return 1;
	}
	else if((memcmp(TxBuf, RxBuf, TXDLEN)))
	{
		ScrClrLine(4,5);
		ScrPrint(0,4,0,"Data Content Err");
		getkey();
		return 1;

	}	
	NetCloseSocket(Socket);
	return 0;
}


void WifiUpdate(void)
{
    uchar ucRet,ch;
    int iRet;

    ScrCls();
	ScrDispStr(0,0,0x81,"    串口转接    ","   COM RELAY    ");
    ucRet = PortOpen(0, "115200,8,n,1");
	if(ucRet)
	{
		ScrPrint(0,4,1,"PortOpen(0)=%d",ucRet);
		getkey();
		PortClose(0);
		return ;
	}

	iRet=WifiOpen();
	if(iRet)
	{
		ScrPrint(0,4,1,"WifiOpen=%d",iRet);
		getkey();
		WifiClose();
		return;
	}

    ScrDispStr(0,4,0x01,"    转接中...   ","   Relaying...  ");
    while(1)
    {
		if(CancelCheck())break;
		if(!PortRecv(0, &ch, 0))
			PortSend(5, ch);
		if(!PortRecv(5, &ch, 0))
			PortSend(0, ch);
    }
	PortClose(0);
	WifiClose();
}


int SelectAp(ST_WIFI_AP *Aps,int iScanNum,ST_WIFI_AP *Ap,ST_WIFI_PARAM *wifi_para)
{
	int i,j;
	uchar temp[30],ucRet;
	while(1)
	{
		ScrClrLine(2,7);
		for(i = 0; i < iScanNum; i++)
		{
			j=i/6;
			ScrPrint(0,i%6+2,0,"%d(%d):%s",i%6+1,Aps[i].Rssi,Aps[i].Ssid);
			if(i%6==5 || i==iScanNum-1)
			{
				ScrPrint(96,1,0," %d ",iScanNum);
				ucRet=getkey();
				if(ucRet==KEYCANCEL)return 1;
				if(ucRet>=KEY1 && ucRet<=KEY6)
				{
					if(j*6+ucRet-KEY1<iScanNum)
					{
						i=j*6+ucRet-KEY1;
						j=100;//为了跳出while
						break;
					}
				}
				ScrClrLine(2,7);
			}
		}
		if(j!=100)continue;
		memcpy(Ap,Aps+i,sizeof(ST_WIFI_AP));
		ScrClrLine(2,7);
		ScrDispStr(0,3,0, "AP:%s", "AP:%s",Ap->Ssid);
		ScrDispStr(0,4,1, "输入密码:", "Input Pwd:");
		ScrFontSet(1);
		ScrGotoxy(0,6);
		memset(temp, 0, sizeof(temp));
		ucRet = GetString(temp, 0xF5, 1, 28);
		if(ucRet!=0xff && temp[0]!=0)
		{
			strcpy(wifi_para->Wpa,temp+1);
		}
		break;
	}
	return 0;
}



// flag:  0-整机 1-单次 2-成功率 3-TCP循环
uchar WifiTest(uchar flag)
{
    uchar temp[30];
    int iRet,fid,i,error;
	uchar *pstr,ucRet;
	ushort TotalNum, FailNum;
	uchar AutoSetIP = 0;
	uchar ssid[SSID_MAXLEN];
	static int key;
	int errno;

	ST_WIFI_AP WIFI_APs[15];
	ST_WIFI_AP WIFI_AP;
	ST_WIFI_PARAM WIFI_PARAM;

	RouteSetDefault(ROUTE_WIFI);

	memset(WIFI_APs, 0, sizeof(WIFI_APs));
	memset(&WIFI_AP, 0, sizeof(WIFI_AP));
	memset(&WIFI_PARAM, 0, sizeof(WIFI_PARAM));
	memset(ssid, 0, sizeof(ssid));
	ScrClrLine(2,7);
	TotalNum=0; 
	FailNum=0;

	if (flag == 0) ScrDispStr(0,2,1,"WIFI测试","WIFI Test");
	if (flag == 1) ScrDispStr(0,0,1,"单次测试","Single Test");
	

	if(flag == 0){
		AutoSetIP = 1;
	}
	else{
		ScrDispStr(0,2,1, "1-自动分配IP", "1-AutoSetIP");
		ScrDispStr(0,4,1, "2-手动分配IP", "2-ManuSetIP");
		kbflush();
		ucRet = getkey();
		ScrClrLine(2,7);
		if (ucRet == KEY1){
			AutoSetIP = 1;
		}
		else if (ucRet == KEY2){
			AutoSetIP = 0;
		}
	}
	
	if (AutoSetIP == 0)
	{
		uchar ip;
		int i; 
		AutoSetIP = 0;
		ScrClrLine(2,7);
		memset(temp, 0x00, sizeof(temp));
		ScrDispStr(2,0,1,"设置Wifi本地IP","Wifi Local IP");
		ip=(rand()%250)+1;
		if(ip==1)  ip=2;          //如果取得的随机本地IP与无线路由器IP一样，则本地IP取为2
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
		sprintf(pstr, "%d", ip);
		ScrPrint(0,4,1,"%s",LANPara[0]);
		ScrGotoxy(0,6);
		memcpy(temp, LANPara[0], 20);
		ucRet = GetString(temp, 0xF5, 7, 15);
		if(ucRet!=0xff && temp[0]!=0)
		{
			memset(LANPara[0], 0x00, 20);
			memcpy(LANPara[0], temp+1, temp[0]);
			fid = open(F_LANPARA,O_RDWR);
			if(fid < 0)
			{
				error = GetLastError();   //获取文件操作出错错误码
				if(error)
				{
					ShowFileCtrlErr(__LINE__);
				}
			}
			else
			{
				iRet = seek(fid,0*20,SEEK_SET);
				if(iRet < 0)
				{
					error = GetLastError();   //获取文件操作出错错误码
					if(error)
					{
						ShowFileCtrlErr(__LINE__);
					}
				}
				iRet = write(fid,LANPara[0],20);
				if(iRet !=20)
				{
					error = GetLastError();   //获取文件操作出错错误码
					if(error)
					{
						ShowFileCtrlErr(__LINE__);
					}
				}
			}
			close(fid);
		}
	}	


	while(1)
	{
		while(2)
		{
			
		
			ScrClrLine(4,7);
			if(flag>=2)
			{
				ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
				if(TotalNum > 1000)break;
				if(CancelCheck())break;
			}
			
			errno = 1;
			while(3)
			{
				iRet = WifiOpen();
				if(iRet)
				{
					ScrDispStr(0,4,1,"WIFI打开出错%d","WifiOpen=%d",iRet);
					sprintf(gErrnoMsg,"WifiOpen=%d",iRet);
					break;
				}

				WifiCtrl(3,NULL,0,gWifiMac,6);
				ScrDispStr(0,4,1,"扫描AP...","Scan AP...");
				iRet = WifiScan(WIFI_APs, sizeof(WIFI_APs)/sizeof(ST_WIFI_AP));
				if(iRet <= 0)
				{
					ScrDispStr(0,4,1,"扫描出错%d","ScanApErr=%d",iRet);
					sprintf(gErrnoMsg,"WifiScan=%d",iRet);
					break;
				}
				else
				{					
					if(flag == 0)
					{
						for(i=0; i<iRet; i++)
						{
							if(strstr(WIFI_APs[i].Ssid,"WIFIHOST")!=NULL) 
							{
									strcpy(WIFI_AP.Ssid, WIFI_APs[i].Ssid);
									WIFI_AP.Rssi = WIFI_APs[i].Rssi;
									WIFI_AP.SecMode = WIFI_APs[i].SecMode;
									break;
							}
						}

						if(i == iRet)
						{
							ScrPrint(0,4,1,"NO WIFIHOST AP");
							sprintf(gErrnoMsg,"NO WIFIHOST AP");
							break;
						}
					}
					else if(  flag>=1 && TotalNum<=1 )
					{
						if ( SelectAp(WIFI_APs,iRet,&WIFI_AP,&WIFI_PARAM) )
						{   //取消测试
							break;
						}
					}
				}
				
				if(AutoSetIP==1)
				{
					WIFI_PARAM.DhcpEnable = 1;
				}
				else
				{
					WIFI_PARAM.DhcpEnable = 0;
					strcpy(WIFI_PARAM.Ip, LANPara[0]);
				}

				if(TotalNum <= 1) ScrCls();
				ScrDispStr(0,4,1, "WIFI与AP连接中", "WIFI Connect AP");
				ScrPrint(0,6,0,"%s,%d",WIFI_AP.Ssid,WIFI_PARAM.DhcpEnable);
				ScrPrint(0,7,0,"%s,%s",WIFI_PARAM.Wpa,WIFI_PARAM.Wep);
				iRet = WifiConnect(&WIFI_AP, &WIFI_PARAM);
				if(iRet)
				{
					ScrDispStr(0,4,1,"WIFI连接出错%d","WIFIConnect=%d  ",iRet);
					sprintf(gErrnoMsg,"WifiConnect=%d",iRet);
					break;
				}

				ScrClrLine(2,5);
				ScrDispStr(0,4,1, "WIFI Check...", "WIFI Check...");
				TimerSet(3, 150);
				while(TimerCheck(3))
				{
					iRet = WifiCheck(&WIFI_AP);
					DelayMs(10);
					if(iRet > 0) break;
				}

				if(iRet <= 0)
				{
					ScrDispStr(0,4,1,"WifiCheck=%d","WifiCheck=%d",iRet);
					sprintf(gErrnoMsg,"WifiCheck=%d",iRet);					
					break;
				}
				if(flag == 1)
				{
					char host_ip_get[30]; 
					char host_mask_get[30];
					char gw_ip_get[30];
					char dns_ip_get[30];
					long state;

					ScrClrLine(2,7);
					iRet = NetDevGet(12,host_ip_get,host_mask_get,gw_ip_get,dns_ip_get);
					ScrPrint(0,3,0,"IP:%s",host_ip_get);
					ScrPrint(0,4,0,"MASK:%s",host_mask_get);
					ScrPrint(0,5,0,"GATE:%s",gw_ip_get);
					ScrPrint(0,6,0,"S_IP:%s",dns_ip_get);
					getkey();	
					
				}

				
				if(flag==3)
				{
					iRet=TcpConnect();
					if(iRet)break;
				}

				ScrClrLine(2,7);
				ScrDispStr(0,4,1, "WIFI断开连接...", "WIFI DisConn...");
				iRet = WifiDisconnect();
				if(iRet)
				{
					ScrDispStr(0,4,1,"WIFI断开出错%d","WifiDisconnect=%d",iRet);
					sprintf(gErrnoMsg,"WifiDisconnect=%d",iRet);
					break;
				}

				ScrClrLine(2,7);
				ScrDispStr(0,4,1, "WIFI关闭...", "WIFI Close...");
				iRet = WifiClose();
				if(iRet)
				{
					ScrDispStr(0,4,1,"WIFI关闭出错%d","WifiClose=%d",iRet);
					sprintf(gErrnoMsg,"WifiClose=%d",iRet);
					break;
				}
				errno = 0;
				break;
			}

					
			if(errno==0)
			{
				ScrClrLine(2,7);
				ScrDispStr(0,4,1,"测试成功","WIFI TEST OK");
				BeepOK();
				DelayMs(500);
				if(flag <= 1)
					goto ENDOK;
				else
					continue;
			}
			else
			{
				ScrDispStr(0,6,1,"测试失败","WIFI TEST FAIL");
				FailNum ++;
				BeepFail();
				if(flag <= 1)
				{
					getkey();
					goto ENDERR;
				}
				else
				{
					DelayMs(500);
					continue;
				}
			}
		}//while(2)
		if(!TotalNum) goto ENDERR;
		ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",TotalNum);
		ShowSuccessRate(TotalNum,FailNum);
		if(kbflush(),getkey()==KEYCANCEL)
		{
			goto ENDERR;
		}
	}//while(1)
ENDOK:
	WifiClose();
	return 0;
ENDERR:
	WifiClose();
	return 1;
}



void WifiTest1(void)
{
	WifiTest(1);
}

void WifiTest2(void)
{
	WifiTest(2);
}

void WifiTest3(void)
{
	WifiTest(3);
}


void WifiTestAll(void)
{
	uchar TestNum;

	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",			"1-Single",		WifiTest1,},
		{"2-循环",			"2-Cycle ",		WifiTest2,},
		{"3-转接",			"3-Relay ",		WifiUpdate,},
		{"1-TCP ",			"1-TCP",		WifiTest3},
		{"2-设置TCP参数",	"2-Set Port",	SetPort},

	};
	
	if(TestModule[T_WIFI]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"本机无WIFI模块", "No WIFI Module");
		getkey();
		return ;
	}
	
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"WIFI单项测试","WIFI Test",3);
}





