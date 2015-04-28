"""
    Written by Soo Yong Lee
    Last updated 28/04/2015
"""

import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)
GPIO.setup(7, GPIO.OUT)
GPIO.output(7, True)