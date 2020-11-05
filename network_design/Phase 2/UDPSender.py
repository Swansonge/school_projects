#Elizabeth Brown, Nolan Gray, Eric Swanson
#Network Design
#Phase 2 - UDPSender.py: Script that will data to a receiver via RDT. This script file open a BMP image, convert it to bytes, and then send it to a receiver.

from socket import *
import os #need to get file size


serverName = 'localhost'
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_DGRAM)

#function for making an image into a packet
#filename is the string containing the file path. barr is the bytearray created from the file
def Make_Packet(filename):
    print(f"The file name is {filename}")

    fileinfo = os.stat(filename) #fileinfo will contain various info about the file
    filesize = fileinfo.st_size #filesize extracts the file size, in bytes, from fileinfo
    print(f"The file is {filesize} bytes")
    
    numPacket = int(filesize/1024) #there will be 1024 bytes (1 KB) per packet, int() rounds down
    print(f"{numPacket} packets will be transferred")

    #open an image. 'r' reads a file and 'b' opens an image in binary mode
    with open(filename,'rb') as image:
        f = image.read() #read the file and write contents to the variable f
        barr = bytearray(f) #convert bytes to byte array. Each index of the array contains 1 byte

    arr = [] #array of packets
    leftovers = int(1024*(filesize/1024)-numPacket*1024) #number of leftover bytes in the incomplete packet
    #loop through every complete packet and add to the array of packets
    for i in range(0, numPacket):
        packet = [] 
        #this will do up to the last complete packet. Does not do last, incomplete, packet
        #j in this range because array starts at 0. Will take bytes 0-1023 for each packet
        for j in range(i*1024, ((i+1)*1024)):
            packet.append(barr[j])
        arr.append(packet) #add packet to array
            
    #this should take care of the last 54 bytes and put them in their own packet
    packet = [] 
    for k in range(numPacket*1024, numPacket*1024 + leftovers):
        packet.append(barr[k])
    arr.append(packet)
        
    return arr #this function returns the array of packets
      

filename = 'LAND.bmp'

#This is the array containing the packets. The contents of the packet will need to be reassembled as one big bytearray again that represents the BMP
arr= Make_Packet(filename)
#Find total number of packets and send to receiver
numPackets = str(len(arr))
clientSocket.sendto(numPackets.encode(), (serverName, serverPort))
#send one packet at a time to the server
for l in range(len(arr)):
    clientSocket.sendto(bytes(arr[l]), (serverName, serverPort))
clientSocket.close()
