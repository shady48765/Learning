#include <main.h>

#define Nop() udelay(1) //一个指令周期（1uS）
#define	SlaveAddress	0x70	//IIC写入时的地址字节数据，+1为读取
#define CUBE_ON 	0xFF
#define CUBE_OFF	0x00
#define KEY_VAL_REG		0x01
u8_t ack;

/*******************************************************************
 * 函数原型: void start_iic()
 * 功能:     启动I2C总线,即发送I2C起始条件.
 * ********************************************************************/
void start_iic()
{
	gpio_direction_output(S5P6818_GPIOD(6),1); //SCL OUTPUT 1
	Nop();
	gpio_direction_output(S5P6818_GPIOD(7),1); //SDA OUTPUT 1

	Nop(); //其实条件建立时间大于4.7us，延时
	Nop();
	Nop();
	Nop();
	Nop();
	gpio_set_value(S5P6818_GPIOD(7),0); //SDA 0 发送起始信号
	Nop(); //起始条件大于4us
	Nop();
	Nop();
	Nop();
	Nop();
	gpio_set_value(S5P6818_GPIOD(6),0); //钳住I2C总线，准备发送或接收数据
	Nop();
	Nop();
}
/*******************************************************************
 * 函数原型: void stop_iic()
 * 功能:     结束I2C总线,即发送I2C结束条件.
 * ********************************************************************/
void stop_iic()
{
	gpio_direction_output(S5P6818_GPIOD(7),0); //SDA OUTPUT 0
	Nop();
	gpio_direction_output(S5P6818_GPIOD(6),1); //SCL OUTPUT 1

	Nop();   //结束条件建立时间大于4μs
	Nop();
	Nop();
	Nop();
	Nop();
	gpio_direction_output(S5P6818_GPIOD(7),1); //SDA OUTPUT 1   发送I2C总线结束信号
	Nop();
	Nop();
	Nop();
	Nop();
}
/******************************************************************
 * 函数原型:void send_byte(u8_t c)
 * 功能: 将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
 * 此状态位进行操作.(不应答或非应答都使ack=0 假)
 * 发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
void send_byte(u8_t c)
{

	u8_t BitCnt;

	Nop();
	for(BitCnt=0;BitCnt<8;BitCnt++) //要传送的数据长度为8位
	{
		if(c&0x80)
			gpio_direction_output(S5P6818_GPIOD(7),1);   //判断发送位
		else
			gpio_direction_output(S5P6818_GPIOD(7),0);
		c<<=1;
		Nop();
		gpio_direction_output(S5P6818_GPIOD(6),1);//置时钟线为高，通知被控器开始接收数据位
		Nop();
		Nop();//保证时钟高电平周期大于4μs*/
		Nop();
		Nop();
		Nop();
		gpio_direction_output(S5P6818_GPIOD(6),0);
	}
	Nop();
	Nop();
	gpio_direction_output(S5P6818_GPIOD(7),1);//8位发送完后释放数据线，准备接收应答位
	Nop();
	Nop();
	gpio_direction_output(S5P6818_GPIOD(6),1);
	gpio_direction_input(S5P6818_GPIOD(7));
	Nop();
	Nop();
	Nop();
if(gpio_get_value(S5P6818_GPIOD(7))==1)
	ack=0;
else
	ack=1;//判断是否接收到应答信号
	gpio_direction_output(S5P6818_GPIOD(6),0);
	Nop();
	Nop();
}
/******************************************************************
 * 函数原型: u8_t recv_byte()
 * 功能: 用来接收从器件传来的数据,并判断总线错误(不发应答信号)
 * 发完后请用应答函数。
********************************************************************/
u8_t recv_byte()
{
	u8_t retc = 0;
	u8_t BitCnt;

	gpio_direction_output(S5P6818_GPIOD(7),1);
	gpio_direction_input(S5P6818_GPIOD(7));//置数据线为输入方式
	Nop();
	for(BitCnt=0;BitCnt<8;BitCnt++){
		Nop();
		gpio_direction_output(S5P6818_GPIOD(6),0);//置时钟线为低，准备接收数据位
		Nop();
		Nop();//时钟低电平周期大于4.7μs
		Nop();
		Nop();
		Nop();
		gpio_direction_output(S5P6818_GPIOD(6),1);//置时钟线为高使数据线上数据有效
		Nop();
		Nop();
		retc=retc<<1;
		if(gpio_get_value(S5P6818_GPIOD(7))==1)
			retc=retc+1;//读数据位,接收的数据位放入retc中
		Nop();
		Nop();
	}
	gpio_direction_output(S5P6818_GPIOD(6),0);
	Nop();
	Nop();
	return(retc);
}
/********************************************************************
 * 原型:void ack_iic(u8_t a)
 * 功能：主控器进行应答信号,(可以是应答或非应答信号)
 * 输入：为0应答；为1非应答
********************************************************************/
void ack_iic(u8_t a)
{

	Nop();
	if(a==0)
		gpio_direction_output(S5P6818_GPIOD(7),0);//在此发出应答或非应答信号
	else
		gpio_direction_output(S5P6818_GPIOD(7),1);
	Nop();
	Nop();
	Nop();
	gpio_direction_output(S5P6818_GPIOD(6),1);
	Nop();
	Nop();//时钟低电平周期大于4μs
	Nop();
	Nop();
	Nop();
	gpio_direction_output(S5P6818_GPIOD(6),0);//清时钟线，钳住I2C总线以便继续接收
	Nop();
	Nop();
}
/*******************************************************************
 * 函数原型: u8_t isend_str(uint8 sla,uint8 *s,uint8 no)
 * 功能:从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
 * 地址sla发送内容是s指向的内容，发送no个字节。
 * 如果返回1表示操作成功，否则操作有误
 * 输入 sla-从器件地址；s-要发送的数据；no-要发送的数据的个数注意
 * 1、使用前必须已结束总线
 * 2、如果从器件的字地址为单字节则发送的数据中第一个数据为子地址；如果子地址为双字节则前两个数据为子地址。
********************************************************************/
u8_t isend_str(u8_t sla,u8_t *s,u8_t no)
{
	u8_t i;

	start_iic();//启动总线
	send_byte(sla);//发送器件地址
	if(ack==0){
		serial_printf(0, "ackisend %02x\r\n",ack );
		return(0);
	}
	for(i=0;i<no;i++){
		send_byte(*s);//发送数据
		if(ack==0)
			return(0);
		s++;
	}
	stop_iic();//结束总线
	return(1);
}
/*******************************************************************
//函数原型:u8_t irecv_str(u8_t sla,u8_t *suba,u8_t *s,u8_t subano,u8_t no)
//功能:从启动总线到发送地址，子地址,读数据，结束总线的全过程,从器件
//地址sla,子地址suba，读出的内容放入s指向的存储区，读no个字节
//如果返回1表示操作成功，否则操作有误
//输入sla-从器件地址
//suba-子地址所指的地址
//s-返回数据的地址
//subano-子地址的字节个数
//no要读取的数据个
//注意
//1、使用前必须已结束总线
********************************************************************/
u8_t irecv_str(u8_t sla,u8_t *suba,u8_t *s,u8_t subano,u8_t no)
{
	u8_t i;
	start_iic();//启动总线
	send_byte(sla);//发送器件地址
	if(ack==0){
		serial_printf(0, "ackirecv1 %02x\r\n",ack );
		return(0);
	}
	for(i=0;i<subano;i++){
		send_byte(*suba);//发送器件子地址
		if(ack==0){
			serial_printf(0, "ackirecv2 %02x\r\n",ack );
			return(0);
		}
		suba++;
	}
	start_iic();
	send_byte(sla+1);
	if(ack==0){
		serial_printf(0, "ackirecv3 %02x\r\n",ack );
		return(0);
	}
	for(i=0;i<no-1;i++){
		*s=recv_byte();//发送数据
		ack_iic(0);//发送就答位
		s++;
	}
	*s=recv_byte();
	ack_iic(1);//发送非应位
	stop_iic();//结束总线
	return(1);
}
unsigned char switch_value(unsigned char num)
{

	unsigned char cube_val;
	switch (num) {
		case 0x01: //key =0, 7led=0xfc
			cube_val = 0xfc;
			break;
		case 0x02: //key = 1,7led=0x0c
			cube_val = 0x60;
			break;
		case 0x03: //key = 2,7led=0xda
			cube_val = 0xda;
			break;
		case 0x04: //key = 3,7led=0xf2
			cube_val = 0xf2;
			break;
		case 0x09: //key = 4,7led=0x66
			cube_val = 0x66;
			break;
		case 0x0a: //key = 5,7led=0xb6
			cube_val = 0xb6;
			break;
		case 0x0b: //key = 6,7led=0xbe
			cube_val = 0xbe;
			break;
		case 0x0c: //key = 7,7led=0xe0
			cube_val = 0xe0;
			break;
		case 0x11: //key = 8,7led=0xfe
			cube_val = 0xfe;
			break;
		case 0x12: //key = 9,7led=0xf6
			cube_val = 0xf6;
			break;
		case 0x13: //key = A,7led=0xEE
			cube_val = 0xee;
			break;
		case 0x14: //key = b,7led=0x3E
			cube_val = 0x3e;
			break;
		case 0x19: //key = C,7led=0x9C
			cube_val = 0x9c;
			break;
		case 0x1a: //key = d,7led=0x9A
			cube_val = 0x7a;
			break;
		case 0x1b: //key = e,7led=0xDE
			cube_val = 0x9e;
			break;
		case 0x1c: //key = F,7led=0x8E
			cube_val = 0x8e;
			break;
		default:
			break;
	}
	return cube_val;
}

void set_7led_value(u8_t addr, u8_t cube_val)
{
	u8_t status = 0x00;
	u8_t s[2] = {addr,cube_val};

	irecv_str(SlaveAddress,&addr,&status,1,1);

	while(status != cube_val) {
		mdelay(1);
		isend_str(SlaveAddress,s,2);
		irecv_str(SlaveAddress,&addr,&status,1,1);

	}
}
/*
 * 通过读取ZLG7290存放键值的寄存器获得当前按下的键值
 *
 * */
unsigned char get_key_value(unsigned char addr)
{
	unsigned char value = 0;
	while(!value) {
		mdelay(1);
		irecv_str(SlaveAddress,&addr,&value,1,1);
	}

	if (value != 0)
		return value;

	return -1;
}
/*-------------------------MAIN FUNCTION------------------------------*/
/**********************************************************************
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int tester_i2c(int argc, char * argv[])
{
	int  i ;
	unsigned char reg[8]; //存放数码管寄存器地址
	unsigned char cube_val; //存放转化后数码管
	unsigned char key_val;


	//配置引脚为GPIO模式
	gpio_set_cfg(S5P6818_GPIOD(6), 0x0);        //GPIOD6, I2C_2_SCL
	gpio_set_cfg(S5P6818_GPIOD(7), 0x0);		//GPIOD7, I2c_2_SDA
	mdelay(100);

	/*
	 * 初始化数码管寄存器地址
	 * */
	for (i = 0; i < 8; i++)
		reg[i]= 0x10 + i;

	//switch_value(buf,cube_val);

	//点亮所有的数码管
	for(i = 0; i < 8; i++)
		set_7led_value(reg[i], CUBE_ON);
	mdelay(500);
	//关闭所有的数码管
	for(i = 0; i < 8; i++)
		set_7led_value(reg[i], CUBE_OFF);

	while(1){
		key_val = get_key_value(KEY_VAL_REG);
		cube_val = switch_value(key_val);
		for(i = 0; i < 8; i++)
			set_7led_value(reg[i], cube_val);
	}


	return 0;
}
