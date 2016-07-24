%% Compute the hand category in the showdown:
%%   0 -- Junk
%%   1 -- One Pair
%%   2 -- Two Pair
%%   3 -- Three of a Kind
%%   4 -- Straight
%%   5 -- Flush
%%   6 -- Full House
%%   7 -- Four of a Kind
%%   8 -- Straight Flush
function [type highcard] = final_type(v)
    [ct highcard_ct] = cardtype(v);
    [sf highcard_sf] = sftype(v);
    
    if (ct <= 3)
        type = ct;
        highcard = highcard_ct;
    end
    
    if (sf == 8)
        type = 4; % straight
        highcard = highcard_sf;
    end
    
    if (sf == 5)
        type = 5; % flush
        highcard = highcard_sf;
    end
    
    if (ct == 4)
        type = 6; % full house
        highcard = highcard_ct;
    end
    
    if (ct == 5)
        type = 7; % four of a kind
        highcard = highcard_ct;
    end
    
    if (sf == 1)
        type = 8; % straight flush
        highcard = highcard_sf;
    end
    
end
