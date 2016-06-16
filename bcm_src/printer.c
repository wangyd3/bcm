#include "public.h"

void ShowPrinterErr(uchar Line,uchar ucRet)
{
	switch(ucRet)
	{
		case 0x01:	ScrDispStr(0,Line,1,"打印机忙:%02x",		"Prn Busy:%02x",		ucRet);break;
		case 0x02:	ScrDispStr(0,Line,1,"打印机缺纸:%02x",		"No Paper:%02x",		ucRet);break;
		case 0x03:	ScrDispStr(0,Line,1,"数据包格式错:%02x",	"Data Fomat Err:%02x",	ucRet);break;
		case 0x04:	ScrDispStr(0,Line,1,"打印机故障:%02x",		"Prn Fault:%02x",		ucRet);break;
		case 0x08:	ScrDispStr(0,Line,1,"打印机过热:%02x",		"Prn Superheat:%02x",	ucRet);break;
		case 0xf0:	ScrDispStr(0,Line,1,"打印未完成:%02x",		"Prt unfinished:%02x",	ucRet);break;
		case 0xfc:	ScrDispStr(0,Line,1,"打印机未装字库:%02x",	"No FontLib:%02x",		ucRet);break;
		case 0xfe:	ScrDispStr(0,Line,1,"数据包过长:%02x",		"Data Length Err:%02x",	ucRet);break;
		default:	ScrDispStr(0,Line,1,"其他错误:%02x",		"Other Err:%02x.",		ucRet);break;
	}
}

//获取黑块
void GetBlackBlock(uint Line)//每行8个点的高度
{
	uchar i,LogoHigh;
	uchar LogoBuff[5000];
	if(TermInfo[1] == 'T'){//热敏打印机
		memset(LogoBuff,(unsigned char)0xff,sizeof(LogoBuff));
		LogoHigh=8;
		LogoBuff[0]=LogoHigh;//8行（其实每一行高度1个点，每行384个点）
		for(i=0; i<LogoHigh; i++){
			LogoBuff[i*50+1]=0x00;
			LogoBuff[i*50+2]=0x30;
		}

	}
	else{//针打式打印机
	    memset(LogoBuff,(unsigned char)0xff,sizeof(LogoBuff));
		LogoHigh =1;
	    LogoBuff[0]=LogoHigh;//1行（其实每行高度为8个点，每行180个点）
	    for(i=0; i<LogoHigh; i++){
		    LogoBuff[i*182+1]=0x00;
		    LogoBuff[i*182+2]=0xb4;
	    }
    }

	for(i=0;i<Line;i++){
		PrnLogo(LogoBuff);
	}
	
}


//连续打印黑块
void PrintLogo(void)
{
	uchar ucRet;
	uint count=0;
	ScrClrLine(0,7);
	while(1)
	{
		if(CancelCheck()) return;
		ScrDispStr(0,0,1,"正在打印,页%d..","Printing,page%d..",++count);        
		PrnInit();
		GetBlackBlock(32);
		PrnStep(80);
		ucRet=PrnStart();
		if(ucRet!=0){
			BeepFail();
			ShowPrinterErr(4,ucRet);
			DelayMs(500);
			return;
		}
	}
	
}


void PrintAlphaH(void)
{
    int PageNum=0;
    uchar i,ucRet;
    ScrClrLine(0,7);
    ScrDispStr(0,0,1, "连续H打印测试","Multi-H Test ");
    while(1)
    {
		if(CancelCheck()) break; 
        ScrDispStr(0,4,1,"正在打印,页%d...","Printing Page%d ",++PageNum);
		PrnInit();
        PrnFontSet(1, 1);
        for(i=0;i<20;i++)
            PrnStr("HHHHHHHHHHHHHHHHHHHHHH\n");
        PrnStr("\f");
		ucRet=PrnStart();
        if(ucRet!=0)
        {
			ShowPrinterErr(6,ucRet);
            BeepFail();
			DelayMs(1500);
			break;
        }
    }//while(1)
}

uchar PrinterTest (uchar flag)
{
	int i,count,BlockNum=0,Temperature=25;
	unsigned char ucRet,buff[10];
	uchar prntStr[10], prntStrCmp[10];
	count=0; 
	

	ScrClrLine(2,7);
	if(flag!=0)gTemperature=1;
	if(TermInfo[1]=='T')
	{
		Temperature=PrnTemperature();
		if( gTemperature==1 && (VerInfo[1]==3 || TermInfo[0]==S900 || TermInfo[0]==S800) )
		{
			RECORD(1,"..Has ComPare Temperature %d",Temperature);
			if(Temperature>35 || Temperature<15)
			{
				
				ScrClrLine(2,5);
				sprintf(gErrnoMsg,"Temperature=%d ",Temperature);
				ScrDispStr(0,2,1,"打印温度%d","Temperature=%d ",Temperature);
				BeepFail();
				DelayMs(500);
				if(flag==0)
				{
					getkey();
					goto ENDERR;
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
		
		ScrClrLine(2,7);
		if (flag == 0) ScrDispStr(0,2,1,"整机打印测试","Inter PRN Test");
		else if (flag == 1) ScrDispStr(0,0,1,"单页打印测试","PRN Single-Page");
		else if (flag == 2) 
		{
			ScrDispStr(0,0,0x81,"多页打印  %6d","MultiPage %6d",++count);
			if(CancelCheck()) return 1;
		}

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
		
		PrnStr("EXSN:[%s]\n",gExSn);//刚好一行，所以不加|| 
		if(flag != 0)
		{
			PrnStr("|| SN  :[%s]\n",gSn);
			PrnStr("|| BIOS:%02u  MON:%u.%02u\n",VerInfo[0],VerInfo[1],VerInfo[2]);
			
			if(VerInfo[3]==0xff)
			{
				uchar MainVer,PortVer,SamVer;
				if(InfoOut[C_MAIN_BOARD][0]!=0)
				{
					MainVer=atoi(&InfoOut[C_MAIN_BOARD][1]);
					PrnStr("|| MAIN:%02d  \n",MainVer);
				}
				if(InfoOut[C_PORT_BOARD][0]!=0)
				{
					PortVer=atoi(&InfoOut[C_PORT_BOARD][1]);
					PrnStr("|| PORT:%02d  \n",PortVer);
				}
				if(InfoOut[C_SAM_BOARD][0]!=0)
				{
					SamVer=atoi(&InfoOut[31][1]);
					PrnStr("|| SAM:%02d  \n",SamVer);
				}
			}
			else PrnStr("|| MAIN:%02x  PORT:%02x\n",  VerInfo[3],VerInfo[4]);


			PrnStr("|| MAGB : %02x\n",VerInfo[6]);
			PrnStr("|| CPU  : ARM11 32BIT\n");
			PrnStr("|| SDRAM:%02dMB FLASH    :%dMB\n", gSdramSize,gFlashSize);
			PrnStr("|| DDR  :%02dMB NANDFLASH:%dMB\n", gDdrSize,gNandFlashSize);

			PrnStr("|| WNET:[%c]  GPRS :[%c]\n",TestModule[T_WNET],TestModule[T_GPRS]);
			PrnStr("|| CDMA:[%c]  WCDMA:[%c]\n",TestModule[T_CDMA],TestModule[T_WCDMA]);
			PrnStr("|| WIFI:[%c]  BT   :[%c]\n",TestModule[T_WIFI],TestModule[T_BT]);
			PrnStr("|| RF  :[%c]  IP   :[%c]\n",TestModule[T_RF],TestModule[T_LAN]);
			PrnStr("|| MODEM:[%c] %s,%s \n",TestModule[T_MODEM],gModemSVer,gModemHVer);
			if(TestModule[T_LAN]=='Y')
				PrnStr("|| MAC :%02x%02x%02x%02x%02x%02x\n",gMac[0],gMac[1],gMac[2],gMac[3],gMac[4],gMac[5]);

			if(TestModule[T_WNET]=='Y')
				PrnStr("|| WNET:%s\n",gWnetVer);
			
			if(TermInfo[0]==S60)
			{
				PrnStr("|| B_FONT :%s\n", BaseFontSize);
				PrnStr("|| B_MAIN :%s\n", BaseMain);
				PrnStr("|| B_BIOS :%s\n", BaseBios);
				PrnStr("|| DRIVER :%s\n", BaseDriver);
				PrnStr("|| B_FLASH :%s\n", BaseFlashSize);
				PrnStr("|| B_SDRAM :%s\n", BaseSdramSize);
			}
			PrnStr("|| abcdefghijklmnopqrstuvwxyz\n");
			PrnStr("|| ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
			PrnStr("|| !@#$%%&*()_-+=[]{}0\n");
			PrnStr("|| ,;.<>|\\/?123456789\n");

		}//if(flag!=0)
		PrnStr("|| PRN:%s %s\n",gPrinterType,InfoOut[C_PRINTER]);
		PrnStr("|| FONT : %s\n",gFontFileName);
	    PrnStr("|| SIZE : %dB\n",gFontLength);
		//PrnStr("|| CONFIG NAME:%s\n",gConfigFileName);
		if( !(TermInfo[0]==S60 && VerInfo[1]!=3) )
			PrnStr("|| Temperature:%d\n",Temperature);
		if(FontType == CHINESE)//有中文字库打印繁体	
		{
		    PrnStr("|| A.啊b.模 _铡-+齄12\n");
		    PrnStr("|| 线处账误录异茗 \n");
			PrnStr("|| 羉砰いゅ\n"); // "羉砰いゅ" big5 为"繁體中文"
		}
		GetTime(buff);
		PrnStr("|| 20%02x/%02x/%02x %02x:%02x:%02x %d\n",
				buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6]);

		if(flag==0 || flag==1)
		{
			GetBlackBlock(6);//获取黑块
			PrnStr("\n       %s",prntStr);
		}

		if( TermInfo[1]=='S' ){
			PrnStr("\f");
		}
		else{
			PrnStep(100);
		}
		
		ucRet=PrnStart();
		if(ucRet)
		{
			sprintf(gErrnoMsg,"PrnStart=0x%02x",ucRet);
			ShowPrinterErr(4,ucRet);
			BeepFail(); 
			getkey();
			goto ENDERR;
		}
		if(flag==0)
		{
			memset(prntStrCmp, 0x00, sizeof(prntStrCmp));
			ScrDispStr(0,4,1,"请检查打印效果并","Pls check effect and");
			InputAsc("请输入底部数字串","input bottom string",prntStrCmp,3,3,0);
			if(memcmp(prntStr, prntStrCmp, 3))
			{
				ScrDispStr(0,6,1,"数字输入错误","EnterString Err");
				sprintf(gErrnoMsg,"enter string error");
				BeepFail();
				getkey();
				goto ENDERR;
			}
		}
		BeepOK();
		if(flag!=2)goto ENDOK;
		
	}// 1

ENDOK:
	return 0;
ENDERR:
	return 1;
	
}


void PrinterTest1(void)
{
	PrinterTest(1);
}
void PrinterTest2(void)
{
	PrinterTest(2);
}

void PrinterTestAll(void)
{
	
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单页",	"1-Single",	PrinterTest1,},
		{"2-多页",	"2-Multi",	PrinterTest2,},
		{"3-黑块",	"3-Block",	PrintLogo,	 },
		{"1-连续H",	"1-MultiH",	PrintAlphaH, }
	};

	if(TestModule[T_PRINTER]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"没有打印机模块","No Priter Module");
		getkey();
		return;
	}

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"打印机测试","Printer Test",3);

}
