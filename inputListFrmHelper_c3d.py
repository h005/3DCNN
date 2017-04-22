import os;

# get Model list


def getModelList():
	model = []
	basePath = "/home/h005/Documents/3dcnn/data/ModelNetMini"
	for root, dirs, files in os.walk(basePath, topdown=True):
		for name in files:
			if(name[-4:] != '.jpg'):
				print root.split(basePath)
				model.append(root.split(basePath)[1] + '/' + name[:-4] + '/')

	print 'extract Model list done'

	modelSorted = sorted(model)

	return modelSorted

basePath = '/home/hejw005/Documents/3dcnn/data/ModelNet40'

inputListFrmName = 'input_list_frm_model40.txt'

myModelList = getModelList()

for ele in myModelList:
	print basePath + ele


# generate the input frame file

# for ele in modelList:
	
