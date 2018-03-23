/*
 * LApp.h
 *
 *  Created on: Feb 5, 2018
 *      Author: mkaffine
 */

#ifndef SRC_LAPP_H_
#define SRC_LAPP_H_

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "ps7_init.h"
#include <xil_io.h>
#include <xil_exception.h>
#include "xscugic.h"
#include "xaxidma.h"
#include "xparameters.h"
#include "platform_config.h"
#include "xgpiops.h"
#include "xuartps.h"
#include "xil_printf.h"
#include "sleep.h"

///SD Card Includes
#include "xparameters.h"	// SDK generated parameters
#include "xsdps.h"			// SD device driver
#include "ff.h"
#include "xil_cache.h"

#include "xscutimer.h"

//Test TEC code
#include "xgpiops.h"

// IiC Interface
//#include "LI2C_Interface.h"
#include "xiicps.h"

/* Globals */
#define LOG_FILE_BUFF_SIZE	120
#define UART_DEVICEID		XPAR_XUARTPS_0_DEVICE_ID
#define SW_BREAK_GPIO		51
#define IIC_DEVICE_ID_0		XPAR_XIICPS_0_DEVICE_ID	//sensor head
#define IIC_DEVICE_ID_1		XPAR_XIICPS_1_DEVICE_ID	//thermometer/pot on digital board
#define TEST_BUFFER_SIZE	2
#define	TEC_PIN				18
#define SCLSH				10 //Added for testing purposes mkaffine 3/1/18
#define SDASH				11
#define WINDOW_START_TIME	200	//units of nanoseconds

// Hardware Interface
XUartPs Uart_PS;					// Instance of the UART Device
XGpioPs Gpio;						// Instance of the GPIO Driver Interface
XGpioPs_Config *GPIOConfigPtr;		// GPIO configuration pointer
static XScuGic_Config *GicConfig; 	// GicConfig
XScuGic InterruptController;		// Interrupt controller

// Timer Settings
#define TIMER_LOAD_VALUE 	0xFFFFFFFF
XScuTimer myTimer;

/* FAT File System Variables */
FATFS fatfs[2];
FRESULT ffs_res;
FILINFO fno;
FILINFO fnoDIR;
int doMount = 0;
char cZeroBuffer[] = "0000000000 ";
char cLogFile[] = "1:/LogFile.txt";	//Create a log file and file pointer
FIL logFile;
char filptr_buffer[11] = {};		// Holds 10 numbers and a null terminator
int filptr_clogFile = 0;
char cDirectoryLogFile0[] = "1:/DirectoryFile.txt";	//Directory File to hold all filenames
FIL directoryLogFile;
char filptr_cDIRFile_buffer[11] = {};
int filptr_cDIRFile = 0;

char cWriteToLogFile[LOG_FILE_BUFF_SIZE] = "";			//The buffer for adding information to the log file
char c_read_from_log_file[LOG_FILE_BUFF_SIZE] = "";
int iSprintfReturn = 0;
double dTime = 12345.67;
uint numBytesWritten = 0;
uint numBytesRead = 0;

int dirSize = 0;
char * dirFileContents;

/* UART Variables */
u8 uartTestBuffer[32];			//testing uart send GJS
static u8 SendBuffer[32];		// Buffer for Transmitting Data	// Used for RecvCommandPoll()
static u8 RecvBuffer[32];		// Buffer for Receiving Data

/* Menu Select Variable */
int	menusel = 99999;	// Menu Select

/* Set Mode Variables */
int mode = 9;			// Mode of Operation

/* Set Enable State Variables */
int enable_state = 0; 	// 0: disabled, 1: enabled

/* Set Threshold Variables */
int iThreshold0 = 0;	// Trigger Threshold
int iThreshold1 = 0;	// Beginning value of the trigger threshold

/* Set Integration Times Arrays */
char updateint = 'N';	// switch to change integral values
int setIntsArray[8] = {};
int setSamples[4] = {};
int setBL = 0;
int setSI = 0;
int setLI = 0;
int setFI = 0;

/* Check the size of the BRAM Buffer */
u32 databuff = 0;		// size of the data buffer

/* I2C Variables */
u8 i2c_Send_Buffer[TEST_BUFFER_SIZE];
u8 i2c_Recv_Buffer[2] = {};
int IIC_SLAVE_ADDR1 = 0x2F; //Hey Graham, I changed these - mkaffine // 2/21/2018
int IIC_SLAVE_ADDR2 = 0x4B;
int IIC_SLAVE_ADDR3 = 0x48;
int *IIC_SLAVE_ADDR;		//pointer to slave
int rdac = 0;
int data_bits = 0;
int a, b;					//used for bitwise operations
short cntrl = 0;
short pot_addr = 0;
unsigned short i2c_send_buff = 0;
unsigned short i2c_recv_buff = 0;

char c_data_file[] = "1:/data_test_001.txt";

// General Purpose Variables
int Status; 					// General purpose Status indicator
int sw = 0;  						// switch to stop and return to main menu  0= default.  1 = return
u32 global_frame_counter = 0;	// Counts ... for ...
int i = 0;						// Iterator in some places

// Methods
int InitializeAXIDma(void); 		// Initialize AXI DMA Transfer
int InitializeInterruptSystem(u16 deviceID);
void InterruptHandler (void );
int SetUpInterruptSystem(XScuGic *XScuGicInstancePtr);
int ReadCommandPoll();				// Read Command Poll Function
void SetIntegrationTimes();			// Set the Registers forIntegral Times
int PrintData();					// Print Data to the Terminal Window
void ClearBuffers();				// Clear Processeed Data Buffers
int DAQ();							// Clear Processeed Data Buffers
int ReadDataIn(int numfilesWritten, FIL * filObj);// Take data from DRAM, process it, save it to SD
int getWFDAQ();						// Print data skipping saving it to SD card
int LNumDigits(int number);			// Determine the number of digits in an int


#endif /* SRC_LAPP_H_ */
