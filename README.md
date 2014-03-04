<!--
# vim: set tw=100:
-->

Tetris evolutionary algorithm
=============================

By Jonas Amundsen

This is the repository for my master project and master thesis at [NTNU][ntnu], an evolutionary
algorithm written in C, capable of evolving strategies for the popular tile-matching puzzle game
Tetris. Along with the actual EA, the project also contains small helper applications for storing
and reading tetris strategies, generating random boards, performing feature detection and playing
games of Tetris.

[ntnu]: http://www.ntnu.no/

## Tools

The project is, as previously mentioned, bundled with a couple of tools to make usage a bit easier.

### Random tetromino placements

You can generate Tetris boards with randomly placed tetrominos with `random_board`.

```
$ ./random_board
|          |
|          |
|          |
|          |
|          |
|          |
|          |
|          |
|       #  |
|       #  |
|       #  |
|    ####  |
|     # #  |
|    ####  |
|  #   ##  |
|  ##  ## #|
|####  ####|
| #    ### |
| ##    ## |
|  ##    # |
------------
```

`random_board` also accepts inputs to specify the number of random placements to perform and the
width and height of the Tetris board.

```
$ ./random_board -n 2 --board-width 5 --board-height 10
|     |
|     |
|     |
|     |
|     |
|     |
|     |
|    #|
|#  ##|
|#### |
-------
```

### Feature detection

You can perform feature detection of Tetris boards with `analyze_board`. It reads the Tetris board
format shown above from standard input.

```
$ ./random_board | ./analyze_board
These are the values of the given board for the specified features.
--f-max-height             11
--f-n-holes                17
--f-landing-height         18
--f-removed-lines          0
--f-weighted-blocks        181
--f-well-sum               0
--f-n-blocks               40
--f-eroded-piece-cells     0
--f-row-transitions        56
--f-column-transitions     22
--f-cumulative-wells-dell  11
--f-cumulative-wells-fast  13
--f-min-height             2
--f-max-height-difference  9
--f-n-adjacent-holes       6
--f-max-well-depth         0
--f-hole-depths            48
--f-n-rows-with-holes      8
```

It also accepts an argument which makes it print out the board it reads from standard input.

```
$ ./random_board -n 2 --board-width 5 --board-height 10 | ./analyze_board -p
|     |
|     |
|     |
|     |
|     |
|     |
|     |
| #   |
|#### |
| ### |
-------
These are the values of the given board for the specified features.
--f-max-height             3
--f-n-holes                1
--f-landing-height         8
--f-removed-lines          0
--f-weighted-blocks        14
--f-well-sum               3
--f-n-blocks               8
--f-eroded-piece-cells     0
--f-row-transitions        24
--f-column-transitions     7
--f-cumulative-wells-dell  5
--f-cumulative-wells-fast  5
--f-min-height             0
--f-max-height-difference  3
--f-n-adjacent-holes       1
--f-max-well-depth         2
--f-hole-depths            1
--f-n-rows-with-holes      1
```

### Playing a strategy

You can play a strategy using `play`. It accepts a variety of parameters, as shown below.

```
$ ./play -h
usage: ./play [options]
Options:
  --n-trials N (defaults to 10)
  --board-width N (defaults to 10)
  --board-height N (defaults to 20)
  --n-piece-lookahead N (defaults to 0)
                      The number of tetrominos that the controller will see
                      ahead of the current tetromino. Higher means that the
                      controller can take more informed choices, but it will
                      result in significantly higher computation times.
  -i                  print parsable boards
  --f-max-height N (defaults to 0)
  --f-n-holes N (defaults to 0)
  --f-landing-height N (defaults to 0)
  --f-removed-lines N (defaults to 0)
  --f-weighted-blocks N (defaults to 0)
  --f-well-sum N (defaults to 0)
  --f-n-blocks N (defaults to 0)
  --f-eroded-piece-cells N (defaults to 0)
  --f-row-transitions N (defaults to 0)
  --f-column-transitions N (defaults to 0)
  --f-cumulative-wells-dell N (defaults to 0)
  --f-cumulative-wells-fast N (defaults to 0)
  --f-min-height N (defaults to 0)
  --f-max-height-difference N (defaults to 0)
  --f-n-adjacent-holes N (defaults to 0)
  --f-max-well-depth N (defaults to 0)
  --f-hole-depths N (defaults to 0)
  --f-n-rows-with-holes N (defaults to 0)
```

With clever use of a functioning terminal, one can easily test existing strategies as shown below.

```
$ ./play --n-trials 3 `cat ./dellacherie-handcrafted`
The following phenotype has been initialized.
--f-landing-height        -1.00
--f-eroded-piece-cells     1.00
--f-row-transitions       -1.00
--f-column-transitions    -1.00
--f-n-holes               -4.00
--f-cumulative-wells-dell -1.00
Play 1. resulted in a score of 421498.
Play 2. resulted in a score of 2195597.
Play 3. resulted in a score of 8255653.
The average score of 3 trials is 3624249.
```

.. where ./dellarchi-handcrafted might contain the following.

```
--f-landing-height        -1
--f-eroded-piece-cells     1
--f-row-transitions       -1
--f-column-transitions    -1
--f-n-holes               -4
--f-cumulative-wells-dell -1
```
