function prob = algorithm(q)

% plot and return the probability

    load sp500;

    %disp(price_move);

    % probability of hidden variable(= good) before observing the price
    % move current day.
    hp = 0.2;

    % Condition probability after observing the price move
    prob = 0.2;
    prob_arr = zeros(size(price_move));

    %disp(size(price_move,1));
    for i = 1:size(price_move,1)
        % calculate probability after observation of market
        yt = price_move(i);
        prob = calc_cond(yt,hp,q);
        prob_arr(i) = prob;
        % Get prior for next day
        hp = 0.2 + 0.6*prob;
    end
    figure();
    plot(prob_arr);
    figure();
    plot(price_move);
end
function cond = calc_cond(yt,hp,q)
    cy = q;
    if(yt == -1)
        cy = 1-q;
    end
    cond = cy * hp / (cy*hp + (1-cy)*(1-hp));
end