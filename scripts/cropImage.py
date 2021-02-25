from cv2 import cv2

path = '/home/narrow/QRCodeDetector/IMG.jpg'
dsize = (900, 1200)

def crop(path):
    img = cv2.imread(path)
    width = img.shape[0]
    height = img.shape[1]
    print(width, height)
    res = img[int(width/2-dsize[0]/2):int(width/2+dsize[0]/2), int(height/2-dsize[1]/2):int(height/2+dsize[1]/2)]
    cv2.imwrite(path, res)

crop(path)
