# BangC 编程练习

使用BangC编程完成下述5个操作。

在测试输出结果时，可采用与对应的纯cpu实现对比精度，常见的误差精度判断标准有 Mean absolute error(MAE) 和 average relative error(ARE)等。

$MAE=\frac{\sum_{i=1}^{n}{|CPU_{result}-MLU_{result}|}}{n}=\frac{\sum_{i=1}^{n}{|e_i|}}{n}$

$ARE=\frac{\sum{|CPU_{result}-MLU_{result}|}}{\sum|CPU_{result}|}$

## 1. Softmax

### 问题描述

利用指数化归一函数使得每一列和为1。

参考公式如下：

$softmax(x)_i=\frac{e^ {x_i}}{\sum_{j}{e^{x_j}}}$ 

通常为了避免输入较大导致指数化数值溢出，需要将所有的输入数值控制在0及0以下，即：

$softmax(x)_i=\frac{e^ {x_i-max}}{\sum_{j}{e^{x_j-max}}}$

给定输入$m\times n(20\times 256)$ 规模的矩阵 $X$，对每一列利用指数化归一函数，输出$m\times n$的归一化矩阵。

建议使用 ARE 对比精度。



### 2. Cosine相似度

### 问题描述

osine相似度是一种相似性度量，输出范围为-1到+1, 0代表无相关性，负值为负相关，正值为正相关。请实现向量间的余弦相似度计算。

参考公式如下：

$c(X,Y)=\frac{X \cdot Y}{|X||Y|}=\frac{{\sum_{i=1}^{n}{X_iY_i}}}{\sqrt{\sum_{i=1}^{n}{X_i^2}}\sqrt{\sum_{i=1}^{n}{Y_i^2}}}$

给定输入$m\times n(256\times 256)$ 规模的矩阵 $X$和$Y$,按对应列求余弦相似度，输出$1\times n$余弦相似度矩阵。

建议使用 MAE 对比精度。



## 3. Batch Normalization

### 问题描述

在神经网络训练过程中，前一层权重参数的改变会造成之后每层输入样本分布的改变，这造成了训练过程的困难。为了解决这个问题，通常会使用小的学习率和参数初始化技巧，这样导致了训练速度变慢，尤其是训练具有饱和非线性的模型。我们将这一现象定义为internal covariate shift，并提出通过规范化输入来解决。

给定输入$m\times n(128\times 256)$ 规模的矩阵 $B$，逐列求BN，输出$m\times n$标准化后的矩阵。

参考步骤：

1. 求出平均值

$\mu_B=\frac{1}{n}\sum_{i=1}^{n}{x_i}$

2. 求方差

$\sigma_B^2=\frac{1}{n}\sum_{i=1}^{n}{(x_i-\mu_B)^2}$

3. 标准化

$\hat{x_i}=\frac{x_i-\mu_B}{\sqrt{\sigma_B^2+\epsilon}}$

4. 缩放和平移

$y_i=\gamma\hat{x_i}+\beta$

建议使用 MAE 对比精度。



## 4. Triplet Loss

### 问题描述

Triplet Loss的核心是锚例、正例、负例共享模型，通过模型，将锚例与正例聚类，远离负例。

参考公式如下：

$L=max(d(a,p)-d(a,n)+margin,0)$

这里，我们指定$d$为曼哈顿距离（Manhattan Distance），即两点间对应坐标分量误差的绝对值之和：

$d(X,Y)=\sum_{i=1}^{n}|X_i-Y_i|$

最终的优化目标是拉近  $a, p$  的距离， 拉远  $a, n$  的距离。

建议使用 ARE 对比精度。



## 5. pow(x,y)

### 问题描述

实现 pow(x,y) 函数的向量、矩阵版本。

参考公式如下：

$power(X,Y)=X^Y$

给定输入$m\times n$ 规模的矩阵 $X$和$Y$，按对应位置以$X$中元素为底，$Y$中元素为指数做幂运算，输出$m\times n$的矩阵。

建议使用 ARE 对比精度。

### 
