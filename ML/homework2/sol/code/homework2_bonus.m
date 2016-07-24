function [] = homework2_bonus( )
% This is a simple example to help you evaluate your clustering algo implementation. You should run your code several time and report the best
% result. The data contains a 400*101 matrix call X, in which the last
% column is the true label of the assignment, but you are not allowed to
% use this label in your implementation, the label is provided to help you
% evaluate your algorithm. 
%
%
% Please implement your clustering algorithm in the other file, mycluster.m. Have fun coding!


load('data');
T = X(:,1:100);
label = X(:,101);





[ WZ,DZ,Z ] = mycluster2(T,4);

load('nips');

show_topics(WZ,wl);

end