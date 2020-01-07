#include "motor.h"

volatile unsigned irq_count;
/**
 * interrupt handler 
 */
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
	if( (speed < 0) || (speed > 100) ) {
		return;
	}
	//printf("nastavuju %x\n", (direction << 30) | (BASE_DUTY * speed));
	PWM_CONTROL = (direction << 30) | ( (BASE_DUTY) * speed);
}
/**
 * function that sets speed of rotation in dependecy with desired position and position
 */
void PID() {
		printf("in pid \n");
        while(1) {
        		if(desiredPosition == position){
        			rotate(0, direction);
        			continue;
        		}
                speed = (desiredPosition - position) / 10;
                if(speed < 0)
                	speed = -speed;
                if(speed > 100)
                	speed = 100;
                direction = ( (desiredPosition - position) < 0) + 1;
        		printf("position %d desPos %d speed %d direction %d\n",position, desiredPosition, speed, direction);
                rotate(speed, direction);
                //sleep(1);
        }
}
