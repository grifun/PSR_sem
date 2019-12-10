#include "motor.c"

#define MOTOR_ADDR (0x43c20000)
#define MOTOR_CONTROL (*((volatile UINT16 *)(MOTOR)))
#define MOTOR_STATUS (*((volatile UINT16 *)(MOTOR + 0x0004)))
#define MOTOR_IRC (*((volatile UINT16 *)(MOTOR + 0x0004)) & 1<<9) 
#define MOTOR_B (*((volatile UINT16 *)(MOTOR + 0x0004)) & 1<<8) 
#define MOTOR_A (*((volatile UINT16 *)(MOTOR + 0x0004)) & 1<<7) 

void motorInit();
void rotate(int speed, char direction);
void stop();
void motorWatcher();
void IRC_reset();