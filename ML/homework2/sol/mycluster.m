function [ class ] = mycluster( bow, K )
%
% Your goal of this assignment is implementing your own text clustering algo.
%
% Input:
%     bow: data set. Bag of words representation of text document as
%     described in the assignment.
%
%     K: the number of desired topics/clusters. 
%
% Output:
%     class: the assignment of each topic. The
%     assignment should be 1, 2, 3, etc. 
%
% For submission, you need to code your own implementation without using
% any existing libraries

% YOUR IMPLEMENTATION SHOULD START HERE!

%Parse input for intialization
% 'd = Num docs' and 'w = num words' 'K = num clusters'
d = size(bow,1);
nw = size(bow,2);

%learning parama
iter = 400;

%all params and posteriors
r = zeros(d,K);

mu = rand(nw,K);
w = sum(mu,2);
mu = mu ./ w(:,ones(1,K));

pi = rand(K,1);
w = sum(pi);
pi = pi./w;

for it = 1:iter
    % E-step
    for i = 1:d
        data = (bow(i,:))';
        data = data(:,ones(1,K));
        A = mu .^ data;
        A = prod(A);
        A = A .* pi';
        r(i,:) = A;
    end
    w = sum(r,2);
    r = r./ w(:,ones(1,K));
    
    % M-step
    %disp(size(pi));
    mu = bow' * r;
    w = sum(mu);
    w = w(ones(nw,1),:);
    mu = mu ./ w;
    
    pi = sum(r);
    pi = (pi')/d;
    %disp(size(pi));
    
end

[~,class] = max(r');
class = class';
end

