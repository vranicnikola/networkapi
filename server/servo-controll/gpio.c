#include "gpio.h"
#include "defines.h"

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

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

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

int mem_fd;
char *gpio_map;

// I/O access
volatile unsigned *gpio;

RT_TASK rtDriveServoTask;

typedef struct{
	unsigned int uiPin;
	unsigned int uiPosition;
}ServoAssignment;

void driveServoTask(void *arg __attribute__((__unused__)));

/**
 * Set up a memory regions to access GPIO
 */
void initIo() {
	if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
		printf("can't open /dev/mem \n");
		exit(-1);
	}
	/* mmap GPIO */
	gpio_map = (char *) mmap(NULL, //Any adddress in our space will do
			BLOCK_SIZE, //Map length
			PROT_READ | PROT_WRITE, // Enable reading & writting to mapped memory
			MAP_SHARED, //Shared with other processes
			mem_fd, //File to map
			GPIO_BASE //Offset to GPIO peripheral
			);

	close(mem_fd); //No need to keep mem_fd open after mmap
	if ((long) gpio_map < 0) {
		printf("mmap error %d\n", (int) gpio_map);
		exit(-1);
	}
	// Always use volatile pointer!
	gpio = (volatile unsigned *) gpio_map;
}

void setOutput(unsigned int uiPin) {
	INP_GPIO(uiPin);
	OUT_GPIO(uiPin);
}

void driveServo(unsigned int uiPin, unsigned int uiPosition) {
	ServoAssignment servoAssignment;
	servoAssignment.uiPin = uiPin;
	servoAssignment.uiPosition = uiPosition;
	rt_task_create(&rtDriveServoTask, "drive servo task", 0, 99, 0);
	rt_task_start(&rtDriveServoTask, &driveServoTask, &servoAssignment);
	sleep(2);
	rt_task_delete(&rtDriveServoTask);
}

void driveServoTask(void *arg __attribute__((__unused__))) {
	ServoAssignment servoAssignment = *((ServoAssignment*)arg);
	unsigned int i;
	GPIO_CLR = 1 << servoAssignment.uiPin;
	for (i = 0; i < SERVO_IMPULS_NUM; i++) {
		GPIO_SET = 1 << servoAssignment.uiPin;
		usleep(servoAssignment.uiPosition);
		GPIO_CLR = 1 << servoAssignment.uiPin;
		usleep(SERVO_PAUSE - servoAssignment.uiPosition);
	}
	//rt_task_set_periodic(NULL, TM_NOW, TIMESLEEP);
	//rt_task_wait_period(NULL);
}
