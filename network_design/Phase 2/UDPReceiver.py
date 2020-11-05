#Elizabeth Brown, Nolan Gray, Eric Swanson
#Network Design
#Phase 2 - UDPReceiver.py: Will receive the data stream from the UDP sender (the bytes of a BMP image), convert from from bytes back to BMP format, and display the image

from socket import *

#extractData
#This method will take the data from a packet and add it to the array data
#Paramaters:
#packet - the packet from which data will be extracted
#data - the array the information from the packet will be added to	
def extractData(packet, data):
	for i in range(0, len(packet)):
		data.append(packet[i])
	
#deliverData
#This method will deliver the data to a new file
#Parameters
#filename - name of the new file
#data - the array containing all of the packet information
def deliverData(filename, data):
	with open(filename, 'wb') as newImage:
		newImage.write(bytearray(data))

#set up server port 
serverPort = 12000
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('', serverPort))
#declare variables that will be used
#name of new file
filename = 'newLAND.bmp'
#array to contain incoming packets
packetArr = []
#array to put data from packets
dataArr = []

print("The server is ready to receive")
#First incoming packet will tell how many packets to expect from client
message, clientAddress = serverSocket.recvfrom(64000)
numPackets = message.decode()
numPackets = int(numPackets)
print(f'The server will recieve {numPackets} packets')
#Recieve each incoming packet from client and put in packetArr
for i in range(0, numPackets):
	message, clientAddress = serverSocket.recvfrom(64000)
	packetArr.append(message)
#Extract data from each packet
for i in range(0, numPackets):
	extractData(packetArr[i],dataArr) 
#Deliver data to new file
deliverData(filename, dataArr)