//这个主要是接口板测试的代码

#ifndef _PORT_TEST_H_
#define _PORT_TEST_H_
#include "public.h"


#ifdef PORT_TEST

#define PORT_PARA "port_para.dat"
#define P_IC		0
#define P_SAM		1 
#define P_RS232A	2
#define P_RS232B	3
#define P_PINPAD	4
#define P_A2B		5
#define P_A2P		6
#define P_B2P		7
#define P_MODEM		8
#define P_LAN		9
#define P_WNET		10
#define P_WIFI		11
#define P_UDISK		12
#define P_UDEV		13
#define P_SCAN		14
#define P_BAT		15
#define P_PRN		16
#define P_BEEP		17
#define P_AUDIO		18
#define P_KEY		19
#define P_MSR		20
#define P_PICC		21
#define P_BYPH		22
#define P_LED		23
#define P_ALL		24


uchar p_test_model[30];
uchar p_test_num;
uchar p_sam_num;

void p_get_config(void);
void p_set_config(void);
void p_port_test(void);

uchar p_ic_test(uchar flag);
uchar p_sam_test(uchar flag);
uchar p_rs232a_test(uchar flag);
uchar p_rs232b_test(uchar flag);
uchar p_rs232p_test(uchar flag);
uchar p_a2b_test(uchar flag);
uchar p_a2p_test(uchar flag);
uchar p_b2p_test(uchar flag);
uchar p_modem_test(uchar flag);
uchar p_lan_test(uchar flag);
uchar p_wnet_test(uchar flag);
uchar p_wifi_test(uchar flag);
uchar p_udisk_test(uchar flag);
uchar p_udev_test(uchar flag);
uchar p_scan_test(uchar flag);
uchar p_bat_test(uchar flag);
uchar p_prn_test(uchar flag); 
uchar p_beep_test(uchar flag);
uchar p_audio_test(uchar flag);
uchar p_key_test(uchar flag);
uchar p_msr_test(uchar flag);
uchar p_picc_test(uchar flag);
uchar p_byph_test(uchar flag);
uchar p_led_test(uchar flag);
uchar p_hearer_test(uchar flag);
uchar p_bt_test(uchar flag);



#endif
#endif
