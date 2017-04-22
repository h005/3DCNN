import os;

# get Model list


def getModelList():
	model = []
	basePath = "/home/hejw005/Documents/3dcnn/data/ModelNet40/"
	for root, dirs, files in os.walk(basePath, topdown=True):
		for name in files:
			if(name[-4:] != '.jpg'):
				model.append(root.split(basePath)[1] + '/' + name)

	print 'extract Model list done'

	modelSorted = sorted(model)

	return modelSorted






basePath = '/home/hejw005/Documents/3dcnn/data/ModelNet40'

inputListFrmName = 'input_list_frm_model40.txt'

# load in the modelList
modelListFile = 'model.list'

with open(modelListFile) as f:
	modelList = f.readlines()

modelList = [x.strip() for x in modelList]


modelList = getModelList()

# for eleVec in modelList:
# 	print len(eleVec)
# 	break
# 	for ele in eleVec:
# 		print ele
# 		break
# 	break


# generate the input frame file

# for ele in modelList:
	
