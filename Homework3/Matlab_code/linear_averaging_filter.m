addpath("./utils");
clc; clear; close all;

L = 256;

# load image
imgPath = "./images/cat.raw";
rows = 480; cols = 640;
f = fopen(imgPath, 'r');
image = fread(f, rows * cols, "uint8=>uint8");
image = reshape(image, cols, rows);
image = image';
figure(1);
imshow(image);

# applay 11x11 linear averaging filter
# implement Matlab `conv2` function
imageLinearAvg = linearFilter(image, 11, "replicate");
figure(2);
imshow(uint8(imageLinearAvg));
imwrite(uint8(imageLinearAvg), "output_images/cat_linear_avg.png");

histSrc = getImgHist(uint8(image), L);
figure(3);
bar(histSrc);
# axis([0, 256, 0, 12000]);
title("source image histogram")

histDst = getImgHist(uint8(imageLinearAvg), L);
figure(4);
bar(histDst);
# axis([0, 256, 0, 12000]);
title("filtered image histogram")

waitforbuttonpress;
close all;
