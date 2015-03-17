
import subprocess


#Runs terminal command to capture wifi data packets for 60 seconds and then logs into a file "test.pcap"
subprocess.call("sudo tshark -w /tmp/test.pcap -T fields -e frame.number -e frame.time -e eth.src -e eth.dst -e ip.src -e ip.dst -e ip.proto -E header=y -E separator=, -E quote=d -E occurrence=f -a duration:60", shell=True)

#Runs terminal command to convert the fil "test.pcap" to "test.csv"
subprocess.call("sudo tshark -r /tmp/test.pcap -T fields -e frame.number -e frame.time -e eth.src -e eth.dst -e ip.src -e ip.dst -e ip.proto -E header=y -E separator=, -E quote=d -E occurrence=f > /home/pi/test.csv", shell=True)
