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

label = 0
dictLabel = {}

ttCounter = 0

testFileName = 'test.txt'
trainFileName = 'train.txt'

testFile = open(testFileName,'w')
trainFile = open(trainFileName,'w')

for ele in modelList:
	ttCounter = ttCounter + 1
	cap = cv2.VideoCapture(basePath + ele)
	eleName = ele.split('.')[0]
	# print prefixDir + eleName 
	# print eleName
	spList = eleName.split('/')
	if spList[1] not in dictLabel:
		dictLabel[spList[1]] = label
		label = label + 1
	length = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
	# print cap.get(CV_CAP_PROP_FRAME_COUNT)
	# print length
	count = 0
	while count < length:
		if ttCounter < 8:
			trainFile.write(prefixDir + eleName + '/' + str(count) + ' ' + str(dictLabel[spList[1]]) + '\n')
		else:
			testFile.write(prefixDir + eleName + '/' + str(count) + ' ' + str(dictLabel[spList[1]]) + '\n')
		count = count + 16
	
	ttCounter = ttCounter % 10

testFile.close()
trainFile.close()