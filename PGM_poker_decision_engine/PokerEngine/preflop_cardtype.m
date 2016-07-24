%% Compute Pre-Flop Hand Categories,
%% See Table 1 in the write-up

function ct = preflop_cardtype(c1,c2)
    v = sort([c1;c2]);
    v1 = floor(v(1)/4)+2;
    s1 = mod(v(1),4);
    v2 = floor(v(2)/4)+2;
    s2 = mod(v(2),4);
    
    ct = 5;  % all others
    
    if (v1 == v2)
        if (v1 == 14 || v1 == 13 || v1 == 12)
            ct = 1;  
        end
        if (v1 == 11 || v1 == 10)
            ct = 2;
        end
        if (v1 == 9 || v1 == 8)
            ct = 4;
        end
    end
    
    if (v1 == 13 && v2 == 14)
        if (s1 == s2)
            ct = 1;
        else
            ct = 2;
        end
    end
    
    if (v1 == 12 && v2 == 14)
        if (s1 == s2)
            ct = 1;
        else
            ct = 3;
        end
    end
    
    if (v1 == 11 && v2 == 14)
        if (s1 == s2)
            ct = 2;
        else
            ct = 4;
        end
    end
    
    if (v1 == 12 && v2 == 13)
        if (s1 == s2)
            ct = 2;
        else
            ct = 4;
        end
    end
    
    if (s1 == s2)
        if ((v1 == 10 && v2 == 14) || (v1 == 11 && v2 == 13))
            ct = 3;
        end
        if ((v1 == 10 && v2 == 11) || (v1 == 11 && v2 == 12))
            ct = 3;
        end
        if ((v1 == 10 && v2 == 13) || (v1 == 10 && v2 == 12))
            ct = 4;
        end
        if ((v1 == 9 && v2 == 11) || (v1 == 9 && v2 == 10) || (v1 == 8 && v2 == 9))
            ct = 4;
        end
    end
    
end


    
    

