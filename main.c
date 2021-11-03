/* 
 * File:   main.c
 * Author: james
 *
 * Created on October 14, 2021, 7:53 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include "canlib/can.h"
#include "canlib/can_common.h"
#include "canlib/pic18f26k83/pic18f26k83_can.h"
#include "canlib/message_types.h"
#include "timer.h"
#include "canlib/util/timing_util.h"
#include "canlib/util/can_tx_buffer.h"



#include "config.h"
#include <xc.h>
#define _XTAL_FREQ 1000000



#define LED_1_ON() (LATB4 = 1)
#define LED_2_ON() (LATB3 = 1)
#define LED_1_OFF() (LATB4 = 0)
#define LED_2_OFF() (LATB3 = 0)

uint8_t tx_pool[500];
static void can_msg_handler(const can_msg_t *msg);
static void send_status_led_on(void);
void led_on_off();
void init(){
    CANRXPPS = 0x11;    // CAN receive pin = RC1
    RC0PPS = 0x33;      // CAN transmit pin = RC0

    // init gpio pins
    // tx
    TRISC0 = 0; // out
    RC0PPS = 0x33;
    ANSELC0 = 0;
    // rx
    TRISC1 = 1; // in
    ANSELC1 = 0;
    
    TRISC2 = 0;     
    LATC2 = 1;      
    
    TRISC3 = 0;    
    LATC3 = 1;     
    
    TRISC4 = 0;    
    LATC4 = 1;      
}
/*
 * 
 */
int main(int argc, char** argv) {
    init();
    can_timing_t can_setup;
    can_generate_timing_params(_XTAL_FREQ, &can_setup);
  
    can_init(&can_setup, can_msg_handler);
    txb_init(tx_pool, sizeof(tx_pool), can_send, can_send_rdy);

    while(1) {
        send_status_led_on();
    }   
    return (EXIT_SUCCESS);
}
void led_on_of(){
    LED_1_ON();
    LED_2_ON();
    __delay_ms(1000);
    LED_1_OFF();
    LED_2_OFF();
}
static void can_msg_handler(const can_msg_t *msg){
    uint16_t msg_type = get_message_type(msg);
    switch(msg_type){
        case MSG_GENERAL_CMD:
            led_on_off();
    }
}   

static void send_status_led_on(void){
    can_msg_t general_cmd_msg;
    build_general_cmd_msg(millis(), 0, &general_cmd_msg);  
    txb_enqueue(&general_cmd_msg);
}

