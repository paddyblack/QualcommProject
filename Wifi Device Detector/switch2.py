import RPi.GPIO as GPIO
import time
import os

GPIO.setmode(GPIO.BOARD)

GPIO.setup(22, GPIO.IN, pull_up_down=GPIO.PUD_UP)

while True:

	input_state2 = GPIO.input(22)
	

		
	if input_state2 == False:

		os.system("sudo shutdown -h now")