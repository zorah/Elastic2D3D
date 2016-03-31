function M = calc_descr(M, options)
% CALC_DESCR Adds (normalized) variants of the heat and wave kernel signature 
% to M. 
%
% written by Zorah Lähner (laehner@in.tum.de)
% version 03.2016

% WKS
% row-wise or completely normalized

M.wks = calc_WKS(M, options.wks_size, options.wks_variance, options.max_eigen);

M.gn_wks = M.wks ./ max(max(M.wks));
M.rn_wks = M.wks ./ repmat(max(M.wks,[],2), 1, size(M.wks,2));

% HKS and scaled HKS
% row-wise or completely normalized

M.hks = calc_HKS(M, options.max_eigen);
M.hks = M.hks ./ repmat(max(M.hks,[],2), 1, size(M.hks,2));

M.shks = M.hks ./ repmat(sum(M.hks .* repmat(full(diag(M.S)), 1, size(M.hks,2))), M.n, 1);

M.gn_shks = M.shks ./ max(max(M.shks));

M.rn_shks = M.shks ./ repmat(max(M.shks,[],2), 1, size(M.shks,2));

end
