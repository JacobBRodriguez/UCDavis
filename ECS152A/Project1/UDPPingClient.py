import time
# Jacob Rodriguez, Devon Johnson
from socket import *
import os
serverName = 'localhost' # May need to change to actual IP
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_DGRAM)

serverAdd = (serverName, serverPort)
weekdays = ['U', 'M', 'T', 'W', 'R', 'F', 'S']

#Set the socket timeout to 1 second
clientSocket.settimeout(1)

# 10 separate ping server requests
for ping in range(1,11):

	# Start time of the message
	startTime = time.time()
	
	# Sending the message over the socket to server
        message = 'Ping %d %s %s %s' % (ping,time.strftime('%Y-%m-%d'),weekdays[int(time.strftime('%w'))],time.strftime('%I:%M %Z'))
	clientSocket.sendto(message, serverAdd)
        print message;
	try:
		# Receiving message from the server side
		data, server = clientSocket.recvfrom(1024)
		endTime = time.time()
		deltaTime = endTime - startTime

		# Printing out results from server
                print data.upper();
                print 'RTT: %1.3f\n' % (deltaTime * 1000)

	# If the response takes longer than 1 second then get timeout error
	except timeout:
		print 'Request timed out\n'
