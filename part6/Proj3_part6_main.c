/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Our Includes */
#include <HAL/I2C.h>
#include <HAL/Timer.h>
#include <HAL/Graphics.h>
#include <HAL/Button_withInterrupts.h>
#include <HAL/LED.h>
//----------------------------------------------------------------
//A special pre-processor directive goes here. You need to find out what that directive is.
//----------------------------------------------------------------

#define SLAVE_ADDRESS                                     0x48

volatile static bool trasmitFlag = false;
volatile static bool receiveFlag = false;
volatile static bool waitingFlag = false;


enum _project_state
{
    WAITING, TRANSMITTING, RECEIVING
};

typedef enum _project_state STATE;

typedef struct
{
    char character;
    int integer;

    char *string;
    //char string[9] = {9, 0, 5, 9, 8, 3, 7, 8, 2};

    //char *pid = "905983782";
} I2C_sendStruct;


void InitI2C() {
    //Testing Comment


    //GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN5 | GPIO_PIN4);
    //GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN5 | GPIO_PIN4);
    //Interrupt_enableInterrupt(INT_EUSCIB1);

    /* Enable and clear the interrupt flag */

    //I2C_clearInterruptFlag(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);
    //I2C_clearInterruptFlag(EUSCI_B1_BASE, EUSCI_B_I2C_NAK_INTERRUPT);
    //I2C_clearInterruptFlag(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    //Enable master Transmit interrupt

    //I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_NAK_INTERRUPT);
    //I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);
    //I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);
    //MAP_Interrupt_enableSleepOnIsrExit();

    //Interrupt_enableInterrupt(INT_EUSCIB1);





    //MAP_I2C_enableModule(EUSCI_B0_BASE);

    //EUSCIB2_IRQHandler();

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN5 | GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    eUSCI_I2C_MasterConfig I2CConfig;
    // Related to baudrate generation and sampling
    I2CConfig.selectClockSource =  EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    I2CConfig.i2cClk = 48000000;
    I2CConfig.dataRate = EUSCI_B_I2C_SET_DATA_RATE_100KBPS;
    I2CConfig.byteCounterThreshold = 0;        // UCBRS  = 0x55
    I2CConfig.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP; // Oversampling

    I2C_initMaster(EUSCI_B1_BASE, &I2CConfig);
    I2C_enableModule(EUSCI_B1_BASE);


}


void EUSCIB1_IRQHandler() {

    if (I2C_getEnabledInterruptStatus(EUSCI_B1_BASE) && EUSCI_B_I2C_TRANSMIT_INTERRUPT1){
        trasmitFlag = true;
    }

    I2C_clearInterruptFlag(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT1);


    if (I2C_getEnabledInterruptStatus(EUSCI_B1_BASE) && EUSCI_B_I2C_RECEIVE_INTERRUPT1){
        trasmitFlag = true;
    }

    I2C_clearInterruptFlag(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT1);


    if (I2C_getEnabledInterruptStatus(EUSCI_B1_BASE) && EUSCI_B_I2C_NAK_INTERRUPT){
        trasmitFlag = true;
    }

    I2C_clearInterruptFlag(EUSCI_B1_BASE, EUSCI_B_I2C_NAK_INTERRUPT);

}


void SendAStructToArduino(){

    char newArray[9] = {9, 0, 5, 9, 8, 3, 7, 8, 2};

    //I2C_sendStruct newStruct;


    //char *array2 = (char*)&newStruct;
    I2C_setSlaveAddress(EUSCI_B1_BASE, SLAVE_ADDRESS);



    I2C_sendStruct test1;
    test1.character = 'c';
    test1.string = newArray;
    test1.integer = 5;

    char *a = (char*)&test1;
    I2C_masterSendMultiByteStart(EUSCI_B1_BASE, newArray[0]);
    int i;
    //I2C_masterSendMultiByteNext(EUSCI_B1_BASE, a[1]);
    for(i = 1; i < 9 ; i++){
        I2C_masterSendMultiByteNext(EUSCI_B1_BASE, newArray[i]);
    }
    I2C_masterSendMultiByteNext(EUSCI_B1_BASE, test1.character);
    I2C_masterSendMultiByteFinish(EUSCI_B1_BASE, test1.integer);
    //printf("The char was: %d  \n",*b);

}


void sleep() {
    // The Launchpad RED LED is used to signify the processor is in low-power mode.
    // From the human perspective, it should seem the processor is always asleep except for fractions of second here and there.

    TurnOn_LL1();
    // Enters the Low Power Mode 0 - the processor is asleep and only responds to interrupts
    PCM_gotoLPM0();
    TurnOff_LL1();

}




int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    /* Initialize the system timing */
    InitSystemTiming();

    initButtons();//initializes the buttons
    initLEDs();   //initializes the LEDs

    while(1)
    {
       sleep(); //force the processor into sleep mode (only waking for interrupts)

       buttons_t buttons = updateButtons();//update the buttons pressings

       //===============================================================
       //TODO: Your code goes here
       //===============================================================
    }




}

