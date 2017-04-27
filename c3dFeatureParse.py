import os
import array
import numpy as np


# feaCate is one of the {'fc6-1' , 'fc7-1'}
feaCate = ['fc6-1','fc7-1']
# train_test is one of the {'train','test'}
train_test = ['train','test']
# basePath
basePath = '/home/hejw005/Documents/3dcnn/code'


file = '/home/hejw005/Documents/3dcnn/C3D/C3D-v1.0/examples/c3d_feature_extraction/000001.fc6-1'
feature_filename = '/home/hejw005/Documents/3dcnn/C3D/C3D-v1.0/examples/c3d_feature_extraction/000001_fc6-1.csv'

def featureParse(fileList):

	f = open(file,"rb")

	s = f.read()
	f.close()

	# print s

	(n, c, l, h, w) = array.array("i", s[:20])
	feature_vec = np.array(array.array("f", s[20:]))


def getModelDict(modelListfile):
	# load in the model list
	with open(modelListfile) as f:
		modelList = f.readlines()
	modelList = [x.strip() for x in modelList]
	modelList = sorted(modelList)

	# convert the mode list to a dict
	modelDict = {}
	counter = 0
	for ele in modelList:
		modelDict[ele] = counter;
		counter += 1
	return modelDict


def getTrainTestModelfeatureList(myModelList, train_test):
	modelList = []
	if train_test == 'train':
		for ele in myModelList:
			# this ele is the category of cate
			# such as 
			# spList = ['airplane_', '_airplane_0627_1.fc6-1']
			spList = ele.split(train_test)
			# this file belongs to the train folder
			if len(spList) == 2:
				# modelName = spList[0][:-1]
				# print modelName
				modelList.append(ele)
	elif train_test == 'test':
		for ele in myModelList:
			spList = ele.split(train_test)
			# this file belongs to the test folder
			if len(spList) == 2:
				modelList.append(ele)
	return modelList

	




# binary feature list c3d feature file
bfl3d = '/home/hejw005/Documents/3dcnn/data/ModelNet40_resC3D.list'
# the bfl3d file looks like:
# airplane_test_airplane_0627_17.fc6-1
# airplane_test_airplane_0627_17.fc7-1
# airplane_test_airplane_0627_1.fc6-1
# airplane_test_airplane_0627_1.fc7-1

# read in the bfl3d file and get the feature file list
with open(bfl3d) as f:
	modelList = f.readlines()
modelList = [x.strip() for x in modelList]
modelList = sorted(modelList)

print modelList

# extract the binary feature files with the suffix of the feaCate
for cate in feaCate:
	for eleFea in modelList:
		if eleFea[-5:] == feaCate:
			myModelList.append(elefea)









# extract the binary feature files with the suffix of feaCate
myModelList = []
for cate in feaCate:
	if cate == feaCate[1]:

for eleFea in modelList:
	if eleFea[-5:] == feaCate:
		myModelList.append(eleFea)

print myModelList


# read in the model list and put them into a dict
modelListfile = '/home/hejw005/Documents/3dcnn/code/model.list'
modelDict = getModelDict(modelListfile)

# get the train and test model feature file list
trainModelList = []
testModelList = []
for cate in train_test:
	if cate == 'train':
		trainModelList = getTrainTestModelfeatureList(myModelList,cate)
	elif cate == 'test':
		testModelList = getTrainTestModelfeatureList(myModelList,cate)

print trainModelList
print testModelList
	


# save the average feature vector as a CSV
# np.savetxt(
#     feature_filename,
#     feature_vec[None, :],
#     fmt='%.16f',
#     delimiter=' '
#     )


print "write done"