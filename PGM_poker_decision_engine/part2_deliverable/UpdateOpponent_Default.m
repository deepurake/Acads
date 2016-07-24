%% Update Opponent Model
%%
%% INPUT: a matrix recording K round history info containing
%%        the following field
%%        showdown: K by 1 binary vector, recording if a game finally went
%%                  to a showdown stage.
%%        board:    k by 5 matrix, recording all the board cards
%%        hole:     k by N*2 matrix, recording hole cards for all players.
%%                  If a player folds, his cards are hidden (-1)
%%        bet:      k*4 by N, betting history of each player in four
%%                  rounds.
%%
%% OUTPUT: a matrix recording opponent model parameters

function oppo = UpdateOpponent(history,i)
    %disp('Printing here');
    %disp(history.money);
    [num_games,num_players] = size(history.money);
    num_board = 5;
    num_hole = 2;
    %disp(num_players);
    
    CALL = 1;                   % CALL or CHECK
    RAISE = 2;                  % BET or RAISE
    FOLD = 3;                   % FOLD
    
    % write for loop to get average final type of betting based on final
    % round
    
    player_cards = zeros(1,7);
    sum_type = zeros(1,num_players);
    count = zeros(1,num_players);
    curr_loop = 1;
    %disp(history.bet);
    for game = 1:num_games
        for i = 1:num_board
            player_cards(i) = history.board(game,i);
        end
        fold_array = zeros(1,num_players);
        for i=1:4
            raise = true;
            while raise
                raise = false;
                not_first = false;
                for player = 1:num_players
                    if (not_first && (history.bet(curr_loop,player) == RAISE))
                        raise = true;
                    end
                    if ((history.bet(curr_loop,player) == RAISE) || (history.bet(curr_loop,player) == CALL))
                        not_first = true;
                    end
                    if  (history.bet(curr_loop,player) == FOLD)
                        fold_array(player) = 1;
                    end
                end
                curr_loop = curr_loop +1;
            end
        end
        for player = 1:num_players
            player_cards(6) = history.hole(game,player*2-1);
            player_cards(7) = history.hole(game,player*2);
            [my_hand_type my_hand_card] = final_type(player_cards); 
            if(my_hand_type > 2.5)
                my_hand_type = 2.5
            elseif(my_hand_type < 0.5)
                my_hand_type = 0.5;
            end
            if (fold_array(player) == 0)
                sum_type(player) = sum_type(player)+my_hand_type;
                count(player) = count(player)+1;
            end
        end
    end
    
    %disp(curr_loop);
    %disp(size(history.bet));
    
    %sum_type = sum_type./num_games;
        
    %disp(history.bet);
    % get average finaltype and then calculate opponent style
    
    oppo1 = zeros(1,num_players);
    
    for player = 1:num_players
        
        if (count(player) == 0)
            sum_type(player) = 1.5;
        else
            sum_type(player) = sum_type(player)/count(player);
        end
        
        if (sum_type(player) > 2.5)
            oppo1(player) = 1;
        elseif (sum_type(player) < 0.5)
            oppo1(player) = 0;
        else
            oppo1(player) = (sum_type(player)-0.5)/2;
        end
        
    end
    oppo = oppo1;
    disp('Printing');
    disp(oppo);
    disp('Printing done');
    %% ----- FILL IN THE MISSING CODE ----- %%
end
