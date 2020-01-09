/**
   \file wheel/motor.c
 */
#include "motor.h"
/**
 * interrupt handler, checkes which way engine rotated and sets global position accordingly.
 * 
 */
/*void motorWatcher() {
        a = (MOTOR_SR & BIT(MOTOR_SR_IRC_A_MON)) != 0;
        b = (MOTOR_SR & BIT(MOTOR_SR_IRC_B_MON)) != 0;

        if(a == prev_a) {
        	if(a == 1){
        		if(b == 1){
        			position++;
        		} else{
        			position--;
        		}
        	} else{
        		if(b == 0){
        			position++;
        		} else{
        			position--;
        		}
        	}
        }else{
        	if(b == 1){
				if(a == 0){
					position++;
				} else{
					position--;
				}
			} else{
				if(a == 1){
					position++;
				} else{
					position--;
				}
			}
        }
        prev_a = a;
        prev_b = b;
        irq_count++;
        GPIO_INT_STATUS = MOTOR_IRQ_PIN; /* clear the interrupt 
}*/
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

        intConnect(INUM_TO_IVEC(INT_LVL_GPIO), motorWatcher, 0);
        intEnable(INT_LVL_GPIO);         /* enable all GPIO interrupts */
}
/**
 * disables GPIO interrupts, disconnect motor interrupt handler
 */
void motorWatcherCleanup() {
        GPIO_INT_DISABLE = MOTOR_IRQ_PIN;

        intDisable(INT_LVL_GPIO);
        intDisconnect(INUM_TO_IVEC(INT_LVL_GPIO), motorWatcher, 0);
}
