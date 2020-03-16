# # import cv2
# 删除一行：command+x
#
# import numpy
# import pandas
# print("1111")
# print("222")
# print("第一次提交")

# cv2.imread()
# print("第二次提交")


# print("第三次提交")
import numpy as np
import cv2
img = cv2.imread('1.jpg', cv2.IMREAD_UNCHANGED)
cv2.namedWindow('img', cv2.WINDOW_AUTOSIZE)
cv2.imshow('img', img)
cv2.waitKey(0)
