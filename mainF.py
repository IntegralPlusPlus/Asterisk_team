import sensor, utime, image, time, pyb, math
from pyb import UART

EXPOSURE_TIME_SCALE = 1
DELTA_ANGLE = 45
MAX_DISTANCE_BETWEEN_BLOBS = 65

uart = UART(3, 460800, timeout = 100, timeout_char = 100)
uart.init(460800, bits = 8, parity = False, stop = 1, timeout_char = 100)
threshold_yellow = (53, 100, 3, 127, 36, 127)#(59, 100, 2, 127, 36, 127)#
threshold_blue = (23, 51, -128, 0, -128, 0)#(23, 54, -128, -3, -128, 2)#
x0 = 163 #170
y0 = 130 #139
r0 = 138 #155

#       pix sm
arr = [[0, 0],
       [20, 11],
       [32, 15],
       [42, 20],
       [53, 25],
       [61, 30],
       [68, 35],
       [75, 40],
       [81, 45],
       [85, 50],
       [89, 55],
       [93, 60],
       [97, 65],
       [102, 70],
       [106, 75],
       [108, 80],
       [111, 85],
       [113, 90],
       [117, 100],
       [122, 110],
       [123, 120],
       [126, 130],
       [127, 140],
       [128, 150],
       [130, 160],
       [131, 180],
       [133, 200],
       [135, 210],
       [137, 220]]

def setup():
   sensor.reset()
   sensor.set_pixformat(sensor.RGB565)
   sensor.set_framesize(sensor.QVGA)
   sensor.set_auto_gain(True)
   sensor.set_auto_whitebal(True)
   sensor.set_auto_exposure(True)
   current_exposure_time_in_microseconds =  sensor.get_exposure_us()
   sensor.set_auto_exposure(True, \
       exposure_us = int(current_exposure_time_in_microseconds* EXPOSURE_TIME_SCALE))
   clock = time.clock()
   sensor.skip_frames(time = 2000)


   #sensor.reset() #reset camera
   sensor.set_pixformat(sensor.RGB565)
   sensor.set_framesize(sensor.QVGA)
   sensor.set_auto_gain(False)
   print(sensor.get_rgb_gain_db())
   sensor.set_auto_whitebal(False)
   sensor.set_auto_exposure(False)
   current_exposure_time_in_microseconds =  sensor.get_exposure_us()
   sensor.set_auto_exposure(False, \
       exposure_us = int(current_exposure_time_in_microseconds* EXPOSURE_TIME_SCALE))
   #sensor.set_gainceiling(2)
   clock = time.clock()
   sensor.skip_frames(time = 2000) #delay

def dist(x0, y0, x1, y1):
   return math.sqrt((x0 - x1)**2 + (y0 - y1)**2)

def toSend(n):
    if n > 255: return 255
    else: return int(n)

def crc8(data, len):
    crc = 0xFF
    j = 0
    for i in range(0, len):
        crc ^= data[i];
        for j in range(0, 8):
            if (crc & 0x80):
                crc = ((crc << 1) ^ 0x31)
            else:
             crc <<= 1
    return crc

data = bytearray(5)
def send_uart(num1, num2, num3, num4):
    uart.writechar(255)
    num1 = toSend(num1)
    num2 = toSend(num2)
    num3 = toSend(num3)
    num4 = toSend(num4)

    data[0] = num1
    data[1] = num2
    data[2] = num3
    data[3] = num4

    #for i in range(0, 4):
    #    print(data[i], "\t", sep = "", end = "")

    data[4] = crc8(data, 4)
    #print(data[4])

    uart.writechar(int(data[0]))
    uart.writechar(int(data[1]))
    uart.writechar(int(data[2]))
    uart.writechar(int(data[3]))
    uart.writechar(int(data[4]))

def getLine(x0, y0, x1, y1):
    return ((y1 - y0) / (x1 - x0), y0 - x0 * (y1 - y0)/(x1 - x0))

def toDistance(pix):
    ind = -1
    count = 0
    for element in arr:
        if pix <= element[0]:
            ind = count
            break
        count += 1

    if ind == -1:
        ind = len(arr)-1
    elif ind == 0:
        ind = 1

    (k, b) = getLine(arr[ind - 1][0], arr[ind - 1][1], arr[ind][0], arr[ind][1])
    myDist = k * pix + b
    if myDist > 254: myDist = 254

    return myDist

def adduction(value):
    while value < 0: value += 360
    while value > 360: value -= 360
    return value

def getMiddlePoint(point1, point2):
    return ((point1[0] + point2[0]) / 2, (point1[1] + point2[1]) / 2)

def funcComparison(point, status = False):
    if status: return point[1] #Y
    else: return point[0] #X

def angInSides(point): #goal not in sides of the image
    ang = 180 / math.pi * math.atan2(point[1] - y0, point[0] - x0)
    return (ang > -DELTA_ANGLE and ang < DELTA_ANGLE) or \
            (ang > 180 - DELTA_ANGLE or ang < -180 + DELTA_ANGLE)

setup()
clock = time.clock()

while(True):
    #####################################################INIT
    clock.tick()

    #inits
    yellow = (0, 0)
    blue = (0, 0)
    pixY = 0
    pixB = 0
    distY = 0
    distB = 0
    alphaB = 0
    alphaY = 0
    pointsY = []
    pointsB = []
    img = sensor.snapshot().mask_circle(x0, y0, r0)

    yellowSidesOfImg = True
    blueSidesOfImg = True

    #####################################################FIND_BLOBS
    for yb in img.find_blobs([threshold_yellow], merge = True, margin = 38, pixel_threshold = 270, area_threshold = 260): #310 300
        img.draw_rectangle(int(yb.x()), int(yb.y()), int(yb.w()), int(yb.h()), thickness = 2)
        pointsY.append([yb.x(), yb.y() + yb.h() / 2])
        pointsY.append([yb.x() + yb.w(), yb.y() + yb.h() / 2])

        #print(180 / math.pi * math.atan2(yb.y() + yb.h() / 2 - y0, yb.x() - x0), " ", end = "")
        #print(180 / math.pi * math.atan2(yb.y() + yb.h() / 2 - y0, yb.x() + yb.w() - x0), " ", end = "")

        if not angInSides([yb.x(), yb.y() + yb.h() / 2]) or not angInSides([yb.x() + yb.w(), yb.y() + yb.h() / 2]):
            yellowSidesOfImg = False

    #print("SIDE:", yellowSidesOfImg)
    if len(pointsY):
        '''if len(pointsY) == 4:
            point1 = getMiddlePoint(pointsY[0], pointsY[1])
            point2 = getMiddlePoint(pointsY[2], pointsY[3])
            distBetween = dist(point1[0], point1[1], point2[0], point2[1])
            if distBetween > MAX_DISTANCE_BETWEEN_BLOBS:
                s1 = dist(pointsY[0][0], pointsY[0][1], pointsY[1][0], pointsY[1][1])
                s2 = dist(pointsY[2][0], pointsY[2][1], pointsY[3][0], pointsY[3][1])
                if s1 > s2:
                    pointsY.pop(2)
                    pointsY.pop(2)
                else:
                    pointsY.pop(0)
                    pointsY.pop

        '''
        minValue = 200000
        maxValue = -200000
        indMin = 0
        indMax = 0

        for i in range(len(pointsY)):
            if i % 2 == 0 and funcComparison(pointsY[i], yellowSidesOfImg) < minValue:
                minValue = funcComparison(pointsY[i], yellowSidesOfImg)
                indMin = i
            if i % 2 != 0 and funcComparison(pointsY[i], yellowSidesOfImg) > maxValue:
                maxValue = funcComparison(pointsY[i], yellowSidesOfImg)
                indMax = i

        yellow = getMiddlePoint(pointsY[indMin], pointsY[indMax])
        img.draw_circle(int(yellow[0]), int(yellow[1]), 4, fill = True, color = (255, 255, 0))
        img.draw_line(int(x0), int(y0), int(yellow[0]), int(yellow[1]), thickness = 2, color = (255, 255, 0))
        pixY = dist(x0, y0, yellow[0], yellow[1])

    mxBb = 0
    bblob = []
    for bb in img.find_blobs([threshold_blue], merge = True, margin = 25, pixel_threshold = 350, area_threshold = 280):
        img.draw_rectangle(int(bb.x()), int(bb.y()), int(bb.w()), int(bb.h()), thickness = 2)
        if bb.area() > mxBb:
            mxBb = bb.area()
            bblob = bb
        #pointsB.append([bb.x(), bb.y() + bb.h() / 2])
        #pointsB.append([bb.x() + bb.w(), bb.y() + bb.h() / 2])

        #if not angInSides([bb.x(), bb.y() + bb.h() / 2]) or not angInSides([bb.x() + bb.w(), bb.y() + bb.h() / 2]):
        #    blueSidesOfImg = False

    '''if len(pointsB):
        minValue = 200000
        maxValue = -200000
        indMin = 0
        indMax = 0

        for i in range(len(pointsB)):
            if i % 2 == 0 and funcComparison(pointsB[i], blueSidesOfImg) < minValue:
                minValue = funcComparison(pointsB[i], blueSidesOfImg)
                indMin = i
            if i % 2 != 0 and funcComparison(pointsB[i], blueSidesOfImg) > maxValue:
                maxValue = funcComparison(pointsB[i], blueSidesOfImg)
                indMax = i

        #img.draw_circle(int(pointsB[indMin][0]), int(pointsB[indMin][1]), 3, fill = True, color = (255, 0, 0))
        #img.draw_circle(int(pointsB[indMax][0]), int(pointsB[indMax][1]), 3, fill = True, color = (255, 0, 0))
        blue = getMiddlePoint(pointsB[indMin], pointsB[indMax])
        img.draw_circle(int(blue[0]), int(blue[1]), 4, fill = True, color = (0, 0, 255))
        img.draw_line(int(x0), int(y0), int(blue[0]), int(blue[1]), thickness = 2, color = (0, 0, 255))
        pixB = dist(x0, y0, blue[0], blue[1])'''
    #####################################################END_FIND
    if mxBb:
        blue = [bblob.x() + bblob.w() / 2, bblob.y() + bblob.h() / 2]#getMiddlePoint(pointsB[indMin], pointsB[indMax])
        img.draw_circle(int(blue[0]), int(blue[1]), 4, fill = True, color = (0, 0, 255))
        img.draw_line(int(x0), int(y0), int(blue[0]), int(blue[1]), thickness = 2, color = (0, 0, 255))
        pixB = dist(x0, y0, blue[0], blue[1])

    if pixY:
        distY = toDistance(pixY)
        alphaY = math.atan2(yellow[0] - x0, yellow[1] - y0) * 180 / math.pi
    else: alphaY = 0

    if pixB:
        distB = toDistance(pixB)
        alphaB = math.atan2(blue[0] - x0, blue[1] - y0) * 180 / math.pi
    else: alphaB = 0

    alphaY = adduction(alphaY - 90) # -90 because camera is rotated
    alphaB = adduction(alphaB - 90) # -90 because camera is rotated
    #print(alphaY)
    #print(clock.fps())
    #print(distY, pixY)
    #print(pixB, distB, alphaB / 2, pixY, distY, alphaY / 2)
    #print(alphaY, end = " ")

    send_uart(distB, alphaB / 2, distY, alphaY / 2)
