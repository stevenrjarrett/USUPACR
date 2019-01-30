# xboxOne.py
# Utility to use an xbox One controller in python 3

#import evdev
from evdev import InputDevice, categorize, ecodes

#creates object 'gamepad' to store the data
#you can call it whatever you like
# gamepad = InputDevice('/dev/input/event22')

#button code variables (change to suit your device)
aBtn = 304
bBtn = 305
xBtn = 307
yBtn = 308

up = 46
down = 32
left = 18
right = 33

start = 24
select = 49

lTrig = 37
rTrig = 50

#prints out device info at start
#print(gamepad)

#loop and filter by event code and print the mapped label
def test():
    for event in gamepad.read_loop():
        if event.type == ecodes.EV_KEY:
            if event.value == 1:
                if event.code == yBtn:
                    print("Y")
                elif event.code == bBtn:
                    print("B")
                elif event.code == aBtn:
                    print("A")
                elif event.code == xBtn:
                    print("X")

                elif event.code == up:
                    print("up")
                elif event.code == down:
                    print("down")
                elif event.code == left:
                    print("left")
                elif event.code == right:
                    print("right")

                elif event.code == start:
                    print("start")
                elif event.code == select:
                    print("select")

                elif event.code == lTrig:
                    print("left bumper")
                elif event.code == rTrig:
                    print("right bumper")
                else:
                    print(event)
        else:
            if event.type == 03:
                if event.code == 16:
                    if event.value == 01:
                        print("Right")
                    elif event.value == -1:
                        print("Left")
                if event.code == 17:
                    if event.value == -1:
                        print("Up")
                    elif event.value == 01:
                        print("Down")

