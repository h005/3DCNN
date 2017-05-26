# -*- coding: utf-8 -*-
# @Author: h005
# @Date:   2017-05-25 20:00:36
# @Last Modified by:   h005
# @Last Modified time: 2017-05-25 20:18:54

# generate the video frames for C3D
# extract the 000001.jpg to 000512.jpg as the frames, and sample every 16 images.
# save to the folder of ModelNet40Data and rename the file as 000001.jpg to 000032.jp

import os
import shutil

def getModelFolderList(basePath):
	modelFolder = []
	for root, dirs, files in os.walk(basePath, topdown=True):
		for name in files:
			if(name[-4:] == '.off'):
				modelFolder.append(root.split(basePath)[1] + '/' + name[:-4])

	print 'extract Model folder list done'

	modelFolderSorted = sorted(modelFolder)

	return modelFolderSorted

def copyAndRenameTo(srcPath, modelFolderSorted, dstFolder, fromId, toId):

	for eleModel in modelFolderSorted:
		if not os.path.isdir(dstFolder + eleModel):
			os.makedirs(dstFolder + eleModel)
		print dstFolder + eleModel
		index = fromId
		while index <= toId:
			srcImgFile = srcPath + eleModel + '/%06d' % ( index ) + '.jpg'
			dstImgFile = dstFolder + eleModel + '/%06d' % ( index - fromId + 1) + '.jpg'
			index = index + 1
			shutil.copyfile(srcImgFile, dstImgFile)
			print 'copy ' + srcImgFile
			print 'to ' + dstImgFile

	print 'done'

def trajectoryCopyAndRenameTo(srcPath, modelFolderSorted, dstFolder, sampleNum, sampleStep):

	for eleModel in modelFolderSorted:
		if not os.path.isdir(dstFolder + eleModel):
			os.makedirs(dstFolder + eleModel)
		print dstFolder + eleModel
		index = 0
		while index < sampleNum:
			srcImgFile = srcPath + eleModel + '/%06d' % ( index * sampleStep + 1 ) + '.jpg'
			dstImgFile = dstFolder + eleModel + '/%06d' % ( index + 1 ) + '.jpg'
			shutil.copyfile(srcImgFile, dstImgFile)
			print 'copy ' + srcImgFile
			print 'to ' + dstImgFile
			index = index + 1

basePath = '/home/h005/Documents/3dcnn/data/ModelNetMini'
modelFolderSorted = getModelFolderList(basePath)

dstFolder = '/home/h005/Documents/3dcnn/data/ModelNetMiniTrackData'

srcFolder = '/home/h005/Documents/3dcnn/data/ModelNet40Track'

trajectoryCopyAndRenameTo(srcFolder, modelFolderSorted, dstFolder, 32, 16)

