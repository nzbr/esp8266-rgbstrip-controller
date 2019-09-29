//
// Created by nzbr on 13.08.2019.
//

#include "bootanimation.h"

void bootAnimation() {
    for (int i = 1; i < 512; i++) {
        if (i <= 255) {
            drvSetStrip(i, i, i);
        } else {
            int mod = 255 - i;
            drvSetStrip(mod, mod, mod);
        }
        drvShow();
		#ifdef DRV_TM1829
			delay(1);
		#endif
    }
}
