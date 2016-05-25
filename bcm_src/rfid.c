#include "public.h"

void RfTest1(void)
{
	RfTest(1);
}
void RfTest2(void)
{
	RfTest(2);
}
void RfTest3(void)
{
	RfTest(3);
}

uchar NewRfTest(uchar flag);
void NewRfTest1(void)
{
	NewRfTest(0);
}

void RfTestAll(void)
{
    uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-����",		"1-Single",		RfTest1},
		{"2-ѭ��",		"2-Cycle ",		RfTest2},
		{"3-�汾����",	"3-Para&Ver",	RfTest3},
		{"1-�²���",	"1-new test",	NewRfTest1},
	};
	
	if(TestModule[T_RF]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1, "��������Ƶģ��", "No RF Module");
		getkey();
		return;
	}

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"�ǽӵ������","RF Test",3);
	
}


// flag  0-���� 1-���� 2-ѭ�� 3-�ɹ��� 4-�ӿڰ�[T60����] 5-��ͨ��ѭ������ 
/*����A���������ƣ�����B��ʱ���Ƶƣ�����M��ʱ�����̵�*/
uchar RfTest(unsigned char flag)
{
    uchar ucRet,ucMode=0;
    uchar BlkNo,SerialNo[11],PwdA[6],tmps[16];
    uint  TotalNum, FailNum;
    uchar CardType[10];
    uchar single=0;
    uchar resultStr[6][20];
    uchar changeFlag=0;
    uchar DataIn[101];
	uchar RfFunErr=0,RfOtherErr=0;

    APDU_SEND ApduSend;
    APDU_RESP ApduResp;
    int ii;
    int PlaceFlag,PlaceNum;
    int detectcnt,detectNum;
    int singleNum;
    PICC_PARA tpicc_para;

	uchar ErrorFlag=0;//0��ʾû�д���1��ʾ�д���
	uchar TestFlag=0;//0 ��ʾ��û���ԣ�1��ʾ���Թ�һ��


    memset(&tpicc_para,0,sizeof(PICC_PARA));
    memset(resultStr, 0x00, sizeof(resultStr));
 
	ScrClrLine(2,7);
	ucRet=PiccOpen();
    if(ucRet) 
    {
		sprintf(gErrnoMsg,"PiccOpen=0x%02x",ucRet);
		ScrDispStr(0,4,1, "��Ƶ��ʧ��", "PiccOpenErr",ucRet);
        getkey();
        return 1;
    }
    else if(flag==3)//��ȡ�汾��Ϣ
    {
		ScrClrLine(2,7);
		ScrDispStr(0,2,1,"��Ƶ���汾��Ϣ","Reading RF Info..."); 
		memset(&tpicc_para,0x00,sizeof(PICC_PARA));    
		PiccSetup('r', &tpicc_para);
		ScrPrint(0,4,0,"RF:[Y] %02x%02x%02x%02x%02x",tpicc_para.a_conduct_val, tpicc_para.m_conduct_val, \
						tpicc_para.b_modulate_val, tpicc_para.card_RxThreshold_val, tpicc_para.f_modulate_val);
		ScrPrint(0,6,0,"Version:%5.5s\nDate:%s",tpicc_para.drv_ver,tpicc_para.drv_date);
		getkey();
		return 0;
    }


    TotalNum=0; 
    FailNum=0;
	if (flag == 0) ScrDispStr(0,2,1,"��Ƶ������","RF Test");
    if (flag == 1) ScrDispStr(0,0,1,"���β���","Single Test");
    if (flag == 2) ScrDispStr(0,0,1,"ѭ������","Cycle Test ");
 
	if(flag<=1)
    {
        singleNum=3;  //����Ѱ��ģʽ
        PlaceNum = 2; //����Ѱ��λ��
        detectNum = 3;//ÿ�ſ���Ѱ������
    }
    else
    {
        singleNum=1;   
        PlaceNum = 1;
        detectNum = 1;
    }
	
    while(1)
    {
		while(2)
        {
                     
			ScrClrLine(2,7);
            memset(CardType, 0x00, sizeof(CardType));
            if(flag==2)
			{	
				if(CancelCheck()) break; 
				ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",++TotalNum);
			}
            if(flag==0 || flag==1)
            {
                for(single = 0; single < singleNum; single++)
                {
                    switch(single)
                    {
						case 2: ucMode='M'; break;
						case 1: ucMode='B'; break;
						case 0: ucMode='A'; break;
                    }
                    for(PlaceFlag = 0; PlaceFlag < PlaceNum; PlaceFlag++)
                    {
                        switch(PlaceFlag)
                        {
							case 0: sprintf(resultStr[single*2+PlaceFlag], "RF.%c.Min=000",ucMode); break;
							case 1: sprintf(resultStr[single*2+PlaceFlag], "RF.%c.Max=000",ucMode); break;
                        }
                    }
                }
            }

START:
            for(PlaceFlag = 0; PlaceFlag < PlaceNum; PlaceFlag++)
            {
                for(single = 0; single < singleNum; single++)
                {
                    DelayMs(1000);//����1s����ʱ,����A��B��M��ֱ�ӵ��л�
                    for(detectcnt = 0; detectcnt < detectNum; detectcnt++)
                    {
                        if( resultStr[single*2+PlaceFlag][detectcnt+9]=='1' )continue;
						PiccRemove('H', 0);
						ScrClrLine(2,7);
                        ScrDispStr(0,2,1,"�ѿ���........","Searching Card..");

                        if(flag <= 1)
                        {
                            switch(single)
                            {
								case 2: ucMode='M';PiccLight(PICC_LED_ALL, 0);PiccLight(PICC_LED_GREEN, 1);break;
								case 1: ucMode='B';PiccLight(PICC_LED_ALL, 0);PiccLight(PICC_LED_YELLOW, 1);break;
								case 0: ucMode='A';PiccLight(PICC_LED_ALL, 0);PiccLight(PICC_LED_BLUE, 1);break;
                            }
            
                            if(PlaceFlag==0)
                                ScrDispStr(0,4,1,"�뽫%c��������С���� 0CM ��","pls wave %c card in 0CM", ucMode);
							else                                  
								ScrDispStr(0,4,1,"�뽫%c������������%dCM","pls wave %c card in %dCM", ucMode,gRfHeight);
                           
                            TimerSet(0, 200);
                            while(TimerCheck(0))
                            {             
								if(CancelCheck())
								{
									sprintf(gErrnoMsg,"User Cancel");
									goto ENDERR;
								}
                                ucRet=PiccDetect(ucMode, CardType, SerialNo, NULL, NULL);
                                if(!ucRet) break;
                            }
                        }
                        else
                        {
                            ScrDispStr(0,4,1,"�뽫RF�������Ӧ��","Pls Wave RF Card ");
							TimerSet(0, 200);
                            while(TimerCheck(0))
                            {
                                if(CancelCheck())
                                {
                                    BeepFail();
                                    FailNum++;
                                    goto LoopResult;
                                }
                                ucRet=PiccDetect('M', CardType, SerialNo, NULL, NULL);
								if(!ucRet) break;
								ucRet=PiccDetect(0, CardType, SerialNo, NULL, NULL);
								if(!ucRet) break;
                            }
                        }
						
                        ScrClrLine(2,7);
                        if(ucRet)
                        {
                            sprintf(gErrnoMsg,"PiccDetect=0x%02x",ucRet);
							ScrDispStr(0,4,1,"�ѿ�����%02x","PiccDetect=%02x",ucRet);
                            BeepFail();
                            FailNum++;
                            if(flag<=1)
                            {
                                goto ENDERR;
                            }
                            kbflush();
							continue;
                            //if(getkey()==KEYCANCEL)break;
                            //else continue;

                        }
                        ScrDispStr(0,4,1,"�� %s ���ɹ�","Detect %s card succeed",CardType);
						DelayMs(100);
                        if((CardType[0] == 'm')||(CardType[0] == 'M'))
                        {
                            ScrClrLine(2,7);
                            ScrDispStr(0,4,1,"������֤...","Autho Password ");
                            BlkNo=37;
                            memcpy(PwdA, "\xff\xff\xff\xff\xff\xff", 6);
                            ucRet = M1Authority('A', BlkNo, PwdA, SerialNo+1);
                            if(ucRet)
                            {
								ScrClrLine(2,7);
								sprintf(gErrnoMsg,"M1Authority=0x%02x",ucRet);
								ScrDispStr(0,4,1, "������֤ʧ��%d","Authority fail%d",ucRet);
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break;  
                            }
                
                            ScrClrLine(2,7);
                            ScrDispStr(0,4,1,"M1 ��д��...","Writing Block...");
                            memcpy(tmps,"\x01\x00\x00\x00", 4);
                            tmps[4]=~tmps[0];
                            tmps[5]=~tmps[1];
                            tmps[6]=~tmps[2];
                            tmps[7]=~tmps[3];
                            memcpy(tmps+8, tmps, 4);
                            tmps[12]=tmps[14]=BlkNo;
                            tmps[13]=tmps[15]=~BlkNo;
                            ucRet = M1WriteBlock(BlkNo,tmps);
                            if(ucRet)
                            {
                            
								ScrClrLine(2,7);
								sprintf(gErrnoMsg,"M1WriteBlock=0x%02x",ucRet);
								ScrDispStr(0,4,1, "д�����:0x%02x", "Write Block Err:0x%02x",ucRet);
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                            }                
                            
                            ScrClrLine(2,7);
                            ScrDispStr(0,4,1,"M1������...","Reading Block...");                        
                            ucRet = M1ReadBlock(BlkNo,tmps);
                            if(ucRet)
                            {                              
								ScrClrLine(2,7);
								sprintf(gErrnoMsg,"M1ReadBlock=0x%02x",ucRet);
								ScrDispStr(0,4,1,"�������:0x%02x","Read Block Err:0x%02x",ucRet);                             
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break;                            
                            }                
                        }
                        else
                        {
                            ScrClrLine(2,7);
                            ScrDispStr(0,4,1, "���ݽ���.....","exchange- data...");
                            memset(&ApduSend,0,sizeof(ApduSend));
                            memset(&ApduResp,0,sizeof(ApduResp));
                            ApduSend.Command[0]=0x00;
                            ApduSend.Command[1]=0xa4;
                            ApduSend.Command[2]=0x04;
                            ApduSend.Command[3]=0x00;
                            ApduSend.Lc=14;
                            strcpy((char*)ApduSend.DataIn, "1PAY.SYS.DDF01");
                            ApduSend.Le=0x00;
                            ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                            if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                            {
                                
								ScrClrLine(2,7);
								sprintf(gErrnoMsg,"MAIN,ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
								ScrDispStr(0,2,1,"ѡ�����ļ�����","Select Main File Err!");//ѡ��3F00���ļ�
								ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                       
                            }
                            
                            //*��Ӷ�д100�ֽ����ݲ���
                            memset(&ApduSend,0,sizeof(ApduSend));
                            memset(&ApduResp,0,sizeof(ApduResp));
                            ApduSend.Command[0] = 0x00;
                            ApduSend.Command[1] = 0xa4;
                            ApduSend.Command[2] = 0x04;
                            ApduSend.Command[3] = 0x00;
                            ApduSend.Lc = 0x09;
                            ApduSend.Le = 256;
                            memcpy(ApduSend.DataIn,(uchar *)"\xA0\x00\x00\x00\x03\x86\x98\x07\x01",9);
                            ApduSend.DataIn[9]=0;
                            ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                            if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                            {                              
								ScrClrLine(2,7);
								sprintf(gErrnoMsg,"APP,ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
								ScrDispStr(0,2,1,"ѡ��Ӧ���ļ�����","Select App File Err!");//ѡ��2F01Ӧ���ļ�
								ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);                                   
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                            }
                            
                            memset(&ApduSend,0,sizeof(ApduSend));
                            memset(&ApduResp,0,sizeof(ApduResp));
                            ApduSend.Command[0] = 0x00;
                            ApduSend.Command[1] = 0xa4;
                            ApduSend.Command[2] = 0x00;
                            ApduSend.Command[3] = 0x00;
                            ApduSend.Lc = 0x02;
                            ApduSend.Le = 0x00;
                            memcpy(ApduSend.DataIn,(uchar *)"\x00\x19",2);
                            ApduSend.DataIn[2]=0;
                            ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                            if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                            {
                                ScrClrLine(2,7);
								sprintf(gErrnoMsg,"RECORD,ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
								ScrDispStr(0,2,1,"ѡ���¼�ļ�����","Select Record File Err!");//ѡ��0019��¼�ļ�
								ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                            }
							
                            ScrClrLine(2,7);
                            ScrDispStr(0,2,1,"д������......","Writing- data...");

                            for(ii=0; ii<100; ii++)
                            {
                                DataIn[ii]=rand()%0xff;
                            }
                    
                            memset(&ApduSend,0,sizeof(ApduSend));
                            memset(&ApduResp,0,sizeof(ApduResp));
                            ApduSend.Command[0] = 0x00;
                            ApduSend.Command[1] = 0xE2;
                            ApduSend.Command[2] = 0x00;
                            ApduSend.Command[3] = 0xC8;
                            ApduSend.Lc = 0x64;
                            ApduSend.Le = 0x00;
                            memcpy(ApduSend.DataIn,DataIn,100);
                    
                            ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                            if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                            {
                                ScrClrLine(2,7);
								sprintf(gErrnoMsg,"WRITE,ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
								ScrDispStr(0,2,1,"д���ݴ���","Write Data Err!");//д����
								ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);                                   
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                            }
                            ScrClrLine(2,7);
                            ScrDispStr(0,4,1,"��������......","Reading- data...");
                            memset(&ApduSend,0,sizeof(ApduSend));
                            memset(&ApduResp,0,sizeof(ApduResp));
                            ApduSend.Command[0] = 0x00;
                            ApduSend.Command[1] = 0xb2;
                            ApduSend.Command[2] = 1;
                            ApduSend.Command[3] = 0xCC;
                            ApduSend.Lc = 0;
                            ApduSend.Le =100;
                            ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                            if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                            {
								ScrClrLine(2,7);
								sprintf(gErrnoMsg,"READ,ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
								ScrDispStr(0,2,1,"�����ݴ���","Read Data Err!");//������
								ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);                                   
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                            }
                            else if(ApduResp.LenOut!=100 || memcmp(ApduResp.DataOut, DataIn,100))
                            {
                               
								ScrClrLine(2,7);
								sprintf(gErrnoMsg,"DATA ERROR");
								ScrDispStr(0,2,1,"��ȡ�������ݴ���","Read Resp Data Error!");
                                BeepFail();
                                FailNum++;
								WaitEsc(0,1);
								break; 
                            }//                               
                            
                        }//
						
                        PiccRemove('H', 0);

                        if( flag==0 || flag==1 )
                        {
							resultStr[single*2+PlaceFlag][detectcnt+9]='1';
							BeepOK();
                            ScrClrLine(2,7);
                            if(PlaceFlag==0)
							{
								ScrDispStr(0,4,1,"����С���봦%s��\n��%d�β��Գɹ�",
									"%s Card Test OK (%d) in Minimum", CardType,detectcnt+1);
							}
							else
							{
								ScrDispStr(0,4,1,"�������봦%s��\n��%d�β��Գɹ�",
									" %s Card Test OK (%d) in Maximum", CardType,detectcnt+1); 
                            }							
                        }
						else
						{
							BeepOK();
							ScrDispStr(0,4,1,"%s�����Գɹ�","%s Card Test OK", CardType,detectcnt+1);
							DelayMs(250);
						}

                    }//for(detect)
                }//for(single)   
            }//for(PlaceFlag)
			if(flag==0 || flag==1)
				goto ENDOK;//�������жϳɹ���ʧ��
			
        }//while(2)

LoopResult:
        if(flag==2)
        {
            ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",TotalNum);
            ShowSuccessRate(TotalNum,FailNum);
			kbflush();
			if(getkey()==KEYCANCEL)break;
        }

    }//while(1)
    
ENDOK:

    if(flag==0 || flag==1)
    {
        for(single = 0; single < singleNum; single++)
        {
            for(PlaceFlag = 0; PlaceFlag < PlaceNum; PlaceFlag++)
            {
                if(memcmp(resultStr[single*2+PlaceFlag]+9, "111", 3))
                {
                    if(TestFlag==1) goto ENDERR;
					else
					{
						memcpy(resultStr[single*2+PlaceFlag]+9, "000", 3);
						ErrorFlag=1;
					}
                }
            }
        }
	}

	if(ErrorFlag==1)
	{
		if(TestFlag==0) {
			ErrorFlag=0;
			TestFlag=1;
			goto START;
		}
		else goto ENDERR;
	}
	

	if(flag == 0)
	{
		for(single = 0; single < singleNum; single++)
		{
			for(PlaceFlag = 0; PlaceFlag < PlaceNum; PlaceFlag++)
			{
				RECORD(1,"%s",resultStr[single*2+PlaceFlag]);
			}
		}
	}
	PiccClose();
	PiccLight(PICC_LED_ALL, 0); 
    return 0;

ENDERR:
    if(flag==0)
    {
        for(single = 0; single < singleNum; single++)
        {
            for(PlaceFlag = 0; PlaceFlag < PlaceNum; PlaceFlag++)
            {
                RECORD(1,"%s",resultStr[single*2+PlaceFlag]);
            }
        }
    }
	PiccClose();
	PiccLight(PICC_LED_ALL, 0); 
    return 1;
}


uchar NewRfTest(uchar flag)
{
    uchar ucRet,ucMode=0;
    uchar BlkNo,SerialNo[11],PwdA[6],tmps[16];
    uchar CardType[10];
    uchar DataIn[101];

    APDU_SEND ApduSend;
    APDU_RESP ApduResp;
    int ii;
    int PlaceFlag,PlaceNum;
    int detectcnt,detectNum;
    int single,singleNum;
	char result[2][3];//2��λ�� 3�ֿ�Ƭ
	char retest,times;
	char height[2][3] = {{0,0,0},{gRfHeight,gRfHeight,gRfHeight}};
	
	ucRet=PiccOpen();
    if(ucRet) 
    {
		sprintf(gErrnoMsg,"PiccOpen=0x%02x",ucRet);
		ScrClrLine(2,7);
		ScrDispStr(0,4,1, "��Ƶ��ʧ��", "PiccOpenErr",ucRet);
        getkey();
        return 1;
    }

    singleNum=3;  //����Ѱ��ģʽ
    PlaceNum = 2; //����Ѱ��λ��
    detectNum = 3;//ÿ�ſ���Ѱ������

	retest = 0;
    memcpy(result,"000000",sizeof(result));

START:
	retest++;
    
	for(single = 0; single < singleNum; single++)
    {
		for(PlaceFlag = 0; PlaceFlag < PlaceNum; PlaceFlag++)
		{
			if(result[PlaceFlag][single] == '1')continue;
			if(single==0)ucMode='A';
			else if(single==1)ucMode='B';
			else ucMode='M';
			ScrClrLine(2,7);
            ScrDispStr(0,4,1,"����%c����%dCM��","test %c at %dCM",ucMode,height[PlaceFlag][single]);
			DelayMs(500);//����500ms����ʱ,����A��B��M��ֱ�ӵ��л�
			times = 0;
            for(detectcnt = 0; detectcnt < detectNum; detectcnt++)
            {

				PiccRemove('H', 0);                        
                TimerSet(0, 200);
                while(TimerCheck(0))
                {             
					if(CancelCheck())
					{
						sprintf(gErrnoMsg,"User Cancel");
						goto ENDERR;
					}
                    ucRet=PiccDetect(ucMode, CardType, SerialNo, NULL, NULL);
                    if(!ucRet) break;
                }
               
                
                if(ucRet)
                {
                    sprintf(gErrnoMsg,"PiccDetect=0x%02x",ucRet);
					ScrClrLine(2,7);
					ScrDispStr(0,4,1,"�ѿ�����%02x","PiccDetect=%02x",ucRet);
                    BeepFail();
                    getkey();
                    goto ENDERR;

                }
                //ScrDispStr(0,4,1,"�� %s ���ɹ�","Detect %s card succeed",CardType);
				DelayMs(100);
                if((CardType[0] == 'm')||(CardType[0] == 'M'))
                {
                    //ScrClrLine(2,7);
                    //ScrDispStr(0,4,1,"������֤...","Autho Password ");
                    BlkNo=37;
                    memcpy(PwdA, "\xff\xff\xff\xff\xff\xff", 6);
                    ucRet = M1Authority('A', BlkNo, PwdA, SerialNo+1);
                    if(ucRet)
                    {
						ScrClrLine(2,7);
						sprintf(gErrnoMsg,"M1Authority=0x%02x",ucRet);
						ScrDispStr(0,4,1, "������֤ʧ��%d","Authority fail%d",ucRet);
                        BeepFail();
						WaitEsc(0,1);
						break;  
                    }
        
                    //ScrClrLine(2,7);
                    //ScrDispStr(0,4,1,"M1 ��д��...","Writing Block...");
                    memcpy(tmps,"\x01\x00\x00\x00", 4);
                    tmps[4]=~tmps[0];
                    tmps[5]=~tmps[1];
                    tmps[6]=~tmps[2];
                    tmps[7]=~tmps[3];
                    memcpy(tmps+8, tmps, 4);
                    tmps[12]=tmps[14]=BlkNo;
                    tmps[13]=tmps[15]=~BlkNo;
                    ucRet = M1WriteBlock(BlkNo,tmps);
                    if(ucRet)
                    {
                    
						ScrClrLine(2,7);
						sprintf(gErrnoMsg,"M1WriteBlock=0x%02x",ucRet);
						ScrDispStr(0,4,1, "д�����:0x%02x", "Write Block Err:0x%02x",ucRet);
                        BeepFail();
						WaitEsc(0,1);
						break; 
                    }                
                    
                    //ScrClrLine(2,7);
                    //ScrDispStr(0,4,1,"M1������...","Reading Block...");                        
                    ucRet = M1ReadBlock(BlkNo,tmps);
                    if(ucRet)
                    {                              
						ScrClrLine(2,7);
						sprintf(gErrnoMsg,"M1ReadBlock=0x%02x",ucRet);
						ScrDispStr(0,4,1,"�������:0x%02x","Read Block Err:0x%02x",ucRet);                             
                        BeepFail();
						WaitEsc(0,1);
						break;                            
                    }                
                }
                else
                {
                    //ScrClrLine(2,7);
                    //ScrDispStr(0,4,1, "���ݽ���.....","exchange- data...");
                    memset(&ApduSend,0,sizeof(ApduSend));
                    memset(&ApduResp,0,sizeof(ApduResp));
                    ApduSend.Command[0]=0x00;
                    ApduSend.Command[1]=0xa4;
                    ApduSend.Command[2]=0x04;
                    ApduSend.Command[3]=0x00;
                    ApduSend.Lc=14;
                    strcpy((char*)ApduSend.DataIn, "1PAY.SYS.DDF01");
                    ApduSend.Le=0x00;
                    ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                    if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                    {
                        
						ScrClrLine(2,7);
						sprintf(gErrnoMsg,"MAIN,ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
						ScrDispStr(0,2,1,"ѡ�����ļ�����","Select Main File Err!");//ѡ��3F00���ļ�
						ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
                        BeepFail();
						WaitEsc(0,1);
						break; 
               
                    }
                    
                    //*��Ӷ�д100�ֽ����ݲ���
                    memset(&ApduSend,0,sizeof(ApduSend));
                    memset(&ApduResp,0,sizeof(ApduResp));
                    ApduSend.Command[0] = 0x00;
                    ApduSend.Command[1] = 0xa4;
                    ApduSend.Command[2] = 0x04;
                    ApduSend.Command[3] = 0x00;
                    ApduSend.Lc = 0x09;
                    ApduSend.Le = 256;
                    memcpy(ApduSend.DataIn,(uchar *)"\xA0\x00\x00\x00\x03\x86\x98\x07\x01",9);
                    ApduSend.DataIn[9]=0;
                    ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                    if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                    {                              
						ScrClrLine(2,7);
						sprintf(gErrnoMsg,"APP,ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
						ScrDispStr(0,2,1,"ѡ��Ӧ���ļ�����","Select App File Err!");//ѡ��2F01Ӧ���ļ�
						ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);                                   
                        BeepFail();
						WaitEsc(0,1);
						break; 
                    }
                    
                    memset(&ApduSend,0,sizeof(ApduSend));
                    memset(&ApduResp,0,sizeof(ApduResp));
                    ApduSend.Command[0] = 0x00;
                    ApduSend.Command[1] = 0xa4;
                    ApduSend.Command[2] = 0x00;
                    ApduSend.Command[3] = 0x00;
                    ApduSend.Lc = 0x02;
                    ApduSend.Le = 0x00;
                    memcpy(ApduSend.DataIn,(uchar *)"\x00\x19",2);
                    ApduSend.DataIn[2]=0;
                    ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                    if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                    {
                        ScrClrLine(2,7);
						sprintf(gErrnoMsg,"RECORD,ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
						ScrDispStr(0,2,1,"ѡ���¼�ļ�����","Select Record File Err!");//ѡ��0019��¼�ļ�
						ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
                        BeepFail();
						WaitEsc(0,1);
						break; 
                    }
					
                    //ScrClrLine(2,7);
                    //ScrDispStr(0,2,1,"д������......","Writing- data...");

                    for(ii=0; ii<100; ii++)
                    {
                        DataIn[ii]=rand()%0xff;
                    }
            
                    memset(&ApduSend,0,sizeof(ApduSend));
                    memset(&ApduResp,0,sizeof(ApduResp));
                    ApduSend.Command[0] = 0x00;
                    ApduSend.Command[1] = 0xE2;
                    ApduSend.Command[2] = 0x00;
                    ApduSend.Command[3] = 0xC8;
                    ApduSend.Lc = 0x64;
                    ApduSend.Le = 0x00;
                    memcpy(ApduSend.DataIn,DataIn,100);
            
                    ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                    if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                    {
                        ScrClrLine(2,7);
						sprintf(gErrnoMsg,"WRITE,ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
						ScrDispStr(0,2,1,"д���ݴ���","Write Data Err!");//д����
						ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);                                   
                        BeepFail();
						WaitEsc(0,1);
						break; 
                    }
                    //ScrClrLine(2,7);
                    //ScrDispStr(0,4,1,"��������......","Reading- data...");
                    memset(&ApduSend,0,sizeof(ApduSend));
                    memset(&ApduResp,0,sizeof(ApduResp));
                    ApduSend.Command[0] = 0x00;
                    ApduSend.Command[1] = 0xb2;
                    ApduSend.Command[2] = 1;
                    ApduSend.Command[3] = 0xCC;
                    ApduSend.Lc = 0;
                    ApduSend.Le =100;
                    ucRet=PiccIsoCommand(0, &ApduSend, &ApduResp);
                    if(ucRet||ApduResp.SWA != 0x90 || ApduResp.SWB != 0x00)
                    {
						ScrClrLine(2,7);
						sprintf(gErrnoMsg,"READ,ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);
						ScrDispStr(0,2,1,"�����ݴ���","Read Data Err!");//������
						ScrPrint(0,4,1,"ucRet=%02x\nSWASWB=%02x%02x",ucRet, ApduResp.SWA, ApduResp.SWB);                                   
                        BeepFail();
						WaitEsc(0,1);
						break; 
                    }
                    else if(ApduResp.LenOut!=100 || memcmp(ApduResp.DataOut, DataIn,100))
                    {
                       
						ScrClrLine(2,7);
						sprintf(gErrnoMsg,"DATA ERROR");
						ScrDispStr(0,2,1,"��ȡ�������ݴ���","Read Resp Data Error!");
                        BeepFail();
						WaitEsc(0,1);
						break; 
                    }//                               
                    
                }//
				
                PiccRemove('H', 0);
				BeepOK();
				times++;
				if(times==3)result[PlaceFlag][single] = '1';
            }//for(detect)
        }//for(single)   
    }//for(PlaceFlag)

	
	if(memcmp(result,"111111",6))//����ʧ��
	{
		goto ENDERR;
		if(retest < 2)goto START;
		else goto ENDERR;
	}

ENDOK:
	PiccClose();
    return 0;

ENDERR:
	PiccClose();
    return 1;
}


//////////////////Felica��////////////////////////
uchar FelicaTest(uchar flag)
{
	  uchar ucRet,DetectFlag;
	  uchar Place=0;
	  uchar rate=0,polarity=0,cmd[512],Resp[512];
	  uint slen=0,rlen=0,rlen1=0;
	  uint TotalNum=0,FailNum=0,OKNum=0;	  

	  //����Ƶ����
	  ScrClrLine(2,7);
	  ucRet=PiccOpen();//������Ƶ��
	  if(ucRet)
	  {
		  ScrPrint(0,4,1,"PiccOpen:%02x",ucRet);
		  getkey();
		  return 1;
	  }

	 //��ʼ��Felica����
	  ucRet = PiccInitFelica(0, 0);//���ó�ʼ��felica��API�ӿڣ�������
	  if(ucRet)
	  {
		 ScrPrint(0,4,1,"PiccInitFelica err:%02x",ucRet);
		 BeepFail();
		 getkey();
		 goto ENDERR;
	  }
     
	  memset(cmd,0,sizeof(cmd));
	  memset(Resp,0,sizeof(Resp));
	  //��װ����
	  cmd[0]=0x06;
	  cmd[1]=0x00;
	  cmd[2]=0xFF;
	  cmd[3]=0xFF;
	  cmd[4]=0x01;
	  cmd[5]=0x00;
	  slen=6;
	  if (flag == 0) ScrDispStr(0,2,1,"FELICA����","FELICA Test");
	  if (flag == 1) ScrDispStr(0,0,1,"���β���","Single Test");
	  if (flag == 2) ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",TotalNum);

	  DetectFlag=0;
	  while(1)
	  {
TEST2CM:
		  ScrClrLine(2,7);
		  if(flag==0 || flag==1){
			  TotalNum=0;
			  OKNum=0;
			  FailNum=0;
			  if(Place==0) ScrDispStr(0,4,1,"�����Ӧ��0CM ˢFeliC��","PLS TAP FELICA AT 0CM");
			  else		   ScrDispStr(0,4,1,"�����Ӧ��2CM ˢFeliC��","PLS TAP FELICA AT 2CM");	
		  }
		  else{
			  ScrDispStr(0,4,1,"���ӦFelica��","PLS TAP FELICA");
		  }	  
		  DetectFlag=0;
		  
		 while(2)
		 {
			 if (CancelCheck())
			 {
				if(flag==0 || flag==1)goto ENDERR;
				else break;
			 }
			 if(flag==2)
			 {
				 if(TotalNum==10000) break;
				 ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",TotalNum);
			 }	 
			 memset(Resp,0,sizeof(Resp));
			 rlen=0;
			 ucRet=PiccCmdExchange(slen,cmd,&rlen,Resp);//����PiccCmdExchange�ӿ���felica������polling����
			 if(ucRet==0 && rlen>0)
			 {
				BeepOK();
				DelayMs(200);
				TotalNum++;
				OKNum++;
				DetectFlag=1;//һ����һ�μ�⵽���Ϳ�ʼһֱ���					
			 }
			 else
			 {
				 if(DetectFlag==1)
				 {
					 BeepFail();
					 TotalNum++;
					 FailNum++;
					 ScrDispStr(0,6,1,"��Ӧʧ��","Tap FeliC Fail");
					 if(flag<=1)goto ENDERR;
				 }

			 }
			 
			 if(flag<=1)
			 {
				 if(TotalNum==3)
				 {
					 if(OKNum==3)
					 {
						 if(Place==0)
						 {
							ScrDispStr(0,6,1,"0CM����Ӧ�ɹ�","At 0CM Test OK");
							Place=2;
							DelayMs(500);
							if(TermInfo[0]==D200)goto ENDOK;
							else goto TEST2CM;
						 }
						 else
						 {
							ScrDispStr(0,6,1,"2CM����Ӧ�ɹ�","At 2CM Test OK");
							DelayMs(500);
							goto ENDOK;
						 }
					 }
					 else
					 {
						goto ENDERR;
					 }
				}

			}
		}
		ScrDispStr(0,0,0x81,"ѭ������  %6d","CycleTest %6d",TotalNum);
		ShowSuccessRate(TotalNum,FailNum);
		kbflush();
		if(getkey()==KEYCANCEL)break;
	}
ENDOK:
	if(flag==0){
		
		RECORD(1,"FELICA=1");
		RECORD(1,"\n");
	}
    PiccClose();
    return 0;
	
ENDERR:
    if(flag==0){
		RECORD(1,"FELICA=0");
		RECORD(1,"\n");
	}
    PiccClose();
    return 1;
}

void FelicaTest1(void)
{
	FelicaTest(1);
}
void FelicaTest2(void)
{
	FelicaTest(2);
}
	 
void FelicaTestAll(void)
{
    
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-����",	"1-Single",FelicaTest1},
		{"2-ѭ��",	"2-Cycle ",FelicaTest2},

	};
	
	if(TestModule[T_FELICA]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"��FeliCģ��","No FeliC!");
		getkey();
		return;
	}

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"FeliC�������","FeliC Test",3);
}
