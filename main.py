import sensor, utime, image, time, pyb, math
from pyb import UART

EXPOSURE_TIME_SCALE = 0.75

def setup():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(False)
    sensor.set_auto_exposure(False)
    current_exposure_time_in_microseconds = sensor.get_exposure_us()
    sensor.set_auto_exposure(False, exposure_us = int(current_exposure_time_in_microseconds * EXPOSURE_TIME_SCALE))
    sensor.skip_frames(time = 1000)

def dist(x0, y0, x1, y1):
    return math.sqrt((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1))

arr = [[127, 185],
       [124, 180],
       [122, 175],
       [121, 170],
       [120, 164],
       [119, 150],
       [118, 140],
       [117, 135],
       [116, 133],
       [115, 127],
       [114, 122],
       [113, 114],
       [112, 111],
       [111, 106],
       [110, 105],
       [109, 103],
       [108, 98],
       [105, 93],
       [104, 88],
       [102, 83],
       [100, 78],
       [98, 73],
       [94, 68],
       [92, 63],
       [89, 58],
       [85, 53],
       [80, 48],
       [73, 43],
       [68, 38],
       [62, 33],
       [56, 28],
       [48, 23],
       [40, 18],
       [27, 13]]

uart = UART(3, 115200)
threshold_yellow = (38, 100, -128, 127, 50, 126)#(38, 100, -128, 127, 38, 125)
threshold_blue = (10, 45, -34, 3, -128, -7)
x0 = 161
y0 = 147
r0 = 140

def getLine(x0, y0, x1, y1):
    return ((y1 - y0) / (x1 - x0), y0 - x0 * (y1 - y0)/(x1 - x0))

def toDistance(pix):
    ind = -1
    count = 0
    for element in arr:
        if pix >= element[0]:
            ind = count
            break
        count += 1

    if ind == -1:
        ind = len(arr)-1
    elif ind == 0:
        ind = 1

    #print(arr[ind - 1][0], arr[ind - 1][1])
    (k, b) = getLine(arr[ind - 1][0], arr[ind - 1][1], arr[ind][0], arr[ind][1])
    #print(k, b)

    return k * pix + b

def crc8(data, len):
    crc = 0xFF
    j = 0
    for i in range(0, len):
        crc = crc ^ data[i];
        for j in range(0, 8):
            if (crc & 0x80):
                crc = (crc << 1) ^ 0x31
            else:
                crc = crc << 1
    return int(crc % 255)

setup()
clock = time.clock()

while(True):
    #####################################################INIT
    clock.tick()
    img = sensor.snapshot().mask_circle(x0, y0, r0)
    blobY = False
    blobB = False
    yellow = (0, 0)
    blue = (0, 0)
    pixY = 0
    pixB = 0
    distY = 0
    distB = 0
    alphaB = 181
    alphaY = 181

    #####################################################FIND_BLOBS
    for yb in img.find_blobs([threshold_yellow], merge = True, margin = 40, pixel_threshold = 880):
        if blobY != False:
            if blobY.area() < yb.area():
                blobY = yb
        else:
            blobY = yb

    if blobY != False:
        img.draw_rectangle(blobY.x(), blobY.y(), blobY.w(), blobY.h(), thickness = 2)
        yellow = (blobY.cx(), blobY.cy())
        img.draw_line(int(x0), int(y0), int(yellow[0]), int(yellow[1]), thickness = 2)
        pixY = dist(x0, y0, yellow[0], yellow[1])
        #print("Y:", pixY, "\t", end = '')

    for bb in img.find_blobs([threshold_blue], merge = True, margin = 40, pixel_threshold = 880):
        if blobB != False:
            if blobB.area() < bb.area():
                blobB = bb
        else:
            blobB = bb

    if blobB != False:
        img.draw_rectangle(blobB.x(), blobB.y(), blobB.w(), blobB.h(), thickness = 2)
        blue = (blobB.cx(), blobB.cy())
        img.draw_line(int(x0), int(y0), int(blue[0]), int(blue[1]), thickness = 2)
        pixB = dist(x0, y0, blue[0], blue[1])
        #print("B:", pixB, " ", end = '')
    #####################################################END_FIND
    #print()

    if blobY:
        distY = toDistance(pixY)
        alphaY = math.atan2(yellow[0] - x0, yellow[1] - y0) * 180/math.pi
    if blobB:
        distB = toDistance(pixB)
        alphaB = math.atan2(blue[0] - x0, blue[1] - y0) * 180/math.pi

    #####################################################SEND_UART
    uart.writechar(255)
    uart.writechar(int(distY))
    uart.writechar(int(alphaY))
    uart.writechar(int(distB))
    uart.writechar(int(alphaB))

    data = (int(distY), int(alphaY), int(distB), int(alphaB))
    crc = crc8(data, 4)
    for i in range(0, 4):
        print(data[i], "\t", end = "")
    print()

    uart.writechar(int(crc))
