# -*- coding: utf-8 -*-
# @Author: h005
# @Date:   2017-05-14 19:12:06
# @Last Modified by:   h005
# @Last Modified time: 2017-05-14 20:35:08

import os
import shutil

srcDir = '/home/h005/Documents/3dcnn/code/3DCNN/trackResult/'
trackFile = 'train_chair_0026.track'

imgDir = '/home/h005/Documents/3dcnn/data/ModelNetMini/wardrobe/'

dstDir = '/home/h005/Documents/3dcnn/data/ModelNet40Track/'
# cate[0] is train or test
cate = trackFile.split('_',1)
print cate

dstFolder = dstDir + cate[0] + '/'
imgFolder = imgDir + cate[0] + '/' + cate[1].split('.')[0]
print imgDir
# load in the track files
with open(srcDir + trackFile) as f:
	imgList = f.readlines()
imgList = [x.strip() for x in imgList]
# imgList = sorted(imgList)
print imgList

# extract the files into the dstFolder
counter = 1
for ele in imgList:
	shutil.copyfile(imgFolder + '/' + ele, dstFolder + cate[1].split('.')[0] + '_' + str(counter) + '.jpg')
	print 'copy to ' + dstFolder + cate[1].split('.')[0] + '_' + str(counter) + '.jpg'
	counter = counter + 1
print 'done'

