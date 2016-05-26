/************************************************************************
* Copyright (c) 2012-2013, PAX Computer Technology (Shenzhen)CO.,Ltd.
* All rights reserved.
* 
* 功能描述：  用于工厂对小S系列进行功能性测试，包括整机测试，单项测试，煲机测试和单板测试。
* 其它描述:   其它补充内容的说明
*************************************************************************/

#include "public.h"
#include "Sound.h"

const APPINFO AppInfo={
    APPNAME,
    "APP-TEST",
    SOFT_VERSION,
    "PAX Co.,20130415",
    "Factory Main Test",
    "",
    0,
    0,
    0,
    ""
};

extern void test_bid(void);
extern void p_port_test(void);
extern int AutoTest(int flag);
extern void AutoTest0(void);
extern void LanHubTestAll(void);

int event_main(ST_EVENT_MSG *msg)
{
    SystemInit();
    return 0;
}

int main(void)
{
    int i,fid, iRet;
    uchar InitFlag;

    InitFlag=SystemInit();
	
    SetSrand();
    ScrCls(); 
	kbflush();
	PiccLight(0x0f,1);
	
	Cmu200Result = 'N';
	gBaseChange = 'N';
	GetWnetVerFlag=0;//无线版本信息只需要获取一次
	GetExtInfoFlag=0;//扩展版本信息只需要获取一次
	gPPQcom = P_RS232A;	 // PPQ上传使用的端口号

    ScrDispStr(0,4,1,"读取终端信息...","Get TermInfo....");
    GetConfig();//获取配置信息

	if(TermInfo[0]==T610)
	{
		if( SetCombKey() )ScrDispStr(0,4,1,"组合键出错...","ComKey Err....");
	}
    for(i=1; i<=8; i++)    //关闭POS所有图标
    {
        ScrSetIcon((unsigned char)i,0);
    }
	remove("TestFlash.dat");//确保删除Flash测试中无用的空间

    if(InitFlag)//非首次运行应用
    {
        fid=open(F_TELNUM,O_RDWR);      // setup telephone number for modem
        if(fid<0) 
		{
			InitFlag=0;
		}
        else
        {
            memset(TelNum, 0x00, sizeof(TelNum));
            iRet = read(fid, TelNum, sizeof(TelNum));
            if(iRet < 0)
            {
                ShowFileCtrlErr(__LINE__);  //获取文件操作出错错误码
            }
            close(fid);
        }

        fid = open(F_LANPARA,O_RDWR);   //setup network parameters
        if(fid<0) 
		{
			InitFlag = 0;
		}
        else
        {
            memset(LANPara, 0x00, sizeof(LANPara));
            iRet = read(fid, (uchar*)LANPara, sizeof(LANPara));
            if(iRet < 0)
            {
                ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
            }
            close(fid);
        }
        
		fid = open(F_CMU200,O_RDWR);   //cmu200测试结果记录
        if(fid<0){
			InitFlag = 0;
		}else{
            iRet = read(fid,&Cmu200Result,1);
            if(iRet < 0){
                ShowFileCtrlErr(__LINE__);
            }
            close(fid);
        }	

		fid = open(F_BASE_CHANGE,O_RDWR);   //底座充电测试结果记录
        if(fid<0){
			InitFlag = 0;
		}else{
            iRet = read(fid,&gBaseChange,1);
            if(iRet < 0){
                ShowFileCtrlErr(__LINE__);
            }
            close(fid);
        }


#ifdef SPEECH_ON
		fid=open("Confirm.wav",O_RDWR);
		if(fid<0) InitFlag = 0;
		close(fid);
#endif
		
    }//if(InitFlag)
    
	if(!InitFlag)//第一次运行应用
    {
#ifdef SPEECH_ON
		fid=open("Confirm.wav",O_CREATE|O_RDWR);
		if( write(fid,Sound,sizeof(Sound)) != sizeof(Sound) )
		ShowFileCtrlErr(__LINE__);
		close(fid);
#endif
		remove(F_TSCB);
		
		//CMU200测试结果
		fid = fexist(F_CMU200);
		if(fid>0){
			close(fid);
			remove(F_CMU200);
		}
		fid=open(F_CMU200,O_CREATE|O_RDWR);
		if(fid<0){
			ShowFileCtrlErr(__LINE__);
		}else{
			iRet = write(fid,"NN",1);
		}
		close(fid);

		//底座充电结果
		fid = fexist(F_BASE_CHANGE);
		if(fid>0){
			close(fid);
			remove(F_BASE_CHANGE);
		}
		fid=open(F_BASE_CHANGE,O_CREATE|O_RDWR);
		if(fid<0){
			ShowFileCtrlErr(__LINE__);
		}else{
			iRet = write(fid,"NN",1);
		}
		close(fid);

		//
		memset(TelNum, 0x00, sizeof(TelNum));//MODEM号码设置
        strcpy((char*)TelNum,"82");
        fid=fexist(F_TELNUM);      
        if(fid>0)
        {
            close(fid);
            remove(F_TELNUM);
        }
        fid=open(F_TELNUM,O_CREATE);         //该文件用于保存MODEM拨号号码
        if(fid<0)
        {
            ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
        }
        else
        {
            iRet = write(fid,TelNum,sizeof(TelNum));
            if(iRet != sizeof(TelNum))
            {
                ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
            }
            close(fid);
        }
        
        memset(LANPara, 0x00, sizeof(LANPara));// Lan和WIFI设置
        memcpy(LANPara[0],"192.168.0.244",13); // init local IP 2009-07-17 为不抢占公司IP，将默认IP：44改为244

#ifdef BAXI
        memcpy(LANPara[1],"192.168.6.231",13); // init remote IP //服务器默认IP
#else  
		memcpy(LANPara[1],"192.168.0.231",13); // init remote IP //服务器默认IP
#endif

        memcpy(LANPara[2],"192.168.0.254",13); // init gateway
        memcpy(LANPara[3],"00060180",8);	   // init service port //服务器默认端口
        memcpy(LANPara[4],"255.255.255.0",13); //init netmask
        memcpy(LANPara[6],"192.168.0.001",13); //AP IP for wifi test， wifi不支持 .000.格式的IP
        fid = fexist(F_LANPARA);
        if(fid > 0)
        {
            close(fid);
            remove(F_LANPARA);
        }
        fid = open(F_LANPARA,O_CREATE);     //该文件用于保存与网络有关的参数
        if(fid<0)
        {
            ShowFileCtrlErr(__LINE__);  //获取文件操作出错错误码
        }
        else
        {
            if(write(fid,(unsigned char*)LANPara,sizeof(LANPara)) != sizeof(LANPara))
            {
                ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
                close(fid);
            }
        }
     
    }//else if(!InitFlag)

#ifdef LAN_HUB
	while(1)
	{
		LanHubTestAll();
	}
#endif

#ifdef PORT_TEST
	p_port_test();
#else
	
	while(TestModule[T_TSCREEN]=='Y' && fexist(F_TSCB)<0 && TermInfo[0]!=MT30)
	{

		if( TouchScreenCalibration()==0)//第一次运行需要屏幕校正
		{
			fid = open(F_TSCB,O_CREATE|O_RDWR);
			ScrDispStr(0,3,1,"    校准成功    ","Calibration  OK ");
			close(fid);
			break;
		}
		else
		{
			ScrDispStr(0,3,1,"    校准失败    ","Calibration Fail");
			DelayMs(500);
			if(CancelCheck())break;
		}
	}

	{//时钟检测年份
		uchar tmp[7];
		GetTime(tmp);
		if(tmp[0] < 0x15 || tmp[0]>0x20)//时间不是在15年～20年之间报错
		{
			ScrDispStr(0,4,1,"时间是 %02x 年!!","It's %02x Year!!",tmp[0]);
			BeepFail(); 
			getkey();
		}
	}

	
#endif

	//LED灯亮1s
	//if(TestModule[T_LED]=='Y')
	{
		PiccLight(0x0f,0);
	}

	//PrinterTestAll();

	//if(GetStep()<=5)TestAll();    //如果有接PPQ，则直接进入整机测试
	{
		TESTSINGLE_ITEM TestItem[]={
			{"1-整机",	"1-ALL",	TestAll,	},
			{"2-单项",	"2-Unit",	TestSingle,	},
			{"3-煲机",	"3-Cyc",	TestCyc,	},
			{"4-删除",	"4-Del",	DelFacApp,	},
			{"5-应用",	"5-Sapp",	AppManager,	},
			{"6-自动",	"6-Auto",	AutoTest0	},
		};

		uchar TestNum;		
		TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
		DisSubMenuEx(TestItem,TestNum,"主菜单","Main Menu",1);
	}
}

extern uchar COMA2PC(uchar flag);
//flag=0 正常测试  flag!=0 PPQ测试
void test_all(int flag)
{
    uchar ucRet,ucKey;
	uchar OKNum,TestNum,AllNum,Times;
    uchar *pStrChinese, *pStrEnglish;
    int i,j,k,iRet,step=0;
	ulong LoopAllTime,LoopAllCnt;
	int CycFailCntTemp[20];
	uint TTT1;

	TEST_ITEM TestItem[]={
#ifdef Venezuela
		{TestModule[T_COMA_TEST],	0,	"COMA",			ComATest	},
#else
		{TestModule[T_COMA_TEST],	0,	"COMA",			COMA2PC		},
#endif
		//{TestModule[T_BT],			0,	"BT",			BtTest		},
		{TestModule[T_TSCREEN],		0,	"TSCREEN",		TsTest		},//TestModule[T_TSCREEN] S900/S300不测试触屏
		{TestModule[T_IC],			0,	"IC",			IcTest		},
		{TestModule[T_LAN],			0,	"LAN",			LanTest		},
		{TestModule[T_WIFI],		0,	"WIFI",			WifiTest	},
		{TestModule[T_PINPORT],		0,	"PINPORT",		PinpadTest	},
		{TestModule[T_COMB_TEST],	0,	"COMB",			ComBTest	},
		{TestModule[T_UDISK],		0,	"UDISK",		UDiskTest	},
		{TestModule[T_SDCARD],		0,	"SDCARD",		SDCardTest	},
		{TestModule[T_WNET],		0,	"WNET",			WnetTest	},//TestSim0 //WnetTest //Cmu200Test
		
		//{TestModule[T_MODEM],		0,	"MODEM",		auto_modem	},//modem预拨号	
		
		{TestModule[T_PRINTER],		0,	"PRINTER",		PrinterTest	},
		{TestModule[T_PAPERBACK],	0,  "PAPERBACK",	PaperBackTest},
		{TestModule[T_SPEECH],		0,	"SPEECH",		SpeechTest	},
		//{TestModule[T_BYPHONE],		0,	"BYPHONE",		ByPhoneTest	},
		{TestModule[T_UDEV_TEST],	0,	"USBDEV",		UsbDevTest	},//T_USBDEV是表示有没有该模块，
		{TestModule[T_LCD],			0,	"LCD",			LcdTest		},//T_UDEV_TEST表示整机需不需要测试
		{TestModule[T_CLK],			0,	"CLK",			ClkTest		},//UHOST与UDEV是同个口，则不测试
		{TestModule[T_SM2],			0,	"SM2",			SM2Test		},//国密测试
		{TestModule[T_KB],			0,	"KB",			KbTest		},
		{TestModule[T_BEEP],		0,	"BEEP",			BeepTest	},
		{TestModule[T_KEYLED],		0,	"KEYLED",		KbLedTest	},
		{TestModule[T_MAG],			0,	"MAG",			MagTest		},
		{TestModule[T_RF],			0,	"RF",			RfTest		},
		{TestModule[T_LED],			0,	"LED",			LedTest		},
		//{TestModule[T_GSENSOR],		0,	"GSENSOR",		GSensorTest	},
		{TestModule[T_SCAN],		0,	"SCAN",			ScanTest	},
		{TestModule[T_BATTERY],		0,	"BATTERY",		BatteryTest	},
		{'N',						0,	"VER",			VerTest     },//TestModule[T_VER]='N',开始必须是N
	};

	TTT1=GetTimerCount();
	AllNum = sizeof(TestItem)/sizeof(TEST_ITEM);//AllNum = sizeof(TestItem)/sizeof(TestItem[0]);
	OKNum=TestNum=0;
	memset(CycFailCntTemp,0,sizeof(CycFailCntTemp));

    ScrCls();
    pStrEnglish = "Integration Test";
    pStrChinese = "    整机测试    ";
    ScrDispStr(0,0,0x81, pStrChinese, pStrEnglish);

	if(TestModule[T_WNET]=='Y')
	{
		ScrClrLine(2, 7);
		ScrDispStr(0,4,1,"无线模块上电...", "WNET POWER ON...");
		WlSwitchPower(1);
	}

	if(TestModule[T_MODEM]=='Y')
	{
		//auto_modem(1); //modem预拨号
	}

    DeleteLog(F_BASIC_INFO);
	RECORD(0,".FTEST=%s",SOFT_VERSION); //记录FTest版本
    RECORD(1, "INTEGRATION TEST BEGIN");
    RECORD(1, "TestStart=%s",DispTimer(7));
    RECORD(1, "\n");
    RECORD(1, "..TestFinished=0");
    step=GetStep();
    while(step>5 && gPPQcom==RS232A) //PPQ上传是串口才需要要求开始就连接PPQ 
	{
		if(CancelCheck())break;
		ScrClrLine(2, 7);
		ScrDispStr(0,4,1,"请插上PPQ","PLS Inser PPQ");
		step = GetStep();
	}

	if(step==2 || step==3)gTemperature=0;//其他的不需要
	else  gTemperature=1; //煲机前全检 需要检测温度

	if(step==2 || TermInfo[0]==D200 || TermInfo[0]==S910 || TermInfo[0]==T60) //|| TermInfo[0]==D210
		gBatChange=1;//S2步骤需要插拔电源测试电源模块
	else gBatChange=0;

	if(gBatChange && TestModule[T_BATTERY]=='Y')BatteryTest(1);

	if(TermInfo[0]==S200)
	{
		if(step==1 || step==3)gTsTest = 0;
		else gTsTest = 1;
		
		if(!gTsTest)
		{
			for(i=0;i<AllNum;i++)
			{
				if(TestItem[i].TestFun == TsTest)
					TestItem[i].flag = 'N';
			}
		}
	}

    RECORD(1, "..TestStep=%d",step);
    CalcLoopTime(&LoopAllTime,&LoopAllCnt); //计算并记录煲机时间
	RECORD(1,"CMU200=%c",Cmu200Result);

	if(flag)RECORD(1, ".PPQTEST=1\r\n");


	/*
	if(step==2  && ( LoopAllTime<900 || memcmp(CycFailCnt,CycFailCntTemp,sizeof(CycFailCnt)) ) )
	{
		BeepFail();
		ShowLoopErr(LoopAllTime,LoopAllCnt);
	}
	*/

	for(i=0;i<AllNum;i++)
	{
		if(TestItem[i].flag=='Y')
		{

			if(flag)
			{
				RECORD(1,"%s=1\r\n",TestItem[i].itemStr);
				OKNum++;
				TestNum++;
				TestItem[i].result=1;
				continue;
			}

			ScrCls();
			ScrDispStr(0,0,0x81, pStrChinese, pStrEnglish);
			Times=0;
			TestNum++;
			while(1)
			{
				Times++;
				memset(gErrnoMsg,0,sizeof(gErrnoMsg));
				ucRet=TestItem[i].TestFun(0);
				RECORD(1,"..%s_TEST_TIME=%d",TestItem[i].itemStr,GetTimerCount()-TTT1);//记录每项目测试的时间
				if(ucRet==0)
				{
					RECORD(1,"%s=1\r\n",TestItem[i].itemStr);
					TestItem[i].result=1;
					OKNum++;
					break;
				}
				else
				{
					RECORD(1,"%s=0\r\n",TestItem[i].itemStr);
					RECORD(1,"..%s.err=%s\r\n",TestItem[i].itemStr,gErrnoMsg);
					if(Times<2)
					{  
						ucRet=Select();
						if(ucRet==KEY1 || ucRet==KEYUP) continue;
						if(ucRet==KEYCANCEL) goto RESULT;
					}
				}
				break;
			}//1			
		}//if(TestItem[i].flag='Y')
	}//for

RESULT:
	ShowVer();   // 取终端信息
    ModifyRecord();
    RECORD(1, "INT.TOTAL.cnt=%d", TestNum);
    RECORD(1, "INT.PASS.cnt=%d", OKNum);
    RECORD(1, "INT.FAIL.cnt=%d", TestNum-OKNum);
	for(i=0,j=0;i<AllNum;i++)
	{
		if(TestItem[i].flag=='Y')
		{
			if(TestItem[i].result==0)
			{
				j++;
				RECORD(1, "#FAIL%d=%s",j,TestItem[i].itemStr);
			}
		}
	}
    RECORD(1, "\n");
    RECORD(1, "INTEGRATION TEST END\n");
    RECORD(1, "\n");

    ScrClrLine(2,7);
    TimerSet(0, 200);
	ucKey=KEYCANCEL;
	ScrDispStr(0,2,1, "[确认]-上传结果", "[ENTER]-Upload");
	ScrDispStr(0,4,1, "[其他]-跳过上传", "[OTHER]-Skip  ");
    while(i=TimerCheck(0))
    {
        ScrPrint(60,6,1," %3d ",i);
        if(!kbhit())
        {
            ucKey=getkey();
            break;
        }
    }

    if(ucKey == KEYENTER)
    {
        {//上传时间进行比对 CLK测试
			uchar tmp[7];
			uchar curTime[20];
			GetTime(tmp);
			if(tmp[0] < 0x15 || tmp[0]>0x20)//时间不是在12年～30年之间报错
			{
				ScrDispStr(0,4,1,"时间是 %02x 年!!","It's %02x Year!!",tmp[0]);
				BeepFail(); 
				getkey();
			}
			sprintf(curTime, "20%02x%02x%02x%02x%02x%02x\r\n",tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5]);
			//RECORD(1, "..CurTime=%s\r\n", curTime);
		}
		while(1)
        {			
			iRet = UploadFile();
            if(iRet == PC_SAVEOK || iRet == PC_VER_CHECK_OK) {
				break;
			}else if(iRet == POS_NO_RECORD || iRet == OPEN_PORT_ERR || iRet == OPEN_RECORD_ERR){
                getkey();
                break;
            }else  if(CancelCheck())break;

        }
        if(iRet == PC_SAVEOK)  //版本信息不符
        {
            TestNum++;
            TestItem[AllNum-1].flag='Y';//最后一项是VER项
            TestItem[AllNum-1].result=0;
        }
        else if(iRet == PC_VER_CHECK_OK)  //版本信息正确
        {
            TestNum++;
			OKNum++;
            TestItem[AllNum-1].flag='Y';
            TestItem[AllNum-1].result=1;
            
        }
    }

    ScrCls();
    kbflush();
	ScrDispStr(0,0,0x81,pStrChinese,pStrEnglish);
	ScrDispStr(0,2,1,"共有%d项测试",  "%dtests in total",TestNum);
	    
    if(OKNum>=TestNum)//
    {
		BeepOK();
		ScrDispStr(0,4,1,"%d项测试全部通过","%d tests all pass",TestNum);
        WaitEsc(1,1);
    }
    else
    {
        BeepFail(); 
        ScrDispStr(0,4,1,"整机测试仅过%d项",	"%dtest pass",OKNum);
        ScrDispStr(0,6,1,"请按键查看失败项",	"CHECK FAIL ITEM");
        getkey();
        ScrClrLine(2,7);
		for(i=0,j=0,k=0;i<AllNum;i++)
		{
			if(TestItem[i].flag=='Y')
			{
				if(TestItem[i].result==0)
				{
					if(j%2==0)
						ScrPrint(0,(uchar)(j/2+2),0,"%s ",TestItem[i].itemStr);
					else
						ScrPrint(64,(uchar)(j/2+2),0,"%s ",TestItem[i].itemStr);
					j++;
				}
				if(++k >=TestNum)break;//可能中途退出，这时候TestNum并不是需要测试的项目合，应该是测试过的项目和
			}
		}

    }
    getkey();
}

void TestCyc(void)
{

#define TIMEOUT 5*60*10  //5分钟
	uchar ucRet,AllNum,ExitFlag=0;
    ulong loops=0;
    uchar *pStrChinese, *pStrEnglish;
	int fid,i,iRet;
	LOOP_STRUCT LoopStruct[5];//保存5次数据
	
	TESTCYC_ITEM TestCycItem[]={
		{TestModule[T_LCD],			LcdTestCyc		},
		{TestModule[T_LEDPLY],		LedplyTestCyc	},
		{TestModule[T_IC],			IcTestCyc		},
		{TestModule[T_MODEM],		ModemTestCyc	},
		{TestModule[T_WNET],		WnetTestCyc		},
		{TestModule[T_PRINTER],		PrinterTestCyc	},
		{TestModule[T_SPEECH],		SpeechTestCyc	},
		{TestModule[T_MAG],			MagTestCyc		},
		{TestModule[T_LED],			LedTestCyc		},
		{TestModule[T_RF],			RfTestCyc		},
		{TestModule[T_FELICA],		FelicaTestCyc	},
		{TestModule[T_GSENSOR],		GSensorTestCyc	},
		{TestModule[T_KB],			KbTestCyc		},
		{TestModule[T_USBHOST],		UsbHostTestCyc	},
		{TestModule[T_SDCARD],		SDCardTestCyc	},
		{TestModule[T_LAN],			LanTestCyc		},
		{TestModule[T_WIFI],		WifiTestCyc		},
		{TestModule[T_BT],			BtTestCyc		},
		{TestModule[T_SCAN],		ScanTestCyc     },

		
	};


	AllNum = sizeof(TestCycItem)/sizeof(TESTCYC_ITEM);
    ScrCls();
    pStrEnglish = "   Cycle Test   ";
    pStrChinese = "    煲机测试    ";
    ScrDispStr(0,0,0x81,pStrChinese, pStrEnglish);
	
	/*
	ucRet=IsQuitOrDone("删除记录?","Del RECORD?");
	if(ucRet==KEYENTER)
	{
		DeleteLog(LOOP_FAIL_RECORD);
		DeleteLog(LOOP_TIME_RECORD);
		DeleteLog(LOOP_FAIL_CNT);
		LoopFailCnt();
	}
	*/

	//DeleteLog(LOOP_TIME_RECORD);
	fid = open(LOOP_FIRST_ERR_RECORD,O_CREATE);
	close(fid);

	memset(LoopStruct,0x00, sizeof(LoopStruct));
    if(fexist(LOOP_TIME_RECORD)<0)  //判断是否已经进行过煲机测试
    {
        fid = open(LOOP_TIME_RECORD, O_CREATE);
        if(fid > 0)
		{
            write(fid,(uchar*)LoopStruct,sizeof(LoopStruct));//是赋值了5个结构体的数据，而且都是初始化为0
            close(fid);
        }
    }
    fid = open(LOOP_TIME_RECORD,O_RDWR);
    if(fid > 0)
    {   
		//把前四次的数据 后移 一个位置，每次煲机都是记录在第一个位置  
        seek(fid, 0*sizeof(LOOP_STRUCT), SEEK_SET);           
        iRet=read(fid, (uchar*)&LoopStruct[1], 4*sizeof(LOOP_STRUCT));
        seek(fid, 1*sizeof(LOOP_STRUCT), SEEK_SET);           
        iRet=write(fid, (uchar*)&LoopStruct[1], 4*sizeof(LOOP_STRUCT));
    }
    else
    {
        ShowFileCtrlErr(__LINE__);
        return; 
    }
	
	//每次煲机的数据都是记录在LoopStruct[0]里面
    seek(fid,0, SEEK_SET);
    GetTime(LoopStruct[0].StartTime);  
    write(fid,(uchar*)&LoopStruct[0], sizeof(LOOP_STRUCT)); //记录开始煲机时间

    TimerSet(4, TIMEOUT);
	LoopStartMs=GetTimerCount();
    while(1)
    {
        loops++;
        if(!TimerCheck(4) || ExitFlag==1)//	退出煲机或者每5分钟到时记录到文件
        {
			LoopTempMs=GetTimerCount();
			LoopStruct[0].LoopSeconds=(LoopTempMs-LoopStartMs)/1000;
			LoopStruct[0].LoopCnt=loops;
			GetTime(LoopStruct[0].StopTime);  
			seek(fid,0, SEEK_SET);
			write(fid,(uchar*)&LoopStruct[0], sizeof(LOOP_STRUCT)); //记录开始煲机时间
            TimerSet(4, TIMEOUT);

			if(ExitFlag==1)
			{
				close(fid);
				break;
			}
        }
		//各项煲机测试
		kbflush();//清除键值
		if(TestModule[T_MAG]=='Y')
		{
			MagOpen();//打开磁卡
			MagReset();//清除磁卡缓存
		}
		for(i=0;i<AllNum;i++)
		{
			if(TestCycItem[i].flag=='Y')
			{
				ucRet=TestCycItem[i].TestCycFun(loops);
				if(ucRet || CancelCheck())
				{
					ExitFlag=1;
					break;
				}
			}//if(TestItem[i].flag='Y')
		}//for
		PiccLight(PICC_LED_ALL,0);
    }

	BtClose();
	WifiClose();
	
}


void TestSingle(void)
{
	TESTSINGLE_ITEM TestItem[]={
		{"蜂鸣器",	"BUZ",		BeepTestAll,	},
		{"时钟",	"CLK",		ClkTestAll,		},
		{"液晶屏",	"LCD",		LcdTestAll,		},
		{"按键",	"KEY",		KbTestAll,		},
		{"IC卡",	"ICC",		IcTestAll,		},
		{"LED",		"LED",		LedTestAll,		},
		{"磁卡",	"MAG",		MagTestAll,		},
		{"打印机",	"PRT",		PrinterTestAll,	},
		{"射频卡",	"RFC",		RfTestAll,		},
		{"串口",	"COM",		PortTestAll,	},
		{"OTG",		"OTG",		ComTestAll,		},
		{"无线",	"WNET",		WnetTestAll,	},
		{"USB从",	"USBD",		UsbDevTestAll,	},
		{"LED灯",	"LED",		LedTestAll,		},
		{"电池",	"BAT",		BatteryTestAll,	},
		{"WIFI",	"WIFI",		WifiTestAll,	},
		{"USB主",	"USBH",		UDiskTestAll,	},
		{"扬声器",	"SPKR",		SpeechTestAll,	},
		{"SD卡",	"SDHC",		SDCardTestAll,	},
		{"触摸屏",	"TP",		TsTestAll,		},
		{"扫描",	"SCAN",		ScanTestAll		},
		{"日志",	"Log",		LogTestAll,		},
		{"密钥",	"PED",		PEDTestAll,		},		
	};
	uchar TestNum;
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"单项测试","Single Test",2);
}

void TestAll(void)
{
	test_all(0);
}







