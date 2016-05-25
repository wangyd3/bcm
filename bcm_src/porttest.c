#include "porttest.h"
#ifdef PORT_TEST

/************************************************************************
   ÿ������һ��ֻ��Ҫ��p_test_list����
   ͬʱ���Ӻ�����ԭ��,ֻ��Ҫע�� p_test_model �Ƿ�����
************************************************************************/


TEST_ITEM p_test_list[]=
{
	{0,0,"IC",			p_ic_test},
	{0,0,"SAM",			p_sam_test},
	{0,0,"RS232A",		p_rs232a_test},
	{0,0,"RS232B",		p_rs232b_test},
	{0,0,"PINPAD",		p_rs232p_test},
	{0,0,"A TO B",		p_a2b_test},
	{0,0,"A TO P",		p_a2p_test},
	{0,0,"B TO P",		p_b2p_test},
	{0,0,"MODEM",		p_modem_test},
	{0,0,"LAN",			p_lan_test},
	{0,0,"WNET",		p_wnet_test},
	{0,0,"WIFI",		p_wifi_test},
	{0,0,"UDISK",		p_udisk_test},
	{0,0,"UDEV",		p_udev_test},
	{0,0,"SCAN",		p_scan_test},
	{0,0,"BAT",			p_bat_test},
	{0,0,"PRN",			p_prn_test},
	{0,0,"BEEP",		p_beep_test},
	{0,0,"AUDIO",		p_audio_test},
	{0,0,"KEY",			p_key_test},
	{0,0,"MAGCARD",		p_msr_test},
	{0,0,"RF",			p_picc_test},
	{0,0,"BYPHONE",		p_byph_test},
	{0,0,"LED",			p_led_test},
	{0,0,"HEARER",		p_hearer_test},
	{0,0,"BT",			p_bt_test},
	
};

uchar p_bt_test(uchar flag)
{
	int ret = BtOpen();
	BtClose();
	if(ret)
	{
		ScrPrint(0,2,1,"BT FAIL = %d",ret);
		BeepFail();
	}
	else
	{
		ScrPrint(0,2,1,"BT TEST OK");
		BeepOK();
	}
	DelayMs(500);
	return (ret? 1 : 0);
}

uchar p_hearer_test(uchar flag)
{

    TEL_STATUS  tel_status;
	uchar HandlePicked_tmp;
	ScrCls();
	if(TestModule[T_WNET]=='Y')//���ߵ绰ģ��
	{
		ScrPrint(0,2,1,"WlInit...");
		WlInit(NULL);
		ScrCls();
	}
	kbflush();
	OnHook();
	ScrDispStr(0,2,1,"����Ͳ,��������?","PickUp,HearOk?");
	ScrDispStr(0,4,1,"[ȷ��]-��","[ENTER]-YES");
	ScrDispStr(0,6,1,"[����]-��","[OTHER]-NO ");
	
	memset(&tel_status,0,sizeof(tel_status));
    TelCheck(&tel_status);
	HandlePicked_tmp=tel_status.HandlePicked;
	while(1)
	{
		TelCheck(&tel_status);
		if(tel_status.HandlePicked!=HandlePicked_tmp)//�ֱ��绰ժ��
		{
			   HandlePicked_tmp=tel_status.HandlePicked;
			   if(tel_status.HandlePicked==1)TelOffHook();
		}

		if (!kbhit())
		{
			OnHook();
			if(getkey()==KEYENTER)
				return 0;
			else 
				return 1;
		}
		
	}
}



uchar p_ic_test(uchar flag)
{
	uchar ucRet,IcNumTmp,SamNumTmp;
	IcNumTmp = gIcNum;
	SamNumTmp = gSamNum;

	gIcNum = 1;
	gSamNum = 0;
	ucRet = IcTest(0);
	
	gIcNum = IcNumTmp;
	gSamNum = SamNumTmp;
	return ucRet;
}

uchar p_sam_test(uchar flag)
{
	uchar ucRet,IcNumTmp,SamNumTmp;
	IcNumTmp = gIcNum;
	SamNumTmp = gSamNum;

	gIcNum = 0;
	gSamNum = p_sam_num;
	ucRet = IcTest(0);
	
	gIcNum = IcNumTmp;
	gSamNum = SamNumTmp;
	return ucRet;
}


uchar port2port(uchar portno1,uchar portno2)
{   
	ushort stringlen;
	uchar inbuff[200];
	uchar outbuff[]="Hello everyone in PAX! Thanks for all of your hard work!";


	stringlen=strlen(outbuff);
	ScrClrLine(2, 7);
	PortOpen(portno1,"115200,8,n,1");
	PortOpen(portno2,"115200,8,n,1");

	memset(inbuff,0x00,sizeof(inbuff));
	PortSends(portno1,outbuff,stringlen);	
	PortRecvs(portno2,inbuff,stringlen,500);
	if(strcmp(outbuff,inbuff))goto ENDERR;
	

	memset(inbuff,0x00,sizeof(inbuff));
	PortSends(portno2,outbuff,stringlen);	
	PortRecvs(portno1,inbuff,stringlen,500);
	if(strcmp(outbuff,inbuff))goto ENDERR;
	

ENDOK:
	PortClose(portno1);
	PortClose(portno2);
	ScrDispStr(0,4,1,"����ͨ�� %d %d ","PORT OK %d %d ",portno1,portno2);
	BeepOK();
	DelayMs(500);
    return 0;
ENDERR:
	PortClose(portno1);
	PortClose(portno2);
	ScrDispStr(0,4,1,"����ʧ�� %d %d ","PORT FAIL %d %d ",portno1,portno2);
	BeepFail();
	getkey();
    return 1;
}



uchar p_rs232a_test(uchar flag)
{
	return( port2port(RS232A,RS232A) );
}

uchar p_rs232b_test(uchar flag)
{
	return( port2port(RS232B,RS232B) );
}

uchar p_rs232p_test(uchar flag)
{
	return( port2port(PINPAD,PINPAD) );
}

uchar p_a2b_test(uchar flag)
{
	return( port2port(RS232A,RS232B) );
}

uchar p_a2p_test(uchar flag)
{
	return( port2port(RS232A,PINPAD) );
}

uchar p_b2p_test(uchar flag)
{
	return( port2port(RS232B,PINPAD) );
}

uchar p_modem_test(uchar flag)
{
	return ModemTest(0);
}
uchar p_lan_test(uchar flag)
{
	return LanTest(0);
}
uchar p_wnet_test(uchar flag)
{
	return WnetTest(4);
}
uchar p_wifi_test(uchar flag)
{
	int iRet;
	ScrClrLine(2,7);
	ScrDispStr(0,2,1,"WIFI����","WIFI Test");
	iRet = WifiOpen();
	WifiClose();
	if(iRet)
	{
		ScrDispStr(0,4,1,"WIFIʧ�� %d","WIFI FAIL %d",iRet);
		BeepFail();
		getkey();
		return 1;
	}
	else
	{
		ScrDispStr(0,4,1,"WIFI���Գɹ�","WIFI TEST OK");
		BeepOK();
		DelayMs(500);
		return 0;
	}
	
}
uchar p_udisk_test(uchar flag)
{
	return UDiskTest(0);
}
uchar p_udev_test(uchar flag)
{
	return UsbDevTest(0);
}
uchar p_scan_test(uchar flag)
{
	return ScanTest(0);
}

uchar p_bat_test(uchar flag)
{
	uchar level;
	ushort Time;
	ScrClrLine(2,7);
	ScrDispStr(0,2,1,"����ģ�����","Power Test");
	level=BatteryCheck();
    ScrDispStr(0,4,1,"��ѹ�ȼ�:%d","Voltage :%d",level);
	if(level <= 4){
		goto ENDERR;
	}
	TimerSet(3,150);
	while(1)
	{
		level=BatteryCheck();
		Time=TimerCheck(3);
		ScrDispStr(0,6,1,"������ %d ","Close Power %d",Time);
		if( level<=4 )
		{
			ScrDispStr(0,4,1,"��ѹ�ȼ�:%d","Voltage :%d",level);
			goto ENDOK;
		}
		if(Time==0 || CancelCheck())goto ENDERR;
	}

ENDERR:
	BeepFail();
	ScrDispStr(0,6,1,"    ����ʧ��    ","   Test Fail   ");
	DelayMs(500);
	return 1;

ENDOK:
	BeepOK();
	ScrDispStr(0,4,1,"    ���Գɹ�    ","    Test OK   ");
	DelayMs(500);
	return 0;

}
uchar p_prn_test(uchar flag)
{
	return PrinterTest(0);

}
uchar p_beep_test(uchar flag)
{
	return BeepTest(1);
}

uchar p_audio_test(uchar flag)
{
	return SpeechTest(0);
}
uchar p_key_test(uchar flag)
{
	return KbTest(0);
}
uchar p_msr_test(uchar flag)
{
	return MagTest(0);
}

uchar p_picc_test(uchar flag)
{
	return RfTest(0);
}

uchar p_byph_test(uchar flag)
{
	return ByPhoneTest(0);
}

uchar p_led_test(uchar flag)
{
	return LedTest(0);
}


void p_get_config(void)
{
	int fid,i;
	fid = fexist(PORT_PARA);      
    if(fid>0)
	{
		fid=open(PORT_PARA,O_CREATE|O_RDWR);
		if(fid<0)ShowFileCtrlErr(__LINE__);   //��ȡ�ļ��������������
		if( read(fid,p_test_model,sizeof(p_test_model)) != sizeof(p_test_model))
			ShowFileCtrlErr(__LINE__);
		close(fid);
	}
	else
	{
		memset(p_test_model,'N',sizeof(p_test_model));
		fid=open(PORT_PARA,O_CREATE|O_RDWR);
		if(fid<0)ShowFileCtrlErr(__LINE__);   //��ȡ�ļ��������������
		if( write(fid,p_test_model,sizeof(p_test_model)) != sizeof(p_test_model))
			ShowFileCtrlErr(__LINE__);
		close(fid);
	}

	for(i=0;i<p_test_num;i++)
	{
		p_test_list[i].flag = p_test_model[i];
	}
	p_sam_num = p_test_list[P_SAM].flag - 0x30;


}



void p_set_single(uchar id)
{

	if(id==P_SAM) //��ʾN ��1��5
	{
		
		if(p_test_list[id].flag<'1' || p_test_list[id].flag>'5')p_test_list[id].flag='1';
		else p_test_list[id].flag++;
		if(p_test_list[id].flag=='6')p_test_list[id].flag='N';

	}
	else
	{
		if(p_test_list[id].flag == 'N')
			p_test_list[id].flag = 'Y';
		else p_test_list[id].flag = 'N';
	}
}



void p_set_config(void)
{
	int set_flag = 0;
	int fid;
	int i,id,page,page_num,num;
	uchar ucRet;

	DelayMs(1000);
	while(!kbhit())
	{
		if(getkey()==KEY9)set_flag=1;
	}
	if(set_flag == 0)return;
	
	p_get_config();
	
	num = 6;//ÿҳ��ʾ6��
	page_num = p_test_num/num + 1;//��ҳ��
	if( (p_test_num % num) == 0 ) page_num--;//���������Ҫ-1
	id=0;//��0��ʼ
	page=0;//��0��ʼ
	
	while(1)
	{
		kbflush();
		ScrCls();
		page=id/num;//��ǰҳ
		ScrDispStr(0,0,1,"���ò����� %d/%d","Set Module %d/%d",page+1,page_num);

		for(i=0;i<num;i++)
		{
			if(i+page*num >= p_test_num)break;
			ScrPrint(0,	 i+2,0," %d-%s",i+1,p_test_list[i+page*num].itemStr);
			ScrPrint(100,i+2,0," %c ",p_test_list[i+page*num].flag);
		}

		if(TermInfo[0]==T610)
		{
			i = id%num;
			ScrPrint(0,	 i+2,0x80," %d-%s",i+1,p_test_list[i+page*num].itemStr);
			ScrPrint(100,i+2,0x80," %c ",p_test_list[i+page*num].flag);
		}		

		ucRet=getkey();
		if(ucRet==KEYCANCEL) break;
		else if(ucRet==KEYUP && TermInfo[0]==T610)id++;
		else if(ucRet==KEYDOWN && TermInfo[0]==T610)id--;
		else if(ucRet==KEY8 || ucRet==KEYF1 || ucRet==KEYUP || ucRet==FNKEYUP)id=id-num;
		else if(ucRet==KEY0 || ucRet==KEYF2 || ucRet==KEYDOWN || ucRet==FNKEYDOWN)id=id+num;
		else if(ucRet==KEYENTER)
		{
			if(TermInfo[0]==T610)p_set_single(id);
			else id=id+num;
		}
		else if( ucRet>=KEY1 && ucRet<=(KEY0+num) )
		{
			i = page*num + ucRet - KEY1;
			if(i>=p_test_num)BeepFail();
			else p_set_single(i);
		}
		if(id<0)id=p_test_num-1;
		if(id>=p_test_num)id=0;
	
	}
	
	for(i=0;i<p_test_num;i++)
	{
		p_test_model[i] = p_test_list[i].flag;
	}

	fid = open(PORT_PARA,O_RDWR);
	if(fid<0)ShowFileCtrlErr(__LINE__);
	if( write(fid,p_test_model,sizeof(p_test_model)) != sizeof(p_test_model) )
		ShowFileCtrlErr(__LINE__);
	close(fid);	
}



void p_port_test(void)
{
	int test_num,times,ok_num;
	uchar i,j,k,ucRet;
	p_test_num = sizeof(p_test_list)/sizeof(p_test_list[0]);
	p_set_config();
	p_get_config();

	
	test_num = 0;
	ok_num = 0;
	for(i=0;i<p_test_num;i++)
	{
		if(p_test_list[i].flag != 'N')
		{
			ScrCls();
			ScrDispStr(0,0,0x81,"   �ӿڰ����   ","   PORT TEST    ");
			times=0;
			test_num++;
			while(1)
			{
				times++;
				p_test_list[i].result=0; //����ǰ�ѽ������Ϊ���Բ�ͨ��
				ucRet=p_test_list[i].TestFun(0);
				if(ucRet==0)
				{
					p_test_list[i].result=1;
					ok_num++;
					break;
				}
				else
				{
					if(times<2)
					{  
						ucRet=Select();
						if(ucRet==KEY1) continue;
						if(ucRet==KEYCANCEL) goto RESULT;
					}
				}
				break;
			}//1			
		}//if(TestItem[i].flag='Y')
	}//for


RESULT:
	ScrCls();
    kbflush();
	ScrDispStr(0,0,0x81,"   �ӿڰ����   ","   PORT TEST    ");
	ScrDispStr(0,2,1,"����%d�����",  "%dtests in total",test_num);
	    
    if(ok_num >= test_num)//
    {
		ScrDispStr(0,4,1,"%d�����ȫ��ͨ��","%d tests all pass",test_num);
		BeepOK();
    }
    else
    {
        BeepFail(); 
        ScrClrLine(2,7);
		for(i=0,j=0,k=0;i<p_test_num;i++)
		{
			
			if(p_test_list[i].flag != 'N')
			{
				ScrPrint(20,j+2,0,"%s:",p_test_list[i].itemStr);
				if(p_test_list[i].result==0)ScrPrint(64,j+2,0,"FAIL");
				else  ScrPrint(64,j+2,0,"OK");
				
				if(++j==6 && test_num > 6 )
				{
					getkey();
					j=0;
					ScrClrLine(2,7);
				}
				if(++k >= test_num)break;//������;�˳�,��ʱ��test_num��������Ҫ���Ե���Ŀ��,Ӧ���ǲ��Թ�����Ŀ��
			}
		}

    }
    getkey();


}

#endif
