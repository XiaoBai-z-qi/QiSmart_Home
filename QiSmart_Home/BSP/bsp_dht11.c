#include "bsp_dht11.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "sys_delay.h"
uint8_t Data[5]={0x00,0x00,0x00,0x00,0x00};   // 0-湿度 2-温度
void DHT_GPIO_SET_OUTPUT(void)     
{
	GPIO_InitTypeDef GPIO_InitStructure;    
	GPIO_InitStructure.Pin=DHT11_PIN;      
	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStructure.Pull=;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void DHT_GPIO_SET_INPUT(void)     
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin=DHT11_PIN;
	GPIO_InitStructure.Mode=GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
}

uint8_t DHT_Read_Byte(void)  
{
	 uint8_t ReadData=0;  
	 uint8_t temp;      
	 uint8_t retry=0;   
	 uint8_t i;    
	 for(i=0; i<8; i++)   //一次温湿度信号读取八位
	 {
			while(HAL_GPIO_ReadPin(GPIOA,DHT11_PIN)==0 && retry<100)  
			//等待直到DHT11输出高电平：当PA5=1，上升沿，表示开始接受数据，可以判断0 or 1，跳出循环，执行后续判断（若PA5=0，将一直循环等待）
			{
					Delay_us(1);
				  retry++;             //retry防止PA5读取不到数据卡死在这一步，当经历100us后retry自增到100，跳出循环。
			}
			retry=0;
			
			Delay_us(40);    
			if(HAL_GPIO_ReadPin(GPIOA,DHT11_PIN)==1)   temp=1;
			 else   temp=0;

			 while(HAL_GPIO_ReadPin(GPIOA,DHT11_PIN)==1 && retry<100)
			 {
				 Delay_us(1);
				 retry++;
			 }
			 retry=0;
			 
			 ReadData<<=1;    
			 ReadData |= temp;        
	 }

		return ReadData;
}

uint8_t DHT_Read(void)
{
	 uint8_t retry=0;
	 uint8_t i;
		
	 DHT_GPIO_SET_OUTPUT();    
	 HAL_GPIO_WritePin(GPIOA,DHT11_PIN,GPIO_PIN_RESET);   
	 vTaskDelay(pdMS_TO_TICKS(18));
	 HAL_GPIO_WritePin(GPIOA,DHT11_PIN,GPIO_PIN_SET);   
	 Delay_us(20);
	
	//MCU通过IO向DHT11发送请求完毕。接下来DHT11向IO发送响应,IO转为输入模式。在这之后就开始信号的转译读取。
	DHT_GPIO_SET_INPUT();
	Delay_us(20);
	if(HAL_GPIO_ReadPin(GPIOA,DHT11_PIN)==0) //DHT11发回低电平响应（读取到低电平，说明DHT11有响应）
	{
		//接下来，DHT11拉低电平一段时间后拉高电平一段时间
		while(HAL_GPIO_ReadPin(GPIOA,DHT11_PIN)==0 && retry<100)  
		{
		   Delay_us(1);
			 retry++;
		}
		retry=0;
		while(HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN)==1 && retry<100) 
		{
		   Delay_us(1);
			 retry++;
		}
		retry=0;
		for(i=0; i<5; i++)
		{
			 Data[i] = DHT_Read_Byte();  //每次读取一字节（8位）
		}
		Delay_us(50);
	}
	
	 uint32_t sum=Data[0]+Data[1]+Data[2]+Data[3];  //校验
	 if((sum)==Data[4])    return 1;  
	   else   return 0;
	
}
