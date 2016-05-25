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
        fileUseSize += FileInfo[i].length;  //ͳ�������ļ��ռ�
        if((FileInfo[i].attr == 0xff) && (FileInfo[i].type == 0x02))   //�ж����ֿ��ļ�
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
	    case 492543:    strcpy(gFontFileName,"pax-font-a(v1.0)_c"); break;//ѹ���ֿ�
	    case 404393:    strcpy(gFontFileName,"pax-font-e(v1.0)_c"); break;//ѹ���ֿ�
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

	
	ReadVerInfo(VerInfo); //boot��monitor�����塢�ӿڰ����Ϣ
	if(TermInfo[0]==S60)
		ScrSetEcho(1);   //����ʾ���ԣ���S60�ֻ��뿪����ʱ��ʾ��ʾ��Ϣ	
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
	for(i=0;i<21;i++)//gTermInfo Ŀǰ��0~20��Ч
	{
		if(TermInfo[i]==0)
			TestModule[i]='N';
		else
			TestModule[i]='Y';
		ComSends("%d-%c-%d",i,TestModule[i],TermInfo[i]);
	}


	
	//�����ʹ������ж�
	if( (TermInfo[19]&0x01)==0 )TestModule[T_TSCREEN]='N';
	else TestModule[T_TSCREEN]='Y';

	if( (TermInfo[19]&0x02)==0 )gIsColorLcd = 0;
	else gIsColorLcd = 1;
	
	//��������
	if( (TermInfo[19]&0x08)==0 )TestModule[T_BT]='N';
	else TestModule[T_BT]='Y';
	//������һЩ��Ϣ
	if(gIsColorLcd==1 || TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900 || InfoOut[C_LCD][0]!=0)
		CLcdGetInfo(&stLcdInfo);
	//ScrPrint(0,4,1,"H%d,W%d,%d,%d",stLcdInfo.height,stLcdInfo.width,stLcdInfo.cpp,stLcdInfo.lpp);
	//getkey();

	TestModule[T_BEEP]='Y';
	TestModule[T_KB]='Y';
	TestModule[T_LCD]='Y';
	TestModule[T_CLK]='Y';

	
	//��RS232B��
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
		|| strstr(InfoOut[C_PN],"S58-M0L-263-0HCC")!=NULL )//S58�в���Ԥ��˫����
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

	//ͨ��PN���ж�SP30�ĸ������
	//SP30-00L-XXX-XXXX  ��̫�� + ˫���ڣ����USB ΪOTG���ܣ�
    //SP30-0UL-XXX-XXXX ��̫�� + USB Device + �����ڣ�SP30���û��USB�ڣ�����Ҫ����U�̣�
    //SP30-000-XXX-XXXX  ���������� �����USBΪOTG���ܣ�
    //SP30-0U0-XXX-XXXX ��USB���ã�USBֻ��Device���ܣ�����Ҫ����U�̣�
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
	else if( strstr(InfoOut[C_PN],"SP30-0UL")!=NULL) //�ĳ�USB�ϴ�PPQ,����RF��USB�Ƿ�OK  //16-03-16 ���Ļش����ϴ�,USB������
	{												 //��Ϊ����USBD,����USBD��ʱ���RF(2016-05-06)
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

	/* S300-000-363-XXXX ��������S300
	  Ĭ�϶��Ǹ�02NA��������:����+LAN+USB,USB������A���ϴ�
	  04NA,01NA:˫����+LAN, A���ϴ�
	  03NA,����+˫LAN+USB,USB������A���ϴ�
	*/

	if(TermInfo[0]==S300)TestModule[T_COMB_TEST]='N';
	else TestModule[T_COMB_TEST]=TestModule[T_COMB]; 

	if( strstr(InfoOut[C_PN],"S300-000-363-04NA")!=NULL || strstr(InfoOut[C_PN],"S300-000-363-01NA")!=NULL )
	{
		TestModule[T_COMB_TEST]='Y';
	}

	if( strstr(InfoOut[C_PN],"S300-000-363-03NA")!=NULL) //S300����˫LAN����
	{
		TestModule[T_LAN2]='Y';
	}



	// ���õ绰
	if(TermInfo[0]==S810 || TermInfo[0]==S78 || TermInfo[0]==S500 || (TermInfo[0]==S800 && InfoOut[C_MAIN_BOARD][1]=='2') )
	{
		if(TestModule[T_MODEM]=='Y')
			TestModule[T_BYPHONE]='Y';
		else
			TestModule[T_BYPHONE]='N';
	}
	else
		TestModule[T_BYPHONE]='N';


	//S58����
	if(TermInfo[0]==S58 && TermInfo[8]==0 && TermInfo[11]==0)
	{
		if(InfoOut[C_PORT_BOARD][2]=='2')//PORT_BOARD="V32"
			TestModule[T_COMB]='Y';
		else if(InfoOut[C_PORT_BOARD][2]=='6')//PORT_BOARD="V36"
			TestModule[T_BYPHONE]='Y';
	}


	// ����
	if(TermInfo[9]!=0 || TermInfo[10]!=0 || TermInfo[18]!=0)
		TestModule[T_WNET]='Y';
	else
		TestModule[T_WNET]='N';

	// U��/USB HOST
	TestModule[T_UDISK] = TestModule[T_USBHOST];
	if( strstr(InfoOut[C_PN],"S300-000-363-04NA")!=NULL || strstr(InfoOut[C_PN],"S300-000-363-01NA")!=NULL )
	{
		TestModule[T_UDISK]='N';
		TestModule[T_USBHOST] = 'N';
		TestModule[T_USBDEV] = 'N';
	}

	
	// S800��������USB�ڵ�
	if(TermInfo[0]==S800) TestModule[T_UDISKA] = 'Y';
	else TestModule[T_UDISKA] = 'N';
	

	// ����
	if(TermInfo[0]==T610 || TermInfo[0]==T620 || TermInfo[0]==T60 || TermInfo[0]==T52
		|| TermInfo[0]==S300 || TermInfo[0]==S800 || TermInfo[0]==S900)//����
		TestModule[T_SPEECH]='Y';
	else
		TestModule[T_SPEECH]='N';

	// ���
	if(TermInfo[0]==S910 || TermInfo[0]==D200 || TermInfo[0]==D210 || TermInfo[0]==T60 || TermInfo[0]==S90 || TermInfo[0]==S900)//���
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
	
	if( strstr(InfoOut[C_PN],"S58-M0L-232-01EE")!=NULL ) //ֻ���������������Ҫ����FELICA
		TestModule[T_FELICA_ALL]='Y';
	else
		TestModule[T_FELICA_ALL]='N';

	
	// LED lights
	if( TermInfo[12]==0 || TermInfo[0]==S500 || TermInfo[0]==D200 || TermInfo[0]==D210 
		|| TermInfo[0]==S900 || TermInfo[0]==S800 || TermInfo[0]==S300 )
	{
		TestModule[T_LED]='N';
	}   //PCI3.X�Ĳ���LED��,�ϵ�Sû��LED�� S900-0G0-363-06CC/S90-0C0-063-01EE   
	else if( strstr(InfoOut[C_PN],"3-") || TermInfo[0]==SP30 || TermInfo[0]==S910 || TermInfo[0]==S200 || TermInfo[0]==T60  || VerInfo[1]==3 )
	{
		TestModule[T_LED]='Y';
	}
	else
	{
		TestModule[T_LED]='N';	
	}
	

	
	// ���
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


	if(InfoOut[C_SD_READER][0]!=0)//������ؼ��֣����������ļ�ȴû��д��ȥ��������Ҫ�����ж�
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

	if(TermInfo[0]==D200 && InfoOut[C_MAIN_BOARD][1]=='3') //D200Q V30~V39û��KEYLED
		TestModule[T_KEYLED]='N';
	


	if(TermInfo[0]==S78 && VerInfo[1]<3 && TestModule[T_USBDEV]=='Y')
	{
		TestModule[T_UDEV_TEST]='Y';//�ϵ�S78��Ҫ����USBDEV
	}
	else if( strstr(InfoOut[C_PN],"SP30-0UL")!=NULL)
	{
		TestModule[T_UDEV_TEST]='Y';//����Ҫ����USBD,���Ҳ��Եõ�ʱ����Ҫ��RFģ��
	}
	else 
	{
		TestModule[T_UDEV_TEST]='N';
	}

	FontType = TermInfo[13];    //�ֿ�����

	//SIM������
	//û�������ļ����������ļ�ͬʱ�ж�sim�����������
	if(TermInfo[9]==0 && TermInfo[10]==0 && TermInfo[18]==0)
		gSimNum=0;
	else if((TermInfo[9]==0x12 && TermInfo[0]==S90) || (TermInfo[9]==0x16 && TermInfo[0]==S90))//S90 wavecom ˫SIM����0x16��G610˫SIM����
	{
		gSimNum=2;
	}
		
	else if(((TermInfo[9] == 0x14 || TermInfo[9] == 0x16)) && (TermInfo[0] == S80) )   //S80 wavecom ˫SIM��
	{
		gSimNum=2;
	}
	else if(InfoOut[C_DUAL_SIM][0]!=0) gSimNum=2;
	else gSimNum=1;
	
	
	if(gSimNum==2)
		TestModule[T_SIM1]='Y';
	else
		TestModule[T_SIM1]='N';

	//����SM2
	if(InfoOut[C_SM_CHIP][0]!=0 || InfoOut[C_CIPHER_CHIP][0]!=0) TestModule[T_SM2]='Y';
	else TestModule[T_SM2]='N';


	if(TermInfo[0]==D210 || TermInfo[0]==D200)
		TestModule[T_BASE] = 'Y';
	else
		TestModule[T_BASE] = 'N';
	

	//TestModelue0--41�����븳ֵ ȫ��������Ҫ���Ե���Ŀ
	for(i=0;i<45;i++)
	{
		if(TestModule[i]==0)
		{
			ScrDispStr(0,6,0x01,"���ô���%d!!","Config Error%d!!",i);
			getkey();
		}
	}

	/*
	//�������ļ��ж����
	if(gTermInfo[9]==0 && gTermInfo[10]==0 && gTermInfo[18]==0)
		gSimNum=0;
	else if(InfoOut[C_DUAL_SIM][0]==0)
		gSimNum=1;
	else 
		gSimNum=2;
	*/
	
	//IC������ ����IC����SAM������
	//ֻ�ܸ��ݻ��ͺ����������жϣ�������ԵĻ����Ϲؼ���IC_NUM,SAM_NUM;

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
	
#ifdef SP30_POS //����СSP30
		case SP30:
			gIcNum=1;
			gSamNum=4;
			break;
#else          //СSP30
		case SP30:
			gIcNum=1;
			if(gSimNum==0)gSamNum=3;
			else if(gSimNum==1)gSamNum=2;
			break;
#endif
		case S78:
			gIcNum=1;
			if( gSimNum==1 && VerInfo[1]>=3 ) gSamNum = 3;//Сs����������4�����ۣ����Ƿ�Сs��������ߵ���5������	
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
			if(gSimNum==0) gSamNum=3;//��СSû�����ߣ�СS��һ�����߿����ǵ����õģ����û��������û���Ǹ�����
			if(gSimNum==1) gSamNum=3;
			if(gSimNum==2) gSamNum=2;//˫SIM��
			break;
			
		case S900:
		case T60:
		case S90:  //S90����������ֻ������sam����
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
		//case S900: //�����0��sim��2��sam,��S90��һ��
		case S300:
			gIcNum=1;
			if(gSimNum==0)gSamNum=3;
			else if(gSimNum==1)gSamNum=2;
			else if(gSimNum==2)gSamNum=1;
			break;
			
		default:
			break;
	}


	if(InfoOut[C_IC_CARD][1]=='0') //�����00���ʾû��IC��
		gIcNum = 0;


	//LAN��MAC��ַ
	if(TestModule[T_LAN]=='Y')
	{
		memset(gMac,0x00,sizeof(gMac));
		EthMacGet(gMac);
	}

	
	//NandFlash ��С ���趼��NandFlash��û�ж�����Ϊ0
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
	s_ModemInfo(gModemSVer,gModemHVer,gLMDate,gOtVer);//ModemVer �������modem�򿪺���ܹ���������

	//GetConfigFileName(gConfigFileName);//����(СS���û�������ļ��Ļ����ǻ����ϵͳ����ģ��������벻��Ӧ��)
	gFontLength=GetFontInfo(gFontFileName);//�ֿ�
	
	
	//RF���Ĳ��Ը߶�
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


/*GetInfoExt��GetWirelessVer����ִ��ʱ��Ƚϳ������Բ��ڿ���ǰ��ͻ�ȡ*/
/*����Ҫʹ�õ�ʱ��Ż�ȡ*/
int GetInfoExt(void)
{
	uchar Info[8*1024],OutInfo[30];
	int iInfoLen,FlagVLen,size,iRet;
	
	if(GetExtInfoFlag==1) return 0;
	else GetExtInfoFlag=1;
	
	ScrClrLine(2,7);
	ScrDispStr(0,2,1,"���ڻ�ȡ�汾��Ϣ...", "Get Version Message...");
	memset(BaseFontSize, 0, sizeof(BaseFontSize));
	memset(BaseMain, 0, sizeof(BaseMain));
	memset(BaseBios, 0, sizeof(BaseBios)); 
	memset(BaseDriver, 0, sizeof(BaseDriver));
	memset(BaseFlashSize, 0, sizeof(BaseFlashSize));
	memset(BaseSdramSize, 0, sizeof(BaseSdramSize));
    
	//��ȡflash��sdram�Ĵ�С
	memset(Info, 0, sizeof(Info));
	iInfoLen = GetTermInfoExt(Info, sizeof(Info));//////////////9s

	//��ȡFlash�Ĵ�С
	memset(OutInfo,0,sizeof(OutInfo));		
	iRet = XmlGetElement(Info, iInfoLen, "FlashSize", OutInfo, sizeof(OutInfo),&FlagVLen);
	size = atoi(OutInfo);
	gFlashSize = size/1024;

	//��ȡDDR��С
	memset(OutInfo,0,sizeof(OutInfo));
	iRet = XmlGetElement(Info, iInfoLen, "DdrSize", OutInfo, sizeof(OutInfo),&FlagVLen);  
	size = atoi(OutInfo);
	gDdrSize = size/1024;

	//��ȡSDRAM�Ĵ�С
	memset(OutInfo,0,sizeof(OutInfo));
	iRet = XmlGetElement(Info, iInfoLen, "SdramSize", OutInfo, sizeof(OutInfo),&FlagVLen);
	size = atoi(OutInfo);
	gSdramSize = size/1024;

	//��ӡ��Ϣ����GetHareWareConfig��ȡ
	if(TermInfo[0]==T52 || TermInfo[0]==S58 || (VerInfo[1]==3 && VerInfo[2]>=2) 
		|| TermInfo[0]==S800 || TermInfo[0]==S300 || TermInfo[0]==S900)
	{
		//�������ļ��Ĳ���Ҫ������ȡ��ӡ����Ϣ��ֱ�������ļ���ȡ
	}
	else if( TermInfo[1]!=0 )//û�������ļ�����Ҫ������ȡ��ӡ����Ϣ
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
			strcpy(gPrinterType,InfoOut[C_PRINTER]);  //��ʾû���趨��ӡ������    
		}
		else strcpy(gPrinterType,"***");
    }

    if(TermInfo[0] == S60 || (TermInfo[0] == SP30 && TermInfo[1]!=0)) //S60���д�ӡ����SP30�е���
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
	ScrDispStr(0,2,0x01,"��ȡ���߰汾��Ϣ", "Get Wireless VerInfo");
	
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
		ScrDispStr(0, 4, 0x01, "ģ���ʼ��ʧ��\n %d ", "WNet Init Fail\n %d ",iRet);
		getkey();
		return iRet;
	}
  
    iRet=WlOpenPort();   //������ͨѶ����
    if(iRet)
    {
        ScrClrLine(4,7);
        BeepFail();
        ScrDispStr(0,4,0x01,"��ͨ�Ŵ���ʧ��:%d","Open Port Err:  %d",iRet);
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
		ScrDispStr(0,4,0x01,"������ģ��汾ʧ��:%d","Get WNet VerInfo Err:%d",iRet); 
		getkey();
		return iRet;
	}

	strcpy((char *)rsp,(char *)rsp_wnet);
	Wl_handle_ver(rsp,sizeof(rsp)); 
	ComSends("Line=%d,%s",__LINE__ ,rsp);
  
	if((p=strstr((char *)rsp,"Q24"))!=NULL)     //Q24ģ��
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
		//��2�ζ��汾
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
		
		if(!memcmp(ModuleVer1, ModuleVer2, 9)) //�Ƚ�ǰ���ζ�ȡ�İ汾��Ϣ
		{
			memcpy(ModuleVer, ModuleVer2, 9);
		}
		else
		{ 
			//��3�ζ��汾
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

	else if(strstr((char *)rsp,"Q26"))     //Q26ģ��
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
	else if(strstr((char *)rsp,"S/W VER: MG815"))   //����CDMA
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
	else if(strstr((char *)rsp,"S/W VER: QSC6020-APP-3350"))   //EM200ģ��  2009-11-26  
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
		memcpy(gImei,rsp+1,15);//IMEI��ȥ������ ��rsp��Ϊrsp+1
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
		memcpy(gImei,rsp+1,15);//IMEI�Ÿ���  ��rsp��Ϊrsp+1
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
		memcpy(gImei,rsp+1,15);//IMEI�Ÿ���  ��rsp��Ϊrsp+1
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
		{//EM701ģ��
			sprintf(ModuleVer, "EM701 %s", tmpVer);
			if(ModuleVer[strlen(tmpVer)+5] == 32) //������һ���ֽ���space
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
		{//CDMA������IMEI��
			strcpy((char *)ModuleVer, "MC323 " );
			strcpy((char *)WireVer, "MC323" );
			strncat( ModuleVer, &tmpVer[3], 13);//MC323�İ汾��Ϣ13���ֽڣ���ֹ�������������
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

//����һ�°汾��Ϣ
void Wl_handle_ver(uchar *s,int len)
{
     int ii,jj;
     unsigned char *p;
	 //��OKȥ��
	 if(s==NULL) return;
     while(1)
     {
          p=strstr(s,"OK");
          if(p==NULL) break;
          memset(p,' ',2);
     }


     // \r\n���ɿո�
     for(ii=0;ii<len;ii++) 
     {
          if(!s[ii]) break;
          if(('\r'==s[ii])||('\n'==s[ii])||(34==s[ii]))//34��ʾ"
          {
                s[ii]=' ';
          }
     }


     //����+��
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

     //����һ��Ŀո�ϲ���һ��
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

     //��ͷ�Ŀո�ȥ��
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

	 //��ATȥ��
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
/* ȡ����Ӳ����������ð汾��Ϣ  */
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
	ScrDispStr(0,4,1, "��ȡ�ն���Ϣ...", "GetTermInfo...");
	
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
		s_ModemInfo(gModemSVer,gModemHVer,gLMDate,gOtVer);//ModemVer �������modem�򿪺���ܹ���������
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
	RECORD(0,".FONT.cnt=%d",FontNums);  //������
	for(i=0; i<FontNums; i++)
	{
		switch(stfontlist[i].CharSet)  //�ַ���
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
		sprintf(fontInfo,".FONT%d=%s %d*%d",i+1,fontStr,stfontlist[i].Width,stfontlist[i].Height);  //��������
		if((stfontlist[i].Bold)==1)   //�����ϸ
		{
			strcat(fontInfo, "Bold"); 
		}
		RECORD(0,"%s",fontInfo);
	}
	RECORD(0, "\n");	
}

/*
���汾��Ϣ��IMEI�ŵ���ȷ��
  */
int IsStringOk(char *str,int flag)
{
	int i;
	if(flag==0) //flag==0 �ж�IMEI��
	{
		if(strlen(str)!=15) return 1;//�жϳ���
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
	else if(flag==1) //�жϰ汾��Ϣ
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
	while((i++)<3)//3�λ���
	{
		memset(rsp_wnet, 0, sizeof(rsp_wnet));
		iRet = WlSendCmd(Cmd,rsp_wnet,100,3000,500);//��ʱ3s
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
	ScrDispStr(0,2,0x01,"��ȡ���߰汾....", "Get Wireless VerInfo...");

	iRet = WlInit(NULL);
	if(iRet!=RET_OK && iRet!=WL_RET_ERR_NOSIM && iRet!=WL_RET_ERR_INIT_ONCE && iRet!=WL_RET_ERR_RSPERR)
	{
		ScrClrLine(4,7);
		ScrDispStr(0, 4, 0x01, "ģ���ʼ��ʧ��\n %d ", "WNet Init Fail\n %d ",iRet);
		BeepFail();
		getkey();
		return iRet;
	}
  
    iRet=WlOpenPort();   //������ͨѶ����
    if(iRet)
    {
        ScrClrLine(4,7);
        ScrDispStr(0,4,0x01,"��ͨ�Ŵ���ʧ��:%d","Open Port Err:  %d",iRet);
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
	
	//AT+GMR\r  ��ȡС�İ汾��,�� G610_V0D.00.3B �� 11.813.01.05.00��
	//AT+CGMM\r ��ȡ��İ汾��,�� MG323-B MU509 �� G610/G620/G510/H330��������,��Ҫ����		
	

	i = 0;
	while((i++)<3)
	{		
		WlSendCmdEx("AT+GMR\r",gWnetVer,sizeof(gWnetVer)); //��ȡС�汾��  ����������   ԭ����ʹ������ж�
		WlSendCmdEx("AT+CGMM\r",gWireVer,sizeof(gWireVer));//��ȡ��汾��  ��Ҫ������

		if( strstr(gWireVer,"G610") || strstr(gWireVer,"G620") || strstr(gWireVer,"G510") || strstr(gWireVer,"H330") )
		{	
			memcpy(gWireVer,gWnetVer,8); //ȡǰ��λΪ���� G610_V0C
			memset(gWireVer+8,0,sizeof(gWireVer)-8);
		}
		else if( strstr(gWnetVer,"Q24") || strstr(gWnetVer,"Q26") || strstr(gWnetVer,"MG815") || strstr(gWnetVer,"QSC6020-APP-3350") )
		{  //��Щ��ģ��,��ʱ��˴���
			if( strstr(gWnetVer,"Q24") ) memcpy(gWireVer,"Q24\x00\x00",5);
			else if( strstr(gWnetVer,"Q26") ) memcpy(gWireVer,"Q26\x00\x00",5);
			else if( strstr(gWnetVer,"MG815") ) memcpy(gWireVer,"MG815\x00\x00",7);
			else if( strstr(gWnetVer,"QSC6020-APP-3350") ) memcpy(gWireVer,"QSC6020-APP-3350\x00\x00",18);
		}
		else
		{
			//������MG323-B/MU509/MC323/MC509/EM701 �ȶ���ֱ����ʹ�� AT+GMR �� AT+CGM���� ����Ҫת��
		}

		if(IsStringOk(gWireVer,1)) continue;
		break;
	}

	//IMEI��
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
		if(gImei[0]==0) //��û�ж���IMEI��,��ʹ�������ָ���ȡ,��������
			WlSendCmdEx("AT+WIMEI?\r",gImei,sizeof(gImei)); //����Q24/Q26 ģ�������Ҫ�õ�
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



