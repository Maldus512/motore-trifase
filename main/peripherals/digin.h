#ifndef DIGIN_H_INCLUDED
#define DIGIN_H_INCLUDED

typedef enum {
    DIGIN_IN1=0, DIGIN_IN2, DIGIN_IN3
} digin_t ;

void digin_init();
int digin_get(digin_t digin);
void digin_take_reading();

#endif