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
  --f-column-heights N (defaults to 0)
  --f-column-difference N (defaults to 0)
  --f-landing-height N (defaults to 0)
  --f-cell-transitions N (defaults to 0)
  --f-deep-well-sum N (defaults to 0)
  --f-height-differences N (defaults to 0)
  --f-mean-height N (defaults to 0)
  --f-v-max-height N (defaults to 0)
  --f-v-n-holes N (defaults to 0)
  --f-v-height-differences N (defaults to 0)
  --f-v-mean-height N (defaults to 0)
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
  --f-max-minus-mean-height N (defaults to 0)
  --f-mean-minux-min-height N (defaults to 0)
  --f-mean-hole-depth N (defaults to 0)
  --f-max-height-difference N (defaults to 0)
  --f-n-adjacent-holes N (defaults to 0)
  --f-max-well-depth N (defaults to 0)
  --f-hole-depths N (defaults to 0)
  --f-n-rows-with-holes N (defaults to 0)
  --f-diversity N (defaults to 0)
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

## The EA

`tetris` represent the actual EA. It supports a number of arguments. Note that it is required
to specify either --log-dir or --no-log. This is so that one can never accidentally run the EA
without logging the results.

```
$ ./tetris -h
usage: ./tetris [options]
Options:
  -h or --help        show this help message and exit
  -v or --verbose
  -i                  read problem instance from standard input
  --pupulation-size N (defaults to 20)
  --tournament-group-size N (defaults to 10)
  --tournament-group-random-selection P (defaults to 0.1)
                      Don't select the best individual in the tournament
                      with a probability of P. A low P will increase
                      selection pressure, while a high P will decrease it.
  --max-n-generations N (defaults to 100)
  --elitism N (defaults to 0)
                      keep the N best individuals for next generation
  --mutation-rate F (defaults to 0.995)
  --crossover-rate F (defaults to 0.5)
  --crossover-points N (defaults to 2)
                      set equal to genotype size to do uniform crossover
  --selection {TOURNAMENT, SUS} (defaults to TOURNAMENT)
                      which method of parent selection to use
  --no-change-duration N (defaults to 50)
                      If above zero, the EA will reset itself if no change
                      in the winning individual has occurred. The number
                      of individuals to be reset is given by --reset-volume.
  --reset-volume N (defaults to 0)
                      The number of individuals to reset if no change has
                      happened during the no-change-duration. If set to 0,
                      all individuals will be reset.
  --n-trials N (defaults to 10)
                      The number of trials to run in order to determine the
                      average fitness score of an individual.
  --board-width N (defaults to 10)
  --board-height N (defaults to 20)
  --n-piece-lookahead N (defaults to 0)
                      The number of tetrominos that the controller will see
                      ahead of the current tetromino. Higher means that the
                      controller can take more informed choices, but it will
                      result in significantly higher computation times.
  --randomization-range N (defaults to 100)
                      Determines the range of value a weight can obtain during
                      randomization which occurs at initialization.
  --mutation-range n (defaults to 100)
                      Determines the range of value that is added to a weight
                      during mutation.
  --feature-enable-rate F (defaults to 1 / 6)
                      Determines the probability of a feature to be enabled
                      during randomization and mutation. The reason for a
                      seemingly low number is that there is quite a lot of
                      features.
  --multi-threading   calculate fitness values in parallel by multi-threading
  -l or --log-dir     specify the location for run logs
  --no-log            do not log results (-i or --no-log needs to be defined)

Additionally, the following arguments can be used to enable features.

  --f-all             enables all features
  --f-max-height
  --f-n-holes
  --f-column-heights
  --f-column-difference
  --f-landing-height
  --f-cell-transitions
  --f-deep-well-sum
  --f-height-differences
  --f-mean-height
  --f-v-max-height
  --f-v-n-holes
  --f-v-height-differences
  --f-v-mean-height
  --f-removed-lines
  --f-weighted-blocks
  --f-well-sum
  --f-n-blocks
  --f-eroded-piece-cells
  --f-row-transitions
  --f-column-transitions
  --f-cumulative-wells-dell
  --f-cumulative-wells-fast
  --f-min-height
  --f-max-minus-mean-height
  --f-mean-minux-min-height
  --f-mean-hole-depth
  --f-max-height-difference
  --f-n-adjacent-holes
  --f-max-well-depth
  --f-hole-depths
  --f-n-rows-with-holes
  --f-diversity
```

The following commands shows one way to execute the EA. Note that debugging output is enabled and
copied to a file using `tee`.

```bash
unbuffer mpirun -n 5 ./../bin/tetris \
  --population-size 10               \
  -l ../../logs                      \
  -v                                 \
  --mutation-rate 0.95               \
  --f-all                            \
  --elitism 1                        \
  --max-n-generations 10             \
  | tee ../../logs/debug.out
```

The results are logged in a line break and tab separated file, where tabs separate individuals of a
generation and line breaks separate generations. The project includes a tool for formatting the data
and preparing it to be viewed with gnuplot. The following bash command shows how the EA can be run
from the project root folder and the results visualized immediately.

```bash
$ ./code/bin/tetris -l logs && \
    ./logs/format_data && \
    GNUPLOT_LIB="logs/`ls -t logs | head -1`/" \
    gnuplot -p logs/candlesticks
```

### Realtime visualization

For runs that take a considerable amount of time, being able to visualize the results in realtime
can become useful. Since the EA simply drops unformatted data continuously, this is possible.
`log_monitor` is a tool for monitoring log files using node.js and inotify. The application utilizes
websockets to push the gatherered data to a web page, which visualizes it using d3.

Using this tool requires the reader to install some npm packages.

```bash
cd ./logs
npm install inotify underscore websockets
```

The following bash command shows how to start `log_monitor`.

```bash
./logs/log_monitor
```

The reader will then have to open `./logs/log_viewer.html` in their browser. Upon running the EA,
data will become present in the browser window.
