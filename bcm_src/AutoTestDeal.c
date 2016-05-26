#include "public.h"

uchar Autotest_result[20];    //自动测试的自测项目结果保存
uchar Autotest;				  //1表示已经测试，0表示未进行测试
uchar gSamTest;

extern uchar auto_kb(uchar flag);
extern uchar NewRfTest(uchar flag);


#define T_LCD1			60
#define T_LED1			61//add lifw 2014-05-13
#define T_LED2			62
#define	T_LCD_R			63
#define	T_LCD_G			64
#define	T_LCD_B			65
#define	T_LCD_BLACK		66
#define	T_LCD_W			67
typedef struct 
{
	uchar model;
	ALL_FUN_P TestFun;
}AutoTestItem;

uchar gTime[32];

//==========================================================================
uchar LcdTestR(uchar flag)
{
	uint TotalNum=0;
	uchar ucRet,i;
	ScrCls();
	
//	while(1)
	{
		
/*		if(flag==0)	ScrDispStr(0,2,1,"LCD测试",	"LCD Test");
		if(flag==1)	ScrDispStr(0,0,1,"单次测试",	"Single Test");
		if(flag==2) 
		{
			if(CancelCheck()) return 1;
			ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
		}
*/
		kbflush();
		
		//彩屏
		if(gIsColorLcd==1 || TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900 || InfoOut[C_LCD][0]!=0)
		{
			CLcdGetInfo(&stLcdInfo);
			ScrBackLight(2);
			for(i=0; i<=8; i++)
			{
				if(i==2) ScrSetIcon(i,6);
				else ScrSetIcon(i,1);
			}
			ScrCls();
			CLcdBgDrawBox(0,0,stLcdInfo.width-1,stLcdInfo.height-1,C_RED);
			DelayMs(100);
		}

		if(flag==3)return 0;//煲机
/*		if(flag <= 1)
		{
			ucRet=JudgeState("屏幕","LCD");
			if(ucRet== KEYENTER)
			{
				RECORD(1, "LCD=1\r\n");
				return 0;
			}
			else
			{
				RECORD(1, "LCD=0\r\n");
				return 1;
			}

		}*/
	}
	return 1;
	
}
uchar LcdTestG(uchar flag)
{
	uint TotalNum=0;
	uchar ucRet,i;
	ScrCls();
	
//	while(1)
	{
		
/*		if(flag==0)	ScrDispStr(0,2,1,"LCD测试",	"LCD Test");
		if(flag==1)	ScrDispStr(0,0,1,"单次测试",	"Single Test");
		if(flag==2) 
		{
			if(CancelCheck()) return 1;
			ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
		}
*/
		kbflush();
		
		//彩屏
		if(gIsColorLcd==1 || TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900 || InfoOut[C_LCD][0]!=0)
		{
			CLcdGetInfo(&stLcdInfo);
			ScrBackLight(2);
			for(i=0; i<=8; i++)
			{
				if(i==2) ScrSetIcon(i,6);
				else ScrSetIcon(i,1);
			}
			ScrCls();
			CLcdBgDrawBox(0,0,stLcdInfo.width-1,stLcdInfo.height-1,C_GREEN);
			DelayMs(100);
		
		}

		if(flag==3)return 0;//煲机
/*		if(flag <= 1)
		{
			ucRet=JudgeState("屏幕","LCD");
			if(ucRet== KEYENTER)
			{
				RECORD(1, "LCD=1\r\n");
				return 0;
			}
			else
			{
				RECORD(1, "LCD=0\r\n");
				return 1;
			}

		}*/
	}
	return 1;
	
}
uchar LcdTestB(uchar flag)
{
	uint TotalNum=0;
	uchar ucRet,i;
	ScrCls();
	
//	while(1)
	{
		
/*		if(flag==0)	ScrDispStr(0,2,1,"LCD测试",	"LCD Test");
		if(flag==1)	ScrDispStr(0,0,1,"单次测试",	"Single Test");
		if(flag==2) 
		{
			if(CancelCheck()) return 1;
			ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
		}
*/
		kbflush();
		
		//彩屏
		if(gIsColorLcd==1 || TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900 || InfoOut[C_LCD][0]!=0)
		{
			CLcdGetInfo(&stLcdInfo);
			ScrBackLight(2);
			for(i=0; i<=8; i++)
			{
				if(i==2) ScrSetIcon(i,6);
				else ScrSetIcon(i,1);
			}
			ScrCls();
			CLcdBgDrawBox(0,0,stLcdInfo.width-1,stLcdInfo.height-1,C_BLUE);
			DelayMs(100);
		}

		if(flag==3)return 0;//煲机
/*		if(flag <= 1)
		{
			ucRet=JudgeState("屏幕","LCD");
			if(ucRet== KEYENTER)
			{
				RECORD(1, "LCD=1\r\n");
				return 0;
			}
			else
			{
				RECORD(1, "LCD=0\r\n");
				return 1;
			}

		}*/
	}
	return 1;
	
}
uchar LcdTestBlack(uchar flag)
{
	uint TotalNum=0;
	uchar ucRet,i;
	ScrCls();
	
//	while(1)
	{
		
/*		if(flag==0)	ScrDispStr(0,2,1,"LCD测试",	"LCD Test");
		if(flag==1)	ScrDispStr(0,0,1,"单次测试",	"Single Test");
		if(flag==2) 
		{
			if(CancelCheck()) return 1;
			ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
		}
*/
		kbflush();
		
		//彩屏
		if(gIsColorLcd==1 || TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900 || InfoOut[C_LCD][0]!=0)
		{
			CLcdGetInfo(&stLcdInfo);
/*			ScrBackLight(0);
			for(i=0; i<=8; i++)
			{
				if(i==2) ScrSetIcon(i,6);
				else ScrSetIcon(i,1);
			}
*/
			ScrCls();

			ScrBackLight(2);
			for(i=0; i<=8; i++)
			{
				ScrSetIcon(i,0);
			}
			DelayMs(100);
			CLcdBgDrawBox(0,0,stLcdInfo.width-1,stLcdInfo.height-1,C_BLACK);
			DelayMs(100);

		}

		if(flag==3)return 0;//煲机
/*		if(flag <= 1)
		{
			ucRet=JudgeState("屏幕","LCD");
			if(ucRet== KEYENTER)
			{
				RECORD(1, "LCD=1\r\n");
				return 0;
			}
			else
			{
				RECORD(1, "LCD=0\r\n");
				return 1;
			}

		}*/
	}
	return 1;
	
}
uchar LcdTestW(uchar flag)
{
	uint TotalNum=0;
	uchar ucRet,i;
	ScrCls();
	
//	while(1)
	{
		
/*		if(flag==0)	ScrDispStr(0,2,1,"LCD测试",	"LCD Test");
		if(flag==1)	ScrDispStr(0,0,1,"单次测试",	"Single Test");
		if(flag==2) 
		{
			if(CancelCheck()) return 1;
			ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
		}
*/
		kbflush();
		
		//彩屏
		if(gIsColorLcd==1 || TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900 || InfoOut[C_LCD][0]!=0)
		{
			CLcdGetInfo(&stLcdInfo);
			ScrBackLight(0);
			for(i=0; i<=8; i++)
			{
				if(i==2) ScrSetIcon(i,6);
				else ScrSetIcon(i,1);
			}
			ScrCls();
			ScrBackLight(2);
			for(i=0; i<=8; i++)
			{
				ScrSetIcon(i,0);
			}
			DelayMs(100);
			CLcdBgDrawBox(0,0,stLcdInfo.width-1,stLcdInfo.height-1,C_WHITE);
			DelayMs(100);
		}

		if(flag==3)return 0;//煲机
/*		if(flag <= 1)
		{
			ucRet=JudgeState("屏幕","LCD");
			if(ucRet== KEYENTER)
			{
				RECORD(1, "LCD=1\r\n");
				return 0;
			}
			else
			{
				RECORD(1, "LCD=0\r\n");
				return 1;
			}

		}*/
	}
	return 1;
	
}
//===========================================================================
/****************************************************************************
  函数名     :  hshake(uchar ComNo)
  描述       :  指定串口与测试架控制板握手程序
  输入参数   :  ComNo-握手用串口
  输出参数   :	
  返回值     :  0-成功
****************************************************************************/
uchar hshake(uchar ComNo,uchar type)
{
	uchar ret,ucRet,loop;
	int nFlag=0;
	loop=0;
	ScrDispStr(0,2,0,"握手中...","HandShake...");
	
	while(1)
	{
		loop++;	
		if(CancelCheck()) return 0xff;
		
		PortSend(ComNo,type);
        DelayMs(1);
		ret=PortRecv(ComNo,&ucRet, 5);
		if(ret)
		{
			DelayMs(100);
			continue;
		}
		if(ucRet==0xfe || ucRet==0xfd)
		{	
			PortSend(ComNo,'k');
			if(ucRet==0xfd)gSamTest = 1;
			else gSamTest = 0;
			return 0;
		}
	}
}


/****************************************************************************
  函数名     :  SendRSP(uchar ComNo,uchar model,uchar flag, uchar ret_code)
  描述       :  发送反应数据
  输入参数   :  ComNo-握手用串口，model-测试模块，flag-测试标志，ret_code-返回值
  输出参数   :	
  返回值     :  0-成功  不为0-失败
****************************************************************************/
uint SendRSP(uchar ComNo,uchar model,uchar flag, uchar ret_code)
{
	uchar sbuff[10];
	uchar ucRet;
	
	ComNo = gPPQcom;

	sbuff[0]='R';
	sbuff[1]=model;
	sbuff[2]=flag;
	sbuff[3]=ret_code;
	
	
	ucRet=PortSends(ComNo,sbuff,4);

	if(ucRet)
	{
		PortClose(ComNo);
		PortOpen(ComNo,"115200,8,n,1");
		return ucRet;
	}
	
	//PortClose(ComNo);
	//PortOpen(ComNo,"115200,8,n,1");
	return 0;
}


/****************************************************************************
  函数名     :  RecvCMD(uchar ComNo, uchar *str)
  描述       :  接收指令
  输入参数   :  ComNo-握手用串口，*str-指令
  输出参数   :	
  返回值     :  0-成功  不为0-失败
****************************************************************************/
uint RecvCMD(uchar ComNo, uchar *str)
{
	
	uchar ret,offset;
	uint i;
	
	offset=0;

	str[0] = 0;

	while(1)
	{
		if(CancelCheck())return 0xff;//user cancel
		ret = PortRecv(ComNo,&str[offset],0);
		if(ret==0 && str[offset]=='C')				// 接收到字符
			break;	
		if(ret==0x02 || ret==0x03 || ret== 0xf0)
			return 0xf0;		// 串口有问题。
		if(ret==0 && str[offset]!='C')
        {
			continue;
			ScrPrint(0,4,1,"CMD:%x\n",str[offset]);
			return 2;
		}
		
	}
	offset++;

	for(i=0;i<3;i++)
	{
		ret = PortRecv(ComNo,&str[offset],30);		// 30ms的等待
		if(ret)
			return 1;
		offset++;
	}
	ScrPrint(0,6,0,"RECV CMD %c,%c,%c,%x ",str[0],str[1],str[2],str[3]);
	SendRSP(ComNo,str[3],'O',0x00);
	return 0;
}




uchar upRecord(void)
{
	uchar buf[8192],temp[512];
	long FileSize;
	int sended,txSize,fd;
	uchar ucRet;
	ucRet = PortOpen(gPPQcom, "115200,8,n,1");
    if(ucRet)
    {
		ScrCls();
		ScrPrint(0,4,1,"PortOpen(%d)=%d",gPPQcom,ucRet);
        getkey();
		return 1;
    }
	
	fd = open(F_BASIC_INFO,O_RDWR);
	FileSize = filesize(F_BASIC_INFO);
    if (fd < 0)  //打开文件出错
    {
        
		ShowFileCtrlErr(__LINE__);
        PortClose(gPPQcom);
        return 1; 
    }
    sended =0;
    memset(buf,0x00,sizeof(buf));
    PortReset(gPPQcom);
    while(sended<FileSize)
    {
        txSize = read(fd,buf,sizeof(buf));
        if (txSize > 0)
        {
            ushort txIdx, txLen;
            sended += txSize;
            txIdx = 0;
			
            while(txIdx<txSize)
            {
                txLen = txSize - txIdx;
                if (txLen > 8192) txLen = 8192;
                ucRet = PortSends(gPPQcom,&((char *)buf)[txIdx],(ushort)txLen);
                if (ucRet == 0x00)
                {
                    txIdx += txLen;
                }
                while(PortTxPoolCheck(gPPQcom));
            }
			
        }
        else
        {
            ShowFileCtrlErr(__LINE__);
            break;
        }
    }
	return 0;
	
}


uchar InfoTest(uchar flag)
{
	uint LoopAllTime,LoopAllCnt;
	uchar ucRet;
	DeleteLog(F_BASIC_INFO);
	CalcLoopTime(&LoopAllTime,&LoopAllCnt);
	ShowVer();
	RECORD(1,"RECORD OK\n"); //数据的结尾
	ucRet=upRecord();
	return ucRet;
}
uchar Auto_InfoTest(uchar flag)
{
	uint LoopAllTime,LoopAllCnt;
	uchar ucRet;
	int	i;
	DeleteLog(F_BASIC_INFO);
	RECORD(0,".FTEST=%s",SOFT_VERSION); //记录FTest版本
    RECORD(1, "INTEGRATION TEST BEGIN");
    RECORD(1, "TestStart=%s",DispTimer(7));
    RECORD(1, "\n");
	CalcLoopTime(&LoopAllTime,&LoopAllCnt);
	ShowVer();
	RECORD(1,"RECORD OK\n"); //数据的结尾
	ucRet=upRecord();
	for(i=0;i<50;i++)
	{
		//PortSend(ComNo,'k');//TestModule[i]
		PortSend(gPPQcom,TestModule[i]);
		//	PortSend(RS232A_PORT,0x01);
	}
	//ucRet=upRecord();
	return ucRet;
}

uchar Auto_BeepTest(uchar flag)
{
	uchar i,ucRet;
	ushort TotalNum=0;
	ScrCls();
	if(flag==0)
	{
		Beef(4,100);
		DelayMs(120);
		Beef(5,100);
		return 0;
	}
	
	
}


typedef struct
{
	uchar KeyValue;
	uchar Flag_OK;
	uchar KeyDIS[10];
}KEY_ORDER;

uchar Auto_KbTest(uchar flag)
{
	return 0;
}

// flag: 0-整机 1-单次 2-循环
uchar Auto_LcdTest(uchar flag)
{
	uint TotalNum=0;
	uchar ucRet,i;
	ScrCls();
	
	while(1)
	{
		if(CancelCheck()) return 1;
		if(flag==0)	ScrDispStr(0,2,1,"LCD测试",	"LCD Test");
		if(flag==1)	ScrDispStr(0,0,1,"单次测试",	"Single Test");
		if(flag==2) ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
		kbflush();
		PiccLight(1, 0);//lifw turn off all led
		PiccLight(4, 0);
		PiccLight(2, 0);
		PiccLight(8, 0);
		//彩屏
		//if(TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900)
		if(0)
		{

		}
		else//非彩屏
		{
			//关背光,开图标,黑屏
			for(i=1; i<=8; i++)
			{
				if(i==7) ScrSetIcon(i,1);
				else ScrSetIcon(i,1);
				ScrPrint(0,i,0x81,"                ");
			}


			ScrBackLight(2);

            PiccLight(1, 1);
			PiccLight(4, 1);
			PiccLight(2, 0);
			PiccLight(8, 0);			
			

/*			DelayMs(600);
            PiccLight(2, 1);
			PiccLight(8, 1);	
			PiccLight(1, 0);
			PiccLight(4, 0);
			
			

			//开背光,关图标,白屏
			for(i=1; i<=8; i++)
			{
				ScrSetIcon(i,0);
				ScrPrint(0,i,0x01,"                ");
			}
			ScrBackLight(2);


			DelayMs(700);
			//光背光 无显示 
            PiccLight(2, 0);
			PiccLight(8, 0);	
			PiccLight(1, 0);
			PiccLight(4, 0);
			ScrBackLight(0);
			DelayMs(600);
*/
			
		}

		if(flag==3)return 0;//煲机
		return 0;

	}
	return 1;
	
}
//======================================================================
uchar Auto_LcdTest1(uchar flag)
{
	uint TotalNum=0;
	uchar ucRet,i;
	ScrCls();
	
//	while(1)
	{
		if(CancelCheck()) return 1;
		if(flag==0)	ScrDispStr(0,2,1,"LCD测试",	"LCD Test");
		if(flag==1)	ScrDispStr(0,0,1,"单次测试",	"Single Test");
		if(flag==2) ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
		kbflush();
		
		//彩屏
		//if(TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900)
		if(0)
		{

		}
		else//非彩屏
		{			

			//开背光,关图标,白屏
			for(i=1; i<=8; i++)
			{
				ScrSetIcon(i,0);
				ScrPrint(0,i,0x01,"                ");
			}
			ScrBackLight(2);
			PiccLight(1, 0);
			PiccLight(4, 0);
			PiccLight(2, 1);
			PiccLight(8, 1);
			DelayMs(1000);
			//光背光 无显示 
			ScrBackLight(0);
			PiccLight(2, 0);
			PiccLight(8, 0);
			DelayMs(100);
		}

		if(flag==3)return 0;//煲机
		return 0;

	}
	return 1;
	
}
uchar Auto_LedTest1(uchar flag)
{
	uint TotalNum=0;
	uchar ucRet,i;
	ScrCls();
	
//	while(1)
	{
		if(CancelCheck()) return 1;
		if(flag==0)	ScrDispStr(0,2,1,"LED测试",	"LED Test");
		if(flag==1)	ScrDispStr(0,0,1,"单次测试",	"Single Test");
		if(flag==2) ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
		kbflush();
		
		//彩屏
		//if(TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900)
		if(0)
		{

		}
		else//非彩屏
		{
			ScrBackLight(0);
			PiccLight(2, 0);
			PiccLight(8, 0);	
			PiccLight(1, 1);
			PiccLight(4, 1);
			DelayMs(100);
		}

		if(flag==3)return 0;//煲机
		return 0;

	}
	return 1;
	
}
uchar Auto_LedTest2(uchar flag)
{
	uint TotalNum=0;
	uchar ucRet,i;
	ScrCls();
	
//	while(1)
	{
		if(CancelCheck()) return 1;
		if(flag==0)	ScrDispStr(0,2,1,"LED测试",	"LED Test");
		if(flag==1)	ScrDispStr(0,0,1,"单次测试",	"Single Test");
		if(flag==2) ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
		kbflush();
		
		//彩屏
		//if(TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900)
		if(0)
		{

		}
		else//非彩屏
		{
			ScrBackLight(0);
            PiccLight(2, 1);
			PiccLight(8, 1);	
			PiccLight(1, 0);
			PiccLight(4, 0);
			DelayMs(100);
	
		}

		if(flag==3)return 0;//煲机
		return 0;

	}
	return 1;
	
}
//===============================================================
// flag: 0-整机 1-单项 2-循环
uchar Auto_MagTest(uchar flag)
{
	int	nTime=0;
	int nTime1=0;
	uint SuccessCnt,FailCnt,TotalCnt;
	uchar ucRet,MagFunErr,MagOtherErr;
	char Track1[256],Track2[256],Track3[256];
	
	memset(Track1, 0x00, sizeof(Track1));
    memset(Track2, 0x00, sizeof(Track2));
	memset(Track3, 0x00, sizeof(Track3));
	ScrCls();
	if(flag==0)ScrDispStr(0,2,1,"磁卡测试","MagCard Test");
	if(flag==1)ScrDispStr(0,0,1,"单次测试","Single Test");
	if(flag==2)ScrDispStr(0,0,1,"循环测试","Cycle Test ");
    MagOpen();	
	SuccessCnt=FailCnt=TotalCnt=0;
//	while(1)
	{
		nTime=GetTimerCount();
		while(GetTimerCount()-nTime<15*1000)
		{	
			//nTime=GetTimerCount();
			MagFunErr=MagOtherErr=0;
			if(CancelCheck())goto ENDERR;
			if(flag==2)ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalCnt);
			ScrDispStr(0,4,1,"请刷磁卡<<<"," SWIPE CARD<<<");
			nTime1=GetTimerCount();
			while(GetTimerCount()-nTime1<1.2*1000)
		//	while(3)
			{
		//		if(!MagSwiped()) 
				if(CancelCheck())//取消按键检查
				{
					if(flag==2)goto ENDTEST;
					else goto ENDERR;
				}
		//	}//while 3
				if(!MagSwiped()) 
				{
					ucRet=MagRead((unsigned char*)Track1,(unsigned char*)Track2,(unsigned char*)Track3);
					if( ShowMagErr(6,ucRet,Track1,Track2,Track3)==0 )//读磁卡正确
					{
						//BeepOK();	
						SuccessCnt++;			
					}
					else
					{
						SuccessCnt = 0;//只要有一次失败就清0
						//BeepFail();
						FailCnt++;
						MagFunErr=ucRet;
						MagOtherErr=1;
						MagReset();
						DelayMs(10);

						if(FailCnt>=2 && flag==0) goto ENDERR;
					}
					if(flag<=1)
					{
						ScrDispStr(0,6,1,"成功次数:%d/3","Succeed:%d/3",SuccessCnt);	
						MagReset();
						if(SuccessCnt==2)
						{
							MagClose();
							ScrDispStr(0,6,1,"磁卡测试通过","Magcard Test OK!");			
							DelayMs(100);//500
							goto ENDOK;
						}
						continue;
					}
					break;
				}//if
			}//	while(3)
		}//while(2)
	}//while(1)
	ScrDispStr(0,6,1,"磁卡测试超时","Magcard Test Timeout!");
	//return 1;
	goto ENDERR;
ENDTEST:	
//		ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalCnt);
//		ShowSuccessRate(TotalCnt,FailCnt);
//		kbflush();
//		if(getkey()==KEYCANCEL) break;
//	}//while 1 

ENDOK:

	MagClose();
	return 0;
ENDERR:

	MagClose();
	return 1;
}



uchar Auto_ClkTest(uchar flag)
{
	uchar i,ucRet,ucBuf[20],old_time[7];
	int ClkFunErr=0,ClkOtherErr=0;//函数返回错误,其他错误情况

	if(Autotest == 1)
	{
		//DelayMs(100);
		//ScrClrLine(2,3);
        return Autotest_result[2];
	}

	ScrCls();
	ScrDispStr(0,2,1,"时钟测试...","Clock Test...");

	GetTime(old_time); 
	
	if(old_time[0] < 0x15 || old_time[0]>0x20){//时间不是在15年～20年之间报错
        ClkOtherErr=3;
		ScrDispStr(0,4,1,"时间是 %02x 年!!","It's %02x Year!!",old_time[0]);
		BeepFail(); 
		DelayMs(1000);
		goto ENDERR;
	}

	
	if(flag==0)
	{
		
		uchar tmp1,tmp2,tmp3,tmp4;
		
		tmp1 = old_time[5]%0x10 + old_time[5]/0x10*10;
		tmp2 = gTime[5]%0x10 + gTime[5]/0x10*10;
		
		if(tmp1<tmp2)
			tmp3 = tmp1+60-tmp2;
		else
			tmp3 = tmp1-tmp2;
		tmp4=(250-TimerCheck(2))/10;
		if(tmp3>=tmp4-1 && tmp3<=tmp4+1) 
		{
			ScrDispStr(0,6,1,"时钟测试OK","Clock Test OK!");
			BeepOK();
			DelayMs(200);
			goto ENDOK;
		}
		else 
		{
			ScrDispStr(0,6,1,"时钟测试失败","Clock Test Fail!");
			BeepFail();
			DelayMs(200);
			goto ENDERR;
		}	
	}
	  
	ucRet=SetTime((unsigned char *)"\x99\x12\x31\x23\x59\x58");//此时关机，时钟会置为2000年.
	if(ucRet){
		ClkFunErr=ucRet;
		ScrDispStr(0,4,1,"时钟设置错误:%d","SetTime Err:%d",ucRet);
		BeepFail(); 
		DelayMs(1000); 
		goto ENDERR;
	}
	memset(ucBuf,0,sizeof(ucBuf));
	GetTime(ucBuf);
	if(memcmp(ucBuf,"\x99\x12\x31\x23\x59\x58",5)){
		ClkOtherErr=1;
		ScrDispStr(0,4,1,"时钟对比错误","Clock CMP ERR");	
		BeepFail(); 
		DelayMs(1000);
		goto ENDERR;
	}
	SetTime((unsigned char *)"\x99\x12\x31\x23\x59\x59");
	for(i=0;i<100;i++){
		memset(ucBuf,0,sizeof(ucBuf));
		GetTime(ucBuf);
		if(!memcmp(ucBuf,"\x00\x01\x01\x00\x00\x00\x06",7)) break;
		DelayMs(100);
	}
	if(i==100){
		ClkOtherErr=2;
		ScrDispStr(0,4,1,"时钟测试错误3","Clock Test ERR3");
		BeepFail(); 
		DelayMs(1000);
		goto ENDERR;
	}else{
		if(old_time[5]>0x56)old_time[5]=0x59;
		else if((old_time[5]&0x0f) < 8)old_time[5]=old_time[5]+2;
		else old_time[5]=old_time[5]+16-10+2;//if((old_time[5]&0x0f) >= 8)old_time[5]=old_time[5]+8;//
		SetTime(old_time);
		BeepOK();
		ScrDispStr(0,4,1,"时钟测试OK","Clock Test OK!");
		DelayMs(500);
		goto ENDOK;
	}
	
ENDOK:

	if(Autotest == 0)
		Autotest_result[2] = 0;
	ScrClrLine(2,3);
	return 0;
ENDERR:

	if(Autotest == 0)
		Autotest_result[2] = 1;
	ScrClrLine(2,3);
	return 1;
}


uchar Auto_PrinterTest (uchar flag)
{
	int i,count,BlockNum=0,Temperature=25;
	unsigned char ucRet,buff[10];
	uchar prntStr[10], prntStrCmp[10];
	int PrnFunErr,PrnOtherErr;
	count=0; 


	ScrCls();
	if(flag!=0)gTemperature=1;
	if(TermInfo[1]=='T')
	{
		Temperature=PrnTemperature();
		if(gTemperature==0 && VerInfo[1]==3)
		{
			RECORD(1,"..Has ComPare Temperature");
			if(Temperature>35 || Temperature<15)
			{
				BeepFail();
				ScrClrLine(2,5);
				ScrPrint(0,2,1, "Temperature=%d         ",Temperature);
				ScrDispStr(0,4, 0x01, "打印机类型错误", "Printer Error");
				DelayMs(1000);
				if(flag==0)
				{
					getkey();
					return 1;
				}
				
			}

		}

	}


	memset(prntStr, 0x00, sizeof(prntStr));
	for(i=0; i<3; i++)
	{
		prntStr[i]=(rand()%10)+'0';
	}
	if(flag!=0)
	{
		GetInfoExt();
		GetWirelessVer();
	}
	while(1)
	{
		PrnFunErr=PrnOtherErr=0;
		if(CancelCheck()) return 1;
		ScrClrLine(2,7);
		if (flag == 0) ScrDispStr(0,2,1,"整机打印测试","Inter PRN Test");
		else if (flag == 1) ScrDispStr(0,0,1,"单页打印测试","PRN Single-Page");
		else if (flag == 2) ScrDispStr(0,0,0x81,"多页打印  %6d","MultiPage %6d",++count);

		PrnInit();

		//字体格式设置
		if(TermInfo[1] == 'S'){
			PrnFontSet(0,0);
			PrnSpaceSet(0,0);
		}
		else{
			PrnFontSet(1,3);
			PrnSpaceSet(1,8);
		}
		PrnStep(20);

		if(FontType == CHINESE)
		{
			if (flag==1) PrnStr("单页打印测试\n");
			else if(flag==2) PrnStr("多页打印,%d页\n",count);
		}
		else
		{
			if (flag==1) PrnStr("SINGLE PAGE TEST\n");
			else if(flag==2) PrnStr("MULTI-PAGE,PAGE%d\n",count);
		}
		


	

		if(flag==0 || flag==1)
		{
			GetBlackBlock(5);//获取黑块
//			PrnStr("\n%s",prntStr);
		}		
		
		PrnStr("                             \n");
		PrnStr("A B C D E 1 2 3 4 5 6 7 8 9 0\n");
		PrnStr(" F G H I J K L M N P R S T X \n");
		PrnStr("                             \n");
		PrnStr("                             \n");
	
		if( TermInfo[1]=='S' ){
			PrnStr("\f");
		}
		else{
			PrnStep(50);
		}
		
		ucRet=PrnStart();
		if(ucRet==0)
		{

		}
		else
		{
			ShowPrinterErr(4,ucRet);
			PrnOtherErr=2;
			PrnFunErr=ucRet;
		}
		if(PrnFunErr==0 && PrnOtherErr==0){
			BeepOK();
			if(flag!=2) return 0;
		}
		else{
			BeepFail(); 
			return 1;
		}
	}// 1


	
}


uchar Auto_RfTest(unsigned char flag)
{
    uchar ucRet,ucMode=0;
    uchar BlkNo,SerialNo[11],PwdA[6],tmps[16];
    uint  TotalNum, FailNum;
    uchar CardType[10];
    uchar single=0;
    uchar resultStr[6][20];
    uchar changeFlag=0;
    uchar DataIn[101];
	uchar RfFunErr=0,RfOtherErr=0;

    APDU_SEND ApduSend;
    APDU_RESP ApduResp;
    int ii;
    int PlaceFlag,PlaceNum;
    int detectcnt,detectNum;
    int singleNum;
    PICC_PARA tpicc_para;

	uchar ErrorFlag=0;//0表示没有错误，1表示有错误
	uchar TestFlag=0;//0 表示还没测试，1表示测试过一次


    memset(&tpicc_para,0,sizeof(PICC_PARA));
    memset(resultStr, 0x00, sizeof(resultStr));
 
	ScrCls();
	ucRet=PiccOpen();
    if(ucRet) 
    {
		ScrDispStr(0,4,1, "射频打开失败", "PiccOpenErr",ucRet);
        getkey();
        return 1;
    }
    else if(flag==3)//读取版本信息
    {
		ScrClrLine(2,7);
		ScrDispStr(0,2,1,"射频卡版本信息","Reading RF Info..."); 
		memset(&tpicc_para,0x00,sizeof(PICC_PARA));    
		PiccSetup('r', &tpicc_para);
		ScrPrint(0,4,0,"RF:[Y] %02x%02x%02x%02x%02x",tpicc_para.a_conduct_val, tpicc_para.m_conduct_val, \
						tpicc_para.b_modulate_val, tpicc_para.card_RxThreshold_val, tpicc_para.f_modulate_val);
		ScrPrint(0,6,0,"Version:%5.5s\nDate:%s",tpicc_para.drv_ver,tpicc_para.drv_date);
		getkey();
		return 0;
    }


    TotalNum=0; 
    FailNum=0;
	if (flag == 0) ScrDispStr(0,2,1,"射频卡测试","RF Test");
    if (flag == 1) ScrDispStr(0,0,1,"单次测试","Single Test");
    if (flag == 2) ScrDispStr(0,0,1,"循环测试","Cycle Test ");
 
	if(flag<=1)
    {
        singleNum=3;  //三种寻卡模式
        PlaceNum = 2; //两个寻卡位置
        detectNum = 3;//每张卡的寻卡次数
    }
    else
    {
        singleNum=1;   
        PlaceNum = 1;
        detectNum = 1;
    }
	
    while(1)
    {
		while(2)
        {
            if(CancelCheck()) break;          
			ScrClrLine(2,7);
            memset(CardType, 0x00, sizeof(CardType));
            if(flag==2)
			{	
				if(TotalNum==10000) break;
				ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
			}
            if(flag==0 || flag==1)
            {
	
                for(single = 0; single < singleNum; single++)
                {
                    switch(single)
                    {
						case 2: ucMode='M'; break;
						case 1: ucMode='B'; break;
						case 0: ucMode='A'; break;
                    }
                    for(PlaceFlag = 0; PlaceFlag < PlaceNum; PlaceFlag++)
                    {
                        switch(PlaceFlag)
                        {
							case 0: sprintf(resultStr[single*2+PlaceFlag], "RF.%c.Min=000",ucMode); break;
							case 1: sprintf(resultStr[single*2+PlaceFlag], "RF.%c.Max=000",ucMode); break;
                        }
                    }
                }
            }

START:
			DelayMs(100);//DelayMs(1300);//2014-06-04
            //for(PlaceFlag = 0; PlaceFlag < PlaceNum; PlaceFlag++)
			for(single = 0; single < singleNum; single++)
            {
				if(single == 0)    DelayMs(100);//1000
				if(single == 2)    DelayMs(500);//800
                for(PlaceFlag = 0; PlaceFlag < PlaceNum; PlaceFlag++)
                {	if(0!=PlaceFlag)DelayMs(1300);//2014-06-04
                    for(detectcnt = 0; detectcnt < detectNum; detectcnt++)
                    {
                        if( resultStr[single*2+PlaceFlag][detectcnt+9]=='1' )continue;
						PiccRemove('H', 0);
						ScrClrLine(2,7);
                        ScrDispStr(0,2,1,"搜卡中........","Searching Card..");

                        if(flag <= 1)
                        {
                            switch(single)
                            {
								case 2: ucMode='M';PiccLight(PICC_LED_ALL, 0);PiccLight(PICC_LED_GREEN, 1);break;
								case 1: ucMode='B';PiccLight(PICC_LED_ALL, 0);PiccLight(PICC_LED_YELLOW, 1);break;
								case 0: ucMode='A';PiccLight(PICC_LED_ALL, 0);PiccLight(PICC_LED_BLUE, 1);break;
                            }
            
    //                        if(PlaceFlag==0)
    //                            ScrDispStr(0,4,1,"请将%c卡放入最小距离 0CM 处","pls wave %c card in 0CM", ucMode);
	//						else                                  
	//							ScrDispStr(0,4,1,"请将%c卡放入最大距离%dCM","pls wave %c card in %dCM", ucMode,gRfHeight);
                           
                            TimerSet(0, 60);//200
                            while(TimerCheck(0))
                            {             
								if(CancelCheck()) goto ENDERR;
                                ucRet=PiccDetect(ucMode, CardType, SerialNo, NULL, NULL);
                                if(!ucRet) break;
                            }
                        }
                        else
                        {
                            ScrDispStr(0,4,1,"请将RF卡放入感应区","Pls Wave RF Card ");
							TimerSet(0, 60);//200
                            while(TimerCheck(0))
                            {
                                if(CancelCheck())
                                {
                                    BeepFail();
                                    FailNum++;
                                    goto LoopResult;
                                }
                                ucRet=PiccDetect('M', CardType, SerialNo, NULL, NULL);
								if(!ucRet) break;
								ucRet=PiccDetect(0, CardType, SerialNo, NULL, NULL);
								if(!ucRet) break;
                            }
                        }
						
                        ScrClrLine(2,7);
                        if(ucRet)
                        {
                            RfFunErr=ucRet;
							RfOtherErr=1;
							ScrDispStr(0,4,1,"搜卡出错!","Detect Card Err!");
                            BeepFail();
                            FailNum++;
                            if(flag<=1)
                            {
                                goto ENDERR;
                            }
                            kbflush();
                            if(getkey()==KEYCANCEL)break;
                            else continue;

                        }
                        ScrDispStr(0,4,1,"搜 %s 卡成功","Detect %s card succeed",CardType);


                        if((CardType[0] == 'm')||(CardType[0] == 'M'))
                        {
                            ScrClrLine(2,7);
                            ScrDispStr(0,4,1,"密码认证...","Autho Password ");
                            BlkNo=37;
                            memcpy(PwdA, "\xff\xff\xff\xff\xff\xff", 6);
                            ucRet = M1Authority('A', BlkNo, PwdA, SerialNo+1);
                            if(ucRet)
                            {
                                RfFunErr=ucRet;
								RfOtherErr=2;
								ScrClrLine(2,7);
								ScrDispStr(0,4,1, "密码认证失败%d","Authority fail%d",ucRet);
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break;  
                            }
                
                            ScrClrLine(2,7);
                            ScrDispStr(0,4,1,"M1 卡写块...","Writing Block...");
                            memcpy(tmps,"\x01\x00\x00\x00", 4);
                            tmps[4]=~tmps[0];
                            tmps[5]=~tmps[1];
                            tmps[6]=~tmps[2];
                            tmps[7]=~tmps[3];
                            memcpy(tmps+8, tmps, 4);
                            tmps[12]=tmps[14]=BlkNo;
                            tmps[13]=tmps[15]=~BlkNo;
                            ucRet = M1WriteBlock(BlkNo,tmps);
                            if(ucRet)
                            {
                                RfFunErr=ucRet;
								RfOtherErr=3;
								ScrClrLine(2,7);
								ScrDispStr(0,4,1, "写块出错！:0x%02x", "Write Block Err:0x%02x",ucRet);
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                            }                
                            
                            ScrClrLine(2,7);
                            ScrDispStr(0,4,1,"M1卡读块...","Reading Block...");                        
                            ucRet = M1ReadBlock(BlkNo,tmps);
                            if(ucRet)
                            {
                                RfFunErr=ucRet;
								RfOtherErr=4;
								ScrClrLine(2,7);
								ScrDispStr(0,4,1,"读块出错！:0x%02x","Read Block Err:0x%02x",ucRet);                             
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break;                            
                            }                
                        }
                        else
                        {
                            ScrClrLine(2,7);
                            ScrDispStr(0,4,1, "数据交换.....","exchange- data...");
                            memset(&ApduSend,0,sizeof(ApduSend));
                            memset(&ApduResp,0,sizeof(ApduResp));
                            ApduSend.Command[0]=0x00;
                            ApduSend.Command[1]=0xa4;
                            ApduSend.Command[2]=0x04;
                            ApduSend.Command[3]=0x00;
                            ApduSend.Lc=14;
                            strcpy((char*)ApduSend.DataIn, "1PAY.SYS.DDF01");
                            ApduSend.Le=256;
                            ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                            if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                            {
                                RfFunErr=ucRet;
								RfOtherErr=5;
								ScrClrLine(2,7);
								ScrDispStr(0,2,1,"选择主文件错误","Select Main File Err!");//选择3F00主文件
								ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                       
                            }
                            
                            //*添加读写100字节数据测试
                            memset(&ApduSend,0,sizeof(ApduSend));
                            memset(&ApduResp,0,sizeof(ApduResp));
                            ApduSend.Command[0] = 0x00;
                            ApduSend.Command[1] = 0xa4;
                            ApduSend.Command[2] = 0x04;
                            ApduSend.Command[3] = 0x00;
                            ApduSend.Lc = 0x09;
                            ApduSend.Le = 256;
                            memcpy(ApduSend.DataIn,(uchar *)"\xA0\x00\x00\x00\x03\x86\x98\x07\x01",9);
                            ApduSend.DataIn[9]=0;
                            ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                            if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                            {
                                RfFunErr=ucRet;
								RfOtherErr=6;
								ScrClrLine(2,7);
								ScrDispStr(0,2,1,"选择应用文件错误","Select App File Err!");//选择2F01应用文件
								ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);                                   
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                            }
                            
                            memset(&ApduSend,0,sizeof(ApduSend));
                            memset(&ApduResp,0,sizeof(ApduResp));
                            ApduSend.Command[0] = 0x00;
                            ApduSend.Command[1] = 0xa4;
                            ApduSend.Command[2] = 0x00;
                            ApduSend.Command[3] = 0x00;
                            ApduSend.Lc = 0x02;
                            ApduSend.Le = 256;
                            memcpy(ApduSend.DataIn,(uchar *)"\x00\x19",2);
                            ApduSend.DataIn[2]=0;
                            ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                            if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                            {
                                RfFunErr=ucRet;
								RfOtherErr=7;
								ScrClrLine(2,7);
								ScrDispStr(0,2,1,"选择记录文件错误","Select Record File Err!");//选择0019记录文件
								ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                            }
							
                            ScrClrLine(2,7);
                            ScrDispStr(0,2,1,"写数据中......","Writing- data...");

                            for(ii=0; ii<100; ii++)
                            {
                                DataIn[ii]=rand()%0xff;
                            }
                    
                            memset(&ApduSend,0,sizeof(ApduSend));
                            memset(&ApduResp,0,sizeof(ApduResp));
                            ApduSend.Command[0] = 0x00;
                            ApduSend.Command[1] = 0xE2;
                            ApduSend.Command[2] = 0x00;
                            ApduSend.Command[3] = 0xC8;
                            ApduSend.Lc = 0x64;
                            ApduSend.Le = 256;
                            memcpy(ApduSend.DataIn,DataIn,100);
                    
                            ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                            if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                            {
                                RfFunErr=ucRet;
								RfOtherErr=8;
								ScrClrLine(2,7);
								ScrDispStr(0,2,1,"写数据错误","Write Data Err!");//写数据
								ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);                                   
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                            }
                            ScrClrLine(2,7);
                            ScrDispStr(0,4,1,"读数据中......","Reading- data...");
                            memset(&ApduSend,0,sizeof(ApduSend));
                            memset(&ApduResp,0,sizeof(ApduResp));
                            ApduSend.Command[0] = 0x00;
                            ApduSend.Command[1] = 0xb2;
                            ApduSend.Command[2] = 1;
                            ApduSend.Command[3] = 0xCC;
                            ApduSend.Lc = 0;
                            ApduSend.Le =100;
                            ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                            if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                            {
                                RfFunErr=ucRet;
								RfOtherErr=9;
								ScrClrLine(2,7);
								ScrDispStr(0,2,1,"读数据错误","Read Data Err!");//读数据
								ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);                                   
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                            }
                            else if(ApduResp.LenOut!=100 || memcmp(ApduResp.DataOut, DataIn,100))
                            {
                                RfFunErr=ucRet;
								RfOtherErr=10;
								ScrClrLine(2,7);
								ScrDispStr(0,2,1,"读取返回数据错误！","Read Resp Data Error!");
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                            }//                               
                            
                        }//
						
                        PiccRemove('H', 0);

                        if( flag==0 || flag==1 )
                        {
							resultStr[single*2+PlaceFlag][detectcnt+9]='1';
							BeepOK();
                            ScrClrLine(2,7);
                            if(PlaceFlag==0)
							{
								ScrDispStr(0,4,1,"在最小距离处%s卡\n第%d次测试成功",
									"%s Card Test OK (%d) in Minimum", CardType,detectcnt+1);
							}
							else
							{
								ScrDispStr(0,4,1,"在最大距离处%s卡\n第%d次测试成功",
									" %s Card Test OK (%d) in Maximum", CardType,detectcnt+1); 
                            }							
                        }
						else
						{
							BeepOK();
							ScrDispStr(0,4,1,"%s卡测试成功","%s Card Test OK", CardType,detectcnt+1);
							DelayMs(250);
						}

                    }//for(detect)
				//	DelayMs(400);//
                }//for(single)   
            }//for(PlaceFlag)
			if(flag==0 || flag==1)
				goto ENDOK;//里面有判断成功与失败
			
        }//while(2)

LoopResult:
        if(flag==2)
        {
            ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
            ShowSuccessRate(TotalNum,FailNum);
			kbflush();
			if(getkey()==KEYCANCEL)break;
        }

    }//while(1)
    
ENDOK:

    if(flag==0 || flag==1)
    {
        for(single = 0; single < singleNum; single++)
        {
            for(PlaceFlag = 0; PlaceFlag < PlaceNum; PlaceFlag++)
            {
                if(memcmp(resultStr[single*2+PlaceFlag]+9, "111", 3))
                {
                    if(TestFlag==1) goto ENDERR;
					else
					{
						memcpy(resultStr[single*2+PlaceFlag]+9, "000", 3);
						ErrorFlag=1;
					}
                }
            }
        }
	}

	if(ErrorFlag==1)
	{
		if(TestFlag==0) {
			ErrorFlag=0;
			TestFlag=1;
		//	goto START;
			goto ENDERR;//2014-06-04
		}
		else goto ENDERR;
	}
	

	PiccClose();
	PiccLight(PICC_LED_ALL, 0); 
    return 0;

ENDERR:

	PiccClose();
	PiccLight(PICC_LED_ALL, 0); 
    return 1;
}



//////////////////Felica卡////////////////////////
uchar Auto_FelicaTest(uchar flag)
{		
	  int nTime1=0;
	  int nTime=0;
	  uchar ucRet,DetectFlag;
	  uchar Place=0;
	  uchar rate=0,polarity=0,cmd[512],Resp[512];
	  uint slen=0,rlen=0,rlen1=0;
	  uint TotalNum=0,FailNum=0,OKNum=0;	  

	  //打开射频卡；
	  ScrCls();
	  ucRet=PiccOpen();//开打射频卡
	  if(ucRet)
	  {
		  ScrPrint(0,4,1,"PiccOpen:%02x",ucRet);
		  getkey();
		  return 1;
	  }

	 //初始化Felica卡；
	  ucRet = PiccInitFelica(0, 0);//调用初始化felica的API接口（新增）
	  if(ucRet)
	  {
		 ScrPrint(0,4,1,"PiccInitFelica err:%02x",ucRet);
		 BeepFail();
		 getkey();
		 goto ENDERR;
	  }
     
	  memset(cmd,0,sizeof(cmd));
	  memset(Resp,0,sizeof(Resp));
	  //组装命令
	  cmd[0]=0x06;
	  cmd[1]=0x00;
	  cmd[2]=0xFF;
	  cmd[3]=0xFF;
	  cmd[4]=0x01;
	  cmd[5]=0x00;
	  slen=6;
	  if (flag == 0) ScrDispStr(0,2,1,"FELICA测试","FELICA Test");
	  if (flag == 1) ScrDispStr(0,0,1,"单次测试","Single Test");
	  if (flag == 2) ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);

	  DelayMs(10);//200

	  DetectFlag=0;
	  nTime=GetTimerCount();
	  while(GetTimerCount()-nTime<7*1000)

	//  while(1)
	  {
TEST2CM:
		  ScrClrLine(2,7);
		  if(flag==0 || flag==1){
			  TotalNum=0;
			  OKNum=0;
			  FailNum=0;
			  if(Place==0) ScrDispStr(0,4,1,"请离感应区0CM 刷FeliC卡","PLS TAP FELICA AT 0CM");
			  else		   ScrDispStr(0,4,1,"请离感应区2CM 刷FeliC卡","PLS TAP FELICA AT 2CM");	
		  }
		  else{
			  ScrDispStr(0,4,1,"请感应Felica大卡","PLS TAP FELICA");
		  }	  
		  DetectFlag=0;
		  nTime1=GetTimerCount();
		  while(GetTimerCount()-nTime1<3*1000)

		 //while(2)
		 {
			 if (CancelCheck())
			 {
				if(flag==0 || flag==1)goto ENDERR;
				else break;
			 }
			 if(flag==2)
			 {
				 if(TotalNum==10000) break;
				 ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
			 }	 
			 memset(Resp,0,sizeof(Resp));
			 rlen=0;
			 ucRet=PiccCmdExchange(slen,cmd,&rlen,Resp);//调用PiccCmdExchange接口向felica卡发送polling命令
			 if(ucRet==0 && rlen>0)
			 {
				BeepOK();
				DelayMs(100);
				TotalNum++;
				OKNum++;
				DetectFlag=1;//一旦第一次检测到卡就开始一直检测					
			 }
			 else
			 {
				 if(DetectFlag==1)
				 {
					 BeepFail();
					 TotalNum++;
					 FailNum++;
					 ScrDispStr(0,6,1,"感应失败","Tap FeliC Fail");
					 if(flag<=1)goto ENDERR;
				 }

			 }
			 
			 if(flag<=1)
			 {
				 if(TotalNum==3)
				 {
					 if(OKNum==3)
					 {
						 if(Place==0)
						 {
							ScrDispStr(0,6,1,"0CM处感应成功","At 0CM Test OK");
							Place=2;
							//DelayMs(200);
							DelayMs(1000);//2014-06-04
							goto TEST2CM;
						 }
						 else
						 {
							ScrDispStr(0,6,1,"2CM处感应成功","At 2CM Test OK");
							DelayMs(200);
							goto ENDOK;
						 }
					 }
					 else
					 {
						goto ENDERR;
					 }
				}

			}
		}
//		ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
//		ShowSuccessRate(TotalNum,FailNum);
//		kbflush();
//		if(getkey()==KEYCANCEL)break;
	}
	ScrDispStr(0,6,1,"测试超时"," Test Timeout");//
	 //return 1;
	goto ENDERR;
ENDOK:
	if(flag==0)
	{
		
		RECORD(1,"FELICA=1");
		RECORD(1,"\n");
	}
    PiccClose();
    return 0;
	
ENDERR:
    if(flag==0)
	{
		RECORD(1,"FELICA=0");
		RECORD(1,"\n");
	}
    PiccClose();
    return 1;
}

uchar Auto_IcTest(uchar flag)
{
	uint TotalNum,FailNum;
	uchar count,slot,ucRet,BuffAtr[40];
	ScrClrLine(2,7);
	
	//if(flag==0) ScrDispStr(0,2,1,"IC卡测试", "IC Test");//防止显示不好看
	if(flag==1) ScrDispStr(0,0,1, "单次测试", "Single Test");
	if(flag==2) ScrDispStr(0,0,1, "循环测试", "Cycle Test ");

	/*
	memcpy(ApduSend.Command,"\x00\xa4\x04\x00",4);   //选择主文件命令
	memcpy(ApduSend.DataIn, "1PAY.SYS.DDF01",14);
	ApduSend.Le=256;       
	ApduSend.Lc=14;
	*/

    if(Autotest == 1)
	{
		DelayMs(100);
	//	return Autotest_result[3];
	}

	TotalNum=0;
	FailNum=0; 
	count=0;
	while(1)
	{

		while(2)
		{
			
			if(flag==2)
			{
				if(TotalNum==10000)break;
				if(CancelCheck())break;
				ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
				ScrClrLine(2,7);
			}
			
			count=0;
			for(slot=0; slot<=(gSamNum+1); slot++)// 有3张卡/0通道/2通道/3通道 或者 2/3/4通道  所以需要+1
			{
				if(gIcNum==0 && slot==0)continue;
				if(slot==1) continue;
				ucRet=IccInit(slot,BuffAtr);
				IccClose(slot);
				if(ucRet){
					ScrPrint(0,(uchar)(6-gSamNum+slot),0,"SLOT%d: ERROR %02x",slot,ucRet);
					RECORD(1, "IC.slot%d.err=%02x",slot,ucRet);
					continue;
				}
				else{
					ScrPrint(0,(uchar)(6-gSamNum+slot),0,"SLOT%d: PASSED",slot);
				}				
				count++;
			}
			
			if( count<(gIcNum+gSamNum) )//
			{
				FailNum++; 
				BeepFail();
				DelayMs(200);
				if(flag<=1) 
				{
					RECORD(1, "IC=0\n");
					RECORD(1, "\n");

					if(Autotest == 0)
						Autotest_result[3] = 1;

					return 1;
				}
			}
			else
			{
				BeepOK();
				DelayMs(200);
				if(flag<=1)
				{
					RECORD(1, "IC=1\n");
					RECORD(1, "\n");

					if(Autotest == 0)
						Autotest_result[3] = 0;

					return 0;
				}
				
			}
		} //2
		ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",TotalNum);
		ShowSuccessRate(TotalNum,FailNum);	
		kbflush();
		if(getkey()==KEYCANCEL)break;
	}
	return 0;
}





uchar myPinpadTest(uchar flag)
{
	if(!Autotest)
	{
		Autotest_result[1] = PinpadTest(flag);	
	}
	//DelayMs(100);
	return Autotest_result[1];
}
uchar myIcTest(uchar flag)
{
	char tmp;
	if(!Autotest)
	{
		tmp = gSamNum;
		gSamNum = 0;
		Autotest_result[3] = IcTest(flag);	
		gSamNum = tmp;
	}
	else if(gSamTest==1)
	{
		Autotest_result[3] = IcTest(flag);	
	}

	return Autotest_result[3];

}
uchar myWnetTest(uchar flag)
{
	if(!Autotest)
	{
		Autotest_result[4] = WnetTest(flag);	
	}
	//DelayMs(100);
	return Autotest_result[4];

}
uchar myLanTest(uchar flag)
{
	if(!Autotest)
	{
		Autotest_result[5] = LanTest(flag);	
	}
	//DelayMs(100);
	return Autotest_result[5];

}
uchar myModemTest(uchar flag)
{
	if(!Autotest)
	{
		//Autotest_result[6] = auto_modem(flag);	
	}
	//DelayMs(100);
	return Autotest_result[6];

}
uchar myWifiTest(uchar flag)
{
	if(!Autotest)
	{
		Autotest_result[7] = WifiTest(flag);	
	}
	//DelayMs(100);
	return Autotest_result[7];

}
uchar myUDiskTest(uchar flag)
{
	if(!Autotest)
	{
		Autotest_result[8] = UDiskTest(flag);	
	}
	//DelayMs(100);
	return Autotest_result[8];

}

uchar myBatTest(uchar flag)
{

	if(TermInfo[0]==S910)
	{
		if(BatteryCheck() <= 4 )
		{
			BeepFail();
			ScrDispStr(0,6,1,"电池测试失败    ","BAT TEST FAIL");
			DelayMs(500);
			return 1;
		}
		else
		{
			BeepOK();
			ScrDispStr(0,4,1,"电池测试成功    ","BAT TEST OK   ");
			DelayMs(500);
			return 0;
		}
	}
	
	return BatteryTest(flag);
}


uchar udisk_tmp;
void config(void)
{
	int i;
	udisk_tmp = TestModule[T_UDISK];
	if(TermInfo[0]==S910)
		udisk_tmp = 'N';

	TestModule[T_VER] = 'Y';
	for(i=60;i<=67;i++)   //T_LCD1	60,T_LCD_W	67
		TestModule[i] = 'Y';
}




int AutoTest(int flag)
{
	uchar ComNo = gPPQcom;
	uchar ucRet;
	uchar buff[30];

	int i;
	int k=0;

/*
	各项的测试顺序如下	
	IC:0
	MODEM:1
	...
*/

    

	AutoTestItem ATestItem[]={
		{TestModule[T_VER],				Auto_InfoTest   },
		{TestModule[T_KB],				Auto_KbTest		},
		{TestModule[T_LCD],				Auto_LcdTest	},//T_UDEV_TEST表示整机需不需要测试//LCD黑屏
		{TestModule[T_BEEP],			Auto_BeepTest	},
		{TestModule[T_MAG],				Auto_MagTest	},
		{TestModule[T_RF],				NewRfTest		},
		{TestModule[T_IC],			    myIcTest		},
        {TestModule[T_PRINTER],			Auto_PrinterTest	},
		{TestModule[T_LAN],				myLanTest		},
		{TestModule[T_MODEM],			myModemTest	},
		{TestModule[T_WIFI],			myWifiTest	},
		//{TestModule[T_BT],				BtTest		},
		{TestModule[T_PINPORT],			myPinpadTest	},
		{TestModule[T_COMB],			ComBTest	},
		{TestModule[T_UDISK],			myUDiskTest	},
		{TestModule[T_SDCARD],			SDCardTest	},
		{TestModule[T_PAPERBACK],		PaperBackTest},
		{TestModule[T_WNET],			myWnetTest	},//TestSim0
		{TestModule[T_BATTERY],			myBatTest	},
		{TestModule[T_SPEECH],			SpeechTest	},
		//{TestModule[T_BYPHONE],			ByPhoneTest	},
		{TestModule[T_UDEV_TEST],		UsbDevTest	},//T_USBDEV是表示有没有该模块，
		{TestModule[T_CLK],				Auto_ClkTest},//UHOST与UDEV是同个口，则不测试
		{TestModule[T_WIFI],			myWifiTest	},
		{udisk_tmp,						myUDiskTest	},
		{TestModule[T_KEYLED],			KbLedTest	},
		{TestModule[T_LED],				LedTest		},
		{TestModule[T_FELICA_ALL],		Auto_FelicaTest	},
		//{'N',							TestSim1	},//TestModule[T_SIM1]
		{TestModule[T_TSCREEN],			TsTest		},//TestModule[T_TSCREEN] S900/S300不测试触屏
		//{TestModule[T_GSENSOR],			GSensorTest	},
		{TestModule[T_SCAN],			ScanTest	},
		{TestModule[T_LCD1],			Auto_LcdTest1	},//add lifw 2014-05-13 白屏
		{TestModule[T_LED1],			Auto_IcTest	},//too
		{TestModule[T_LED2],			Auto_LedTest2	},//too
		{TestModule[T_BYPHONE],			ModemTest},//
		{TestModule[T_LCD_R],			LcdTestR},
		{TestModule[T_LCD_G],			LcdTestG},
		{TestModule[T_LCD_B],			LcdTestB},
		{TestModule[T_LCD_BLACK],		LcdTestBlack},
		{TestModule[T_LCD_W],			LcdTestW},
		{TestModule[T_KB],				auto_kb	},
		
	};


	ucRet = PortOpen(ComNo,"115200,8,n,1");
	if(ucRet)
	{
		ScrPrint(0,4,1,"PortOpen(%d)=%d",ComNo,ucRet);
		getkey();
		PortClose(ComNo);
		return ucRet;
	}


	while(1)
	{
		if(CancelCheck())return 0;
		ucRet=hshake(ComNo,'I');
		if(ucRet)
		{
			if(ucRet==0xff)return 0;
			ScrPrint(0,4,0,"HandShake1 fail!");
			DelayMs(1000);
			continue;
		}
		break;
	}

	Autotest = 0;
    TimerSet(2,250);//为了时钟测试
	GetTime(gTime);//为了时钟测试

	
	if(TestModule[T_MODEM]=='Y'){
		//auto_modem(1);//进行预拨号
	}

	//先进行自测项目的测试
	if(TestModule[T_VER] != 'N')
		Auto_InfoTest(0);

	if(TestModule[T_PRINTER] != 'N')
		if( Auto_PrinterTest(0) )
		{
			while(1)
			{
				if(CancelCheck())return 0;
				PortSend(0,'J');
				DelayMs(10);
			}
		}
	if(TestModule[T_PINPORT] != 'N')
		if( myPinpadTest(0) )
		{
			while(1)
			{
				if(CancelCheck())return 0;
				PortSend(0,'J');
				DelayMs(10);
			}
		}

	if(TestModule[T_CLK] != 'N')
		if( Auto_ClkTest(0) )
		{
			while(1)
			{
				if(CancelCheck())return 0;
				PortSend(0,'J');
				DelayMs(10);
			}
		}

    if(TestModule[T_IC] != 'N')
		if( myIcTest(0) )
		{
			while(1)
			{
				if(CancelCheck())return 0;
				PortSend(0,'J');
				DelayMs(10);
			}
		}

    if(TestModule[T_WNET] != 'N')
		if( myWnetTest(0) )
		{
			while(1)
			{
				if(CancelCheck())return 0;
				PortSend(0,'J');
				DelayMs(10);
			}
		}

	if(TestModule[T_LAN] != 'N')
		if( myLanTest(0) )
		{
			while(1)
			{
				if(CancelCheck())return 0;
				PortSend(0,'J');
				DelayMs(10);
			}
		}

    if(TestModule[T_MODEM] != 'N')
	    if( myModemTest(0) )
		{
			while(1)
			{
				if(CancelCheck())return 0;
				PortSend(0,'J');
				DelayMs(10);
			}
		}

	if(TestModule[T_WIFI] != 'N')
		if( myWifiTest(0) )
		{
			while(1)
			{
				if(CancelCheck())return 0;
				PortSend(0,'J');
				DelayMs(10);
			}
		}
	if( udisk_tmp != 'N')
        if( myUDiskTest(0) )
		{
			while(1)
			{
				if(CancelCheck())return 0;
				PortSend(0,'J');
				DelayMs(10);
			}
		}

	Autotest = 1;
	

	ucRet = PortOpen(ComNo,"115200,8,n,1");
	if(ucRet)
	{
		ScrPrint(0,4,1,"PortOpen(%d)=%d",ComNo,ucRet);
		getkey();
		PortClose(ComNo);
		return ucRet;
	}

	

	while(1)
	{
		if(k==0)
		{
		
			ScrCls();
			ScrDispStr(0,0,0x81,"    自动测试    ","    AUTO TEST   ");
			ucRet=hshake(ComNo,'H');
			if(ucRet)
			{
				if(ucRet==0xff)return 0;
				ScrPrint(0,4,0,"HandShake fail!");
				DelayMs(1000);
				continue;
			}
		}
		k++;
		ScrCls();
		//ScrDispStr(0,0,1,"接收命令 ...","Recv cmd ...");
		memset(buff,0,sizeof(buff));
		
		ucRet=RecvCMD(ComNo,buff);
		if(ucRet)
		{
			if(ucRet == 0xff) return 1;
			ScrPrint(0,6,0,"RecvCMD fail:%d",ucRet);
			continue;
		}
		
		i=buff[3];
		if(i > sizeof(ATestItem)/sizeof(ATestItem[0]))
		{
			ScrPrint(0,4,1,"TestModel Err =%d ",i);
			DelayMs(1500);
			return;
		}
		ScrPrint(0,4,0,"i =%d ",i);
		ScrPrint(0,5,0,"TestModel =%d ",ATestItem[i].model);
		if(ATestItem[i].model=='N')
		{
			DelayMs(200);
			SendRSP(ComNo,0x01,'O',0xff);
			continue;
		}
		else ucRet = ATestItem[i].TestFun(0);
		
		if(i==0)     //PPQ信息，不返回结果
			continue;  

		if(ucRet)
			SendRSP(ComNo,0x01,'F',ucRet);
		else
			SendRSP(ComNo,0x01,'O',0x00);	
	
	}

}


void AutoTest0(void)
{
	config();
	AutoTest(0);
}

