#include"public.h"

#define GETTIME1(_time,_run) \
do{ \
	unsigned long timeStart; \
	timeStart = GetTimerCount(); \
	_run; \
	(_time) = GetTimerCount()-timeStart; \
}while(0)


#define	UCL_CIPHER_ENCRYPT  0X01
#define UCL_CIPHER_DECRYPT  0X00
#define	PED_ENCRYPT			0X01
#define	PED_DECRYPT			0X00


//SM
#define PED_SM2_PVT_KEY  0x30 
#define PED_SM2_PUB_KEY  0x31 
#define PED_SM4_TMK      0x32 
#define PED_SM4_TPK      0x33 
#define PED_SM4_TAK      0x34 
#define PED_SM4_TDK      0x35 
#define NO_SM_FUN
//#ifdef NO_SM_FUN 
#ifdef WIN32
int PedGenSM2KeyPair(uchar *PvtKey, uchar *PubKey, ushort KeyLenBit){return 0;};
int PedWriteSM2Key (uchar KeyIdx, uchar KeyType, uchar *KeyValue){return 0;};
int PedSM2Sign (uchar PubKeyIdx, uchar PvtKeyIdx, uchar *Uid, ushort UidLen, uchar *Input, uint InputLen, uchar *Signature){return 0;};
int PedSM2Verify (uchar PubKeyIdx,uchar *Uid, ushort UidLen, uchar *Input, uint InputLen, const uchar *Signature){return 0;};
int PedSM2Recover (uchar KeyIdx, uchar *Input, ushort InputLen, uchar *Output, ushort *OutputLen, uchar Mode){return 0;};
int PedSM3 (uchar *Input, uint InputLen, uchar *Output, uchar Mode){return 0;};
int PedSM4 (uchar KeyIdx, uchar *Initvector, uchar *Input, ushort InputLen, uchar *Output, uchar Mode){return 0;};
int PedGetMacSM4 (uchar KeyIdx,uchar *InitVector, uchar *Input, ushort InputLen, uchar *MacOut,  uchar Mode){return 0;};
int PedGetPinBlockSM4(uchar KeyIdx,uchar *ExpPinLenIn, uchar * DataIn, uchar *PinBlockOut, uchar Mode, ulong TimeOutMs){return 0;};
#endif 
//end SM






int  Tdes(uchar * pszIn,uchar * pszOut,int iDataLen,uchar * pszKey,int iKeyLen,uchar ucMode)
{
	int iLoop;

	if(24==iKeyLen)
	{

		if(ucMode==PED_ENCRYPT)
		{
			for(iLoop=0;iLoop<iDataLen/8;iLoop++)
			{
				des(pszIn+iLoop*8,pszOut+iLoop*8,pszKey,UCL_CIPHER_ENCRYPT);
				des(pszOut+iLoop*8,pszOut+iLoop*8,pszKey+8,UCL_CIPHER_DECRYPT);
				des(pszOut+iLoop*8,pszOut+iLoop*8,pszKey+16,UCL_CIPHER_ENCRYPT);
			}
		}
		else if(ucMode==PED_DECRYPT)
		{
			for(iLoop=0;iLoop<iDataLen/8;iLoop++)
			{
				des(pszIn+iLoop*8,pszOut+iLoop*8,pszKey+16,UCL_CIPHER_DECRYPT);
				des(pszOut+iLoop*8,pszOut+iLoop*8,pszKey+8,UCL_CIPHER_ENCRYPT);
				des(pszOut+iLoop*8,pszOut+iLoop*8,pszKey,UCL_CIPHER_DECRYPT);
			}

		}
	}
	else if (16==iKeyLen)
	{
		if(ucMode==PED_ENCRYPT)
		{
			for(iLoop=0;iLoop<iDataLen/8;iLoop++)
			{
				des(pszIn+iLoop*8,pszOut+iLoop*8,pszKey,UCL_CIPHER_ENCRYPT);
				des(pszOut+iLoop*8,pszOut+iLoop*8,pszKey+8,UCL_CIPHER_DECRYPT);
				des(pszOut+iLoop*8,pszOut+iLoop*8,pszKey,UCL_CIPHER_ENCRYPT);
			}
		}
		else if(ucMode==PED_DECRYPT)
		{
			for(iLoop=0;iLoop<iDataLen/8;iLoop++)
			{
				des(pszIn+iLoop*8,pszOut+iLoop*8,pszKey,UCL_CIPHER_DECRYPT);
				des(pszOut+iLoop*8,pszOut+iLoop*8,pszKey+8,UCL_CIPHER_ENCRYPT);
				des(pszOut+iLoop*8,pszOut+iLoop*8,pszKey,UCL_CIPHER_DECRYPT);
			}

		}
		else 
		{
			return -2;
		}
	}
	else if(8==iKeyLen)
	{	
		if(ucMode==PED_ENCRYPT)
		{
			for(iLoop=0;iLoop<iDataLen/8;iLoop++)
			{
				des(pszIn+iLoop*8,pszOut+iLoop*8,pszKey,UCL_CIPHER_ENCRYPT);
			}
		}
		else if(ucMode==PED_DECRYPT)
		{
			for(iLoop=0;iLoop<iDataLen/8;iLoop++)
			{
				des(pszIn+iLoop*8,pszOut+iLoop*8,pszKey,UCL_CIPHER_DECRYPT);
			}
		}
		else 
		{
			return -2;
		}
	}
	else  
	{
		return -3;
	}
	for(iLoop=0;iLoop<iDataLen%8;iLoop++)
	{
		pszOut[(iDataLen/8)*8+iLoop]=pszIn[(iDataLen/8)*8+iLoop]^0xff;
	}
	return 0;
}

void MAC_Arithmetic1(uchar *DataIn, ushort inlen, uchar *MacKey, uchar MacKeyLen, uchar *VerifyOut)
{
	int i,j;
	uchar tempdata[65535];

	memset(tempdata,0x00,sizeof(tempdata));
	memset(VerifyOut,0x00,8);
	memcpy(tempdata, DataIn, inlen);
	if(inlen%8) inlen=inlen+8;
	for(i=0; i<inlen/8; i++)
	{
		for(j=0; j<8; j++)
		{
			VerifyOut[j]^=tempdata[i*8+j];
		}
		Tdes(VerifyOut,VerifyOut,8,MacKey,MacKeyLen,1);
	}
}

/*奇校验,每个字节都校验,最低位是校验位*/
void make_odd(uchar *key, uchar ucKeyLen)
{
	uchar tmpc,i,j,weight;
	
	for(i=0;i<ucKeyLen;i++)
	{
		for(weight=j=0;j<8;j++)
		{
			tmpc=key[i]<<(j%8);
			if(tmpc&0x80) 
				weight++;
		}
		if(!(weight%2))
		{
			if(key[i]&0x01) 
				key[i]&=0xfe;
			else 
				key[i]|=0x01;
		}
	}
}



///////////////////////////  初始化随机100组TMK ////////////////////
uchar PEDInit(uchar flag)
{
	uchar i,aucKcvData[8];
	uchar SrcKey[24],DstKey[24];
	int iRet;
	int iDstKeyLen=16;
	int iSrcKeyLen=16;


	ST_KEY_INFO st_key_info;
	ST_KCV_INFO st_kcv_info;

	ScrClrLine(2,7);
	ScrPrint(0,2,1,"PedErase...");
	iRet = PedErase();
	if(iRet)
	{
		ScrPrint(0,4,1,"PedErase=%d",iRet);
		getkey();
		return 1;
	}
	
	
	//初始化
	for(i=1;i<101;i++)
	{
		
		ScrPrint(0,4,0,"Init MasKey(%3d%%)",i);
		memset(&st_kcv_info,0,sizeof(st_kcv_info));
		memset(&st_key_info,0,sizeof(st_key_info));
		memset(SrcKey,0,sizeof(SrcKey));
		memset(DstKey,0,sizeof(DstKey));

		memset(DstKey,0xce,16);//明文赋值
		st_key_info.ucSrcKeyType=PED_TMK;
		st_key_info.ucSrcKeyIdx= 0;
		st_key_info.ucDstKeyType=PED_TMK;
		st_key_info.ucDstKeyIdx=i;
		st_key_info.iDstKeyLen=16;

		
		st_kcv_info.iCheckMode = 0x02;
		if(st_kcv_info.iCheckMode==0x02)make_odd(DstKey, iDstKeyLen);
		memcpy(st_key_info.aucDstKeyValue,DstKey,16);
		
		memset(aucKcvData, 0x00, sizeof(aucKcvData));
		switch(st_kcv_info.iCheckMode)     //KCV验证值
		{
		case 0x00:
			break;
		case 0x01:
			Tdes("\x00\x00\x00\x00\x00\x00\x00\x00", aucKcvData,8,DstKey,iDstKeyLen,1);//DstKey
			st_kcv_info.aucCheckBuf[0] = 4;    //KCV长度4字节
			memcpy(st_kcv_info.aucCheckBuf+1, aucKcvData, 4);
			break;
		case 0x02:
			Tdes("\x12\x34\x56\x78\x90\x12\x34\x56", aucKcvData,8,DstKey,iDstKeyLen,1);//DstKey
			st_kcv_info.aucCheckBuf[0] = 4;    //KCV长度4字节
			memcpy(st_kcv_info.aucCheckBuf+1, aucKcvData, 4);
			break;	
		}
		iRet=PedWriteKey(&st_key_info,&st_kcv_info);
		if(iRet)	//写主密钥										  
		{
			ScrPrint(0,4,0,"PedWriteKey(TMK)=%d",iRet);
			BeepFail();
			getkey();
			return 1;
		}

	}
	BeepOK();
	return 0;
}


uchar PEDTest(uchar flag)
{
	uchar i,j,aucKcvData[8];
	uchar TmpKey[24],DstKey[24],SrcKey[24];
	int iRet;
	int iDstKeyLen=16;
	int iSrcKeyLen=16;
	int iKcvDataLen;
	uchar aucKcvMode3Data[110];
	ST_KEY_INFO st_key_info;
	ST_KCV_INFO st_kcv_info;
	
	ScrClrLine(2,7);
	for(i=1;i<101;i++)
	{

		ScrPrint(0,5,0,"Test PEDKey(%3d%%)",i);
		for(j=3;j<=5;j++)// PED_TDK=5,PED_TAK=4,PED_TPK=3
		{
			memset(&st_kcv_info,0,sizeof(st_kcv_info));
			memset(&st_key_info,0,sizeof(st_key_info));
			memset(DstKey,0,sizeof(DstKey));
			memset(SrcKey,0,sizeof(SrcKey));
			memset(TmpKey,0,sizeof(TmpKey));
			
			st_key_info.ucSrcKeyType=PED_TMK;
			st_key_info.ucSrcKeyIdx= i;
			st_key_info.ucDstKeyType=j;//PED_TDK=5,PED_TAK=4,PED_TPK=3
			st_key_info.ucDstKeyIdx=i;
			st_key_info.iDstKeyLen=16;
			
			memset(SrcKey,0xce,16);
			memset(DstKey,0x08,16);
			Tdes(DstKey,TmpKey,iSrcKeyLen,SrcKey,iDstKeyLen, 1);
			memcpy(st_key_info.aucDstKeyValue,TmpKey, 16);
			

			st_kcv_info.iCheckMode = 0x03;
			memset(aucKcvData, 0x00, sizeof(aucKcvData));
			switch(st_kcv_info.iCheckMode)     //KCV验证值
			{
			case 0x00:
				break;
			case 0x01:
				Tdes("\x00\x00\x00\x00\x00\x00\x00\x00", aucKcvData,8,DstKey,iDstKeyLen,1);//DstKey
				st_kcv_info.aucCheckBuf[0] = 4;    //KCV长度4字节
				memcpy(st_kcv_info.aucCheckBuf+1, aucKcvData, 4);
				break;
			case 0x02:
				Tdes("\x12\x34\x56\x78\x90\x12\x34\x56", aucKcvData,8,DstKey,iDstKeyLen,1);//DstKey
				st_kcv_info.aucCheckBuf[0] = 4;    //KCV长度4字节
				memcpy(st_kcv_info.aucCheckBuf+1, aucKcvData, 4);
				break;	
			case 0x03:
				memset(aucKcvMode3Data, 0xee, sizeof(aucKcvMode3Data));//ee
				iKcvDataLen=sizeof(aucKcvMode3Data)-iDstKeyLen;
				memcpy(aucKcvMode3Data, TmpKey, iDstKeyLen);
				MAC_Arithmetic1(aucKcvMode3Data, sizeof(aucKcvMode3Data),SrcKey, iSrcKeyLen, aucKcvData);//SrcKey
				st_kcv_info.aucCheckBuf[0] = iKcvDataLen;    //KCV长度4字节
				memcpy(st_kcv_info.aucCheckBuf+1, aucKcvMode3Data+iDstKeyLen, iKcvDataLen);
				st_kcv_info.aucCheckBuf[0+1+iKcvDataLen] = 0x00;  //计算Mac的模式
				st_kcv_info.aucCheckBuf[0+2+iKcvDataLen] = 8;
				memcpy(st_kcv_info.aucCheckBuf+3+iKcvDataLen, aucKcvData, 8);
			}

			iRet=PedWriteKey(&st_key_info,&st_kcv_info);
			if(iRet)//写DES密钥								  
			{
				ScrClrLine(2,7);//调试用
				ScrPrint(0,5,0,"PedWriteKey(%d)=%d",j,iRet);
				BeepFail();
				getkey();
				return 1;
			}
		}
	}
	BeepOK();
	return 0;
}



uchar SM2Test(uchar flag)
{

	int iRet,total=0,fail=0;
	uchar PvtKey[32];
	uchar PubKey[64];
	ushort KeyLen = 256;
	uchar Uid[16] = {
		0x31, 0x32,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 
		0x31, 0x32,0x33, 0x34, 0x35, 0x36, 0x37, 0x38
	};
	uchar Input[40] = "the original information";
	uchar Sig[64];
	ScrCls();

	if(TestModule[T_SM2]!='Y'){
		ScrPrint(0,2,1,"NO SM2");
		if(flag)getkey();
		return 0;
	}
		

	while(1)
	{
	
		ScrPrint(0,0,1,"SM2 TEST = %d,%d",total++,fail++);
		if(CancelCheck())break;
		ComSends("**** BEGIN *****");
		iRet = PedGenSM2KeyPair (PvtKey,PubKey,KeyLen);
		ComSends("PedGenSM2KeyPair = %d",iRet);
		if(iRet)
		{
			sprintf(gErrnoMsg,"PedGenSM2KeyPair = %d",iRet);
			ScrPrint(0,2,1,"PedGenSM2KeyPair = %d",iRet);
			BeepFail();
			if(getkey()==KEYCANCEL || flag<=1)goto ENDERR;
			else continue;
		}

		iRet = PedWriteSM2Key (1,PED_SM2_PVT_KEY,PvtKey);
		ComSends("PedWriteSM2Key = %d",iRet);
		if(iRet)
		{
			sprintf(gErrnoMsg,"PedWriteSM2Key = %d",iRet);
			ScrPrint(0,2,1,"PedWriteSM2Key = %d",iRet);
			BeepFail();
			if(getkey()==KEYCANCEL || flag<=1)goto ENDERR;
			else continue;
		}

		iRet = PedWriteSM2Key (2,PED_SM2_PUB_KEY,PubKey);
		ComSends("PedWriteSM2Key = %d",iRet);
		if(iRet)
		{
			sprintf(gErrnoMsg,"PedWriteSM2Key = %d",iRet);
			ScrPrint(0,2,1,"PedWriteSM2Key = %d",iRet);
			BeepFail();
			if(getkey()==KEYCANCEL || flag<=1)goto ENDERR;
			else continue;
		}

		
		iRet = PedSM2Sign(2,1,Uid,sizeof(Uid),Input,strlen(Input),Sig);
		ComSends("PedSM2Sign = %d",iRet);
		if(iRet)
		{
			sprintf(gErrnoMsg,"PedSM2Sign = %d",iRet);
			ScrPrint(0,2,1,"PedSM2Sign = %d",iRet);
			BeepFail();
			if(getkey()==KEYCANCEL || flag<=1)goto ENDERR;
			else continue;
		}

		iRet = PedSM2Verify(2,Uid,sizeof(Uid),Input,strlen(Input),Sig);
		ComSends("PedSM2Verify = %d",iRet);
		if(iRet)
		{
			sprintf(gErrnoMsg,"PedSM2Verify = %d",iRet);
			ScrPrint(0,2,1,"PedSM2Verify = %d",iRet);
			BeepFail();
			if(getkey()==KEYCANCEL || flag<=1)goto ENDERR;
			else continue;
		}
		fail--;
		ComSends("****  END  *****\r\n\r\n");
		ScrPrint(0,2,1,"TEST SM2 OK");
		BeepOK();
		DelayMs(500);
		if(flag<=1)goto ENDOK;
	}

ENDOK:
	return 0;
ENDERR:
	return 1;
}


void SM2Test0(void)
{
	SM2Test(0);
}

void SM2Test1(void)
{
	SM2Test(1);
}


void SM2Test2(void)
{
	SM2Test(2);
}



void PEDErase(void)
{
	int iRet;
	ScrClrLine(2,7);
	ScrPrint(0,2,1,"PedErase...");
	iRet = PedErase();
	if(iRet)
	{
		ScrPrint(0,4,1,"PedErase=%d",iRet);
		getkey();
	}
}


void PEDInit1(void)
{
	PEDInit(0);
}

void PEDTest1(void)
{
	PEDTest(0);
}


void PEDTestAll(void)
{
	uchar TestNum;
	TESTSINGLE_ITEM TestItem[]={
		{"1-初始化密钥",	"1-PEDInit",PEDInit1},
		{"2-测试密钥  ",	"2-PEDTest",PEDTest1},
		{"3-删除密钥  ",	"3-PEDClr ",PEDErase},
		{"1-国密测试1",		"1-SM2Test1",SM2Test1},
		{"2-国密测试2",		"2-SM2Test2",SM2Test2},
		{"3-国密测试0",		"3-SM2Test0",SM2Test0},

	};

	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"PED 测试","PED TEST",3);
}
