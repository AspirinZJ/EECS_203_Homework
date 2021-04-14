clc; clear; close all;

L = 256;
grayLevelIn = 0 : (L-1);

# load image
rows = 480; cols = 640;
f = fopen("./cat.raw", 'r');
image = fread(f, rows*cols, "uint8=>uint8");
image = reshape(image, cols, rows);
image = image';
figure(1);
imshow(image);

# get the histgram and accumulated distribution of the source image
# [histSrc, bins] = imhist(image, L);
histSrc = zeros(1, L);
for i = 1:rows
  for j = 1:cols
    histSrc(image(i, j)+1) = histSrc(image(i, j)+1) + 1; # +1是因为matlab从1开始
  end
end
cumuDistSrc = cumsum(histSrc); # 累加

figure(2);
bar(histSrc);
grid on;
axis([0, 256, 0, 10000]);
xlabel("graylevel");
ylabel("number of pixels");
title("original histogram");
saveas(gcf, "./output_images/original_hist", "png");

# calculate the accumulated distribution of the desired image
histDesire = ones(1, L) * (rows * cols / L);
cumuDistDesire = cumsum(histDesire);

src2dstGrayLevel = zeros(1, 256); # map the gray level on source image to gray level on target image
for i = 1:L
  # 寻找最相近处的灰度
  [minVal, minInd] = min(abs(cumuDistDesire - cumuDistSrc(i)));
  src2dstGrayLevel(i) = minInd - 1; # -1 是因为matlab从1开始计算
end

imageEqu = src2dstGrayLevel(image+1);
figure(3);
imshow(uint8(imageEqu));
imwrite(uint8(imageEqu), "./output_images/cat_equ.png");

histDst = zeros(1, L);
for i = 1:rows
  for j = 1:cols
    histDst(imageEqu(i, j)+1) = histDst(imageEqu(i, j)+1) + 1; # +1是因为matlab从1开始
  end
end

figure(4);
bar(histDst);
grid on;
axis([0, 256, 0, 10000]);
xlabel("graylevel");
ylabel("number of pixels");
title("equalized histogram");
saveas(gcf, "./output_images/equ_hist", "png");

waitforbuttonpress;
close all;