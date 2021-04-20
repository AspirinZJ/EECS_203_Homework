function hist = getImgHist(imgIn, grayScale)
    # get histogram of image according to the grayScale
    hist = zeros(1, grayScale);
    [nRow, nCol] = size(imgIn);
    for i = 1:nRow
        for j = 1:nCol
            hist(imgIn(i, j) + 1) = hist(imgIn(i, j) + 1) +1; # +1是因为matlab从1开始
        end
    end
end
