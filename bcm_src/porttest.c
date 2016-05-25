#include "porttest.h"
#ifdef PORT_TEST

/************************************************************************
   每次增加一项只需要在p_test_list增加
   同时增加函数的原型,只需要注意 p_test_model 是否会溢出
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
	if(TestModule[T_WNET]=='Y')//无线电话模块
	{
		ScrPrint(0,2,1,"WlInit...");
		WlInit(NULL);
		ScrCls();
	}
	kbflush();
	OnHook();
	ScrDispStr(0,2,1,"拿听筒,听到声音?","PickUp,HearOk?");
	ScrDispStr(0,4,1,"[确认]-是","[ENTER]-YES");
	ScrDispStr(0,6,1,"[其他]-否","[OTHER]-NO ");
	
	memset(&tel_status,0,sizeof(tel_status));
    TelCheck(&tel_status);
	HandlePicked_tmp=tel_status.HandlePicked;
	while(1)
	{
		TelCheck(&tel_status);
		if(tel_status.HandlePicked!=HandlePicked_tmp)//手柄电话摘机
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
	ScrDispStr(0,4,1,"串口通过 %d %d ","PORT OK %d %d ",portno1,portno2);
	BeepOK();
	DelayMs(500);
    return 0;
ENDERR:
	PortClose(portno1);
	PortClose(portno2);
	ScrDispStr(0,4,1,"串口失败 %d %d ","PORT FAIL %d %d ",portno1,portno2);
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
	ScrDispStr(0,2,1,"WIFI测试","WIFI Test");
	iRet = WifiOpen();
	WifiClose();
	if(iRet)
	{
		ScrDispStr(0,4,1,"WIFI失败 %d","WIFI FAIL %d",iRet);
		BeepFail();
		getkey();
		return 1;
	}
	else
	{
		ScrDispStr(0,4,1,"WIFI测试成功","WIFI TEST OK");
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
	ScrDispStr(0,2,1,"供电模块测试","Power Test");
	level=BatteryCheck();
    ScrDispStr(0,4,1,"电压等级:%d","Voltage :%d",level);
	if(level <= 4){
		goto ENDERR;
	}
	TimerSet(3,150);
	while(1)
	{
		level=BatteryCheck();
		Time=TimerCheck(3);
		ScrDispStr(0,6,1,"请关外电 %d ","Close Power %d",Time);
		if( level<=4 )
		{
			ScrDispStr(0,4,1,"电压等级:%d","Voltage :%d",level);
			goto ENDOK;
		}
		if(Time==0 || CancelCheck())goto ENDERR;
	}

ENDERR:
	BeepFail();
	ScrDispStr(0,6,1,"    测试失败    ","   Test Fail   ");
	DelayMs(500);
	return 1;

ENDOK:
	BeepOK();
	ScrDispStr(0,4,1,"    测试成功    ","    Test OK   ");
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
		if(fid<0)ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
		if( read(fid,p_test_model,sizeof(p_test_model)) != sizeof(p_test_model))
			ShowFileCtrlErr(__LINE__);
		close(fid);
	}
	else
	{
		memset(p_test_model,'N',sizeof(p_test_model));
		fid=open(PORT_PARA,O_CREATE|O_RDWR);
		if(fid<0)ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
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

	if(id==P_SAM) //显示N 和1～5
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
	
	num = 6;//每页显示6项
	page_num = p_test_num/num + 1;//总页数
	if( (p_test_num % num) == 0 ) page_num--;//如果整除需要-1
	id=0;//从0开始
	page=0;//从0开始
	
	while(1)
	{
		kbflush();
		ScrCls();
		page=id/num;//当前页
		ScrDispStr(0,0,1,"设置测试项 %d/%d","Set Module %d/%d",page+1,page_num);

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
			ScrDispStr(0,0,0x81,"   接口板测试   ","   PORT TEST    ");
			times=0;
			test_num++;
			while(1)
			{
				times++;
				p_test_list[i].result=0; //测试前把结果设置为测试不通过
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
	ScrDispStr(0,0,0x81,"   接口板测试   ","   PORT TEST    ");
	ScrDispStr(0,2,1,"共有%d项测试",  "%dtests in total",test_num);
	    
    if(ok_num >= test_num)//
    {
		ScrDispStr(0,4,1,"%d项测试全部通过","%d tests all pass",test_num);
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
				if(++k >= test_num)break;//可能中途退出,这时候test_num并不是需要测试的项目和,应该是测试过的项目和
			}
		}

    }
    getkey();


}

#endif
