#include "motor.h"

volatile unsigned irq_count;

void motorWatcher() {
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
        GPIO_INT_STATUS = MOTOR_IRQ_PIN; /* clear the interrupt */
}

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

void motorInit() {
        MOTOR_CR = 0b1000000;
	PWM_PERIOD = 5000;
}

void watcherCleanup(void){
        GPIO_INT_DISABLE = MOTOR_IRQ_PIN;

        intDisable(INT_LVL_GPIO);
        intDisconnect(INUM_TO_IVEC(INT_LVL_GPIO), motorWatcher, 0);
}

/*
 * Entry point for DKM.
 */
void motor()
{
        TASK_ID st;

        motorInit();

        while (1) {
            printf("IRQ count: %u\n", irq_count);
            sleep(1);
        }

        irc_cleanup();
}

void rotate(unsigned speed, char direction) {
	/* optimal fast boi
	MOTOR_CR = 0b1000000; set manual control
        PWM_PERIOD = 50000; perioda v 10ns
        PWM_CONTROL = 0xA0000000; prvni 2 bity vlevo smer, ostatni byty percentage
        */
	//PWM_PERIOD = 50000;
	if( (direction != LEFT) && (direction != RIGHT) ) {
		return;
	}
	if( (speed < 0) || (speed > 100) ) {
		return;
	}
	//printf("nastavuju %x\n", (direction << 30) | (BASE_DUTY * speed));
	PWM_CONTROL = (direction << 30) | (BASE_DUTY * speed);
}

void PID() {
        while(1) {
                speed = abs(desiredPosition - position) / 512;
                direction = sign(desiredPosition - position);
                rotate(speed, direction);
        }
}
