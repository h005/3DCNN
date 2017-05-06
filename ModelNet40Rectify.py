# -*- coding: utf-8 -*-
# @Author: h005
# @Date:   2017-05-01 22:39:31
# @Last Modified by:   h005
# @Last Modified time: 2017-05-01 23:43:06

import os

errModelDir = '/home/h005/Documents/3dcnn/data/ModelNet40Err'
rectiryModelDir = '/home/h005/Documents/3dcnn/data/ModelNet40Rectify'

# get Model list
def getModelList(basePath):
	model = []
	
	for root, dirs, files in os.walk(basePath, topdown=True):
		for name in files:
			if(name[-4:] != '.jpg'):
				model.append(root.split(basePath)[1] + '/' + name)

	print 'extract Model list done'

	modelSorted = sorted(model)

	return modelSorted


modelList = getModelList(errModelDir)

for ele in modelList:
	spList = ele.rsplit('/',1)
	# print ele.rsplit('/',1)
	if not os.path.exists(rectiryModelDir + spList[0]):
		# print rectiryModelDir + spList[0]
		os.makedirs(rectiryModelDir + spList[0])

	print ele
	# read in the .off file
	errOff = open(errModelDir + ele,'r')
	rectifyOff = open(rectiryModelDir + ele, 'w')

	# read in headline
	line = errOff.readline()
	print line
	rectifyOff.write(line[:3] + '\n')
	rectifyOff.write(line[3:])

	# break

	for line in errOff.readlines():
		# print line
		rectifyOff.write(line)

	errOff.close()
	rectifyOff.close()
