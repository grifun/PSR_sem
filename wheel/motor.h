volatile unsigned irq_count;
volatile char a, b, prev_a, prev_b;
volatile int position;


void motorWatcher();
void motorWatcherInit();
void motorWatcherCleanup();