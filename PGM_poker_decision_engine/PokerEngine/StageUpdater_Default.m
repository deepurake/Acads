%% Stage Updater
%%
%% INPUT: a structure "info" containing the following field
%%        stage, pot, cur_pos, cur_pot, first_pos, board card,
%%        hole_card, active, paid, history, su_info, oppo_model
%% OUTPUT : a matrix su_info recording the info you want to save,
%%          this matrix will be included in the "info" in the 
%%          MakeDecision function

function su_info = StageUpdater(info)
    %su_info = [];
    
    %% ----- FILL IN THE MISSING CODE ----- %%
    
    oppo_dis = PredictHoleCards(info);
    su_info = info.oppo{1};
    disp('opp model');
    disp(su_info);
    disp('opp model done');
    
    %% ----- FILL IN THE MISSING CODE ----- %%
end

function oppo_dis = PredictHoleCards(info)
    oppo_dis = [];
    %% ----- FILL IN THE MISSING CODE ----- %%
end