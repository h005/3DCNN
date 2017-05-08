# -*- coding: utf-8 -*-
# @Author: h005
# @Date:   2017-05-06 09:59:31
# @Last Modified by:   h005
# @Last Modified time: 2017-05-06 23:43:06

import os

errModelFile = '/home/hejw005/Documents/3dcnn/code/ModelNet40.log'

errFile = open(errModelFile,'r');
errFileList = []

while True:
	line = errFile.readline()
	line = line.strip()
	if line == '':
		break
	errFileList.append(line)
	

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

