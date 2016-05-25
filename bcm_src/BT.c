#include "public.h"

void send_record_log(char *file,char * Str,...)
{
	va_list vaMarker;
	char TheBuff[1024 + 4096];
	va_start(vaMarker, Str); 
    vsprintf(TheBuff,Str,vaMarker); 
	va_end(vaMarker);
	RecordLog(file,TheBuff);
	ComSends(TheBuff);
}


//flag=0 没有握手信息的服务器  flag=1有握手信息的服务器
//flag=2 用于测试没有握手信息  flag=3用于测试有握手信息
uchar DataSendRecv(uchar flag)
{
	int i,iret0,iret1;
	uchar ucRet,str[500],strin[500];

	if(flag==0)
	{
		memset(str,0,sizeof(str));
		iret0 = PortRecvs(COM_BT,str,256+64,4000);
		iret1 = PortSends(COM_BT,str+64,256);
		send_record_log(BT_LOG,"%s",str);
		send_record_log(BT_LOG,"PortRecvSend = %d,%d",iret0,iret1);
		return 0;
	}
	else if(flag==1)
	{
		i = 0;
		memset(str,0,sizeof(str));
		TimerSet(0,30);
		while(TimerCheck(0)) //3s超时接收握手信息
		{
			ucRet = PortRecv(COM_BT,&str[i],0); 
			if(!ucRet)i++;
			if(strstr(str,"READY"))break;
		}
		send_record_log(BT_LOG,"%s",str);
		memset(str,0,sizeof(str));
		iret0 = PortRecvs(COM_BT,str,256,4000);
		iret1 = PortSends(COM_BT,str,256);
		send_record_log(BT_LOG,"PortRecvSend = %d,%d",iret0,iret1);
		return 0;
	}
	else
	{
		memset(str,0,sizeof(str));
		memset(strin,0,sizeof(strin));
		for(i=0;i<256;i++)str[i]=i;
		PortReset(COM_BT);
		if(flag==3)PortSends(COM_BT,"READY",5); //发送握手信息
		iret0 = PortSends(COM_BT,str,256);//发送数据
		iret1 = PortRecvs(COM_BT,strin,256,5000);//接受数据
		if(memcmp(str,strin,256)==0) //对比数据
			return 0;
		else 
			return 1;
	}

}

/*
蓝牙从服务器的设备名称
服务器: SLAVE_SERVER_XXXX
测试机: 机型-蓝牙模块-XXXX
*/
char gbt_slave_server_name[20];
char gbt_slave_test_name[20];

void get_bt_slave_name(void)
{
	memset(gbt_slave_server_name,0,sizeof(gbt_slave_server_name));
	memcpy(gbt_slave_server_name,"SLAVE_SERVER_",13);
	memcpy(gbt_slave_server_name+13,gExSn,4);

	memset(gbt_slave_test_name,0,sizeof(gbt_slave_test_name));
	memcpy(gbt_slave_test_name,"PAX_2564_",9);
	memcpy(gbt_slave_test_name+9,gExSn+10,4);
}

uchar check_ver_mfi(void)
{
	return 0;
}

void lcd_twinkle(void)
{
	while(kbhit())
	{
		ScrBackLight(0);
		DelayMs(300);
		ScrBackLight(2);
		DelayMs(100);
	}
	getkey();
	ScrBackLight(2);
}


void bt_slave_test(void)
{
	ST_BT_STATUS st_status;
	ST_BT_CONFIG st_cfg;
	int iRet,recvlen;
	uchar ucRet;
	char ch;
	
	ScrCls();
	if(TestModule[T_BT]!='Y')
	{
		ScrDispStr(0,4,1,"无蓝牙模块","No Bluetooth");
		getkey();
		goto END;
	}

	get_bt_slave_name();
	ScrDispStr(0,0,1,"蓝牙从测试","BT SLAVE TEST");

	iRet = BtOpen();
	if(iRet)
	{
		BeepFail();
		ScrDispStr(0,4,1,"BtOpen = %d", "BtOpen = %d",iRet);
		getkey();
		goto END;
	}

	BtGetConfig(&st_cfg);
	memset(st_cfg.pin,0,sizeof(st_cfg.pin));
	memcpy(st_cfg.pin,"0000",4);//密码统一默认为0000
	memset(st_cfg.name,0,sizeof(st_cfg.name));//名字固定,另一端的设备固定连接这个设备的名字
	memcpy(st_cfg.name,gbt_slave_test_name,sizeof(gbt_slave_test_name));
	BtSetConfig(&st_cfg);
	BtGetConfig(&st_cfg);
	memcpy(gBtMac,st_cfg.mac,6);

	send_record_log(BT_LOG,"name=%s",st_cfg.name);
	send_record_log(BT_LOG,"pin=%s",st_cfg.pin);
	send_record_log(BT_LOG,"mac=%02x:%02x:%02x:%02x:%02x:%02x",st_cfg.mac[0],st_cfg.mac[1],st_cfg.mac[2],
		st_cfg.mac[3],st_cfg.mac[4],st_cfg.mac[5]);

	PortOpen(COM_BT,"115200,8,n,1"); //这里跟MIPS有区别

	ScrClrLine(2,7);
	ScrPrint(0,2,0,"  %s",st_cfg.name);
	ScrPrint(0,3,0,"  %02x:%02x:%02x:%02x:%02x:%02x",st_cfg.mac[0],st_cfg.mac[1],st_cfg.mac[2],
		st_cfg.mac[3],st_cfg.mac[4],st_cfg.mac[5]);
	while(1)
	{
BEGIN:
		if(CancelCheck())break;
		ScrClrLine(6,7);
		BtGetStatus(&st_status);
		ScrPrint(0,4,1,"BtGetStatus=%d",st_status.status);
		recvlen=0;
		while(st_status.status == 1)//连接成功
		{
			ucRet = PortRecv(COM_BT,&ch,1000);
			if(ucRet)goto BEGIN;
			recvlen++;
			PortSend(COM_BT,ch);
			ScrPrint(0,6,1,"recv = %d",recvlen);
		}
		DelayMs(500);
	}

END:
	BtClose();
}

// flag:  0-整机 1-单次 2-成功率 3-循环 4-单板 
uchar BtTest(uchar flag)
{
	
	ushort TotalNum, FailNum;
	ushort rxLen=0;
	ST_BT_STATUS st_status;
	ST_BT_CONFIG st_cfg;
    int SlaverCount = 0;
    int TimeOutMs = 10000;
	int iRet,i = 0;
	
	ScrCls();
	get_bt_slave_name();
	if (flag == 0) ScrDispStr(0,0,1,"蓝牙从测试","BT TEST");
	if (flag == 1) ScrDispStr(0,0,1,"单次从测试","Slave Single test");
	if (flag == 2) ScrDispStr(0,0,1,"循环从测试","Slave Cycle  test");

	TotalNum=0;
	FailNum=0;
	
	iRet = BtOpen();
	if(iRet)
	{
		BeepFail();
		sprintf(gErrnoMsg,"BtOpen=%d",iRet);
		ScrDispStr(0,4,1,"BtOpen = %d", "BtOpen = %d",iRet);
		getkey();
		goto ENDERR;
	}

	BtGetConfig(&st_cfg);
	memset(st_cfg.pin,0,sizeof(st_cfg.pin));
	memcpy(st_cfg.pin,"0000",4);//密码统一默认为0000
	memset(st_cfg.name,0,sizeof(st_cfg.name));//名字固定,另一端的设备固定连接这个设备的名字
	memcpy(st_cfg.name,gbt_slave_test_name,sizeof(gbt_slave_test_name));
	BtSetConfig(&st_cfg);
	
	BtGetConfig(&st_cfg);
	memcpy(gBtMac,st_cfg.mac,6);

	send_record_log(BT_LOG,"name=%s",st_cfg.name);
	send_record_log(BT_LOG,"pin=%s",st_cfg.pin);
	send_record_log(BT_LOG,"mac=%02x:%02x:%02x:%02x:%02x:%02x",st_cfg.mac[0],st_cfg.mac[1],st_cfg.mac[2],
		st_cfg.mac[3],st_cfg.mac[4],st_cfg.mac[5]);
	
	

	PortOpen(COM_BT,"115200,8,n,1"); //这里跟MIPS有区别
BEGIN:
	ScrClrLine(2,7);
	ScrPrint(0,2,0,"  %s",st_cfg.name);
	ScrPrint(0,3,0,"  %02x:%02x:%02x:%02x:%02x:%02x",st_cfg.mac[0],st_cfg.mac[1],st_cfg.mac[2],
		st_cfg.mac[3],st_cfg.mac[4],st_cfg.mac[5]);
	while(1)
	{
		if(CancelCheck())break;
		ScrClrLine(4,7);
		BtGetStatus(&st_status);
		ScrPrint(0,4,1,"BtGetStatus=%d",st_status.status);
		if (st_status.status == 1)//连接成功
		{
		
			if(flag==2)ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",++TotalNum);
			ScrPrint(0,6,1,"DataSendRecv");
			iRet = DataSendRecv(3);
			if(iRet) //失败
			{
				ScrPrint(0,6,1,"SendRecv Fail");
				BeepFail();
				FailNum++;
			}
			else
			{
				ScrPrint(0,6,1,"SendRecv OK ");
				BeepOK();
				while(flag<=1)//单次测试
				{
					BtGetStatus(&st_status);
					DelayMs(1000);
					if (st_status.status == 0)//断开连接
						goto ENDOK;
				}
			}
						
		}
		DelayMs(1000);
	}
	if(flag<=1)goto ENDERR;
	else
	{
        ScrDispStr(0,0,0x81,"循环测试  %6d","CycleTest %6d",TotalNum);
        ShowSuccessRate(TotalNum,FailNum);
		if(kbflush(),getkey()==KEYCANCEL)goto ENDERR;
		else goto BEGIN;
    }

ENDOK:
	BtClose();
	lcd_twinkle();
	return 0;
ENDERR:
	BtClose();
	return 1;
}


void BtTest1(void)
{
	BtTest(1);
}

void BtTest2(void)
{
	BtTest(2);
}


void ScanMore(void)
{
	
	ST_BT_SLAVE	pSlave[30];
	int iRet,i;
	ScrCls();
	ScrDispStr(0,2,1,"打开蓝牙...","Open Bt...");
	iRet = BtOpen();
	if(iRet)
	{
		ScrDispStr(0,2,1,"BtOpen = %d","BtOpen = %d",iRet);
		getkey();
		BtClose();
		return;
	}
	ScrDispStr(0,2,1,"扫描中....","Scan....");
	iRet = BtScan(pSlave,10, 30);
	
	if(iRet>0)
	{
		ScrCls();
		for(i = 0; i < iRet && i < 8; i++)
		{
			ScrPrint(0, i, 0x00, "Dev%d:%02x%02x%02x%02x%02x%02x", i,
				pSlave[i].mac[5], pSlave[i].mac[4], pSlave[i].mac[3], 
				pSlave[i].mac[2], pSlave[i].mac[1], pSlave[i].mac[0]);
		}
	}
	else
	{
		ScrDispStr(0,2,1,"BtScan = %d","BtScan = %d",iRet);
	}
	getkey();
	BtClose();

}





void BtTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",		"1-Single",		BtTest1,	},
		{"2-循环",		"2-Cycle ",		BtTest2,	},
		{"3-从测试",	"3-slaver",		bt_slave_test},
		{"1-扫描",		"1-Scan",		ScanMore	},
	};
	

	if(TestModule[T_BT]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1," 没有蓝牙模块"," No BlueTooth");
		getkey();
		return;
	}
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"蓝牙单项测试","BT Test",3);

}



