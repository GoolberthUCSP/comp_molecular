import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
import numpy as np
import sys

abs_dir = sys.path[0]
args = sys.argv

if __name__ == "__main__":
    with open(abs_dir + "\\data\\best_result.txt", "r") as f:
        s1 = f.readline()
        s1 = f.readline()
        s2 = f.readline()
        mtx = np.full((len(s1), len(s2)), False, dtype=np.bool_)
        for i in range(len(s1)):
            for j in range(len(s2)):
                mtx[i][j] = s1[i] == s2[j]
        plt.figure(figsize=(5, 5))
        plt.title("Best alignment matrix")
        plt.xlabel(args[1])
        plt.ylabel(args[2])
        colormap = ListedColormap(['white', 'black'])
        plt.imshow(mtx, cmap=colormap)
        # plt.show()
        plt.savefig(abs_dir + "\\data\\" + args[1] + "_" + args[2] + ".png")