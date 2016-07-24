**Poker Engine**

![PokerEngine](https://bitbucket.org/walllab/pokerengine/raw/master/docs/Poker.png)

[TOC]

##  Dependencies
The poker engine is self-contained and does not require any third-party library. However, some of the included agents require [BNT]. Please make sure that you have installed [BNT] correctly.  

## Getting Started
Run:
> poker_demo.m

##  Background
Card Representation: We use an integer 0 to 51 to represent all cards(from diamonds 2 to spades Ace).  

The suite cane be: 0-diamonds 1-clubs 2-hearts 3-spades.

 - 0 ![diamonds](https://bitbucket.org/walllab/pokerengine/raw/master/docs/diamonds-32.png)
 - 1 ![clubs](https://bitbucket.org/walllab/pokerengine/raw/master/docs/clubs-32.png)
 - 2 ![hearts](https://bitbucket.org/walllab/pokerengine/raw/master/docs/hearts-32.png)
 - 3 ![spades](https://bitbucket.org/walllab/pokerengine/raw/master/docs/spades-32.png)

> The suite is computed as: **suit = mod(card,4)**

> The value is computed as: **val = floor(card/4)+2**. (Value 14 means Ace).

**Texas Hold'Em**: There are four stages: **pre-flop**, **flop**, **turn**, **river**. Our game engine will call function MakeDecision(info) at every stage. At each stage your decision could be:

 1. Call or Check 
 2. Bet or Raise
 3. Fold.

If you fold, you lose money previously invested and are out of the game.

##  Tips and Notes
**poker_main.m** is the main game engine. Our scaffolding code already contains several top agents from last year. These agents are provided for a 10-player game. Do not worry about your performance at this stage since most of the agents has implemented both parts of the project. If you just run it without writing your own code, you should be able to see the records in your console. It records game info and player status of every round in every game. Make sure you understand this output before you proceed. 

You can modify **poker_main.m** for debugging purposes. For example you can output more variables to **"record.txt"**. You don't need to understand how all the functions in poker_main.m are working, you just to need to understand how the functions MakeDecision, UpdateOpponent, and StageUpdater are called and how their outputs are used.

> **Note:** Please do not add variables or additional functions to poker_main.m. During grading and the poker tournament, we'll replace this file with the standard version before running your agent.

The function *MakeDecision(info)* is the most important function. All other functions you wrote serve for the purpose of generating a decision in each round. Please make sure it has the correct value (1 to 3). If not, our engine will assume that your agent decided to fold.

Try playing with it for a few rounds with the existing agents. It would be helpful to set breakpoint and check the values if you want to understand the variables quickly.

##  Tasks for each Project Part
 
 - Part I:   **MakeDecision.m,** along with your report into a single file.
 - Part II: **MakeDecision.m** UpdateOpponent.m StageUpdater.m, along with your report into a single file.

## Playing a Game

Type poker_main to run the game engine and play a match. Constants at the start of the file control how many players and how many hands will be played. The default behavior is to pause for a keypress after every game, to allow you the chance to look back over the history. You can modify variable rounds_not_to_pause to control the pause . In order to compute statistically-meaningful summaries of performance you will need to simulate at least 100's of hands or more.


##  Additional notes on the game engine code.

### Make Decision

The file MakeDecision_Default contains stub code for you to fill in the functions you will use for deciding how to bet. This file is where you should start with Part I of the project. It implements a standard preflop betting strategy and a random postflop betting strategy by default. You don't need to modify the preflop strategy, but you will improve the postflop strategy.

### Betting History

In the model for history.bet, the betting for each stage of the game can occupy multiple rows in the bet matrix. The first row is padded with zeros until the position of the first player is reached, and the last row is also padded with zeros after the last betting action has been taken. A typical scenario might look like this:


```
0 0 0 1 1 1 2 1
1 2 1 1 1 3 3 1
1 0 0 0 0 0 0 0
```


In this example, player 4 is in first position and begins by checking, player 7 places the first bet and then player 2 re-raises, with everyone calling, except players 6 and 7 who fold.

In order to accommodate the fact that games take a variable number of rows, there is a field history.start, which contains a matrix that is n x 4, where n is the number of games that have been played so far. Each row of history.start contains an index to the start of the row in the bet matrix for each of the game stages. An example would look like:

row 2:  12 14 17 18

which means that the preflop betting round for game 2 begins at row 12 in history.bet, the postflop betting round begins at 14 (and ends at 16), and the rounds for the turn and river begin on 17 and 18. You can use these indices to examine the betting behavior for a particular stage of a particular game if you care to do so.

Note that in order to simplify the implementation, the history.start field is not updated until the game is over, but history.bet is updated incrementally at the end of each game stage. If you want to access history.bet within a game (while it is being played) the indices to rows are stored temporarily in history.stage_starts, which is updated incrementally and then copied into history.start when the game is over.

Note that you can search the columns of history.bet to obtain information about a given player. See lines 379-400 in the new version of poker_main.m for an example of how to extract such betting behavior.

In poker, ties are handled by dividing the pot among the players with the same hand. In games with 8 players this happens maybe 5% of the time. The function compare_showdown in poker_main.m implements this.


  [MATLAB]: http://www.mathworks.com/products/matlab/
  [LaTeX]: http://www.latex-project.org/
  [BNT]: https://github.com/bayesnet/bnt