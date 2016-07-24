function [ class, centroid ] = mykmedoids( pixels, K )
%
% Your goal of this assignment is implementing your own K-medoids.
% Please refer to the instructions carefully, and we encourage you to
% consult with other resources about this algorithm on the web.
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
    
    disp('starting k-medoids');
    
    num_pixels = size(pixels,1);
    dim = size(pixels,2);
    iter = 100;
    
    %convergance params
    cost = 0;
    tolerance = 0.001;
	
    
    centroid = datasample(pixels,K);
    %disp(size(centroid));
    
    class = zeros(num_pixels,1);
    for it = 1:iter
        %cluster arrays
        centers_new = centroid;
        centers_count = zeros(K,1);
        %mag_sum = zeros(K,1);
        for i = 1:num_pixels
            point = (pixels(i,:));
            
            %cosine distance
            % numerator a column vector of size K
            dot_prod = sum(centroid.*point(ones(K,1),:),2);
            mag_point  = sqrt(sum(point(ones(K,1),:).*point(ones(K,1),:),2));
            mag_center = sqrt(sum(centroid.*centroid,2));
            dist = dot_prod./(mag_point.*mag_center);
            
            [~,I] = max(dist);
            class(i) = I;

            %calculate center
            centers_new(I,:) = centers_new(I,:) + point;
            centers_count(I) = centers_count(I) + 1;
            %mag_sum(I) = mag_sum(I)+mag_point;
        end
        centers_count = centers_count(:,ones(1,dim));
        centroid = centers_new./centers_count;
        
        curr_cost = sum(dist);
        
        if it > 20
            if abs(curr_cost-cost)/cost < tolerance
                break;
            end
        end
        cost = curr_cost;
    end
    disp('end of k-medoids')
    disp(cost);
    [class, centroid] = kmeans(pixels, K);
end