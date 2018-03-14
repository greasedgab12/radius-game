#!/usr/bin/env python2

cfilename = "fx.c"
hfilename = "fx.h"

with open(hfilename, "w") as hfile:
    with open(cfilename, "w") as cfile:
        cfile.write("#include \"music.h\"\n\n")
        hfile.write("#ifndef FX_H\n#define FX_H\n\n#include <inttypes.h>\n#include <avr/pgmspace.h>\n\n")
        #gun / disk / missle /  /shotgun / bounce / impact / lasor / explosion / collision / impact / menu / heavy                                                     
        delays= [100,   120,    250,    100,    150,    100,    130,  100,    100,  200,    80 ,90]
        start = [2000,  1000,   0,      200,    300,    0,      100,  0,      1000, 400,    500 ,300]
        end =   [3000,  3000,   1000,   1100,   1500,   1000,   1000, 2000,   1300, 2000,   1200 ,2000]
        step =  [100,   100,    50,     100,    150,    100,    100,  100,    100,  100 ,   80 ,100]

        for inc in range(0,12,1):            
            arrayname = "effect" + str(inc+1)
            delay = delays[inc]
            array = "const uint16_t " + arrayname + "[] PROGMEM = {\n"
            rng = range(start[inc], end[inc], step[inc]) 
            for increment in rng:
                array += "    " + str(delay) + ", " + str(increment) + ",\n"
            
            array += "    0xFFFF\n};"
            hfile.write("extern const uint16_t " + arrayname + "[" + str(2 * len(rng) + 1) + "] PROGMEM;\n\n")
            cfile.write(array)
            cfile.write("\n\n")
        hfile.write("#endif")


#gun        delay = 100      rng = range(000, 3000, 100)
#disk       delay = 100      rng = range(1000, 3000, 100)
#missle     delay = 250      rng = range(0, 1000, 50) 
#shotgun    
#bounce      delay = 150        rng = range(300, 1500, 150) 
#impact     delay = 100      rng = range(0, 1000, 100)
#explosion
#collision  delay = 100      rng = range(0, 2000, 100)
#menu        delay = 100    rng = range(1000, 1300, 100) 


#songs