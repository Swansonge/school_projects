#Elizabeth Brown, Nolan Gray, Eric Swanson
#Network Design
#Phase 4 - UDPSender.py: Script that will data to a receiver via RDT3.0. This script file open a BMP image, convert it to bytes, and then send it to a receiver.
#                        This script will verify that an ACK was properly received from the Receiver side using checksums and sequence numbers. If an ACK is corrupted
#                        or a sequence number is incorrect, the packet will be resent when the timer runs out.
from socket import *
import os #need to get file size
import binascii #To convert between bytearray and string
import random #to randomize the packets corrupted
import pickle #to convert list to bytes representation
import time #to time it
import sys #for command line arguments
import threading #for timer thread


########## FUNCTIONS ###########
#Takes a bytearray of packet data and calculates a 16 bit checksum returned as an integer
def calc_checksum(packet_data):
    #variables for calculating checksum
    packet_data = bytearray(packet_data)
    sum = 0
    temp = 0
    chunk1 = 0
    chunk2 = 0
    #constant to check if there is a carry
    two_byte_max = 65536
    #convert bytearray to string for concatinating bytes
    str_data = binascii.hexlify(packet_data)
    for i in range(0, len(str_data)-7, 8):
        #Select 2 16 bit chunks
        chunk1 = int(bytes(str_data[i:i+4]),16)
        chunk2 = int(bytes(str_data[i+4:i+8]),16)
        #Add the 16 bit chunks together
        temp = (chunk1 + chunk2) % two_byte_max
        #Check if there was a carry, if so add 1 to result
        if (temp < max(chunk1, chunk2)):
            temp = temp + 1
        #Add result to checksum and check for carry
        sum = (sum + temp) % two_byte_max
        if (sum < max(sum, temp)):
            sum = sum + 1
    #return final 16 bit checksum as integer
    return sum % two_byte_max

#function for making an image into a packet. Take sequence #, data (1 kB), and checksum, and put into a packet. This is packet is just an array w/ length=3
def Make_Packet(seqnum, data, checksum):
    Packet = [seqnum, data, checksum]
    return Packet #this function returns the packet to be sent

def Send(packet):
    #SEND INDIVIDUAL PACKET
    clientSocket.sendto(pickle.dumps(packet), (serverName, serverPort))
    return

def Receive():
    #RECEIVE ACK PACKET
    message, serverAddress = clientSocket.recvfrom(64000)
    packet = pickle.loads(message)
    #return ACK, checksum to check for corruption, and sequence # to check in isACK for duplicates
    ACK = packet[1]
    checksum = packet[2]
    seqnum = packet[0]
    return ACK, checksum, seqnum

def check_corrupt(checksum, ACKchecksum):
    #CHECK FOR CORRUPTION
    #return 1 for corrupt, 0 for not corrupt
    if (checksum == ACKchecksum):
        return 0
    else:
        return 1

#compare current sequence number to sequence number from ACK. If they are the same, the sent packet is a duplicate
def isACK(seqnum, ACKseqnum):
    isDuplicate = 0 #assume not a duplicate
    if not (seqnum == ACKseqnum):
        isDuplicate = 1
    return isDuplicate

def percent_corrupt(ranPacket, perc):
    #find percent of packets to be corrupted and place in array
    percent = int(perc)
    decimal = percent * 0.01
    percPacket = numPacket * decimal
    percPacket = int(percPacket)
    packetList = list()
    for i in range(0, numPacket):
        packetList.append(i)
    ranPacket = random.sample(packetList, percPacket)
    ranPacket.sort()
    return ranPacket

# corrupt packets by a % given by the user.
#given array of packets
def corrupt_packet(packet):
    #for every packet
    for i in range(0, len(corruptPackets)-1):
        if numSent == corruptPackets[i]:
            #if that numSent is in ranPacket then corrupt that packet
            corruptPackets.pop(i)
            corruptPkt = bytearray([1,2,3,4,5])
            return corruptPkt
    #return regular packet if not
    return packet

def lose_packet():
        #for every packet
    for i in range(0, len(lossPackets)-1):
        if numSent == lossPackets[i]:
            #if that numSent is in ranPacket then corrupt that packet
            lossPackets.pop(i)
            return True
    return False
  
        

##timer functions##
def thread_start_timer():
    global done
    global timeout
    global time_thread
    global lock
    while not done:
        if timeout.isSet():#After timeout/timer stop
            time_thread = start_timer()#start timer again
            lock.acquire()
            timeout.clear()#clear shared timeout flag
            lock.release()

def thread_timer_function():
    global done
    global timeout
    if not done:
        global sndpkt
        sndpkt = Make_Packet(seqnum, corrupt_packet(dataArr[numSent]), checksumArr[numSent])
        Send(sndpkt) #Resend packet on timeout
        lock.acquire()
        timeout.set() #Set shared timeout flag
        lock.release()

def start_timer():
    time_thread = threading.Timer(wait_time, thread_timer_function)
    time_thread.start() #Start the timer thread
    return time_thread

def stop_timer(time_thread):
    time_thread.cancel() #Stops the timer
    timeout.set()


########## MAIN ##########
#timing for graph
startTime = time.time()

#set up client socket
serverName = 'localhost'
serverPort = 12000
clientPort = 12001
clientSocket = socket(AF_INET, SOCK_DGRAM)
clientSocket.bind(('', clientPort))

#Finite State Machine (FSM). What action will occur depends on state
FSM = "wait for call 0 from above" #this is the initial state at the state of the program
#other possible states: "wait for ACK 0", "wait for call 1 from above", and "wait for ACK 1"

filename = 'LAND.BMP'

print(f"Transferring {filename}")

fileinfo = os.stat(filename) #fileinfo will contain various info about the file
filesize = fileinfo.st_size #filesize extracts the file size, in bytes, from fileinfo

numPacket = int(filesize/1024) #there will be 1024 bytes (1 KB) of data per packet, int() rounds down. SO, this variable is actually 1 less than the total # of packets. The 'leftover' data will make up the last packet


#open an image. 'r' reads a file and 'b' opens an image in binary mode
with open(filename,'rb') as image:
    f = image.read() #read the file and write contents to the variable f
    barr = bytearray(f) #convert bytes to byte array. Each index of the array contains 1 byte

dataArr = [] #array of data in the packets. Each array index contains the data portion of the packet.
leftovers = int(1024*(filesize/1024)-numPacket*1024) #number of leftover bytes in the incomplete packet
#loop through every complete packet of data and add to the data array
for i in range(0, numPacket):
    packetData = []
    #this will do up to the last complete packet. Does not do last, incomplete, packet
    #j in this range because array starts at 0. Will take bytes 0-1023 for each packet
    for j in range(i*1024, ((i+1)*1024)):
        packetData.append(barr[j])
    dataArr.append(packetData) #add packet of data to array

#this should take care of the last 54 bytes and put them in their own set of packet data
packetData = []
for k in range(numPacket*1024, numPacket*1024 + leftovers):
    packetData.append(barr[k])
dataArr.append(packetData)

numPacket = len(dataArr) #This is the true number of packets to be sent

#calculate the checksum for each packet
checksumArr = [] #create array of checksums, one for each packet
#calculate checksum for each data packet. Each checksum will be appended to the array of checksums
for l in range(0, numPacket):
    checksum  = calc_checksum(bytearray(dataArr[l]))
    checksumArr.append(checksum)

numSent = 0 #variable indicating number of packets sent. This number lines up with the indices of the data and checksum arrays
seqnum = 0 #sequence number for packets, either 0 or 1. Is 0 to start

#first packet sent will tell receiver how many packets are needed to complete the data
clientSocket.sendto(str(numPacket).encode(), (serverName, serverPort))

corruptPackets = list()
lossPackets = list()
corruptPackets = percent_corrupt(corruptPackets, sys.argv[1])
lossPackets = percent_corrupt(lossPackets, sys.argv[2])
lost = False

#Initialize timer variables
lock = threading.Lock()
done = False #to prevent re-transmit after completion
wait_time = 0.05 #in seconds
timeout = threading.Event()
first = True
start_thread = threading.Thread(target = thread_start_timer)
global time_thread
time_thread = threading.Timer(wait_time, thread_timer_function())

#keep sending packets until all packets are sent
while numSent < numPacket:
    #waiting on call for packet with sequence # 0
    if FSM == "wait for call 0 from above":
        #NOTE: if packet is sent from server in this state, it will be ignored by client
        seqnum = 0
        sndpkt = Make_Packet(seqnum, corrupt_packet(dataArr[numSent]), checksumArr[numSent]) #make packet
        lost = lose_packet()
        if not lost:
            Send(sndpkt) #send packet
        if first:
            start_thread.start()
            first = False
        FSM = "wait for ACK 0" #transition to next state

    #Waiting for ACK from server that packet with seq # 0 was received
    elif FSM == "wait for ACK 0":
        ACK, ACKchecksum, ACKseqnum = Receive() #receive ACK from server. store checksum and seqnum from ACK in variables
        checkACK = calc_checksum(ACK) #calculate checksum from data received in ACK
        isCorrupted = check_corrupt(checkACK, ACKchecksum) #check is ACK is corrupted. store result in variable
        isDuplicate = isACK(seqnum, ACKseqnum) #check is seqnum from ACK is 0. If not 0, then duplicate. store result in variable
        #if ACK corrupted or packet is duplicate, go back to beginning of loop
        if isCorrupted or isDuplicate:
            continue #Timer handles re-transmit. Return to current state
        else:
            stop_timer(time_thread) #uncorrupted/non-duplicate
            numSent = numSent + 1 #If no corruption/duplicate, packet with seq # 0 sent successfully. Move on to next packet. 
            FSM = "wait for call 1 from above" #move to next state

    #waiting on call for packet with sequence # 1         
    elif FSM == "wait for call 1 from above":
        #NOTE: if packet is sent from server in this state, it will be ignored by client
        seqnum = 1
        sndpkt = Make_Packet(seqnum, corrupt_packet(dataArr[numSent]), checksumArr[numSent])
        lost = lose_packet()
        if not lost:
            Send(sndpkt) #send packet
        FSM = "wait for ACK 1" #move to next state

    #Waiting for ACK from server that packet with seq # 1 was received
    elif FSM == "wait for ACK 1":
        ACK, ACKchecksum, ACKseqnum = Receive()
        checkACK = calc_checksum(ACK)
        isCorrupted = check_corrupt(checkACK, ACKchecksum)
        isDuplicate = isACK(seqnum, ACKseqnum) #check is seqnum from ACK is 1. If not 1, then duplicate. store result in variable
        if isCorrupted or isDuplicate:
            continue #Timer handles re-transmit. Return to current state
        else:
            stop_timer(time_thread)
            numSent = numSent + 1 #If no corruption/duplicate, packet with seq # 1 sent successfully. Move on to next packet. 
            FSM = "wait for call 0 from above" #move to next state
done = True #transfer is done
start_thread.join() #join start thread to main thread
print("Done")

finalTime = time.time() - startTime
print(f"Execution time: {finalTime}")


##This is the array containing the packets. The contents of the packet will need to be reassembled as one big bytearray again that represents the BMP
#arr= Make_Packet(filename)
##Find total number of packets and send to receiver
#numPackets = str(len(arr))
#clientSocket.sendto(numPackets.encode(), (serverName, serverPort))
##send one packet at a time to the server
#for l in range(len(arr)):
#    clientSocket.sendto(bytes(arr[l]), (serverName, serverPort))
#clientSocket.close()
