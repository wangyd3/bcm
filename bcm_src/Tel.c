#include "public.h"


// flag: 0-���� 1-���β���
uchar TelTest(uchar flag)
{
#ifdef TEL_ON
	TEL_STATUS  tel_status;
	uchar dialbuf[255];// longest telno is 16 
    uchar hf_test;// handfree state
    uchar HandlePicked_tmp;  //handle
	uchar key,dial_read,dial_write;
	uchar HFOffHookFlag,TelOffHookFlag,TelInFlag,TelOutFlag1,TelOutFlag2;//�����Ƿ�ִ�еı�־λ
	int ucRet;
 
    dial_read=0;
	dial_write=0;
	hf_test=0;
	HFOffHookFlag=0;
	TelOffHookFlag=0;
	TelInFlag=0;
	TelOutFlag1=0;
	TelOutFlag2=0;
	memset(dialbuf,0,255);
	memset(&tel_status,0,sizeof(tel_status));

	
	 ScrClrLine(2,7);
	 kbflush();
	 if(flag==1)ScrDispStr(0,0,1,"   ���β���  ","  Single Test   ");
	 
	 OnHook();	 
     TelCheck(&tel_status);
	 HandlePicked_tmp=tel_status.HandlePicked;

     if (HandlePicked_tmp==0) 
		 ScrDispStr(0,2,0x00,"�绰�һ� ", "handle is Onhook  ");
	 else  
		 ScrDispStr(0,2,0x00,"��һ� ", "  Please Onhook ");

	 
	 ScrDispStr(0,4,0x00,"�ֱ�ժ������","handle test");

	 while (1)
	 {
		   TelCheck(&tel_status);	//ѭ�������绰״̬

			if (!kbhit())
			{
				key=getkey();
			
				if (key==KEYCANCEL)
				{					
					OnHook();
					return 1;
					/*break;*/
				}
				switch(key)
				{
				   case KEYHF://handfree key 
					  if (hf_test==0)//handfree is off
					  {
						  hf_test=1;// offhook ժ��
						  TelOffHook();
						  ScrClrLine(2,7);
						  ScrDispStr(0,2,0x00,"����绰ժ�� ", "freehandle offhook");
						  ScrDispStr(0,4,0x00,"�������Ͳ�ͻ�Ͳ\n������......", "Please test phone/mike\nAnd dial......");
						  dial_read=dial_write=0;
						  memset(dialbuf,0,255);
 						  HFOffHookFlag=1;
					  }
					  else            //handfree is on
					  {
					      if (tel_status.HandlePicked==0)
					      {
							  ScrClrLine(2,7);
							  hf_test=0;
							  ScrDispStr(0,2,0x00,"�绰�һ� ", "telephone onhook ");
                              ScrDispStr(0,4,0x00,"��������� ", "please wait a call ");
							  dial_read=dial_write=0;
							  memset(dialbuf,0,255);	  
					          OnHook();
 							  DelayMs(200);
						  }
					  }
					  break;
                   
				   case KEY1:
				   case KEY2:
				   case KEY3:
				   case KEY4:
				   case KEY5:
                   case KEY6:
				   case KEY7:
				   case KEY8:
				   case KEY9:
				   case KEY0:
				   case KEYSTAR:			
				   case KEYNUM:
                   
				 if (tel_status.HandlePicked==1||hf_test==1)//ժ�������ֱ�����
				 {
						   if (dial_write<255)
						   {
							   dialbuf[dial_write]=key;
							   dial_write++;
						   }
				  }
				  break;
				 case KEYCANCEL: OnHook(); 	DelayMs(200); return 1;
	 			 default: return 0xff;			
               
				}//switch
			}//if
             
			if(tel_status.HandlePicked!=HandlePicked_tmp)//�ֱ��绰ժ��
			{
			   HandlePicked_tmp=tel_status.HandlePicked;
			   if(tel_status.HandlePicked==1)
			   {
				   hf_test=0;
				   ScrClrLine(2,7);			   
				   ScrDispStr(0,2,0x00,"�ֱ��绰ժ�� ", "telephone offhook ");
				   ScrDispStr(0,4,0x00,"�������Ͳ�ͻ�Ͳ\n������......", "Please test phone/mike\nAnd dial......");
			       TelOffHook();//    offhook
 				   TelOffHookFlag=1;
				   dial_read=dial_write=0;
				   memset(dialbuf,0,255);
			   }
			   else
			   {
			     if (hf_test==0)//handfree is off
			     {
					 dial_read=dial_write=0;					
					 memset(dialbuf,0,255);
					 ScrClrLine(2,7);		
					 ScrDispStr(0,2,0x00,"�绰�ѹһ� ", "telephone OnHOOK");
					 ScrDispStr(0,5,0x00,"������������","please test freehandle");
					 				 
					 OnHook();
 					 DelayMs(200);
			     }
			   }
			}

			if (tel_status.HandlePicked==0&&hf_test==0)//incoming 
			{
				if (tel_status.CidFound==1)
				{
					ScrClrLine(2,7);
					if (tel_status.CidTimeStr[0]!=0) ScrPrint(0,2,0x00,"call time %s",tel_status.CidTimeStr);
					if (tel_status.CidTelNo[0]!=0)ScrPrint(0,3,0x00,"call num %s",tel_status.CidTelNo);
					if(tel_status.CidUserName[0]!=0)ScrPrint(0,4,0x00,"CidUserName %s",tel_status.CidUserName);
				   	
					//if(flag==0)
				   {
					    ScrDispStr(0,5,0x01, "�绰����ok?", "test ok?");
						dial_read=dial_write=0;					
						memset(dialbuf,0,255);
						key =getkey();
		
						//Ϊ��Ϲ�������ʱʹ��1076������ʱ���������T52��������ժ���������Գ����޸�Ϊǿ��ժ�������Ϲһ�
						//liutt  2012.05.17
						TelOffHook();//ժ����ǿ�ƽ�����������
						OnHook();//���绰�ָ�Ϊ�����һ�״̬

						if(key==KEYENTER)
						{
 							TelInFlag=1;
							if(HFOffHookFlag && TelOffHookFlag && TelOutFlag1 && TelOutFlag2)
							{
								ScrClrLine(2,7);
								ScrDispStr(0,2,0x01, "���Գɹ�", "Test OK");
								BeepOK();
								DelayMs(500);
								return  0;//�����в�����ȫ������ͨ��ʱ�����������˳�����   liutt  2012.05.17
							}
							else
							{
								ScrClrLine(2,7);
								ScrDispStr(0,2,0x01, "�绰����ʧ��", "Test fail");
								BeepFail();
								DelayMs(500);			//������ת�����²���ѡ�����     liutt  2012.05.17
								return 1;
									/*break;*/
							}
						}
						else
						{
							ScrClrLine(2,7);
							ScrDispStr(0,2,0x01, "�绰����ʧ��", "Test fail!");
							BeepFail();
							DelayMs(500);     	//������ת�����²���ѡ�����     liutt  2012.05.17
							return 1;
							/*break;*/
						}
				   }
				}
			}
		   else
		   {
                if (dial_read!=	dial_write)
                {
					ucRet=TelDial(dialbuf+dial_read);
					if (ucRet!=0)
					{
					  ScrPrint(0,5,0x41,"error num dial ");
					  OnHook();
					  return 1;//����ʧ��
					}
					 dial_read++;  					 
					 ScrClrLine(4,7);
				 	 ScrDispStr(0,4,0x00,"���ڲ���......", "Dial......");
					 ScrPrint(0,6,0x00,"%s",dialbuf);

					if(dial_read==12 && flag<=1)//�����͵��β��ԣ���ͨ�绰����Ҫ�Ա�����
					{
						if(!memcmp(dialbuf,"123456789*0#",12))  //�жϲ���˳���Ƿ���123456789*0#
						{
							if(tel_status.HandlePicked==1)//�ֱ��绰����
							{
								TelOutFlag1 = 1;
							}
							else if(hf_test == 1)//����绰����
							{
								TelOutFlag2 = 1;
							}
						}
						else
						{
							ScrClrLine(2,7);
							ScrDispStr(0,2,0x01, "�绰����ʧ��", "Test fail");
							BeepFail();
							DelayMs(500);
							OnHook();
							return 1;//����˳�������ʧ��
						}
					}
               }
		   }
	}  //while

#endif
	return 0;
}


//��ͨ�绰
void TelePhone(void)
{

#ifdef TEL_ON
    TEL_STATUS  tel_status;
	uchar dialbuf[255];// longest telno is 16 
    uchar hf_test;// handfree state
    uchar HandlePicked_tmp;  //handle
	uchar key;
	uchar dial_read;
	uchar dial_write;
	uchar HFOffHookFlag,TelOffHookFlag,TelInFlag,TelOutFlag1,TelOutFlag2;//�����Ƿ�ִ�еı�־λ
	int ucRet;
	 
    dial_read=0;
	dial_write=0;
	hf_test=0;
	HFOffHookFlag=0;
	TelOffHookFlag=0;
	TelInFlag=0;
	TelOutFlag1=0;
	TelOutFlag2=0;

	 ScrCls();
	 kbflush();
	 ScrDispStr(0,0,0x81," �绰����ӭʹ�� "," Phone Welcome ");
	 memset(dialbuf,0,255);
	 OnHook();

	 memset(&tel_status,0,sizeof(tel_status));
     TelCheck(&tel_status);
	 HandlePicked_tmp=tel_status.HandlePicked;

	 while (1)
	 {
		   TelCheck(&tel_status);	//ѭ�������绰״̬

			if (!kbhit())
			{
				key=getkey();
			
				if (key==KEYCANCEL)
				{					
					OnHook();
					return ;
					/*break;*/
				}
				switch(key)
				{
				   case KEYHF://handfree key 
					  if (hf_test==0)//handfree is off
					  {
						  hf_test=1;// offhook ժ��
						  ScrClrLine(2,7);
						  ScrDispStr(0,2,0x01,"�������/������... ", "Dail Or Answer... ");
						  TelOffHook();
						  dial_read=dial_write=0;
						  memset(dialbuf,0,255);
 						  HFOffHookFlag=1;
					  }
					  else            //handfree is on
					  {
					      if (tel_status.HandlePicked==0)
					      {
							  ScrClrLine(2,7);
							  hf_test=0;
							  dial_read=dial_write=0;
							  memset(dialbuf,0,255);	  
					          OnHook();
 							  DelayMs(200);
						  }
					  }
					  break;
                   
				   case KEY1:
				   case KEY2:
				   case KEY3:
				   case KEY4:
				   case KEY5:
                   case KEY6:
				   case KEY7:
				   case KEY8:
				   case KEY9:
				   case KEY0:
				   case KEYSTAR:			
				   case KEYNUM:
                   
				 if (tel_status.HandlePicked==1||hf_test==1)//ժ�������ֱ�����
				 {
						   if (dial_write<255)
						   {
							   dialbuf[dial_write]=key;
							   dial_write++;
						   }
				  }
				  break;
				 case KEYCANCEL: OnHook(); 	DelayMs(200); return ;
	 			 default: return ;			
               
				}//switch
			}//if
             
			if(tel_status.HandlePicked!=HandlePicked_tmp)//�ֱ��绰ժ��
			{
			   HandlePicked_tmp=tel_status.HandlePicked;
			   if(tel_status.HandlePicked==1)
			   {
				   hf_test=0;
				   ScrClrLine(2,7);	
				   ScrDispStr(0,2,0x01,"��Ͳ����/������... ", "Dail Or Answer... ");
			       TelOffHook();//    offhook
 				   TelOffHookFlag=1;
				   dial_read=dial_write=0;
				   memset(dialbuf,0,255);
			   }
			   else
			   {
			     if (hf_test==0)//handfree is off
			     {
					 dial_read=dial_write=0;					
					 memset(dialbuf,0,255);
					 ScrClrLine(2,7);		
					 OnHook();
 					 DelayMs(200);
			     }
			   }
			}

			if (tel_status.HandlePicked==0&&hf_test==0)//incoming 
			{
				if (tel_status.CidFound==1)
				{
					ScrClrLine(2,7);
					if (tel_status.CidTimeStr[0]!=0) ScrPrint(0,2,0x00,"call time %s",tel_status.CidTimeStr);
					if (tel_status.CidTelNo[0]!=0)ScrPrint(0,3,0x00,"call num %s",tel_status.CidTelNo);
					if(tel_status.CidUserName[0]!=0)ScrPrint(0,4,0x00,"CidUserName %s",tel_status.CidUserName);
				}
			}
		   else
		   {
                if (dial_read!=	dial_write)
                {
					ucRet=TelDial(dialbuf+dial_read);
					if (ucRet!=0)
					{
					  ScrPrint(0,5,0x41,"error num dial ");
					  OnHook();
					  return ;//����ʧ��
					}
					 dial_read++;  					 
					 ScrClrLine(4,7);
				 	 ScrDispStr(0,4,0x00,"���ڲ���......", "Dial......");
					 ScrPrint(0,6,0x00,"%s",dialbuf);
               }
		   }
	}  //while

#endif

}


void TelTest1(void)
{
	TelTest(0);
}

void TelTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-���β���",	"1-Single Test",TelePhone},
		{"2-��ͨ�绰",	"2-Telephone  ",TelTest1 },

	};

	if(TestModule[T_TEL]=='N')
	{		
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"�޵绰ģ��", "NO PHONE Module");
		getkey();
		return ;
	}
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"�绰�������","Phone Test",3);
}
