% Recreate results from Figure 2 in the paper. 
% The naming of M and N is opposite to the paper. Sorry for any confusion.
% written by Zorah Lähner (laehner@in.tum.de)
% version 03.2016
%% PREPARE
clc
close all
clear all

options = struct;

options.max_eigen = 25;

options.wks_variance = 6.0;
options.wks_size = 100;

options.use_wks = false;
options.use_gn_wks = false;
options.use_rn_wks = true;
options.use_hks = false;
options.use_shks = false;
options.use_gn_shks = true;
options.use_rn_shks = false;

% cat

% load('./data/cat0.mat');
% load('./data/cat3_sketch.mat');

% human

% load('./data/faust006.mat'); this is the original shape in the paper 
% but its too large for github, can be downloaded from
% vision.in.tum.de/~laehner/Elastic2D3D/faust006.mat
load('./data/faust000.mat');
load('./data/faust005_sketch.mat');


%% CALC DESCRIPTORS

M = calc_descr(M, options);
N = calc_descr(N, options);

bound = find(N.boundary == 1);
contour_length = length(bound);
bound = [bound; bound(1)];

Phi = [];
Psi = [];

if options.use_wks
    Phi = [Phi N.wks];
    Psi = [Psi M.wks];
end
if options.use_gn_wks
    Phi = [Phi N.gn_wks];
    Psi = [Psi M.gn_wks];
end
if options.use_rn_wks
    Phi = [Phi N.rn_wks];
    Psi = [Psi M.rn_wks];
end
if options.use_hks
    Phi = [Phi N.hks];
    Psi = [Psi M.hks];
end
if options.use_shks
    Phi = [Phi N.shks];
    Psi = [Psi M.shks];
end
if options.use_gn_shks
    Phi = [Phi N.gn_shks];
    Psi = [Psi M.gn_shks];
end
if options.use_rn_shks
    Phi = [Phi N.rn_shks];
    Psi = [Psi M.rn_shks];
end
Phi = [Phi(bound,:)];

symmetry_map_Psi = M.segmentation;
symmetry_map_Phi = N.segmentation;

descr_M = [M.VERT Psi symmetry_map_Psi];
descr_N = [N.VERT(bound, 1:2) zeros(contour_length+1,1) Phi [symmetry_map_Phi(bound)]];

%% OPTIMIZE

mex -v dijkstra/dijkstra.cpp dijkstra/MinHeap.cpp 

tic
[e, path, corr] = dijkstra(descr_M', M.TRIV'-1, descr_N');
toc

fprintf('Final energy: %.4f\n', e);

% path and corr come out as double and 0-index based
path = uint64(path) + 1;
corr = uint64(corr) + 1;

%% VISUALIZE

plot_path(M, N.VERT(bound, 1:2), path, corr);
