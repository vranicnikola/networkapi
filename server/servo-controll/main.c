#include "gpio.h"
#include "log.h"
#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/mman.h>

#include <native/task.h>
#include <native/timer.h>

int mains(int argc, char **argv) {
	unsigned int i;

	/* Avoids memory swapping for this program */
	mlockall(MCL_CURRENT|MCL_FUTURE);

	// Set up gpi pointer for direct register access
	doLog("INFO: start initIo()...\n");
	initIo();
	doLog("INFO: end initIo().\n");
	setOutput(22);
	while(1){
		for (i = 0; i < 10; i++) {
			doLog("INFO: drive: %d", i);
			driveServo(22, SERVO_OPEN);
			sleep(2);
			driveServo(22, SERVO_CLOSE);
			sleep(2);
		}
	}

	return 0;
}
