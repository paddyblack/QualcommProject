"""
    Written by Soo Yong Lee
    Last updated 28/04/2015
    """


import csv
import subprocess
import pandas as pd
import datetime

import time
from time import strftime

import MySQLdb



var = 1
while var == 1:

	#Runs terminal command to capture wifi data packets for 120 seconds and then logs into a file "test.pcap"
	subprocess.call("sudo tshark -w /tmp/test.pcap -T fields -e frame.number -e frame.time -e eth.src -e eth.dst -e ip.src -e ip.dst -e ip.proto -E header=y -E separator=, -E quote=d -E occurrence=f -a duration:120", shell=True)

	#Runs terminal command to convert the file "test.pcap" to "test.csv"
	subprocess.call("sudo tshark -r /tmp/test.pcap -T fields -e frame.number -e frame.time -e eth.src -e eth.dst -e ip.src -e ip.dst -e ip.proto -E header=y -E separator=, -E quote=d -E occurrence=f  > /home/pi/test.csv" , shell=True)

	#Read file "test.csv"
	df = pd.read_csv("/home/pi/test.csv")

	#Rename the columns
	df = df.rename(columns = {'eth.src':'eth_src'})
	df = df.rename(columns = {'eth.dst':'eth_dst'})


	#Sort table by column names
	sorted = df.groupby(["eth_src"]).eth_src.nunique()
	sorted2 = df.groupby(["eth_dst"]).eth_dst.nunique()


	#Count number of wifi devices by counting unique MAC addresses ( -1 so the router does not get counted)
	count = len(sorted.index) + len(sorted2.index) - 1

	#Get current date and time
	date = datetime.datetime.now()



	#Set properties for the result table
	columns = ['Time', 'Number']

	result = pd.DataFrame(index=['0'], columns=columns)

	#Change column data type to Date and Time
	result['Time'] = pd.to_datetime(result['Time'])

	#Set current time to ISO format
	result.Time[0] = date.isoformat()

	#Get number of devices
	if count < 0:
		result.Number[0] = 0
	else:
		result.Number[0] = count
	
	#Write result file
	with open('/home/pi/result.csv', 'w') as fp:
		a = csv.writer(fp, delimiter=',')
		data = [[result.Time[0], result.Number[0]]]
		a.writerows(data)



	#Connect to MySql sever

	mydb = MySQLdb.connect(host='192.168.0.4', user='sooyong', passwd='dltndyd', db='comp2010')

	cursor =mydb.cursor()

	
	csv_data = csv.reader(file('/home/pi/result.csv'))


	for row in csv_data:
		
		cursor.execute('INSERT INTO wifi_presence(Timestamp, Number_Of_Devices)' \
		'VALUES(%s,%s)', row)

	mydb.commit()
	cursor.close()
