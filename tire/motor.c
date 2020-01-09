/** \file tire/motor.c */

#include "motor.h"

volatile unsigned irq_count;
/**
 * interrupt handler 
 */
 static const int table[16] = {
          [TRANSITION(0,1)] = +1,
          [TRANSITION(1,3)] = +1,
          [TRANSITION(3,2)] = +1,
          [TRANSITION(2,0)] = +1,
          [TRANSITION(1,0)] = -1,
          [TRANSITION(3,1)] = -1,
          [TRANSITION(2,3)] = -1,
          [TRANSITION(0,2)] = -1,
  };
void motorWatcher()
{
    int irc_state = (MOTOR_SR & BIT(MOTOR_SR_IRC_A_MON) ? 1 : 0) | (MOTOR_SR & BIT(MOTOR_SR_IRC_B_MON) ? 2 : 0);
    position += table[TRANSITION(last_irc_state, irc_state)];
    last_irc_state = irc_state;
    GPIO_INT_STATUS = MOTOR_IRQ_PIN; /* clear the interrupt */
}
/**
 * initiation of all needed constants and registers, connecting interrupt hanlder and enabling of all GPIO interrupts
 */
void motorWatcherInit() {
        GPIO_INT_STATUS = MOTOR_IRQ_PIN; /* reset status */
        GPIO_DIRM = 0x0;                 /* set as input */
        GPIO_INT_TYPE = MOTOR_IRQ_PIN;   /* interrupt on edge */
        GPIO_INT_POLARITY = 0x0;         /* rising edge */
        GPIO_INT_ANY = 0x0;              /* ignore falling edge */
        GPIO_INT_ENABLE = MOTOR_IRQ_PIN; /* enable interrupt on MOTOR_IRQ pin */
        position = 0;
        intConnect(INUM_TO_IVEC(INT_LVL_GPIO), motorWatcher, 0);
        intEnable(INT_LVL_GPIO);         /* enable all GPIO interrupts */
}

void motorInit() {
		MOTOR_CR = 0b1000000;
		PWM_PERIOD = 5000;
}

void watcherCleanup(void){
        GPIO_INT_DISABLE = MOTOR_IRQ_PIN;

        intDisable(INT_LVL_GPIO);
        intDisconnect(INUM_TO_IVEC(INT_LVL_GPIO), motorWatcher, 0);
}
/**
 * sets rotation of engine in direction and with speed params
 * @param speed percent of base
 * direction LEFT or RIGHT 
 */
void rotate(unsigned speed, char direction) {
	if( (direction != LEFT) && (direction != RIGHT) ) {
		return;
	}
	if(speed < 0)
		return;
	if(speed > 100)
		speed = 100;
	PWM_CONTROL = (direction << 30) | ( BASE_DUTY * speed);
}
/**
 * function that sets speed of rotation in dependecy with desired position and position
 */
void PID() {
		printf("PID started \n");
		int delta;
        while(1) {
        		if(desiredPosition == position){
        			rotate(0, direction);
        			continue;
        		}
                delta = desiredPosition - position; //higher delta -> higher speed
                if(delta < 0)
                	delta = -delta;
                if(delta > 200)
                	speed = 100;
                else
                    speed = delta/2;
                direction = ( (desiredPosition - position) < 0) + 1;
				//printf("position %d, desPos %d, speed %d, direction %d\n", position, desiredPosition, speed, direction);
                rotate(speed+25, direction);//a constant bias to make the motor actually moving
                nanosleep(&tim , &tim2);
        }
}