import numpy as np
import os
import time
os.environ['MLU_VISIBLE_DEVICES']=""
import tensorflow as tf

def squared_difference_op(input_x,input_y):
    ##return (x-y)**2
    with tf.Session() as sess:
      x = tf.placeholder(tf.float32, name='x')
      y = tf.placeholder(tf.float32, name='y')
      z = tf.squared_difference(x,y)
#      z = tf.pow(x-y,2)
      return sess.run(z, feed_dict = {x: input_x, y: input_y})

def main():
    start = time.time()
    input_x = np.loadtxt("../data/in_x.txt",delimiter=',')
    input_y = np.loadtxt("../data/in_y.txt") 
    output = np.loadtxt("../data/out.txt")
   #print(input_x)
    end = time.time()
    print("load data cost "+ str((end-start)*1000) + "ms" )
    start = time.time()
    res = squared_difference_op(input_x, input_y)
    end = time.time()
    print("comput op cost "+ str((end-start)*1000) + "ms" )
    err = sum(abs(res - output))/sum(output)
    print("err rate= "+ str(err*100))


if __name__ == '__main__':
    main()
