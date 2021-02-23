import os
from cv2 import cv2
from PIL import Image

imageSet = "../data/tmp"

def dir_resize(dir_):
	files = os.listdir(dir_)
	for file_ in files:
		print(os.path.join(dir_, file_))
		img = cv2.imread(os.path.join(dir_, file_), 1)
		img = cv2.resize(img, (720, 960))
		cv2.imwrite(os.path.join(dir_, file_), img)

if __name__ == '__main__':
	
	
	dir_resize(imageSet)
