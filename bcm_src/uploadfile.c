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
        ShowFileCtrlErr(__LINE__);   //��ȡ�ļ��������������
        return -1;
    }
    seek(fd,0,SEEK_END); // �Ƶ��ļ�ĩβ
    wrLen = strlen(str);
    ret = write(fd,(uchar *)str,wrLen);
    if (ret != wrLen)
    {
        ShowFileCtrlErr(__LINE__);   //��ȡ�ļ��������������
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
* result -1 ����¼��Ϣ 0- ��¼������Ϣ 1-��¼���Խ��
* Ŀǰ����������Ϣ����Խ����¼��Ϣ������F_BASIC_INFO�ļ���
* ���Ժ�����Ҫ�ٷֿ���š�
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
/*			���ļ�FileName�����¼����					*/
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
POS��PCͨѶЭ��
1��pos����pc׼�����գ� pos@ready
2��pc׼�����գ�pc@ready
3��pos��������
3��pos������ϣ�pos@stop
4��pc������ϣ�pc@saveok=
5��������Ϣƥ����ϣ�pc@saveok=0/0/0/d��dΪԤ����Ϣ����ʱδ�õ���

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
    ScrDispStr(0,4,1,"����...","Hand shake...");
    
    ucRet = PortOpen(gPPQcom, "115200,8,n,1");
    if(ucRet)
    {
        ScrDispStr(0,4,1,"�򿪴���ʧ��","Open Port Err!");
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
            if(strstr((char *)buf, (char *)"pc@ready") != NULL ) //����PC���ֳɹ�
            {
                break;
            }
            if(ucRet)  //���ճ�ʱ
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
        ScrDispStr(0,4,1,"��¼Ϊ�գ����Ȳ���","No Record!Pls execute test");
        WaitEsc(0,1);
        PortClose(gPPQcom);
        return POS_NO_RECORD;
    }

    //���ͼ�¼,����ʱ�ɶ�����ͣ�����ʹ��ʱֻ������һ��
    for(sendi=0; sendi<1; sendi++)   
    {   
        fd = open(F_BASIC_INFO,O_RDWR);
        if (fd < 0)  //���ļ�����
        {
            ShowFileCtrlErr(__LINE__);   //��ȡ�ļ��������������
            PortClose(gPPQcom);
            return OPEN_RECORD_ERR; 
        }
        sended =0;
        fileSize = filesize(F_BASIC_INFO);

        ScrDispStr(0,4,1,"�ϴ��ļ��У�����ػ�","Upload file...");
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
                    if(buf[i]==0x0d)  //ͳ������
                    {
                        linecnt++;
                    }
                }
            }
            else
            {
                ShowFileCtrlErr(__LINE__);   //��ȡ�ļ��������������
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
    ScrDispStr(0,4,1,"���ڱ����ļ�..","Saving file...!"); //PC����������˶Ի���������Ϣ

    i = 0;
    ret = PC_TIMEOUT;
    TimerSet(4,300);
    memset(buf, 0x00, sizeof(buf));
    while(TimerCheck(4))
    {
        ucRet=PortRecv(gPPQcom, &buf[0], 500);//100ms����֤���յ��޸ĳ�500ms
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
        if(strstr((char *)buf, "pc@saveok=0/0/0") != NULL )  //POS����������Ϣ�Ѿ����׼����ƥ���
        {
            ret = PC_VER_CHECK_OK;
        }
        else if(strstr((char *)buf, "pc@saveok=") != NULL )  //PC�յ�POS�ϴ��ļ�¼�������׼���ò�ƥ��
        {
            ret = PC_SAVEOK;
        }   
    }

    if(ret == PC_VER_CHECK_OK)
    {
        ScrDispStr(0,4,1,"������Ϣƥ��ɹ�! ","Basic Info Match OK!");
        DeleteLog(F_BASIC_INFO);   //ɾ�����Խ����Ϣ
		/*��ɾ���һ���log���ڽ����һ���ʱ�����ʾ�Ƿ�ɾ��
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
        ScrDispStr(0,4,1,"�ϴ��ļ��ɹ�!","Upload file OK!");
		ScrDispStr(0,6,1,"��Ϣƥ��ʧ��! ","Info Match Fail!");
        DeleteLog(F_BASIC_INFO);   //ɾ�����Խ����Ϣ
        /*��ɾ���һ���log���ڽ����һ���ʱ�����ʾ�Ƿ�ɾ��
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
        ScrDispStr(0,4,1,"�򿪴���ʧ��%d","OpenPort=%d",ucRet);
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
        
        if(strstr((char *)buf, (char *)"pc@step:S1") != NULL ) //���صĹ����
        {
            PortClose(gPPQcom);
            return 1;
        }
        if(strstr((char *)buf, (char *)"pc@step:S2") != NULL ) //���صĹ����
        {
            PortClose(gPPQcom);
            return 2;
        }
        if(strstr((char *)buf, (char *)"pc@step:S3") != NULL ) //���صĹ����
        {
            PortClose(gPPQcom);
            return 3;
        }
        if(strstr((char *)buf, (char *)"pc@step:S4") != NULL ) //���صĹ����
        {
            PortClose(gPPQcom);
            return 4;
        }
        if(strstr((char *)buf, (char *)"pc@step:S5") != NULL ) //���صĹ����
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
    
    for(i=0; i<1; i++)   //����ʱ���Զ���ϴ�
    {   
        fd = open(F_BASIC_INFO,O_RDWR);
        if (fd < 0)  //���ļ�����
        {
            ShowFileCtrlErr(__LINE__);   //��ȡ�ļ��������������
            return; 
        }
        j=0;
        k=0;
        while(1)//�ҵ�TestFinished=1������¼
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


/*�����һ�ʱ�䣬���ص�λΪmin*/
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
        ScrDispStr(0,2,1, "���һ�ʱ���ļ�ʧ��", "Open LoopTimeFile fail!");
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
		if(LoopSturct[i].LoopSeconds == 0)break;// �����0˵����¼�Ѿ�����
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

	//��¼�쳣��Ϣ
	if(fexist(LOOP_FIRST_ERR_RECORD)<0)
    {
		return ;
    }
	fid = open(LOOP_FIRST_ERR_RECORD,O_RDWR);
    if(fid < 0)
    {
        ScrClrLine(2,7);
        ScrDispStr(0,2,1,"���һ��쳣�ļ�ʧ��", "Open LoopErrFile fail!");
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
	ScrDispStr(0,0,1,"1-�ϴ�������־","1-UpLoad Inter Log");
	ScrDispStr(0,2,1,"2-�ϴ��鿴�һ�","2-Up&Check CYC Log");
	ScrDispStr(0,4,1,"3-�ϴ��������","3-UpLoad WNET  Log");

	ucRet = PortOpen(gPPQcom, "115200,8,n,1");
    if(ucRet)
    {
		ScrCls();
        ScrDispStr(0,4,1,"�򿪴���ʧ��","Open Port Err!");
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
		seek(fd,0,SEEK_SET);//ָ���ļ�ͷ
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

    if (fd < 0)  //���ļ�����
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
	ScrDispStr(0,0,1,"1-ɾ��������־","1-Del TestAll Log");
	ScrDispStr(0,2,1,"2-ɾ���һ���־","2-Del TestCyc Log");
	ScrDispStr(0,4,1,"3-ɾ��������־","3-Del   WNET  Log");
	ScrDispStr(0,6,1,"4-ɾ��������־","4-Del   ALL   Log");
	ucRet=getkey();
	ScrCls();
	if(ucRet==KEY1 || ucRet==FNKEYUP)
	{
		ucRet=IsQuitOrDone("ɾ��������־?","Del AllLog?");
		if(ucRet==KEYENTER)DeleteLog(F_BASIC_INFO);
	}
	else if(ucRet==KEY2)
	{
		ucRet=IsQuitOrDone("ɾ���һ���־?","Del CycLog?");
		if(ucRet==KEYENTER || ucRet==FNKEYDOWN)
		{
			DeleteLog(LOOP_TIME_RECORD);
			DeleteLog(LOOP_FIRST_ERR_RECORD);
		}
	}
	else if(ucRet==KEY3)
	{
		ucRet=IsQuitOrDone("ɾ��������־?","Del WNetLog?");
		if(ucRet==KEYENTER || ucRet==FNKEYDOWN)
		{
			DeleteLog(WNET_TEST_RECORD);
		}
	}
	else if(ucRet==KEY4)
	{
		ucRet=IsQuitOrDone("ɾ��������־?","Del AllLog?");
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
		{"1-�ϴ�������־",	"1-UploadAll  ",	UploadFileTmp	},
		{"2-�ϴ���ͨ��־",	"2-UploadOther",	UpLoadLogAll	},
		{"3-ɾ����־",		"3-DelLog",			DeleteLogAll	},
	};

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"��־����","Log Control",3);

}
