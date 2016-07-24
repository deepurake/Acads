% Given the hole cards and board cards for a single hand,
% generate strings containing the hand categories for N of a Kind
% and Straight-Flush, including high cards, which are listed by value only
% Note: board card list can contain -1 for undealt cards
function [Kname SFname] = hand_category(hole_card, board_card)
    global VALnames Knames SFnames
    if nargin == 1
        % Passed in vector of card indices with hole cards at front
        v = hole_card;
    else
        v = [hole_card board_card];
    end
    [ct high_ct] = cardtype(v);
    highnames = VALnames(high_ct-1);     % -1 comes from fact that val is in range 2-14
    Kname = sprintf('%7s-%2s', Knames{ct+1}, highnames);
    [sf high_sf] = sftype(v);
    highnames = VALnames(high_sf-1);     % -1 comes from fact that val is in range 2-14
    SFname = sprintf('%8s-%s', SFnames{sf+1}, highnames);
end
