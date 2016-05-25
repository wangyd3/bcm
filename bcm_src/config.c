#include "public.h"

uchar gKeyWordNum;
const char KeyWord[][20]=
{
	"PRINTER",		"TOUCH_SCREEN",		"MAG_CARD",			"IC_CARD",
	"RF_1356M",		"RF_24G",			"G_SENSOR",			"ETHERNET",
	"MODEM",		"CDMA",				"GPRS",				"WCDMA",
	"TD",			"WIFI_NET",			"BAR_CODE",			"BLUE_TOOTH",
	"SD_READER",	"CONFIG_FILE_VER",	"MAIN_BOARD",		"PORT_BOARD",
	"MODEM_BOARD",	"RF_BOARD",			"ANT_BOARD",		"GPRS_BOARD",
	"WCDMA_BOARD",	"TD_BOARD",			"WIFI_BOARD",		"KEY_BOARD",
	"IP_BOARD",		"PRINT_BOARD",		"POWER_BOARD",		"SAM_BOARD",
	"ICCARD_BOARD",	"MESH_BOARD",		"TELEPHONE_BOARD",	"DISP_BOARD",
	"RF_PARA",		"G_SENSOR_PARA",	"DUAL_SIM",			"PN",
	"RF_PARA_1",	"RF_PARA_2",		"RF_PARA_3",		"LCD",
	"SM_CHIP",		"CIPHER_CHIP",
};

void GetConfigFileName( char *gConfigFileName)
{
	int iRet,i;
	FILE_INFO fileinfo[256];
	
	iRet = GetFileInfo(fileinfo);
	if(iRet > 0)
	{
		for(i=0; i<iRet; i++)
		{
			if(strstr(fileinfo[i].name, "Config"))
			{
				strcpy(gConfigFileName,fileinfo[i].name);
				return;
			}
		}
	}	
	strcpy(gConfigFileName,"No ConfigFile");
}

uint GetFontInfo( char *gFontFileName)
{
    FILE_INFO FileInfo[256];
    uint FileCnt, i,FontLength=0;
    ulong fileFreeSize=0, fileUseSize=0, flshSize=0;
	
    fileFreeSize = freesize();
    FileCnt = GetFileInfo(FileInfo);
    
    for(i=0; i<FileCnt; i++)
    {
        fileUseSize += FileInfo[i].length;  //统计已用文件空间
        if((FileInfo[i].attr == 0xff) && (FileInfo[i].type == 0x02))   //判断是字库文件
        {
            FontLength = FileInfo[i].length;
        }
    }
    switch(FontLength)
    {
	    case 996336:	strcpy(gFontFileName,"FONT_GB2312_CHN_01");	break;
	    case 448112:	strcpy(gFontFileName,"FONT_GB2312_CHN_02");	break;
	    case 2034320:	strcpy(gFontFileName,"FONT_BIG5_HKG_01");   break;
	    case 971728:	strcpy(gFontFileName,"FONT_BIG5_HKG_02");   break;
	    case 195776:	strcpy(gFontFileName,"FONT_EUR_INT_01");    break;
	    case 817168:	strcpy(gFontFileName,"PAX-FONT-A(V1.0)");	break;
	    case 268944:    strcpy(gFontFileName, "PAX-FONT-B(V1.0)");	break;
	    case 25280:		strcpy(gFontFileName,"PAX-FONT-C(V1.0)");	break;
	    case 16192:		strcpy(gFontFileName, "PAX-FONT-D(V1.0)");  break;
	    case 650954:    strcpy(gFontFileName,"PAX-FONT-E(V1.0)");   break;
	    case 492543:    strcpy(gFontFileName,"pax-font-a(v1.0)_c"); break;//压缩字库
	    case 404393:    strcpy(gFontFileName,"pax-font-e(v1.0)_c"); break;//压缩字库
	    case 791504:    strcpy(gFontFileName, "P58_FONT");          break;
	    case 437296:    strcpy(gFontFileName,"P58_FONT_CN_SIMP");   break;
	    case 184960:    strcpy(gFontFileName, "PAX-FONT-EU");       break;
	    case 1956432:   strcpy(gFontFileName,"PAX-FONT-BIG5-E-V1.0");break;
	    case 2023504:   strcpy(gFontFileName,"091217-PAX-FONT-BIG5-A");break;
		case 960912:    strcpy(gFontFileName,"FONT-BIG5-HKG-02-H"); break;		
		case 439916:	strcpy(gFontFileName,"FONT_EUR_INT_02");	break;
		case 9992028:	strcpy(gFontFileName,"FONT_GBK_CHN_03");	break;
		case 6727004:	strcpy(gFontFileName,"FONT_BIG5_HKG_03");	break;
		case 3526492:	strcpy(gFontFileName,"FONT_GB2312_CHN_04");	break;
	    default:        strcpy(gFontFileName,"FONT_UNKNOW");		break;    
   	}  
	return FontLength;
}


void GetSoVer(void)
{
#ifdef LIB_SO
	int ret;
	uint index=0;
	SO_INFO sofullinfo;
	while(1)
	{
		ret=MpReadSoInfo(index,NULL,&sofullinfo);
		if(ret < 0)break;
		else 
		{
			ComSends("%d %s %s",index,sofullinfo.head.so_name,sofullinfo.head.version);
			RECORD(0,".%s=%s",sofullinfo.head.so_name,sofullinfo.head.version);
			index++;
		}
		if(CancelCheck()) break;
	}
#endif
}

void GetConfig(void)
{
	uchar ucRet;
	int i;
	memset(TermInfo,0,sizeof(TermInfo));
	memset(VerInfo,0,sizeof(VerInfo));
	memset(InfoOut,0,sizeof(InfoOut));

	GetTermInfo(TermInfo);

	
	ReadVerInfo(VerInfo); //boot、monitor、主板、接口板等信息
	if(TermInfo[0]==S60)
		ScrSetEcho(1);   //打开显示回显，供S60手机离开座机时显示提示信息	
	gKeyWordNum = sizeof(KeyWord)/sizeof(KeyWord[0]);
	/*
	if( TermInfo[0]==S500 || TermInfo[0]==S200 || TermInfo[0]==T52 || TermInfo[0]==S58 || (VerInfo[1]==3 && VerInfo[2]>=2) 
		|| TermInfo[0]==S800 || TermInfo[0]==S300 || TermInfo[0]==S900 || TermInfo[0]==T60
		|| TermInfo[0]==T610 || TermInfo[0]==T620 || TermInfo[0]==SP30)
	*/
	{	
		for(i=0;i<gKeyWordNum;i++)
			ucRet=GetHardwareConfig(KeyWord[i],InfoOut[i],25);
	}
	
	memset(TestModule,'N',sizeof(TestModule));
	for(i=0;i<21;i++)//gTermInfo 目前有0~20有效
	{
		if(TermInfo[i]==0)
			TestModule[i]='N';
		else
			TestModule[i]='Y';
		ComSends("%d-%c-%d",i,TestModule[i],TermInfo[i]);
	}


	
	//彩屏和触屏的判断
	if( (TermInfo[19]&0x01)==0 )TestModule[T_TSCREEN]='N';
	else TestModule[T_TSCREEN]='Y';

	if( (TermInfo[19]&0x02)==0 )gIsColorLcd = 0;
	else gIsColorLcd = 1;
	
	//蓝牙配置
	if( (TermInfo[19]&0x08)==0 )TestModule[T_BT]='N';
	else TestModule[T_BT]='Y';
	//彩屏的一些信息
	if(gIsColorLcd==1 || TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900 || InfoOut[C_LCD][0]!=0)
		CLcdGetInfo(&stLcdInfo);
	//ScrPrint(0,4,1,"H%d,W%d,%d,%d",stLcdInfo.height,stLcdInfo.width,stLcdInfo.cpp,stLcdInfo.lpp);
	//getkey();

	TestModule[T_BEEP]='Y';
	TestModule[T_KB]='Y';
	TestModule[T_LCD]='Y';
	TestModule[T_CLK]='Y';

	
	//有RS232B口
	if(
		TermInfo[0]==S60 || TermInfo[0]==MT30 || TermInfo[0]==T52 || TermInfo[0]==S810 || TermInfo[0]==S300
		|| TermInfo[0]==S78 || (TermInfo[0]==SP30 && TermInfo[1]!=0)
		|| (TermInfo[0]==S80 && TermInfo[8]==0 && TermInfo[11]==0)
		|| (TermInfo[0]==S800 && TermInfo[8]==0 && TermInfo[11]==0)
		|| (TermInfo[0]==S500 && TermInfo[8]==0 && TermInfo[11]==0)
	)
		TestModule[T_COMB]='Y';
	else
		TestModule[T_COMB]='N';	

	if( strstr(InfoOut[C_PN],"S58-MGL-263-0CCC")!=NULL
		|| strstr(InfoOut[C_PN],"S58-M0L-263-0HCC")!=NULL )//S58有部分预留双串口
		TestModule[T_COMB]='Y';


	if(TermInfo[0]==S58 && strstr(InfoOut[C_PORT_BOARD],"39"))//S58 PORT_BOARD="V39", S500 PORT_BOARD="V09"
		TestModule[T_COMB]='Y';
	if(TermInfo[0]==S500 && strstr(InfoOut[C_PORT_BOARD],"09"))
		TestModule[T_COMB]='Y';
	if(TermInfo[0]==S800 && strstr(InfoOut[C_PORT_BOARD],"42"))
		TestModule[T_COMB]='Y';

	TestModule[T_COMA_TEST]='N';
#ifdef Venezuela
	if(TermInfo[0]!=S90)
		TestModule[T_COMA_TEST]='Y';
#endif

	//通过PN号判断SP30的各种情况
	//SP30-00L-XXX-XXXX  以太网 + 双串口（左侧USB 为OTG功能）
    //SP30-0UL-XXX-XXXX 以太网 + USB Device + 单串口（SP30左侧没有USB口，不需要测试U盘）
    //SP30-000-XXX-XXXX  单串口配置 （左侧USB为OTG功能）
    //SP30-0U0-XXX-XXXX 单USB配置（USB只有Device功能，不需要测试U盘）
	if( strstr(InfoOut[C_PN],"SP30-00L")!=NULL)
	{
		TestModule[T_COMB] = 'Y';
		TestModule[T_LAN] = 'Y';
		TestModule[T_UDISK] = 'Y';
		TestModule[T_USBHOST] = 'Y';
		TestModule[T_USBDEV] = 'Y';
		TestModule[T_UDEV_TEST]= 'N';
		gPPQcom = P_RS232A;
	}
	else if( strstr(InfoOut[C_PN],"SP30-0UL")!=NULL) //改成USB上传PPQ,测试RF后USB是否OK  //16-03-16 更改回串口上传,USB不测试
	{												 //改为测试USBD,测试USBD的时候打开RF(2016-05-06)
		TestModule[T_COMB] = 'N';
		TestModule[T_LAN] = 'Y';
		TestModule[T_UDISK] = 'N';
		TestModule[T_USBHOST] = 'N';
		TestModule[T_USBDEV] = 'Y';
		TestModule[T_UDEV_TEST]= 'Y';
		gPPQcom = P_RS232A;
		//gPPQcom = P_USB_DEV;
		TestModule[T_COMA_TEST]='N';
	}
	else if( strstr(InfoOut[C_PN],"SP30-000")!=NULL)
	{
		TestModule[T_COMB] = 'N';
		TestModule[T_LAN] = 'N';
		TestModule[T_UDISK] = 'Y';
		TestModule[T_USBHOST] = 'Y';
		TestModule[T_USBDEV] = 'Y';
		TestModule[T_UDEV_TEST]= 'N';
		gPPQcom = P_RS232A;
	}
	else if( strstr(InfoOut[C_PN],"SP30-0U0")!=NULL)
	{
		TestModule[T_COMB] = 'N';
		TestModule[T_LAN] = 'N';
		TestModule[T_UDISK] = 'N';
		TestModule[T_USBHOST] = 'N';
		TestModule[T_USBDEV] = 'Y';
		TestModule[T_UDEV_TEST]= 'N';
		gPPQcom = P_USB_DEV;
	}

	if(TermInfo[0]==S910)
	{
		gPPQcom = P_USB_DEV;
	}

	/* S300-000-363-XXXX 和其他的S300
	  默认都是跟02NA这种配置:串口+LAN+USB,USB不测试A口上传
	  04NA,01NA:双串口+LAN, A口上传
	  03NA,串口+双LAN+USB,USB不测试A口上传
	*/

	if(TermInfo[0]==S300)TestModule[T_COMB_TEST]='N';
	else TestModule[T_COMB_TEST]=TestModule[T_COMB]; 

	if( strstr(InfoOut[C_PN],"S300-000-363-04NA")!=NULL || strstr(InfoOut[C_PN],"S300-000-363-01NA")!=NULL )
	{
		TestModule[T_COMB_TEST]='Y';
	}

	if( strstr(InfoOut[C_PN],"S300-000-363-03NA")!=NULL) //S300增加双LAN功能
	{
		TestModule[T_LAN2]='Y';
	}



	// 旁置电话
	if(TermInfo[0]==S810 || TermInfo[0]==S78 || TermInfo[0]==S500 || (TermInfo[0]==S800 && InfoOut[C_MAIN_BOARD][1]=='2') )
	{
		if(TestModule[T_MODEM]=='Y')
			TestModule[T_BYPHONE]='Y';
		else
			TestModule[T_BYPHONE]='N';
	}
	else
		TestModule[T_BYPHONE]='N';


	//S58特殊
	if(TermInfo[0]==S58 && TermInfo[8]==0 && TermInfo[11]==0)
	{
		if(InfoOut[C_PORT_BOARD][2]=='2')//PORT_BOARD="V32"
			TestModule[T_COMB]='Y';
		else if(InfoOut[C_PORT_BOARD][2]=='6')//PORT_BOARD="V36"
			TestModule[T_BYPHONE]='Y';
	}


	// 无线
	if(TermInfo[9]!=0 || TermInfo[10]!=0 || TermInfo[18]!=0)
		TestModule[T_WNET]='Y';
	else
		TestModule[T_WNET]='N';

	// U盘/USB HOST
	TestModule[T_UDISK] = TestModule[T_USBHOST];
	if( strstr(InfoOut[C_PN],"S300-000-363-04NA")!=NULL || strstr(InfoOut[C_PN],"S300-000-363-01NA")!=NULL )
	{
		TestModule[T_UDISK]='N';
		TestModule[T_USBHOST] = 'N';
		TestModule[T_USBDEV] = 'N';
	}

	
	// S800是有两个USB口的
	if(TermInfo[0]==S800) TestModule[T_UDISKA] = 'Y';
	else TestModule[T_UDISKA] = 'N';
	

	// 语音
	if(TermInfo[0]==T610 || TermInfo[0]==T620 || TermInfo[0]==T60 || TermInfo[0]==T52
		|| TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900)//语音
		TestModule[T_SPEECH]='Y';
	else
		TestModule[T_SPEECH]='N';

	// 电池
	if(TermInfo[0]==S910 || TermInfo[0]==D200 || TermInfo[0]==D210 || TermInfo[0]==T60 || TermInfo[0]==S90 || TermInfo[0]==S900)//电池
		TestModule[T_BATTERY]='Y';
	else
		TestModule[T_BATTERY]='N';

	// PINPAD
	if(TermInfo[0]==S60 || TermInfo[0]==S78 || TermInfo[0]==T620 || TermInfo[0]==S500 ||
		TermInfo[0]==S810 || TermInfo[0]==S80 || TermInfo[0]==S58 || TermInfo[0]==S800)
		TestModule[T_PINPORT]='Y';
	else 
		TestModule[T_PINPORT]='N';

	// FeliCa
	if(TermInfo[12]>=2)
		TestModule[T_FELICA]='Y';
	else
		TestModule[T_FELICA]='N';
	
	if( strstr(InfoOut[C_PN],"S58-M0L-232-01EE")!=NULL ) //只有这个配置整机需要测试FELICA
		TestModule[T_FELICA_ALL]='Y';
	else
		TestModule[T_FELICA_ALL]='N';

	
	// LED lights
	if( TermInfo[12]==0 || TermInfo[0]==S500 || TermInfo[0]==D200 || TermInfo[0]==D210 
		|| TermInfo[0]==S900 || TermInfo[0]==S800 || TermInfo[0]==S300 )
	{
		TestModule[T_LED]='N';
	}   //PCI3.X的才有LED灯,老的S没有LED灯 S900-0G0-363-06CC/S90-0C0-063-01EE   
	else if( strstr(InfoOut[C_PN],"3-") || TermInfo[0]==SP30 || TermInfo[0]==S910 || TermInfo[0]==S200 || TermInfo[0]==T60  || VerInfo[1]==3 )
	{
		TestModule[T_LED]='Y';
	}
	else
	{
		TestModule[T_LED]='N';	
	}
	

	
	// 针打
	if((TermInfo[0]==S60 || TermInfo[0]==SP30) && TermInfo[1]=='S')
		TestModule[T_PAPERBACK]='Y';
	else
		TestModule[T_PAPERBACK]='N';

	// BarCode
	if(InfoOut[C_BAR_CODE][0]!=0)
		TestModule[T_SCAN]='Y';
	else 
		TestModule[T_SCAN]='N';


	// 
	if(TermInfo[0]==T620 || TermInfo[0]==T60)
		TestModule[T_LEDPLY]='Y';
	else
		TestModule[T_LEDPLY]='N';
	

	if(TermInfo[0]==T52)
		TestModule[T_TEL]='Y';
	else
		TestModule[T_TEL]='N';


	if(InfoOut[C_SD_READER][0]!=0)//有这个关键字，但是配置文件却没有写进去，所以需要机型判断
		TestModule[T_SDCARD]='Y';
	else
	{	
		TestModule[T_SDCARD]='N';
		if(TermInfo[0]==T610 || TermInfo[0]==T620 || TermInfo[0]==S800 || TermInfo[0]==S900 || TermInfo[0]==MT30)
			TestModule[T_SDCARD]='Y';
	}

	
	if(TermInfo[0]==D200 || TermInfo[0]==D210 || TermInfo[0]==S90 || TermInfo[0]==S900)
	{
		TestModule[T_KEYLED]='Y';
	}
	else
	{
		TestModule[T_KEYLED]='N';
	}

	if(TermInfo[0]==D200 && InfoOut[C_MAIN_BOARD][1]=='3') //D200Q V30~V39没有KEYLED
		TestModule[T_KEYLED]='N';
	


	if(TermInfo[0]==S78 && VerInfo[1]<3 && TestModule[T_USBDEV]=='Y')
	{
		TestModule[T_UDEV_TEST]='Y';//老的S78需要测试USBDEV
	}
	else if( strstr(InfoOut[C_PN],"SP30-0UL")!=NULL)
	{
		TestModule[T_UDEV_TEST]='Y';//这需要测试USBD,并且测试得到时候需要打开RF模块
	}
	else 
	{
		TestModule[T_UDEV_TEST]='N';
	}

	FontType = TermInfo[13];    //字库类型

	//SIM卡个数
	//没有配置文件和有配置文件同时判断sim卡个数的情况
	if(TermInfo[9]==0 && TermInfo[10]==0 && TermInfo[18]==0)
		gSimNum=0;
	else if((TermInfo[9]==0x12 && TermInfo[0]==S90) || (TermInfo[9]==0x16 && TermInfo[0]==S90))//S90 wavecom 双SIM卡；0x16：G610双SIM机型
	{
		gSimNum=2;
	}
		
	else if(((TermInfo[9] == 0x14 || TermInfo[9] == 0x16)) && (TermInfo[0] == S80) )   //S80 wavecom 双SIM卡
	{
		gSimNum=2;
	}
	else if(InfoOut[C_DUAL_SIM][0]!=0) gSimNum=2;
	else gSimNum=1;
	
	
	if(gSimNum==2)
		TestModule[T_SIM1]='Y';
	else
		TestModule[T_SIM1]='N';

	//国密SM2
	if(InfoOut[C_SM_CHIP][0]!=0 || InfoOut[C_CIPHER_CHIP][0]!=0) TestModule[T_SM2]='Y';
	else TestModule[T_SM2]='N';


	if(TermInfo[0]==D210 || TermInfo[0]==D200)
		TestModule[T_BASE] = 'Y';
	else
		TestModule[T_BASE] = 'N';
	

	//TestModelue0--41都必须赋值 全部可能需要测试的项目
	for(i=0;i<45;i++)
	{
		if(TestModule[i]==0)
		{
			ScrDispStr(0,6,0x01,"配置错误%d!!","Config Error%d!!",i);
			getkey();
		}
	}

	/*
	//有配置文件判断情况
	if(gTermInfo[9]==0 && gTermInfo[10]==0 && gTermInfo[18]==0)
		gSimNum=0;
	else if(InfoOut[C_DUAL_SIM][0]==0)
		gSimNum=1;
	else 
		gSimNum=2;
	*/
	
	//IC卡个数 包括IC卡和SAM卡总数
	//只能根据机型和无线配置判断，如果可以的话加上关键字IC_NUM,SAM_NUM;

	switch(TermInfo[0])
	{		
		case S200:
			gIcNum = 0;
			gSamNum = 2;
			break;
		case D210:
			gIcNum=1;
			if(gSimNum==2)gSamNum = 1;
			else gSamNum = 2;
			break;
		case D200:
		case S910:
			gIcNum=1;
			if(gSimNum==0)gSamNum = 2;
			else if(gSimNum==1)gSamNum = 1;
			break;
		case T610:
		case T620:
			gIcNum=0;
			if(gSimNum==0)gSamNum = 4;
			else if(gSimNum==1)gSamNum = 3;
			else if(gSimNum==2)gSamNum = 2;
			break;
		case MT30:
			gIcNum=1;
			if(gSimNum==0)gSamNum = 4;
			else if(gSimNum==1)gSamNum = 3;
			else if(gSimNum==2)gSamNum = 2;
			break;
	
#ifdef SP30_POS //不是小SP30
		case SP30:
			gIcNum=1;
			gSamNum=4;
			break;
#else          //小SP30
		case SP30:
			gIcNum=1;
			if(gSimNum==0)gSamNum=3;
			else if(gSimNum==1)gSamNum=2;
			break;
#endif
		case S78:
			gIcNum=1;
			if( gSimNum==1 && VerInfo[1]>=3 ) gSamNum = 3;//小s的外面是有4个卡槽，但是非小s如果有无线的有5个卡槽	
			else gSamNum = 4;
			break;
			
		case S58:
		case S60:
			gIcNum=1;
			if( gSimNum==1 && VerInfo[1]>=3 )gSamNum=2;
			else gSamNum=3;
			break;
				
		case S80:
			gIcNum=1;
			if(gSimNum==0) gSamNum=3;//非小S没有无线，小S有一个无线卡槽是单独用的，如果没有无线则没有那个卡槽
			if(gSimNum==1) gSamNum=3;
			if(gSimNum==2) gSamNum=2;//双SIM卡
			break;
			
		case S900:
		case T60:
		case S90:  //S90不管如何最多只有两个sam卡槽
			gIcNum=1;
			if(gSimNum==0)gSamNum=2;
			if(gSimNum==1)gSamNum=2;
			if(gSimNum==2)gSamNum=1;
			break;
		case S500:
			gIcNum=1;
			if(gSimNum==0)gSamNum=3;
			if(gSimNum==1)gSamNum=2;
			if(gSimNum==2)gSamNum=1;
			break;

		case T52:
    		gIcNum=1;
			gSamNum=2;
			break;
			
		case S800:
		//case S900: //如果是0个sim则2个sam,跟S90的一样
		case S300:
			gIcNum=1;
			if(gSimNum==0)gSamNum=3;
			else if(gSimNum==1)gSamNum=2;
			else if(gSimNum==2)gSamNum=1;
			break;
			
		default:
			break;
	}


	if(InfoOut[C_IC_CARD][1]=='0') //如果是00则表示没有IC卡
		gIcNum = 0;


	//LAN的MAC地址
	if(TestModule[T_LAN]=='Y')
	{
		memset(gMac,0x00,sizeof(gMac));
		EthMacGet(gMac);
	}

	
	//NandFlash 大小 假设都有NandFlash，没有读出来为0
	{
		int iRet;
		FS_DISK_INFO info;
		memset(&info,0,sizeof(info));
		iRet = FsGetDiskInfo(1,&info);
		if (!iRet)
		{
			gNandFlashSize = (info.free_space+info.used_space)/(1024*1024);
			if (gNandFlashSize >= 126)
				gNandFlashSize = 128;
		}
		else
		{
			gNandFlashSize = 0;
		}
	}


	//Sn,ExSn,Modem Ver,
	memset(gSn,0x00,sizeof(gSn));
	memset(gExSn,0x00,sizeof(gExSn));
	memset(gModemSVer,0x00,sizeof(gModemSVer));
	memset(gModemHVer,0x00,sizeof(gModemHVer));
	memset(gLMDate,0x00,sizeof(gLMDate));
	memset(gOtVer,0x00,sizeof(gOtVer));
	memset(gConfigFileName,0x00,sizeof(gConfigFileName));
	memset(gFontFileName,0x00,sizeof(gFontFileName));

	ReadSN(gSn); //Sn 
	EXReadSN(gExSn);//Exsn
	s_ModemInfo(gModemSVer,gModemHVer,gLMDate,gOtVer);//ModemVer 这个必须modem打开后才能够正常返回

	//GetConfigFileName(gConfigFileName);//配置(小S如果没有配置文件的话，是会出现系统错误的，根本进入不了应用)
	gFontLength=GetFontInfo(gFontFileName);//字库
	
	
	//RF卡的测试高度
	if(   TermInfo[0]==T610 || TermInfo[0]==T620 || TermInfo[0]==MT30 || TermInfo[0]==D210 || TermInfo[0]==D200
		  || TermInfo[0]==S810 )
	{
		gRfHeight=3;
	}
	else if(TermInfo[0]==S78 || TermInfo[0]==S58 || TermInfo[0]==SP30 || TermInfo[0]==S90)
	{
		gRfHeight=2;
	}
	else
	{
		gRfHeight=3;
	}
	
}


/*GetInfoExt和GetWirelessVer函数执行时间比较长，所以不在开机前面就获取*/
/*在需要使用的时候才获取*/
int GetInfoExt(void)
{
	uchar Info[8*1024],OutInfo[30];
	int iInfoLen,FlagVLen,size,iRet;
	
	if(GetExtInfoFlag==1) return 0;
	else GetExtInfoFlag=1;
	
	ScrClrLine(2,7);
	ScrDispStr(0,2,1,"正在获取版本信息...", "Get Version Message...");
	memset(BaseFontSize, 0, sizeof(BaseFontSize));
	memset(BaseMain, 0, sizeof(BaseMain));
	memset(BaseBios, 0, sizeof(BaseBios)); 
	memset(BaseDriver, 0, sizeof(BaseDriver));
	memset(BaseFlashSize, 0, sizeof(BaseFlashSize));
	memset(BaseSdramSize, 0, sizeof(BaseSdramSize));
    
	//读取flash和sdram的大小
	memset(Info, 0, sizeof(Info));
	iInfoLen = GetTermInfoExt(Info, sizeof(Info));//////////////9s

	//读取Flash的大小
	memset(OutInfo,0,sizeof(OutInfo));		
	iRet = XmlGetElement(Info, iInfoLen, "FlashSize", OutInfo, sizeof(OutInfo),&FlagVLen);
	size = atoi(OutInfo);
	gFlashSize = size/1024;

	//读取DDR大小
	memset(OutInfo,0,sizeof(OutInfo));
	iRet = XmlGetElement(Info, iInfoLen, "DdrSize", OutInfo, sizeof(OutInfo),&FlagVLen);  
	size = atoi(OutInfo);
	gDdrSize = size/1024;

	//读取SDRAM的大小
	memset(OutInfo,0,sizeof(OutInfo));
	iRet = XmlGetElement(Info, iInfoLen, "SdramSize", OutInfo, sizeof(OutInfo),&FlagVLen);
	size = atoi(OutInfo);
	gSdramSize = size/1024;

	//打印信息可以GetHareWareConfig获取
	if(TermInfo[0]==T52 || TermInfo[0]==S58 || (VerInfo[1]==3 && VerInfo[2]>=2) 
		|| TermInfo[0]==S800 || TermInfo[0]==S300 || TermInfo[0]==S900)
	{
		//有配置文件的不需要这样获取打印机信息，直接配置文件读取
	}
	else if( TermInfo[1]!=0 )//没有配置文件的需要这样获取打印机信息
	{
		memset(OutInfo, 0, sizeof(OutInfo));
		memset(gPrinterType, 0, sizeof(gPrinterType));
		
		iRet = XmlGetElement(Info, iInfoLen, "PRINTER", OutInfo, sizeof(OutInfo),&FlagVLen);	
		if(!iRet && FlagVLen>0)
		{
			strcpy(gPrinterType, OutInfo);
		}
		else if(InfoOut[C_PRINTER][0]!=0)
		{
			strcpy(gPrinterType,InfoOut[C_PRINTER]);  //表示没有设定打印机类型    
		}
		else strcpy(gPrinterType,"***");
    }

    if(TermInfo[0] == S60 || (TermInfo[0] == SP30 && TermInfo[1]!=0)) //S60及有打印机的SP30有底座
    {
    	memset(OutInfo, 0, sizeof(OutInfo));
    	iRet = XmlGetElement(Info, iInfoLen, "BASEFONTSIZE", OutInfo, sizeof(OutInfo),&FlagVLen);
		if(!iRet && FlagVLen>0)
		{
			strcpy(BaseFontSize, OutInfo);
		}
		
		memset(OutInfo, 0, sizeof(OutInfo));
		iRet = XmlGetElement(Info, iInfoLen, "BASEMAIN", OutInfo, sizeof(OutInfo),&FlagVLen);
		if(!iRet && FlagVLen>0)
		{
			strcpy(BaseMain, OutInfo);
		}
		
		memset(OutInfo, 0, sizeof(OutInfo));
		iRet = XmlGetElement(Info, iInfoLen, "BASEBIOS", OutInfo, sizeof(OutInfo),&FlagVLen);
		if(!iRet && FlagVLen>0)
		{
			strcpy(BaseBios, OutInfo);
		}
	
		memset(OutInfo, 0, sizeof(OutInfo));
		iRet = XmlGetElement(Info, iInfoLen, "BASEDRIVER", OutInfo, sizeof(OutInfo),&FlagVLen);
		if(!iRet && FlagVLen>0)
		{
			strcpy(BaseDriver, OutInfo);
		}
		
		memset(OutInfo, 0, sizeof(OutInfo));
		iRet = XmlGetElement(Info, iInfoLen, "BASEFLASHSIZE", OutInfo, sizeof(OutInfo),&FlagVLen);
		if(!iRet && FlagVLen>0)
		{
			strcpy(BaseFlashSize, OutInfo);
		}
		
		memset(OutInfo, 0, sizeof(OutInfo));
		iRet = XmlGetElement(Info, iInfoLen, "BASESDRAMSIZE", OutInfo, sizeof(OutInfo),&FlagVLen);
		if(!iRet && FlagVLen>0)
		{
			strcpy(BaseSdramSize, OutInfo);
		} 
    }
	return 0;
}



int OldGetWirelessVer()
{
	int iRet;
	uint i;
	uchar rsp[300],rsp_wnet[300];
	uchar tmpVer[50],ModuleVer[50],WireVer[50];
	uchar verlen,*p;

	if(GetWnetVerFlag==1) return 0;
	else GetWnetVerFlag=1;

	if(TestModule[T_WNET]=='N')
	{
		return 0xff;
	}

	ScrClrLine(2, 7);
	ScrDispStr(0,2,0x01,"读取无线版本信息", "Get Wireless VerInfo");
	
	memset(ModuleVer, 0, sizeof(ModuleVer));
	memset(WireVer, 0, sizeof(WireVer));
	memset(gImei, 0, sizeof(gImei));

	iRet = WlInit(NULL);
	//RecordLog(WNET_TEST_RECORD,"WlInit(NULL)=%d,time=%d\n",iRet,1000-TimerCheck(0));
	//ComSends("WlInit(NULL)=%d,time=%d\n",iRet,1000-TimerCheck(0));
	if(iRet!=RET_OK && iRet!=WL_RET_ERR_NOSIM && iRet!=WL_RET_ERR_INIT_ONCE && iRet!=WL_RET_ERR_RSPERR)
	{
		ScrClrLine(4,7);
		BeepFail();
		ScrDispStr(0, 4, 0x01, "模块初始化失败\n %d ", "WNet Init Fail\n %d ",iRet);
		getkey();
		return iRet;
	}
  
    iRet=WlOpenPort();   //打开无线通讯串口
    if(iRet)
    {
        ScrClrLine(4,7);
        BeepFail();
        ScrDispStr(0,4,0x01,"打开通信串口失败:%d","Open Port Err:  %d",iRet);
        getkey();
		return iRet;
    }

	memset(rsp_wnet,0,sizeof(rsp_wnet));
    iRet=WlSendCmd(NULL, rsp_wnet, 100, 100, 50);
    for(i=0;i<3;i++)
    {
        if((i%3)==0) ScrPrint(0,6,0x01,".....");
        else if((i%3)==1) ScrPrint(0,4,0x01,"..........");
        else if((i%3)==2) ScrPrint(0,4,0x01,"...............");        
        memset(rsp_wnet, 0x00, sizeof(rsp_wnet));
        iRet=WlSendCmd((unsigned char*)"at+GMR\r", rsp_wnet, 100, 30000, 1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
        if(iRet || strstr((char *)rsp_wnet, "ERROR: 515") != NULL )  
        {
            DelayMs(500);
            continue;
        }
		break;
    }
    

	if(iRet)
	{
		ScrClrLine(4,7);
		BeepFail();
		ScrDispStr(0,4,0x01,"读无线模块版本失败:%d","Get WNet VerInfo Err:%d",iRet); 
		getkey();
		return iRet;
	}

	strcpy((char *)rsp,(char *)rsp_wnet);
	Wl_handle_ver(rsp,sizeof(rsp)); 
	ComSends("Line=%d,%s",__LINE__ ,rsp);
  
	if((p=strstr((char *)rsp,"Q24"))!=NULL)     //Q24模块
	{
		uchar ModuleVer1[50], ModuleVer2[50];
		uchar rsp1[300], rsp2[300];
		uchar *p1, *p2;

		if(strstr((char*)rsp,"PL002"))
		{
			strcpy(ModuleVer1,"Q24C ");
		}
		else if(strstr((char*)rsp,"PL001"))
		{
			strcpy(ModuleVer1,"Q24E ");
		}
		if((p-9)!=NULL)
		{
			memcpy(&ModuleVer1[5],p-9,4);  //657G00gg.Q24PL002
		}
		ModuleVer1[9]=0;
		//第2次读版本
		WlSendCmd((unsigned char*)"at+GMR\r", rsp_wnet, 100, 30000, 1000); 
		strcpy((char *)rsp1,(char *)rsp_wnet);
		Wl_handle_ver(rsp1,sizeof(rsp1));
		ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
		if((p1=strstr((char *)rsp1,"Q24"))!=NULL) 
		{
			 if(strstr((char*)rsp1,"PL002"))
			 {
				 strcpy(ModuleVer2,"Q24C ");
			 }
			 else if(strstr((char*)rsp1,"PL001"))
			 {
				 strcpy(ModuleVer2,"Q24E ");
			 }
			 if((p1-9)!=NULL)
			 {
				 memcpy(&ModuleVer2[5],p1-9,4);  //657G00gg.Q24PL002
			 }
			 ModuleVer2[9]=0;
		}  
		
		if(!memcmp(ModuleVer1, ModuleVer2, 9)) //比较前两次读取的版本信息
		{
			memcpy(ModuleVer, ModuleVer2, 9);
		}
		else
		{ 
			//第3次读版本
			WlSendCmd((unsigned char*)"at+GMR\r", rsp_wnet, 100, 30000, 1000); 
			strcpy((char *)rsp2,(char *)rsp_wnet);
			Wl_handle_ver(rsp2,sizeof(rsp2)); 
			ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
			if((p2=strstr((char *)rsp2,"Q24"))!=NULL) 
			{
				if(strstr((char*)rsp2,"PL002"))
				{
					strcpy(ModuleVer,"Q24C ");
				}
				else if(strstr((char*)rsp2,"PL001"))
				{
					strcpy(ModuleVer,"Q24E ");
				}
				if((p2-9)!=NULL)
				{
					memcpy(&ModuleVer[5],p2-9,4);  //657G00gg.Q24PL002
				}
				ModuleVer[9]=0;
			}  
		}

		WlSendCmd((unsigned char*)"AT+CGMM\r",rsp,sizeof(rsp),30000, 1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp);
		if(strstr((char *)rsp,"850"))
		{
			strcat(ModuleVer," 850");
		}
		else if(strstr((char *)rsp,"900E"))
		{
			strcat(ModuleVer," 900E");
		}
		else if(strstr((char *)rsp,"1800"))
		{
			strcat(ModuleVer," 1800");
		}
		else if(strstr((char *)rsp,"1900"))
		{
			strcat(ModuleVer," 1900");
		}
        
		memset(rsp_wnet, 0x00, sizeof(rsp_wnet));//get imei number
		WlSendCmd((unsigned char*)"AT+WIMEI?\r", rsp_wnet, 100, 30000, 1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
		if (strstr((char *)rsp_wnet, "+WIMEI"))
		{
			memcpy(gImei,rsp_wnet+10,15);
			gImei[15]='\0';
		}
	}

	else if(strstr((char *)rsp,"Q26"))     //Q26模块
	{
		if(strstr((char*)rsp,"EL001"))
		{
			strcpy(ModuleVer,"Q26ELP5C ");
		}
		else if(strstr((char*)rsp,"87RD"))
		{
			strcpy(ModuleVer,"Q26");
			strncat(ModuleVer, rsp, 5);
		}

		memset(rsp, 0, sizeof(rsp));
		WlSendCmd((unsigned char*)"AT+CGMM\r",rsp,sizeof(rsp),30000, 1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
		if(strstr((char *)rsp,"850"))
		{
			strcat(ModuleVer,"850");
		}
		else if(strstr((char *)rsp,"900E"))
		{
			strcat(ModuleVer,"900E");
		}
		else if(strstr((char *)rsp,"1800"))
		{
			strcat(ModuleVer,"1800");
		}
		else if(strstr((char *)rsp,"1900"))
		{
			strcat(ModuleVer,"1900");
		}

		memset(rsp_wnet, 0x00, sizeof(rsp_wnet));//Imei number
		if(TermInfo[0]==S90 && VerInfo[5]==0x25)
		{
			WlSendCmd((unsigned char*)"AT+CGSN\r", rsp_wnet, 100, 30000, 1000);
			ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
			if((p=strstr((char *)rsp_wnet, "+CGSN:"))!=NULL)
			{
				memcpy(gImei,p+7,14);
				gImei[14]='\0';
			}
		}
		else
		{
			WlSendCmd((unsigned char*)"AT+WIMEI?\r", rsp_wnet, 100, 30000, 1000);
			ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
			if (strstr((char *)rsp_wnet, "+WIMEI"))
			{
				memcpy(gImei,rsp_wnet+10,15);
				gImei[15]='\0';
			}
		}
	}

	
	else if(strstr((char *)rsp,"SW REVISION: "))    //Huawei
	{
		char *p;
		p=strstr((char *)rsp,"SW REVISION: ");
		WlSendCmd("AT+GMM\r",rsp_wnet, 100, 30000, 1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
		Wl_handle_ver(rsp_wnet,sizeof(rsp_wnet));
		memset(ModuleVer,0,sizeof(ModuleVer));
		strcpy(ModuleVer,rsp_wnet);
		verlen=strlen((char *)ModuleVer);
		memcpy(ModuleVer+verlen,p+13,6);
	}
	else if(strstr((char *)rsp,"S/W VER: MG815"))   //中兴CDMA
	{
		memcpy(ModuleVer,rsp+9,13);
		ModuleVer[13]=0;
		memset(rsp_wnet,0x00,sizeof(rsp_wnet)); ////get imei number
		WlSendCmd((unsigned char*)"AT+CIMI\r",rsp_wnet,100,30000,1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
		for(i=0;i<strlen(rsp_wnet);i++)
		{
			if(rsp_wnet[i]==':')  
			{ 
				memcpy(gImei,rsp_wnet+i+2,15);
				gImei[15]='\0';
			}
		}
	}
	else if(strstr((char *)rsp,"S/W VER: QSC6020-APP-3350"))   //EM200模块  2009-11-26  
	{
		uchar *p;
		WlSendCmd("AT+CVER\r",rsp,sizeof(rsp),30000,1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp);
		strcpy(ModuleVer,"EM200 ");
		p=strstr((char *)rsp,"+CVER: WL32SMPU, 502.11.00.");
		if(p)
		{
			memcpy(ModuleVer+6,p+27,6);
			ModuleVer[6+6]=0;
		}
		else
		{ 
			memcpy(ModuleVer+6,"???",4);
		}       
		memset(rsp_wnet,0x00,sizeof(rsp_wnet));
		WlSendCmd((unsigned char*)"AT+CIMI\r",rsp_wnet,100,30000,1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
		for(i=0;i<strlen(rsp_wnet);i++)
		{
			if(rsp_wnet[i]==':')   
			{	memcpy(gImei,rsp_wnet+i+2,15);
				gImei[15]='\0';
			}
		}
	}
	else if(strstr((char *)rsp,"S/W v1.1.4"))   //ENFORA1.1.4 2011-04-22
	{
		memcpy(ModuleVer,"ENFORA1.1.4",sizeof("ENFORA1.1.4"));
		memset(rsp_wnet, 0x00, sizeof(rsp_wnet));//Get Imei number
		WlSendCmd((unsigned char*)"AT+GSN\r", rsp_wnet, 100, 30000, 1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
		strcpy((char *)rsp,(char *)rsp_wnet);
		Wl_handle_ver(rsp,sizeof(rsp));
		memcpy(gImei,rsp,15);
		gImei[15]='\0';
	}


	else if(strstr((char *)rsp,"G610"))//G610
	{
		
		if(strstr((char*)rsp, "G610_V0C"))
		{
			strcpy(WireVer, "G610_VOC");
		}
		else if(strstr((char*)rsp, "G610_V0D"))
		{
			strcpy(WireVer, "G610_VOD");
		}
		else 
		{
			strcpy(WireVer, "G610");
		}
		
		p = strstr((char *)rsp,"G610");		
		strcpy((char *)ModuleVer,p);
		ModuleVer[20] = 0;
		
		memset(rsp_wnet, 0x00, sizeof(rsp_wnet));//iMEI Number
		WlSendCmd((unsigned char*)"AT+GSN\r", rsp_wnet, 100, 30000, 1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
		strcpy((char *)rsp,(char *)rsp_wnet);
		Wl_handle_ver(rsp,sizeof(rsp));
		memcpy(gImei,rsp+1,15);//IMEI号去掉引号 由rsp改为rsp+1
		gImei[15]='\0';

	}
	else if(strstr((char *)rsp,"G620"))//G620
	{
		memset(tmpVer, 0, sizeof(tmpVer));
		strncpy((char *)tmpVer,rsp,sizeof(tmpVer));
		strncpy((char *)ModuleVer, tmpVer, sizeof(tmpVer));
		strcpy(WireVer, "G620");

		memset(rsp_wnet, 0x00, sizeof(rsp_wnet));//get imei number
		WlSendCmd((unsigned char*)"AT+GSN\r", rsp_wnet, 100, 30000, 1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
		strcpy((char *)rsp,(char *)rsp_wnet);
		Wl_handle_ver(rsp,sizeof(rsp));
		memcpy(gImei,rsp+1,15);//IMEI号更改  由rsp改为rsp+1
		gImei[15]='\0';
	}	
	else if(strstr((char *)rsp,"H330"))//H330
	{
		memset(tmpVer, 0, sizeof(tmpVer));
		strncpy((char *)tmpVer,rsp,sizeof(tmpVer));
		strncpy((char *)ModuleVer, tmpVer, sizeof(tmpVer));
		strcpy(WireVer, "H330");

		memset(rsp_wnet, 0x00, sizeof(rsp_wnet));//get imei number
		WlSendCmd((unsigned char*)"AT+GSN\r", rsp_wnet, 100, 30000, 1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
		strcpy((char *)rsp,(char *)rsp_wnet);
		Wl_handle_ver(rsp,sizeof(rsp));
		memcpy(gImei,rsp+1,15);//IMEI号更改  由rsp改为rsp+1
		gImei[15]='\0';
	}
	else
	{
		memset(tmpVer, 0, sizeof(tmpVer));
		strncpy((char *)tmpVer,rsp,sizeof(tmpVer));
		memset(rsp_wnet, 0x00, sizeof(rsp_wnet));//get module type
		WlSendCmd((unsigned char*)"AT+GMM\r", rsp_wnet, 100, 30000, 1000);
		ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
		strcpy((char *)rsp,(char *)rsp_wnet);
		Wl_handle_ver(rsp,sizeof(rsp));
		ComSends("%s\n",rsp_wnet);
		ComSends("%s\n",rsp);
		
		if(strstr((char *)rsp, "EM701"))
		{//EM701模块
			sprintf(ModuleVer, "EM701 %s", tmpVer);
			if(ModuleVer[strlen(tmpVer)+5] == 32) //如果最后一个字节是space
				ModuleVer[strlen(tmpVer)+5] = 0;

			memset(rsp_wnet, 0x00, sizeof(rsp_wnet));//iMEI Number
			WlSendCmd((unsigned char*)"AT+GSN\r", rsp_wnet, 100, 30000, 1000);
			ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
			strcpy((char *)rsp,(char *)rsp_wnet);
			Wl_handle_ver(rsp,sizeof(rsp));
			memcpy(gImei,rsp,15);
			gImei[15]='\0';
		}
		else if(strstr((char *)rsp, "MC323"))
		{//CDMA机器无IMEI号
			strcpy((char *)ModuleVer, "MC323 " );
			strcpy((char *)WireVer, "MC323" );
			strncat( ModuleVer, &tmpVer[3], 13);//MC323的版本信息13个字节，防止多接收无用数据
		}
		else if(strstr((char *)rsp, "MG323"))
		{
			if(strstr((char *)rsp, "MG323-B"))
			{
				strcpy((char *)ModuleVer, "MG323-B" );
				strcpy((char *)WireVer, "MG323-B" );	
			}
			else
			{
				strcpy((char *)ModuleVer, "MG323");
				strcpy((char *)WireVer, "MG323");
			}
			strncat( ModuleVer, &tmpVer[3], strlen(tmpVer)-3);

			memset(rsp_wnet, 0x00, sizeof(rsp_wnet));//iMEI Number
			WlSendCmd((unsigned char*)"AT+GSN\r", rsp_wnet, 100, 30000, 1000);
			ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
			strcpy((char *)rsp,(char *)rsp_wnet);
			Wl_handle_ver(rsp,sizeof(rsp));
			memcpy(gImei,rsp,15);
			gImei[15]='\0';
		}
		else if(strstr((char *)rsp, "MU509"))
		{
			if(VerInfo[1]==3)
			{
				strcpy((char *)WireVer, "MU509 ");
				strcpy((char *)ModuleVer, "MU509 ");
			}			
			else if(TermInfo[0]==S90 && VerInfo[5]==0x33)
			{
				strcpy((char *)WireVer, "MU509B");
				strcpy((char *)ModuleVer, "MU509B");
			}
			else if(TermInfo[0]==S90 && VerInfo[5]==0x34)
			{
				strcpy((char *)WireVer, "MU509G");
				strcpy((char *)ModuleVer, "MU509G");
			}
			else if(TermInfo[0]==S90 && VerInfo[5]==0x35)
			{
				strcpy((char *)WireVer, "MU509C");
				strcpy((char *)ModuleVer, "MU509C");
			}
			else
			{
				strcpy((char *)WireVer, "MU509 ");
				strcpy((char *)ModuleVer, "MU509 ");
			}
			strncat(ModuleVer, tmpVer, sizeof(tmpVer));
			memset(rsp_wnet, 0x00, sizeof(rsp_wnet));//iMEI Number
			WlSendCmd((unsigned char*)"AT+GSN\r", rsp_wnet, 100, 30000, 1000);
			ComSends("Line=%d,%s",__LINE__ ,rsp_wnet);
			strcpy((char *)rsp,(char *)rsp_wnet);
			Wl_handle_ver(rsp,sizeof(rsp));
			memcpy(gImei,rsp,15);
			gImei[15]='\0';

		}
		else if(strstr((char *)rsp, "MC509"))
		{

			strcpy((char *)WireVer, "MC509 ");
			strcpy((char *)ModuleVer, "MC509 ");
			strncat(ModuleVer, tmpVer, 13);
		}
		else
		{
			strncpy((char *)ModuleVer, tmpVer, sizeof(tmpVer));
			strcpy(WireVer, ModuleVer);
		}               
	}

	strcpy(gWnetVer, ModuleVer);
	strcpy(gWireVer, WireVer);
	return 0;

}

//处理一下版本信息
void Wl_handle_ver(uchar *s,int len)
{
     int ii,jj;
     unsigned char *p;
	 //把OK去掉
	 if(s==NULL) return;
     while(1)
     {
          p=strstr(s,"OK");
          if(p==NULL) break;
          memset(p,' ',2);
     }


     // \r\n换成空格
     for(ii=0;ii<len;ii++) 
     {
          if(!s[ii]) break;
          if(('\r'==s[ii])||('\n'==s[ii])||(34==s[ii]))//34表示"
          {
                s[ii]=' ';
          }
     }


     //缩进+号
     for(ii=0;ii<len;ii++) 
     {
          if('+'==s[ii])
          {
              for(jj=ii+1;jj<len;jj++)
              {
                   if((s[jj]==' '))
                   {
						break;
                   }

              }
              memmove(s+ii,s+jj,len-jj);
              len=len-jj+ii;

          }
          else if(!s[ii])
          {
               break;

          }
     }

     //连在一起的空格合并成一个
     for(ii=0;ii<len;ii++) 
     {
          if(!s[ii]) break;
          if(' '==s[ii])
          {
              ii++;
              for(jj=ii;jj<len;jj++)
              {
					if(' '!=s[jj]) break;
              }
              if(ii!=jj)
              {
                    memmove(s+ii,s+jj,len-jj);
                    len=len-jj+ii;
              }
          }

     }

     //打头的空格去掉
	 s[len] = 0;
     if(' '==s[0])
     {
          if(' '==s[1])
          {
                  len-=2;
                  memmove(s,s+2,len);
          }
          else
          {
                  len--;
                  memmove(s,s+1,len);
          }

     }

	 //将AT去掉
    for(ii=0; ii<len-2; ii++)
    {
    	if(s[ii]=='A' && s[ii+1]=='T' && s[ii+2]==' ')
    	{
    		
			memmove(s, s+3+ii, len-3-ii);
			len -= 3;
    		break;
    	}
    }

    if(len>1 && s[len-1]==' ') 
	{
		 s[len-1]=0;
		 len--;
	}
    else s[len]=0;

}

/************************************************************************/
/* 取机器硬件与软件配置版本信息  */
/************************************************************************/
void ShowVer(void)
{
	unsigned char i;
	PICC_PARA tpicc_para;
	ST_FONT stfontlist[50];
	int FontNums=0;
	uchar fontStr[20],fontInfo[50];
	memset(&tpicc_para, 0x00, sizeof(tpicc_para));

	//ScrClrLine(2,7);
	ScrCls();
	ScrDispStr(0,4,1, "获取终端信息...", "GetTermInfo...");
	
	GetInfoExt();
	GetWirelessVer();
	GetSoVer();

	for(i=0;i<gKeyWordNum;i++)
	{
		if(InfoOut[i][0]!=0)
		{ 
			RECORD(0,".F_%s=%s",KeyWord[i],InfoOut[i]);
		}
	}
	
	
	
	switch(TermInfo[0])
	{
		case T610: RECORD(0,".MODULE=T610"); break;
		case T620: RECORD(0,".MODULE=T620"); break;
		case D210: RECORD(0,".MODULE=D210"); break;
		case D200: RECORD(0,".MODULE=D200"); break;
		case MT30: RECORD(0,".MODULE=MT30"); break;
		case S300: RECORD(0,".MODULE=S300"); break;
		case S800: RECORD(0,".MODULE=S800"); break;
		case S900: RECORD(0,".MODULE=S900"); break;
		case S80:  RECORD(0,".MODULE=S80");  break;
		case S90:  RECORD(0,".MODULE=S90");  break;
		case S58:  RECORD(0,".MODULE=S58");  break;
		case S60:  RECORD(0,".MODULE=S60");  break;
		case SP30: RECORD(0,".MODULE=SP30"); break;
		case S78:  RECORD(0,".MODULE=S78");  break;
		case T60:  RECORD(0,".MODULE=T60");  break;
		case T52:  RECORD(0,".MODULE=T52");  break;
		case S500: RECORD(0,".MODULE=S500"); break;
		case S200: RECORD(0,".MODULE=S200"); break;
		default:   RECORD(0,".MODULE=");     break;	
	}
	
	RECORD(0,"..SN=%s",gSn);
	RECORD(0,"..EXSN=%s",gExSn);
	RECORD(0,".BIOS=%02u",VerInfo[0]);
	RECORD(0,".MON=%u.%02u",VerInfo[1],VerInfo[2]);
    RECORD(0,".MAIN=%02x",VerInfo[3]);
    RECORD(0,".PORT=%02x",VerInfo[4]);
	RECORD(0,".MAGB=%02x",VerInfo[6]);
	if(TermInfo[0]==S200)RECORD(0,".CPU=ARM CM3");
	else RECORD(0,".CPU=ARM11 32BIT\n");
	RECORD(0,".SDRAM=%dMB", gSdramSize);
	RECORD(0,".DDR=%dMB", gDdrSize);
	RECORD(0,".FLASH=%dMB", gFlashSize);
	RECORD(0,".NANDFLASH=%dMB",gNandFlashSize);
	RECORD(0,".GPRS=[%c]",TestModule[T_GPRS]);
	RECORD(0,".CDMA=[%c]",TestModule[T_CDMA]);
	RECORD(0,".WCDMA=[%c]",TestModule[T_WCDMA]);
	RECORD(0,".WIFI=[%c]",TestModule[T_WIFI]);
	RECORD(0,".BT=[%c]",TestModule[T_BT]);
	RECORD(0,".RF=[%c]",TestModule[T_RF]);
	RECORD(0,".IP=[%c]",TestModule[T_LAN]);
	if(TestModule[T_MODEM]=='Y')
	{
		s_ModemInfo(gModemSVer,gModemHVer,gLMDate,gOtVer);//ModemVer 这个必须modem打开后才能够正常返回
		RECORD(0,".MODEM=[%c] %s,%s",TestModule[T_MODEM],gModemSVer,gModemHVer);
	}
	else
	{
		RECORD(0,".MODEM=[%c]",TestModule[T_MODEM]);
	}
	if(TestModule[T_LAN]=='Y')
		RECORD(0,"..LAN.mac=%02x%02x%02x%02x%02x%02x",gMac[0],gMac[1],gMac[2],gMac[3],gMac[4],gMac[5]);
	if(TestModule[T_BT]=='Y')
	{
		RECORD(0,"..BT.mac=%02x%02x%02x%02x%02x%02x",gBtMac[0],gBtMac[1],gBtMac[2],gBtMac[3],gBtMac[4],gBtMac[5]);
		if(gBtAppleFlag==1)RECORD(0,".BT_IOS=[Y]");
	}

	if(TestModule[T_WIFI]=='Y')
	{
		RECORD(0,"..WIFI.mac=%02x%02x%02x%02x%02x%02x",gWifiMac[0],gWifiMac[1],gWifiMac[2],gWifiMac[3],gWifiMac[4],gWifiMac[5]);
	}
	
	if(fexist(F_TSCB)<0)
		RECORD(0,".F_TSCB=0");
	else
		RECORD(0,".F_TSCB=1");
	
	if(TestModule[T_RF]=='Y')
	{	
		if(!PiccOpen())
		{
			PiccSetup('r', &tpicc_para);
			RECORD(0,".RFpara=[%02x] %02X%02X%02X%02X%02X", TermInfo[12], tpicc_para.a_conduct_val,\
			tpicc_para.m_conduct_val, tpicc_para.b_modulate_val, tpicc_para.card_RxThreshold_val, tpicc_para.f_modulate_val);
			PiccClose();
		}
	}

	RECORD(0,".USBHOST=[%c]",TestModule[T_USBHOST]);
	RECORD(0,".USBDEV =[%c]",TestModule[T_USBDEV] );
  
    if(TestModule[T_WNET]=='Y') 
    {
        RECORD(0,".WNET=%s", gWireVer);

#ifdef ALL_WNET_VER_MATCH
		RECORD(0,".WNET.ver=%s", gWnetVer);			
#else
		RECORD(0,"..WNET.ver=%s", gWnetVer);			
#endif

        RECORD(0,"..WNET.imei=%s", gImei); 
    }
	else
	{
		RECORD(0,"..WNET.imei=000000000000000");
	}
    
    if(TermInfo[1]!=0)RECORD(0, ".PRN.type=%c", TermInfo[1]);
    else  RECORD(0, ".PRN=[N]");


	//ConfigFileName
	//RECORD(0,".CFGFile=%s",gConfigFileName);
	
	//Handset font information
	FontNums=EnumFont(stfontlist, 50);  
	RECORD(0,".FONT.name=%s",gFontFileName);
	RECORD(0,".FONT.size=%dB",gFontLength);
	RECORD(0,".FONT.cnt=%d",FontNums);  //字体数
	for(i=0; i<FontNums; i++)
	{
		switch(stfontlist[i].CharSet)  //字符集
		{
			case 0x01: strcpy(fontStr, "WEST"); break;
			case 0x02: strcpy(fontStr, "TAI"); break;
			case 0x03: strcpy(fontStr, "MID_EUROPE"); break;
			case 0x04: strcpy(fontStr, "VIETNAM"); break;
			case 0x05: strcpy(fontStr, "GREEK"); break;
			case 0x06: strcpy(fontStr, "BALTIC"); break;
			case 0x07: strcpy(fontStr, "TURKEY"); break;
			case 0x08: strcpy(fontStr, "HEBREW"); break;
			case 0x09: strcpy(fontStr, "RUSSIAN"); break;
			case 0x0A: strcpy(fontStr, "GB2312"); break;
			case 0x0B: strcpy(fontStr, "GBK"); break;
			case 0x0C: strcpy(fontStr, "GB18030"); break;
			case 0x0D: strcpy(fontStr, "BIG5"); break;
			case 0x0E: strcpy(fontStr, "SHIFT_JIS"); break;	
			case 0x0F: strcpy(fontStr, "KOREAN"); break;	
			case 0x010: strcpy(fontStr, "ARABIA"); break;	
			default:   strcpy(fontStr, "DIY"); break;	
		}
		memset(fontInfo, 0x00, sizeof(fontInfo));
		sprintf(fontInfo,".FONT%d=%s %d*%d",i+1,fontStr,stfontlist[i].Width,stfontlist[i].Height);  //字体宽与高
		if((stfontlist[i].Bold)==1)   //字体粗细
		{
			strcat(fontInfo, "Bold"); 
		}
		RECORD(0,"%s",fontInfo);
	}
	RECORD(0, "\n");	
}

/*
检查版本信息和IMEI号的正确性
  */
int IsStringOk(char *str,int flag)
{
	int i;
	if(flag==0) //flag==0 判断IMEI号
	{
		if(strlen(str)!=15) return 1;//判断长度
		for(i=0;i<15;i++)
		{
			if( (str[i]>='0' && str[i]<='9') || (str[i]>='a' && str[i]<='z') || (str[i]>='A' && str[i]<='Z') )
			{
			
			}
			else
			{
				return 2;
			}
		}
		return 0;
	}
	else if(flag==1) //判断版本信息
	{
		if(strlen(str)==0)return 3;
		for(i=0;i<strlen(str);i++)
		{
			if( (str[i]>='0' && str[i]<='9') || (str[i]>='a' && str[i]<='z') || (str[i]>='A' && str[i]<='Z')
				|| str[i]=='-' || str[i]=='_' || str[i]=='.')
			{
				
			}
			else
			{
				return 4;
			}
		
		}
		return 0;	
	}

	return 5;

}




int WlSendCmdEx(char *Cmd,char *rsp,int len)
{
	
	int iRet,i=0;
	char rsp_wnet[100];

	ComSends("&&&&&&&&&&&&&&&BEGIN   %s",Cmd);
	while((i++)<3)//3次机会
	{
		memset(rsp_wnet, 0, sizeof(rsp_wnet));
		iRet = WlSendCmd(Cmd,rsp_wnet,100,3000,500);//超时3s
		ComSends("%d:%d:%s\n",i,iRet,rsp_wnet);
		if( iRet || strstr((char *)rsp_wnet, "ERROR") )continue;		
		Wl_handle_ver(rsp_wnet, strlen(rsp_wnet));
		memset(rsp,0,len);
		memcpy(rsp,rsp_wnet,len-1);
		ComSends("%d:%s\n",i,rsp);
		return 0;
	}
	ComSends("&&&&&&&&&&&&&&& END \n\n");
	return 1;

}




int GetWirelessVer()
{
	int iRet,i;

	if(TestModule[T_WNET]=='N')return 0xff;
	if(GetWnetVerFlag==1) return 0;
	else GetWnetVerFlag=1;

	ScrClrLine(2, 7);
	ScrDispStr(0,2,0x01,"读取无线版本....", "Get Wireless VerInfo...");

	iRet = WlInit(NULL);
	if(iRet!=RET_OK && iRet!=WL_RET_ERR_NOSIM && iRet!=WL_RET_ERR_INIT_ONCE && iRet!=WL_RET_ERR_RSPERR)
	{
		ScrClrLine(4,7);
		ScrDispStr(0, 4, 0x01, "模块初始化失败\n %d ", "WNet Init Fail\n %d ",iRet);
		BeepFail();
		getkey();
		return iRet;
	}
  
    iRet=WlOpenPort();   //打开无线通讯串口
    if(iRet)
    {
        ScrClrLine(4,7);
        ScrDispStr(0,4,0x01,"打开通信串口失败:%d","Open Port Err:  %d",iRet);
		BeepFail();
        getkey();
		return iRet;
    }

	/*
	WlSendCmdEx("AT+GMR\r",rsp,sizeof(rsp));
	WlSendCmdEx("AT+CGMM\r",rsp,sizeof(rsp));
	WlSendCmdEx("AT+CGSN\r",rsp,sizeof(rsp));
	WlSendCmdEx("AT+GMM\r",rsp,sizeof(rsp));
	WlSendCmdEx("AT+CIMI\r",rsp,sizeof(rsp));
	WlSendCmdEx("AT+CVER\r",rsp,sizeof(rsp));
	WlSendCmdEx("AT+WIMEI?\r",rsp,sizeof(rsp));
	WlSendCmdEx("AT+MEID?\r",rsp,sizeof(rsp));
	*/
	
	memset(gWnetVer, 0, sizeof(gWnetVer));
	memset(gWireVer, 0, sizeof(gWireVer));
	memset(gImei, 0, sizeof(gImei));
	
	//AT+GMR\r  获取小的版本号,如 G610_V0D.00.3B 和 11.813.01.05.00等
	//AT+CGMM\r 获取大的版本号,如 MG323-B MU509 等 G610/G620/G510/H330数据特殊,需要处理		
	

	i = 0;
	while((i++)<3)
	{		
		WlSendCmdEx("AT+GMR\r",gWnetVer,sizeof(gWnetVer)); //获取小版本号  不用做标配   原来是使用这个判断
		WlSendCmdEx("AT+CGMM\r",gWireVer,sizeof(gWireVer));//获取大版本号  需要做标配

		if( strstr(gWireVer,"G610") || strstr(gWireVer,"G620") || strstr(gWireVer,"G510") || strstr(gWireVer,"H330") )
		{	
			memcpy(gWireVer,gWnetVer,8); //取前八位为标配 G610_V0C
			memset(gWireVer+8,0,sizeof(gWireVer)-8);
		}
		else if( strstr(gWnetVer,"Q24") || strstr(gWnetVer,"Q26") || strstr(gWnetVer,"MG815") || strstr(gWnetVer,"QSC6020-APP-3350") )
		{  //这些老模块,暂时如此处理
			if( strstr(gWnetVer,"Q24") ) memcpy(gWireVer,"Q24\x00\x00",5);
			else if( strstr(gWnetVer,"Q26") ) memcpy(gWireVer,"Q26\x00\x00",5);
			else if( strstr(gWnetVer,"MG815") ) memcpy(gWireVer,"MG815\x00\x00",7);
			else if( strstr(gWnetVer,"QSC6020-APP-3350") ) memcpy(gWireVer,"QSC6020-APP-3350\x00\x00",18);
		}
		else
		{
			//其他的MG323-B/MU509/MC323/MC509/EM701 等都是直接那使用 AT+GMR 和 AT+CGM处理 不需要转换
		}

		if(IsStringOk(gWireVer,1)) continue;
		break;
	}

	//IMEI号
	i = 0;
	while((i++)<3)
	{
		if(TestModule[T_CDMA]=='Y')
		{
			uchar imei[30];
			WlSendCmdEx("AT^DSN\r",imei,sizeof(imei));//AT^DSN //AT+CIMI\r
			memcpy(gImei,imei+5,15);
			gImei[14] = 0;
			if(strstr(imei,"DSN")!=NULL)return 0;
			else continue;
		}
		
		WlSendCmdEx("AT+CGSN\r",gImei,sizeof(gImei));
		if(gImei[0]==0) //如没有读到IMEI号,则使用下面的指令读取,可能有用
			WlSendCmdEx("AT+WIMEI?\r",gImei,sizeof(gImei)); //比如Q24/Q26 模块可能需要用到
		if(IsStringOk(gImei,0))continue;
		break;
	}
		
	gImei[15] = 0;
	if(IsStringOk(gImei,0))
	{
		ScrPrint(0,3,0,"IMEI=%s",gImei);
		while(1)
		{
			Beep();
			DelayMs(1000);
		}
	}
	return 0;	

}



