#ifndef  _PUBLIC_H_
#define  _PUBLIC_H_
#include <stdarg.h>
#include <posapi.h>


//#define LAN_HUB	//��Ҫ��LAN_HUB�Ĳ��Գ���
//#define PORT_TEST //��Ҫ�ǽӿڰ�Ĳ���
//#define DEBUG		//���Դ�ӡ
//#define LIB_SO		//��Ҫ����һЩ�����Ǿ�̬������û�ж����

#define ALL_WNET_VER_MATCH //����WNET�汾�����ȶ�
//#define BCM      //����
//#define Venezuela //ί������
//#define BAXI      //����


#ifdef  BCM
#define APPNAME    "BCM_FTEST"
#define STRING_VERSION	 " SXX_POS_FTEST_V3.13  " 
#define SOFT_VERSION	"_BCM_V3.13"
#define SPEECH_ON
#define WIFI_ON
#define BT_ON
#endif


#ifdef  BAXI
#define APPNAME    "BAXI_FTEST"
#define STRING_VERSION	 "BAXI_POS_FTEST_V3.11  " 
#define SOFT_VERSION	"_BAXI_V3.11"
#define SPEECH_ON
#define WIFI_ON
#define BT_ON
#endif


#ifdef  Venezuela
#define APPNAME    "VEN_FTEST"
#define STRING_VERSION	 "  VEN_POS_FTEST_V3.08 " 
#define SOFT_VERSION	"_VEN_V3.08"
#define SPEECH_ON
#define WIFI_ON
#define BT_ON
#endif

#ifdef ALL_WNET_VER_MATCH
#define BCM
#define APPNAME    "MATCH_FTEST"
#define STRING_VERSION	 " SXX_POS_FTEST_V3.13A " 
#define SOFT_VERSION	"_MATCH_V3.13A"
#define SPEECH_ON
#define WIFI_ON
#define BT_ON
#endif


//����ͨ���ն���Ϣ������ѡ�Ļ��ͣ�
#define  S80   7   
#define  SP30  8
#define  S60   9
#define  S90   10
#define  S78   11
#define  S58   14 
#define  T60   0x86 
#define  T52   13
#define  MT30  12
#define  D200  15
#define  D210  16
#define  T610  18
#define  T620  19
#define  S810  200//20
#define  S300  20
#define  S800  21
#define  S900  22
#define  S500  24
#define  S200  0x89

#define  S910	26
#define  CHINESE 1


// T52_POS����
    #define KEYNO 31
	#define KEYSET		0x81		//����
	#define KEYLEFT	    0x83		//���
	#define KEYPBOOK	0x85		//�绰��
	#define KEYPAY		0x86		//֧��
	#define KEYPHONE	0x87		//IP/���뷨
	#define KEYBACK		0x88		//�ز�
	#define KEYRIGHT	0x89		//�Ҽ�
	#define KEYMSG		0x8a		//�˵�
	#define KEYDIALED   0x8e		//ȥ��

//T610_T620 ���ӵİ���
#define     KEYF1               0x01                    
#define     KEYF2               0x02                    
#define     KEYF3               0x03                    
#define     FNKEYF2             0xa5             
#define     FNKEYF3             0Xa6               

#define     KEYADD              0x2B
#define     KEYSUB              0x2D
#define     KEYMUL              0x26
#define     KEYDOT              0x2E

//T620���ӵ���ϼ�
#define     FNKEYADD              0xBB
#define     FNKEYSUB              0xBD
#define     FNKEYMUL              0xB6
#define     FNKEYDOT              0xBE




//��posapi.h�����ж���
#ifdef WIN32
#define TelDial(a) 0
#define TelOffHook() 
#define TelCheck(a)
#define GetHardwareConfig(_A_,_B_,_C_) 1   //ͨ���ؼ��ֲ�ѯӲ��������Ϣ
#define WlSelSim(_A_) 0
#define GetTermInfoExt(_A_,_B_) 0     
#define EnumBaseFont(_A_,_B_) 0
#define s_GetPrnPinTime()     1
#define s_OpenPrnPinTime()    1
#define s_PrnStart(_A_)        1
#define s_ClosePrnPinTime()  1
#define EthMacGet(_A_) 1
#define s_ModemInfo(_A,_B,_C,_D) NULL

#else
extern void s_ModemInfo(uchar *drv_ver,uchar *mdm_name,uchar *last_make_date,uchar *others);
#endif

#ifdef WIN32
int TouchScreenCalibration(void);
void ScrRect(int X1,int Y1,int X2,int Y2,COLORREF BorderColor,COLORREF FillColor);
#endif


#define PICC_LED_ALL 0xff
#define WIFIPORT 5
#define PINPAD 3
#define RS232  0
#define LANPORT	2
#define COM1 0


char Cmu200Result,gBaseChange;
#define F_BASE_CHANGE	"baseChange.dat"
#define F_TSCB			"tscb.dat"
#define F_CMU200		"cmu200.dat"
#define F_TELNUM		"TelNum.dat"
#define F_LANPARA		"LANPara.dat"
#define F_BASIC_INFO	"basicinfo.ini"
#define LOOP_TIME_RECORD  "LoopTimeRecord.dat"//�һ�ʱ���¼���ܴ�С��5��LOOP_STRUCT�Ĵ�С��¼5�ε��һ�ʱ��
#define LOOP_FIRST_ERR_RECORD "CycErrRecord.dat"//�һ���һ�γ��������ʱ��ͳ����ķ����롣

#define WNET_TEST_RECORD  "WNETTEST.DAT"  //���߲��Լ�¼
#define BT_LOG "bt.dat"

typedef struct
{
	uchar StartTime[7];//�һ���ʼʱ��
	uchar StopTime[7]; //�һ�����ʱ��
	ulong LoopSeconds; //�һ���ʱ�䣬��λs
	ulong LoopCnt;	   //�һ�����
}LOOP_STRUCT;

uint LoopStartMs;
uint LoopTempMs;
uint LoopStopMs;
uint LoopMin;


//�������һ���ģ��ʧ�ܴ�����¼
int RecordLog(const char *FileName,const char * fmt,...);
void CalcLoopTime(ulong *Time,ulong *Cnt);

//#define LOOP_FAIL_RECORD  "LoopFailRecord.dat"//�һ��쳣��¼,��ϸ��¼�����һ������,��Сû������,�����ⶼ���ļ�����׷��


/*
void LoopFailCnt(void);
void ShowLoopErr(int LoopAllTime,int LoopAllCnt);
int CycFailCnt[20];
#define LOOP_FAIL_CNT	"LoopFailCnt.dat"//�һ�ʧ�ܴ�����¼���ܴ�С��20��int������
#define CYC_LED     0
#define CYC_LCD		1
#define CYC_KB		2
#define CYC_IC		3
#define CYC_MODEM	4
#define CYC_WNET	5
#define CYC_PRT		6
#define CYC_SPEECH	7
#define CYC_MAG		8
#define CYC_RF		9
#define CYC_FELICA	10
#define CYC_GSEN	11
#define CYC_UHOST	12
#define CYC_SD		13
#define CYC_LAN		14
#define CYC_WIFI	15
#define CYC_BT		16
*/


//��ȡ�汾��Ϣ���㷨����

void xml_strlwr(char *turned);
int  xml_find(uchar *doc, int doc_len, char *find_flag, int find_flag_len, int *find_location);
int  XmlGetElement(uchar *xml_doc, int xmldoc_len, char *ele_name, uchar *ele_value, int va_max_len, int *value_real_len);
int  XmlAddElement(uchar *xml_doc, int xml_doc_max_len, char *ele_name, uchar *ele_value, int value_len, int *xml_real_len);
int  XmlDelElement(uchar *xml_doc, char *ele_name, int *xml_len);

// Public.c�Ĺ�������
int SetCombKey(void);
void DelFacApp(void);
void AppManager(void);
void GetConfig(void);
void SetSrand(void);
void SetComPort(int port);
void BeepOK(void);
void BeepFail(void);
void BeepPrompt(void);
void ShowVer(void);
void ShowFileCtrlErr(uint Line);
uchar *DispTimer(unsigned char line);
int   WaitEsc(unsigned char flag,unsigned short seconds);
int   SendCommand(char *command);
int   RecvReturn(ushort inTimeOut);
void  ShowErr(char *msg);
int   Select(void);
ulong PubCalInterval(uchar *psTimeBegin, uchar *psTimeEnd, uchar *psCalMode, uchar *pucSignal);
unsigned char InputDec(unsigned char inlen,char *dispEnglish,char *dispChinese,unsigned short *dec,unsigned char flag);
uchar InputAsc(uchar *StrCh,uchar *StrEn,uchar *asc, uchar ascLen, uchar ascMaxLen, uchar flag);
uchar GetInputStr(uchar line,uchar mode,uchar maxlen,char *Str);
void ModifyRecord(void);
void DeleteLog(char *fileName);
void ShowSuccessRate(uint TotalNum,uint FailNum);
void Wl_handle_ver(uchar *s,int len);
void AscToBcd(uchar *Bcd, uchar *Asc, ushort Asc_len);
void ScrDispStr(uchar col,uchar row,uchar mode,uchar * szStrCH, uchar * szStrEn,...);
uchar JudgeState(uchar *StrCh,uchar *StrEn);
uchar IsQuitOrDone(uchar *StrCh,uchar *StrEn);
int  ComSendBuf(void *buf,ushort size);
int  ComSends(char * Str,...);
void see_debug(char * Str,...);
int  ComRecvBuf(void);
int  UploadFile(void);
int  UploadLog(void);
int  GetStep(void);
int  CancelCheck(void);
int  GetWirelessVer(void);
int WlSendCmdEx(char *Cmd,char *rsp,int len);
int get_1_num(int input);
int get_0_num(int input);
int  GetInfoExt(void);

int _cat_record_function_for_none_GNUC_(int result,const char * fmt,...);
#define RECORD _cat_record_function_for_none_GNUC_ 




//
void TestAll(void);
void TestCyc(void);
void TestSingle(void);

// flag=0 ��������      flag=1 ���β���        flag=2 ѭ������
// ����0  ���Գɹ�		����1  ����ʧ��
uchar PrinterTest(uchar flag);
uchar ModemTest(uchar flag);
uchar UsbHostTest(uchar flag);
uchar UsbDevTest(uchar flag);
uchar LanTest(uchar flag);

uchar WnetTest(uchar flag);
uchar Cmu200Test(uchar flag);

uchar RfTest(uchar flag);
uchar WifiTest(uchar flag);  
uchar IcTest(uchar flag);
uchar MagTest(uchar flag);
uchar GSensorTest(uchar flag);
uchar TScreenTest(uchar flag);
uchar BtTest(uchar flag);
uchar BeepTest(uchar flag);
uchar KbTest(uchar flag);
uchar LcdTest(uchar flag);
uchar ComATest(uchar flag);
uchar ComBTest(uchar flag);
uchar UDiskTest(uchar flag);
uchar ClkTest(uchar flag);
uchar SpeechTest(uchar flag);
uchar BatteryTest(uchar flag);
uchar PinpadTest(uchar flag);
uchar FelicaTest(uchar flag);
uchar LedTest(uchar flag);
uchar ByPhoneTest(uchar flag);
uchar PaperBackTest(uchar flag);
uchar ScanTest(uchar flag);
uchar LedplyTest(uchar flag);
uchar TelTest(uchar flag);
uchar SDCardTest(uchar flag);
uchar VerTest(uchar flag);
uchar TsTest(uchar flag);
uchar KbLedTest(uchar flag);
uchar TestSim0 (uchar flag);
uchar TestSim1 (uchar flag);
uchar  UDiskATest(uchar flag);//S800˫USB������
uchar SM2Test(uchar flag);
uchar BaseTest(uchar flag);
uchar Lan2Test(uchar flag);


//��������ܲ˵�
void PrinterTestAll(void);
void ModemTestAll(void);
void UsbHostTestAll(void);
void UsbDevTestAll(void);
void LanTestAll(void);
void WnetTestAll(void);
void RfTestAll(void);
void WifiTestAll(void);  
void IcTestAll(void);
void MagTestAll(void);
void GSensorTestAll(void);
void TScreenTestAll(void);
void BtTestAll(void);
void BeepTestAll(void);
void KbTestAll(void);
void LcdTestAll(void);
void ComBTestAll(void);
void UDiskTestAll(void);
void ClkTestAll(void);
void SpeechTestAll(void);
void BatteryTestAll(void);
void PinpadTestAll(void);
void FelicaTestAll(void);
void LedTestAll(void);
void ByPhoneTestAll(void);
void PaperBackTestAll(void);
void ScanTestAll(void);
void LedplyTestAll(void);
void TelTestAll(void);
void PortTestAll(void);
void FlashTestAll(void);
void IrdaTestAll(void);
void ComTestAll(void);
void BandTestAll(void);
void SDCardTestAll(void);
void TsTestAll(void);
void LogTestAll(void);

//�һ��ܲ˵�
uchar PrinterTestCyc(ulong);
uchar ModemTestCyc(ulong);
uchar UsbHostTestCyc(ulong);
uchar UsbDevTestCyc(ulong);
uchar LanTestCyc(ulong);
uchar WnetTestCyc(ulong);
uchar RfTestCyc(ulong);
uchar WifiTestCyc(ulong);  
uchar IcTestCyc(ulong);
uchar MagTestCyc(ulong);
uchar GSensorTestCyc(ulong);
uchar TScreenTestCyc(ulong);
uchar BtTestCyc(ulong);
uchar BeepTestCyc(ulong);
uchar KbTestCyc(ulong);
uchar LcdTestCyc(ulong);
uchar ComBTestCyc(ulong);
uchar UDiskTestCyc(ulong);
uchar ClkTestCyc(ulong);
uchar SpeechTestCyc(ulong);
uchar BatteryTestCyc(ulong);
uchar PinpadTestCyc(ulong);
uchar FelicaTestCyc(ulong);
uchar LedTestCyc(ulong);
uchar ByPhoneTestCyc(ulong);
uchar PaperBackTestCyc(ulong);
uchar ScanTestCyc(ulong);
uchar LedplyTestCyc(ulong);
uchar TelTestCyc(ulong);
uchar SDCardTestCyc(ulong);
uchar TsTestCyc(ulong);



//��������
typedef uchar(*ALL_FUN_P)(uchar flag);
typedef struct
{
	uchar flag;   // 'Y' ��Ҫ����    'N' ����Ҫ����
	uchar result; //  0  ����δͨ��   1  ����ͨ��
	uchar itemStr[10]; //������Ŀ��
	ALL_FUN_P  TestFun; //���Ժ���ָ��
} TEST_ITEM;

//����˵�
typedef void(*SINGLE_FUN_P)(void);
typedef struct 
{
	char ChStr[20];
	char EnStr[20];
	SINGLE_FUN_P TestSingleFun;
}TESTSINGLE_ITEM;


//�һ�����
typedef uchar(*CYC_FUN_P)(ulong Loops);
typedef struct
{
	uchar flag;   // 'Y' ��Ҫ����    'N' ����Ҫ����
	CYC_FUN_P  TestCycFun; //���Ժ���ָ��
} TESTCYC_ITEM;


//�������û�ȡ
uchar gBtAppleFlag;//�Ƿ���ƻ��������֤��1����    0����
unsigned char gIsColorLcd;  // 1�ǲ���  0���ǲ���
unsigned char gBatChange;   //�Ƿ�����ģ�飬1����      0����
unsigned char gTemperature; //�Ƿ�����¶�   1������¶�    0��������¶�
unsigned char gTsTest;		//������ 1����Ҫ����  0������Ҫ����  S200��S1/S2Ҫȡ��
unsigned char gComPort;  //ͨѶ����
unsigned char gIcNum;     //IC������
unsigned char gSamNum;    //SAM������ 
unsigned char gSimNum;    //SIM������
unsigned char gMac[6];    //Mac��ַ

unsigned char TestModule[100];   //��Ҫ���Ե�ģ��
unsigned char VerInfo[10];      //Ҫ��8�ֽ�
unsigned char TermInfo[32];     //Ҫ��30�ֽ�

unsigned char TelNum[20];       //modem���ź���
unsigned char LANPara[10][20];
unsigned char FontType;
unsigned char RcvBuff[200];//

uchar gSn[33],gExSn[33];
uchar gModemSVer[30],gModemHVer[30],gLMDate[30],gOtVer[30];
uchar gWnetVer[50];
uchar gWireVer[50];
uchar gImei[30];
uchar GetWnetVerFlag;
uchar GetExtInfoFlag;
uchar gPPQcom;

char gConfigFileName[20];//ֻ��17���ֽ�
char gFontFileName[30]; 
uint gFontLength;
uint gFlashSize;
uint gNandFlashSize;
uint gSdramSize;
uint gDdrSize;
uint gRfHeight;


uchar gPrinterType[25];
uchar BaseFontSize[10];
uchar BaseMain[5];
uchar BaseBios[5];
uchar BaseDriver[5];
uchar BaseFlashSize[10];
uchar BaseSdramSize[10];
uchar gBTVer[20]; //BT�汾��Ϣ
uchar gBtMac[6];  //BT MAC��ַ
uchar gWifiMac[6]; // WIFI MAC��ַ
uchar gErrnoMsg[100];//���ԵĴ�����


//T_ ��ʾ��Terminfo��Ӧ��Ҳ��������ĿTestModule[]��Ӧ
#define T_TYPE     0
#define T_PRINTER  1
#define T_MODEM    2
//3 MODEMģ��������Ϣ
//4 MODEM����첽������Ϣ
//5 PCI������Ϣ
#define T_USBHOST  6
#define T_USBDEV   7
#define T_LAN      8
#define T_GPRS     9
#define T_CDMA     10
#define T_WIFI     11
#define T_RF       12
#define T_FONT     13

//14 �ֿ�汾��
#define T_IC	   15
#define T_MAG      16
#define T_GSENSOR  17
#define T_WCDMA    18
#define T_TSCREEN  19
#define T_BT       20
//���� ��������Ŀ��Ӧ

#define T_BEEP		21
#define T_KB		22
#define T_LCD		23
#define T_COMB		24
#define T_WNET		25
#define T_UDISK	    26
#define T_SPEECH		27
#define T_CLK			28
#define T_BATTERY		29
#define T_PINPORT		30
#define T_FELICA		31
#define T_LED			32
#define T_BYPHONE		33
#define T_PAPERBACK	34
#define T_SCAN		35
#define T_LEDPLY		36
#define T_TEL			37
#define T_SDCARD        38
#define T_KEYLED		39
#define T_SIM1			40
#define T_UDEV_TEST		41
#define T_COMB_TEST		42	
#define T_UDISKA		43
#define T_FELICA_ALL	44
#define T_SM2			45
#define T_COMA_TEST		46
#define T_BASE			47
#define T_VER			48
#define T_LAN2			49



//�汾��Ϣ���    //ϵͳ������
//#define VER				0
#define PC_READY			1
#define PC_SAVEOK			2
#define PC_VER_CHECK_OK		3
#define PC_NOANSWER			4
#define POS_NO_RECORD		5
#define OPEN_PORT_ERR		6
#define PC_TIMEOUT			7
#define OPEN_RECORD_ERR		8



char InfoOut[50][25];
/*��������,���ܶ���,��Ȼ�����ض���
char *KeyWord[]=
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
	"RF_PARA_1",	"RF_PARA_2",		"RF_PARA_3",		""
};
*/
//C_ ��ʾ�����ñ���Ӧ ����KeyWord[]��Ӧ
#define C_PRINTER 0 
#define C_TOUCH_SCREEN 1 
#define C_MAG_CARD 2
#define C_IC_CARD 3
#define C_RF_1356M 4
#define C_RF_24G 5
#define C_G_SENSOR 6
#define C_ETHERNET 7
#define C_MODEM 8
#define C_CDMA 9
#define C_GPRS 10
#define C_WCDMA 11
#define C_TD 12
#define C_WIFI_NET 13
#define C_BAR_CODE 14
#define C_BLUE_TOOTH 15
#define C_SD_READER 16
#define C_CONFIG_FILE_VER 17
#define C_MAIN_BOARD 18
#define C_PORT_BOARD 19
#define C_MODEM_BOARD 20
#define C_RF_BOARD 21
#define C_ANT_BOARD 22
#define C_GPRS_BOARD 23
#define C_WCDMA_BOARD 24
#define C_TD_BOARD 25
#define C_WIFI_BOARD 26
#define C_KEY_BOARD  27
#define C_IP_BOARD 28
#define C_PRINT_BOARD 29
#define C_POWER_BOARD 30
#define C_SAM_BOARD 31
#define C_ICCARD_BOARD 32
#define C_MESH_BOARD 33
#define C_TELEPHONE_BOARD 34
#define C_DISP_BOARD 35
#define C_RF_PARA 36
#define C_G_SENSOR_PARA 37
#define C_DUAL_SIM 38
#define C_PN  39
#define C_RF_PARA_1  40
#define C_RF_PARA_2  41
#define C_RF_PARA_3  42	
#define C_LCD		 43
#define C_SM_CHIP	 44
#define C_CIPHER_CHIP 45



void  DisSubMenuEx(TESTSINGLE_ITEM TestItem[],char TestNum,char *ChTitle,char *EnTitle,uchar mode);
void  PEDTestAll(void);
uchar PEDTest(uchar flag);
uchar PEDInit(uchar flag);

ST_LCD_INFO stLcdInfo;

#define C_BLUE		0xff0000ff
#define C_GREEN		0xff00ff00
#define C_RED		0xffff0000
#define C_WHITE     0xffffffff
#define C_BLACK     0xff000000
#define C_YELLOW    0x00ffffff


#define ROUTE_LAN 0 //��̫������
#define ROUTE_PPPOE 1 //��ʾPPPoE
#define ROUTE_MODEM 10//��ʾmodem PPP��·
#define ROUTE_WNET 11//��ʾ����(GPRS/CDMA)PPP��·
#define ROUTE_WIFI 12//��ʾWIFI��·



#endif
