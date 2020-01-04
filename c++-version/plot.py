import matplotlib.pyplot as plt
import json
import os

try:
    os.remove("result_files/result_matrix.json")
except:
    pass

try:
    os.rename(r"result_files/result_matrix.txt", r"result_files/result_matrix.json")
except:
    pass

with open("result_files/result_matrix.json", "r") as f:
    matrix = json.load(f)

plt.matshow(matrix)
plt.show()
