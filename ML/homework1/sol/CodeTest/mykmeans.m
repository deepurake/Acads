function [ class, centroid ] = mykmeans( pixels, K )
%
% Your goal of this assignment is implementing your own K-means.
%
% Input:
%     pixels: data set. Each row contains one data point. For image
%     dataset, it contains 3 columns, each column corresponding to Red,
%     Green, and Blue component.
%
%     K: the number of desired clusters. Too high value of K may result in
%     empty cluster error. Then, you need to reduce it.
%
% Output:
%     class: the class assignment of each data point in pixels. The
%     assignment should be 1, 2, 3, etc. For K = 5, for example, each cell
%     of class should be either 1, 2, 3, 4, or 5. The output should be a
%     column vector with size(pixels, 1) elements.
%
%     centroid: the location of K centroids in your result. With images,
%     each centroid corresponds to the representative color of each
%     cluster. The output should be a matrix with size(pixels, 1) rows and
%     3 columns. The range of values should be [0, 255].
%     
%
% You may run the following line, then you can see what should be done.
% For submission, you need to code your own implementation without using
% the kmeans matlab function directly. That is, you need to comment it out.

    disp('starting k-means');
    tic;
    %initial variables, input params (properties from input)
    num_pixels = size(pixels,1);
    dim = size(pixels,2);
    centroid = datasample(pixels,K);
    
    % define learning rate param and initialize cost
    iter = 100;
    cost = 0;

    %convergance params
    tolerance = 0.001;
    
    class = zeros(num_pixels,1);
    for it = 1:iter
        centers_new = centroid;
        centers_count = zeros(K,1);
        for i = 1:num_pixels
            
            %current point
            point = (pixels(i,:));
            
            %calculate Distance value (D)
            dist_vec = centroid-point(ones(K,1),:);
            dist_vec = dist_vec.*dist_vec;
            dist = sum(dist_vec,2);
            
            % find closest cluster (I)
            [~,I] = min(dist);

            %parameters used to calculate new cluster.
            centers_new(I,:) = centers_new(I,:) + point;
            class(i) = I;
            centers_count(I) = centers_count(I) + 1;
        end
        %calculate the cluster center value
        centers_count = centers_count(:,ones(1,dim));
        centroid = centers_new./centers_count;
        
        %calculate current cost(sum  of J or D functions defined above)
        curr_cost = sum(dist);
        
        %experimental  code to speed up the function. exit if the
        %%increment is less than tolerance level defined
        if it > 20
            if abs(curr_cost-cost)/cost < tolerance
                break;
            end
            if(curr_cost > cost)
                %disp('error');
                %disp(curr_cost);
                %disp(cost);
            end
        end
        
        %update final cost to current cost.(unless cost decreased)
        cost = curr_cost;
    end
    toc;
    disp('exiting k-means');
    disp(cost);
    
end


