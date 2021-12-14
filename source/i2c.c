
/**
 * @file    i2c.c
 * @brief   This source file consists of functions definitions of I2C protocol
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 * @References
 *1) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8
 */

#include <MKL25Z4.H>
#include "i2c.h"

#define DETECT_TIME 200
int lock_detect=0;
int i2c_lock=0;

/*
 * @brief Initialize I2C protocol
 *
 * @return void
 */
void i2c_init(void)
{

	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;				/*Set clock for port E and I2C peripheral*/
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);
	PORTE->PCR[24] |= PORT_PCR_MUX(5);
	PORTE->PCR[25] |= PORT_PCR_MUX(5);					/*setting the  pins to I2C function*/
 	I2C0->F = (I2C_F_ICR(0x10) | I2C_F_MULT(0));		/*Set 100k baud rate*/
	I2C0->C1 |= (I2C_C1_IICEN_MASK);			  		/*setting to master mode*/
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
}


/*
 * @brief Transmission of the I2C packet
 *
 * @return void
 */
void i2c_busy(void)
{

	lock_detect=0;							/* Starting the Signal */
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C_TRAN;
	I2C_M_START;
	I2C0->C1 |=  I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK; 			/* setting it in MASTER mode */
	I2C0->C1 |= I2C_C1_TX_MASK; 			/* Set transmit (TX) mode */
	I2C0->D = 0xFF;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0U) 	/* wait till  interrupt is triggered */
	{

	}
	I2C0->S |= I2C_S_IICIF_MASK; /* clear interrupt bit */
	I2C0->S |= I2C_S_ARBL_MASK; /* Clear arbitration error flag*/
	I2C0->C1 &= ~I2C_C1_IICEN_MASK; /* Send start */
	I2C0->C1 |= I2C_C1_TX_MASK; /* Set transmit (TX) mode */
	I2C0->C1 |= I2C_C1_MST_MASK; /* START signal generated */
	I2C0->C1 |= I2C_C1_IICEN_MASK;	/*Wait until start is send*/

	I2C0->C1 &= ~I2C_C1_IICEN_MASK; /* Send stop */
	I2C0->C1 |= I2C_C1_MST_MASK;	/* set SLAVE mode */
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_TX_MASK; /* Set Rx mode*/
	I2C0->C1 |= I2C_C1_IICEN_MASK;

	I2C0->S |= I2C_S_IICIF_MASK; 	/* wait */
	I2C0->S |= I2C_S_ARBL_MASK; /* Clear arbitration error & interrupt flag*/
	lock_detect=0;
	i2c_lock=1;
}


/*
 * @brief Wait for the i2c packet to be received
 *
 * @return void
 */

void i2c_wait(void)
{
	lock_detect = 0;
	while(((I2C0->S & I2C_S_IICIF_MASK)==0) & (lock_detect < DETECT_TIME))
	{
		lock_detect++;
	}
	if (lock_detect >= DETECT_TIME)
		i2c_busy();
	I2C0->S |= I2C_S_IICIF_MASK;
}

/*
 * @brief send i2c start sequence
 *
 * @return void
 */

void i2c_start()
{
	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;						/*send start	*/
}


/*
 * @brief Initializing the I2C read function
 * @param1 dev Device Address
 * @param2 address Read Address
 * @return void
 */

void i2c_read_setup(uint8_t dev, uint8_t address)
{
	I2C0->D = dev;			 /*send dev address	*/
	I2C_WAIT				/*wait for completion */
	I2C0->D =  address;		/*send read address	*/
	I2C_WAIT				/*wait for completion */
	I2C_M_RSTART;		    /*repeated start */
	I2C0->D = (dev|0x1);	 /*send dev address (read)	*/
	I2C_WAIT				/*wait for completion */

	I2C_REC;				/*set to receive mode */

}

/*
 * @brief Do I2C repeated read
 * @param1 The last read data
 * @return void
 */

uint8_t i2c_repeated_read(uint8_t isLastRead)
{
	uint8_t data;
	lock_detect = 0;
	if(isLastRead)
	{
		NACK;							/*set NACK after read	*/
	}
	else
	{
		ACK;							/*ACK after read	*/
	}
	data = I2C0->D;						/*dummy read	*/
	I2C_WAIT							/*wait for completion */
	if(isLastRead)
	{
		I2C_M_STOP;					/*send stop	*/
	}
	data = I2C0->D;				/*read data	*/

	return  data;
}


/*
 * @brief Read the I2C read byte
 * @param1 device address
 * @param2 read address
 * @return void
 */

uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
	uint8_t data;

	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
	I2C0->D = dev;			  /*send dev address	*/
	I2C_WAIT							/*wait for completion */

	I2C0->D =  address;		/*send read address	*/
	I2C_WAIT							/*wait for completion */

	I2C_M_RSTART;				   /*repeated start */
	I2C0->D = (dev|0x1);	 /*send dev address (read)	*/
	I2C_WAIT							 /*wait for completion */

	I2C_REC;						   /*set to recieve mode */
	NACK;									 /*set NACK after read	*/

	data = I2C0->D;					/*dummy read	*/
	I2C_WAIT								/*wait for completion */

	I2C_M_STOP;							/*send stop	*/
	data = I2C0->D;					/*read data	*/

	return data;
}



/*
 * @brief Wrtie the I2C byte
 * @param1 device address
 * @param2 read address
 * @param3 data
 * @return void
 */

void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{

	I2C_TRAN;				 /*set to transmit mode */
	I2C_M_START;			  /*send start	*/
	I2C0->D = dev;			  /*send dev address	*/
	I2C_WAIT				/*wait for ack */

	I2C0->D =  address;		/*send write address	*/
	I2C_WAIT

	I2C0->D = data;			/*send data	*/
	I2C_WAIT
	I2C_M_STOP;

}
