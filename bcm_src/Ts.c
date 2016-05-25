#include "public.h"

#ifdef WIN32
int TouchScreenCalibration(void)
{
	return 0;
}
#endif

#define _RED(c)        (((unsigned short)c & 0x00f8) << 8)
#define _GREEN(c)      (((unsigned short)c & 0x00fc) << 3)
#define _BLUE(c)       (((unsigned short)c & 0x00f8) >> 3)
//#define RGB(r, g, b)   (_RED(r) | _GREEN(g) | _BLUE(b))
void discolor(void)
{
	uchar dat;
	int i,j,k,n;

	for(i=0; i< stLcdInfo.width; i++)
	{
		for(j=0; j< stLcdInfo.height; j++)
		{
			k = j%40;//
			n = j/40;//

			dat = 255-k*6;
			switch(n%6)
			{
				case 0:CLcdDrawPixel(i,j,RGB(dat, dat, 0));break;				
				case 1:CLcdDrawPixel(i,j,RGB(0, dat, dat));break;
				case 2:CLcdDrawPixel(i,j,RGB(dat, 0, dat));break;
				case 3:CLcdDrawPixel(i,j,RGB(dat, dat, 0xc0));break;
				case 4:CLcdDrawPixel(i,j,RGB(0xc0, dat, dat));break;
				case 5:CLcdDrawPixel(i,j,RGB(dat, 0xc0, dat));break;
			}

		}       
	}
	getkey();
	ScrPrint(0,2,1,"TESTTEST");
	getkey();
}



void test(void)
{
	ScrPrint(0,2,4,"iRet=%d",TouchScreenCalibration());
	getkey();
}

void CLcdDrawBigPixel(int x,int y,int color)
{
	int i,j;
	for(i=x-1;i<=x+1;i++)
		for(j=y-1;j<=y+1;j++)
			CLcdDrawPixel(i,j,color);
}

typedef struct 
{
	int x0;
	int y0;
	int x1;
	int y1;
}RECT_XY;

uchar TsTest(uchar flag)
{
	int iRet,i,j,n;
	TS_POINT_T ts_points[710];
	TS_POINT_T ts_point_tmp;
	TS_ATTR_T  ts_attr_t;
	RECT_XY RectSize[6];
	int size = 30;

	RectSize[0].x0 = 0;
	RectSize[0].y0 = 0;
	RectSize[0].x1 = stLcdInfo.width - 1;
	RectSize[0].y1 = size -1;

	RectSize[1].x0 = 0;
	RectSize[1].y0 = stLcdInfo.height/2 - size/2;
	RectSize[1].x1 = stLcdInfo.width - 1;
	RectSize[1].y1 = stLcdInfo.height/2 + size/2 - 1;

	RectSize[2].x0 = 0;
	RectSize[2].y0 = stLcdInfo.height - size;
	RectSize[2].x1 = stLcdInfo.width - 1;
	RectSize[2].y1 = stLcdInfo.height - 1;

	RectSize[3].x0 = 0;
	RectSize[3].y0 = 0;
	RectSize[3].x1 = size - 1;
	RectSize[3].y1 = stLcdInfo.height - 1;

	RectSize[4].x0 = stLcdInfo.width/2 - size/2;
	RectSize[4].y0 = 0;
	RectSize[4].x1 = stLcdInfo.width/2 + size/2 + 1;
	RectSize[4].y1 = stLcdInfo.height - 1;

	RectSize[5].x0 = stLcdInfo.width - size;
	RectSize[5].y0 = 0;
	RectSize[5].x1 = stLcdInfo.width - 1;
	RectSize[5].y1 = stLcdInfo.height - 1;

	ScrCls();
	iRet=TouchScreenOpen(0);
	if(iRet)
	{
		sprintf(gErrnoMsg,"TouchScreenOpen=%d",iRet);
		ScrPrint(0,4,1,"OpenTs Fail%d",iRet);
		TouchScreenClose();
		getkey();
		goto ENDERR;
	}

	memset(&ts_attr_t,0,sizeof(ts_attr_t));
	ts_attr_t.sampleRateModel=1;
	TouchScreenAttrSet(&ts_attr_t);

	for(i=0;i<6;i++)
	{
		CLcdBgDrawBox(RectSize[i].x0,RectSize[i].y0,RectSize[i].x1,RectSize[i].y1,C_GREEN);
	}

	for(i=0;i<6;i++)//0,1,2,表示横线   3,4,5,表示竖线
	{
		TouchScreenFlush();
		memset(&ts_points,0,sizeof(ts_points));
		memset(&ts_point_tmp,0,sizeof(ts_point_tmp));
		while(1)
		{
			if(CancelCheck())
			{
				sprintf(gErrnoMsg,"User Cancel");
				goto ENDERR;
			}
			iRet=TouchScreenRead(&ts_point_tmp,1);
			//ScrPrint(0,6,1,"iRet=%d,%d",iRet,ts_point_tmp.pressure);
			if(ts_point_tmp.pressure>0)
			{
				ts_points[0].x=ts_point_tmp.x;
				ts_points[0].y=ts_point_tmp.y;
				//CLcdDrawPixel(ts_point_tmp.x,ts_point_tmp.y,C_RED);
				CLcdDrawBigPixel(ts_point_tmp.x,ts_point_tmp.y,C_RED);
				break;
			}
		}
		n=0;
		while(1)
		{
			TouchScreenRead(&ts_point_tmp,1);
			if(ts_point_tmp.pressure==0)break;
			else if(ts_point_tmp.x!=ts_points[n].x || ts_point_tmp.y!=ts_points[n].y) 
			{
				ts_points[n+1].x=ts_point_tmp.x;
				ts_points[n+1].y=ts_point_tmp.y;
				//CLcdDrawPixel(ts_point_tmp.x,ts_point_tmp.y,C_RED);
				CLcdDrawBigPixel(ts_point_tmp.x,ts_point_tmp.y,C_RED);
				n++;
				if(n==700)goto ENDERR;
			}
		}

//		ScrPrint(0,6,1,"iRet=%d,%d,n",iRet,ts_point_tmp.pressure,n);
		if(i<=2) //横线
		{
			
			if( ts_points[0].x>size || ts_points[n].x<(stLcdInfo.width-size) )
			{
				i--;
				continue;
			}
			
			for(j=0;j<n;j++)//每个点判断
			{
				if(ts_points[j].y<RectSize[i].y0 || ts_points[j].y>RectSize[i].y1)goto ENDERR;
			}
		}
		else  //竖线
		{
			
			if(ts_points[0].y>size || ts_points[n].y<(stLcdInfo.height-size) )
			{
				i--;
				continue;
			}
			
			for(j=0;j<n;j++)//每个点判断
			{
				if(ts_points[j].x<RectSize[i].x0 || ts_points[j].x>RectSize[i].x1)goto ENDERR;
			}
		}
		CLcdClrRect(RectSize[i].x0,RectSize[i].y0,RectSize[i].x1,RectSize[i].y1,0);
		CLcdBgDrawBox(RectSize[i].x0,RectSize[i].y0,RectSize[i].x1,RectSize[i].y1,C_BLUE);

	}
	ScrDispStr(0,4,1,"测试成功","Test OK");
	BeepOK();
	ScrCls();
	TouchScreenClose();
	CLcdSetBgColor(C_WHITE);
	return 0;
ENDERR:
	ScrDispStr(0,4,1,"测试失败","Test Fail");
	BeepFail();
	ScrCls();
	CLcdSetBgColor(C_WHITE);
	TouchScreenClose();
	return 1;

}

void TsRectTest(void)
{
	TS_POINT_T ts_point_tmp;
	TS_ATTR_T ts_attr_t;
	int i,j,iRet;
	int x0,y0,x1,y1,x,y;

	int rect_size = 40;//
	char cid[10][10];
	int color[2] ={C_GREEN,C_WHITE};

	iRet=TouchScreenOpen(0);
	if(iRet)
	{
		ScrPrint(0,4,1,"OpenTs Fail%d",iRet);
		TouchScreenClose();
		getkey();
		return;
	}
	memset(&ts_attr_t,0,sizeof(ts_attr_t));
	ts_attr_t.sampleRateModel=1;
	TouchScreenAttrSet(&ts_attr_t);
	ScrCls();
	ScrDispStr(0,4,1,"    触摸变色","Hit Change Color");

	memset(cid,0,sizeof(cid));
	for(i=0; i<(stLcdInfo.height/rect_size + 1); i++){
		for(j=0; j<(stLcdInfo.width/rect_size + 1); j++){
			x0 = j*rect_size;
			y0 = i*rect_size;
			x1 = (j+1)*rect_size - 1;
			y1 = (i+1)*rect_size - 1;

			if(x1 > (stLcdInfo.width - 1))x1 = stLcdInfo.width - 1;
			if(y1 > (stLcdInfo.height - 1))y1 = stLcdInfo.height - 1;

			CLcdBgDrawBox(x0,y0,x1,y1,color[cid[i][j]]);
			CLcdDrawRect(x0,y0,x1,y1,C_BLACK);

		}
	}

	while(1)
	{
BEGIN:
		if(CancelCheck())break;
		memset(&ts_point_tmp,0,sizeof(ts_point_tmp));
		iRet=TouchScreenRead(&ts_point_tmp,1);
		if(ts_point_tmp.pressure>0)
		{
			x = ts_point_tmp.x;
			y = ts_point_tmp.y;
			for(i=0; i<(stLcdInfo.height/rect_size + 1); i++){
				for(j=0; j<(stLcdInfo.width/rect_size + 1); j++){
					x0 = j*rect_size;
					y0 = i*rect_size;
					x1 = (j+1)*rect_size - 1;
					y1 = (i+1)*rect_size - 1;
					if(x1 > (stLcdInfo.width - 1)) x1 = stLcdInfo.width - 1;
					if(y1 > (stLcdInfo.height - 1))y1 = stLcdInfo.height - 1;
					
					if(x>=x0 && x<=x1 && y>=y0 && y<=y1)
					{
						if(cid[i][j])cid[i][j]=0;
						else cid[i][j] = 1;
						CLcdBgDrawBox(x0,y0,x1,y1,color[cid[i][j]]);
						CLcdDrawRect(x0,y0,x1,y1,C_BLACK);
						while(ts_point_tmp.pressure)TouchScreenRead(&ts_point_tmp,1);
						goto BEGIN;
					}
										
				}
			}

		}
	}
	TouchScreenClose();

}



void TsTest1(void)
{
	TsTest(1);
}

void Tsts(void)
{
	TouchScreenCalibration();
}

void TsTestAll(void)
{
	uchar TestNum;

	TESTSINGLE_ITEM TestItem[]={
		{"1-单次",	"1-Single",		TsTest1,	},
		{"2-方块",	"2-Rectangle  ",TsRectTest,	},
		{"3-校验",	"3-Calibration",Tsts,	},
		{"1-测试",	"1-test",		discolor,},
	};
	
	if(TestModule[T_TSCREEN]=='N')
	{
		ScrClrLine(2,7);
		ScrDispStr(0,4,1," 没有触摸屏模块 "," No TsScreen ");
		getkey();
		return;
	}
	
	TestNum=sizeof(TestItem)/sizeof(TestItem[0]);
	DisSubMenuEx(TestItem,TestNum,"触屏单项测试","TsScreen Test",3);

}