#include "public.h"

void SetComPort(int comport)
{
	if (comport >= 0 && comport <=5){
		gComPort = comport;
	}
	else{
		gComPort=0;
	}	
	
}


void BeepOK(void)
{
	Beef(2,100);
}


void BeepFail(void)
{
	Beef(5,700);
}


//处理AT命令回送信息

void Wifi_handle_ver(uchar *s,int len)
{
    int ii,jj;
     //把I/OK去掉
    unsigned char *p;
	if(s==NULL)  return;
    while(1)
    {
		p=strstr(s,"I/OK");
        if(p==NULL) break;
        memset(p,' ',4);
    }
    // \r\n换成空格
    for(ii=0;ii<len;ii++) 
    {
        if(!s[ii]) break;
        if(('\r'==s[ii])||('\n'==s[ii]))
        {
           s[ii]=' ';
        }
    }
    //去掉AT+命令
    while(1)
    {
		p=strstr(s,"AT+");
        if(p==NULL) break;
		while(*p!=' ' && *p!=0)
        {
			memset(p,' ',1);
			p++;
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
     if(s[len-1]==' ') s[len-1]=0;
     else
     s[len]=0;
}

uchar *DispTimer(unsigned char line)
{
	int year;
	uchar buff[8];
	static uchar timeStr[30];
	memset(buff,0,sizeof(buff));
	memset(timeStr,0,sizeof(timeStr));
	GetTime(buff);
	year=1900+(buff[0] >>4)*10+(buff[0] & 0x0f); // year
	if(buff[0]<=0x49) year+=100;
	buff[1]=(buff[1] >>4)*10+(buff[1] & 0x0f);   // month 
	buff[2]=(buff[2] >>4)*10+(buff[2] & 0x0f);   // date
	buff[3]=(buff[3] >>4)*10+(buff[3] & 0x0f);   // hour
	buff[4]=(buff[4] >>4)*10+(buff[4] & 0x0f);   // minute
	buff[5]=(buff[5] >>4)*10+(buff[5] & 0x0f);   // second
	ScrPrint(0,line,0x00," %04d/%02d/%02d %02d:%02d:%02d ",year,buff[1],buff[2],buff[3],buff[4],buff[5]);
	sprintf(timeStr,"%04d-%02d-%02d %02d:%02d:%02d",year,buff[1],buff[2],buff[3],buff[4],buff[5]);
	return timeStr;
}

// flag: 0-no disp time  1-disp time  2-ENTERKEY invalid + no disp time  3-ENTERKEY invalid + disp time
/* flag: 0--不显示时间(在屏幕的第7行)，按CANCEL键返回1，按其他键返回0；
         1--显示时间，按CANCEL键返回1，按其他键返回0；
         2--不显示时间，按CANCEL键返回1，按其他键不作响应，直到定时完毕为止；
         3--显示时间，按CANCEL键返回1，按其他键不作响应，直到定时完毕为止。
  */
int WaitEsc(unsigned char flag,unsigned short seconds)
{
	unsigned short LeftTime_Last,LeftTime_Now;

	if(seconds)
	{
		LeftTime_Last=seconds*10;
		TimerSet(0,(unsigned short)(seconds*10));
	}
	if(flag==1 || flag==3) DispTimer(7);
	while(1)
	{
		if(seconds)
		{
			LeftTime_Now=TimerCheck(0);
			if(!LeftTime_Now) return 0;
			if(LeftTime_Now!=LeftTime_Last && (flag==1 || flag==3))
			{
				DispTimer(7);
				LeftTime_Last=LeftTime_Now;
			}
		}
		if(!kbhit())
		{
			if(getkey()==KEYCANCEL) return 1;
			else if(flag<=1) return 0;
			else continue;
		}
	}
}
int Select(void)
{
	uchar ucRet;
	ScrClrLine(2,7);
	ScrDispStr(0,2,1,"[1/向上]-重测","[1/UP]-ReTest");
	ScrDispStr(0,4,1,"[取消]-退出","[CANCLE]-Quit");
	ScrDispStr(0,6,1,"[确认]-继续","[ENTER]-GoOn");
	Beef(4,100);

	while(1)
	{
	    kbflush();
		ucRet=getkey();
		if(ucRet!=KEY1 && ucRet!=KEYUP && ucRet!=KEYCANCEL && ucRet!=KEYENTER)
		{
		    Beef(5,200);
			continue;
		 }
		 break;
	}
	return ucRet;
}

int CancelCheck(void)
{
	while(1)
	{
		if(kbhit()) return 0;
		else if(getkey()==KEYCANCEL){
			kbflush(); return 1;
		}
		else continue;
	}
}

void ShowSuccessRate(uint TotalNum,uint FailNum)
{
	ScrClrLine(2,7);
	ScrDispStr(0,2,0x01,"测试次数:%ld",	"Test Times:%d",	TotalNum);
	ScrDispStr(0,4,0x01,"失败次数:%ld",	"Fail Times:%ld",	FailNum);
	if(TotalNum==0)	ScrDispStr(0,6,0x01,"成功率  :0%%",	"SucceedRate:0%%");
	else ScrDispStr(0,6,0x01,"成功率  :%d%%",	"SucceedRate:%d%%",	(((TotalNum-FailNum)*100)/TotalNum));
}

unsigned char InputDec(unsigned char inlen,char *dispEnglish,char *dispChinese,unsigned short *dec,unsigned char flag)
{
	unsigned char ret,buff[10];

	if(dispChinese==NULL || dispEnglish==NULL )
	{
		return 1;
	}
	ScrClrLine(2,7);
	if(FontType == CHINESE)
	{
		ScrPrint(0,2,0x01,"%s",dispChinese);
	}
	else
	{
		ScrPrint(0,2,0x01,"%s",dispEnglish);
	}

	if(flag) ScrPrint(0,4,0x01,"%d",*dec);
	ScrGotoxy(0,6);
	kbflush();
	while(1)
	{
		memset(buff,0x00,sizeof(0x00));
		ret = GetString(buff,0xe1,1,inlen);
		if(ret == 0xff) return 1;
		if((ret == 0x0d) && flag) return 0;
		if(buff[0] == 0) Beep();
		else break;
	}
	buff[buff[0] + 1] = 0;
	*dec = (unsigned short)atoi((char*)buff + 1);
	ScrClrLine(2,7);
	return 0;
}




uchar InputAsc(uchar *StrCh,uchar *StrEn,uchar *asc, uchar ascLen, uchar ascMaxLen, uchar flag)
{
	unsigned char ret,buff[100];

	if(StrEn==NULL || StrCh==NULL || asc==NULL)
	{
		return 1;
	}

	ScrClrLine(2,7);
	ScrDispStr(0,2,1,StrCh,StrEn);

	if(flag) ScrPrint(0,4,0x01,"%s",asc);
	ScrGotoxy(0,6);
	kbflush();
	while(1)
	{
		memset(buff,0x00,sizeof(0x00));
		ret = GetString(buff, 0xF1, 0, ascMaxLen);
		if(ret == 0xff) return 1;
		if((ret == 0x0d) && flag) return 0;
		if(buff[0] == 0) Beep();
		else break;
	}

	memset(asc,0x00,ascLen);
	memcpy(asc,buff+1,buff[0]);
	asc[buff[0]]='\0';
	ScrClrLine(2,7);
	return 0;
}

void SetSrand(void)
{
	unsigned char szGetSysTime[8],ucTmp;
	unsigned int usSeed;

	memset(szGetSysTime,0,sizeof(szGetSysTime));
	usSeed=0;
	GetTime(szGetSysTime);
	
	ucTmp=szGetSysTime[5];
	usSeed+=((ucTmp&0xF0)>>4)*10 + ucTmp&0x0F;
	ucTmp=szGetSysTime[4];
	usSeed+=(((ucTmp&0xF0)>>4)*10 + ucTmp&0x0F)*60;
	ucTmp=szGetSysTime[3];
	usSeed+=(((ucTmp&0xF0)>>4)*10 + ucTmp&0x0F)*3600;
	srand(usSeed);	
}

void AscToBcd(uchar *Bcd, uchar *Asc, ushort Asc_len)
{
	/*~~~~~~~~~~~~~~~~*/
	uchar	is_high, by;
	/*~~~~~~~~~~~~~~~~*/

	is_high = !(Asc_len % 2);
	*Bcd = 0x00;
	while(Asc_len-- > 0)
	{
		by = *Asc++;

		if(!(by & 0x10) && (by > 0x30))
		{
			by += 9;
		}
		/* 对字母和空格的处理,小写变大写,空格变0 */
		if(is_high)
		{
			*Bcd = by << 4;
		}
		else
		{
			by &= 0x0f;
			*Bcd++ |= by;
		}

		is_high = !is_high;
	}
}


//处理一下版本信息


// send command
int SendCommand(char *command)
{
	int i,len;
	unsigned char ucRet;
	
	len = strlen(command);
	for(i=0; i<len; i++)
	{
		ucRet = PortSend(gComPort,command[i]);
		if(ucRet)
		{
			ScrPrint(0,4,1,"PortSend()=0x%02x",ucRet);
			BeepFail();
			kbflush();
			getkey();
			return 1;
		}
	}
	return 0;
}


// flag 0 - 其它命令  1 - 连接
int RecvReturn(ushort inTimeOut)
{
	int i;
	unsigned char ucRet;
	ushort timeout;	
	i=0;
	memset(RcvBuff,0x00,sizeof(RcvBuff));
	timeout = 60000;
	while(1)
	{
		ucRet=PortRecv(gComPort, &RcvBuff[i++], timeout);
		if(strstr((char *)RcvBuff, "I/OK") != NULL )	 
		{
			return 0;
		}
		if(strstr((char *)RcvBuff, "I/ERROR") != NULL )	 
		{
			return 1;
		}
		
		if(!ucRet)
		{
			timeout=inTimeOut;
		}
		if(ucRet)
		{
			break;
		}
	}
	return 1;
}

int SetCombKey(void)
{
#define  ELE_NAME_MAX 100
#define  XML_DOC_MAX 8192

	char ele_name[ELE_NAME_MAX], InfoOut[XML_DOC_MAX], ele_doc[1024];
	int r, len;

	memset(ele_name, 0, sizeof(ele_name));
	memset(InfoOut, 0, sizeof(InfoOut));
	memset(ele_doc, 0, sizeof(ele_doc));

	ScrCls();
	strcpy(ele_name, "root"); 
	r = XmlAddElement(InfoOut, XML_DOC_MAX, ele_name, ele_doc, strlen(ele_doc), &len); 

	if (r != 0)
	{
		  if (r==108) 
				 return -1;
		  else
				 return -2;
	}

	strcpy(ele_name, "KeyCombination"); 
	sprintf(ele_doc, "1");
	r = XmlAddElement(InfoOut, XML_DOC_MAX, ele_name, ele_doc, strlen(ele_doc), &len); 
	
	if (r != 0)
	{
		  if (r==108) return -1;
		  else        return -2;
	}
	r = SysConfig(InfoOut, len);
	return r;

}

void ShowFileCtrlErr(uint Line)
{
	int	error = GetLastError();   //获取文件操作出错错误码
    if(error)
    {
        BeepFail();
        ScrPrint(0,7,0, "Line=%4d FileErr=%3d",Line, error);
        getkey();
    }
}

void ScrDispStr(uchar col,uchar row,uchar mode, uchar* szStrCH, uchar*szStrEn,...)
{
	va_list vaMarker;
	char TheBuff[512];

	if(szStrCH == NULL || szStrEn == NULL)
	{
		return;
	}
	memset(TheBuff,0,sizeof(TheBuff));
	va_start(vaMarker,szStrEn); 

	if(TermInfo[13]==1)//中文
		vsprintf(TheBuff,szStrCH,vaMarker); 
	else
		vsprintf(TheBuff,szStrEn,vaMarker); 
	
	va_end(vaMarker);
	
    ScrPrint(col,row,mode,TheBuff);
	
	return;
}


void AppManager()
{
    uchar i, ret;
    int appNum=0;//应用总数目；
    APPINFO info;
    uchar AppNameList[4][30];
    uchar AppNo[4];

    memset(AppNameList, 0, sizeof(AppNameList));
    memset(AppNo, 0, sizeof(AppNo));

    for(i=1; i<=23; i++)
    {
        if(ReadAppInfo((uchar)i, &info) == 0)
        {
            appNum++;
            if(appNum <= 4)
            {
                strcpy(AppNameList[appNum-1], info.AppName);
                AppNo[appNum-1]=info.AppNum;

                if(appNum == 4) break;  
            }
        }
    }

    if(appNum == 0)
    {
        return;
    }


    ScrCls();
    for(i=0; i<appNum; i++)
    {
        if(AppNameList[i] != NULL)
            ScrPrint(0,i,0,"%d-%s", i+1, AppNameList[i]);
    }
	
    ret = getkey();
    if(ret>=KEY1 && ret<=KEY4)
    {
        RunApp(AppNo[ret-0x31]);
    }
	
}

void DelFacApp()
{
	uchar ucRet;
	APPINFO Info;
	memset(&Info, 0, sizeof(Info));
	ReadAppInfo(0, &Info);
	ScrCls();
	ScrDispStr(0,0,1,"要删除工厂程序??", "DeL Factory APP?");
	ScrDispStr(0,2,1,"[1/向上]-是\n[其他]-否", "[1/UP]-YES\n[OTHER]-NO");
	
	ucRet = getkey();
	if(ucRet==KEY1 || ucRet==KEYUP) 
	{
		DeleteLog(LOOP_FIRST_ERR_RECORD);
		DeleteLog(LOOP_TIME_RECORD);
		DeleteLog(F_BASIC_INFO);
		if(DelAppFile(Info.AppName)==0)
		{
			ScrCls();
			ScrDispStr(0,4,1, "删除成功", "Delete Success");
		}
		else
		{
			ScrCls();
			ScrDispStr(0,4,1, "删除失败", "Delete Failed");
		}
		DelayMs(2000);
	}
	
}



/************************************************************************/
/*判断对话框  大小：x/2 * y/2                                           */
/*输入：需要判断的项目													*/
/*mode=0：某项测试是否OK      mode=1：某项测试是否要退出				*/
/************************************************************************/
uchar JudgeState(uchar *StrCh,uchar *StrEn)
{
	uchar ucRet;
	if(StrCh==NULL || StrEn==NULL){
		return 0;
	}
	ScrCls();
	ScrDispStr(0,0,1,StrCh,StrEn);
	ScrDispStr(0,2,1,"测试是否正常?","Test Is OK?");
	ScrDispStr(0,4,1,"[确认]-是","[ENTER]-YES");
	ScrDispStr(0,6,1,"[其他]-否","[OTHER]-NO ");
	Beef(4,100);
	kbflush();
	ucRet=getkey();
	return ucRet;
}

uchar IsQuitOrDone(uchar *StrCh,uchar *StrEn)
{
	uchar ucRet;
	if(StrCh==NULL || StrEn==NULL){
		return 0;
	}
	ScrClrLine(2,7);
	ScrDispStr(0,2,1,StrCh,StrEn);
	ScrDispStr(0,4,1,"[确认]-是","[ENTER]-YES");
	ScrDispStr(0,6,1,"[其他]-否","[OTHER]-NO ");
	Beef(4,100);
	kbflush();
	ucRet=getkey();
	return ucRet;
}

void get_cdma_meid(void)
{
	ScrCls();
	GetWirelessVer();
	see_debug("SN=%s,MEID=%s",gSn,gImei);
	ScrCls();
	ScrPrint(0,2,1,"upload ok");
	while(1){
		if(CancelCheck())return;
	}
}

extern void check_base_change(void);
extern void check_battery(void);
//mode:  1 程序主界面        2 单项主界面            3 模块主界面
void DisSubMenuEx(TESTSINGLE_ITEM TestItem[],char TestNum,char *ChTitle,char *EnTitle,uchar mode)
{
	uchar Page,ucRet;
	uchar PageMum,i,Num;
	int TestId;
	if(mode==1 || mode==2)Num=6;//每页6个菜单
	if(mode==3)Num=3;//每页3个菜单

	PageMum=TestNum/Num+1;//总页数
	if((TestNum%Num)==0)PageMum--;//如果整除需要-1

	TestId=0;//从0开始
	Page=0;//从0开始	
	while(1)
	{
		ScrCls();
		CLcdSetBgColor(C_WHITE);
		Page=TestId/Num;//当前页
		if(mode==1)ScrPrint(0,0,0x80,STRING_VERSION);
		else{
			ScrDispStr(0,0,1,ChTitle,EnTitle);
			ScrPrint(104,0,1,"%d/%d",Page+1,PageMum);
		}	
	
		for(i=0;i<Num;i++)
		{
			if(i+Page*Num>=TestNum)break;
			if(Num==6)
			{
				if(i%2==0)ScrDispStr(0,(i/2+1)*2,0x01,TestItem[i+Page*Num].ChStr,TestItem[i+Page*Num].EnStr);
				else ScrDispStr(64,(i/2+1)*2,0x01,TestItem[i+Page*Num].ChStr,TestItem[i+Page*Num].EnStr);
			}
			else if(Num==3)
			{
				ScrDispStr(0,(i+1)*2,0x01,TestItem[i+Page*Num].ChStr,TestItem[i+Page*Num].EnStr);
			}
		}

		while(2)
		{
			
			if(TermInfo[0]==T610)
			{
				i=TestId%Num;
				if(Num==6)
				{
					if(i%2==0)ScrDispStr(0,(i/2+1)*2,0x81,TestItem[i+Page*Num].ChStr,TestItem[i+Page*Num].EnStr);
					else ScrDispStr(64,(i/2+1)*2,0x81,TestItem[i+Page*Num].ChStr,TestItem[i+Page*Num].EnStr);
				}
				else if(Num==3)
				{
					ScrDispStr(0,(i+1)*2,0x81,TestItem[i+Page*Num].ChStr,TestItem[i+Page*Num].EnStr);
				}
			}
			
			while(kbhit())
			{ 
				if(mode==1)DispTimer(1);
				DelayMs(200);
			}
			
			ucRet=getkey();
			if(ucRet==KEYCANCEL) return ;
			else if(ucRet==KEYFN){get_cdma_meid();break;}
			else if(ucRet==KEY9){BaseTest(1);break;}
			else if(ucRet==KEY7){check_battery();break;}
			else if(ucRet==KEYDOWN && TermInfo[0]==T610)TestId++;
			else if(ucRet==KEYUP && TermInfo[0]==T610)TestId--;
			else if(ucRet==KEY8 || ucRet==KEYF1 || ucRet==KEYUP || ucRet==FNKEYUP)TestId=TestId-Num;
			else if(ucRet==KEY0 || ucRet==KEYF2 || ucRet==KEYDOWN || ucRet==FNKEYDOWN)TestId=TestId+Num;
			else if(ucRet==KEYENTER)
			{
				if(TermInfo[0]==T610)
				{
					TestItem[TestId].TestSingleFun();
					break;
				}
				else TestId=TestId+Num;
			}
			else if( ucRet>=KEY1 && ucRet<=(KEY0+Num) )
			{
				i=Page*Num+ucRet-0x31;
				if(i>=TestNum)BeepFail();
				else TestItem[i].TestSingleFun();
				break;
			}
			if(TestId<0)TestId=TestNum-1;
			if(TestId>=TestNum)TestId=0;
			break;
		}//2
	}//1

}

int get_1_num(int input)
{
	int i;
	int num = 0;
	for(i=0;i<32;i++)
	{
		if(input&(0x01<<i))
			num++;
	}
	return num;

}

int get_0_num(int input)
{
	return (32-get_1_num(input));
}

int ComSends(char * Str,...)
{
	
#ifdef DEBUG
	va_list vaMarker;
	char TheBuff[2048];
	int StrLen;
	uchar ucRet;
	va_start(vaMarker, Str); 
    StrLen = vsprintf(TheBuff,Str,vaMarker); 
	va_end(vaMarker);
	TheBuff[StrLen] = 0x00;
	TheBuff[StrLen] = 0x0D;
	TheBuff[StrLen+1] = 0x0A;
	TheBuff[StrLen+2] = 0x00;
	StrLen += 2;
	ucRet = PortOpen(0,"115200,8,n,1");
	if(ucRet ==0)
	{
		PortSends(0,TheBuff,(ushort)StrLen);
		PortClose(0);
	}
#endif
	return 0;
}

void see_debug(char * Str,...)
{
	va_list vaMarker;
	char TheBuff[2048];
	int StrLen;
	uchar ucRet;
	va_start(vaMarker, Str); 
    StrLen = vsprintf(TheBuff,Str,vaMarker); 
	va_end(vaMarker);
	TheBuff[StrLen] = 0x00;
	TheBuff[StrLen] = 0x0D;
	TheBuff[StrLen+1] = 0x0A;
	TheBuff[StrLen+2] = 0x00;
	StrLen += 2;
	ucRet = PortOpen(0,"115200,8,n,1");
	if(ucRet ==0)
	{
		PortSends(0,TheBuff,(ushort)StrLen);
		PortClose(0);
	}
}

