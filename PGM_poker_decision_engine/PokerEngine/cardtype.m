%% Compute N of a Kind Hand Categories,
% Student do not need to understand this.

% See Table 2 in the write-up
%   0 -- No Pair
%   1 -- One Pair
%   2 -- Two pair
%   3 -- Three of a Kind
%   4 -- Full House
%   5 -- Four of a Kind
%
% Note: Input v1 should be combination of board and hole cards. Any card types with value -1 will 
% be stripped out automatically, so you can always pass in the full board_card array
% The meaning of the highcard output depends upon what category it is

function [ct highcard] = cardtype(v1)
    v1 = v1(v1 ~= -1); % Remove any -1 codes from undealt board cards
    len = size(v1,2);
    v1 = sort(v1);
    v = floor(v1/4)+2;
        
    ct = 0;  % junk
    highcard = v(len);   % JIM: changed to v
    
    pair_count = 0;
    pair_val = [];
    tmp = [];
    for i = 2:len
        if (v(i) == v(i-1))
            pair_count  = pair_count + 1;  
            
            if (size(pair_val) > 0)
                if (v(i)~=pair_val(size(pair_val,2)))
                    pair_val = [pair_val v(i)];
                end
            else
                pair_val = [pair_val v(i)];
            end
        end
    end
    
    if (pair_count == 1)
        ct = 1;  % one pair
        highcard = pair_val(size(pair_val,2));
        for i = 1:len
            if (v(i)~=highcard)
                tmp = v(i);   % JIM: changed to v
            end
        end
        highcard = [highcard tmp];
    end
    
    if (pair_count >= 2)
        ct = 2;  % two pairs
        if ((size(pair_val,2)) >= 2)
            highcard = pair_val(size(pair_val,2));
            highcard = [highcard pair_val(size(pair_val,2)-1)];
            % JIM: Removed below so two high cards in K2K2 case are just
            % the pairs
%             for i = 1:len
%                 if (v(i)~=highcard(1) && v(i)~=highcard(2))
%                     tmp = v1(i);
%                 end
%            end
%            highcard = [highcard tmp];
        end
    end
    
    triple_count = 0;
    for i = 3:len
        if (v(i) == v(i-1) && v(i) == v(i-2))
            triple_count = triple_count + 1;
            highcard = v(i);   % JIM: changed to v
        end
    end
    
    if (triple_count >= 1)
        ct = 3;  % three of a kind
        if (pair_count >= 3)
            ct = 4;  % full house
            % JIM added code to extract value of pair (at this point
            % highcard holds the triple value)
            for i = 1:size(pair_val,2)
                if (pair_val(i)~=highcard)
                    tmp = pair_val(i);
                end
            end
            highcard = [highcard tmp];   
        end
    end
    
    quad_count = 0;
    for i = 4:len
        if (v(i) == v(i-1) && v(i) == v(i-2) && v(i) == v(i-3))
            quad_count = quad_count + 1;
            highcard = v(i);   % JIM: changed to v
        end
    end
    
    if (quad_count >= 1)
        ct = 5;  % four of a kind
    end
    
end


    
    

