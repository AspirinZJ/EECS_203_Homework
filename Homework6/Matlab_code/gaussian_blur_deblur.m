clc; clear; close all;

# Load image
%rows = 480; cols = 640;
%f = fopen("./triangle.raw", 'r');
%image = fread(f, rows*cols, "uint8=>uint8");
%image = reshape(image, cols, rows);
%image = image';
%figure(1);
%imshow(image);
image = imread("./triangle.png");
kernel = fspecial('gaussian', [31, 31], 7);
imageBlur = conv2(image, kernel);
figure(1);
imshow(uint8(imageBlur));

kernelDFT = fft2(kernel, size(imageBlur, 1), size(imageBlur, 2));
imageBlurDFT = fft2(imageBlur);
division = imageBlurDFT ./ kernelDFT;

imageDeblur = real(ifft2(division));
figure(2);
imshow(uint8(imageDeblur));
imageOut = imageDeblur(1:480, 1:640);
figure(3);
imshow(uint8(imageOut));
imwrite(uint8(imageOut), "deblurred_image.png");