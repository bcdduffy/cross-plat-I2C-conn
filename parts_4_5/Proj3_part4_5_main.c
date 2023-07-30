/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Our includes */
#include "HAL/Button_withInterrupt.h"
#include "HAL/Graphics.h"
#include "HAL/LED.h"
#include "HAL/Timer.h"

#pragma pack(1)

/* Our Defines */
#define SOURCE EUSCI_B1_BASE
#define ARRAY_SIZE 25


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


void sleep() {
    // The Launchpad LL1 is used to signify the processor is in low-power mode.
    // From the human perspective, it should seem the processor is always asleep except for fractions of second here and there.

    TurnOn_LL1();
    // Enters the Low Power Mode 0 - the processor is asleep and only responds to interrupts
    PCM_gotoLPM0();
    TurnOff_LL1();
}


// TODO write the body of this function
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


// TODO write the body of this function
void SendSingleByteToArduino(int8_t byte) {
    //Set default slave address to 0x48
    I2C_setSlaveAddress(EUSCI_B1_BASE, SLAVE_ADDRESS);

    I2C_masterSendSingleByte(EUSCI_B1_BASE, byte);
}

// TODO write the body of this function
char ReceiveSingleByteFromArduino() {
    I2C_setSlaveAddress(EUSCI_B1_BASE, SLAVE_ADDRESS);
    return I2C_masterReceiveSingleByte(EUSCI_B1_BASE);

}

// TODO: send the declared array to the Arduino
void SendAnArrayToArduino() {
    char array[ARRAY_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                              17, 18, 19, 20, 21, 22, 23, 24};

    I2C_setSlaveAddress(EUSCI_B1_BASE, SLAVE_ADDRESS);

    int index;
    I2C_masterSendMultiByteStart(EUSCI_B1_BASE, array[0]);
    for (index = 1; index < ARRAY_SIZE - 1; index++){
        I2C_masterSendMultiByteNext(EUSCI_B1_BASE, array[index]);
    }
    I2C_masterSendMultiByteFinish(EUSCI_B1_BASE, array[24]);

}

// TODO: Declare a struct and send it to the Arduino
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

int main(void)

{

    //I2C_initMaster(EUSCI_B1_BASE, I2CConfig);
    //I2C_initMaster();
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    /* Init the custom system timing... may cause problems */
    InitSystemTiming();

    InitI2C();

    initButtons();

    initLEDs();

    GFX gfx = GFX_construct(GRAPHICS_COLOR_BLACK, GRAPHICS_COLOR_WHITE);
    GFX_clear(&gfx);
    GFX_print(&gfx, "Proj 3 F21", 1, 1);
    GFX_print(&gfx, "Parts 4 and 5", 2, 1);

    // TODO: change this character to various values and see the effect on your Logic Analyzer output
    char TxByte = 'L';
    char RxByte;
    while (1)
    {
        sleep();
        buttons_t buttons = updateButtons();

        //_project_state newState;


        //Transmit one byte
        if (buttons.BB1tapped)
        {
            Toggle_LLB();  //comment out for debugging purpose
            SendSingleByteToArduino(TxByte);    // Write operation
        }

        //Receive one byte
        else if (buttons.BB2tapped)
        {
            Toggle_LLR(); //comment out for debugging purpose
            RxByte = ReceiveSingleByteFromArduino();   // Read operation
        }

        // Transmit an array
        else if (buttons.JSBtapped)
        {
            Toggle_LLG(); //comment out for debugging purpose
            SendAnArrayToArduino();
        }

        // Transmit an array
        else if (buttons.LB2tapped)
        {
            Toggle_LLG(); //comment out for debugging purpose
            SendAStructToArduino();
        }
    }
}
