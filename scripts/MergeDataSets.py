import os
from PIL import Image

root = "../data/dataSet"
imageSet = "../data/Images"
labelSet = "../data/Labels"

def readDir(dir_):

    files = os.listdir(dir_)
    print(files)
    for file_ in files:
        img = Image.open(os.path.join(dir_, file_))
        img.save(os.path.join(imageSet, os.path.basename(file_)))
        with open(os.path.join(labelSet, os.path.basename(file_)[:-4]+'.txt'), 'w') as f:
            print(os.path.basename(dir_))
            f.write(os.path.basename(dir_))
        print(file_ + ' done!')       

def writeDir(dir_):
    pass

if __name__ == '__main__':
    print('-----start-----')
    dirs = os.listdir(root)

    for dir_ in dirs:
        cur_dir = os.path.join(root, dir_)
        print(cur_dir)
        readDir(cur_dir)
        writeDir(cur_dir)

