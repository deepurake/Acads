function [ U, V ] = myRecommender( rateMatrix, lowRank )
    % Please type your name here:
    name = 'Rakesh, S';
    disp(name); % Do not delete this line.

    % Parameters
    maxIter = 2000; % Choose your own.
    learningRate = 0.000584; % Choose your own.
    regularizer = 0.4; % Choose your own.
    
    % Random initialization:
    [n1, n2] = size(rateMatrix);
    U = rand(n1, lowRank) / lowRank;
    V = rand(n2, lowRank) / lowRank;

    % Gradient Descent:
    
    % IMPLEMENT YOUR CODE HERE.
    
    learningRate_double = 2*learningRate;
    factor = 1-(2* regularizer * learningRate);
    prev = 100;
    count = 0;
    for i=1:maxIter
        %temp matrices used to calculate
        %diff is gonna be 2(M-UV') which will be used for gradient calculation.
        diff = (rateMatrix - U*V').*(rateMatrix > 0);
        rmse = norm(diff, 'fro') / sqrt(nnz(rateMatrix > 0));
        if(rmse > prev)
            count = count + 1;
        else
            count = 0;
        end
        if(count == 3)
            break
        end
        prev = rmse;
        diff = diff .* learningRate_double;
        %calclulation of new U,V
        %disp(sum(sum(diff)));
        U1 = factor*U + diff * V;
        V1 = factor*V + diff'* U;
        U = U1;
        V = V1;
    end


end