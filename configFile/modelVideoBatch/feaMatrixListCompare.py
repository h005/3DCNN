import os

feaListFile = '/media/hejw005/code/3dcnn/data/fea.list'
matrixListFile = '/media/hejw005/code/3dcnn/data/matrix.list'


def readInFiles(file):
	with open(file) as f:
		myList = f.readlines()
	myList = [x.strip() for x in myList]
	myList = sorted(myList)
	return myList

feaList = readInFiles(feaListFile)
matrixList = readInFiles(matrixListFile)

index = 0
print len
while index < len(feaList):
	feaPrefix = feaList[index].split('.')[0]
	matrixPrefix = matrixList[index].split('_ViewpointEntropy')[0]
	if feaPrefix == matrixPrefix:
		print feaList[index]
		print matrixList[index]
	else:
		print feaList[index] + ' ----- diff'
		print matrixList[index] + ' ----- diff'
		break
	index = index + 1

print 'done'