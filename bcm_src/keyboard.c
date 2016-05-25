#include "public.h"

typedef struct
{
	uchar KeyValue;
	uchar Flag_OK;
	uchar KeyDIS[10];
}KEY_ORDER;


uchar KbOneTest(uchar flag)
{

	uchar i,count_getkey,KeyNum,ucRet,buff_kbhit[50];
	uchar *pStrChinese, *pStrEnglish;
	
	switch(TermInfo[0])
	{
	case SP30:
	case S200:
		KeyNum = 16;
		break;
	case S80:
		KeyNum = 22;
		break;
	case S78:
		KeyNum = 19;
		break;
	case S60:
		KeyNum = 22;
		break;
	case S90:
		KeyNum = 22;
		break;
	
	case S800:
		KeyNum = 19;
		break;
		
	case S500:
	case S58:
		KeyNum = 18;
		break;

	case S300:
	case S900:
    case T60:
        KeyNum = 15;  
        break;
    case T52:
        KeyNum = 31;  
        break;
	case MT30:
		KeyNum = 13;
		break;
	case T620:
		KeyNum = 20;
		break;
	case T610:
		KeyNum = 4;
		break;
	case D200:
		KeyNum = 15;
		break;
	case S910:
	case D210:
		KeyNum = 18;
		break;
		

   default:
        break;
	}

	ScrClrLine(2,7); 
	kbflush();

	pStrEnglish = "Pls Press Key...";
	pStrChinese = "按键测试,请按键 ";
	ScrDispStr(0,2,1,pStrChinese, pStrEnglish);
	memset(buff_kbhit,0x00,sizeof(buff_kbhit));

	while(1)
	{
		ucRet=getkey();
		ScrClrLine(4,7);
		if(ucRet==KEYCANCEL)
		{
			count_getkey=1;//取消键算一个
			for(i=0;i<sizeof(buff_kbhit);i++)
			{
				if(buff_kbhit[i]) 
					count_getkey++;
			}
						
			if(count_getkey==KeyNum)
			{
				ScrDispStr(0,4,1,"%d个按键全测通过","%d keys all pass",KeyNum);
				BeepOK();
				WaitEsc(0,1);
				return 0;
			}
			else
			{
				
				ScrDispStr(0,4,1,"键未测完,仅%d键","Only %dKeys Pass",count_getkey);
				BeepFail();
				WaitEsc(0,2);
				return 1;
			}		
		}
		
		switch(ucRet)
		{
			case 0:		ScrDispStr(0, 6, 0x01, "按键：NULL"," Key: NULL");	buff_kbhit[38]=1;	break;
			case KEY0:	ScrDispStr(0, 6, 0x01, "按键：0", " Key: 0");	buff_kbhit[0]=1;	break;	
			case KEY1:	ScrDispStr(0, 6, 0x01, "按键：1", " Key: 1");	buff_kbhit[1]=1;	break;	
			case KEY2:	ScrDispStr(0, 6, 0x01, "按键：2", " Key: 2");	buff_kbhit[2]=1;	break;	
			case KEY3:	ScrDispStr(0, 6, 0x01, "按键：3", " Key: 3");	buff_kbhit[3]=1;	break;	
			case KEY4:	ScrDispStr(0, 6, 0x01, "按键：4", " Key: 4");	buff_kbhit[4]=1;	break;	
			case KEY5:	ScrDispStr(0, 6, 0x01, "按键：5", " Key: 5");	buff_kbhit[5]=1;	break;	
			case KEY6:	ScrDispStr(0, 6, 0x01, "按键：6", " Key: 6");	buff_kbhit[6]=1;	break;	
			case KEY7:	ScrDispStr(0, 6, 0x01, "按键：7", " Key: 7");	buff_kbhit[7]=1;	break;	
			case KEY8:	ScrDispStr(0, 6, 0x01, "按键：8", " Key: 8");	buff_kbhit[8]=1;	break;	
			case KEY9:	ScrDispStr(0, 6, 0x01, "按键：9", " Key: 9");	buff_kbhit[9]=1;	break;	
			case KEYUP:		ScrDispStr(0, 6, 0x01, "按键：上箭头", " Key: UP");		buff_kbhit[10]=1;	break;	
			case KEYDOWN:	ScrDispStr(0, 6, 0x01, "按键：下箭头", " Key: DOWN");	buff_kbhit[11]=1;	break;		
			case KEYCLEAR:	ScrDispStr(0, 6, 0x01, "按键：清除",   " Key: CLEAR");	buff_kbhit[12]=1;	break;	
			case KEYENTER:	ScrDispStr(0, 6, 0x01, "按键：确认", " Key: ENTER");	buff_kbhit[13]=1;	break;	
			case KEYCANCEL:	ScrDispStr(0, 6, 0x01, "按键：取消", " Key: CANCEL");	buff_kbhit[14]=1;	break;
			//T52没有的按键
			case KEYMENU:	ScrDispStr(0, 6, 0x01, "按键：菜单", " Key: MENU");	buff_kbhit[15]=1;	break;
			case KEYALPHA:	ScrDispStr(0, 6, 0x01, "按键：字母", " Key: ALPHA");	buff_kbhit[16]=1;	break;
			case KEYFN:		ScrDispStr(0, 6, 0x01, "按键：功能", " Key: FUNCT");	buff_kbhit[17]=1;	break;
			//T52独有按键
			case KEYNUM:  	ScrDispStr(0, 6, 0x01, "按键：#", " Key: #");               	buff_kbhit[18]=1;	break;
			case KEYFLASH:  ScrDispStr(0, 6, 0x01, "按键：闪断/返回", " Key: FLASH/BACK");	buff_kbhit[19]=1;	break;
			case KEYRD:     ScrDispStr(0, 6, 0x01, "按键：重拨/暂停", " Key: REDIAL/STOP");	buff_kbhit[20]=1;	break;
			case KEYHF:     ScrDispStr(0, 6, 0x01, "按键：免提", " Key: FREEHANd");	        buff_kbhit[21]=1;	break;
			case KEYSET:    ScrDispStr(0, 6, 0x01, "按键：设置", " Key: SET");	            buff_kbhit[22]=1;	break;
			case KEYLEFT:   ScrDispStr(0, 6, 0x01, "按键：左键", " Key: LEFT");	            buff_kbhit[23]=1;	break;
			case KEYPBOOK:  ScrDispStr(0, 6, 0x01, "按键：电话本", " Key: PHONE");	        buff_kbhit[24]=1;	break;
			case KEYPAY:    ScrDispStr(0, 6, 0x01, "按键：支付", " Key: PAY");	            buff_kbhit[25]=1;	break;
			case KEYPHONE:  ScrDispStr(0, 6, 0x01, "按键：IP/输入法", " Key: IP/INPUT");	buff_kbhit[26]=1;	break;
			case KEYBACK:   ScrDispStr(0, 6, 0x01, "按键：回拨", " Key: BACKCALL");	        buff_kbhit[27]=1;	break;
			case KEYRIGHT:  ScrDispStr(0, 6, 0x01, "按键：右键", " Key: RIGHT");	        buff_kbhit[28]=1;	break;
			case KEYMSG:    ScrDispStr(0, 6, 0x01, "按键：账单", " Key: PAYMENT");	        buff_kbhit[29]=1;	break;
			case KEYDIALED: ScrDispStr(0, 6, 0x01, "按键：去电", " Key: DIALED");	        buff_kbhit[30]=1;	break;
			case KEYSTAR:  	ScrDispStr(0, 6, 0x01, "按键：*", " Key: *");	                buff_kbhit[31]=1;	break;
				
	
			case KEYF1:		ScrDispStr(0, 6, 0x01, "按键：F1", " Key: F1");	buff_kbhit[32]=1;	break;
			case KEYF2:		ScrDispStr(0, 6, 0x01, "按键：F2", " Key: F2");	buff_kbhit[33]=1;	break;
	
			case KEYATM1:	ScrDispStr(0, 6, 0x01, "按键：ATM1", " Key: ATM1");		buff_kbhit[34]=1;	break;	
			case KEYATM2:	ScrDispStr(0, 6, 0x01, "按键：ATM2", " Key: ATM2");		buff_kbhit[35]=1;	break;	
			case KEYATM3:	ScrDispStr(0, 6, 0x01, "按键：ATM3", " Key: ATM3");		buff_kbhit[36]=1;	break;	
			case KEYATM4:	ScrDispStr(0, 6, 0x01, "按键：ATM4", " Key: ATM4");		buff_kbhit[37]=1;	break;	

			case KEYADD:	ScrDispStr(0,6,0x01, "按键：加", " Key: +");		buff_kbhit[38]=1;	break;
			case KEYSUB:	ScrDispStr(0,6,0x01, "按键：减", " Key: -");		buff_kbhit[39]=1;	break;
			case KEYMUL:	ScrDispStr(0,6,0x01, "按键：乘", " Key: *");		buff_kbhit[40]=1;	break;
			case KEYDOT:	ScrDispStr(0,6,0x01, "按键：点", " Key: .");		buff_kbhit[41]=1;	break;
			case KEYF3:		ScrDispStr(0,6,0x01, "按键：F3", " Key: F3");	buff_kbhit[42]=1;	break;
			case KEYF4:		ScrDispStr(0,6,0x01, "按键：F4", " Key: F4");	buff_kbhit[43]=1;	break;
			
		}
	}

}

// 抖动测试
uchar KbTwoTest(uchar flag)
{
	uchar col,ucRet,old_ucRet;
	char kstr[10];

	ScrClrLine(2,7); 
	kbflush();
	ScrDispStr(0,2,1,"  抖动测试 ", "Dithering Test");
	ScrDispStr(0,4,1,"[取消]-退出", "[CANCEL]-Quit ");
	ScrDispStr(0,6,1,"[清除]-清屏", "[CLEAR]-ScrCls");
	old_ucRet=0; col=0;

	while(1)
	{
		ucRet=getkey();
		if(!old_ucRet)
		{
			ScrCls();
			ScrFontSet(0);
			ScrGotoxyEx(0,0);
		}
		if(ucRet==old_ucRet) BeepFail();
		old_ucRet=ucRet;
		if(ucRet==KEYCANCEL) break;
		if(ucRet==KEYCLEAR)
		{
			ScrCls(); col=0; continue;
		}
		switch(ucRet)
		{
		case KEY0:		strcpy(kstr," 0");			break;
		case KEY1:		strcpy(kstr," 1");			break;
		case KEY2:		strcpy(kstr," 2");			break;
		case KEY3:		strcpy(kstr," 3");			break;
		case KEY4:		strcpy(kstr," 4");			break;
		case KEY5:		strcpy(kstr," 5");			break;
		case KEY6:		strcpy(kstr," 6");			break;
		case KEY7:		strcpy(kstr," 7");			break;
		case KEY8:		strcpy(kstr," 8");			break;
		case KEY9:		strcpy(kstr," 9");			break;
		case KEYCLEAR:	strcpy(kstr," CLEAR");		break;
		case KEYF1:		strcpy(kstr," F1");			break;
		case KEYF2:		strcpy(kstr," F2");			break;
		case KEYF3:		strcpy(kstr," F3");			break;
		case KEYUP:		strcpy(kstr," UP");			break;
		case KEYDOWN:	strcpy(kstr," DOWN");		break;
		case KEYFN:		strcpy(kstr," FUNCT");		break;
		case KEYENTER:	strcpy(kstr," ENTER");		break;		
		case KEYCANCEL:	strcpy(kstr," CANCEL");		break;
		case KEYMENU:	strcpy(kstr," MENU");		break;
		case KEYALPHA:	strcpy(kstr," ALPHA");		break;
		case KEYATM1:	strcpy(kstr," ATM1");		break;
		case KEYATM2:	strcpy(kstr," ATM2");		break;
		case KEYATM3:	strcpy(kstr," ATM3");		break;
		case KEYATM4:	strcpy(kstr," ATM4");		break;
        case KEYSTAR:  	strcpy(kstr," *");			break;
        case KEYNUM:  	strcpy(kstr," #");			break;
        case KEYFLASH:  strcpy(kstr," Flash");		break; 
		case KEYRD:     strcpy(kstr," REDIAL");		break;
		case KEYHF:     strcpy(kstr, " FH");	   	break;
		case KEYSET:    strcpy(kstr, " SET");		break;
		case KEYLEFT:   strcpy(kstr, " LEFT");		break;
		case KEYPBOOK:  strcpy(kstr, " PHONE");		break;
		case KEYPAY:    strcpy(kstr, " PAY");		break;
		case KEYPHONE:  strcpy(kstr, " IP");	    break;
		case KEYBACK:   strcpy(kstr, " BACKCALL");	break;
        case KEYRIGHT:  strcpy(kstr, " RIGHT");		break;
		case KEYMSG:    strcpy(kstr, " RPAYMENT");	break;
		case KEYDIALED: strcpy(kstr, " DIALED");	break;
		case KEYADD:		strcpy(kstr," +");		break;
		case KEYSUB:		strcpy(kstr," -");		break;
		case KEYMUL:		strcpy(kstr," *");		break;
		case KEYDOT:		strcpy(kstr," .");		break;
		default:		strcpy(kstr," X");			break;

		}
		col+=strlen(kstr);
		if(col>168)
		{
			ScrCls();
			col=0;
		}
		Lcdprintf("%s",kstr);
	}
	return 0;

}

// flag=0 整机测试，有序按键测试
uchar KbTest(uchar flag)
{
	int j,count_getkey;
	unsigned char ucRet,buff_kbhit[30];
	unsigned char *pStrChinese, *pStrEnglish;
	unsigned char KeyNum=22;
	unsigned char flagErr=0;
	KEY_ORDER *KEYORDER;

	//S80整机测试时的按键顺序:F1 F2 F3 F4 上下 功能 1 2 3 菜单 4 5 6 字母 7 8 9 确认 清除 0 取消
	KEY_ORDER KEYORDER_S80[22]={{KEYATM1,0,"ATM1"},{KEYATM2,0,"ATM2"},{KEYATM3,0,"ATM3"},{KEYATM4,0,"ATM4"},{KEYUP,0,"UP"},{KEYDOWN,0,"DOWN"},{KEYFN,0,"FUN"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEYMENU,0,"MENU"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEYALPHA,0,"ALPHA"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},{KEY0,0,"0"},{KEYCANCEL,0,"CANCEL"}}; 
	
	//S78
	KEY_ORDER KEYORDER_S78[19]={{KEYUP,0,"UP"},{KEYDOWN,0,"DOWN"},{KEYF1,0,"F1"},{KEYF2,0,"F2"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEYMENU,0,"MENU"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEYALPHA,0,"ALPHA"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},{KEY0,0,"0"},{KEYCANCEL,0,"CANCEL"}};
	
	//S90整机测试时的按键测试:F1 F2 F3 F4 上 功能 字母 下 菜单 1 2 3 4 5 6 7 8 9 确认 0 消除 取消
	KEY_ORDER KEYORDER_S90[22]={{KEYATM1,0,"ATM1"},{KEYATM2,0,"ATM2"},{KEYATM3,0,"ATM3"},{KEYATM4,0,"ATM4"},{KEYUP,0,"UP"},{KEYFN,0,"FUN"},{KEYALPHA,0,"ALPHA"},{KEYDOWN,0,"DOWN"},{KEYMENU,0,"MENU"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEY0,0,"0"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}};  
	KEY_ORDER KEYORDER_S90_[22]={{KEYATM1,0,"ATM1"},{KEYATM2,0,"ATM2"},{KEYATM3,0,"ATM3"},{KEYATM4,0,"ATM4"},{KEYUP,0,"UP"},{KEYFN,0,"DOWN"},{KEYALPHA,0,"ALPHA"},{KEYDOWN,0,"FUN"},{KEYMENU,0,"MENU"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEY0,0,"0"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}};  
	
	//S60整机测试时的按键测试:F1 F2 F3 F4 上 功能 字母 下 菜单 1 2 3 4 5 6 7 8 9 确认 0 消除 取消
	KEY_ORDER KEYORDER_S60[22]={{KEYATM1,0,"ATM1"},{KEYATM2,0,"ATM2"},{KEYATM3,0,"ATM3"},{KEYATM4,0,"ATM4"},{KEYUP,0,"UP"},{KEYFN,0,"FUN"},{KEYALPHA,0,"ALPHA"},{KEYDOWN,0,"DOWN"},{KEYMENU,0,"MENU"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEY0,0,"0"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}};  
	
	//S58
	KEY_ORDER KEYORDER_S58[18]={{KEYUP,0,"UP"},{KEYDOWN,0,"DOWN"},{KEYFN,0,"FUNCT"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEYMENU,0,"MENU"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEYALPHA,0,"ALPHA"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},{KEY0,0,"0"},{KEYCANCEL,0,"CANCEL"}};
	
	//SP30整机测试时的按键测试: 功能 上 下 1 2 3 4 5 6 7 8 9 确认 0 消除 取消   
	KEY_ORDER KEYORDER_SP30[16]={{KEYFN,0,"FUN"},{KEYUP,0,"UP"},{KEYDOWN,0,"DOWN"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEY0,0,"0"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}};        
	
	//T60整机测试时的按键测试:1 2 3 4 5 6 功能 7 8 9 字母 确认 清除 0 取消   
	KEY_ORDER KEYORDER_T60[15]={{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEYFN,0,"FUN"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYALPHA,0,"ALPHA"},{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},{KEY0,0,"0"},{KEYCANCEL,0,"CANCEL"}};
    
	//T52整机测试时的按键顺序: 上下 功能 1 2 3 菜单 4 5 6 字母 7 8 9 确认 清除 0 取消
    KEY_ORDER KEYORDER_T52[31]={{KEYPBOOK,0,"PHONE"},{KEYDIALED,0,"DIALED"},{KEYBACK,0,"BACKCALL"},{KEYFLASH,0,"FLASH"},{KEYRD,0,"REDIAL"},{KEYMENU,0,"MENU"},{KEYUP,0,"UP"},{KEYDOWN,0,"DOWN"},{KEYLEFT,0,"LEFT"},{KEYRIGHT,0,"RIGHT"},{KEYHF,0,"FREEHAND"},{KEYPHONE,0,"IP/INPUT"},{KEYSET,0,"SET"},{KEYMSG,0,"PAYMENT"},{KEYFN,0,"FUN"},{KEYPAY,0,"PAY"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYSTAR,0,"*"},{KEY0,0,"0"},{KEYNUM,0,"#"},{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}}; 
    
	//MT30整机测试时的按键顺序: 1 2 3 4 5 6 7 8 9 确认 0 清除 取消
	KEY_ORDER KEYORDER_MT30[13]={{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEY0,0,"0"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}};
	
	
	//S80整机测试时的按键顺序:F1 F2 F3 F4 上下 功能 1 2 3 菜单 4 5 6 字母 7 8 9 确认 清除 0 取消
	KEY_ORDER KEYORDER_S300[15]={{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYFN,0,"FUN"},{KEY0,0,"0"},{KEYALPHA,0,"ALPHA"},{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}}; 
	KEY_ORDER KEYORDER_S800[19]={{KEYATM1,0,"ATM1"},{KEYUP,0,"UP"},{KEYDOWN,0,"DOWN"},{KEYMENU,0,"MENU"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYFN,0,"FUN"},{KEY0,0,"0"},{KEYALPHA,0,"ALPHA"},{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}}; 
	KEY_ORDER KEYORDER_S900[18]={{KEYUP,0,"UP"},{KEYDOWN,0,"DOWN"},{KEYMENU,0,"MENU"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYFN,0,"FUN"},{KEY0,0,"0"},{KEYALPHA,0,"ALPHA"},{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}}; 
	
	KEY_ORDER KEYORDER_S200[16]={{KEYF1,0,"F1"},{KEYF2,0,"F2"},{KEYF3,0,"F3"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEY0,0,"0"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}}; 
	//T610 T620按键情况
	KEY_ORDER KeyOrder_T610[4]={{KEYUP,0,"UP"},{KEYDOWN,0,"DOWN"},{KEYENTER,0,"ENTER"},{KEYCANCEL,0,"CANCEL"}}; 
    KEY_ORDER KeyOrder_T620[20]={{KEYF1,0,"F1"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEYADD,0,"+"},{KEYF2,0,"F2"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEYSUB,0,"-"},{KEYF3,0,"F3"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYMUL,0,"*"},{KEYENTER,0,"ENTER"},{KEYDOT,0,"."},{KEY0,0,"0"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}};
	KEY_ORDER KeyOrder_D200[15]={{KEYF1,0,"F1"},{KEYF2,0,"F2"},{KEYCLEAR,0,"CLEAR"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEY0,0,"0"},{KEYCANCEL,0,"CANCEL"}};		   //D200整机测试时的按键测试: F1 F2 < 1 2 3 4 5 6 7 8 9 确认 0 取消

	KEY_ORDER KeyOrder_D210[18]={{KEYUP,0,"UP"},{KEYDOWN,0,"DOWN"},{KEYMENU,0,"MENU"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYFN,0,"FUN"},{KEY0,0,"0"},{KEYALPHA,0,"ALPHA"},{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}};//D210整机测试时的按键测试: F1 F2 < 1 2 3 4 5 6 7 8 9 确认 0 取消
	KEY_ORDER KeyOrder_S910[18]={{KEYUP,0,"UP"},{KEYDOWN,0,"DOWN"},{KEYMENU,0,"MENU"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYFN,0,"FUN"},{KEY0,0,"0"},{KEYALPHA,0,"ALPHA"},{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}}; 
	//memset(KEYORDER,0,sizeof(KEYORDER));
	switch(TermInfo[0])
	{
		case S80: KEYORDER = KEYORDER_S80; KeyNum = 22;break;
		case S90: KEYORDER = KEYORDER_S90; KeyNum = 22;break;
		case S60: KEYORDER = KEYORDER_S60; KeyNum = 22;break;
		case S78: KEYORDER = KEYORDER_S78; KeyNum = 19;break;
			
		case S500:
		case S58: KEYORDER = KEYORDER_S58; KeyNum = 18;break;
		case T52: KEYORDER = KEYORDER_T52; KeyNum = 31;break;
		case T60: KEYORDER = KEYORDER_T60; KeyNum = 15;break;
		case SP30:KEYORDER = KEYORDER_SP30;KeyNum = 16;break;
		case MT30:KEYORDER = KEYORDER_MT30;KeyNum = 13;break;
		case S300:KEYORDER = KEYORDER_S300;KeyNum = 15;break;
		case S900:KEYORDER = KEYORDER_S900;KeyNum = 18;break;
		case S800:KEYORDER = KEYORDER_S800;KeyNum = 19;break;
		case S200:KEYORDER = KEYORDER_S200;KeyNum = 16;break;
		case D200:KEYORDER = KeyOrder_D200; KeyNum = 15;break;
		case D210:KEYORDER = KeyOrder_D210; KeyNum = 18;break;
		case S910:KEYORDER = KeyOrder_S910; KeyNum = 18;break;

		case T610:KEYORDER = KeyOrder_T610;KeyNum = 4;break;
		case T620:KEYORDER = KeyOrder_T620;KeyNum = 20;break;
		default:break;
	}

	if(strstr(InfoOut[C_PN],"S90-0W0-363-02EA")!=NULL)
	{
		KEYORDER = KEYORDER_S90_; KeyNum = 22;
	}

	ScrClrLine(2,7); 
	kbflush();

	pStrEnglish = "Pls Press Key...";
	pStrChinese = "按键测试,请按键 ";
	ScrDispStr(0,2,1,pStrChinese, pStrEnglish);
	memset(buff_kbhit,0x00,sizeof(buff_kbhit)); 
	

    while(1)
	{
		count_getkey=0;
		for(j=0;j<KeyNum;j++)
		{
			ucRet=getkey();
			ScrClrLine(4,7);
			if(ucRet==KEYORDER[j].KeyValue)
			{
        		ScrDispStr(0,6,1,"按键:%s","Key:%s",KEYORDER[j].KeyDIS);
				KEYORDER[j].Flag_OK=1;
				kblight((uchar)(count_getkey%2));
				count_getkey++;
				if(ucRet==KEYCANCEL)  break;
			}
			else
			{	
				pStrEnglish = "Error!";
				pStrChinese = "按键错误！";
				ScrDispStr(0,4,1,pStrChinese, pStrEnglish);  
				KEYORDER[j].Flag_OK=0;
				BeepFail();	
                WaitEsc(0,1);
				goto ENDERR; 
			}
		}

		if(ucRet==KEYCANCEL)
		{    
		    ScrClrLine(2,7);
            //如果获取的已按键数目count_getkey等于相应机型应测试的按键数目KeyNum，屏幕显示通过，并转到ENDOK；否则，显示键未测完，转到ENDERR
            if(count_getkey == KeyNum)               
		    {
				ScrDispStr(0,4,1,"%d个按键全测通过","%d keys all pass",KeyNum);
			    BeepOK();
				WaitEsc(0,1);
			    goto ENDOK;
		    }
        }
    }

ENDOK:
	return 0;
ENDERR:
	return 1;
}

uchar KbLedTest(uchar flag)
{
	uchar ucRet;
	ScrClrLine(2,7);

	if(TestModule[T_KEYLED]=='N')
	{		
		ScrClrLine(2,7);
		ScrDispStr(0,4,1,"无按键背光", "No KEY LED");
		getkey();
		return 1;
	}
	
	ScrDispStr(0,2,1,"按键背光亮灭测试","KEY LED TEST");
	
	TimerSet(2,20);
	while(1)
	{
		kblight(2);
		DelayMs(200);
		kblight(0);
		DelayMs(200);
		if(TimerCheck(2)==0)break;		
	}
	kblight(1);
	ucRet=JudgeState("按键背光","KEY LED");
	if(ucRet==KEYENTER){
		RECORD(1, "KEYLED=1\r\n");
		return 0;
	}
	else{
		RECORD(1, "KEYLED=0\r\n");
		return 1;
	}
}

void KbOneTest1(void)
{
	KbOneTest(1);
}
void KbTwoTest1(void)
{
	KbTwoTest(1);
}
void KbLedTest1(void)
{
	KbLedTest(1);
}



void KbTestAll(void)
{
   
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-单次测试",		"1-Single Test",		KbOneTest1,},
		{"2-抖动测试",		"2-Dithering Test",		KbTwoTest1,},
		{"3-按键背光",		"3-KEYLED Test",		KbLedTest1,}
	};
	
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"按键单项测试","KeyPress Test",3);
}

// flag=0 整机测试，有序按键测试
uchar auto_kb(uchar flag)
{
	int j,count_getkey;
	unsigned char ucRet,buff_kbhit[30];
	unsigned char *pStrChinese, *pStrEnglish;
	unsigned char KeyNum=22;
	unsigned char flagErr=0;
	KEY_ORDER *KEYORDER;
	
	//S58
	KEY_ORDER KEYORDER_S58[18]={
		{KEYFN,0,"FUNCT"},{KEYDOWN,0,"DOWN"},{KEYUP,0,"UP"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEYMENU,0,"MENU"},
		{KEYALPHA,0,"ALPHA"},{KEY6,0,"6"},{KEY5,0,"5"},{KEY4,0,"4"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},
		{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},{KEY0,0,"0"},{KEYCANCEL,0,"CANCEL"}};
	//KEY_ORDER KEYORDER_S90[22]={{KEYUP,0,"UP"},{KEYFN,0,"FUN"},{KEYALPHA,0,"ALPHA"},{KEYDOWN,0,"DOWN"},{KEYMENU,0,"MENU"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEY0,0,"0"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"},{KEYATM1,0,"ATM1"},{KEYATM2,0,"ATM2"},{KEYATM3,0,"ATM3"},{KEYATM4,0,"ATM4"}};  
	//KEY_ORDER KEYORDER_S80[22]={{KEYUP,0,"UP"},{KEYFN,0,"FUN"},{KEYALPHA,0,"ALPHA"},{KEYDOWN,0,"DOWN"},{KEYMENU,0,"MENU"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEY0,0,"0"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"},{KEYATM1,0,"ATM1"},{KEYATM2,0,"ATM2"},{KEYATM3,0,"ATM3"},{KEYATM4,0,"ATM4"}};  
	KEY_ORDER KEYORDER_S90[22]={{KEYATM1,0,"ATM1"},{KEYATM2,0,"ATM2"},{KEYATM3,0,"ATM3"},{KEYATM4,0,"ATM4"},{KEYUP,0,"UP"},{KEYFN,0,"FUN"},{KEYMENU,0,"MENU"},{KEYDOWN,0,"DOWN"},{KEYALPHA,0,"ALPHA"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY6,0,"6"},{KEY5,0,"5"},{KEY4,0,"4"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEY0,0,"0"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}};  
	//KEY_ORDER KEYORDER_S80[22]={{KEYATM1,0,"ATM1"},{KEYATM2,0,"ATM2"},{KEYATM3,0,"ATM3"},{KEYATM4,0,"ATM4"},{KEYUP,0,"UP"},{KEYFN,0,"FUN"},{KEYMENU,0,"MENU"},{KEYDOWN,0,"DOWN"},{KEYALPHA,0,"ALPHA"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEY6,0,"6"},{KEY5,0,"5"},{KEY4,0,"4"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},{KEYENTER,0,"ENTER"},{KEY0,0,"0"},{KEYCLEAR,0,"CLEAR"},{KEYCANCEL,0,"CANCEL"}};  
	KEY_ORDER KEYORDER_S80[22]={
		{KEYATM1,0,"ATM1"},{KEYATM2,0,"ATM2"},{KEYATM3,0,"ATM3"},{KEYATM4,0,"ATM4"},
		{KEYFN,0,"FUNCT"},{KEYDOWN,0,"DOWN"},{KEYUP,0,"UP"},{KEY1,0,"1"},{KEY2,0,"2"},{KEY3,0,"3"},{KEYMENU,0,"MENU"},
		{KEYALPHA,0,"ALPHA"},{KEY6,0,"6"},{KEY5,0,"5"},{KEY4,0,"4"},{KEY7,0,"7"},{KEY8,0,"8"},{KEY9,0,"9"},
		{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},{KEY0,0,"0"},{KEYCANCEL,0,"CANCEL"}};
	



	KEY_ORDER KEYORDER_S910[18]={
		{KEYUP,0,"UP"},{KEYDOWN,0,"DOWN"},{KEYMENU,0,"MENU"},{KEY3,0,"3"},{KEY2,0,"2"},{KEY1,0,"1"},
		{KEY4,0,"4"},{KEY5,0,"5"},{KEY6,0,"6"},{KEY9,0,"9"},{KEY8,0,"8"},{KEY7,0,"7"},
		{KEYFN,0,"FUNCT"},{KEY0,0,"0"},{KEYALPHA,0,"ALPHA"},{KEYENTER,0,"ENTER"},{KEYCLEAR,0,"CLEAR"},
		{KEYCANCEL,0,"CANCEL"}};

	switch(TermInfo[0])
	{
		case S500:
		case S58: KEYORDER = KEYORDER_S58; KeyNum = 18;break;
		case S80: KEYORDER = KEYORDER_S80; KeyNum = 22;break;
		case S90: KEYORDER = KEYORDER_S90; KeyNum = 22;break;
		case S910: KEYORDER = KEYORDER_S910; KeyNum = 18;break;
		default:  KEYORDER = KEYORDER_S58; KeyNum = 18;break;
	}

	ScrClrLine(2,7); 
	kbflush();
	pStrEnglish = "Pls Press Key...";
	pStrChinese = "按键测试,请按键 ";
	ScrDispStr(0,2,1,pStrChinese, pStrEnglish);
	memset(buff_kbhit,0x00,sizeof(buff_kbhit)); 
    while(1)
	{
		count_getkey=0;
		for(j=0;j<KeyNum;j++)
		{
			ucRet=getkey();
			ScrClrLine(4,7);
			if(ucRet==KEYORDER[j].KeyValue)
			{
        		ScrDispStr(0,6,1,"按键:%s","Key:%s",KEYORDER[j].KeyDIS);
				KEYORDER[j].Flag_OK=1;
				kblight((uchar)(count_getkey%2));
				count_getkey++;
				if(ucRet==KEYCANCEL)  break;
			}
			else
			{	
				pStrEnglish = "Error!";
				pStrChinese = "按键错误！";
				ScrDispStr(0,4,1,pStrChinese, pStrEnglish);  
				KEYORDER[j].Flag_OK=0;
				BeepFail();	
				goto ENDERR; 
			}
		}

		if(ucRet==KEYCANCEL)
		{    
		    ScrClrLine(2,7);
            //如果获取的已按键数目count_getkey等于相应机型应测试的按键数目KeyNum，屏幕显示通过，并转到ENDOK；否则，显示键未测完，转到ENDERR
            if(count_getkey == KeyNum)               
		    {
				ScrDispStr(0,4,1,"%d个按键全测通过","%d keys all pass",KeyNum);
			    goto ENDOK;
		    }
        }
    }

ENDOK:
	return 0;
ENDERR:
	return 1;
}
