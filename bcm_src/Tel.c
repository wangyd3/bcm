#include "public.h"


// flag: 0-整机 1-单次测试
uchar TelTest(uchar flag)
{
#ifdef TEL_ON
	TEL_STATUS  tel_status;
	uchar dialbuf[255];// longest telno is 16 
    uchar hf_test;// handfree state
    uchar HandlePicked_tmp;  //handle
	uchar key,dial_read,dial_write;
	uchar HFOffHookFlag,TelOffHookFlag,TelInFlag,TelOutFlag1,TelOutFlag2;//测试是否执行的标志位
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
	 if(flag==1)ScrDispStr(0,0,1,"   单次测试  ","  Single Test   ");
	 
	 OnHook();	 
     TelCheck(&tel_status);
	 HandlePicked_tmp=tel_status.HandlePicked;

     if (HandlePicked_tmp==0) 
		 ScrDispStr(0,2,0x00,"电话挂机 ", "handle is Onhook  ");
	 else  
		 ScrDispStr(0,2,0x00,"请挂机 ", "  Please Onhook ");

	 
	 ScrDispStr(0,4,0x00,"手柄摘机测试","handle test");

	 while (1)
	 {
		   TelCheck(&tel_status);	//循环监听电话状态

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
						  hf_test=1;// offhook 摘机
						  TelOffHook();
						  ScrClrLine(2,7);
						  ScrDispStr(0,2,0x00,"免提电话摘机 ", "freehandle offhook");
						  ScrDispStr(0,4,0x00,"请测试听筒和话筒\n并拨号......", "Please test phone/mike\nAnd dial......");
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
							  ScrDispStr(0,2,0x00,"电话挂机 ", "telephone onhook ");
                              ScrDispStr(0,4,0x00,"请呼入来电 ", "please wait a call ");
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
                   
				 if (tel_status.HandlePicked==1||hf_test==1)//摘机或者手柄拿起
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
             
			if(tel_status.HandlePicked!=HandlePicked_tmp)//手柄电话摘机
			{
			   HandlePicked_tmp=tel_status.HandlePicked;
			   if(tel_status.HandlePicked==1)
			   {
				   hf_test=0;
				   ScrClrLine(2,7);			   
				   ScrDispStr(0,2,0x00,"手柄电话摘机 ", "telephone offhook ");
				   ScrDispStr(0,4,0x00,"请测试听筒和话筒\n并拨号......", "Please test phone/mike\nAnd dial......");
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
					 ScrDispStr(0,2,0x00,"电话已挂机 ", "telephone OnHOOK");
					 ScrDispStr(0,5,0x00,"请进行免提测试","please test freehandle");
					 				 
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
					    ScrDispStr(0,5,0x01, "电话测试ok?", "test ok?");
						dial_read=dial_write=0;					
						memset(dialbuf,0,255);
						key =getkey();
		
						//为配合工厂生产时使用1076分析仪时，来电测试T52不能正常摘机，将测试程序修改为强制摘机后马上挂机
						//liutt  2012.05.17
						TelOffHook();//摘机以强制结束来电铃声
						OnHook();//将电话恢复为正常挂机状态

						if(key==KEYENTER)
						{
 							TelInFlag=1;
							if(HFOffHookFlag && TelOffHookFlag && TelOutFlag1 && TelOutFlag2)
							{
								ScrClrLine(2,7);
								ScrDispStr(0,2,0x01, "测试成功", "Test OK");
								BeepOK();
								DelayMs(500);
								return  0;//当所有测试项全部测试通过时，才能正常退出程序   liutt  2012.05.17
							}
							else
							{
								ScrClrLine(2,7);
								ScrDispStr(0,2,0x01, "电话测试失败", "Test fail");
								BeepFail();
								DelayMs(500);			//否则跳转到重新测试选择界面     liutt  2012.05.17
								return 1;
									/*break;*/
							}
						}
						else
						{
							ScrClrLine(2,7);
							ScrDispStr(0,2,0x01, "电话测试失败", "Test fail!");
							BeepFail();
							DelayMs(500);     	//否则跳转到重新测试选择界面     liutt  2012.05.17
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
					  return 1;//拨号失败
					}
					 dial_read++;  					 
					 ScrClrLine(4,7);
				 	 ScrDispStr(0,4,0x00,"正在拨号......", "Dial......");
					 ScrPrint(0,6,0x00,"%s",dialbuf);

					if(dial_read==12 && flag<=1)//整机和单次测试，普通电话不需要对比数据
					{
						if(!memcmp(dialbuf,"123456789*0#",12))  //判断拨号顺序是否是123456789*0#
						{
							if(tel_status.HandlePicked==1)//手柄电话拨号
							{
								TelOutFlag1 = 1;
							}
							else if(hf_test == 1)//免提电话拨号
							{
								TelOutFlag2 = 1;
							}
						}
						else
						{
							ScrClrLine(2,7);
							ScrDispStr(0,2,0x01, "电话测试失败", "Test fail");
							BeepFail();
							DelayMs(500);
							OnHook();
							return 1;//拨号顺序错，测试失败
						}
					}
               }
		   }
	}  //while

#endif
	return 0;
}


//普通电话
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
	uchar HFOffHookFlag,TelOffHookFlag,TelInFlag,TelOutFlag1,TelOutFlag2;//测试是否执行的标志位
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
	 ScrDispStr(0,0,0x81," 电话—欢迎使用 "," Phone Welcome ");
	 memset(dialbuf,0,255);
	 OnHook();

	 memset(&tel_status,0,sizeof(tel_status));
     TelCheck(&tel_status);
	 HandlePicked_tmp=tel_status.HandlePicked;

	 while (1)
	 {
		   TelCheck(&tel_status);	//循环监听电话状态

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
						  hf_test=1;// offhook 摘机
						  ScrClrLine(2,7);
						  ScrDispStr(0,2,0x01,"免提接听/拨号中... ", "Dail Or Answer... ");
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
                   
				 if (tel_status.HandlePicked==1||hf_test==1)//摘机或者手柄拿起
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
             
			if(tel_status.HandlePicked!=HandlePicked_tmp)//手柄电话摘机
			{
			   HandlePicked_tmp=tel_status.HandlePicked;
			   if(tel_status.HandlePicked==1)
			   {
				   hf_test=0;
				   ScrClrLine(2,7);	
				   ScrDispStr(0,2,0x01,"听筒接听/拨号中... ", "Dail Or Answer... ");
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
					  return ;//拨号失败
					}
					 dial_read++;  					 
					 ScrClrLine(4,7);
				 	 ScrDispStr(0,4,0x00,"正在拨号......", "Dial......");
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
		{"1-单次测试",	"1-Single Test",TelePhone},
		{"2-普通电话",	"2-Telephone  ",TelTest1 },

	};

	if(TestModule[T_TEL]=='N')
	{		
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"无电话模块", "NO PHONE Module");
		getkey();
		return ;
	}
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"电话单项测试","Phone Test",3);
}
