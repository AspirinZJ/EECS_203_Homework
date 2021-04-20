function imgOut = medianFilter(imgIn, kernelSize, paddingMethod)
    # apply median filter to to input image according to the kernel size and padding method

    # check if the kernel size is odd
    if mod(kernelSize, 2) != 1
        error("Error: Wrong kernel size!");
        quit;
    end

    [nRow, nCol] = size(imgIn);
    imgOut = zeros(nRow, nCol);

    if "replicate" == paddingMethod
        paddingSize = (kernelSize - 1) / 2;
        imgPad = padarray(imgIn, [paddingSize, paddingSize], "replicate"); # pad image

        for i = 1:nRow
            for j = 1:nCol
                imgOut(i, j) = median(reshape(imgPad(i:i + 2 * paddingSize, j:j + 2 * paddingSize), kernelSize^2, 1));
            end
        end
    end

end
