% check BNT toolbox, this is used by some agents
if isempty(which('mk_bnet'))
    warning('BNT not found. Add BNT folder to path\n');
end