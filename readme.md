

## model2VideoHelper.py 

这个脚本用来生成Model2Video project下的配置文件中的一个model.mlt,存放的是模型列表

## model2Video .ini file

ini file Sample:

```
[path]
baseDir=/home/hejw005/Documents/3dcnn/data/ModelNet40

[model]
model=/home/hejw005/Documents/3dcnn/code/model.mlt

[matrix]
matrixFile=/home/hejw005/Documents/3dcnn/code/sample16_16_2.matrix
```

mlt file Sample:

```
bathtub/test/bathtub_0107.off
bathtub/test/bathtub_0108.off
bathtub/test/bathtub_0109.off
```

contact the baseDir and the item in mlt file, we can get the path of the model.

i. e. the basePath in model2VideoHelper.py is the same as the baseDir in the .ini file

## .matrix file

.matrix file is the input file of the Model2Video in the .ini file

the .matrix file was generated by 3DfeatureCheck project

16_16 generate 16 * 16 viewpoints for the model 16 rotations with the x axis and 16 rotations with z axis


sample16_16.matrix 

sample16_16_2.matrix

'''
Key parameters:
glm::mat4 proj = glm::perspective(glm::pi<float>() / 3, 4.0f / 3.0f, 0.5f, 10.f);
float angle_x = 2.0 * glm::pi<float>() / MAX_X_LEN;
float angle_z = 2.0 * glm::pi<float>() / MAX_Z_LEN;
'''

---

## Model rectify

I have found that there are seven models have some problems with the format of **.off** file. They are
**bathtub, desk, dresser, monitor, night_stand, sofa, and table**. And I created the python script of 
**ModelNet40Rectiry.py** to rectify these model files.


Useage:

python ModelNet40Rectify.py