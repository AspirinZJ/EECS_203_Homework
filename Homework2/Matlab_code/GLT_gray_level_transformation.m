# Assume L = 256 and generate a lookup table that maps input gray-levels to output gray-levels for a power law transform with γ = 0.4 and γ = 2.5
clc; clear; close all;
L = 256;
grayLevelIn = 0 : (L-1);

# γ = 0.4 and 2.5
grayLevelGamma1 = grayLevelIn .^ 0.4;
grayLevelGamma1 = round((L - 1)/max(grayLevelGamma1) * grayLevelGamma1);
grayLevelGamma2 = grayLevelIn .^ 2.5;
grayLevelGamma2 = round((L - 1)/max(grayLevelGamma2) * grayLevelGamma2);

# show plots
figure(1);
plot(grayLevelIn, grayLevelGamma1);
xlabel("r"); ylabel("s"); title("\gamma = 0.4");
axis([0, L-1, 0, L-1]);
grid on;
saveas(gcf, "./output_images/glt_gamma_0.4", "png");

figure(2);
plot(grayLevelIn, grayLevelGamma2);
xlabel("r"); ylabel("s"); title("\gamma = 2.5");
axis([0, L-1, 0, L-1]);
grid on;
saveas(gcf, "./output_images/glt_gamma_2.5", "png");

# Load image
rows = 480; cols = 640;
f = fopen("./cat.raw", 'r');
image = fread(f, rows*cols, "uint8=>uint8");
image = reshape(image, cols, rows);
image = image';
figure(3);
imshow(image);
imwrite(image, "./output_images/cat_original.png");

imageGamma1 = grayLevelGamma1(image + 1); # +1是因为Matlab从1开始计数
figure(4);
imshow(uint8(imageGamma1));
imwrite(uint8(imageGamma1), "./output_images/cat_gamma_0.4.png");

imageGamma2 = grayLevelGamma2(image + 1);
figure(5);
imshow(uint8(imageGamma2));
imwrite(uint8(imageGamma2), "./output_images/cat_gamma_2.5.png");

waitforbuttonpress; # 等待按键
close all;