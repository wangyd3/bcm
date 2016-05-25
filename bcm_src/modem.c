#include "public.h"

static COMM_PARA modem_para;
void ModemInit(void)
{
	modem_para.DP=0;		//双音多频拨号
	modem_para.CHDT=0x00;	//检测拨号音  2009-03-23 由0x41改为0x00 //0x00
	modem_para.DT1=20;		//摘机后等待拨号音的最长时间：20*100ms
	modem_para.DT2=5;		//拨外线时","等待时间：5*100ms
	modem_para.HT=100;		//双音拨号单一号码保持时间：100*1ms
	modem_para.WT=5;		//双音拨号两个号码之间的间隔时间：5*10ms
	modem_para.SSETUP=0x00; //同步、1200、CCITT、等待应答音的超时时间：0x00 - 5s;
							//										   0x02 - 11s  
//	modem_para.SSETUP=0x80; //异步拨号
	modem_para.DTIMES=1;	//循环拨号次数：1
	modem_para.TimeOut=6;	//通讯超时挂断：6*10s
	modem_para.AsMode=0;	//由SSETUP的D6D5设置波特率
}

int ModemTelNum(void)
{
	int fid;
	int iRet;

	ScrClrLine(2,7);
	ScrDispStr(0,2,1,"请接好电话线路..","Check Phone Link");			
	BeepOK();
	getkey();
	if(InputAsc("请输入拨号号码","Input dial num",TelNum, 0, 20, 1)) return 1;
	fid=open(F_TELNUM,O_RDWR);
	if(fid<0)
	{
		ShowFileCtrlErr(__LINE__);  //获取文件操作出错错误码
	}
	else
	{
		iRet = write(fid,TelNum,sizeof(TelNum));
		if(iRet != sizeof(TelNum))
		{
			ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
		}
	}
	close(fid);
	return 0;
}


uchar ModemSetLevel(void)
{
	uchar ucRet;
	uchar DialLevel[20]="11";
	int i;
	ScrClrLine(2,7);
	ScrPrint(0,2,1,"Select Level(db)");
	ScrPrint(0,4,0,"1:-2db  2:-4   3:-6 ");
	ScrPrint(0,5,0,"4:-8db  5:-10  6:-12");
	ScrPrint(0,6,0,"7:-14db 8:other");
	ScrPrint(0,7,0,"OTHER:defaut(-11db)");
	ucRet=getkey();
	if(ucRet>=KEY1 && ucRet<=KEY7) return (ucRet-0x30)*2; 
	else if(ucRet==KEY8)
	{
		ScrClrLine(2,7);
		if(InputAsc("输入电平:(-db)","INPUT LEVEL(-db)",DialLevel,0,2, 1)) return 1;
		i=atoi(DialLevel);
		if(i>0 && i<16) return i;
		else return 0;
	}
	else return 0;
}

void SetLevel(uchar Level)
{
	switch(Level)
	{
	case 1:		ModemExCommand("ATS91=1",   NULL, NULL, 0); break;
	case 2:		ModemExCommand("ATS91=2",   NULL, NULL, 0); break;
	case 3:		ModemExCommand("ATS91=3",   NULL, NULL, 0); break;
	case 4:		ModemExCommand("ATS91=4",   NULL, NULL, 0); break;
	case 5:		ModemExCommand("ATS91=5",   NULL, NULL, 0); break;
	case 6:		ModemExCommand("ATS91=6",   NULL, NULL, 0); break;
	case 7:		ModemExCommand("ATS91=7",   NULL, NULL, 0); break;
	case 8:		ModemExCommand("ATS91=8",   NULL, NULL, 0); break;
	case 9:		ModemExCommand("ATS91=9",   NULL, NULL, 0); break;
	case 10:	ModemExCommand("ATS91=10",   NULL, NULL, 0); break;
	case 11:	ModemExCommand("ATS91=11",   NULL, NULL, 0); break;
	case 12:	ModemExCommand("ATS91=12",   NULL, NULL, 0); break;
	case 13:	ModemExCommand("ATS91=13",   NULL, NULL, 0); break;
	case 14:	ModemExCommand("ATS91=14",   NULL, NULL, 0); break;
	case 15:	ModemExCommand("ATS91=15",   NULL, NULL, 0); break;
	default:break;
	}
}





void ModemTest1(void)
{
	ModemTest(1);
}

void ModemTest2(void)
{
	ModemTest(2);
}

void ModemTest3(void)
{
	ModemTest(3);
}

void ModemTest5(void)
{
	ModemTest(5);
}

void ByPhoneTest4(void)
{
	ByPhoneTest(4);
}



void ModemTestAll(void)
{
	
		uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",		"1-Single",		ModemTest1,},//modem_test  ModemTest1
		{"2-循环",		"2-Cycle",		ModemTest2,},
		{"3-脉冲",		"3-Pulse",		ModemTest3,},
		{"1-旁置电话",	"1-ByPhone",	ByPhoneTest4,},
		{"2-可设置电平","2-CanSetLevel",ModemTest5, }
	};

	if(TestModule[T_MODEM]=='N')      //检查是否有MODEM模块
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"本机无MODEM模块", "No Modem Module");
		getkey();
		return ;
	}

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"MODEM单项测试","MODEM Test",3);

}

// flag: 0-整机 1-单次 2-循环 3-成功率 4-音频 5-脉冲
uchar ModemTest(uchar flag)
{
	uchar ModemFunErr=0,ModemOtherErr=0;
	uint TotalNum,FailNum;
	ushort i,outlen,inlen;
	unsigned char mode,dialbuff[100],outbuff[1000],inbuff[1000];
    unsigned char ucRet,ucRet1,ucRet2,ucKey,Level;
	unsigned char *errmsg="";
 	unsigned char TempBuff[]="11";//拨空号
	uchar SetLevelFlag = 0;
	ScrClrLine(0,7);


	if(flag == 0)       ScrDispStr(0,2,1,"MODEM测试","MODEM Test");
	else if (flag == 1) ScrDispStr(0,0,1,"单次测试","SingleTest");
	else if (flag == 2) ScrDispStr(0,0,1,"循环测试","Cycle Test");
	else if (flag == 3) ScrDispStr(0,0,1,"脉冲测试","Pulse Test");
	RouteSetDefault(ROUTE_MODEM);//配置系统缺省路由Modem
	if(flag!=0)
	{
		ModemTelNum();
		if(flag==5)
		{
			Level=ModemSetLevel();
			ScrClrLine(2,7);
			if(Level==0)ScrPrint(0,4,1,"defautl level=11 (-db) ");
			else ScrPrint(0,4,1,"Level=%d (-db) ",Level);
			SetLevelFlag =1;
			flag = 1;
			getkey();
		}
	}
	ModemInit(); 
    mode=1; 
    outlen=20;
	memset(dialbuff,0x00,sizeof(dialbuff));
	memset(outbuff,0x00,sizeof(outbuff));
	memset(inbuff,0x00,sizeof(inbuff));
	ScrPrint(0,6,1,"%s",TelNum);
	outbuff[0]=0x60;
	for(i=1; i<5; i++) outbuff[i]=0;
	for(i=5; i<outlen; i++) outbuff[i]=rand();
	TotalNum=0; 
    FailNum=0;
	while(1)
	{
		ScrDispStr(0,2,1,"MODEM通讯测试","  MODEM  Test   ");	
		while(2)
		{
			OnHook();
			if(flag==2)
			{
				if(CancelCheck()) break;
				if(TotalNum==10000)break;
				if(TotalNum>0)DelayMs(4000);//循环测试时充分挂机	
				ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
			}

			inlen=0;
            ScrClrLine(4,7); 
            ScrDispStr(0,4,1,"开始拨号...","Dialing....");

			if (flag == 1) //单次测试需先拨空号再转换极性开关
			{
				ModemDial(&modem_para,TempBuff,0);
				TimerSet(0, 400);          
				do
				{
					if (!TimerCheck(0))	break;
					if(CancelCheck())goto ENDERR;
					ucRet = ModemCheck();
                    if(ucRet == 0x33)
                    {
                        ScrClrLine(4,7);
                        ScrDispStr(0,4,1,"未接电话线%02x","Phone line not connected!%02x",ucRet);
                        getkey();
                        break;
                    }
				}while(ucRet==0x0a);
				OnHook();
		
                //无应答、线路忙都表示modem拨号成功 djl。
                if (ucRet!=0x0a && ucRet!=0 && ucRet!=0x0d && ucRet!=0x05)
                {
					ScrDispStr(0,4,1,"拨空号错误:%02x","Dail Null Err:%02x",ucRet);
					goto ENDERR;
                }

				ScrClrLine(4,7);
				ScrDispStr(0,4,1,"请转换极性开关","Pls alter switch");
				ScrDispStr(0,6,1,"按任意键继续  ","Any key continue");
				kbflush();
				ucKey=getkey();
				if(ucKey==KEYCANCEL)
					goto ENDERR;
				ScrClrLine(4,7);
				ScrDispStr(0,4,1,"开始拨号...","Dialing....");
			}
			ScrPrint(0,6,1,"%s",TelNum);

			if(flag == 3)					//脉冲拨号
			{
				modem_para.CHDT=0x41;
				modem_para.DP=0x01; 
				modem_para.SSETUP=0x07; 
			}
			if(SetLevelFlag)SetLevel(Level);//设置电平
			ucRet=ModemDial(&modem_para,TelNum,mode);
			if(ucRet)
			{
				ScrClrLine(4,7);
				sprintf(gErrnoMsg,"ModemDial=0x%02x",ucRet);
				ScrDispStr(0,4,1,"拨号错误:%02x","Dail ERR:%02x",ucRet);
				OnHook(); 
				BeepFail(); 
				if(flag==0 || flag==1 || flag==3)//整机与单次以及音频，脉冲测试时返回
				{
					DelayMs(1000);
					goto ENDERR;
				}
				else if(flag==2)//成功率测试时，
				{
					if(ucRet==0xfd){TotalNum--; break;}
					else{FailNum++; continue;}
				}
			}
			if(!ucRet)
			{
				ScrClrLine(4,7);
				ScrDispStr(0,4,1,"正在发送数据... ","Sending Data...");
				ucRet1 = ModemTxd(outbuff,outlen);
				if(ucRet1)
				{
					sprintf(gErrnoMsg,"ModemTxd=0x%02x",ucRet1);
					BeepFail();
					FailNum++; 
					if(flag==0 || flag==1 || flag==3)
					{

						DelayMs(1000);
						goto ENDERR;
					}
					else continue;
				}
				else
				{
					ScrDispStr(0,4,1,"正在接收数据... ","Receiving Data.."); 
					do{
						if(CancelCheck())
						{
							OnHook(); 
							break;
						}
						ucRet2=ModemRxd(inbuff,&inlen);
					}while(ucRet2==0x0c);//接收数据被拒绝（缓冲区为空）

				}
				if(ucRet2)
				{
					sprintf(gErrnoMsg,"ModemRxd=0x%02x",ucRet2);
					BeepFail();
					FailNum++; 
					if(flag==0 || flag==1 || flag==3)
					{
						ScrDispStr(0,4,1,"接收数据错误:%d","Receiving Err:%d",ucRet2); 
						goto ENDERR;
					}
					else if(flag==2)continue;
				}
			}
			if(inlen==outlen && !memcmp(outbuff+5,inbuff+5,inlen-5))
			{
				ScrDispStr(0,4,1,"MODEM通讯OK!    "," MODEM COM OK!  ");								
				BeepOK(); 
				DelayMs(500);
				if(flag==1 || flag==0 || flag==3)
				goto ENDOK;
					else
				continue;
			}
			else
			{
				sprintf(gErrnoMsg,"Data Error");
				ScrDispStr(0,4,1,"接收内容出错","Receive Data ERR");
				FailNum++;
				BeepFail();
				if(flag==0 || flag==1 || flag==3)
				{
					DelayMs(500);
					goto ENDERR;
				}
			}								
		
		}// 2
		ShowSuccessRate(TotalNum,FailNum);
		kbflush();
		if(getkey()==KEYCANCEL) break;
	} //1

ENDOK:
	OnHook();
	return 0;
ENDERR:
	OnHook();
	return 1;
}


// flag: 0-并线 1-旁置
uchar ByPhoneTest(uchar flag)
{
	uchar ucRet;
	ScrClrLine(2,7);
	if(TestModule[T_BYPHONE]=='N')
	{
		ScrDispStr(0,4,1,"无旁置电话模块","No ByPhone ");
		if(flag!=0)	getkey();
		return 0xff;
	}
		
	ModemInit(); 
	ScrDispStr(0,2,1,"旁置电话测试","BY-PHONE Test");
	ScrDispStr(0,4,1,"拿起旁置电话后,按键开始拨号","PickUp BY-PHONE And Dial");								
	BeepOK();
	if(WaitEsc(0,20)) return 1;
	ScrClrLine(2,7);
	ScrDispStr(0,2,1,"开始拨号...","Dialing....");
	OnHook();
	ModemDial(&modem_para,"1234.",0);
	TimerSet(1,500);
	do
	{
		if(!TimerCheck(1)) break;
		ucRet=ModemCheck();
	}while(ucRet==0x0a);
	OnHook(); 
	if(ucRet!=0x03)
	{
		ScrClrLine(4,7);
		sprintf(gErrnoMsg,"ModemCheck=0x%02x",ucRet);
		ScrDispStr(0,4,1,"旁置电话错误:%02x","ByPhone Err:%02x",ucRet);							
		BeepFail();
		getkey();
		return 1;
	}
	BeepOK();
	ScrDispStr(0,4,1,"已OK,放回电话后,\n按确认键继续测试","OK,put it down\nENT to go on");
	getkey();
	return 0;
}
// flag: 0-测试  1--与拨号  2--预拨号看结果 
uchar auto_modem(uchar flag)
{
	
	ushort i,outlen,inlen;
	uchar outbuff[1000],inbuff[1000];
    uchar ucRet;
	ScrClrLine(0,7);

	//RouteSetDefault(10);//配置系统缺省路由Modem
	if(flag)
	{
		ModemInit(); 	
		OnHook();
		ucRet=ModemDial(&modem_para,TelNum,0);
		i = 0;
		while(flag==2)
		{
			ScrDispStr(0,2,1,"开始预拨号...","Pre Dialing....");
			ScrPrint(0,4,1,"%s",TelNum);
			ScrPrint(0,6,1,"Check=%02x,%d",ModemCheck(),i++);
			if(CancelCheck())break;
			DelayMs(1000);
		}
		return 0;
	}

	ScrDispStr(0,2,1,"MODEM测试","MODEM Test");
	i=0;
	while(1)
	{
		ucRet = ModemCheck();
		ScrPrint(0,4,1,"  Check=%02x,%d",ucRet,i);
		if(ucRet==0x0b)return ModemTest(0); //正常挂机和空闲 
		else if(ucRet!=0x0a || CancelCheck() || i++>30)break;
		DelayMs(1000);
	}

	if(ucRet)
	{
		sprintf(gErrnoMsg,"ModemCheck=0x%02x",ucRet);
		ScrDispStr(0,4,1,"Fail Check=%02x","Fail Check=%02x",ucRet);
		goto ENDERR;
	}
	outlen=20;
	memset(outbuff,0x00,sizeof(outbuff));
	memset(inbuff,0x00,sizeof(inbuff));
	outbuff[0]=0x60;
	for(i=1; i<5; i++) outbuff[i]=0;
	for(i=5; i<outlen; i++) outbuff[i]=rand();

	ScrClrLine(4,7);
	ScrDispStr(0,4,1,"正在发送数据... ","Sending Data...");
	ucRet = ModemTxd(outbuff,outlen);
	if(ucRet)
	{
		sprintf(gErrnoMsg,"ModemTxd=0x%02x",ucRet);
		ScrDispStr(0,4,1,"发送数据错误:%d","Sending Err:%d",ucRet); 
		goto ENDERR;
	}
	
	ScrDispStr(0,4,1,"正在接收数据... ","Receiving Data.."); 
	do{
		if(CancelCheck()) goto ENDERR;
		ucRet=ModemRxd(inbuff,&inlen);
	}while(ucRet==0x0c);//接收数据被拒绝（缓冲区为空）

	if(ucRet)
	{
		sprintf(gErrnoMsg,"ModemRxd=0x%02x",ucRet);
		ScrDispStr(0,4,1,"接收数据错误:%d","Receiving Err:%d",ucRet); 
		goto ENDERR;
	}
	
	if(inlen==outlen && !memcmp(outbuff+5,inbuff+5,inlen-5))
	{
		ScrDispStr(0,4,1,"MODEM通讯OK!    "," MODEM COM OK!  ");
		goto ENDOK;													
	}
	else
	{
		sprintf(gErrnoMsg,"Data Error");
		ScrDispStr(0,4,1,"数据出错","Data ERR");
		goto ENDERR;
	}
	
ENDOK:
	BeepOK();
	OnHook();
	return 0;
ENDERR:
	BeepFail();
	OnHook();
	return 1;
}
