import matplotlib.pyplot as plt
import numpy as np

def plotGraph(x_arr, y_arr, x_label, y_label, file_name):
    """
    Plots graph and saves image as png file.
    """

    file_name = file_name.decode("ascii")
    y_label   = y_label  .decode("ascii")
    x_label   = x_label  .decode("ascii")

    plt.figure()

    plt.plot(x_arr, y_arr)
    
    plt.ylabel(y_label)
    plt.xlabel(x_label)

    plt.savefig(file_name +  ".png")

    plt.close()