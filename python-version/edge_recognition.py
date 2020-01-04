
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
import time

class edge_rec:
    def __init__(self, image):       
        self.image = image
        self.PILimg = Image.open(self.image)

        self.width = self.PILimg.size[0]
        self.height = self.PILimg.size[1]

        self.x_kernel = [[1, 2, 1],
                         [0, 0, 0],
                         [-1, -2, -1]]
        
        self.y_kernel = [[1, 0, -1],
                         [2, 0, -2],
                         [1, 0, -1]]

        self.kernel_size = len(self.x_kernel[0])

    # convert image to gray scale image
    def gray_scale(self):
        lst = []
        for x in range(0, self.height):
            for y in range(0, self.width):
                pixel = (y, x)
                rgb = self.PILimg.getpixel(pixel)

                gray_scale = rgb[0] / 255
                lst.append(gray_scale)

        gray_im = []
        for i in range(0, len(lst), self.width):
            gray_im.append(lst[i:i + self.width])

        return gray_im

    # apply convolution on image with a sobel filter
    def convolution(self, kernel):
        matrice = self.gray_scale()

        lst = []

        for k_j in range(len(matrice) - self.kernel_size + 1):
            for k_i in range(len(matrice[1]) - self.kernel_size + 1):
                scalar = []
                for i in range(self.kernel_size):
                    for j in range(self.kernel_size):
                        scalar.append(matrice[i + k_j][j + k_i] * kernel[i][j])
                        
                lst.append(sum(scalar))

        return lst

    def main(self):
        # plot original image
        plt.matshow(self.gray_scale(), cmap="gray")
        plt.title("original")
        plt.show()
        
        start = time.time()

        # create edges in x-axis
        x_edges = self.convolution(self.x_kernel)
        # create edges in y-axis
        y_edges = self.convolution(self.y_kernel)
        
        # combine x- and y-edges
        for i in range(len(x_edges)):
            x_edges[i] = np.sqrt(pow(x_edges[i], 2) + pow(y_edges[i], 2))
            x_edges[i] = x_edges[i]

        edges = x_edges
        
        # reshape to original height - 2 and original width - 2 (the amount of kernels that fit into height and width)
        convolutioned = np.array(edges).reshape(self.height - 2, self.width - 2)

        duration = time.time() - start
        print("took: ", duration, " sec")

        plt.matshow(convolutioned)
        plt.title("edges")
        plt.show()


if __name__ == "__main__":
    edge_rec("images/house.png").main()

