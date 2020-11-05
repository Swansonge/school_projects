#Elizabeth Brown, Nolan Gray, Eric Swanson
#Network Design
#Phase 5 - UDPReceiver.py: Implement Go-Back-N protocol for pipelined reliable data transfer

from socket import *
import binascii #To convert between bytearray and string
import pickle #to convert between list and bytes
import sys #for command line arguments
import random #for random sample of packets to corrupt

########### FUNCTIONS ###########
#extractData
#This method will take the data from a packet and add it to the array data
#Paramaters:
#packet - the packet from which data will be extracted
#data - the array the information from the packet will be added to	
def extractData(packet, data):
    packet = bytearray(packet)
    data.append(packet)

#deliverData
#This method will deliver the data to a new file
#Parameters
#filename - name of the new file
#data - the array containing all of the packet information
def deliverData(filename, data):
    with open(filename, 'ab+') as newImage:
        newImage.write(data)

def check_corrupt(checksum, ACKchecksum):
    #CHECK FOR CORRUPTION
    #return 1 for corrupt, 0 for not corrupt
    if (checksum == ACKchecksum):
        return 0
    else:
        return 1

def calc_checksum(packet_data):
    #variables for calculating checksum
    packet_data = bytearray(packet_data)
    checksum = 0
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
        checksum = (checksum + temp) % two_byte_max
        if (checksum < max(checksum, temp)):
            checksum = checksum + 1
    #return final 16 bit checksum as integer
    return checksum % two_byte_max

def Receive():
    #RECEIVE DATA PACKET
    message, clientAddress = serverSocket.recvfrom(64000)
    packet = pickle.loads(message)
    #return data, checksum to check for corruption, and sequence # to check  for duplicates
    data = packet[1]
    checksum = packet[2]
    seqnum = packet[0]
    return data, checksum, seqnum

def Send(packet):
    # SEND INDIVIDUAL PACKET
    serverSocket.sendto(pickle.dumps(packet), clientAddress)
    return

#this function is used to check if the sequence numbers match up between the sent over data and the sequence number the receiver is currently looking for. This checks for duplicates.
def check_seq(seqnum, DataSeqnum):
    isExpected = 0 #assume not a duplicate
    if not (seqnum == DataSeqnum):
        isExpected = 1
    return isExpected

#function for making an image into a packet. Take sequence #, data (1 kB), and checksum, and put into a packet. This is packet is just an array w/ length=3
def Make_Packet(seqnum, ACK, checksum):
    Packet = [seqnum, ACK, checksum]
    return Packet #this function returns the packet to be sent

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
    for i in range(0, len(corruptPackets)):
        if packet[0] == corruptPackets[i]:
            #if that seqnum is in ranPacket then corrupt that packet
            print(f"ACK {packet[0]} is corrupt")
            corruptPackets.pop(i)
            packet[1] = bytearray([1,2,3,4,5])
            return packet
    #return regular packet if not
    return packet


def lose_packet(packet):
    # for every packet
    for i in range(0, len(lossPackets) - 1):
        if packet[0] == lossPackets[i]:
            # if that numSent is in ranPacket then corrupt that packet
            lossPackets.pop(i)
            return True
    return False

############ MAIN ############
#set up server port
serverPort = 12000
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('', serverPort))

#declare variables that will be used
#name of new file
filename = 'newLAND.bmp'
#Finite State Machine (FSM). What action will occur depends on state
FSM = "wait for 0 from below" #this is the initial state at the state of the program. Other possible state is "wait for 1 from below"
#array to put data from packets
dataArr = []

print("The server is ready to receive")
#First incoming packet will tell how many packets to expect from client
message , clientAddress = serverSocket.recvfrom(64000)
numPacket = message.decode()
numPacket = int(numPacket)

numReceived = 0 #variable indicating number of packets successfully received (no duplicates or corruption)
seqnum = 0 #seqnum used to check for duplicates. First seqnum will be 0.
ACK = [0,1] #ACK 1 if packet 0 is corrupt
checksum = calc_checksum(ACK) #checksum for ACK 1 if packet 0 is corrupt
expectedseqnum = 0
sndpkt = Make_Packet(-1, ACK, checksum)

corruptPackets = list()
lossPackets = list()
corruptPackets = percent_corrupt(corruptPackets, sys.argv[1])
lossPackets = percent_corrupt(lossPackets, sys.argv[2])
lost = False

#keep sending packets until all packets are sent
while expectedseqnum < numPacket:
    data, DataChecksum, DataSeqnum = Receive()  # get data, checksum, and seqnum from received packet
    checkData = calc_checksum(data)  # calculate checksum from received data
    isCorrupted = check_corrupt(checkData, DataChecksum)  # check if data packet corrupted
    hasWrongSeqnum = check_seq(expectedseqnum,DataSeqnum)  # check is seqnum from data is expectedseqnum
    # if data corrupted or duplicate
    if isCorrupted or hasWrongSeqnum:
        print(f"packet {DataSeqnum} is corrupt or has wrong seqnum")
        sndpkt = Make_Packet(expectedseqnum-1, ACK, checksum)  # Make ACK
        sndpkt = corrupt_packet(sndpkt) #check if packet should be corrupted
        Send(sndpkt)#resend last ACK
        continue
    else:
        extractData(data, dataArr)
        deliverData(filename, dataArr[expectedseqnum])
        ACK = bytearray(expectedseqnum)
        checksum = calc_checksum(ACK)
        sndpkt = Make_Packet(expectedseqnum, ACK, checksum)  # Make ACK
        sndpkt = corrupt_packet(sndpkt) #corrupt packet
        lost = lose_packet(sndpkt) #check if lost
        if not lost: #Send packet
            Send(sndpkt)
        else: #ACK is lost
            print(f"ACK {expectedseqnum} lost")
        expectedseqnum = expectedseqnum + 1  # move on to next seqnum
