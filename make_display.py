from PIL import Image
import math
import os.path as path

# This code creates image to display on the tanvas and saves it to the correct folder

# How to run from command on windows
# go to folder and type python example1.py

# Size of haptic display
H_tablet = 780; #738;
W_tablet = 1280; #1024;

im = Image.new('RGB',(W_tablet,H_tablet),(255,255,255));
name =  path.abspath(path.join(__file__ ,"..")); # move up two files in directory

for x in range(0,249,1):
    for y in range(0,H_tablet,1):
        im.putpixel((x,y),(0,0,0));
for x in range(1030,1280,1):
    for y in range(0,H_tablet,1):
        im.putpixel((x,y),(0,0,0));

im.save(name+'\\TouchDetection_demo\\sharedcontrol\\sharedcontrol\\Assets\\demo.png');

im.show()
