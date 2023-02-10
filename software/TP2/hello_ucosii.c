/*************************************************************************
 * Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
 * All rights reserved. All use of this software and documentation is     *
 * subject to the License Agreement located at the end of this file below.*
 **************************************************************************
 * Description:                                                           *
 * The following is a simple hello world program running MicroC/OS-II.The *
 * purpose of the design is to be a very simple application that just     *
 * demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
 * for issues such as checking system call return codes. etc.             *
 *                                                                        *
 * Requirements:                                                          *
 *   -Supported Example Hardware Platforms                                *
 *     Standard                                                           *
 *     Full Featured                                                      *
 *     Low Cost                                                           *
 *   -Supported Development Boards                                        *
 *     Nios II Development Board, Stratix II Edition                      *
 *     Nios Development Board, Stratix Professional Edition               *
 *     Nios Development Board, Stratix Edition                            *
 *     Nios Development Board, Cyclone Edition                            *
 *   -System Library Settings                                             *
 *     RTOS Type - MicroC/OS-II                                           *
 *     Periodic System Timer                                              *
 *   -Know Issues                                                         *
 *     If this design is run on the ISS, terminal output will take several*
 *     minutes per iteration.                                             *
 **************************************************************************/


#include <stdio.h>
#include "includes.h"
#include <unistd.h>
#include <altera_avalon_pio_regs.h>
#include <system.h>
#include <stdlib.h>
#include <os_cfg.h>

#define true 1
#define false 0

#define bool unsigned int



// addresses of the different components
const int hex0 = 0x8005080;
const int hex1 = 0x8005070;
const int hex2 = 0x8005020;
const int hex3 = 0x8005040;
const int hex4 = 0x8005050;
const int hex5 = 0x8005060;

const int jtag_uart = 0x80050b0;
const int key = 0x8005030;
const int leds = 0x80050a0;
const int onchip_memory2_0 = 0x8002000;
const int sdram = 0x4000000;
const int switches = 0x8005090;
const int timer = 0x8005000;

// global variables
volatile bool wantToPlayAGame=false;
volatile bool stopGame=false;

volatile int nbTry=0;
volatile int totalTime=0;

volatile int time = 0;

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    task3_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define TASK3_PRIORITY      3

#define MSG_QUEUE_SIZE 20
OS_EVENT *msgQueue;
OS_EVENT *msgQueue_trigger;
void *msgQueueTbl [20];

OS_EVENT* mailBox1_2;
OS_EVENT* mailBox3_1;

int OSTmrCtr;
INT32U taskStartTimestamp;


typedef struct {
	char TaskName[30] ;
	INT16U TaskCtr ;
	INT16U TaskExecTime ;
	INT16U TaskTotExecTime;
} TASK_USER_DATA_HOME;

TASK_USER_DATA_HOME tasks[3];

volatile bool triggerAverage1_3=false;

static int cpt=0;

// util methods 
void divideNumberIntoArray(int number, int *array)
{
	int i = 0;
	int arrayLength = 6;
	while (number > 0)
	{
		array[arrayLength - 1 - i] = number % 10;
		number = number / 10;
		i++;
	}
}

int convertTo7segments(int input)
{
	switch (input)
	{
	case 0:
		return 0b1000000;
	case 1:
		return 0b1111001;
	case 2:
		return 0b0100100;
	case 3:
		return 0b0110000;
	case 4:
		return 0b0011001;
	case 5:
		return 0b0010010;
	case 6:
		return 0b0000010;
	case 7:
		return 0b1111000;
	case 8:
		return 0b0000000;
	case 9:
		return 0b0010000;
	case 10:
		return 0b0001000;
	case 11:
		return 0b0000011;
	case 12:
		return 0b1000110;
	case 13:
		return 0b0100001;
	case 14:
		return 0b0000110;
	case 15:
		return 0b0001110;
	default:
		return 0x00;
	}
}

void displayDecimalNumber(int number)
{
	int array[6] = {0, 0, 0, 0, 0, 0};
	divideNumberIntoArray(number, array);
	IOWR_ALTERA_AVALON_PIO_DATA(hex0, convertTo7segments(array[5]));
	IOWR_ALTERA_AVALON_PIO_DATA(hex1, convertTo7segments(array[4]));
	IOWR_ALTERA_AVALON_PIO_DATA(hex2, convertTo7segments(array[3]));
	IOWR_ALTERA_AVALON_PIO_DATA(hex3, convertTo7segments(array[2]));
	IOWR_ALTERA_AVALON_PIO_DATA(hex4, convertTo7segments(array[1]));
	IOWR_ALTERA_AVALON_PIO_DATA(hex5, convertTo7segments(array[0]));
}

// 7 segments display methods 
void displayNothing()
{
	IOWR_ALTERA_AVALON_PIO_DATA(hex0, 0b1111111);
	IOWR_ALTERA_AVALON_PIO_DATA(hex1, 0b1111111);
	IOWR_ALTERA_AVALON_PIO_DATA(hex2, 0b1111111);
	IOWR_ALTERA_AVALON_PIO_DATA(hex3, 0b1111111);
	IOWR_ALTERA_AVALON_PIO_DATA(hex4, 0b1111111);
	IOWR_ALTERA_AVALON_PIO_DATA(hex5, 0b1111111);
}

void display0(){
	displayDecimalNumber(0);
}

void displayBinaryNumber(int number)
{
	switch (number)
	{
	case 14:
		displayDecimalNumber(4);
		break;
	case 13:
		displayDecimalNumber(3);
		break;
	case 11:
		displayDecimalNumber(2);
		break;
	case 7:
		displayDecimalNumber(1);
		break;
	default:
		break;
	}
}


// leds
void ledOff()
{
	IOWR_ALTERA_AVALON_PIO_DATA(leds, 0b0000000000);
}

void ledOn()
{
	IOWR_ALTERA_AVALON_PIO_DATA(leds, 0b1111111111);
}

void oddLedsOn()
{
	IOWR_ALTERA_AVALON_PIO_DATA(leds, 0b0101010101);
}

void evenLedsOn()
{
	IOWR_ALTERA_AVALON_PIO_DATA(leds, 0b1010101010);
} 

// run exercices 
void runC2()
{
	displayNothing();

	usleep(1000000);

	for (int k = 0; k <= 9999; k++)
	{
		displayDecimalNumber(k);
		usleep(10000);
	}

	for (;;)
	{
		usleep(100000);
		displayDecimalNumber(9999);
		usleep(100000);
		displayNothing();
	}
}

// capture button presses 

volatile int edge_capture;
volatile float averageTime;

static void handle_button_interrupts(void *context, alt_u32 id)
{
	/* Cast context to edge_capture's type. It is important that this be
	 * declared volatile to avoid unwanted compiler optimization.
	 */
	volatile int *edge_capture_ptr = (volatile int *)context;

	/* Store the value in the Button's edge capture register in *context. */
	*edge_capture_ptr = IORD_ALTERA_AVALON_PIO_EDGE_CAP(key);

	/* Reset the Button's edge capture register. */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(key, 0);

	//if we push the first button, we start the game
	if(*edge_capture_ptr==1){
		wantToPlayAGame=true;
	}
	//if we push the second button, we show the average time since the beginning of the game
	else if(*edge_capture_ptr==2){
		averageTime=totalTime/nbTry;
		//displayDecimalNumber(averageTime);
		triggerAverage1_3=true;
	}

	/* Perform the button press handling code here. */
	//Nothing in particular for the 3rd button
	else if(*edge_capture_ptr==4){
		nbTry=0;
		totalTime=0;
	}

	//we push the fourth button to play the game
	else if(*edge_capture_ptr==8){
		stopGame=true;
	}
}

static void init_button_pio()
{
	/* Recast the edge_capture pointer to match the alt_irq_register() function
	 * prototype. */
	void *edge_capture_ptr = (void *)&edge_capture;

	/* Enable all 4 button interrupts. */
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(key, 0xf);

	/* Reset the edge capture register. */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(key, 0x0);

	/* Register the ISR */
	alt_irq_register(KEY_IRQ, edge_capture_ptr, handle_button_interrupts);
}


void play(){
	nbTry++;

	// wait for a random time between 1 and 5 seconds
	int randomTime = rand() % time + 1;
	printf("random time : %d\n", randomTime);
	for (int i = 0; i < randomTime; i++)
	{
		usleep(500000);
		evenLedsOn();
		usleep(500000);
		oddLedsOn();
	}

	
	INT32U end;
	INT32U start=OSTimeGet();
	ledOn();

	while (!stopGame)
	{
		// wait for the user to press the button
	}

	int end_time = OSTimeGet();

	// display the time
	int time = end - start;
	//printf("time : %d", time);
	totalTime += time;
	displayDecimalNumber(time);

	// clean variables
	stopGame=false;

	OSQPost(msgQueue, (void *)time);
}


/* Prints "Hello World" and sleeps for three seconds */
void task1(void* pdata)
{
	INT8U err;
	char message[30];
	char* message2 = "Hello";
	averageTime=0;
	while (1)
	{
		if(wantToPlayAGame){
			OSMboxPost(mailBox1_2, (void *)message);
			OSMboxPend(mailBox3_1, 0, &err);
			wantToPlayAGame=false;
			stopGame=false;
			triggerAverage1_3=false;
		}

		if (triggerAverage1_3)
		{
			OSQPost(msgQueue,(void *)&averageTime);
			OSMboxPend(mailBox3_1, 0, &err);
			triggerAverage1_3=false;
		}
		//sprintf(message, "%d", cpt);
		//cpt++;
		//OSMboxPost(mailBox1_2,(void *)message);
		//OSQPost(msgQueue,(void *)message);
		//OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
/* Prints "Hello World" and sleeps for three seconds */
void task2(void* pdata)
{
	INT8U err;
	int message;
	while (1)
	{
		OSMboxPend(mailBox1_2, 0, &err);
		play();
		wantToPlayAGame=false;

		//message = atoi(OSMboxPend(mailBox1_2,0,&err));
		//message = atoi(OSQPend(msgQueue,0,&err));
		//displayDecimalNumber(message);
		//OSTimeDlyHMSM(0, 0, 1, 0);
	}
}

void task3(void* pdata) {

	int* time;
	INT8U err;
	char* message="Hello";
	while (1) {
		time = (int*)OSQPend(msgQueue, 0, &err);
		displayDecimalNumber(*time);
		OSMboxPost(mailBox3_1, (void *)message);
	}

}
/* The main function creates two task and starts multi-tasking */
int main(void)
{

	averageTime=0;
	mailBox1_2=OSMboxCreate((void *)0);
	mailBox3_1=OSMboxCreate((void *)0);
	msgQueue = OSQCreate(&msgQueueTbl[0],MSG_QUEUE_SIZE);
	printf("Starting C2\n");

	displayNothing();
	ledOff();

	init_button_pio();

	int switchValue = IORD_ALTERA_AVALON_PIO_DATA(switches); // 10 switches returned as an int
	time = IORD_ALTERA_AVALON_PIO_DATA(switches) - 511;
	IOWR_ALTERA_AVALON_PIO_DATA(leds, switchValue);

	OSTaskCreateExt(task1,
			NULL,
			(void *)&task1_stk[TASK_STACKSIZE-1],
			TASK1_PRIORITY,
			TASK1_PRIORITY,
			task1_stk,
			TASK_STACKSIZE,
			NULL,
			0);


	OSTaskCreateExt(task2,
			NULL,
			(void *)&task2_stk[TASK_STACKSIZE-1],
			TASK2_PRIORITY,
			TASK2_PRIORITY,
			task2_stk,
			TASK_STACKSIZE,
			NULL,
			0);
	OSStart();
	return 0;
}

/******************************************************************************
 *                                                                             *
 * License Agreement                                                           *
 *                                                                             *
 * Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
 * All rights reserved.                                                        *
 *                                                                             *
 * Permission is hereby granted, free of charge, to any person obtaining a     *
 * copy of this software and associated documentation files (the "Software"),  *
 * to deal in the Software without restriction, including without limitation   *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
 * and/or sell copies of the Software, and to permit persons to whom the       *
 * Software is furnished to do so, subject to the following conditions:        *
 *                                                                             *
 * The above copyright notice and this permission notice shall be included in  *
 * all copies or substantial portions of the Software.                         *
 *                                                                             *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
 * DEALINGS IN THE SOFTWARE.                                                   *
 *                                                                             *
 * This agreement shall be governed in all respects by the laws of the State   *
 * of California and by the laws of the United States of America.              *
 * Altera does not recommend, suggest or require that this reference design    *
 * file be used in conjunction or combination with any other product.          *
 ******************************************************************************/
