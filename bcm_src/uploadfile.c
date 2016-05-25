#include "public.h"
#include <stdarg.h>
#include <stdio.h>  


#define COMPORT 0

static int _cat_log_file(const char *file, const char *str)
{
    int fd;
    int wrLen;
    int ret;
    
    if (freesize() < 4096) return -4;  
    
    fd = open(file, (uchar)(O_RDWR | O_CREATE));
    if (fd < 0) 
    {
        ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
        return -1;
    }
    seek(fd,0,SEEK_END); // 移到文件末尾
    wrLen = strlen(str);
    ret = write(fd,(uchar *)str,wrLen);
    if (ret != wrLen)
    {
        ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
        return -2;
    }
    close(fd);
    return 0;
}

static int _cat_log(const char *file, int line,const char *str)
{
    _cat_log_file(file,str);
    _cat_log_file(file,"\x0D\x0A");
    return 0;
}

/******************************************************
* result -1 不记录信息 0- 记录基本信息 1-记录测试结果
* 目前基本配置信息与测试结果记录信息都放在F_BASIC_INFO文件内
* 若以后有需要再分开存放。
*******************************************************/
int _cat_record_function_for_none_GNUC_(int result,const char * fmt,...)
{
    char buf[1024];
    va_list ap;
    if (result>=0)
    {
        va_start(ap,fmt);
        vsprintf(buf, fmt, ap);
        va_end(ap);
        _cat_log(F_BASIC_INFO,0,buf);
    }
    return 0;
}

/********************************************************/
/*			在文件FileName里面记录数据					*/
/********************************************************/
int RecordLog(const char *FileName,const char * fmt,...)
{
    char buf[1024];
    va_list ap;

#ifndef DEBUG
	if(strstr(FileName,"WNETTEST")!=NULL)
		return 0;
#endif
	
    va_start(ap,fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);
    _cat_log(FileName,0,buf);
	
	return 0;
}

/*****************************************
POS与PC通讯协议
1、pos请求pc准备接收： pos@ready
2、pc准备接收：pc@ready
3、pos发送数据
3、pos发送完毕：pos@stop
4、pc保存完毕：pc@saveok=
5、基本信息匹配完毕：pc@saveok=0/0/0/d（d为预留信息，暂时未用到）

******************************************/
int UploadFile(void)
{
    char buf[8192];
    int txSize;
    int fd;
    int sended;
    int fileSize;
    int i,j;
    int  linecnt=0;
    int ret;
    int sendi;
    uchar ucRet;

    ScrClrLine(2,7);
    ScrDispStr(0,4,1,"握手...","Hand shake...");
    
    ucRet = PortOpen(gPPQcom, "115200,8,n,1");
    if(ucRet)
    {
        ScrDispStr(0,4,1,"打开串口失败","Open Port Err!");
        return OPEN_PORT_ERR;
    }

    i=0;
    TimerSet(4,100);
    while(TimerCheck(4))
    {
        if(!(i%3))        ScrPrint(0,6,1,".....");
        else if((i%3)==1) ScrPrint(0,6,1,"..........");
        else if((i%3)==2 )ScrPrint(0,6,1,"...............");

        PortSends(gPPQcom, (char *)"pos@ready\n", 10);
        if(gPPQcom == RS232A)while(PortTxPoolCheck(gPPQcom));
		else DelayMs(100);
        j=0;
        memset(buf, 0x00, sizeof(buf));
        while(1)
        {
            ucRet=PortRecv(gPPQcom, &buf[j++], 1000);
            if(strstr((char *)buf, (char *)"pc@ready") != NULL ) //已与PC握手成功
            {
                break;
            }
            if(ucRet)  //接收超时
            {
                break;
            }
        }
        if(!ucRet)
        {
            break;
        }
    }
    if(ucRet)
    {
        PortClose(gPPQcom);
        return PC_TIMEOUT;
    }
	ScrClrLine(2,7);
    if ((fd=fexist(F_BASIC_INFO))<0 || filesize(F_BASIC_INFO)<0)
    {
        ScrDispStr(0,4,1,"记录为空！请先测试","No Record!Pls execute test");
        WaitEsc(0,1);
        PortClose(gPPQcom);
        return POS_NO_RECORD;
    }

    //上送记录,调试时可多次上送，正常使用时只用上送一次
    for(sendi=0; sendi<1; sendi++)   
    {   
        fd = open(F_BASIC_INFO,O_RDWR);
        if (fd < 0)  //打开文件出错
        {
            ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
            PortClose(gPPQcom);
            return OPEN_RECORD_ERR; 
        }
        sended =0;
        fileSize = filesize(F_BASIC_INFO);

        ScrDispStr(0,4,1,"上传文件中，请勿关机","Upload file...");
        memset(buf,0x00,sizeof(buf));

        PortReset(gPPQcom);
        while(sended<fileSize)
        {
            txSize = read(fd,buf,sizeof(buf));
            if (txSize > 0)
            {
                ushort txIdx, txLen;
                
                sended += txSize;
                ScrPrint(0,6,1,"Sended %d%%",sended*100/fileSize);

                txIdx = 0;
                while(txIdx<txSize)
                {
                    txLen = txSize - txIdx;
                    if (txLen > 8192) txLen = 8192;
                    ucRet = PortSends(gPPQcom,&((char *)buf)[txIdx],(ushort)txLen);
                    if (ucRet == 0x00)
                    {
                        txIdx += txLen;
                    }
                    while(PortTxPoolCheck(gPPQcom));
                }

                for(i=0; i<txSize; i++)
                {
                    if(buf[i]==0x0d)  //统计行数
                    {
                        linecnt++;
                    }
                }
            }
            else
            {
                ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
                break;
            }
        }
    }
    sprintf(buf,"LINE.cnt=%d\r\n",linecnt);
    

    PortReset(gPPQcom);
    PortSends(gPPQcom,(char *)buf, (ushort)(strlen(buf)));
    while(PortTxPoolCheck(gPPQcom));
    PortSends(gPPQcom,(char *)"pos@stop\n",9);
    while(PortTxPoolCheck(gPPQcom));
    ScrClrLine(2,7);
    ScrDispStr(0,4,1,"正在保存文件..","Saving file...!"); //PC保存数据与核对基本配置信息

    i = 0;
    ret = PC_TIMEOUT;
    TimerSet(4,300);
    memset(buf, 0x00, sizeof(buf));
    while(TimerCheck(4))
    {
        ucRet=PortRecv(gPPQcom, &buf[0], 500);//100ms不保证能收到修改成500ms
        if(!ucRet)
        {
            i++;
            break;
        }
    }
    if(!ucRet)
    {
        while(1)
        {
            ucRet=PortRecv(gPPQcom, &buf[i++], 3000);
            if(ucRet)
            {
                ret = PC_TIMEOUT;
                break;
            }
        }
        if(strstr((char *)buf, "pc@saveok=0/0/0") != NULL )  //POS基本配置信息已经与标准配置匹配好
        {
            ret = PC_VER_CHECK_OK;
        }
        else if(strstr((char *)buf, "pc@saveok=") != NULL )  //PC收到POS上传的记录，但与标准配置不匹配
        {
            ret = PC_SAVEOK;
        }   
    }

    if(ret == PC_VER_CHECK_OK)
    {
        ScrDispStr(0,4,1,"基本信息匹配成功! ","Basic Info Match OK!");
        DeleteLog(F_BASIC_INFO);   //删除测试结果信息
		/*不删除煲机的log，在进入煲机的时候会提示是否删除
        if (fexist(LOOP_TIME_RECORD)>=0)
        {
            DeleteLog(LOOP_TIME_RECORD);
			DeleteLog(LOOP_FAIL_RECORD);
			DeleteLog(LOOP_FAIL_CNT);
        }
        */
        DelayMs(500);
    }
    if(ret == PC_SAVEOK)
    {
        ScrDispStr(0,4,1,"上传文件成功!","Upload file OK!");
		ScrDispStr(0,6,1,"信息匹配失败! ","Info Match Fail!");
        DeleteLog(F_BASIC_INFO);   //删除测试结果信息
        /*不删除煲机的log，在进入煲机的时候会提示是否删除
        if (fexist(LOOP_TIME_RECORD)>=0)
        {
            DeleteLog(LOOP_TIME_RECORD);
			DeleteLog(LOOP_FAIL_RECORD);
			DeleteLog(LOOP_FAIL_CNT);
        }
        */
		BeepFail();
        getkey();
    }
    PortClose(gPPQcom);
    close(fd);
    return ret;
}

void DeleteLog(char *fileName)
{
    int fid;

    if ((fid=fexist(fileName))>=0)
    {
        close(fid);
        remove(fileName);
    }
}

int GetStep(void)
{
    int ucRet,j,readyflag;
    uchar buf[8192];
    readyflag=0;

    ucRet = PortOpen(gPPQcom, "115200,8,n,1");
    if(ucRet)
    {
        ScrDispStr(0,4,1,"打开串口失败%d","OpenPort=%d",ucRet);
        return OPEN_PORT_ERR;
    }
    PortSends(gPPQcom, (char *)"pos@step\x0a",9);
    
	if(gPPQcom == RS232A)while(PortTxPoolCheck(gPPQcom));
	else DelayMs(100);
	
    j=0;
    memset(buf, 0x00, sizeof(buf));
    TimerSet(1,5);
    while(TimerCheck(1))
    {
        ucRet=PortRecv(gPPQcom, &buf[j++], 500);
        
        if(strstr((char *)buf, (char *)"pc@step:S1") != NULL ) //返回的工序号
        {
            PortClose(gPPQcom);
            return 1;
        }
        if(strstr((char *)buf, (char *)"pc@step:S2") != NULL ) //返回的工序号
        {
            PortClose(gPPQcom);
            return 2;
        }
        if(strstr((char *)buf, (char *)"pc@step:S3") != NULL ) //返回的工序号
        {
            PortClose(gPPQcom);
            return 3;
        }
        if(strstr((char *)buf, (char *)"pc@step:S4") != NULL ) //返回的工序号
        {
            PortClose(gPPQcom);
            return 4;
        }
        if(strstr((char *)buf, (char *)"pc@step:S5") != NULL ) //返回的工序号
        {
            PortClose(gPPQcom);
            return 5;
        }
        if(ucRet) break;
    }
    PortClose(gPPQcom);
    return PC_TIMEOUT;
}

void ModifyRecord(void)
{
    uchar i,readbuf[8192];
    int fd,writelen,j,k,readsize;
    
    for(i=0; i<1; i++)   //测试时可以多次上传
    {   
        fd = open(F_BASIC_INFO,O_RDWR);
        if (fd < 0)  //打开文件出错
        {
            ShowFileCtrlErr(__LINE__);   //获取文件操作出错错误码
            return; 
        }
        j=0;
        k=0;
        while(1)//找到TestFinished=1这条记录
        {
            seek(fd,--j,SEEK_END);          
            readsize = read(fd,&readbuf[k],1);
            if(k>=13)
            {
                if(readbuf[k]=='T' && readbuf[k-1]=='e' && readbuf[k-2]=='s' && readbuf[k-3]=='t' &&  readbuf[k-6]=='n' && readbuf[k-7]=='i' && readbuf[k-12]=='=' && readbuf[k-13]=='0')
                {
                    seek(fd,-1,SEEK_CUR);
                    writelen=write(fd,"TestFinished=1",14);
                    close(fd);
                    break;
                }
            }
            if(k>=8192) 
            {
                close(fd);
                break;
            }
            k++;
        }
    }
}


/*计算煲机时间，返回单位为min*/
void CalcLoopTime(ulong *Time,ulong *Cnt)
{
	LOOP_STRUCT LoopSturct[5];
    int fid,i,ret;
	uchar temp[512];
    uchar StrTimeStart[30],StrTimeExit[30];
    ulong LoopTime[5];
	ulong LoopAllTime=0;
	ulong LoopAllCnt=0;
	*Time = *Cnt =0;

    if(fexist(LOOP_TIME_RECORD)<0)
    {
        RECORD(0, "..LOOPTIME=%dmin", 0);
        return;
    }
    
    fid = open(LOOP_TIME_RECORD, O_RDWR);
    if(fid < 0)
    {
        ScrClrLine(2,7);
        ScrDispStr(0,2,1, "打开煲机时间文件失败", "Open LoopTimeFile fail!");
        ScrPrint(0,6,1, "Error:%02x", GetLastError());
        getkey(); 
        RECORD(0, "..LOOPTIME=%dmin", 0);
        return; 
    }

	seek(fid,0,SEEK_SET);
    ret = read(fid,(uchar*)&LoopSturct, sizeof(LoopSturct));
	if( ret != sizeof(LoopSturct) )
	{
		ShowFileCtrlErr(__LINE__);
		RECORD(0, "..LOOPTIME=%dmin", 0);
        return; 
	}
	close(fid);
	
    for (i=0;i<5;i++)
    {
		if(LoopSturct[i].LoopSeconds == 0)break;// 如果是0说明记录已经结束
		LoopTime[i] = LoopSturct[i].LoopSeconds/60;
		LoopAllTime += LoopTime[i];
		LoopAllCnt += LoopSturct[i].LoopCnt;

		memset(StrTimeExit,0x00,sizeof(StrTimeExit));
		memset(StrTimeStart,0x00,sizeof(StrTimeStart));
		
        sprintf((uchar *)StrTimeStart, "%02x%02x-%02x-%02x  %02x:%02x:%02x", 0x20, 
			LoopSturct[i].StartTime[0],LoopSturct[i].StartTime[1],LoopSturct[i].StartTime[2],
			LoopSturct[i].StartTime[3],LoopSturct[i].StartTime[4],LoopSturct[i].StartTime[5]);
        
        sprintf((uchar *)StrTimeExit,  "%02x%02x-%02x-%02x  %02x:%02x:%02x", 0x20,
			LoopSturct[i].StopTime[0],LoopSturct[i].StopTime[1],LoopSturct[i].StopTime[2],
			LoopSturct[i].StopTime[3],LoopSturct[i].StopTime[4],LoopSturct[i].StopTime[5]);  
        
        RECORD(0, "..the %d loop StartTime =%s",i+1,StrTimeStart);                               
        RECORD(0, "..the %d loop ExistTime =%s",i+1,StrTimeExit); 
		RECORD(0, "..LOOPTIME=%dmin",LoopTime[i] );
		RECORD(0, "..LOOP=%d", LoopSturct[i].LoopCnt);
    }
    RECORD(0, "..LOOP_ALL_TIME=%dmin",LoopAllTime );
	RECORD(0, "..LOOP_ALL_CNT= %d ",LoopAllCnt );
	*Time = LoopAllTime;
	*Cnt = LoopAllCnt;

	//记录异常信息
	if(fexist(LOOP_FIRST_ERR_RECORD)<0)
    {
		return ;
    }
	fid = open(LOOP_FIRST_ERR_RECORD,O_RDWR);
    if(fid < 0)
    {
        ScrClrLine(2,7);
        ScrDispStr(0,2,1,"打开煲机异常文件失败", "Open LoopErrFile fail!");
        ScrPrint(0,6,1,"Error:%02x", GetLastError());
        getkey(); 
        return; 
    }
	else
	{
		memset(temp,0,sizeof(temp));
		seek(fid,0,SEEK_SET);
		read(fid,temp,sizeof(temp)-1);
		close(fid);
		RECORD(0,"\n%s\n",temp);
	}
}


void UpLoadLogAll(void)
{
	uchar buf[8192],temp[512];
	long FileSize,LoopAllTime,LoopAllCnt;
	int sended,txSize,fd;
	uchar ucRet;

	memset(buf,0,sizeof(buf));
	ScrCls();
	ScrDispStr(0,0,1,"1-上传整机日志","1-UpLoad Inter Log");
	ScrDispStr(0,2,1,"2-上传查看煲机","2-Up&Check CYC Log");
	ScrDispStr(0,4,1,"3-上传无线情况","3-UpLoad WNET  Log");

	ucRet = PortOpen(gPPQcom, "115200,8,n,1");
    if(ucRet)
    {
		ScrCls();
        ScrDispStr(0,4,1,"打开串口失败","Open Port Err!");
        getkey();
		return;
    }

	ucRet=getkey();
	if(ucRet==KEY1 || ucRet==FNKEYUP) 
	{
		CalcLoopTime(&LoopAllTime,&LoopAllCnt);
		fd = open(F_BASIC_INFO,O_RDWR);
		FileSize = filesize(F_BASIC_INFO);
	}
	else if(ucRet==KEY2 || ucRet==FNKEYDOWN)
	{
		fd = open(LOOP_FIRST_ERR_RECORD,O_RDWR);
		FileSize = filesize(LOOP_FIRST_ERR_RECORD);
		memset(temp,0,sizeof(temp));
		read(fd,temp,sizeof(temp)-1);
		seek(fd,0,SEEK_SET);//指向文件头
		ScrCls();
		ScrPrint(0,0,0,"\n%s\n",temp);
	}
	else if(ucRet==KEY3)
	{
		fd = open(WNET_TEST_RECORD,O_RDWR);
		FileSize = filesize(WNET_TEST_RECORD);
	}
	else
	{
		return;
	}

    if (fd < 0)  //打开文件出错
    {
        
		ShowFileCtrlErr(__LINE__);
        PortClose(gPPQcom);
        return ; 
    }

    sended =0;
    memset(buf,0x00,sizeof(buf));
    PortReset(gPPQcom);
    while(sended<FileSize)
    {
        txSize = read(fd,buf,sizeof(buf));
        if (txSize > 0)
        {
            ushort txIdx, txLen;
            sended += txSize;
            txIdx = 0;
			
            while(txIdx<txSize)
            {
                txLen = txSize - txIdx;
                if (txLen > 8192) txLen = 8192;
                ucRet = PortSends(gPPQcom,&((char *)buf)[txIdx],(ushort)txLen);
                if (ucRet == 0x00)
                {
                    txIdx += txLen;
                }
                while(PortTxPoolCheck(gPPQcom));
            }

        }
        else
        {
            ShowFileCtrlErr(__LINE__);
            break;
        }
    }
	getkey();
	PortClose(gPPQcom);
}


void DeleteLogAll(void)
{
	uchar ucRet;
	ScrCls();
	ScrDispStr(0,0,1,"1-删除整机日志","1-Del TestAll Log");
	ScrDispStr(0,2,1,"2-删除煲机日志","2-Del TestCyc Log");
	ScrDispStr(0,4,1,"3-删除无线日志","3-Del   WNET  Log");
	ScrDispStr(0,6,1,"4-删除所有日志","4-Del   ALL   Log");
	ucRet=getkey();
	ScrCls();
	if(ucRet==KEY1 || ucRet==FNKEYUP)
	{
		ucRet=IsQuitOrDone("删除整机日志?","Del AllLog?");
		if(ucRet==KEYENTER)DeleteLog(F_BASIC_INFO);
	}
	else if(ucRet==KEY2)
	{
		ucRet=IsQuitOrDone("删除煲机日志?","Del CycLog?");
		if(ucRet==KEYENTER || ucRet==FNKEYDOWN)
		{
			DeleteLog(LOOP_TIME_RECORD);
			DeleteLog(LOOP_FIRST_ERR_RECORD);
		}
	}
	else if(ucRet==KEY3)
	{
		ucRet=IsQuitOrDone("删除无线日志?","Del WNetLog?");
		if(ucRet==KEYENTER || ucRet==FNKEYDOWN)
		{
			DeleteLog(WNET_TEST_RECORD);
		}
	}
	else if(ucRet==KEY4)
	{
		ucRet=IsQuitOrDone("删除所有日志?","Del AllLog?");
		if(ucRet==KEYENTER || ucRet==FNKEYDOWN)
		{
			DeleteLog(WNET_TEST_RECORD);
			DeleteLog(LOOP_TIME_RECORD);
			DeleteLog(LOOP_FIRST_ERR_RECORD);
			DeleteLog(F_BASIC_INFO);
		}
	}

}

void UploadFileTmp(void)
{
	UploadFile();
}
void LogTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-上传整机日志",	"1-UploadAll  ",	UploadFileTmp	},
		{"2-上传普通日志",	"2-UploadOther",	UpLoadLogAll	},
		{"3-删除日志",		"3-DelLog",			DeleteLogAll	},
	};

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"日志操作","Log Control",3);

}
