import os
model = []
# modelCategory = 'bed'
# basePath = "/home/hejw005/Documents/3dcnn/data/ModelNet40/"
basePath='/home/h005/Documents/3dcnn/data/ModelNetMini'
for root, dirs, files in os.walk(basePath, topdown=True):
    for name in files:
        # model = model + ' '.join(root).join(name)
        # print root.split(basePath)[1]
        # print name
        if(name[-4:] != '.jpg'):
        	model.append(root.split(basePath)[1] + '/' + name)
        	# print root.split(basePath)[1] + '/' + name

# print 'done'
#    for name in dirs:
#        print(os.path.join(root, name))

mltFile = 'miniModel.mlt'
mltfileHandler = open(mltFile,'w')

modelSorted = sorted(model)

for ele in modelSorted:
	print ele
	mltfileHandler.write(ele[1:] + '\n')

mltfileHandler.close()
print 'done'