#include "wm8978.h"
#include "hal_i2c1.h"

// static uint16_t WM8978_REGVAL_TBL[58] = {0};

static const char *WM8978_TAG = "WM8978";

//WM8978寄存器值缓存区(总共58个寄存器,0~57),占用116字节内存
//因为WM8978的IIC操作不支持读操作,所以在本地保存所有寄存器值
//写WM8978寄存器时,同步更新到本地寄存器值,读寄存器时,直接返回本地保存的寄存器值.
//注意:WM8978的寄存器值是9位的,所以要用uint16_t来存储.
static uint16_t WM8978_REGVAL_TBL[58]=
{
	0X0000,0X0000,0X0000,0X0000,0X0050,0X0000,0X0140,0X0000,
	0X0000,0X0000,0X0000,0X00FF,0X00FF,0X0000,0X0100,0X00FF,
	0X00FF,0X0000,0X012C,0X002C,0X002C,0X002C,0X002C,0X0000,
	0X0032,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
	0X0038,0X000B,0X0032,0X0000,0X0008,0X000C,0X0093,0X00E9,
	0X0000,0X0000,0X0000,0X0000,0X0003,0X0010,0X0010,0X0100,
	0X0100,0X0002,0X0001,0X0001,0X0039,0X0039,0X0039,0X0039,
	0X0001,0X0001
};

//WM8978写寄存器
//reg:寄存器地址
//val:要写入寄存器的值
//返回值:0,成功;
//    其他,错误代码
static esp_err_t WM8978_Write_Reg(uint8_t reg, uint16_t val) {
    uint8_t data[2];
    data[0] = (reg << 1) | ((val >> 8) & 0x01);
    data[1] = val & 0xFF;

    esp_err_t ret = hal_i2c_master_write_byte(WM8978_ADDR, data[0], data[1]);

    if (ret == ESP_OK) {
        WM8978_REGVAL_TBL[reg] = val;
    } else {
		log_printf(WM8978_TAG, LOG_ERROR, "Failed to write register: 0x%02X", reg);
    }

    return ret;
}
//WM8978读寄存器
//就是读取本地寄存器值缓冲区内的对应值
//reg:寄存器地址
//返回值:寄存器值
static uint16_t WM8978_Read_Reg(uint8_t reg) {
    return WM8978_REGVAL_TBL[reg];
}
//WM8978 DAC/ADC配置
//adcen:adc使能(1)/关闭(0)
//dacen:dac使能(1)/关闭(0)
void WM8978_ADDA_Cfg(uint8_t dacen, uint8_t adcen) {
    uint16_t regval;

    regval = WM8978_Read_Reg(3);
    regval = dacen ? (regval | 3) : (regval & ~3);
    WM8978_Write_Reg(3, regval);

    regval = WM8978_Read_Reg(2);
    regval = adcen ? (regval | 3) : (regval & ~3);
    WM8978_Write_Reg(2, regval);
}
//WM8978 输入通道配置
//micen:MIC开启(1)/关闭(0)
//lineinen:Line In开启(1)/关闭(0)
//auxen:aux开启(1)/关闭(0)
void WM8978_Input_Cfg(uint8_t micen, uint8_t lineinen, uint8_t auxen) {
    uint16_t regval;

    regval = WM8978_Read_Reg(2);
    regval = micen ? (regval | (3 << 2)) : (regval & ~(3 << 2));
    WM8978_Write_Reg(2, regval);

    regval = WM8978_Read_Reg(44);
    regval = micen ? (regval | (3 << 4) | (3 << 0)) : (regval & ~((3 << 4) | (3 << 0)));
    WM8978_Write_Reg(44, regval);

    if (lineinen) {
        WM8978_LINEIN_Gain(5);
    } else {
        WM8978_LINEIN_Gain(0);
    }

    if (auxen) {
        WM8978_AUX_Gain(7);
    } else {
        WM8978_AUX_Gain(0);
    }
}
//WM8978 输出配置
//dacen:DAC输出(放音)开启(1)/关闭(0)
//bpsen:Bypass输出(录音,包括MIC,LINE IN,AUX等)开启(1)/关闭(0)
void WM8978_Output_Cfg(uint8_t dacen, uint8_t bpsen) {
    uint16_t regval = dacen ? (1 << 0) : 0;

    if (bpsen) {
        regval |= (1 << 1) | (5 << 2);
    }

    WM8978_Write_Reg(50, regval);
    WM8978_Write_Reg(51, regval);
}
//WM8978 MIC增益设置(不包括BOOST的20dB,MIC-->ADC输入部分的增益)
//gain:0~63,对应-12dB~35.25dB,0.75dB/Step
void WM8978_MIC_Gain(uint8_t gain)
{
	gain&=0X3F;
	WM8978_Write_Reg(45,gain);		//R45,左通道PGA设置
	WM8978_Write_Reg(46,gain|1<<8);	//R46,右通道PGA设置
}
//WM8978 L2/R2(也就是Line In)增益设置(L2/R2-->ADC输入部分的增益)
//gain:0~7,0表示通道禁止,1~7,对应-12dB~6dB,3dB/Step
void WM8978_LINEIN_Gain(uint8_t gain)
{
	uint16_t regval;
	gain&=0X07;
	regval=WM8978_Read_Reg(47);	//读取R47
	regval&=~(7<<4);			//清除原来的设置
	WM8978_Write_Reg(47,regval|gain<<4);//设置R47
	regval=WM8978_Read_Reg(48);	//读取R48
	regval&=~(7<<4);			//清除原来的设置
	WM8978_Write_Reg(48,regval|gain<<4);//设置R48
}
//WM8978 AUXR,AUXL(PWM音频部分)增益设置(AUXR/L-->ADC输入部分的增益)
//gain:0~7,0表示通道禁止,1~7,对应-12dB~6dB,3dB/Step
void WM8978_AUX_Gain(uint8_t gain)
{
	uint16_t regval;
	gain&=0X07;
	regval=WM8978_Read_Reg(47);	//读取R47
	regval&=~(7<<0);			//清除原来的设置
	WM8978_Write_Reg(47,regval|gain<<0);//设置R47
	regval=WM8978_Read_Reg(48);	//读取R48
	regval&=~(7<<0);			//清除原来的设置
	WM8978_Write_Reg(48,regval|gain<<0);//设置R48
}
//设置I2S工作模式
//fmt:0,LSB(右对齐);1,MSB(左对齐);2,飞利浦标准I2S;3,PCM/DSP;
//len:0,16位;1,20位;2,24位;3,32位;
void WM8978_I2S_Cfg(uint8_t fmt,uint8_t len)
{
	fmt&=0X03;
	len&=0X03;//限定范围
	WM8978_Write_Reg(4,(fmt<<3)|(len<<5));	//R4,WM8978工作模式设置
}

//设置耳机左右声道音量
//voll:左声道音量(0~63)
//volr:右声道音量(0~63)
void WM8978_HPvol_Set(uint8_t voll,uint8_t volr)
{
	voll&=0X3F;
	volr&=0X3F;//限定范围
	if(voll==0)voll|=1<<6;//音量为0时,直接mute
	if(volr==0)volr|=1<<6;//音量为0时,直接mute
	WM8978_Write_Reg(52,voll);			//R52,耳机左声道音量设置
	WM8978_Write_Reg(53,volr|(1<<8));	//R53,耳机右声道音量设置,同步更新(HPVU=1)
}
//设置喇叭音量
//voll:左声道音量(0~63)
void WM8978_SPKvol_Set(uint8_t volx)
{
	volx&=0X3F;//限定范围
	if(volx==0)volx|=1<<6;//音量为0时,直接mute
 	WM8978_Write_Reg(54,volx);			//R54,喇叭左声道音量设置
	WM8978_Write_Reg(55,volx|(1<<8));	//R55,喇叭右声道音量设置,同步更新(SPKVU=1)
}
//设置3D环绕声
//depth:0~15(3D强度,0最弱,15最强)
void WM8978_3D_Set(uint8_t depth)
{
	depth&=0XF;//限定范围
 	WM8978_Write_Reg(41,depth);	//R41,3D环绕设置
}
//设置EQ/3D作用方向
//dir:0,在ADC起作用
//    1,在DAC起作用(默认)
void WM8978_EQ_3D_Dir(uint8_t dir)
{
	uint16_t regval;
	regval=WM8978_Read_Reg(0X12);
	if(dir)regval|=1<<8;
	else regval&=~(1<<8);
	WM8978_Write_Reg(18,regval);//R18,EQ1的第9位控制EQ/3D方向
}

//设置EQ1
//cfreq:截止频率,0~3,分别对应:80/105/135/175Hz
//gain:增益,0~24,对应-12~+12dB
void WM8978_EQ1_Set(uint8_t cfreq,uint8_t gain)
{
	uint16_t regval;
	cfreq&=0X3;//限定范围
	if(gain>24)gain=24;
	gain=24-gain;
	regval=WM8978_Read_Reg(18);
	regval&=0X100;
	regval|=cfreq<<5;	//设置截止频率
	regval|=gain;		//设置增益
	WM8978_Write_Reg(18,regval);//R18,EQ1设置
}
//设置EQ2
//cfreq:中心频率,0~3,分别对应:230/300/385/500Hz
//gain:增益,0~24,对应-12~+12dB
void WM8978_EQ2_Set(uint8_t cfreq,uint8_t gain)
{
	uint16_t regval=0;
	cfreq&=0X3;//限定范围
	if(gain>24)gain=24;
	gain=24-gain;
	regval|=cfreq<<5;	//设置截止频率
	regval|=gain;		//设置增益
	WM8978_Write_Reg(19,regval);//R19,EQ2设置
}
//设置EQ3
//cfreq:中心频率,0~3,分别对应:650/850/1100/1400Hz
//gain:增益,0~24,对应-12~+12dB
void WM8978_EQ3_Set(uint8_t cfreq,uint8_t gain)
{
	uint16_t regval=0;
	cfreq&=0X3;//限定范围
	if(gain>24)gain=24;
	gain=24-gain;
	regval|=cfreq<<5;	//设置截止频率
	regval|=gain;		//设置增益
	WM8978_Write_Reg(20,regval);//R20,EQ3设置
}
//设置EQ4
//cfreq:中心频率,0~3,分别对应:1800/2400/3200/4100Hz
//gain:增益,0~24,对应-12~+12dB
void WM8978_EQ4_Set(uint8_t cfreq,uint8_t gain)
{
	uint16_t regval=0;
	cfreq&=0X3;//限定范围
	if(gain>24)gain=24;
	gain=24-gain;
	regval|=cfreq<<5;	//设置截止频率
	regval|=gain;		//设置增益
	WM8978_Write_Reg(21,regval);//R21,EQ4设置
}
//设置EQ5
//cfreq:中心频率,0~3,分别对应:5300/6900/9000/11700Hz
//gain:增益,0~24,对应-12~+12dB
void WM8978_EQ5_Set(uint8_t cfreq,uint8_t gain)
{
	uint16_t regval=0;
	cfreq&=0X3;//限定范围
	if(gain>24)gain=24;
	gain=24-gain;
	regval|=cfreq<<5;	//设置截止频率
	regval|=gain;		//设置增益
	WM8978_Write_Reg(22,regval);//R22,EQ5设置
}

//WM8978初始化
//返回值:0,初始化正常
//    其他,错误代码
uint8_t WM8978_Init(void) {

    if (WM8978_Write_Reg(0, 0) != ESP_OK) {
		log_printf(WM8978_TAG, LOG_ERROR, "WM8978 init failed");
        return 1;
    }

    // General initialization sequence
    WM8978_Write_Reg(1, 0X1B);
    WM8978_Write_Reg(2, 0X1B0);
    WM8978_Write_Reg(3, 0X6C);
    WM8978_Write_Reg(6, 0);
    WM8978_Write_Reg(43, 1 << 4);
    WM8978_Write_Reg(47, 1 << 8);
    WM8978_Write_Reg(48, 1 << 8);
    WM8978_Write_Reg(49, 1 << 1);
    WM8978_Write_Reg(10, 1 << 3);
    WM8978_Write_Reg(14, 1 << 3);
	log_printf(WM8978_TAG, LOG_INFO, "WM8978 initialized successfully");
    return 0;
}









