from cv2 import cv2

path = '/home/narrow/QRCodeDetector/IMG.jpg'
dsize = (800, 600)

img = cv2.imread(path)
resized_img = cv2.resize(img, dsize)
cv2.imwrite(path, resized_img)
