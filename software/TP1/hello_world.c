/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <altera_avalon_pio_regs.h>
#include <system.h>

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



volatile bool waiting_reaction=false;
volatile bool end_time=false;
volatile bool start_game=false;

volatile float totalTime=0;
volatile int nbTry=0;

void separateNumber(int number, int *array)
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
  separateNumber(number, array);
  IOWR_ALTERA_AVALON_PIO_DATA(hex0, convertTo7segments(array[5]));
  IOWR_ALTERA_AVALON_PIO_DATA(hex1, convertTo7segments(array[4]));
  IOWR_ALTERA_AVALON_PIO_DATA(hex2, convertTo7segments(array[3]));
  IOWR_ALTERA_AVALON_PIO_DATA(hex3, convertTo7segments(array[2]));
  IOWR_ALTERA_AVALON_PIO_DATA(hex4, convertTo7segments(array[1]));
  IOWR_ALTERA_AVALON_PIO_DATA(hex5, convertTo7segments(array[0]));
}

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

void ledOff()
{
  IOWR_ALTERA_AVALON_PIO_DATA(leds, 0b0000000000);
}


// display a binary number as a decimal number on the 7 segments display
void displayBinaryNumber(int number)
{
  int value;
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

volatile int edge_capture;


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

  /* Perform the button press handling code here. */
  IOWR_ALTERA_AVALON_PIO_DATA(hex0, 0b1111111);
  displayDecimalNumber(edge_capture);
  printf("triggered %d", edge_capture);
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

}

int main()
{
  printf("Hello from Nios II!\n");

  displayNothing();
  ledOff();

  init_button_pio();
  


  for (;;)
  {
    // indiquer le num�ro du bouton press� sur l'afficheur 7 segments
    

    // les leds rouges sont allum�es si le switch en face est en position haute
    int switchValue = IORD_ALTERA_AVALON_PIO_DATA(switches); // 10 switches returned as an int
    IOWR_ALTERA_AVALON_PIO_DATA(leds, switchValue);

    // cette config des switch est utilis�e pour programmer le temps d'attente de la boucle principale (plus l'utilisateur l�ve le switch, plus le temps d'attente est long)
    usleep(10000);
  }
  return 0;
}
