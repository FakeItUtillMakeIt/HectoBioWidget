#include "Circuit.h"


void Circuit::init_circuit_para() {
	//通道默认所有通道
	for (int i=0;i<24;i++)
	{
		para_init.lEnCh[i] = true;
	}

	//触发源
	para_init.TriggerSource = TRIG_SRC_EXT_RISING;
	//采集模式
	para_init.TriggerMode = TRIG_MODE_CONTINUE;
	//采样率
	para_init.ADFREQ = 240000;
	//触发长度
	para_init.TriggerLength = 10;
	para_init.TriggerDelay = 0;
	para_init.TriggerLevel = 2048;
	

	return;
}

bool Circuit::init_usb2069_AD() {
	return 0;
}

bool Circuit::judge_high_usbdevice(PUCHAR high_device) {
	//UCHAR hign_device = 1;
	bool is_highdevice = USB2069_IsHighDevice(link_device, high_device);
	if (is_highdevice)
	{
		return true;
	}
	return false;
}


int Circuit::judge_link_device() {
	
	link_device = USB2069_Link(DEFAULT_DEVICE_NUM);
	if (link_device !=INVALID_HANDLE_VALUE)
	{
		device_flag = true;
		return true;
	}
	else
	{
		return false;
	}
	
}
bool Circuit::judge_unlink_device() {
	
	//link_device = USB2069_Link(DEFAULT_DEVICE_NUM);
	BOOL unlink_flag = USB2069_UnLink(link_device);
	if (!unlink_flag)
	{
		//打开设备失败
		return true;
	}
}

//初始化AD采集器，并启动AD采集
//启动AD采集之后，主界面才能获取数据
bool Circuit::init_start_AD_device() {

	//device_flag = judge_link_device();
	if (!device_flag)
	{
		ret_flag = "读取输入设备失败";
		return false;
	}

	//更新选中通道
	for (int i=0;i<24;i++)
	{
		para_init.lEnCh[i] = LONG(channel[i]);
		
	}

	//更新触发模式
	switch (trig_mode)
	{
	case 0:
		para_init.TriggerMode = TRIG_MODE_CONTINUE;
		break;
	case 1:
		para_init.TriggerMode = TRIG_MODE_POST;
		break;
	case 2:
		para_init.TriggerMode = TRIG_MODE_DELAY;
		break;
	default:
		para_init.TriggerMode=TRIG_MODE_CONTINUE;
	}

	//更新触发源
	switch (trig_source)
	{
	case 0:
		para_init.TriggerSource = TRIG_SRC_EXT_RISING;
		break;
	case 1:
		para_init.TriggerSource = TRIG_SRC_EXT_FALLING;
		break;
	case 2:
		para_init.TriggerSource = TRIG_SRC_SOFT;
		break;
	default:
		para_init.TriggerSource = TRIG_SRC_EXT_RISING;
	}
	//更新采样频率
	para_init.ADFREQ = sample_freq;
	//更新触发长度
	para_init.TriggerLength = trig_length;
	//更新触发电平
	para_init.TriggerLevel = trig_level;
	//更新触发延时，仅延时触发模式有效
	para_init.TriggerDelay = delay;
	//设置每次读取采样长度
	read_data_length = 1024 * 100;

	init_AD_flag= USB2069_InitAD(link_device,&para_init);
	if (!init_AD_flag)
	{
		ret_flag = "初始化AD失败，请检测设备";
		return false;
	}
	else
	{
		ret_flag = "初始化AD成功，开始采集数据";
		return true;
	}

	return 0;
}


bool Circuit::stop_AD_device() {
	USB2069_StopAD(link_device,DEFAULT_DEVICE_NUM);
	return 0;
}

//数字输出
bool Circuit::set_DO(LONG pDO) {
	bool set_DOflag = USB2069_SetDO(link_device,pDO);
	if (set_DOflag)
	{
		return true;
	}
	return false;
}

//数字输入
bool Circuit::get_DI(PLONG pDI	) {
	bool get_DIfalg = USB2069_GetDI(link_device, pDI);
	if (get_DIfalg)
	{
		return true;
	}
	return false;
}

//校正零偏
// 零偏读取 WtRd_flag=0 :写 1:读
// Chn:0-23 表示通道1-24
//取值范围-32768-32767
bool Circuit::recover_ADoffset(LONG AD_offset,bool WtRdflag,int Chn) {
	if (AD_offset<-32768 )
	{
		AD_offset = -32768;
	}
	else if (AD_offset>32767)
	{
		AD_offset = 32767;
	}
	bool recover_ADoffsetfalg = USB2069_ADoffset(link_device,WtRdflag,Chn,&AD_offset);
	if (recover_ADoffsetfalg)
	{
		return true;
	}
	return false;
}


//读取硬件缓存溢出位
//溢出，则pBufOVer 返回1，否则返回0
bool Circuit::get_bufover(PLONG pBufOVer) {
	bool get_bufoverfalg = USB2069_GetBufOver(link_device, pBufOVer);
	if (get_bufoverfalg)
	{
		return true;
	}
	return false;
}

//读频率
//freq[0]返回测频通道1值，freq[1]返回测频通道2值，
//freq[2]返回测频通道3值，freq[3]返回测频通道4值，
//频率数值=FRQ_BASE_CLOCK/freq[n]
bool Circuit::get_freq(double* freq) {
	LONG pData[4];
	bool freqflag = USB2069_GetFreq(link_device, pData);
	if (freqflag)
	{
		char output[100];
		if (pData[0]>0)
		{
			*freq = (double)FRQ_BASE_CLOCK/(double)pData[0];
			
		}
		else
		{
			*freq = 0;
		}
		if (pData[1]>0)
		{
			*freq = (double)FRQ_BASE_CLOCK / (double)pData[1];
		}
		else 
		{
			*freq = 0;
		}
		if (pData[2] > 0)
		{
			*freq = (double)FRQ_BASE_CLOCK / (double)pData[2];
		}
		else
		{
			*freq = 0;
		}
		if (pData[3] > 0)
		{
			*freq = (double)FRQ_BASE_CLOCK / (double)pData[3];
		}
		else
		{
			*freq = 0;
		}

		return true;
	}
	return false;
}

//读AD数据  更新data_buffer
//按照FCFR_USB_2069说明书说明
//AD数据应该转为实际数据
//转换公式：v_value=10.0*AD_data/0xFFFF-5.0
bool Circuit::read_AD(PUSHORT data_buffer,ULONG data_length) {
	if (init_AD_flag)
	{
		bool read_data_flag = USB2069_ReadAD(link_device, data_buffer, data_length);

		if (!read_data_flag)
		{
			ret_flag = "读取AD失败";
			return false;
		}
		else {
			ret_flag = "读取AD成功";
		}
		
		//stop_AD_device();
		return true;
	}
	
}

//设置DA 数字转模拟  24位
bool Circuit::set_DA(int DA_num, bool DA_enable, long DA_freq, bool DA_loop, int DA_cycleCnt, bool DA_stopflag,PUSHORT DA_buf, int DA_cyclePoint) {
	//设置USB_DA参数
	
	/*bool set_DA_flag=USB2069_SetDA(link_device, DA_num, DA_enable, &DA_freq,
		DA_loop, DA_cycleCnt, DA_stopflag, DA_buf, DA_cyclePoint);*/
	bool set_DA_flag = USB2069_SetDA(link_device, DA_num, DA_enable, &DA_freq,
		DA_loop, DA_cycleCnt, DA_stopflag, DA_buf,DA_cyclePoint);
	
	return set_DA_flag;
}

