
#include "B_LUX_V30.h"  

#define TIMEOUT 10000  



B_LUX_V30::B_LUX_V30(uint8_t _sda, uint8_t _scl)
{
	SDA = _sda;
	SCL = _scl;
	pinMode(SCL, OUTPUT);
	pinMode(SDA, OUTPUT);
	val32 = 0;
}

float B_LUX_V30::read()
{
	//---------------------------读程序
	B_LUX_Start();
	B_LUX_SendByte(B_LUX_SlaveAddress + 0);         //发送设备地址+读信号
	B_LUX_SendByte(0x00);

	B_LUX_Start();
	B_LUX_SendByte(B_LUX_SlaveAddress + 1);         //发送设备地址+读信号
	delay(50);										//延时
	for (uint8_t i = 0; i <= 4; i++)                        //连续读取6个地址数据，存储中BUF
	{
		BUF_0[i] = B_LUX_RecvByte();                //BUF[0]存储0x32地址中的数据
		if (i == 4)
		{
			B_LUX_SendACK(1);                       //最后一个数据需要回NOACK
		}
		else
		{
			B_LUX_SendACK(0);                       //回应ACK
		}
	}
	B_LUX_Stop();									//停止信号

	val32 = BUF_0[3];
	val32 <<= 8;
	val32 |= BUF_0[2];
	val32 <<= 8;
	val32 |= BUF_0[1];
	val32 <<= 8;
	val32 |= BUF_0[0];
	//0x00 ~0x03为光照值存储寄存器，存储32位光照值, 光照值精确到小数后3位。光照值 = 寄存器值 / 1000，例：267856 / 1000 = 267.856(实际光照值)
	return ((float)val32*1.4) / 1000;//*1.4半球矫正值  
}

/*---------------------------------------------------------------------
功能描述: 向IIC总线发送一个字节数据
参数说明: dat - 写字节
函数返回: 无
---------------------------------------------------------------------*/
void B_LUX_V30::B_LUX_SendByte(uint8_t dat)
{
	pinMode(SDA, OUTPUT);
	pinMode(SCL, OUTPUT);
	for (uint8_t i = 0; i < 8; i++)         			//8位计数器
	{
		if (dat & 0x80)	digitalWrite(SDA, HIGH);
		else			digitalWrite(SDA, LOW);         //送数据口 
		delay(5);            				//延时
		digitalWrite(SCL, HIGH);       		//拉高时钟线
		delay(5);             				//延时
		digitalWrite(SCL, LOW);           	//拉低时钟线
		delay(5);				      		//延时
		dat <<= 1;              			//移出数据的最高位
	}

	digitalWrite(SDA, HIGH);
	pinMode(SDA, INPUT);
	digitalWrite(SCL, HIGH);                //拉高时钟线
	delay(5);								//延时
	digitalRead(SDA);				        //读应答信号
	delay(5);								//延时
	digitalWrite(SCL, LOW);                 //拉低时钟线
	pinMode(SDA, OUTPUT);
}


/*---------------------------------------------------------------------
功能描述: 从IIC总线接收一个字节数据
参数说明: 无
函数返回: 接收字节
---------------------------------------------------------------------*/
uint8_t  B_LUX_V30::B_LUX_RecvByte()
{
	uint8_t dat = 0;
	pinMode(SDA, INPUT);
	digitalWrite(SDA, HIGH);                    //使能内部上拉,准备读取数据,
	for (uint8_t i = 0; i < 8; i++)         	        //8位计数器
	{
		dat <<= 1;
		digitalWrite(SCL, HIGH);                //拉高时钟线
		delay(5);             					//延时
		dat |= digitalRead(SDA);				//读数据               
		digitalWrite(SCL, LOW);                 //拉低时钟线
		delay(5);             					//延时 
	}
	pinMode(SDA, OUTPUT);
	return dat;
}


/*---------------------------------------------------------------------
功能描述: 发送应答信号
参数说明: ack - 应答信号(0:ACK 1:NAK)
函数返回: 无
---------------------------------------------------------------------*/
void B_LUX_V30::B_LUX_SendACK(uint8_t ack)
{
	if (ack & 0x01)	digitalWrite(SDA, HIGH);		//写应答信号
	else			digitalWrite(SDA, LOW);
	digitalWrite(SCL, HIGH);                        //拉高时钟线
	delay(5);										//延时
	digitalWrite(SCL, LOW);                         //拉低时钟线
	digitalWrite(SDA, HIGH);
	delay(5);										//延时
}


/*---------------------------------------------------------------------
功能描述: 起始信号
参数说明: 无
函数返回: 无
---------------------------------------------------------------------*/
void B_LUX_V30::B_LUX_Start()
{
	digitalWrite(SDA, HIGH);                         //拉高数据线
	digitalWrite(SCL, HIGH);                         //拉高时钟线
	delay(5);										//延时
	digitalWrite(SDA, LOW);                         //产生下降沿
	delay(5);										//延时
	digitalWrite(SCL, LOW);                         //拉低时钟线
}

/*---------------------------------------------------------------------
功能描述: 停止信号
参数说明: 无
函数返回: 无
---------------------------------------------------------------------*/
void  B_LUX_V30::B_LUX_Stop()
{
	digitalWrite(SDA, LOW);                         //拉低数据线
	digitalWrite(SCL, HIGH);                        //拉高时钟线
	delay(5);										//延时
	digitalWrite(SDA, HIGH);                        //产生上升沿
	delay(5);										//延时
	digitalWrite(SCL, LOW);
	delay(180);
}