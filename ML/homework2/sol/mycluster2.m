function [ WZ,DZ,Z ] = mycluster2( bow, K )
    
% 'd = Num docs' and 'w = num words' 'K = num clusters'
nd = size(bow,1);
nw = size(bow,2);
nz = K;

%learning param
iter = 100;

%intiailization
%Pr = prior matrix representing P(Z|d,w) 
Pr = zeros(nd,nw,nz);

WZ = rand(nw,nz);
tmp = sum(WZ);
tmp = tmp(ones(1,nw),:);
WZ = WZ ./ tmp;

DZ = rand(nd,nz);
tmp = sum(DZ);
tmp = tmp(ones(1,nd),:);
DZ = DZ ./ tmp;

Z = rand(nz,1);
tmp = sum(Z);
Z = Z / tmp;
%disp(Z);
for it = 1:iter
    %E-step
    for i = 1:nz
        Pr(:,:,i) = Z(i) * (DZ(:,i) * (WZ(:,i))');
    end
    tmp = sum(Pr,3);
    Pr = Pr ./ tmp(:,:,ones(1,nz));
    Pr(isnan(Pr)) = 0;
    %M-step
    for i = 1:nz
        tmp = bow .* Pr(:,:,i);
        WZ(:,i) = (sum(tmp))';
        DZ(:,i) = sum(tmp,2);
        Z(i) = sum(sum(tmp));
    end
    tmp = sum(WZ);
    tmp = tmp(ones(1,nw),:);
    WZ = WZ ./ tmp;
    %disp(WZ);
    
    tmp = sum(DZ);
    tmp = tmp(ones(1,nd),:);
    DZ = DZ ./ tmp;
    %disp(DZ);
    
    Z = Z / (sum(Z));
    %disp(Z);
end


%disp(Z);
end