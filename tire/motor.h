
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

#define LEFT 2
#define RIGHT 1
#define MIN_DUTY 0xB0
#define MAX_DUTY 0x4000
#define BASE_DUTY 0xB0

void motorWatcher(void);
void motorWatcherInit();
void motorInit();
void watcherCleanup(void);
void motor();
void rotate(unsigned speed, char direction);
