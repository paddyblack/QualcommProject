"""
    Written by Soo Yong Lee
    Last updated 28/04/2015
    """



import RPi.GPIO as GPIO
import os
import thread
import time

GPIO.setmode(GPIO.BOARD)
GPIO.setup(18, GPIO.IN, pull_up_down=GPIO.PUD_UP)

def capture(command):
	os.system(command)

while True:
	input_state = GPIO.input(18)
	if input_state == False:
		try:
		 	thread.start_new_thread( capture,("sudo python /home/pi/blink.py", ) )
			thread.start_new_thread( capture,("sudo python /home/pi/start.py", ) )
		except:
			print "@Error: unable to start thread"
		while 1:
			pass