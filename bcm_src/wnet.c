#include "public.h"

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
	if(TermInfo[9] && 0)memcpy(dialNum,"smartone-vodafone",17);
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
		{"1-����",		"1-Single",		WnetTest1},
		{"2-ѭ��",		"2-Cycle ",		WnetTest2},
		{"3-ת��",		"3-Relay ",		WNetUpdate},
		{"1-����",		"1-Wnet0",		WnetTest0},
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
