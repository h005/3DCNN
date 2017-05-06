import os
import array
import numpy as np


# feaCate is one of the {'fc6-1' , 'fc7-1'}
feaCate = ['fc6-1','fc7-1']
# train_test is one of the {'train','test'}
train_test = ['train','test']
# feaLayer is one of the {'1','17'}
feaLayer = ['1','17']
# basePath for the binary features directory
basePath = '/home/hejw005/Documents/3dcnn/data/ModelNet40_resC3D'
# baseDir for the feature after parsing
baseDir = '/home/hejw005/Documents/3dcnn/data/ModelNet40Fea'

file = '/home/hejw005/Documents/3dcnn/C3D/C3D-v1.0/examples/c3d_feature_extraction/000001.fc6-1'
feature_filename = '/home/hejw005/Documents/3dcnn/C3D/C3D-v1.0/examples/c3d_feature_extraction/000001_fc6-1.csv'

# modelDict contains the model label
def featureParse(fileList,modelDict,baseDir,cate,tCate,eleLayer):

	featureFile = baseDir + '/' + cate + '/' + tCate + eleLayer + '.fea'
	featureListFile = baseDir + '/' + cate + '/' + tCate + eleLayer + '.list'
	feaFile = open(featureFile,'w')
	feaListFile = open(featureListFile,'w')
	print 'feature File ' + featureFile
	print len(fileList)
	for file in fileList:
		print file
		f = open(basePath + '/' + file,"rb")
		s = f.read()
		f.close()
		(n, c, l, h, w) = array.array("i", s[:20])
		feature_vec = np.array(array.array("f", s[20:]))
		for ele in feature_vec:
			feaFile.write('%.16f ' % ele)
		# get the model label
		# get the model name first
		spModel = file.split(tCate)[0][:-1]
		feaFile.write(str(modelDict[spModel]) + '\n')
		feaListFile.write(file + '\n')
	feaFile.close()
	feaListFile.close()


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
modelList.sort()


# read in the model list and put them into a dict
modelListfile = '/home/hejw005/Documents/3dcnn/code/model.list'
modelDict = getModelDict(modelListfile)

print len(modelDict.keys())



# print modelList

# mkdir for the different category of the features
# os.mkdir()

# extract the binary feature files with the suffix of the feaCate
for cate in feaCate: # one of the {'fc6-1','fc7-1'}
	for tCate in train_test: # one of the {'train','test'}
		for eleLayer in feaLayer: # one of the {'1','17'}
			myModelList = []
			for eleFea in modelList:
				if eleFea[-5:] == cate:
					# this ele is the category of cate
					# such as 
					# spList = ['airplane_', '_airplane_0627_1.fc6-1']
					spList = eleFea.split(tCate)
					if len(spList) == 2:
						# eleFea has the format of wardrobe_train_wardrobe_0045_1.fc6-1
						# spListdot is wardrobe_train_wardrobe_0045_1
						spListdot = eleFea.split('.')[0]
						# spList_ is one of the feaLayer
						spList_ = spListdot.split('_')[-1]

						if spList_ == eleLayer:
							myModelList.append(eleFea)
			myModelList.sort()
			featureParse(myModelList,modelDict,baseDir,cate,tCate,eleLayer)
			print cate + ' ' + tCate + ' ' + eleLayer + ' extracted done' 





# train_testModelList = getTrainTestModelfeatureList(cateModelList,train_test)		






# # get the train and test model feature file list
# trainModelList = []
# testModelList = []
# for cate in train_test:
# 	if cate == 'train':
# 		trainModelList = getTrainTestModelfeatureList(myModelList,cate)
# 	elif cate == 'test':
# 		testModelList = getTrainTestModelfeatureList(myModelList,cate)

# print trainModelList
# print testModelList
	


# save the average feature vector as a CSV
# np.savetxt(
#     feature_filename,
#     feature_vec[None, :],
#     fmt='%.16f',
#     delimiter=' '
#     )


print "write done"