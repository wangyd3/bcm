#include "public.h"

void WNetUpdate(void)
{
    ScrClrLine(2,7);
	ScrDispStr(0,4,1,"    串口转接    ","   COM RELAY    ");
	//WlSwitchPower(1);//加这句话不要WlInit(NULL);是不行的
    PortOpen(0,"115200,8,n,1");
	WlInit(NULL);//少了这个进行不了转接,即使下电了,这个命令会先进行上电
	WlOpenPort();
	WlSendCmd(0,0,0,0,0);
	WlClosePort();
	PortClose(0);  
}

uchar dialNum[30];
uchar uid[10];
uchar pwd[10];
#define CLARO_CARD 1
#define VIVO_CARD  2
void SelectApn(void)
{
#ifdef BAXI
	int i,j,card;
	char rsp[50];
	
	memset(dialNum,0,sizeof(dialNum));
	memset(uid,0,sizeof(uid));
	memset(pwd,0,sizeof(pwd));
	
	card = CLARO_CARD;
	i = 0;
	while(i++ < 3)
	{
		
		WlOpenPort();
		WlSendCmdEx("AT+COPS?\r",rsp,sizeof(rsp));
		ScrPrint(0,6,0,"info=%s",rsp);
		for(j=0;j<strlen(rsp);j++)
		{
			if(rsp[j]>='a' && rsp[j]<='z')
				rsp[j] = rsp[j] - 'a' + 'A';
		}
		ScrPrint(0,7,0,"info=%s",rsp);
		DelayMs(1000);
		
		if(strstr(rsp,"CLARO"))card = CLARO_CARD;
		else if(strstr(rsp,"VIVO")) card = VIVO_CARD;
		else continue;
		break;
	}
	if(card == CLARO_CARD)
	{
		memcpy(uid,"claro",5);
		memcpy(pwd,"claro",5);
		memcpy(dialNum, "claro.com.br",sizeof("claro.com.br"));
	}
	else if(card == VIVO_CARD)
	{
		memcpy(uid,"vivo",4);
		memcpy(pwd,"vivo",4);
		memcpy(dialNum, "mms.vivo.com.br",sizeof("mms.vivo.com.br"));
	}
#endif

}


void SelectApn2(uchar flag)
{
	uchar ucRet;
	memset(dialNum, 0x00, sizeof(dialNum));
	memset(uid,0x00,sizeof(uid)); //初始化用户名
	memset(pwd,0x00,sizeof(pwd)); //初始化密码
	ScrClrLine(2,7);
#ifdef BAXI
	if(flag) //不是整机测试
	{
		
		ScrPrint(0,2,0,"1-claro");
		ScrPrint(0,3,0,"2-oi");
		ScrPrint(0,4,0,"3-tim");
		ScrPrint(0,5,0,"4-vivo");
		ScrPrint(0,6,0,"5-zap.vivo");
		ScrPrint(0,7,0,"6-mms.vivo");
		//ScrPrint(0,7,0,"7-wap.vivo");
		ucRet = getkey();
		if(ucRet==KEY1)
		{
			memcpy(uid,"claro",5); //设置用户名
			memcpy(pwd,"claro",5); //设置密码
			if(TermInfo[9])memcpy(dialNum, "claro.com.br",sizeof("claro.com.br"));
			else if(TermInfo[10])memcpy(dialNum, "#777", 4);
			else if(TermInfo[18])memcpy(dialNum, "3GNET", 5);
		}
		else if(ucRet==KEY2)
		{
			//memcpy(uid,"card",4); //设置用户名
			//memcpy(pwd,"card",4); //设置密码
			if(TermInfo[9])memcpy(dialNum, "gprs.oi.com.br",sizeof("gprs.oi.com.br"));
			else if(TermInfo[10])memcpy(dialNum, "#777", 4);
			else if(TermInfo[18])memcpy(dialNum, "3GNET", 5);
		}
		else if(ucRet==KEY3)
		{
			memcpy(uid,"tim",3); //设置用户名
			memcpy(pwd,"tim",3); //设置密码
			if(TermInfo[9])memcpy(dialNum, "tim.br",sizeof("tim.br"));
			else if(TermInfo[10])memcpy(dialNum, "#777", 4);
			else if(TermInfo[18])memcpy(dialNum, "3GNET", 5);
		
		}
		else
		{
			memcpy(uid,"vivo",4); //设置用户名
			memcpy(pwd,"vivo",4); //设置密码
			if(TermInfo[9])
			{
				if(ucRet==KEY4)memcpy(dialNum, "vivo.com.br",sizeof("vivo.com.br"));
				else if(ucRet==KEY5)memcpy(dialNum, "zap.vivo.com.br",sizeof("zap.vivo.com.br"));
				else if(ucRet==KEY6)memcpy(dialNum, "mms.vivo.com.br",sizeof("mms.vivo.com.br"));
				else memcpy(dialNum, "wap.vivo.com.br",sizeof("wap.vivo.com.br"));
			}
			else if(TermInfo[10])memcpy(dialNum, "#777", 4);
			else if(TermInfo[18])memcpy(dialNum, "3GNET", 5);
		}
	}
	else  if(0)//整机测试
	{

		ScrPrint(0,2,1,"1-claro");
		ScrPrint(0,4,1,"2-mms.vivo");
		ucRet = getkey();
		if(ucRet==KEY1)
		{
			memcpy(uid,"claro",5); //设置用户名
			memcpy(pwd,"claro",5); //设置密码
			if(TermInfo[9])memcpy(dialNum, "claro.com.br",sizeof("claro.com.br"));
		}
		else
		{
			memcpy(uid,"vivo",4); //设置用户名
			memcpy(pwd,"vivo",4); //设置密码
			if(TermInfo[9])memcpy(dialNum, "mms.vivo.com.br",sizeof("mms.vivo.com.br"));
		}		
		if(TermInfo[10])memcpy(dialNum, "#777", 4);
		if(TermInfo[18])memcpy(dialNum, "3GNET", 5);
	}

#elif defined Venezuela //
	if(flag)
	{
		ScrPrint(0,2,1,"1-internet");
		ScrPrint(0,4,1,"2-gprsweb");
		ScrPrint(0,6,1,"3-int");
		ucRet = getkey();
		if(ucRet==KEY1){
			memcpy(dialNum,"internet.movistar.ve",sizeof("internet.movistar.ve"));
		} else if(ucRet==KEY2) {
			memcpy(dialNum,"gprsweb.digitel.ve",sizeof("gprsweb.digitel.ve"));
		} else { 
			memcpy(dialNum,"int.movilnet.com.ve",sizeof("int.movilnet.com.ve"));	
		}
	}
	else
	{
		memcpy(dialNum,"gprsweb.digitel.ve",sizeof("gprsweb.digitel.ve"));		
	}
#else //BCM  //国内
	memcpy(uid,"card",4); //设置用户名
	memcpy(pwd,"card",4); //设置密码
	if(TermInfo[9] && 0)memcpy(dialNum,"smartone-vodafone",17);
	else if(TermInfo[9])memcpy(dialNum, "cmnet",5);
	else if(TermInfo[10])memcpy(dialNum, "#777", 4);
	else if(TermInfo[18])memcpy(dialNum, "3GNET", 5);
#endif

}




// flag  0-整机 1-单项 2-循环 3-成功率 4-接口板 5-锁定频段
uchar WnetTest(uchar flag)
{
	long TotalNum,FailNum, DialCnt;
	int i, count, iRet, iVer;
	unsigned char pSignalLevel, pSignalLevelTemp;
	uchar Sim=0,ChkSignal=0,ucRet;
	uchar SimInfo,rsp[2][50];

	//memset(dialNum, 0x00, sizeof(dialNum));
	//memset(uid,0x00,sizeof(uid)); //初始化用户名
	//memset(pwd,0x00,sizeof(pwd)); //初始化密码
	//memcpy(uid,"CARD",4); //设置用户名
	//memcpy(pwd,"CARD",4); //设置密码
	
	SetComPort(WNETPORT); //设置wnet通讯口；
	RouteSetDefault(ROUTE_WNET);//配置系统缺省路由WNET
	kbflush();
	ScrClrLine(2,7);

	SelectApn2(flag);

	if (flag == 0) ScrDispStr(0,2,1,"无线测试","WNET Test");
	if (flag == 1) ScrDispStr(0,0,1,"单次测试","Single Test");
	if (flag == 2) ScrDispStr(0,0,1,"循环测试","Cycle Test ");
	if (flag == 3) ScrDispStr(0,0,1,"信号测试","Signal Test");
	if (flag==3){ flag=1;ChkSignal=1;}
	
	TotalNum=0; 
	FailNum=0;
	iVer = 0;

	ScrDispStr(0,2,1, "无线模块测试", "WNET TEST");
	if(flag!=0) WlSwitchPower(1);

	while(1)
	{
		while(2)
		{
			SimInfo = 0;
			if(flag==2)
			{
				ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
				if(TotalNum==10000)break;
				if(CancelCheck())break;
			}
			
			for(Sim=0; Sim<gSimNum; Sim++)
			{
				if(gSimNum==2)//双SIM卡
				{
					ScrClrLine(2,7);
					ScrDispStr(0,4,1, "选择SIM卡%d", "Select SIM %d",Sim);
					iRet = WlSelSim(Sim);
					if(iRet<0)
					{
						BeepFail();
						sprintf(gErrnoMsg,"WlSelSim(%d)=%d",Sim,iRet);
						ScrDispStr(0,4,1,"选择SIM卡%d失败!%d", "Select SIM%d fail!%d",Sim,iRet);
						DelayMs(1000);
						if(flag==1 || flag==0 || flag==4) goto ENDERR;	
						if(flag==2)	break;
					}
				}
				
				ScrClrLine(2,7);
				ScrDispStr(0,4,1, "模块初始化...", "WNET Init...");
				iRet = WlInit(NULL);
				
				if(flag==0)
				{
					SelectApn();
					ScrPrint(0,6,1,dialNum);
				}

#ifdef BCM
				//获取SIM卡运营商的信息
				if(gSimNum==2 && TestModule[T_GPRS]=='Y' && flag<2)
				{
					i = 0;
					while(i++ < 3)
					{
						WlOpenPort();
						WlSendCmdEx("AT+COPS?\r",rsp[Sim],sizeof(rsp[Sim]));
						ScrPrint(0,6,0,"%d=%s",Sim,rsp[Sim]);
						DelayMs(1000);
						if(strstr(rsp[Sim],"MOBILE")) SimInfo |= 0x01;
						else if(strstr(rsp[Sim],"UNICOM")) SimInfo |= 0x02;
						else continue;
						break;
					}
					if(Sim==1 && SimInfo!=0x03)
					{
						ScrDispStr(0,4,1,"SIM 信息错误!","Sim Info Error");
						ScrPrint(0,6,0,"%d,%s",SimInfo,rsp[0]);
						ScrPrint(0,7,0,"%d,%s",SimInfo,rsp[1]);
						sprintf(gErrnoMsg,"SIM INFO = %d,%s,%s",SimInfo,rsp[0],rsp[1]);
						BeepFail();
						getkey();
						goto ENDERR;
					}
				}

#endif
				if(Sim==1 && flag==0)//1通道整机测试只判断初始化
				{
					if(iRet==RET_OK || iRet==WL_RET_ERR_INIT_ONCE)
					{
						BeepOK();
						ScrDispStr(0,4,1,"SIM%d测试成功!","SIM%d TEST OK!",Sim);
						DelayMs(500);
						goto ENDOK;
					}
					else
					{
						sprintf(gErrnoMsg,"WlInit(1)=%d",iRet);
						ScrDispStr(0,4,1,"WlInit(1)=%d","WlInit(1)=%d",iRet);
						DelayMs(500);
						BeepFail();
						goto ENDERR;
					}
				}
				
				ScrClrLine(4,7);
				if(iRet==WL_RET_ERR_NOSIM) //flag=4是接口板测试，没有插卡
				{
					if(flag!=4)
					{
						sprintf(gErrnoMsg,"WlInit(%d)=%d",Sim,iRet);
						ScrDispStr(0,4,1,"请插入SIM卡%d", "No SIM Card%d",iRet);
						BeepFail(); 
						getkey();
						if(flag==0 || flag==1 || flag==4)goto ENDERR;
						else  break;
					}
				}
				else if(iRet!=RET_OK && iRet!=WL_RET_ERR_INIT_ONCE)
				{
					sprintf(gErrnoMsg,"WlInit(%d)=%d",Sim,iRet);
					ScrDispStr(0,4,1, "模块初始化失败:%d", "WNet Init Fail:%d",iRet);
					BeepFail();
					getkey();
					if(flag==0 || flag==1 || flag==4)	goto ENDERR;
					else break;
				}
				
				iRet = GetWirelessVer();

				ScrClrLine(2,7);
				ScrDispStr(0,2,1,"版本:%s,%s","VERSION:%s,%s",gWireVer,gWnetVer);
				//ScrPrint(0,4,1,"%s,%s",gWireVer,gWnetVer);   //显示版本。
				if(flag==4 || strstr(InfoOut[C_PN],"S90-MC0-363-01EA")!=NULL)
				{
					ScrDispStr(0,6,1,"无线测试成功","WNET TEST OK");
					BeepOK();
					DelayMs(1000);
					if(!iRet)goto ENDOK;
					else goto ENDERR;
				}

				if(WaitEsc(0,1))
				{
					if(flag==1 || flag==0)goto ENDERR;
					else goto ENDTEST;//TotalNum--;
				}
	
				ScrClrLine(2,7);
				ScrDispStr(0,4,1,"查询信号强度","Check Signal");					
				iRet=WlGetSignal(&pSignalLevel); //查询信号强度
				if(iRet==-202)//第一次整机测试成功后测试第二次（无论单次还整机），都返回-202（没有应答），所以需要重新上电
				{
					ScrDispStr(0,4,1,"模块重新上电...","Module Power On");	
					WlSwitchPower(0);
					WlSwitchPower(1);
					WlInit(NULL);
					DelayMs(4000);
					ScrClrLine(4,7);
					ScrDispStr(0,4,1,"查询信号强度","Check Signal");	
					
				}
				
				i = 0;
				while(1)
				{
					iRet=WlGetSignal(&pSignalLevel); //查询信号强度
					if(pSignalLevel<=4 || CancelCheck())break;
					if(flag==0 && i==5) break; //整机
					if(flag!=0 && i==50) break;//其他非整机情况
					
					if(!(i%3)) ScrPrint(0,6,1,"....");
					else if((i%3)==1) ScrPrint(0,6,1,"........");
					else if((i%3)==2) ScrPrint(0,6,1,"............ ");
					DelayMs(100);
					i++;
				}
				if(iRet)
				{
					ScrClrLine(4,7);
					sprintf(gErrnoMsg,"WlGetSignal=%d",iRet);
					ScrDispStr(0,4,1,"查信号失败:%d","Check Signal:%d",iRet);
					if(WaitEsc(0,1))
					{
						if(flag==1 || flag==0)goto ENDERR;
						else goto ENDTEST;//TotalNum--;
					}
				}
				
				ScrClrLine(4,7);
				switch(pSignalLevel) {
				case 0:ScrDispStr(0,4,1,"信号最强","Signal Highest");break;
				case 1:ScrDispStr(0,4,1,"信号较强","Signal Higher");break;
				case 2:ScrDispStr(0,4,1,"信号一般","Signal Normal");break;
				case 3:ScrDispStr(0,4,1,"信号较弱","Signal Weak");break;
				case 4:ScrDispStr(0,4,1,"信号很弱","Signal Weaker");break;
				default:ScrDispStr(0,4,1,"无信号","No Signal");break;
				}			
				
				pSignalLevelTemp = pSignalLevel;
				pSignalLevelTemp = (pSignalLevelTemp <= 5)? (5 - pSignalLevelTemp):0;//根据信号强度点亮图标
				ScrSetIcon(2, (unsigned char)(1+pSignalLevelTemp));
				
				if(WaitEsc(0,1))
				{
					if(flag==1 || flag==0)goto ENDERR;
					else goto ENDTEST;//TotalNum--;
				}
			
				ScrClrLine(4,7);
				
				if(flag==0 || flag==1) DialCnt=3; //整机与单次，三次拔号机会
				else DialCnt=1;//


				if(strstr((char *)gWnetVer, "EM701"))//这个是没有PPP链接的,只能根据信号值判断
				{
					ScrClrLine(4,7);
					ScrPrint(0,4,1,"Signal Value = %d", pSignalLevel);
					sprintf(gErrnoMsg,"SignalValue=%d",pSignalLevel);
					DelayMs(1000);
					if(pSignalLevel>3) //0~5 越大表示信号越弱
						goto ENDERR;
				}
				else
				{
					ScrClrLine(4,7);
					for(count=0; count<DialCnt; count++)  // 三次拔号机会
					{
						ScrDispStr(0,4,1,"正在PPP连接","PPP Connecting");
						if(!(count%3)) ScrPrint(0,6,1,".....");
						else if((count%3)==1) ScrPrint(0,6,1,"..........");
						else if((count%3)==2) ScrPrint(0,6,1,"...............");
						if(CancelCheck())break;

						//2009-04-14 表示一直等待到启用PPP链路成功或失败,防止退出无线模块测试，串口仍占用
						#ifdef  Venezuela
						iRet = WlPppLogin(dialNum,NULL,NULL, 0xff, -1, 10);  
						#else
						iRet = WlPppLogin(dialNum, uid,pwd, 0xff, -1, 10);
						#endif
						
						if(iRet==RET_OK) break;
					}//for

					if(iRet != RET_OK)
					{
						ScrClrLine(4,7);
						sprintf(gErrnoMsg,"WlPppLogin=%d",iRet);
						ScrDispStr(0,4,1,"PPP连接失败:%d","PPP fail:%d",iRet);
					}

					ScrDispStr(0,6,1,"断开PPP连接","PPP Disconnect");
					WlPppLogout();
					while( WlPppCheck()>=0 );  //等待链路断开			
					
				}

				if(iRet==0)
				{
					BeepOK();
					ScrDispStr(0,6,1,"SIM%d测试成功!","SIM%d TEST OK!",Sim);					
					if(WaitEsc(0,1))
					{
						if(flag==2)goto ENDTEST;//TotalNum--;
					}
					if(flag==1 || flag==0)
					{
						if(gSimNum==2 && Sim==0) continue;//双SIM卡
						goto ENDOK;
					}
					else if(flag==2)
					{
						continue;//for(Sim=0; Sim<=SimSlot; Sim++)
					}
					
				}
				else
				{
					//FailNum ++;
					BeepFail();
					if(flag==1 || flag==0)
					{
						ScrDispStr(0,6,1,"SIM%d测试失败!","SIM%d TEST Fail!",Sim);
						DelayMs(1500);
						goto ENDERR;
					}
					else if(flag == 2)
					{
						continue;// for(Sim=0; Sim<=SimSlot; Sim++)
					}
					
				}
			}//for(Sim=0; Sim<=SimSlot; Sim++)
			if(iRet)FailNum++;//for里面break后就到这里判断
			
		}//while(2)
ENDTEST:
		ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",TotalNum);
		ShowSuccessRate(TotalNum,FailNum);
		kbflush();
		if(getkey()==KEYCANCEL) break;
	}//while(1)
ENDOK:
	ScrSetIcon(2,0);	
	return 0;
ENDERR:
	ScrSetIcon(2,0);
	return 1;

}



void WnetTest0(void)
{
	WnetTest(0);
}

void WnetTest1(void)
{
	WnetTest(1);
}
void WnetTest2(void)
{
	WnetTest(2);
}

void WnetTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",		"1-Single",		WnetTest1},
		{"2-循环",		"2-Cycle ",		WnetTest2},
		{"3-转接",		"3-Relay ",		WNetUpdate},
		{"1-整机",		"1-Wnet0",		WnetTest0},
	};
	
	if(TestModule[T_WNET]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"本机无无线模块", "No WNET Module");
		getkey();
		return ;
	}

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"无线单项测试","WNET Test",2);
	

}
