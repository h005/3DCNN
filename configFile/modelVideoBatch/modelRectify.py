# -*- coding: utf-8 -*-
# @Author: h005
# @Date:   2017-05-06 09:59:31
# @Last Modified by:   h005
# @Last Modified time: 2017-05-06 23:43:06

import os

errModelFileList = ['/home/hejw005/Documents/3dcnn/code/configFile/modelVideoBatch/miniBatch0.log',
					'/home/hejw005/Documents/3dcnn/code/configFile/modelVideoBatch/miniBatch1.log',
					'/home/hejw005/Documents/3dcnn/code/configFile/modelVideoBatch/miniBatch2.log',
					'/home/hejw005/Documents/3dcnn/code/configFile/modelVideoBatch/miniBatch3.log']

errFileList = []

for errModelFile in errModelFileList:
	# read in each log file	
	errFile = open(errModelFile, 'r')
	# read in the .off file with error in the .log file
	while True:
		line = errFile.readline()
		line = line.strip()
		if line == '':
			break
		if len(line) > 20:
			errFileList.append(line.rsplit(' ',1)[1])

# modelRectifyMltFile = '/home/hejw005/Documents/3dcnn/code/configFile/modelRectify.mlt'
# modelRectifyMltFileHandler = open(modelRectifyMltFile,'w')
# for ele in errFileList:
# 	# print '/' + ele.split('//')[1]
# 	modelRectifyMltFileHandler.write('/' + ele.split('//')[1] + '\n')
# modelRectifyMltFileHandler.close()


for ele in errFileList:
	ele = ele.strip()
	errOffFile = ele.strip();
	rectifyOffFile = ele + '_'
	errOff = open(errOffFile,'r')
	rectifyOff = open(rectifyOffFile,'w')
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

	os.rename(rectifyOffFile,errOffFile)

