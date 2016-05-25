#include "public.h"

char gApnType=0;
char gRsp[300];
#define G610_FILE "WNETTEST.DAT"
int WlRecordCmd(char *Cmd)
{
	int iRet;
	memset(gRsp,0,sizeof(gRsp));
	iRet=WlSendCmd(Cmd,gRsp,250,3000,1000);
	ComSends("%s\r\niRet=%d\r\n%s\r\n",Cmd,iRet,gRsp);
	RecordLog(G610_FILE,"%s\r\niRet=%d\r\n%s\r\n",Cmd,iRet,gRsp);
	return 0;
}

int band_test(void)
{
	int iRet,n1800=0,n900=0,nfail=0,nall=0;
	int time_start,time_end,i;
	uchar *p,time_tmp[10];
	memset(time_tmp,0,sizeof(time_tmp));
	GetTime(time_tmp);
	ComSends("20%02x-%02x-%02x  %02x:%02x:%02x",time_tmp[0],
		time_tmp[1],time_tmp[2],time_tmp[3],time_tmp[4],time_tmp[5]);
	RecordLog(G610_FILE,"20%02x-%02x-%02x  %02x:%02x:%02x",time_tmp[0],
		time_tmp[1],time_tmp[2],time_tmp[3],time_tmp[4],time_tmp[5]);
	
	ScrCls();
	WlSwitchPower(1);
	WlInit(NULL);
	iRet=WlOpenPort();   //������ͨѶ����
    if(iRet)
    {
        ScrClrLine(4,7);
        BeepFail();
        ScrDispStr(0,4,0x01,"��ͨ�Ŵ���ʧ��:%d","Open Port Err:  %d",iRet);
        getkey();
		return iRet;
    }
	
	while(1)
	{
		if(CancelCheck())break;
		nall++;
		time_start = GetTimerCount();
		ComSends("\r\n&&&&&&&&&&&&&&&&& %d &&&&&&&&&&&&&&&",nall);	
		RecordLog(G610_FILE,"\r\n&&&&&&&&&&&&&&&&& %d &&&&&&&&&&&&&&&",nall);
		
		//�ȸ�λ
		WlRecordCmd("AT+CFUN=15\r");
		DelayMs(5000);

		//�鿴��Ƶ��
		WlRecordCmd("AT+CBAND?\r");

		//�鿴�Ƿ�ע����
		for(i=0;i<20;i++)
		{
			ComSends("********%d********",i);
			RecordLog(G610_FILE,"********%d********",i);
			WlRecordCmd("AT+CGREG?\r");
			if( strstr(gRsp,"+CGREG: 0,1")!=NULL || strstr(gRsp,"+CGREG: 0,5")!=NULL )break;
			DelayMs(2000);
		}
		
		if(i>=19)
		{
			nfail++;
		}
		else 
		{
			//�鿴ע������Ǹ�Ƶ��
			WlRecordCmd("AT+MCELL=0,21\r");
			p = strstr(gRsp,"ARFCN:0");	
			*(p+10) = 0;
			if( strcmp(p,"ARFCN:0000")>0 && strcmp(p,"ARFCN:0124")<0 )
			{
				n900++;
				ComSends("It is 900");
				RecordLog(G610_FILE,"It is 900");
			}
			else if(strcmp(p,"ARFCN:0512")>0 && strcmp(p,"ARFCN:0885")<0 )
			{
				n1800++;
				ComSends("It is 1800");
				RecordLog(G610_FILE,"It is 1800");
			}
		}
		time_end = GetTimerCount();
		ComSends("It is %dms",time_end-time_start);
		RecordLog(G610_FILE,"It is %dms",time_end-time_start);
		ComSends("all=%d,fail=%d,n1800=%d,n900=%d",nall,nfail,n1800,n900);
		RecordLog(G610_FILE,"all=%d,fail=%d,n1800=%d,n900=%d",nall,nfail,n1800,n900);
		ScrPrint(0,2,1,"all:%d,fail:%d",nall,nfail);
		ScrPrint(0,4,1,"n1800:%d",n1800);
		ScrPrint(0,6,1,"n900:%d",n900);		
	}
	GetTime(time_tmp);
	ComSends("20%02x-%02x-%02x  %02x:%02x:%02x",time_tmp[0],
		time_tmp[1],time_tmp[2],time_tmp[3],time_tmp[4],time_tmp[5]);
	RecordLog(G610_FILE,"20%02x-%02x-%02x  %02x:%02x:%02x",time_tmp[0],
		time_tmp[1],time_tmp[2],time_tmp[3],time_tmp[4],time_tmp[5]);
	return 0;

}

void WNetUpdate(void)
{
    ScrClrLine(2,7);
	ScrDispStr(0,4,1,"    ����ת��    ","   COM RELAY    ");
	//WlSwitchPower(1);//����仰��ҪWlInit(NULL);�ǲ��е�
    PortOpen(0,"115200,8,n,1");
	WlInit(NULL);//����������в���ת��,��ʹ�µ���,���������Ƚ����ϵ�
	WlOpenPort();
	WlSendCmd(0,0,0,0,0);
	WlClosePort();
	PortClose(0);  
}


/*��ͨ���˳���  flag=0���Զ��Ҷ�,flag=1�����Զ��Ҷ�,flag=2 CMU����*/
int AntennaTest(int flag)
{
	int iRet;
	static char dialnum[20]="112";
	char cmd[50],ucKey;

	ScrCls();
	memset(cmd,0,sizeof(cmd));
	if(flag==2)
	{
		if(TestModule[T_CDMA]=='Y')
			sprintf(cmd,"AT+CDV112;\r");
		else
			sprintf(cmd,"ATD112;\r");
	}
	else
	{
		if(InputAsc("�����벦�ź���","Input dial num",dialnum, 0, 20, 0)) return 1;
		if(TestModule[T_CDMA]=='Y')
			sprintf(cmd,"AT+CDV%s;\r",dialnum);
		else
			sprintf(cmd,"ATD%s;\r",dialnum);
	}

	ScrDispStr(0,2,1, "ģ���ϵ�...", "WNET Power On...");
	WlSwitchPower(1);

	if(gSimNum==2)
	{
		ScrDispStr(0,2,1,"ѡ��SIM��0", "Select SIM0");
		iRet = WlSelSim(0);
		if(iRet<0)
		{
			BeepFail();
			ScrDispStr(0,2,1,"ѡ��SIM0ʧ��!%d", "Select SIM0 fail!%d",iRet);
			getkey();
			return 1;
		}
	}

	ScrDispStr(0,2,1,"ģ���ʼ��...", "WNET Init...");
	iRet = WlInit(NULL);
	if(iRet!=0 && iRet!=-212)
	{
		BeepFail();
		ScrDispStr(0,4,1,"ģ���ʼ��ʧ��:%d","WlInit Err:  %d",iRet);
		getkey();
		return 1;
	}

	if(flag==2)
	{
		GetWirelessVer();
		ScrClrLine(2,7);
		ScrDispStr(0,2,1,"�汾��","VERSION:");
		ScrPrint(0,4,1,"%s,%s",gWireVer,gWnetVer);   //��ʾ�汾��
	}

	iRet=WlOpenPort();   //������ͨѶ����
	if(iRet)
	{
		BeepFail();
		ScrDispStr(0,4,1,"��ͨ�Ŵ���ʧ��:%d","Open Port Err:  %d",iRet);
		getkey();
		return 1;   
    }
	
	ScrDispStr(0,4,1,"��%s...", "Dialing %s...",cmd+3);
	iRet=WlSendCmd(cmd,NULL,0,0,0);
	if (iRet!=RET_OK)
	{
		BeepFail();
		ScrPrint(0,6,1,"ATD err:%d",iRet);
		getkey();
		return 1;
	}

	if(flag==2)
	{
		DelayMs(1000);
		ucKey = JudgeState("������,����","SeeData,WNET");
		if(ucKey == KEYENTER)
		{
			RECORD(1,"WNET_CMU = 1\r\n");
			return 0;
		}
		RECORD(1,"WNET=0\r\n");
		return 1;
	}

	ScrDispStr(0,4,1,"��%s�ɹ�", "Dial %s Success",dialnum);
	getkey();
	if(flag==0) //���Զ��Ҷ�
	{
		if(TestModule[T_CDMA]=='Y')
			WlSendCmd("AT+CHV\r",NULL,0,8*1000,0);
		else
			WlSendCmd("AT+CHUP\r",NULL,0,8*1000,0);
		DelayMs(50);
		WlClosePort();
	}
	return 0;
}


void RingTest(void)
{
	int iRet;
	char rsp[100];

	ScrCls();
	ScrDispStr(0,0,1,"��������", "Ring Test");
	ScrDispStr(0,2,1,"ģ���ϵ�...", "WNET Power On...");
	//WlSwitchPower(0);
	WlSwitchPower(1);

	if(gSimNum==2)
	{
		ScrDispStr(0,2,1,"ѡ��SIM��0", "Select SIM0");
		iRet = WlSelSim(0);
		if(iRet<0)
		{
			BeepFail();
			ScrDispStr(0,4,1,"ѡ��SIM0ʧ��!%d", "Select SIM0 fail!%d",iRet);
			getkey();
			return;
		}
	}

	ScrDispStr(0,2,1,"ģ���ʼ��...", "WNET Init...");
	iRet = WlInit(NULL);
	
	if(iRet!=0 && iRet!=-212)
	{
		BeepFail();
		ScrDispStr(0,4,1,"ģ���ʼ��ʧ��:%d","WlInit Err:  %d",iRet);
		getkey();
		return ;
	}

	iRet=WlOpenPort();   //������ͨѶ����

	ScrDispStr(0,2,1,"�����绰...","Please Ring...");
	memset(rsp,0,sizeof(rsp));
	while(1)
	{
		if(CancelCheck())break;
		WlSendCmd(NULL,rsp,50,3*1000,0);
		ComSends("rsp=%s",rsp);
		if(strstr(rsp,"RING") || strstr(rsp,"ring") )
		{
			ScrPrint(0,4,1,"Get Ring....");
			DelayMs(2000);
			if(1)//XuiGetKey()==KEYENTER
			{
				WlSendCmd("ATA\r",NULL,0,8*1000,0);
				ScrPrint(0,6,1,"Answer...");
				getkey();
				if(TestModule[T_CDMA]=='Y')
					WlSendCmd("AT+CHV\r",NULL,0,8*1000,0);
				else
					WlSendCmd("AT+CHUP\r",NULL,0,8*1000,0);
				WlClosePort();
				return;
			}
			else
			{
				return;
			}
		
		}
	}

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
	memset(uid,0x00,sizeof(uid)); //��ʼ���û���
	memset(pwd,0x00,sizeof(pwd)); //��ʼ������
	ScrClrLine(2,7);
#ifdef BAXI
	if(flag) //������������
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
			memcpy(uid,"claro",5); //�����û���
			memcpy(pwd,"claro",5); //��������
			if(TermInfo[9])memcpy(dialNum, "claro.com.br",sizeof("claro.com.br"));
			else if(TermInfo[10])memcpy(dialNum, "#777", 4);
			else if(TermInfo[18])memcpy(dialNum, "3GNET", 5);
		}
		else if(ucRet==KEY2)
		{
			//memcpy(uid,"card",4); //�����û���
			//memcpy(pwd,"card",4); //��������
			if(TermInfo[9])memcpy(dialNum, "gprs.oi.com.br",sizeof("gprs.oi.com.br"));
			else if(TermInfo[10])memcpy(dialNum, "#777", 4);
			else if(TermInfo[18])memcpy(dialNum, "3GNET", 5);
		}
		else if(ucRet==KEY3)
		{
			memcpy(uid,"tim",3); //�����û���
			memcpy(pwd,"tim",3); //��������
			if(TermInfo[9])memcpy(dialNum, "tim.br",sizeof("tim.br"));
			else if(TermInfo[10])memcpy(dialNum, "#777", 4);
			else if(TermInfo[18])memcpy(dialNum, "3GNET", 5);
		
		}
		else
		{
			memcpy(uid,"vivo",4); //�����û���
			memcpy(pwd,"vivo",4); //��������
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
	else  if(0)//��������
	{

		ScrPrint(0,2,1,"1-claro");
		ScrPrint(0,4,1,"2-mms.vivo");
		ucRet = getkey();
		if(ucRet==KEY1)
		{
			memcpy(uid,"claro",5); //�����û���
			memcpy(pwd,"claro",5); //��������
			if(TermInfo[9])memcpy(dialNum, "claro.com.br",sizeof("claro.com.br"));
		}
		else
		{
			memcpy(uid,"vivo",4); //�����û���
			memcpy(pwd,"vivo",4); //��������
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
#else //BCM  //����
	memcpy(uid,"card",4); //�����û���
	memcpy(pwd,"card",4); //��������
	if(TermInfo[9] && gApnType==1)memcpy(dialNum,"smartone-vodafone",17);
	else if(TermInfo[9])memcpy(dialNum, "cmnet",5);
	else if(TermInfo[10])memcpy(dialNum, "#777", 4);
	else if(TermInfo[18])memcpy(dialNum, "3GNET", 5);
#endif

}




// flag  0-���� 1-���� 2-ѭ�� 3-�ɹ��� 4-�ӿڰ� 5-����Ƶ��
uchar WnetTest(uchar flag)
{
	long TotalNum,FailNum, DialCnt;
	int i, count, iRet, iVer;
	unsigned char pSignalLevel, pSignalLevelTemp;
	uchar Sim=0,ChkSignal=0,ucRet;
	uchar SimInfo,rsp[2][50];

	//memset(dialNum, 0x00, sizeof(dialNum));
	//memset(uid,0x00,sizeof(uid)); //��ʼ���û���
	//memset(pwd,0x00,sizeof(pwd)); //��ʼ������
	//memcpy(uid,"CARD",4); //�����û���
	//memcpy(pwd,"CARD",4); //��������
	
	SetComPort(WNETPORT); //����wnetͨѶ�ڣ�
	RouteSetDefault(ROUTE_WNET);//����ϵͳȱʡ·��WNET
	kbflush();
	ScrClrLine(2,7);

	SelectApn2(flag);

	if (flag == 0) ScrDispStr(0,2,1,"���߲���","WNET Test");
	if (flag == 1) ScrDispStr(0,0,1,"���β���","Single Test");
	if (flag == 2) ScrDispStr(0,0,1,"ѭ������","Cycle Test ");
	if (flag == 3) ScrDispStr(0,0,1,"�źŲ���","Signal Test");
	if (flag==3){ flag=1;ChkSignal=1;}
	
	TotalNum=0; 
	FailNum=0;
	iVer = 0;

	ScrDispStr(0,2,1, "����ģ�����", "WNET TEST");
	if(flag!=0) WlSwitchPower(1);

	while(1)
	{
		while(2)
		{
			SimInfo = 0;
			if(flag==2)
			{
				ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",++TotalNum);
				if(TotalNum==10000)break;
				if(CancelCheck())break;
			}
			
			for(Sim=0; Sim<gSimNum; Sim++)
			{
				if(gSimNum==2)//˫SIM��
				{
					ScrClrLine(2,7);
					ScrDispStr(0,4,1, "ѡ��SIM��%d", "Select SIM %d",Sim);
					iRet = WlSelSim(Sim);
					if(iRet<0)
					{
						BeepFail();
						sprintf(gErrnoMsg,"WlSelSim(%d)=%d",Sim,iRet);
						ScrDispStr(0,4,1,"ѡ��SIM��%dʧ��!%d", "Select SIM%d fail!%d",Sim,iRet);
						DelayMs(1000);
						if(flag==1 || flag==0 || flag==4) goto ENDERR;	
						if(flag==2)	break;
					}
				}
				
				ScrClrLine(2,7);
				ScrDispStr(0,4,1, "ģ���ʼ��...", "WNET Init...");
				iRet = WlInit(NULL);
				
				if(flag==0)
				{
					SelectApn();
					ScrPrint(0,6,1,dialNum);
				}

#ifdef BCM
				//��ȡSIM����Ӫ�̵���Ϣ
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
						ScrDispStr(0,4,1,"SIM ��Ϣ����!","Sim Info Error");
						ScrPrint(0,6,0,"%d,%s",SimInfo,rsp[0]);
						ScrPrint(0,7,0,"%d,%s",SimInfo,rsp[1]);
						sprintf(gErrnoMsg,"SIM INFO = %d,%s,%s",SimInfo,rsp[0],rsp[1]);
						BeepFail();
						getkey();
						goto ENDERR;
					}
				}

#endif
				if(Sim==1 && flag==0)//1ͨ����������ֻ�жϳ�ʼ��
				{
					if(iRet==RET_OK || iRet==WL_RET_ERR_INIT_ONCE)
					{
						BeepOK();
						ScrDispStr(0,4,1,"SIM%d���Գɹ�!","SIM%d TEST OK!",Sim);
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
				if(iRet==WL_RET_ERR_NOSIM) //flag=4�ǽӿڰ���ԣ�û�в忨
				{
					if(flag!=4)
					{
						sprintf(gErrnoMsg,"WlInit(%d)=%d",Sim,iRet);
						ScrDispStr(0,4,1,"�����SIM��%d", "No SIM Card%d",iRet);
						BeepFail(); 
						getkey();
						if(flag==0 || flag==1 || flag==4)goto ENDERR;
						else  break;
					}
				}
				else if(iRet!=RET_OK && iRet!=WL_RET_ERR_INIT_ONCE)
				{
					sprintf(gErrnoMsg,"WlInit(%d)=%d",Sim,iRet);
					ScrDispStr(0,4,1, "ģ���ʼ��ʧ��:%d", "WNet Init Fail:%d",iRet);
					BeepFail();
					getkey();
					if(flag==0 || flag==1 || flag==4)	goto ENDERR;
					else break;
				}
				
				iRet = GetWirelessVer();

				ScrClrLine(2,7);
				ScrDispStr(0,2,1,"�汾:%s,%s","VERSION:%s,%s",gWireVer,gWnetVer);
				//ScrPrint(0,4,1,"%s,%s",gWireVer,gWnetVer);   //��ʾ�汾��
				if(flag==4 || strstr(InfoOut[C_PN],"S90-MC0-363-01EA")!=NULL)
				{
					ScrDispStr(0,6,1,"���߲��Գɹ�","WNET TEST OK");
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
				ScrDispStr(0,4,1,"��ѯ�ź�ǿ��","Check Signal");					
				iRet=WlGetSignal(&pSignalLevel); //��ѯ�ź�ǿ��
				if(iRet==-202)//��һ���������Գɹ�����Եڶ��Σ����۵��λ���������������-202��û��Ӧ�𣩣�������Ҫ�����ϵ�
				{
					ScrDispStr(0,4,1,"ģ�������ϵ�...","Module Power On");	
					WlSwitchPower(0);
					WlSwitchPower(1);
					WlInit(NULL);
					DelayMs(4000);
					ScrClrLine(4,7);
					ScrDispStr(0,4,1,"��ѯ�ź�ǿ��","Check Signal");	
					
				}
				
				i = 0;
				while(1)
				{
					iRet=WlGetSignal(&pSignalLevel); //��ѯ�ź�ǿ��
					if(pSignalLevel<=4 || CancelCheck())break;
					if(flag==0 && i==5) break; //����
					if(flag!=0 && i==50) break;//�������������
					
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
					ScrDispStr(0,4,1,"���ź�ʧ��:%d","Check Signal:%d",iRet);
					if(WaitEsc(0,1))
					{
						if(flag==1 || flag==0)goto ENDERR;
						else goto ENDTEST;//TotalNum--;
					}
				}
				
				ScrClrLine(4,7);
				switch(pSignalLevel) {
				case 0:ScrDispStr(0,4,1,"�ź���ǿ","Signal Highest");break;
				case 1:ScrDispStr(0,4,1,"�źŽ�ǿ","Signal Higher");break;
				case 2:ScrDispStr(0,4,1,"�ź�һ��","Signal Normal");break;
				case 3:ScrDispStr(0,4,1,"�źŽ���","Signal Weak");break;
				case 4:ScrDispStr(0,4,1,"�źź���","Signal Weaker");break;
				default:ScrDispStr(0,4,1,"���ź�","No Signal");break;
				}			
				
				pSignalLevelTemp = pSignalLevel;
				pSignalLevelTemp = (pSignalLevelTemp <= 5)? (5 - pSignalLevelTemp):0;//�����ź�ǿ�ȵ���ͼ��
				ScrSetIcon(2, (unsigned char)(1+pSignalLevelTemp));
				
				if(WaitEsc(0,1))
				{
					if(flag==1 || flag==0)goto ENDERR;
					else goto ENDTEST;//TotalNum--;
				}
			
				ScrClrLine(4,7);
				
				if(flag==0 || flag==1) DialCnt=3; //�����뵥�Σ����ΰκŻ���
				else DialCnt=1;//


				if(strstr((char *)gWnetVer, "EM701"))//�����û��PPP���ӵ�,ֻ�ܸ����ź�ֵ�ж�
				{
					ScrClrLine(4,7);
					ScrPrint(0,4,1,"Signal Value = %d", pSignalLevel);
					sprintf(gErrnoMsg,"SignalValue=%d",pSignalLevel);
					DelayMs(1000);
					if(pSignalLevel>3) //0~5 Խ���ʾ�ź�Խ��
						goto ENDERR;
				}
				else
				{
					ScrClrLine(4,7);
					for(count=0; count<DialCnt; count++)  // ���ΰκŻ���
					{
						ScrDispStr(0,4,1,"����PPP����","PPP Connecting");
						if(!(count%3)) ScrPrint(0,6,1,".....");
						else if((count%3)==1) ScrPrint(0,6,1,"..........");
						else if((count%3)==2) ScrPrint(0,6,1,"...............");
						if(CancelCheck())break;

						//2009-04-14 ��ʾһֱ�ȴ�������PPP��·�ɹ���ʧ��,��ֹ�˳�����ģ����ԣ�������ռ��
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
						ScrDispStr(0,4,1,"PPP����ʧ��:%d","PPP fail:%d",iRet);
					}

					ScrDispStr(0,6,1,"�Ͽ�PPP����","PPP Disconnect");
					WlPppLogout();
					while( WlPppCheck()>=0 );  //�ȴ���·�Ͽ�			
					
				}

				if(iRet==0)
				{
					BeepOK();
					ScrDispStr(0,6,1,"SIM%d���Գɹ�!","SIM%d TEST OK!",Sim);					
					if(WaitEsc(0,1))
					{
						if(flag==2)goto ENDTEST;//TotalNum--;
					}
					if(flag==1 || flag==0)
					{
						if(gSimNum==2 && Sim==0) continue;//˫SIM��
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
						ScrDispStr(0,6,1,"SIM%d����ʧ��!","SIM%d TEST Fail!",Sim);
						DelayMs(1500);
						goto ENDERR;
					}
					else if(flag == 2)
					{
						continue;// for(Sim=0; Sim<=SimSlot; Sim++)
					}
					
				}
			}//for(Sim=0; Sim<=SimSlot; Sim++)
			if(iRet)FailNum++;//for����break��͵������ж�
			
		}//while(2)
ENDTEST:
		ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",TotalNum);
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


void SwitchBand(void)
{
	uchar uckey;
	int iRet;
	uchar Imei[50];
	GetWirelessVer();
	ScrClrLine(2,7);
	if(strstr((char *)gWnetVer,"Q24")==NULL)						
	{
		ScrDispStr(0,4,1, "û���л�Ƶ�ι���", "No Band Module");
		getkey();
		return;
	}

	WlSwitchPower(1);
	ScrDispStr(0,4,1,"ģ���ʼ��...","WNet Init..");
	iRet=WlInit(NULL);
	if(iRet != WL_RET_ERR_INIT_ONCE)
	{
		WaitEsc(0, 10);
	}

	ScrClrLine(2,7);
	WlClosePort();
	iRet=WlOpenPort();   //������ͨѶ����
	if(iRet)
	{
		BeepFail();
		ScrPrint(0,4,1,"��ͨ�Ŵ���ʧ��:%d","Open Port Err:  %d",iRet);
		WaitEsc(0, 2);
		return;
	}
	ScrClrLine(2,7);
	ScrDispStr(0,2,1, "��ȡƵ��...","Get Band");
	WlSendCmd((unsigned char*)"at+wmbs?\r", Imei, 100, 1000, 300);
	if(strstr(Imei, (uchar *)"+WMBS: 5,")!=NULL)
	{
		ScrDispStr(0,4,1,"��ǰƵ��:900-1800","Current Band: 900-1800");
	}
	else if(strstr(Imei, (uchar *)"+WMBS: 4,")!=NULL)
	{
		ScrDispStr(0,4,1,"��ǰƵ��:\n850-1900","Current Band: 850-1900");
	}
	if(getkey()==KEYCANCEL) return;
	ScrClrLine(2, 7);	
	ScrDispStr(0,2,1,"������Ƶ��\n1-900-1800\n2-850-1900", "Pls Set Bands\n1-900-1800\n2-850-1900");
	uckey=getkey();
	if(uckey == KEY1)
	{
		iRet=WlSendCmd((unsigned char*)"at+wmbs=5,1\r", Imei, 100, 1000, 300);
	}
	else if(uckey == KEY2)
	{
		iRet=WlSendCmd((unsigned char*)"at+wmbs=4,1\r", Imei, 100, 1000, 300);
	}
	else 
	{
		return;
	}
	
	ScrClrLine(2,7);
	if(strstr(Imei, (uchar *)"OK")!=NULL)
	{
		ScrDispStr(0,4,1,"����Ƶ�γɹ�","Set Bands OK!");
		WlSendCmd((unsigned char*)"at+wmbs?\r", Imei, 100, 1000, 300);
		if(strstr(Imei, (uchar *)"+WMBS: 5,")!=NULL)
		{
			ScrDispStr(0,4,1,"��ǰƵ��:\n900-1800","Current Band: 900-1800");
		}
		else if(strstr(Imei, (uchar *)"+WMBS: 4,")!=NULL)
		{
			ScrDispStr(0,4,1,"��ǰƵ��:\n850-1900","Current Band: 850-1900");
		}
	}
	else 
	{
		ScrDispStr(0,4,1,"����Ƶ��ʧ��","Set Bands Fail!");
	}
	if(getkey() == KEYCANCEL)
	{
		return;
	}
}





//Sim=0,����SIM0     //Sim=1,����SIM1
uchar WnetSimpleTest(uchar Sim)
{
	int iRet,i;
	uchar Exitflag=0;
	uchar rsp_wnet[300],*p,Csq,nCsq[5];

	TimerSet(0,1000);
	memset(nCsq,0,sizeof(nCsq));
	ScrClrLine(2,7);
	if(Sim==0)
	{	
		ScrPrint(0,4,1,"WNET POWER ON");
		
		WlSwitchPower(1);
		RecordLog(WNET_TEST_RECORD,"WlSwitchPower(1) time=%d\n",1000-TimerCheck(0));
		ComSends("WlSwitchPower(1) time=%d\n",1000-TimerCheck(0));
		if(gSimNum==2)
		{
			ScrPrint(0,4,1,"WlSelSim(0)...");
			iRet=WlSelSim(0);
			ComSends("WlSelSim(0)=%d, time=%d\n",iRet,1000-TimerCheck(0));
			RecordLog(WNET_TEST_RECORD,"WlSelSim(0)=%d, time=%d\n",iRet,1000-TimerCheck(0));
			if(iRet)
			{
				BeepFail();
				ScrPrint(0,4,1,"WlSelSim(0)=%d",iRet);
				getkey();
				goto ENDERR;
			}
		}
		
		
		ScrPrint(0,4,1,"WlInit(NULL)...");
		iRet = WlInit(NULL);
		RecordLog(WNET_TEST_RECORD,"WlInit(NULL)=%d,time=%d\n",iRet,1000-TimerCheck(0));
		ComSends("WlInit(NULL)=%d,time=%d\n",iRet,1000-TimerCheck(0));
		if(iRet!=RET_OK && iRet!=WL_RET_ERR_INIT_ONCE)
		{
			BeepFail();
			ScrPrint(0, 4, 0x01,"WlInit Fail%d ",iRet);
			getkey();
			return iRet;
		}
		


		GetWnetVerFlag=0;//0��ʾ��û��ȡ�汾��Ϣ����Ҫ���»�ȡ�汾��Ϣ
		iRet=GetWirelessVer();
		RecordLog(WNET_TEST_RECORD,"GetWirelessVer()=%d,time=%d\n",iRet,1000-TimerCheck(0));
		ComSends("GetWirelessVer()=%d,time=%d\n",iRet,1000-TimerCheck(0));
		
		RecordLog(WNET_TEST_RECORD,"VER=%s,%s\n",gWnetVer,gWireVer);
		ComSends("VER=%s,%s\n",gWnetVer,gWireVer);
		

		if(iRet)
		{
			BeepFail();
			ScrPrint(0,4,1,"GetWnetVer=%d",iRet);
			getkey();
			goto ENDERR;
		}
		ScrPrint(0,4,1,"VER:%s",gWnetVer);

		
		TimerSet(1,300);
		while(TimerCheck(1))
		{
			memset(rsp_wnet,0,sizeof(rsp_wnet));
			iRet=WlSendCmd((unsigned char*)"at+CSQ\r", rsp_wnet, 100, 30000, 1000);
			RecordLog(WNET_TEST_RECORD,"%s",rsp_wnet);
			ComSends("%s",rsp_wnet);
			if(strstr(rsp_wnet,"CSQ: 99")==NULL && strstr(rsp_wnet,"CSQ:99")==NULL 
				&& strstr(rsp_wnet,"CSQ: 0")==NULL && strstr(rsp_wnet,"CSQ:0")==NULL)
			{
				i=0;
				memset(nCsq,0,sizeof(nCsq));
				while(TimerCheck(1))
				{
					memset(rsp_wnet,0,sizeof(rsp_wnet));
					iRet=WlSendCmd((unsigned char*)"at+CSQ\r", rsp_wnet, 100, 30000, 100);
					RecordLog(WNET_TEST_RECORD,"%s",rsp_wnet);
					ComSends("%s",rsp_wnet);
					if(strstr(rsp_wnet,"CSQ: 99")==NULL && strstr(rsp_wnet,"CSQ:99")==NULL 
						&& strstr(rsp_wnet,"CSQ: 0")==NULL && strstr(rsp_wnet,"CSQ:0")==NULL)
					{
						p=strstr(rsp_wnet,"CSQ:");
						p=p+4;
						nCsq[i] = atoi(p);
						i++;
						if(i==5)
						{
							Exitflag=1;
							break;
						}
					}
					else
					{
						memset(nCsq,0,sizeof(nCsq));
						i=0;
					}
				
				}
				
			}
			if(Exitflag==1)break;
		}			
		RecordLog(WNET_TEST_RECORD,"Get CSQ Time=%d\n",300-TimerCheck(1));
		ComSends("Get CSQ Time=%d\n",300-TimerCheck(1));
		RecordLog(WNET_TEST_RECORD,"at+CSQ=%d,time=%d\n",iRet,1000-TimerCheck(0));
		ComSends("at+CSQ=%d,time=%d\n",iRet,1000-TimerCheck(0));

		if(iRet || nCsq[4]==0)
		{
			BeepFail();
			RecordLog(WNET_TEST_RECORD,"CSQ ERROR=%d,%d,%d,%d,%d,%d\n",iRet,nCsq[0],nCsq[1],nCsq[2],nCsq[3],nCsq[4]);
			ComSends("CSQ ERROR=%d,%d,%d,%d,%d,%d\n",iRet,nCsq[0],nCsq[1],nCsq[2],nCsq[3],nCsq[4]);
			ScrPrint(0,4,1,"CSQ ERROR=%d,%d,%d,%d,%d,%d\n",iRet,nCsq[0],nCsq[1],nCsq[2],nCsq[3],nCsq[4]);
			goto ENDERR;
		}
		Csq = (nCsq[0]+nCsq[1]+nCsq[2]+nCsq[3]+nCsq[4])/5;
		ScrClrLine(2,7);
		ScrPrint(0,4,1,"CSQ= %d ",Csq);
		RecordLog(WNET_TEST_RECORD,"CSQavg = %d\n",Csq);
		ComSends("CSQavg = %d\n",Csq);
		if(Csq>=15 && Csq!=99)
		{
			ScrPrint(0,6,1,"SIM0 Test OK");
			RecordLog(WNET_TEST_RECORD,"SIM0 Test OK");
			ComSends("SIM0 Test OK\n");
			BeepOK();
			goto  ENDOK;
		}
		else
		{
			ScrPrint(0,6,1,"SIM0 Test Fail");
			BeepFail();
			goto ENDERR;
		}

	}
	else if(Sim==1 && gSimNum==2)
	{
		ScrPrint(0,4,1,"WlSelSim(1)...");
		iRet=WlSelSim(1);//���������ж���û�г�ʼ������û�в忨
		ComSends("WlSelSim(1)=%d, time=%d\n",iRet,1000-TimerCheck(0));
		RecordLog(WNET_TEST_RECORD,"WlSelSim(1)=%d, time=%d\n",iRet,1000-TimerCheck(0));
		if(iRet==0)
		{
			ScrPrint(0,4,1,"WlInit(NULL)...");
			iRet = WlInit(NULL);
			RecordLog(WNET_TEST_RECORD,"WlInit(SIM1)=%d,time=%d\n",iRet,1000-TimerCheck(0));
			ComSends("WlInit(SIM1)=%d,time=%d\n",iRet,1000-TimerCheck(0));
			if(iRet!=RET_OK && iRet!=WL_RET_ERR_INIT_ONCE)
			{
				BeepFail();
				ScrPrint(0, 4, 0x01,"WlInit Fail%d ",iRet);
				getkey();
				return iRet;
			}
			else
			{
				ScrPrint(0,4,1,"SIM1 Test OK ");
				BeepOK();
				goto ENDOK;
			}
		}
		else
		{
			ScrPrint(0,4,1,"WlSelSim(1)=%d",iRet);
			BeepFail();
			goto ENDERR;
		}
	
	}

ENDOK:
	//WlSwitchPower(0);
	RECORD(1,"WNET=1");
	RECORD(1,"\r\n");	
	return 0;
ENDERR:
	//WlSwitchPower(0);
	RecordLog(WNET_TEST_RECORD,"SIM0 Test Fail");
	ComSends("SIM0 Test Fail\n");
	RECORD(1,"WNET=0");
	RECORD(1,"\r\n");
	return 1;
}


uchar TestSim0 (uchar flag)
{
	return WnetSimpleTest(0);
}

uchar TestSim1 (uchar flag)
{
	return WnetSimpleTest(1);
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
void WnetTest3(void)
{
	WnetTest(3);
}

void WnetCsqTest(void)
{
	WnetSimpleTest(0);
	WnetSimpleTest(1);
}

void WnetCsqCyc(void)
{
	uint TotalNum=0,FailNum=0;
	uchar ucRet1=0,ucRet2=0;
	ComSends("\n\nStart��%s\n",DispTimer(7));
	RecordLog(WNET_TEST_RECORD,"\n\nStart��%s\n",DispTimer(7));
	while(1)
	{
		while(2)
		{
			ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",++TotalNum);
			if(CancelCheck())break;
			ucRet1=WnetSimpleTest(0);
			if(gSimNum==2)
				ucRet2=WnetSimpleTest(1);
			if(ucRet1 || ucRet2) FailNum++;
			ComSends("\n\nTotalNum=%d,FailNum=%d,\n\n",TotalNum,FailNum);
			RecordLog(WNET_TEST_RECORD,"\n\nTotalNum=%d,FailNum=%d,\n\n",TotalNum,FailNum);
			if(WaitEsc(0,15))break;
		}
		ShowSuccessRate(TotalNum,FailNum);
		kbflush();
		if(getkey()==KEYCANCEL) break;
	}
	
	ComSends("END��%s\n\n",DispTimer(7));
	RecordLog(WNET_TEST_RECORD,"END��%s\n\n",DispTimer(7));

}

void HongkongSelectApn(void)
{
	uchar ucRet;
	ScrClrLine(2,7);
	ScrDispStr(0,2,1,"ѡ��APN %d","Select APN %d",gApnType);
	ScrPrint(0,4,0,"1:smartone-vodafone\n2:cmnet");
	while(1)
	{
		ucRet = getkey();
		if(ucRet==KEY1)
		{
			gApnType=1;
			ScrClrLine(4,5);
			ScrPrint(0,5,0,"smartone-vodafone");
			getkey();
			break;
		}
		else if(ucRet==KEY2)
		{
			gApnType=0;
			ScrClrLine(4,5);
			ScrPrint(0,5,0,"cmnet");
			getkey();
			break;
		}
	}

}

void dial_un_hangoff(void)
{
	AntennaTest(1);	 //���Զ��Ҷ�
}

void dial_hangoff(void)
{
	AntennaTest(0);  //�Զ��Ҷ�
}

uchar cmu_test(uchar flag)
{
	return AntennaTest(2);
}


void WnetTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-����",		"1-Single",		WnetTest1},
		{"2-ѭ��",		"2-Cycle ",		WnetTest2},
		{"3-ת��",		"3-Relay ",		WNetUpdate},
		{"4-����",		"4-Atena ",		dial_hangoff},
		{"5-ѡAPN",		"5-APN",		HongkongSelectApn},
		{"6-����1",		"6-Atena1",		dial_un_hangoff},
		{"1-����",		"1-Ring",		RingTest},
		{"2-����",		"2-Wnet0",		WnetTest0},
	};
	
	if(TestModule[T_WNET]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"����������ģ��", "No WNET Module");
		getkey();
		return ;
	}

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"���ߵ������","WNET Test",2);
	

}


extern void bt_slave_test(void);
void wnet_test(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-����",		"1-dail",		dial_hangoff},
		{"2-����",		"2-answer",		RingTest},
	};
	
	if(TestModule[T_WNET]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"������ģ��", "No Wireless");
		getkey();
		return ;
	}

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"����ģ�����","wireless test",3);
}

void test_bid(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-����ģ��",	"1-Wireless",		wnet_test},
		{"2-����",		"2-Bluetooth",		bt_slave_test},
	};
	

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"���ܼ��","function test",3);
}

