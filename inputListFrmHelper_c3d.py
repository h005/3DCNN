import os;

# the total number of the photos, where every 16 is a batch
Num_photo = 256

# get Model list
def getModelList():
	model = []
	basePath = "/home/hejw005/Documents/3dcnn/data/ModelNetMini"
	for root, dirs, files in os.walk(basePath, topdown=True):
		for name in files:
			if(name[-4:] != '.jpg'):
				model.append(root.split(basePath)[1] + '/' + name[:-4] + '/')

	print 'extract Model list done'

	modelSorted = sorted(model)

	return modelSorted

basePath = '/home/hejw005/Documents/3dcnn/data/ModelNet40'

inputListFrmName = 'input_list_frm_model40.txt'

# load in the model list
modelListfile = 'model.list'
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

myModelList = getModelList()

# write to the input_list_frm.txt file
for ele in myModelList:
	modelName = ele.split('/')[1]
	modelLabel = modelDict[modelName]
	
	index = 1
	while index < Num_photo:
		inputListFrmFile.write(basePath + ele + " " + str(index) +" " + str(modelLabel) + "\n")
		index = index + 16


inputListFrmFile.close()
	
