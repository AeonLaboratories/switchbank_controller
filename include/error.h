///////////////////////////////////////////////////////
// error.h
//
// switchbank controller

#pragma once // Include this file only once

#include "..\\..\\common_controller\\include\\c99types.h"

#define ERROR_NONE			0		// No error
#define ERROR_BUF_OVFL		1		// RSS232 input buffer overflow
#define ERROR_CRC			2		// RS232 CRC error
#define ERROR_COMMAND		4		// unrecognized command from RS232
#define ERROR_CHANNEL		8		// Invalid device channel


extern volatile uint16_t Error;
