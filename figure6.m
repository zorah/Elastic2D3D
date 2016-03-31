% Recreate results from Figure 2 in the paper. 
% The naming of M and N is opposite to the paper. Sorry for any confusion.
% written by Zorah Lähner (laehner@in.tum.de)
% version 03.2016
%% PREPARE
clc
close all
clear all

options = struct;

n_efcts1 = 20;
n_efcts2 = 200;

options.wks_variance = 6.0;
options.wks_size = 100;

options.use_wks = false;
options.use_gn_wks = false;
options.use_rn_wks = true;
options.use_hks = false;
options.use_shks = false;
options.use_gn_shks = true;
options.use_rn_shks = false;

% human

load('./data/faust000.mat');
load('./data/faust000_sketch.mat');
N.boundary = zeros(N.n, 1);
N.boundary(1:427) = 1;

%% CALC DESCRIPTORS

options.max_eigen = n_efcts1;

M1 = calc_descr(M, options);
N1 = calc_descr(N, options);

options.max_eigen = n_efcts2;

M2 = calc_descr(M, options);
N2 = calc_descr(N, options);

bound = find(N.boundary == 1);
contour_length = length(bound);
bound = [bound; bound(1)];

Phi1 = [];
Psi1 = [];

Phi2 = [];
Psi2 = [];

if options.use_wks
    Phi1 = [Phi1 N1.wks];
    Psi1 = [Psi1 M1.wks];
    
    Phi2 = [Phi2 N2.wks];
    Psi2 = [Psi2 M2.wks];
end
if options.use_gn_wks
    Phi1 = [Phi1 N1.gn_wks];
    Psi1 = [Psi1 M1.gn_wks];
    
    Phi2 = [Phi2 N2.gn_wks];
    Psi2 = [Psi2 M2.gn_wks];
end
if options.use_rn_wks
    Phi1 = [Phi1 N1.rn_wks];
    Psi1 = [Psi1 M1.rn_wks];
    
    Phi2 = [Phi2 N2.rn_wks];
    Psi2 = [Psi2 M2.rn_wks];
end
if options.use_hks
    Phi1 = [Phi1 N1.hks];
    Psi1 = [Psi1 M1.hks];
    
    Phi2 = [Phi2 N2.hks];
    Psi2 = [Psi2 M2.hks];
end
if options.use_shks
    Phi1 = [Phi1 N1.shks];
    Psi1 = [Psi1 M1.shks];
    
    Phi2 = [Phi2 N2.shks];
    Psi2 = [Psi2 M2.shks];
end
if options.use_gn_shks
    Phi1 = [Phi1 N1.gn_shks];
    Psi1 = [Psi1 M1.gn_shks];
    
    Phi2 = [Phi2 N2.gn_shks];
    Psi2 = [Psi2 M2.gn_shks];
end
if options.use_rn_shks
    Phi1 = [Phi1 N1.rn_shks];
    Psi1 = [Psi1 M1.rn_shks];
    
    Phi2 = [Phi2 N2.rn_shks];
    Psi2 = [Psi2 M2.rn_shks];
end
Phi1 = [Phi1(bound,:)];
Phi2 = [Phi2(bound,:)];

symmetry_map_Psi1 = M1.segmentation;
symmetry_map_Phi1 = N1.segmentation;

symmetry_map_Psi2 = M2.segmentation;
symmetry_map_Phi2 = N2.segmentation;

descr_M1 = [M1.VERT Psi1 symmetry_map_Psi1];
descr_N1 = [N1.VERT(bound, 1:2) zeros(contour_length+1,1) Phi1 [symmetry_map_Phi1(bound)]];

descr_M2 = [M2.VERT Psi2 symmetry_map_Psi2];
descr_N2 = [N2.VERT(bound, 1:2) zeros(contour_length+1,1) Phi2 [symmetry_map_Phi2(bound)]];

%% OPTIMIZE

mex -v dijkstra/dijkstra.cpp dijkstra/MinHeap.cpp 

% first eigenfunction number

tic
[e, path1, corr1] = dijkstra(descr_M1', M1.TRIV'-1, descr_N1');
toc

fprintf('Final energy: %.4f\n', e);

% path and corr come out as double and 0-index based
path1 = uint64(path1) + 1;
corr1 = uint64(corr1) + 1;

% second eigenfunction number

tic
[e, path2, corr2] = dijkstra(descr_M2', M2.TRIV'-1, descr_N2');
toc

fprintf('Final energy: %.4f\n', e);

% path and corr come out as double and 0-index based
path2 = uint64(path2) + 1;
corr2 = uint64(corr2) + 1;

%% VISUALIZE

figure, 
% contour
subplot(1,2,1), 
Cplot = [N.VERT(logical(N.boundary),:); N.VERT(1,:)];
black = [0 0 0];
for k = 1:(size(Cplot,1)-1)
    plot(Cplot((k:k+1),1),Cplot((k:k+1),2),'color',black), hold on,
end
axis equal, axis off


% shape

blue = [0 0 1];
red = [1 0 0];

subplot(1,2,2),
for k = 1:(size(path1,1)-1)
    plot3(M.VERT(path1(k:k+1),1),M.VERT(path1(k:k+1),2),M.VERT(path1(k:k+1),3), '-', 'color',blue, 'LineWidth', 2), hold on,
end
for k = 1:(size(path2,1)-1)
    plot3(M.VERT(path2(k:k+1),1),M.VERT(path2(k:k+1),2),M.VERT(path2(k:k+1),3), '-', 'color',red, 'LineWidth', 2), hold on,
end
whites = [1 1 1];
colormap(whites)
trisurf(M.TRIV,M.VERT(:,1),M1.VERT(:,2),M.VERT(:,3),ones(size(M.VERT(:,1))), 'EdgeColor', 'none', 'FaceAlpha', 1), axis equal, caxis auto, hold on; light; lighting phong
title(sprintf('Path using %d eigenfunctions', n_efcts1)),
axis equal, axis off,
view([0 90])
