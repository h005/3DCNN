# -*- coding: utf-8 -*-
# @Author: h005
# @Date:   2017-05-02 10:01:00
# @Last Modified by:   h005
# @Last Modified time: 2017-05-02 10:18:20

import os
import cv2

basePath = '../landmark'


def getModelList(basePath):
	model = []
	
	for root, dirs, files in os.walk(basePath, topdown=True):
		for name in files:
				model.append(root.split(basePath)[1] + '/' + name)

	# print 'extract Model list done'

	modelSorted = sorted(model)

	return modelSorted

modelList = getModelList(basePath)

prefixDir = '/home/hjzhang/landmark'

for ele in modelList:
	# print ele
	eleName = ele.split('.')[0]
	print eleName
	if not os.path.exists(basePath + eleName):
		os.makedirs(basePath + eleName)
	# read in the video file
	cap = cv2.VideoCapture(basePath + ele)
	success, image = cap.read()
	count = 1
	success = True
	while success:
		success, image = cap.read()
		name = basePath + eleName + '/%06d.jpg' % count
		print name
		cv2.imwrite(name, image)
		count = count + 1
	break