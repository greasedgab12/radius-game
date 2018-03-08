#! /usr/bin/env python3
#
#use this script to convert .bmp files in grayscale into the "sprite-format" used by our game
#bytes per page are ordered for window mode top-bottom -> left-right (UC1610 window mode 2)
#

#aray format width,lengthxwidth,, data

#add func to delete unneccessary pages

 
from PIL import Image
from math import sqrt
import glob
import os

print("creating sprites...")
 
with open("./sprites.h","w+") as sprites_h:  
    sprites_h.write("#ifndef SPRITES_H\n")
    sprites_h.write("#define SPRITES_H\n")
    sprites_h.write("#include <avr/pgmspace.h>\n")
    sprites_h.write("#include <inttypes.h>\n\n")
    
     
with open("./sprites.c","w+") as sprites_c:  
    sprites_c.write("#include \"sprites.h\"\n")
    sprites_c.write("\n")     
     
sprite_files = [os.path.basename(x) for x in glob.glob("./bitmaps/*.bmp")]  

for file_number,sprites in enumerate(sprite_files): # 'file' is a builtin type, 'name' is a less-ambiguous variable name.
    print(sprite_files[file_number])
    im = Image.open("./bitmaps/" + sprite_files[file_number],"r") 
    pixel = list(im.getdata())
    page_width = im.size[0]   
    page_height = im.size[1]    
        
    #if pictureheight allows for half full pages add white to fill it 
    while True: 
        if page_height % 4 == 0:
            break
        for j in range(0, page_width):
            pixel.append(255)
        page_height +=1
    
    #convert 8bit to 2bit grayscale    
    for counter in range(0,len(pixel)):
        if pixel[counter] >=180:
            pixel[counter] = 0b00
        elif pixel[counter] >=120:
            pixel[counter] = 0b01               
        elif pixel[counter] >=60:
            pixel[counter] = 0b10
        else:
            pixel[counter] = 0b11 
            
    #fit pixel into pages windowmode 1
    window = []
    for counter in range(0,page_width):    
        for col in range(0,page_height-1,4):  
            page = hex( (pixel[page_width*(col) + counter]) | (pixel[page_width*(col+1) + counter])<<2 | (pixel[page_width*(col+2) + counter])<<4 | (pixel[page_width*(col+3) + counter])<<6 )
            window.append(page)
    
    #write finised sprites in "window-format" to file 
    with open("./sprites.c","a") as sprites_c:
        sprites_c.write("const uint8_t " + os.path.splitext(sprite_files[file_number])[0] +"[] "+ " PROGMEM = {" + str(page_width) + "," + str(len(window)) + "," + " \t //" + os.path.splitext(sprite_files[file_number])[0] + ".bmp")
        for counter,value in enumerate(window):
            if counter % page_width == 0:
                sprites_c.write("\n")
            if counter == len(window)-1:
                sprites_c.write(value)    
            else:
                sprites_c.write(value +",")
        sprites_c.write("\n};\n\n")
        
    with open("./sprites.h","a") as sprites_h: 
        defines = "#define " + os.path.splitext(sprite_files[file_number])[0].upper()+" " + str(file_number) + "\n";
        sprites_h.write(defines);
        sprites_h.write("extern const uint8_t "+ os.path.splitext(sprite_files[file_number])[0]+"["+ str(len(window)+2)+"]" + " PROGMEM;\n")

with open("./sprites.c","a") as sprites_c:
    prefix = "uint8_t getSpriteIndex(uint8_t* sprite){\n"
    sprites_c.write(prefix);
    for file_number,sprites in enumerate(sprite_files):
        sprites_c.write("if (sprite == " +  os.path.splitext(sprite_files[file_number])[0]+ "){ return " + str(file_number) +";}\n")
    sprites_c.write("return 0;\n}")
    

with open("./sprites.h","a") as sprites_h: 
    sprites_h.write("\nuint8_t getSpriteIndex(uint8_t* sprite);")
    sprites_h.write("\n#endif")
    
    
print("all sprites were created.")
