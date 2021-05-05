M = 100; N = 100;
u = linspace(-M/2, M/2, M+1);
v = linspace(-N/2, N/2, N+1);

[uu, vv] = meshgrid(u, v); % uu and vv are matrice in size[M+1, N+1]

magnitude = 0.5 * abs(cos(2 * pi * vv / N) + cos(2 * pi * uu / M)); % magnitude is matrix in size[M+1, N+1]

figure(1);
% mesh(uu, vv, magnitude);
surf(uu, vv, magnitude);
xlabel("u");
ylabel("v");
zlabel("magnitude");
title("magtnitude of the frequency response function");
saveas(gcf, "./output_images/frequency_response", "png");


magnitude2 = 2 * abs(cos(2 * pi * uu / M) + cos(2 * pi * vv / N) - 2);
figure(2);
% mesh(uu, vv, magnitude2);
surf(uu, vv, magnitude2);
xlabel("u");
ylabel("v");
zlabel("magnitude");
title("magnitude of the frequency response function");
saveas(gcf, "./output_images/frequency_response2", "png");