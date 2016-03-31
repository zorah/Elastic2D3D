function [WKS] = calc_WKS(M, wks_size, wks_variance, n)
% CALC_WKS Computes wks_size wave kernel signatures on M with n eigenfunctions
% using wks_variance. 
%
% written by Emanuele Rodolà

WKS = zeros(M.n, wks_size);

log_E = log(max(M.evals(1:n), 1e-6))';
e = linspace(log_E(2), (max(log_E))/1.02, wks_size);
sigma = (e(2)-e(1)) * wks_variance;

C = zeros(1, wks_size);
for i = 1:wks_size
    WKS(:,i) = sum(...
        (M.evecs(:,1:n)).^2 .* repmat( exp((-(e(i) - log_E).^2) ./ (2*sigma.^2)),M.n,1), 2);
    C(i) = sum(exp((-(e(i)-log_E).^2)/(2*sigma.^2)));
end

WKS(:,:) = WKS(:,:)./repmat(C,M.n,1);

end
