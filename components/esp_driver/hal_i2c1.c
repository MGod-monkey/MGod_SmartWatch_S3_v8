#include "hal_i2c1.h"

static const char *HALI2C1_TAG = "Hal I2C1";
void myiic_delay_us(int us)
{
	uint32_t time=40*us;
	while(--time);

}

void myiic_delay_ms(int ms)
{
	myiic_delay_us(ms*1000);
}

//初始化IIC
void IIC_Init(void)
{
    //gpio_pad_select_gpio(IIC_SCL_IO);
    //gpio_pad_select_gpio(IIC_SDA_IO);

	gpio_set_direction(IIC_SDA_IO, GPIO_MODE_OUTPUT);
	gpio_set_direction(IIC_SCL_IO, GPIO_MODE_OUTPUT);
	IIC_SCL(1);
	IIC_SDA(1);

}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA(1);
	IIC_SCL(1);
	myiic_delay_us(4);
    IIC_SDA(0);//START:when CLK is high,DATA change form high to low
	myiic_delay_us(4);
	IIC_SCL(0);//钳住IIC总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL(0);
	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
	myiic_delay_us(4);
	IIC_SCL(1);
	IIC_SDA(1);//发送IIC总线结束信号
	myiic_delay_us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint16_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入
	IIC_SDA(1);
	myiic_delay_us(1);
	IIC_SCL(1);
	myiic_delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>2500)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);//时钟输出0
	return 0;
}
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);
	myiic_delay_us(2);
	IIC_SCL(1);
	myiic_delay_us(2);
	IIC_SCL(0);
}
//不产生ACK应答
void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	myiic_delay_us(2);
	IIC_SCL(1);
	myiic_delay_us(2);
	IIC_SCL(0);
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
	SDA_OUT();
    IIC_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        IIC_SDA((txd&0x80)>>7);
        txd<<=1;
		myiic_delay_us(1);   //对TEA5767这三个延时都是必须的
		IIC_SCL(1);
		myiic_delay_us(1);
		IIC_SCL(0);
		myiic_delay_us(1);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t IIC_Read_Byte(uint8_t ack)
{
	uint8_t i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL(0);
        myiic_delay_us(1);
		IIC_SCL(1);
        receive<<=1;
        if(READ_SDA)receive++;
		myiic_delay_us(1);
    }
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
    return receive;
}




uint8_t IIC_read_addr_byte(uint8_t device_addr,uint8_t read_addr)
{
	uint8_t dat;
	IIC_Start();
	IIC_Send_Byte(device_addr<<1);
	IIC_Send_Byte(read_addr);
	IIC_Stop();

	IIC_Start();
	IIC_Send_Byte((device_addr<<1) | 0x01);
	dat=IIC_Read_Byte(0);
	IIC_Stop();
	return(dat);
}

void IIC_read_addr_str(uint8_t device_addr,uint8_t read_addr,uint8_t read_amount,uint8_t *read_buf)
{
//	uchar dat;
	uint8_t i;
	IIC_Start();
	IIC_Send_Byte(device_addr<<1);
	IIC_Send_Byte(read_addr);
	IIC_Stop();

	IIC_Start();
	IIC_Send_Byte((device_addr<<1) | 0x01);

	for(i=0;i<read_amount-1;i++)
		read_buf[i] = IIC_Read_Byte(1);

	read_buf[i] = IIC_Read_Byte(0);
	IIC_Stop();
}


void IIC_write_addr_byte(uint8_t device_addr,uint8_t write_addr,uint8_t write_dat)
{
	IIC_Start();
	IIC_Send_Byte(device_addr<<1);
	IIC_Send_Byte(write_addr);
	IIC_Send_Byte(write_dat);
	IIC_Stop();
	myiic_delay_ms(2);
}

unsigned int IIC_read_addr_int(uint8_t device_addr,uint8_t read_addr)
{
	uint8_t read_buf[2];
	IIC_read_addr_str(device_addr,read_addr,2,read_buf);
	return (read_buf[0]<<8)|read_buf[1];
}

void IIC_write_addr_str(uint8_t device_addr,uint8_t write_addr,uint8_t write_amount,uint8_t *write_buf)
{
	uint8_t i;
	IIC_Start();
	IIC_Send_Byte(device_addr<<1);
	IIC_Send_Byte(write_addr);
	for(i=0;i<write_amount;i++)
		IIC_Send_Byte(write_buf[i]);
	IIC_Stop();
	myiic_delay_ms(2);
}
