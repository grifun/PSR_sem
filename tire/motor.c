#include <taskLib.h>
#include <stdio.h>
#include <kernelLib.h>
#include <semLib.h>
#include <intLib.h>
#include <iv.h>

#include <xlnx_zynq7k.h>

#define REGISTER(base, offs) (*((volatile UINT32 *)((base) + (offs))))
#define BIT(i) ((1) << (i))


// MOTOR macros
// See section FPGA registers for more information.

#define PMOD1_BASE 0x43c20000
#define PMOD2_BASE 0x43c30000

#define MOTOR_BASE PMOD1_BASE

// GPIO register definitions
// See Zynq-7000 Technical Reference Manual for more information
//     Section: 14.2.4 Interrupt Function (pg. 391, pg. 1348).

// Pin on GPIO selected for interrupt
// Note: Each bit in a register refers to one pin. Setting some bit to `1`
//       also means which pin is selected.
#define MOTOR_IRQ_PIN BIT(2)

// Setting a bit in DIRM to `1` makes the corresponding pin behave as an output,
// for `0` as input.
// Note: So setting this to `MOTOR_IRQ_PIN` means, that this pin is an output
//       (which it is not so do not do it!).
//       This is similar with other GPIO/INT registers.
#define GPIO_DIRM         REGISTER(ZYNQ7K_GPIO_BASE, 0x00000284)

// Writing 1 to a bit enables IRQ from the corresponding pin.
#define GPIO_INT_ENABLE   REGISTER(ZYNQ7K_GPIO_BASE, 0x00000290)

// Writing 1 to a bit disables IRQ from the corresponding pin.
#define GPIO_INT_DISABLE  REGISTER(ZYNQ7K_GPIO_BASE, 0x00000294)

// Bits read as `1` mean that the interrupt event has occurred on a corresponding pin.
// Writing `1` clears the bits, writing `0` leaves the bits intact.
#define GPIO_INT_STATUS   REGISTER(ZYNQ7K_GPIO_BASE, 0x00000298)

// Setting TYPE to `0` makes interrupt level sensitive, `1` edge sensitive.
#define GPIO_INT_TYPE     REGISTER(ZYNQ7K_GPIO_BASE, 0x0000029c)

// Setting POLARITY to `0` makes interrupt active-low (falling edge),
//                     `1` active-high (raising edge).
#define GPIO_INT_POLARITY REGISTER(ZYNQ7K_GPIO_BASE, 0x000002a0)

// Setting ANY to `1` while TYPE is `1` makes interrupts act on both edges.
#define GPIO_INT_ANY      REGISTER(ZYNQ7K_GPIO_BASE, 0x000002a4)

// FPGA register definition
#define MOTOR_CR REGISTER(MOTOR_BASE, 0x0)
#define MOTOR_SR REGISTER(MOTOR_BASE, 0x4)
#define MOTOR_SR_IRC_A_MON 8
#define MOTOR_SR_IRC_B_MON 9

#define PWM_PERIOD REGISTER(MOTOR_BASE, 0x8)
#define PWM_CONTROL REGISTER(MOTOR_BASE, 0xC)


volatile unsigned irq_count;

void motorWatcher(void)
{
        bool a = (MOTOR_SR & BIT(MOTOR_SR_IRC_A_MON)) != 0;
        bool b = (MOTOR_SR & BIT(MOTOR_SR_IRC_B_MON)) != 0;
        // ...
        irq_count++;
        GPIO_INT_STATUS = MOTOR_IRQ_PIN; /* clear the interrupt */
}

void motorInit(void)
{
        GPIO_INT_STATUS = MOTOR_IRQ_PIN; /* reset status */
        GPIO_DIRM = 0x0;                 /* set as input */
        GPIO_INT_TYPE = MOTOR_IRQ_PIN;   /* interrupt on edge */
        GPIO_INT_POLARITY = 0x0;         /* rising edge */
        GPIO_INT_ANY = 0x0;              /* ignore falling edge */
        GPIO_INT_ENABLE = MOTOR_IRQ_PIN; /* enable interrupt on MOTOR_IRQ pin */

        intConnect(INUM_TO_IVEC(INT_LVL_GPIO), irc_isr, 0);
        intEnable(INT_LVL_GPIO);         /* enable all GPIO interrupts */
}

void irc_cleanup(void)
{
        GPIO_INT_DISABLE = MOTOR_IRQ_PIN;

        intDisable(INT_LVL_GPIO);
        intDisconnect(INUM_TO_IVEC(INT_LVL_GPIO), irc_isr, 0);
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

void rotate(int speed, char direction) {
    PWM_PERIOD = 50000;
    PWM_CONTROL = 0xE000;
}