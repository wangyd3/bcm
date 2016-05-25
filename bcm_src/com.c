#include "public.h"

#define PARAMETER "115200,8,n,1"
#define MAX_CMD_LEN  300
#define MAX_RSP_LEN  100
#define COMM_ERR     0xff
#define NO_PIN       0x0a
#define NO_AMT       0x0b
#define USER_CANCEL  0x06

static uchar ucRet,ucLen, CmdStr[MAX_CMD_LEN], RespStr[MAX_RSP_LEN];
static uint  uiLen;

static uchar RecvResp(uchar Head, uchar *Resp, uint *RecvLen,ushort TimeOut)
{
	uint i;
	Resp[0]=0;
	while(1)
	{
		if(TimeOut)
		{
            if( !PortRecv(gComPort, Resp, TimeOut)) 
			{			
				break;
			}
            return COMM_ERR;
        }
		else
		{
			PortRecv(gComPort, Resp, 50);
			if(CancelCheck()) return USER_CANCEL;
        }
		if(Resp[0]==Head || Resp[0] == Head -0x80) break;
	}
	if(PortRecv(gComPort, Resp+1, 200)) return COMM_ERR;
	for(i=0;i<(uint)(Resp[1]+1);i++)
	{
		if(PortRecv(gComPort, Resp+i+2, 200)) return COMM_ERR;
	}
	*RecvLen = Resp[1]+3;
	return 0;
}

static void GetEDC(uchar *str, uint len)
{
	uint i;

	str[len]=str[0];
	for(i=1;i<len;i++)
		str[len] ^= str[i];
}

static uchar SendRecv(uchar *CmdStr,uint SendLen, uchar *RespStr, uint *RecvLen,ushort TimeOut)
{
    uchar EDC;
    uint i,j;

    if(PortOpen(gComPort, "9600,8,e,1")) return COMM_ERR;
	
	GetEDC(CmdStr,SendLen);
	for(j=0;j<3;j++)
	{
    	for(i=0;i<SendLen+1;i++) PortSend(gComPort, CmdStr[i]);
		ucRet=RecvResp(CmdStr[0],RespStr,RecvLen,TimeOut);
		if (ucRet)
		{
			if(ucRet ==COMM_ERR)	continue;
			return ucRet;
		}
		EDC=RespStr[*RecvLen-1];
		GetEDC(RespStr,(uint)(*RecvLen-1));
		if(RespStr[*RecvLen-1] ==EDC)	break;
		DelayMs(10);
	}
	if(j >=3){	
		PortClose(gComPort);
		return COMM_ERR;
	}
	PortClose(gComPort);
	return 0;
}

static uchar PPVerInfo(uchar *ver)
{
	ucRet=1;RespStr[2]=1;//
	memcpy(CmdStr,"\x90\x00",2);
	ucRet=SendRecv(CmdStr, 2, RespStr, &uiLen,1500);
	if(ucRet) return ucRet;
	if(RespStr[0] == 0x90)
	{
		memcpy(ver,RespStr+2,8);
		return 0;
	}
	return RespStr[2];
}

static uchar ReadPPVerinfo(const uchar *ComType)
{
	uchar retRecv;
	uchar ppver[8];	
    ScrClrLine(2,7);
	ScrDispStr(0,2,1," %s读PP20版本信息","%s READ PP20",ComType);
#ifdef BAXI
	ScrDispStr(0,4,1, "[确认]-测试", "[ENTER]-TEST");
	ScrDispStr(0,6,1, "[其他]-跳过", "[OTHER]-SKIP");
	if(getkey()!=KEYENTER)return 0;
	ScrClrLine(4,7);
#endif

	memset(ppver,0x00,sizeof(ppver)); 
	retRecv=PPVerInfo(ppver);
    if(retRecv) 
	{
		BeepFail();
		ScrDispStr(0,6,1,"失败","Fail");
		DelayMs(500);
		return 1;
    }
    else
	{
		BeepOK();
		ScrDispStr(0,6,1,"成功","PASS");
		DelayMs(500);
		return 0;
	}
}

/************************************************************************/
/* 测试PINPAD/RS232B/RS232A口：读PP20的版本信息							*/
/************************************************************************/
uchar PinpadTest(uchar flag)  
{
	if(TestModule[T_PINPORT]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"无PINPAD口","No PINPORT ");
		if(flag!=0)	getkey();
		return 0xff;
	}
	gComPort=P_PINPAD;
	return ( ReadPPVerinfo("PINPAD") );
}

uchar ComBTest(uchar flag)  
{
	if(TestModule[T_COMB]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"无RS232B","No RS232B ");
		if(flag!=0)	getkey();
		return 0xff;
	}
	gComPort=P_RS232B;
	return ( ReadPPVerinfo("RS232B") );
}

uchar ComATest(uchar flag)
{
	gComPort=P_RS232A;
	return ( ReadPPVerinfo("RS232A") );
}

//为了满足统一菜单，统一函数格式
void ComATest1(void)
{
	ComATest(1);
}
void ComBTest1(void)
{
	ComBTest(1);
}
void PinpadTest1(void)
{
	PinpadTest(1);
}




#define PORT_NO 0
#define DATA_LEN 8192
uchar COMA2PC(uchar flag)
{
	int i,ret,totle,recCnt;
	uchar sendBuff[DATA_LEN];
	uchar recvBuff[DATA_LEN];
	
	ScrCls();
	ScrDispStr(0,2,1,"RS232A 测试","RS232A TEST");		

	for(i=0;i<sizeof(sendBuff);i++)
		sendBuff[i] = i;

	if(ret = PortOpen(PORT_NO, "115200,8,n,1"))
	{
		ScrPrint(0,6,1,"PortOpen(%d) = %d",PORT_NO,ret);
		goto ENDERR;
	}
			
	PortReset(PORT_NO);
   	if(ret = PortSends(PORT_NO, sendBuff, DATA_LEN))
	{
   		ScrPrint(0,6,1,"PortSends(%d) = %d",PORT_NO,ret);
		goto ENDERR;
   	}		

	totle=0;
	recCnt=0;
	TimerSet(1,100);
	memset(recvBuff,0,sizeof(recvBuff));
	while(TimerCheck(1))
	{
		if(totle>=DATA_LEN) break;
		recCnt = PortRecvs(PORT_NO, &recvBuff[totle], DATA_LEN, 0);
		if(recCnt>0) totle+=recCnt;				
	}

	if(memcmp(sendBuff, recvBuff, DATA_LEN))
	{
		ScrPrint(0,6,1,"TEST FAIL ");
		goto ENDERR;
	}
	ScrPrint(0,6,1,"TEST OK  ");
	PortClose(PORT_NO);
	BeepOK();
	return 0;
ENDERR:
	PortClose(PORT_NO);
	BeepFail();
	getkey();
	return 1;
}


void PortTestAll(void)
{

	TESTSINGLE_ITEM TestItem[]={
		{"1-RS232A",			"1-RS232A",				ComATest1,},
		{"2-RS232B",			"2-RS232B",				ComBTest1,},
		{"3-PINPAD",			"3-PINPAD",				PinpadTest1,},
		{"1-COMA2PC",			"1-COMA2PC",			COMA2PC,}
	};
	uchar TestNum;
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"串口单项测试","COM Port Test",3);
}