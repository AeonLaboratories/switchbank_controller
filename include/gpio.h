///////////////////////////////////////////////////////
// gpio.h
//
// switchbank controller

#pragma once // Include this file only once

#include <ez8.h>
#include "..\\..\\common_controller\\include\\mask.h"

///////////////////////////////////////////////////////
//
// Conventions for configuring gpio pins
// 	(Note: keep DBG & -RESET reserved for flashing/debugging)
//
// UART uses PA4/RX, PA5/TX
//
// Preferred analog inputs:
//		PB0/ANA0 .. PB3/ANA3
//
// Preferred digital inputs
// 		PC0 .. PC3
//
// Preferred digital outputs (open-drain or push-pull possible)
// 		PA0, PA1, PA2, PA3, PA6, PA7
//
// Alternate/additional analog inputs:
//		PC0/ANA4 .. PC2/ANA6
//
// Alternate/additional digital outputs (push-pull only)
//		PC0/ANA4 .. PC2/ANA6
//		PB0/ANA0 .. PB3/ANA3
//
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//
// NOTE: Open drain mode is only available on Port A gpio pins
// (See "Errata for Z8 Encore XP F082A Series UP0069.pdf")
//
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// Port A
// PA7 = OUT: ADDR5
// PA6 = OUT: ADDR4
// PA5 = OUT: TXD0 (Alt. function)
// PA4 =  IN: RXD0 (Alt. function)
// PA3 = OUT: SIGNAL1 / -MR
// PA2 = OUT: ADDR_EN (-LE TO SELECTED CHANNEL)
// PA1 = OUT: SIGNAL2 / (DATA == ON/OFF STATE FOR SELECTED CHANNEL)
// PA0 = OUT:  no connect
//
// DD == data direction
// OC == output control
// AF == alternate function
//
// PADD		= 00010000		// 1 = IN; 0 = OUT (set unused pins to OUT)
// PAOC		= 00001000		// 1 = open drain; 0 = push-pull (the default)
// PAAF		= 00110000		// alternate functions
// PAOUT	= 00000000		// defaults
#define PA_DD					0x10
#define PA_OC					0x08
#define PA_AF					0x30
#define PA_OUT					0x00

#define ADDR_EN					0x04
#define ADDR_EN_low()			mask_clr(PAOUT, ADDR_EN)
#define ADDR_EN_high()			mask_set(PAOUT, ADDR_EN)

#define LATCH()					ADDR_EN_high()	// sets -LE for selected channel to low
#define UNLATCH()				ADDR_EN_low()	// sets -LE for all channels to high
#define STORE_SS()				LATCH(), UNLATCH()

// -MR, active low
#define MASTER_RESET			0x08
#define RESET_ON()				mask_clr(PAOUT, MASTER_RESET)
#define RESET_OFF()				mask_set(PAOUT, MASTER_RESET)

// D (DATA) pin, signal for desired switch state
#define SWITCH_STATE			0x02
#define SS_ON()					mask_set(PAOUT, SWITCH_STATE)
#define SS_OFF()				mask_clr(PAOUT, SWITCH_STATE)

///////////////////////////////////////////////////////
// Port B
// PB7 = N/A
// PB6 = N/A
// PB5 = N/A
// PB4 = N/A
// PB3 = OUT:  no connect
// PB2 =  IN:  ANA2 (Alt. function) = V
// PB1 =  IN:  ANA1 (Alt. function) = I
// PB0 = OUT:  no connect
//
// PBDD		= 00000110		// 1 = IN; 0 = OUT (set unused pins to OUT)
// PBAF		= 00000110		// alternate functions
// PBOUT	= 00000000		// defaults
//
// NOTE: open drain mode does not work for Port B gpio pins
// (See "Errata for Z8 Encore XP F082A Series UP0069.pdf")
//
#define PB_DD					0x06
#define PB_AF					0x06
#define PB_OUT					0x00

// voltage and current inputs are connected, but not used by software at this time


///////////////////////////////////////////////////////
// Port C
// PC7 = N/A
// PC6 = N/A
// PC5 = N/A
// PC4 = N/A
// PC3 = OUT: ADDR3
// PC2 = OUT: ADDR2
// PC1 = OUT: ADDR1
// PC0 = OUT: ADDR0
//
// PCDD		= 00000000		// 1 = IN; 0 = OUT (set unused pins to OUT)
// PCAF		= 00000000		// alternate functions
// PCOUT	= 00000000		// default values
//
// NOTE: open drain mode does not work for Port C gpio pins
// (See "Errata for Z8 Encore XP F082A Series UP0069.pdf")
//
#define PC_DD					0x00
#define PC_AF					0x00
#define PC_OUT					0x00

///////////////////////////////////////////////////////
// prototypes
//
void init_gpio(void);
