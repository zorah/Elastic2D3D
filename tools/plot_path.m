function plot_path(M, X, path, corr)
% PLOT_PATH Creates figure showing the contour and the contour plotted 
% on M with the path and correspondences given. 
%   M - struct containing M.VERT and M.TRIV
%   X - m x 2, coordinates of contour
%   path - k x 1, indices on M
%   corr - k x 1, indices on the contour
%
% written by Zorah Lähner (laehner@in.tum.de)
% version 03.2016

figure, 

% contour

subplot(1,2,1), 
cmp = jet(size(X,1));
Cplot = [X; X(1,:)];
for k = 1:(size(X,1))
    plot(Cplot((k:k+1),1),Cplot((k:k+1),2),'color',cmp(k,:)), hold on,
end
scatter(X(:,1), X(:,2), [], cmp, 'filled'), hold on, view([0 -90]),
title(sprintf('Contour, %d points', size(X, 1))),
axis equal, axis off

% model

subplot(1,2,2),
for k = 1:(size(path,1)-1)
    plot3(M.VERT(path(k:k+1),1),M.VERT(path(k:k+1),2),M.VERT(path(k:k+1),3), '-', 'color',cmp(corr(k+1),:), 'LineWidth', 2), hold on,
%   comment in for circles at points where the contour level changes
%     if corr(k) ~= corr(k+1)
%         plot3(M.VERT(path(k),1),M.VERT(path(k),2),M.VERT(path(k),3), 'ro')
%     end
end
whites = [1 1 1];
colormap(whites)
trisurf(M.TRIV,M.VERT(:,1),M.VERT(:,2),M.VERT(:,3),ones(size(M.VERT(:,1))), 'EdgeColor', 'none', 'FaceAlpha', 1), axis equal, caxis auto, hold on; light; lighting phong
title(sprintf('Model, %d points', size(M.VERT,1))),
axis equal, axis off

end
