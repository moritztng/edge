import cv2
import numpy as np

image_line = None

with open("gdb.txt") as file:
    for line in file:
        if line[0] == "~":
            image_line = line
data = image_line.replace('}', '{').split('{')[-2]
data = data.split(',')
data = list(map(int, data))
data = np.reshape(data, (60, 80))
cv2.imwrite('image.png', data)
