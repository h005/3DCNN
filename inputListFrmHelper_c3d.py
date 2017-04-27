import os;

# the total number of the photos, where every 16 is a batch
# Num_photo = 256
# only use 32 images
Num_photo = 32
# basePath is the dir contains the model
basePath = "/home/hejw005/Documents/3dcnn/data/ModelNet40"
# basePath = "/home/hejw005/Documents/3dcnn/data/ModelNetMini"
# outputBasePath is the dir contains the output result of the output prefix
outputBasePath = "/home/hejw005/Documents/3dcnn/data"
# c3d_outputBasePath is the prefix used for c3d
c3d_outputBasePath = "/home/dinggou/3dData/ModelNet40_resC3D"
# c3d_inputBasePath is the prefix used for c3d
c3d_inputBasePath = "/home/dinggou/3dData/ModelNet40"
# inputListFrmName is the output file of this program and the input file for the C3D
inputListFrmName = 'input_list_frm_model40.txt'
# outputFrmPrefixFile is the output file list for the features extracted from the C3D 
outputFrmPrefixFile = 'output_list_frm_model40.txt'
# only the model in this list file will be processed
modelListfile = 'model.list'

# get Model list
def getModelList(basePath):
	model = []
	
	for root, dirs, files in os.walk(basePath, topdown=True):
		for name in files:
			if(name[-4:] != '.jpg'):
				model.append(root.split(basePath)[1] + '/' + name[:-4] + '/')

	print 'extract Model list done'

	modelSorted = sorted(model)

	return modelSorted

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

# test the modelDict

# for ele in modelDict:
# 	print ele + " " + str(modelDict[ele])

# generate the input frame file
inputListFrmFile = open(inputListFrmName,"w")
# generate the output prefix list file
outputFrmPrefixFile = open(outputFrmPrefixFile,"w")

myModelList = getModelList(basePath)

print "get model list in " + basePath +" done"

# write to the input_list_frm.txt file
for ele in myModelList:
	modelName = ele.split('/')[1]
	print modelName
	if modelName in modelDict:
		modelLabel = modelDict[modelName]
		index = 1
		while index < Num_photo:
			inputListFrmFile.write(c3d_inputBasePath + ele + " " + str(index) +" " + str(modelLabel) + "\n")
			tmp = ele.replace('/','_')
			tmp = '/' + tmp[1:]
			outputFrmPrefixFile.write(c3d_outputBasePath + tmp + str(index) + '\n')
			index = index + 16


inputListFrmFile.close()
outputFrmPrefixFile.close()	

