///////////////////////////////////////////////////////
// irq.c
//
// Aeon Laboratories SB64 64-channel switchbank controller

#include <eZ8.h>
#include <defines.h>
#include "..\\..\\common_controller\\include\\c99types.h"
#include "..\\..\\common_controller\\include\\z082a.h"
#include "..\\..\\common_controller\\include\\timer.h"
#include "..\\..\\common_controller\\include\\adc.h"
#include "..\\..\\common_controller\\include\\uart.h"
#include "..\\..\\common_controller\\include\\irq.h"
#include "config.h"
#include "error.h"
#include "gpio.h"

// Store big strings in ROM to conserve RData and EData space.
rom char FIRMWARE[]	= R"Aeon Laboratories SB64 ";
rom char VERSION[]	= R"V.20171210-0000";

//#define SERNO					0

#define CHANNELS				64

///////////////////////////////////////////////////////
//
// global variables
//

int CommandedChannel;					// commanded channel

volatile uint8_t Channel;				// the selected channel
volatile BOOL State[(CHANNELS-1)];		// switch state

//////////////////////////////////////////////////////
//
// internal prototypes
//
///////////////////////////////////////////////////////

void reset_all()
{
	int i;
	
	RESET_ON();
	UNLATCH();
	RESET_OFF();

	for(i = 0; i < CHANNELS; State[++i] = 0);
}


///////////////////////////////////////////////////////
// set defaults
void init_irq()
{
	reset_all();
	Channel = CommandedChannel = 0;
	Error = ERROR_NONE;	
}



///////////////////////////////////////////////////////
void preset() { }


///////////////////////////////////////////////////////
void selectChannel(uint8_t ch)
{
	uint8_t ta, tc;		// workspaces for bit manipulations
	ta = PAIN & 0x3F;	// clear ADDR bits in PA (54xxxxxx)
	tc = PCIN & 0xF0;	// clear ADDR bits in PC (xxxx3210)

	mask_set(ta, (ch & 0x30) << 2);		// select and move PA bits of ADDR (54xxxxxx)
	mask_set(tc, (ch & 0x0F));  		// select and move PC bits of ADDR (xxxx3210)

	PAOUT = ta;
	PCOUT = tc;

	Channel = ch;
}

///////////////////////////////////////////////////////
void setSwitchState(BOOL state)
{
	if (state)
		SS_ON();
	else
		SS_OFF();
	STORE_SS();
	State[Channel] = state;
}

///////////////////////////////////////////////////////
void update_controller() {}


///////////////////////////////////////////////////////
void report_header()
{
	printromstr(R"CH S Error");
	endMessage();
}


////////////////////////////////////////////////////////
void report_device()
{
	printi(Channel, 2, ' '); printSpace();
	printi(State[Channel], 1, ' '); printSpace();
	printi(Error, 5, ' ');
	endMessage();
}

///////////////////////////////////////////////////////
void do_commands()
{
	char c;
	int n;

	while (!RxbEmpty())				// process a command
	{
		c = getc();		
		mask_clr(Error, ERROR_COMMAND);
		
		// single-byte commands
		if (c == '\0')				// null command
		{
			// (treat as single-byte command that does nothing)
		}
		else if (c == 'z')			// program data
		{
			printromstr(FIRMWARE); printromstr(VERSION); endMessage();
			endMessage();
		}
		else if (c == 'h')			// report header
		{
			report_header();
		}
		else if (c == 'r')			// report
		{
			report_device();
		}
		else if (c == '0')			// set selected channel OFF
		{
			setSwitchState(FALSE);
		}
		else if (c == '1')			// set selected channel ON
		{
			setSwitchState(TRUE);
		}
		else if (c == 'x')				// reset all switches to OFF
		{
			reset_all();
		}
		else						// multi-byte command
		{
			getArgs();

			if (c == 'n')					// select channel
			{				
				n = tryArg(0, CHANNELS - 1, ERROR_CHANNEL, Channel, FALSE);
				if (!(Error & ERROR_CHANNEL))
					selectChannel(n);
			}
			else					// unrecognized command
			{
				mask_set(Error, ERROR_COMMAND);
			}
		}
	}
}