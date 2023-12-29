import sensor, utime, image, time, pyb, math
from pyb import UART

EXPOSURE_TIME_SCALE = 0.8

def setup():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_auto_gain(True)
    sensor.set_auto_whitebal(True)
    sensor.set_auto_exposure(True)
    current_exposure_time_in_microseconds =  sensor.get_exposure_us()
    sensor.set_auto_exposure(True, exposure_us = int(current_exposure_time_in_microseconds* EXPOSURE_TIME_SCALE))
    clock = time.clock()
    sensor.skip_frames(time = 500)

    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(False)
    sensor.set_auto_exposure(False)
    current_exposure_time_in_microseconds = sensor.get_exposure_us()
    sensor.set_auto_exposure(False, exposure_us = int(current_exposure_time_in_microseconds * EXPOSURE_TIME_SCALE))
    sensor.skip_frames(time = 1000)

def dist(x0, y0, x1, y1):
    return math.sqrt((x0 - x1)**2 + (y0 - y1)**2)

#       pix  sm
arr = [[20, 9],
       [29, 14],
       [44, 19],
       [53, 24],
       [72, 29],
       [76, 34],
       [79, 39],
       [82, 44],
       [85, 49],
       [88, 54],
       [90, 59],
       [93, 64],
       [94, 69],
       [97, 74],
       [99, 79],
       [100, 84],
       [101, 89],
       [103, 94],
       [105, 99],
       [106, 104],
       [107, 109],
       [108, 114],
       [109, 120],
       [110, 126],
       [111, 131],
       [112, 141],
       [113, 147],
       [114, 157],
       [115, 175],
       [116, 199],
       [117, 217]]

uart = UART(3, 460800, timeout = 100, timeout_char = 100)
uart.init(460800, bits = 8, parity = False, stop = 1, timeout_char = 100)
threshold_yellow = (41, 100, -29, 127, 23, 127)#(38, 100, -128, 127, 50, 126)#(38, 100, -128, 127, 38, 125)
threshold_blue = (30, 73, -128, 127, -128, -24)#(10, 45, -34, 18, -128, -12)#(10, 45, -34, 3, -128, -7)
x0 = 184 #162 #161
y0 = 119 #117#117 #147
r0 = 125#90 #140

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

    for i in range(0, 4):
        print(data[i], "\t", sep = "", end = "")

    data[4] = crc8(data, 4)
    print(data[4])

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

    return k * pix + b

def adduction(value):
    while value < 0: value += 360
    while value > 360: value -= 360
    return value

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
    alphaB = 0
    alphaY = 0

    #####################################################FIND_BLOBS
    for yb in img.find_blobs([threshold_yellow], merge = True, margin = 15, pixel_threshold = 880):
        if blobY != False:
            if blobY.area() < yb.area():
                blobY = yb
        else:
            blobY = yb

    if blobY != False:
        img.draw_rectangle(blobY.x(), blobY.y(), blobY.w(), blobY.h(), thickness = 2)
        yellow = (blobY.x()  + blobY.w() / 2, blobY.y() + blobY.h() / 2)
        img.draw_line(int(x0), int(y0), int(yellow[0]), int(yellow[1]), thickness = 2)
        pixY = dist(x0, y0, yellow[0], yellow[1])

    for bb in img.find_blobs([threshold_blue], merge = True, margin = 15, pixel_threshold = 880):
        if blobB != False:
            if blobB.area() < bb.area():
                blobB = bb
        else:
            blobB = bb

    if blobB != False:
        img.draw_rectangle(blobB.x(), blobB.y(), blobB.w(), blobB.h(), thickness = 2)
        blue = (blobB.x()  + blobB.w() / 2, blobB.y() + blobB.h() / 2)
        img.draw_line(int(x0), int(y0), int(blue[0]), int(blue[1]), thickness = 2)
        pixB = dist(x0, y0, blue[0], blue[1])
    #####################################################END_FIND

    print(int(pixB), end = '\t')

    if blobY:
        distY = toDistance(pixY)
        alphaY = math.atan2(yellow[0] - x0, yellow[1] - y0) * 180/math.pi
    else: alphaY = 0

    if blobB:
        distB = toDistance(pixB)
        alphaB = math.atan2(blue[0] - x0, blue[1] - y0) * 180/math.pi
    else: alphaB = 0

    alphaY = adduction(alphaY)
    alphaB = adduction(alphaB)
    #dataN = (pixB, distB, alphaB / 2, pixY, distY, alphaY / 2)
    #print(pixB, end = " ")

    send_uart(distB, alphaB / 2, distY, alphaY / 2)
