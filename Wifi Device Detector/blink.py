import RPi.GPIO as GPIO
import time


GPIO.setmode(GPIO.BOARD)
GPIO.setup(7, GPIO.OUT)

var = 1
while var == 1:
	GPIO.output(7, True)
	time.sleep(0.3)
	GPIO.output(7, False)
	time.sleep(0.3)