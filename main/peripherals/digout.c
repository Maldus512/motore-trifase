#include "peripherals/digout.h"
#include "peripherals/hardwareprofile.h"

void digout_init() {
    RELE1_TRIS=0;
    RELE2_TRIS=0;
    RELE3_TRIS=0;
}

void digout_set(digout_t digout, int val) {
    switch(digout) {
        case DIGOUT_RELE1 :
            RELE1=val;
            break;
        case DIGOUT_RELE2 :
            RELE2=val;
            break; 
        case DIGOUT_RELE3 :
            RELE3=val;
            break;
    }
}