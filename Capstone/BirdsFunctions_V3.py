
import numpy as np
from scipy import interpolate
#import threading
from multiprocessing import Process, Lock

def disparityAverage(Xpos,Ypos,Xlen,Ylen, filteredImg):
	DispSum = 0
	for x in range(Xpos, Xpos+Xlen):
		for y in range(Ypos, Ypos+Ylen):
			DispSum = filteredImg[y,x] + DispSum
			DispAvg = DispSum/(Xlen*Ylen)
	print(str(filteredImg[150, 350]))
	print(str(DispAvg))
	return DispAvg

	#calculate the depth of an object given a found disparity
def getDepth(disparity):
	#Baseline distance = 14cm
	#PPI = 9079
    #B = 50041 #baseline, the distance between camera centers, in pixels (px)
    B = 0.140
    f = 1430 #focal length for Logitech C270(px)
    Z = (B*f)/disparity #Z is the depth of the object (px). Comes from equation disparity = Bf/Z
    return Z
def DisparityCalc(lock):
	#lock.acquire()
	LeftLoop = np.load('LeftLoop.npy',mmap_mode='r', allow_pickle=True)
	print('Left = ' + str(LeftLoop))
	RightLoop = np.load('RightLoop.npy',mmap_mode='r', allow_pickle=True)
	print('RightLoop = ' + str(RightLoop))
	left_XValues = np.load('Left_XValues.npy',mmap_mode='r', allow_pickle=True)
	#print('Left = ' + str(left_XValues))
	Right_XValues = np.load('Right_XValues.npy',mmap_mode='r', allow_pickle=True)
	#lock.release()
	#print('right = ' + str(Right_XValues))
	if len(left_XValues) > 0 and len(Right_XValues) > 0:
		if LeftLoop <= RightLoop:
			disparity = abs(left_XValues[LeftLoop - 2] - Right_XValues[LeftLoop -2])
			print ('disparity = ' + str(disparity))
		elif RightLoop < LeftLoop:
			disparity = abs(left_XValues[RightLoop - 2] - Right_XValues[RightLoop -2 ])
			print ('disparity = ' + str(disparity))
		return getDepth(disparity)
	else:
		return 0


def extrapolate(pointListX, pointListY, extrapolatedPoint):
	N = 10
	if len(pointListX)>N:
		knownPointsX = pointListX[-N:]
		knownPointsY = pointListY[-N:]
		x = np.arange(0,N)
		Yx = knownPointsX
		fx = interpolate.interp1d(x, Yx, fill_value='extrapolate')
		Yy = knownPointsY
		fy = interpolate.interp1d(x, Yy, fill_value='extrapolate')
		extrapolatedX = fx(N+7)
		extrapolatedY = fy(N+7)
		if len(pointListX)<N+2:
			extrapolatedPoint = (int(extrapolatedX),int(extrapolatedY))
			return extrapolatedPoint
		else:
			if extrapolatedX > extrapolatedPoint[0]:
				#extrapolatedX = extrapolatedPoint[0] + 1
				extrapolatedX = (extrapolatedPoint[0] + extrapolatedX)/2
			else:
				#extrapolatedX = extrapolatedPoint[0] - 1
				extrapolatedX = (extrapolatedPoint[0] + extrapolatedX)/2
			if extrapolatedY > extrapolatedPoint[1]:
				#extrapolatedY = extrapolatedPoint[1] + 1
				extrapolatedY = (extrapolatedPoint[1] + extrapolatedY)/2
			else:
				#extrapolatedY = extrapolatedPoint[1] - 1
				extrapolatedY = (extrapolatedPoint[1] + extrapolatedY)/2
		extrapolatedPoint = (int(extrapolatedX),int(extrapolatedY))
		#print (extrapolatedPoint)
		return extrapolatedPoint

