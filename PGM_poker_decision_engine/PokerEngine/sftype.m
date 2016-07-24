%% Compute Flush and Straight Hand Categories
% Student do not need to understand this.

%% See Table 2 in the write-up
%%   0 -- Junk
%%   1 -- SF
%%   2 -- SFO4
%%   3 -- SFO3
%%   4 -- SFI4
%%   5 -- F
%%   6 -- F4
%%   7 -- F3
%%   8 -- S
%%   9 -- SO4
%%   10 -- SO3
%%   11 -- SI4
%%   12 -- SFO3 & F4
%%   13 -- SFO3 & SI4
%%   14 -- SFO3 & SO4
%%   15 -- SI4 & F3
%%   16 -- SI4 & F4
%%   17 -- SO3 & F3
%%   18 -- SO3 & F4
%%   19 -- SO4 & F3
%%   20 -- SO4 & F4
%%
%% Note: Input v should be combination of board and hole cards. Any card types with value -1 will 
%% be stripped out automatically, so you can always pass in the full board_card array

function [type, highcard] = sftype(v)
   v = v(v ~= -1); % Remove any -1 codes from undealt board cards
   v = sort(v);
   val = floor(v/4)+2;  % value
   s = mod(v,4); % suit
   [type1,highcard1] = sftype_1(v,val,s);
   
   % deal with special case of ace
   for i = 1:size(v,2)
       if (v(i) >= 48 && v(i) < 52)
           v(i) = v(i) - 52;
       end
   end
   v = sort(v);
   val = floor(v/4)+2;  % value
   s = mod(v,4); % suite
   [type2,highcard2] = sftype_1(v,val,s);

   [type,highcard] = priority(type1,type2,highcard1,highcard2);
end

% In general lower numbers are better, but some strange ordering choices
% for the indices create trouble. Just check for cases where you prefer the
% higher index.
function [type highcard] = priority(type1,type2,highcard1,highcard2)
    if (type1 ~= type2)
        % Changing ace changed value of hand, sort the choices
        if type2 < type1
            t1 = type2;
            h1 = highcard2;
            t2 = type1;
            h2 = highcard1;
        else
            t1 = type1;
            h1 = highcard1;
            t2 = type2;
            h2 = highcard2;
        end
        type = t1; highcard = h1;
        % Check for cases where you prefer the higher number
        if (t1 == 0)
            type = t2; highcard = h2;           
        elseif ((t1 == 3) && (t2 >= 12))
            type = t2; highcard = h2;
        elseif ((t1 == 10) && (t2 >= 11))
            type = t2; highcard = h2;
        elseif ((t1 == 12) && ((t2 == 16) || (t2 == 20)))
            type = t2; highcard = h2;
        elseif (((t1 >= 15) && (t1 <= 18)) && ((t2 == 19) || (t2 == 20)))
            type = t2; highcard = h2;
        end
    else
        type = type1;
        highcard = max(highcard1, highcard2);
    end
end
            
% a = sort([type1,type2]);
%     if (type1 == 1)
%         type = type1;
%         highcard = highcard1;
%     elseif (type2 >= 12)
%         type = type2;
%         highcard = highcard2;
%     else
%         type = type1;
%         highcard = highcard1;
%     end

function [type highcard] = sftype_1(v,val,s)
   % highcard = -1; % we only record high cards for category 1,5,8 
   % JIM modified
   highcard = max(val); % we only record type-specific high cards for category 1,5,8 
   len = size(val,2) ;
   val_set = []; % value set, unique
   for i = 2:len
       if (val(i)~=val(i-1))
           val_set = [val_set val(i-1)];
       end
   end
   val_set = [val_set val(len)];
   len_set = size(val_set,2);
   
   type = 0; % junk
   
   [flush,suite] = flush_type(v);
   if (flush == 3)
       type = 7; % F3
   elseif (flush == 4)
       type = 6; % F4
   elseif (flush == 5)
       type = 5; % F
   end
       
   for i = 3:len_set
       if (val_set(i) - val_set(i-2) == 2)
           type = 10; % SO3
           lower = val_set(i-2);
           higher = val_set(i);
       end
   end
   
   if (type == 10)
       count = 0;
       for i = 1:len
           if (val(i)>=lower && val(i)<=higher && s(i) == suite)
               count = count + 1;
           end
       end
       if (count == 3)
           type = 3; % SFO3
           if (flush == 4)
               type = 12; % SFO3 & F4
           end
       end
   end
   
   for i = 4:len_set
       if (val_set(i) - val_set(i-3) == 4)
           if (type == 3)
               type = 13; % SF03 & SI4
           else
               type = 11; % SI4
               lower = val_set(i-3);
               higher = val_set(i);
           end
       end
       if (val_set(i) - val_set(i-3) == 3)
           if (type == 3)
               type = 14; % SFO3 & SO4
           else
               type = 9; % SO4
               lower = val_set(i-3);
               higher = val_set(i);
           end
       end
   end 
   
   if (type == 9 || type == 11)
       count = 0;
       for i = 1:len
           if (val(i)>=lower && val(i)<=higher && s(i) == suite)
               count = count + 1;
           end
       end
       if (count == 4)
           if (type == 9)
               type = 2; % SFO4
           end
           if (type == 11)
               type = 4; % SFI4
           end
       end
   end
   
   if (flush == 5)
       type = 5;
       for i = 1:len
           if (s(i) == suite)
               highcard = val(i);    % JIM changed to val
           end
       end
   end
   
   for i = 5:len_set
       if (val_set(i) - val_set(i-4) == 4)
           type = 8; % S
           lower = val_set(i-4);
           higher = val_set(i);
           for j = 1:len
               if (val(j) == higher)
                   highcard = val(j);    % JIM changed to val
               end
           end
           count = 0;
           for j = 1:len
               if (val(j)>=lower && val(j) <= higher && s(j) == suite)
                   count = count + 1;
               end
           end
           if (count >= 5)        % YL changed to >=
               type = 1; % SF
           end
       end
   end  
   
   if (type == 10)
       if (flush == 3)
           type = 17; % SO3 & F3
       elseif (flush == 4)
           type = 18; % SO3 & F4
       end
   end
   
   if (type == 11)
       if (flush == 3)
           type = 15; % SI4 & F3
       elseif (flush == 4)
           type = 16; % SI4 & F4
       end
   end
   
   if (type == 9)
       if (flush == 3)
           type = 19; % SO4 & F3
       elseif (flush == 4)
           type = 20; % SO4 & F4
       end
   end
   
end

function [type,suite] = flush_type(v)
    len = size(v,2);
    v = mod(v,4);
    v = sort(v);
    
    overlap = 1;
    max = 1;
    suite = -1;
    
    for i = 2:len
        if (v(i) == v(i-1))
            overlap = overlap + 1;
            if (overlap > max)
                max = overlap;
                suite = v(i); 
            end
        else
            overlap = 1;
        end
    end
    
    if (max >= 5) 
        max = 5; 
    end
    
    type = max;
    
end


    
    

