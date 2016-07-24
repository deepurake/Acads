%% Util function for Return Agents
% Student do not need to understand this.

function [ varargout ] = agent_utils( varargin )
% evaluate function according to the number of inputs and outputs
    if nargout(varargin{1}) > 0
        [varargout{1:nargout(varargin{1})}] = feval(varargin{:});
    else
        feval(varargin{:});
    end
end

%% Return Agents
function [agents_info] = return_agents(num_builtin_agents, agents_dir, exclude_dir)
    builtin_agents_MD = {'MakeDecision_Default'};
    builtin_agents_SU = {'StageUpdater_Default'};
    builtin_agents_UO = {'UpdateOpponent_Default'};
    builtin_agents_strs = {'Default'};
    
    agents_info.agents_strs = {};
    
    agents_info.agents_MakeDecision = {};
    agents_info.agents_StageUpdater = {};
    agents_info.agents_UpdateOpponent = {};
    
    % first add built-in agents
    if ~isempty(num_builtin_agents)
        assert(length(num_builtin_agents) == length(builtin_agents_MD), ...
            ['The num_builtin_agents should be an array of length ', ...
            num2str(length(builtin_agents_MD)), ' where each position ', ... 
            'specifies the number of agents for a builtin type']);
        
        for agent_idx = 1:length(num_builtin_agents)
            for idx = 1:num_builtin_agents(agent_idx)
                agents_info.agents_strs{end+1} = builtin_agents_strs{agent_idx};
                
                agents_info.agents_MakeDecision{end+1} = str2func(builtin_agents_MD{agent_idx});
                agents_info.agents_StageUpdater{end+1} = str2func(builtin_agents_SU{agent_idx});
                agents_info.agents_UpdateOpponent{end+1} = str2func(builtin_agents_UO{agent_idx});
            end
        end
    end
    
    if exist('agents_dir','var') == 1 && ~isempty('agents_dir')
        curr_dir = fileparts(which(mfilename));
        agents_dir = fullfile(curr_dir, agents_dir);

        if exist('exclude_dir','var') == 0
            exclude_dir = {};
        end

        agents_dirs = dir(agents_dir);
        for idx = 1:length(agents_dirs)
            if agents_dirs(idx).isdir && agents_dirs(idx).name(1) ~= '.'  && ...
                    ~any(strcmpi(exclude_dir, agents_dirs(idx).name))
                agents_info.agents_strs{end+1} = agents_dirs(idx).name;

                % add agent to the path
                addpath(fullfile(agents_dir, agents_dirs(idx).name));

                % search for 3 agent files
                agent_MD_file = ...
                    dir(fullfile(agents_dir, agents_dirs(idx).name, '*MakeDecision*'));
                [a, temp] = fileparts(agent_MD_file(1).name);
                agents_info.agents_MakeDecision{end+1} = str2func(temp);

                agent_SU_file = ...
                    dir(fullfile(agents_dir, agents_dirs(idx).name, '*StageUpdater*'));
                [a, temp] = fileparts(agent_SU_file(1).name);
                agents_info.agents_StageUpdater{end+1} = str2func(temp);

                agent_UO_file = ...
                    dir(fullfile(agents_dir, agents_dirs(idx).name, '*UpdateOpponent*'));
                [a, temp] = fileparts(agent_UO_file(1).name);
                agents_info.agents_UpdateOpponent{end+1} = str2func(temp);
            end
        end
    end
end