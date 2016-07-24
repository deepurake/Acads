function prob = algorithm(q)

% plot and return the probability

    load sp500;

    %disp(price_move);

    % probability of hidden variable(= good) before observing the price
    % move current day.
    alpha_good = ones(size(price_move));
    alpha_bad = ones(size(price_move));
    beta_good = ones(size(price_move));
    beta_bad = ones(size(price_move));
    alpha_good(1) = cond_prob(price_move(1),q)* 0.2;
    alpha_bad(1) = (1-cond_prob(price_move(1),q))* 0.8;

    len = size(price_move,1);
    %disp(size(price_move,1));
    for i = 2:len
        alpha_good(i) = cond_prob(price_move(i),q)*(alpha_good(i-1) * 0.8 + alpha_bad(i-1)*0.2);
        alpha_bad(i) = (1-cond_prob(price_move(i),q))*(alpha_good(i-1) * 0.2 + alpha_bad(i-1)*0.8);
    end
    for j = 1:(len-1)
        i = len-j;
        beta_good(i) = (0.8 * cond_prob(price_move(i+1),q) * beta_good(i+1) + ...
            0.2 * (1- cond_prob(price_move(i+1),q)) * beta_bad(i+1));
        beta_bad(i) = (0.2 * cond_prob(price_move(i+1),q) * beta_good(i+1) + ...
            0.8 * (1- cond_prob(price_move(i+1),q)) * beta_bad(i+1));
    end
    %calc P(X)
    disp(alpha_good);
    disp(alpha_bad);
    disp(beta_good);
    disp(beta_bad);
    obs_prob = alpha_good(len) + alpha_bad(len); 
    prob_arr = alpha_good.*beta_good;
    prob_arr = prob_arr/obs_prob;
    prob_arr(1) = prob_arr(1);
    figure();
    plot(prob_arr);
    prob = prob_arr(len);
end
function cond = cond_prob(obs,q)
    if(obs == 1)
        cond = q;
    else
        cond = 1-q;
    end
end