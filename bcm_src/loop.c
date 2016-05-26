#include "public.h"

void UpdateLoopTime(void)
{
	ScrClrLine(6,6);
	ScrPrint(32,6,0," %d min",(GetTimerCount()-LoopStartMs)/(1000*60));
}

uchar RecordFirstError(ulong Loops)
{
	LoopStopMs = GetTimerCount();
	while(1)
	{
		LoopTempMs = GetTimerCount();
		LoopMin = ((LoopTempMs-LoopStartMs)/1000)/60;
		if(LoopMin>=10)
		{
			LoopMin = ((LoopStopMs-LoopStartMs)/1000)/60;
			RecordLog(LOOP_FIRST_ERR_RECORD,"First Error= %d min %d Times",LoopMin,Loops);
			ScrClrLine(2,3);
			ScrPrint(0,2,1," At %d Min",LoopMin);
			return 0;
		}
		Beep();
		DelayMs(1000);
		if(CancelCheck())
		{
			return 1;
		}
	}
}

uchar WaitForCancel(ulong Loops)
{
	int fid,i,ExitFlag,len;
	LOOP_STRUCT loopStructTemp;

	memset(&loopStructTemp,0,sizeof(loopStructTemp));	
	fid = open(LOOP_TIME_RECORD,O_RDWR);
	if(fid<0)ShowFileCtrlErr(__LINE__);
	len=read(fid,(uchar*)&loopStructTemp,sizeof(loopStructTemp));
	if(len!=sizeof(loopStructTemp))ShowFileCtrlErr(__LINE__);

	/*
	ComSends("fid=%d\n",fid);
	ComSends("start:%02x,%02x,%02x,%02x,%02x,%02x,%02x\n",loopStructTemp.StartTime[0],loopStructTemp.StartTime[1],
		loopStructTemp.StartTime[2],loopStructTemp.StartTime[3],loopStructTemp.StartTime[4],
		loopStructTemp.StartTime[5],loopStructTemp.StartTime[6]);
	ComSends("stop:%02x,%02x,%02x,%02x,%02x,%02x,%02x\n",loopStructTemp.StopTime[0],loopStructTemp.StopTime[1],
		loopStructTemp.StopTime[2],loopStructTemp.StopTime[3],loopStructTemp.StopTime[4],
		loopStructTemp.StopTime[5],loopStructTemp.StopTime[6]);
	ComSends("CNT=%d,SE=%d\n",loopStructTemp.LoopCnt,loopStructTemp.LoopSeconds);
	*/


	i=180;//进来马上记录时间
	ExitFlag=0;
	while(1)
	{
		if(i==180 || ExitFlag==1)//5min
		{
			LoopTempMs=GetTimerCount();
			loopStructTemp.LoopSeconds = (LoopTempMs-LoopStartMs)/1000;
			loopStructTemp.LoopCnt = Loops;
			GetTime(loopStructTemp.StopTime);

			/*
			ComSends("start:%02x,%02x,%02x,%02x,%02x,%02x,%02x\n",loopStructTemp.StartTime[0],loopStructTemp.StartTime[1],
				loopStructTemp.StartTime[2],loopStructTemp.StartTime[3],loopStructTemp.StartTime[4],
				loopStructTemp.StartTime[5],loopStructTemp.StartTime[6]);
			ComSends("stop:%02x,%02x,%02x,%02x,%02x,%02x,%02x\n",loopStructTemp.StopTime[0],loopStructTemp.StopTime[1],
				loopStructTemp.StopTime[2],loopStructTemp.StopTime[3],loopStructTemp.StopTime[4],
				loopStructTemp.StopTime[5],loopStructTemp.StopTime[6]);
			ComSends("CNT=%d,SE=%d\n",loopStructTemp.LoopCnt,loopStructTemp.LoopSeconds);
			*/

			seek(fid,0,SEEK_SET);
			len=write(fid,(uchar*)&loopStructTemp,sizeof(loopStructTemp));
			if(len!=sizeof(loopStructTemp))ShowFileCtrlErr(__LINE__);
			if(ExitFlag==1) return 1;
			i=0;
			
			/*
			seek(fid,0,SEEK_SET);
			len=read(fid,(uchar*)&loopStructTemp,sizeof(loopStructTemp));

			ComSends("start:%02x,%02x,%02x,%02x,%02x,%02x,%02x\n",loopStructTemp.StartTime[0],loopStructTemp.StartTime[1],
				loopStructTemp.StartTime[2],loopStructTemp.StartTime[3],loopStructTemp.StartTime[4],
				loopStructTemp.StartTime[5],loopStructTemp.StartTime[6]);
			ComSends("stop:%02x,%02x,%02x,%02x,%02x,%02x,%02x\n",loopStructTemp.StopTime[0],loopStructTemp.StopTime[1],
				loopStructTemp.StopTime[2],loopStructTemp.StopTime[3],loopStructTemp.StopTime[4],
				loopStructTemp.StopTime[5],loopStructTemp.StopTime[6]);
			ComSends("CNT=%d,SE=%d\n",loopStructTemp.LoopCnt,loopStructTemp.LoopSeconds);
			*/
		}
		if(CancelCheck()) ExitFlag = 1;
		Beep();
		DelayMs(1000);
		i++;
		
	}
}


uchar KbTestCyc(ulong Loops)
{
	ScrCls();
	ScrDispStr(0,0,0x81,"按键煲机  %6d","KEY LOOP  %6d",Loops);
	UpdateLoopTime();
	if(!kbhit())
	{
		ScrPrint(0,4,1,"Has Key Down");
		if(RecordFirstError(Loops)) return 1;
		else RecordLog(LOOP_FIRST_ERR_RECORD,"GetKey=0x%02x",getkey());
		WaitForCancel(Loops);
		return 1;
	}
	kblight( (uchar)(Loops%2) );
	DelayMs(500);
	return 0;
}

uchar LcdTestCyc(ulong Loops)	
{
	ScrCls();
	ScrDispStr(0,0,0x81,"LCD 煲机  %6d","LCD LOOP  %6d",Loops);
	UpdateLoopTime();
	LcdTest(3);
	return 0;
}

uchar LedplyTestCyc(ulong Loops)
{
	return 0;
}

uchar LedTestCyc(ulong Loops)
{
	ScrCls();
	ScrDispStr(0,0,0x81,"LED 煲机  %6d","LED LOOP  %6d",Loops);
	UpdateLoopTime();
	PiccLight(PICC_LED_ALL,1);
	DelayMs(500);
	return 0;
}

uchar IcTestCyc(ulong Loops)
{
	uchar slot;	
	int Num=0,jj=0;
	ScrCls();
	ScrDispStr(0,0,0x81,"IC卡煲机  %6d","ICC LOOP  %6d",Loops);
	UpdateLoopTime();
	kbflush();
	DispTimer(7);
	for(slot=0; slot<=gSamNum+1; slot++)
	{
		if(slot==0 && gIcNum==0)continue;
		if(slot==1)continue;
		if(!IccDetect(slot))
		{
			ScrPrint(0,4,1,"%d Slot has card ",slot);
			if(RecordFirstError(Loops)) return 1;
			else RecordLog(LOOP_FIRST_ERR_RECORD,"IC ERR=%d Slot Has Card",slot);
			WaitForCancel(Loops);
			return 1;

		}
	}
	ScrDispStr(0,4,1,"没有IC卡片...","No IC Card");			
	DelayMs(500);
	return 0;
}
		

uchar ModemTestCyc(ulong Loops) 
{
	return 0;
}


uchar WnetTestCyc(ulong Loops)	
{
	int iRet;
	int Sim;
	int i;
	uchar rsp_wnet[300];
	uchar rsp[300];
	uchar SignalLevel = 0;
	
	ScrCls();
	ScrDispStr(0,0,0x81,"无线煲机  %6d","WNET LOOP %6d",Loops);
	UpdateLoopTime();
	DispTimer(7);

	RouteSetDefault(ROUTE_WNET);

	ScrDispStr(0,2,1, "模块上电...", "Power on...");
	WlSwitchPower(1);

	for(Sim=0; Sim<gSimNum; Sim++)
	{
		ScrClrLine(2,5);
		if(gSimNum==2)    //wavecom 双SIM卡
		{
			ScrDispStr(0,2,1,"选择SIM卡%d","Select SIM %d",Sim);
			WlSelSim(Sim);
		}
		ScrDispStr(0,2,1,"模块初始化...", "WLInit....");
		iRet= WlInit(NULL);
		while(iRet!=WL_RET_ERR_NOSIM && iRet!=WL_RET_ERR_INIT_ONCE)
		{
			if(iRet==0 && strstr(InfoOut[C_PN],"S90-MC0-363-01EA")!=NULL)
				break;
			ScrPrint(0,4,1,"WlInit ERR=%d",iRet);
			if(RecordFirstError(Loops)) return 1;
			else RecordLog(LOOP_FIRST_ERR_RECORD,"WlInit ERR=%d",iRet);
			WaitForCancel(Loops);
			return 1;
		}
	}
	
	if(TestModule[T_WCDMA]=='N')
	{
		iRet= WlGetSignal(&SignalLevel); //查询信号强度	
		if (iRet!= 0)
		{
			ScrPrint(0,4,1,"WlGetSignal:%d",iRet);
			if(RecordFirstError(Loops)) return 1;
			else RecordLog(LOOP_FIRST_ERR_RECORD,"WlGetSignal ERR=%d",iRet);
			WaitForCancel(Loops);
			return 1;
		}
	}

	ScrDispStr(0,2,1,"读取无线版本信息", "Get Wireless VerInfo");
	DelayMs(3000);
	iRet= WlOpenPort();   //打开无线通讯串口
    if(iRet)
    {
		ScrPrint(0,4,1,"WlOpenPort:%d",iRet);
		if(RecordFirstError(Loops)) return 1;
		else RecordLog(LOOP_FIRST_ERR_RECORD,"WlOpenPort ERR=%d",iRet);
		WaitForCancel(Loops);
		return 1;
    }
    WlSendCmd(NULL, rsp_wnet, 100, 100, 50);
    i = 0;
    while(1)
    {
         i++;
         if(!(i%3)) ScrPrint(0,4,1,".....");
         else if((i%3)==1) ScrPrint(0,4,1,"...........");
         else if((i%3)==2) ScrPrint(0,4,1,"..................."); 
         memset(rsp_wnet, 0x00, sizeof(rsp_wnet));
         iRet= WlSendCmd((unsigned char*)"at+GMR\r", rsp_wnet, 100, 30000, 3000);
         if(iRet)  break;
         if(strstr((char *)rsp_wnet, "ERROR: 515") != NULL )  
         {
             DelayMs(500);
             continue;
         }
         else
         {
             break;
         }
         if(CancelCheck())break;
    }
    if(iRet)
    {
         
   		 ScrPrint(0,2,1, "WlSendCmd:%d",iRet);
		 if(RecordFirstError(Loops)) return 1;
		 else RecordLog(LOOP_FIRST_ERR_RECORD,"WlSendCmd ERR=%d",iRet);
		 WaitForCancel(Loops);
		 return 1;
   		 
    }
    else
    {
		 memset(rsp, 0, sizeof(rsp));
		 strcpy((char *)rsp,(char *)rsp_wnet);
		 Wl_handle_ver(rsp,sizeof(rsp)); 
     	 ScrClrLine(2,5);
		 ScrDispStr(0,2,1,"版本:\n%s","VERSION:\n%s",rsp);//显示版本
    }
	WlSwitchPower(0);
	DelayMs(1000);
	return 0;
}


uchar PrinterTestCyc(ulong Loops)
{
	uchar ucRet;

	ScrCls();
	ScrDispStr(0,0,0x81,"打印煲机  %6d","PRN LOOP  %6d",Loops);
	UpdateLoopTime();
	kbflush();
	DispTimer(7);
	PrnInit();
	PrnStr("ABCDEFG HIJKLMN");
	PrnStart();
	do
	{
		ucRet=PrnStatus();
	}while(ucRet==0x01);
	
	if(ucRet==0x02)
	{
		ScrDispStr(0,4,1,"缺纸...", "No paper...");
		DelayMs(500);
	}
	else
	{
		ScrDispStr(0,4,1,"其他错误: 0x%02x","Other ERR: 0x%02x",ucRet);
		if(RecordFirstError(Loops)) return 1;
		else RecordLog(LOOP_FIRST_ERR_RECORD,"PRN ERR=0x%02x",ucRet);
		WaitForCancel(Loops);
		return 1;
	}
	return 0;
}

uchar MagTestCyc(ulong Loops)
{
	uchar ucRet;
	ScrCls();
	ScrDispStr(0,0,0x81,"磁卡煲机  %6d","MAG LOOP  %6d",Loops);
	UpdateLoopTime();
	kbflush();
	DispTimer(7);
	//MagOpen();//放在每次煲机的前面
	//MagReset();//
	TimerSet(0,10);
	while(1)
	{
		if(!TimerCheck(0)) break;
		ucRet=MagSwiped();
		if(!ucRet) break;
	}
	if(!ucRet)
	{
		ScrDispStr(0,4,1,"检测到刷卡... ","Checked Has Card");
		if(RecordFirstError(Loops)) return 1;
		else RecordLog(LOOP_FIRST_ERR_RECORD,"MagCard ERR=0x%02x",ucRet);
		WaitForCancel(Loops);
		return 1;
	}
	else
	{
		ScrDispStr(0,4,1,"没有检测到刷卡","No Checked Card");
		DelayMs(500);
	}
	MagClose();
	return 0;
}

uchar RfTestCyc(ulong Loops)
{
	uchar ucRet;
	uchar CardType[10],SerialNo[11];

	PiccLight(PICC_LED_ALL,1);//LED煲机
	ScrCls();
	ScrDispStr(0,0,0x81,"RF煲机    %6d","RF LOOP   %6d",Loops);
	UpdateLoopTime();
	kbflush();
	DispTimer(7);
    
	PiccOpen();
	ucRet=PiccDetect(0,CardType,SerialNo, NULL,NULL);
	if(ucRet!=0x03 && ucRet!=0x17)
	{
		ScrDispStr(0,4,1,"其他错误: 0x%02x","Other ERR: 0x%02x",ucRet);
		if(RecordFirstError(Loops)) return 1;
		else RecordLog(LOOP_FIRST_ERR_RECORD,"RF ERR=0x%02x",ucRet);
		WaitForCancel(Loops);
		return 1;
	}
	else 
	{
		ScrDispStr(0,4,1,"感应区无卡", "No card Detect");
		DelayMs(500);
	}
	PiccClose();
	return 0; 
}

uchar FelicaTestCyc(ulong Loops)
{
	return 0; 
}

uchar GSensorTestCyc(ulong Loops)
{
	return 0;
}

uchar SpeechTestCyc(ulong Loops)
{
	return 0;
}	


uchar UsbHostTestCyc(ulong Loops)
{
	uchar ucRet;
	ScrCls();
	ScrDispStr(0,0,0x81,"HOST 煲机 %6d","HOST LOOP %6d",Loops);
	UpdateLoopTime();
	PortClose(P_USB_DEV);//防止中途退出而没有关闭Device
	PortClose(P_USB_HOST);//
	DelayMs(500);
							
	ucRet = PortOpen(P_USB_HOST,"UDISK");
	if(ucRet!= 16)//16表示设备不在位
	{
		ScrPrint(0,4,1,"PortOpen(12)=%d",ucRet);
		if(RecordFirstError(Loops)) return 1;
		else RecordLog(LOOP_FIRST_ERR_RECORD,"PortOpen(12)=%d",ucRet);
		WaitForCancel(Loops);
		return 1;
	}
	PortClose(P_USB_HOST);
	return 0;
}

// flag: 0-整机/主板  1-单次 2-循环 3-成功率
uchar  SDCardTestCyc(ulong Loops)
{
	int fd;
	FS_W_STR  sd_card;
	uchar ucFileName[30];

	ScrCls();
	ScrDispStr(0,0,0x81,"SD 煲机   %6d","SD LOOP   %6d",Loops);
	UpdateLoopTime();
	
	sd_card.fmt = NAME_FMT_ASCII;
	sd_card.str = ucFileName;
	sd_card.size = sprintf(sd_card.str,"/sd_card/test");
	fd = FsOpen(&sd_card, FS_ATTR_C|FS_ATTR_R);
	if(fd == -5)//设备不存在
	{
		ScrDispStr(0,4,1,"没有插入SD卡", "No SDCard Insert");
		DelayMs(500);
	}
	else
	{
				
		ScrDispStr(0,4,1,"SD卡错误%d", "SD Error %d",fd);
		if(RecordFirstError(Loops)) return 1;
		else RecordLog(LOOP_FIRST_ERR_RECORD,"SD FsOpen=%d",fd);
		WaitForCancel(Loops);
		return 1;
	}
	FsClose(fd);
    return 0;

}

uchar LanTestCyc(ulong Loops)
{
	return 0;
}
 
uchar BtTestCyc(ulong Loops)
{
	return 0;
}


uchar WifiTestCyc(ulong Loops)
{
#ifdef WIFI_ON
	int iRet;
	ST_WIFI_AP WIFI_APs[10];
	ScrCls();
	ScrDispStr(0,0,0x81,"WIFI煲机  %6d","WIFI LOOP  %6d",Loops);
	UpdateLoopTime();

	RouteSetDefault(ROUTE_WIFI);

	memset(WIFI_APs, 0, sizeof(WIFI_APs));
	iRet = WifiOpen();
	if(iRet)
	{
		ScrPrint(0,4,1,"WifiOpen=%d",iRet);
		if(RecordFirstError(Loops)) return 1;
		else RecordLog(LOOP_FIRST_ERR_RECORD,"WifiOpen=%d",iRet);
		WaitForCancel(Loops);
		return 1;
	}
	DelayMs(1000);
	iRet = WifiScan(WIFI_APs, sizeof(WIFI_APs)/sizeof(ST_WIFI_AP));
	if(iRet<0 && iRet!=-2)
	{
		ScrPrint(0,4,1, "WifiScan=%d",iRet);
		if(RecordFirstError(Loops)) return 1;
		else RecordLog(LOOP_FIRST_ERR_RECORD,"WifiScan=%d",iRet);
		WaitForCancel(Loops);
		return 1;
	}
	WifiClose();
	DelayMs(2000);
	

#endif
	return 0;
}

