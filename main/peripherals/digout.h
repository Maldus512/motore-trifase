#ifndef DIGOUT_H_INCLUDED
#define DIGOUT_H_INCLUDED

typedef enum {
    DIGOUT_RELE1=0,DIGOUT_RELE2, DIGOUT_RELE3,
} digout_t;

void digout_init();
void digout_set(digout_t digout, int val);

#endif