%% Decision Making
%
% INPUT: a structure "info" containing the following field
%        stage, pot, cur_pos, cur_pot, first_pos, board card,
%        hole_card, active, paid, history, su_info, oppo_model
% OUTPUT: an integer from 1 to 3 indicating:
%         1: CALL or CHECK
%         2: BET or RAISE
%         3. FOLD

% We provide some auxiliary probability tables, see section 3.4 
% in the write-up. If you use BNT by Kevin Murphy, please check
% sample BNT code student_BNT.m in T-Square to see how to use it.
%
% Some zero entries do not really mean zero probability, instead
% they mean states we do not care about because they can not
% contribute to any effective hand category in the showdown

%% Table 1: Prior probability of final categories given 7 cards

%  Busted          0.1728
%  One Pair        0.438
%  Two Pair        0.2352
%  3 of a Kind     0.0483
%  Straight        0.048
%  Flush           0.0299
%  Full House      0.0255
%  4 of a Kind     0.0019
%  Straight Flush	0.0004

%% Table 2: Straight and Flush CPT from flop (given two draws)

%                  SF      Flush	Straight
%  J               0.0     0.0     0.0
%  SF              1.0     0.0     0.0
%	SFO4            0.0842	0.2784	0.2414
%	SFO3            0.0028	0.0389	0.0416
%	SFI4            0.0426	0.3145	0.1249
%  F               0.0     1.0     0.0
%	F4              0       0.3497	0
%	F3              0       0.0416	0
%  S               0       0       1.0
%	SO4             0       0       0.3145
%	SO3             0       0       0.0444
%	SI4             0       0       0.1647
%	SF03 & F4       0.0028	0.3469	0.0416
%	SF03 & SI4      0.0028	0.0389	0.1360
%	SF03 & SO4      0.0028	0.0389	0.2784
%	SI4 & F3        0       0.0416	0.1647
%	SI4 & F4        0       0.3497	0.1249
%	SO3 & F3        0    	0.0416	0.0416
%	SO3 & F4        0    	0.3497	0.0250
%	SO4 & F3        0    	0.0416	0.2756
%	SO4 & F4        0    	0.3497	0.2414

%% Table 3: N of a Kind CPT from flop (given two draws)

%          K4      K3K2	K3      K2K2	K2      Junk
%  K4      1.0     0.0     0.0     0.0     0.0     0.0
%	K3K2	0.0435	09565   0.0     0.0  	0.0     0.0
%	K3      0.0426	0.1249	0.8326  0.0     0.0     0.0
%	K2K2	0.0019	0.1619	0.0000	0.8362  0.0     0.0
%	K2      0.0009	0.0250	0.0666	0.3000	0.6075  0.0
%	Junk	0.0000	0.0000	0.0139	0.0832	0.4440  0.4589

%% Table 4: Straight and Flush CPT from turn (given one draw)

%          SF      Flush	Straight
%  SF      1.0     0.0     0.0
%	SFO4	0.0435	0.1522	0.1739
%	SFI4	0.0217	0.1739	0.0870
%  F       0.0     1.0     0.0
%	F4      0       0.1957	0
%  S       0       0       1.0
%	SO4     0       0       0.1739
%	SI4     0       0       0.0870

%% Table 5: N of a Kind CPT from turn (given one draw)

%          K4          K3K2	K3      K2K2	K2      Junk
%  K4      1.0         0.0     0.0     0.0     0.0     0.0
%	K3K2	0.0217      0.9783  0.0     0.0     0.0     0.0
%	K3      0.0217      0.196	0.7823  0.0     0.0     0.0
%	K2K2	0.0000      0.0870	0.0     0.9130  0.0     0.0
%	K2      0.0000      0.0     0.0435	0.2609	0.6956  0.0
%	Junk	0.0000      0.0     0.0     0.0     0.3910  0.609

% Default decision maker
% Start your project here
%
function decision = MakeDecision_Default(info)
    if (info.stage == 0) 
        decision = MakeDecisionPreFlop(info);
    else
        decision = MakeDecisionPostFlop(info);
    end
end

function decision = MakeDecisionPreFlop(info)
    persistent decProb
    decProb = [0.1 0.9 0.0; 0.3 0.7 0.0; 0.5 0.5 0.0; 0.7 0.3 0.0; 0.2 0.0 0.8];
    
    % this is a simple pre flop decision making function
    pfcat = preflop_cardtype(info.hole_card(1), info.hole_card(2));
    decision = sample_discrete(decProb(pfcat,:), 1, 1);
end

function decision = MakeDecisionPostFlop(info)
   
	%% fill in missing code here for Part I
    
	% Calculate the winning probability based on the available info.
    win_prob = PredictWin(info);
    
    % calculate the parameters required for calculating raise and call
    % probability thresholds.
    curr_bet = info.cur_pot-info.paid;
    pot = info.pot;
    num_players = sum(info.active);
    
    % Bet threshold can be used to influence the style of betting of the
    % agent. We tested against different values and found the best results
    % with following threshold.
    bet_thresh = 0;
    
    %disp(bet_thresh);
    
    % calculate Raise Threshold based on current bet, pot value, number
    % of active players and bet_threshold. 
    raise_thresh = (curr_bet+1)/(pot+num_players+curr_bet+1) - bet_thresh;
    
    % calculate Call Threshold using current bet value, pot value and bet
    % threshold.
    call_thresh = (curr_bet)/(pot+curr_bet) - bet_thresh;
    
    RAISE = 2;
    CALL = 1;
    FOLD = 3;
    
    if(win_prob > raise_thresh)
        % If Win probability is greater than RAISE Threshold, we decide to
        % RAISE.
        decision = RAISE; 
    elseif(win_prob > call_thresh)
        % Else if Win probability is greater than CALL Threshold, we decide to
        % CALL.
        decision = CALL;
    else
        % We fold if winning probability is too low.
        decision = FOLD;
    end
    
    %% fill in missing code here for Part II
    mustpay = info.cur_pot - info.paid(info.cur_pos);
    if mustpay > 0
        
    else
        
    end
end

% Compute probability of winning vs N opponents
function win_prob = PredictWin(info)
    % To make the code more moduler, we made three seperate functions for
    % each stage.
    if(info.stage == 1)
        % POST FLOP stage.
        win_prob = PredictWinFlop(info);
    elseif(info.stage == 2)
        % POST TURN stage.
        win_prob = PredictWinTurn(info);
    else
        % POST RIVER stage.
        win_prob = PredictWinRiver(info);
    end
   
end

% Compute probability of winning vs N opponents post FLOP
function win_prob = PredictWinFlop(info)
    % Number of cards on the board
    num_board = 3;
    win_prob = PredictWinGeneric(info,num_board);
end

% Compute probability of winning vs N opponents post TURN
function win_prob = PredictWinTurn(info)
    % Number of cards on the board
    num_board = 4;
    win_prob = PredictWinGeneric(info,num_board);
end

% Compute probability of winning vs N opponents post RIVER
function win_prob = PredictWinRiver(info)
    % Number of cards on the board
    num_board = 5;
    win_prob = PredictWinGeneric(info,num_board);
end

% Compute probability of winning vs N opponents with num_board community cards
function win_prob = PredictWinGeneric(info,num_board)

    % Total existing cards with the agent (two hole cards and num_board community cards)
    total_existing = 2+num_board;
    
    % List of existing cards
    existing = zeros(1,total_existing+1);
    for i = 1:2
        existing(i) = info.hole_card(i);
    end
    for i = 1:num_board
        existing(i+2) = info.board_card(i);
    end
    % We use an extra junk card with large value just for ease of coding. 
    % It doesn't affect the calculation part.
    existing(total_existing+1) = 100;
    
    % Sorted list of existing cards. 
    sorted_existing = sort(existing);
    
    % Total number of cards.
    total_cards = 52;
    
    % Total remaining cards to be sampled from.
    num_cards_deck = total_cards-total_existing;
    
    % Remaining community cards that need to be sampled.
    cards_to_pick = 5-num_board;
    
    % Mapping array used to sample the cards.
    mapping = zeros(1,num_cards_deck);
    curr_idx = 1;
    map_idx = 0;
    for curr_num = 1:total_cards
        if(curr_num == sorted_existing(curr_idx))
            curr_idx = curr_idx + 1;
            continue;
        end
        map_idx = map_idx+1;
        mapping(map_idx) = curr_num;
    end
    
    % Total number of active apponents
    active_opp = sum(info.active)-1;
    
    % Model style of player data from history
    su_info_active = zeros(1,active_opp+1);
    disp('su_info');
    [m n] = size(info.su_info);
    disp(size(info.su_info));
    disp(info.su_info);
    disp(size(info.oppo(1)));
    idx = 1;
    for i = 1:info.num_oppo
        if (info.active(i) == 1 && n > 1)
            su_info_active(idx) = 0.5*(info.su_info(i+1)-0.25);
            idx = idx+1;
        end
    end
    disp(su_info_active);
    disp('su_info_done');
    % Total number of samples to be generated.
    num_samples = active_opp *2 + cards_to_pick;
    % arrays of final set of cards(agent and opponents)
    my_cards = zeros(1,7);
    opp_cards = zeros(1,7);
    for i = 1:total_existing
        my_cards(i) = existing(i);
    end
    for i = 1:num_board
        opp_cards(i) = info.board_card(i);
    end
    
    % Number of times, our Agent wins in the sample runs.
    num_wins = 0;
    % total sample runs
    num_runs = 3000;
    
    % We sample all unknown cards(opponent hole cards and missing community cards)
    % We call final_type and decide winner in each round. If agent win, we
    % record it in num_wins.
%     disp('hole cards');
%     for c=info.hole_card
%         disp(GetStringFromNumber(c));
%     end
%     disp('board cards');
%     for c=info.board_card
%         disp(GetStringFromNumber(c));
%     end
    %disp(info.hole_card);
    %disp(info.board_card);
    for i = 1:num_runs
        my_win = 1;
        samples = randperm(num_cards_deck,num_samples);
        % convert samples to cards
        for sample_idx = 1:num_samples
            samples(sample_idx) = mapping(samples(sample_idx));
        end
        % Assign first 'cards_to_pick' sample cards as community cards
        for next_to_pick = 1:cards_to_pick
            my_cards(next_to_pick + total_existing) = samples(next_to_pick);
            opp_cards(next_to_pick+num_board) = samples(next_to_pick);
        end
        
        % Get Agent's final hand from all seven cards
        [my_hand_type my_hand_card] = final_type(my_cards);
        
        for opp = 1:active_opp
            % Pick two cards for each opponent.
            opp_cards(6) = samples(opp*2+(cards_to_pick - 1));
            opp_cards(7) = samples(opp*2+cards_to_pick);
            
            % Get current opponent's final hand.
            [opp_hand_type opp_hand_card]= final_type(opp_cards);
            prob = su_info_active(opp);
            b = rand(1,1000);
            if(prob < 0)
                if(b <= -1*prob*1000)
                    opp_hand_type = opp_hand_type - 1;
                end
            else
                if(b <= prob*1000)
                    opp_hand_type = opp_hand_type + 1;
                end
            end
            
            % Check for winner
            if(my_hand_type > opp_hand_type)
                % Agent's hand type better than opponent.
                continue;
            elseif ((my_hand_type == opp_hand_type) && (my_hand_card(1) > opp_hand_card(1)))
                % Agent's hand type is same as opponent's, however Agent's
                % highest hand card is better.
                continue;
            elseif ((my_hand_type == opp_hand_type) && (my_hand_card(1) == opp_hand_card(1)))
                % If first high card is also same, check for second.
                [row1 col1] = size(my_hand_card);
                [row2 col2] = size(opp_hand_card);
                if((col1 > 1) && (col2 > 1) && (my_hand_card(2) > opp_hand_card(2)))
                    continue;
                end
            end
            % We reach here if Agent looses to current opponent.
            my_win = 0;
            break;
        end
        if(my_win == 1)
            % If agent wins current round, record it in num_wins.
            num_wins = num_wins+1;
        end
    end
    % Calculate Winning Probability.
    win_prob = num_wins/num_runs;
    %disp(win_prob);
    %disp(active_opp);
    %pause;
end

function card_str = GetStringFromNumber(card_no)
    suit = mod(card_no, 4);
    val = floor(card_no/4) + 2;
    if (suit == 0)
        suit = 'D';
    elseif (suit == 1)
        suit = 'C';
    elseif (suit == 2)
        suit = 'H';
    else
        suit = 'S';
    end
    card_str = strcat(num2str(val), suit);
    if (card_no == -1)
        card_str = num2str(card_no);
    end
end
