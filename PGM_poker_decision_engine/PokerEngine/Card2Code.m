function code = Card2Code(val, suit)
% Card2Code - Function converts from a desired card with a value and suit
% to a card code (integer from 0 to 51)
%   val - number giving value of card from 2 to 14 (Ace)
%   suit - One of 'D' 'C' 'H' 'S'
% Formula is code = 4*(val-2) + suit_inc
% where suit_inc is 0..3 following order of suits above

    code = 4*(val-2);
    switch suit
        case 'D'
            code = code + 0;
        case 'C'
            code = code + 1;
        case 'H'
            code = code + 2;
        case 'S'
            code = code + 3;
    end
end

