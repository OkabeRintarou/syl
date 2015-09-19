#!/usr/bin/python
# -*- coding: utf-8 -*- 

from PIL import Image,ImageDraw,ImageFont,ImageFilter

red_color = (255,0,0)

open_file = input("open file:")

try:
    image = Image.open(open_file)
except IOError as e:
    print("open '%s' failed:%s" % (open_file,e))
    exit(1)

font = ImageFont.truetype('Arial.ttf',36)

draw = ImageDraw.Draw(image)

width,height = image.size
font_w = width // 5
font_h = height // 5
draw_x = width - font_w
draw_y = 2
draw_number = '4'

draw.text((draw_x,draw_y),draw_number,font=font,fill=red_color)

image.save('code.png','png')
