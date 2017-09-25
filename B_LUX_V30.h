

#ifndef B_LUX_V30_h  
#define B_LUX_V30_h  

#if ARDUINO < 100  
#include <WProgram.h>  
#else  
#include <Arduino.h>  
#endif  

#define DHT_LIB_VERSION "0.1.01"  
#define	B_LUX_SlaveAddress	  0x94    
class B_LUX_V30
{
public:
	float read();
	B_LUX_V30(uint8_t _sda, uint8_t _scl);
private:

	uint8_t B_LUX_RecvByte();
	void B_LUX_SendByte(uint8_t dat);
	void B_LUX_SendACK(uint8_t ack);
	void B_LUX_Start();
	void B_LUX_Stop();
	uint8_t BUF_0[6];
	uint32_t val32;
	uint8_t SDA;
	uint8_t SCL;
};
#endif  
