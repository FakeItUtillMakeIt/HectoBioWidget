#pragma once
//设计一个电路驱动类
//加载电路驱动参数
//电路驱动
#pragma comment(lib,"USB2069.lib")
#include <usb2069.h>
#include <string>

class Circuit
{
public:
	BOOL bsoftTrig;//软件触发标志
	//采集参数初始化结构体
	//加载主界面设置参数
	USB2069_PARA_INIT para_init;

	//选中界面的参数 默认为连续触发:0 后触发：1 延时：2
	int trig_mode=0;
	//通道，默认全为false
	bool channel[24];
	//触发源 外正沿：0 外负沿：1 边缘触发：2
	int trig_source = 0;
	//采样频率
	int sample_freq = 0;
	//触发电平
	int trig_level = 0;
	//触发长度
	int trig_length = 0;
	//延时
	int delay = 0;
	//AD设备

	//read_data_length
	ULONG read_data_length;
	//设备号  默认只连接一个 所以为0
	CONST UCHAR DEFAULT_DEVICE_NUM=0;
	HANDLE link_device =nullptr;
	//初始化AD返回信息
	char* ret_flag;
	bool device_flag=false;
	bool init_AD_flag;

	void init_circuit_para();
	bool init_start_AD_device();

public:
	
	//设置
	bool init_usb2069_AD();
	bool judge_high_usbdevice(PUCHAR hign_device);
	int judge_link_device();
	bool judge_unlink_device();
	//bool init_start_AD_device();
	bool stop_AD_device();
	//
	bool set_DO(LONG pDO);
	bool get_DI(PLONG pDI);
	//校正零偏
	bool recover_ADoffset(LONG AD_offset, bool WtRdflag = 0,int Chn=0);
	//读取
	bool get_bufover(PLONG pBufOVer);
	bool get_freq(double* freq);
	bool read_AD(PUSHORT data_buffer, ULONG data_length);

	bool set_DA(int DA_num, bool DA_enable, long DA_freq, bool DA_loop, int DA_cycleCnt, bool DA_stopflag, PUSHORT DA_buf, int DA_cyclePoint);
};

