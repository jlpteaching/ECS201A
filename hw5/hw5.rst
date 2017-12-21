:Author: Jason Lowe-Power
:Editor: Justin Perona

(TO DO) Convert the Reversi code from C to C++.
Also, potentially find a machine with 16+ cores to compare on, sort of like what David Wood did with the Xeon Phi?

=====================
ECS 201A - Homework 5
=====================

Due: Thursday, 7 December 2017 at 1:40 PM
-----------------------------------------

Submission of your code and your report will be done through Canvas_.
See the Submission_ section below for more information.

*Please submit your report as a separate PDF, and not in the archive!*
Not only does this make things easier for me to grade, but I can give you feedback directly onto your report.

.. _Canvas: https://canvas.ucdavis.edu/courses/146759

Overview
--------

In this assignment, you will practice writing programs with `Cilk Plus`_.
You will also get practice debugging with the Cilk Plus race detector, ``cilkscreen``, and understanding performance using the ``cilkview`` tool.

For this assignment, all work must be done on the CSIF.
This is so that everybody uses a consistent CPU.
All the CSIF machines have an Intel Haswell i7-4790, with four physical cores and eight logical cores.

This assignment is adapted from `Homework 3`_ of David Wood's CS 758 at University of Wisconsin - Madison.
That assignment, in turn, was adapted from an assignment by John Mellor-Crummey at Rice University.

.. _Cilk Plus: https://www.cilkplus.org/
.. _Homework 3: http://pages.cs.wisc.edu/~david/courses/cs758/Fall2016/wiki/index.php?n=Main.Homework3

Cilk and Cilk Plus
~~~~~~~~~~~~~~~~~~

`Cilk`_ is a parallel extension to the C and C++ programming languages.
For this assignment, you will be using `Cilk Plus`_, which is Intel's expansion of Cilk.
Cilk Plus adds three new keywords to C and C++.
They are listed below:

- ``cilk_spawn``, which is equivalent to a fork in a `fork-join model`_.
- ``cilk_sync``, which is equivalent to a join in a `fork-join model`_.
- ``cilk_for``, which is used for parallelizing for loops.

It also adds some pragmas and many functions.
The following is taken from the `Wikipedia article on Cilk`_:

    The biggest principle behind the design of the Cilk language is that the programmer should be responsible for exposing the parallelism, identifying elements that can safely be executed in parallel; it should then be left to the run-time environment, particularly the scheduler, to decide during execution how to actually divide the work between processors.
    It is because these responsibilities are separated that a Cilk program can run without rewriting on any number of processors, including one.

Unfortunately, Cilk Plus is being deprecated in the near future.
It will be replaced by the other major parallelization APIs, OpenMP_ and `Intel's Threading Building Blocks (TBB)`_.

.. _Cilk: http://supertech.csail.mit.edu/cilk/
.. _fork-join model: https://en.wikipedia.org/wiki/Fork%E2%80%93join_model
.. _Wikipedia article on Cilk: https://en.wikipedia.org/wiki/Cilk#Language_features
.. _OpenMP: http://www.openmp.org/
.. _Intel's Threading Building Blocks (TBB): https://www.threadingbuildingblocks.org/

Setup
-----

As noted before, you will need to work on the CSIF for this assignment.
In particular, the tools that we will be using require a specific version of ``gcc`` to use.
To get the code that you will be modifying, clone the following repository:

.. code-block:: sh

    git clone https://github.com/jlpteaching/reversi.git

There will be some Cilk shell scripts for you to run later.
For now, compile the basic program and make sure that it and the Cilk tools work:

.. code-block:: sh

    make
    ./run.sh
    ./cilkview.sh
    ./cilkscreen.sh

The exact output of the program does not matter, just make sure that you don't have any errors.
The Cilk tools should report that they cannot find any Cilk code.

Assignment
----------

For this assignment, you will be creating and parallelizing an AI for the game *Reversi*.
Reversi, also known as *Othello*, is a strategy game played on an 8 x 8 game board.
The rules to the game and a brief explanation of strategy issues are available on the `Wikipedia page for Reversi`_.
You can also play Reversi online on various websites, if you want to get a better feel for it.

.. _Wikipedia page for Reversi: https://en.wikipedia.org/wiki/Reversi

Reversi Program
~~~~~~~~~~~~~~~

A sequential program that enables two players, human or AI, to play Reversi is included in the given files.
You can run the basic version with the shell script included in the repository:

.. code-block:: sh

    ./run.sh

You may use this program as you see fit to get a jump start on your assignment.
Feel free to use the code directly as the basis for your parallel solution.

You may be uncomfortable with the board representation, which uses bits of a 64-bit integer.
If so, you may prefer to rewrite the board representation to use an 8 by 8 array of characters.
While this is less compact, it is equally acceptable.

If you would find it more intuitive to develop your own solution from scratch, rather than building upon the code that is provided, that is fine, but not necessary nor recommended.
The template provided is in pure (C99) C.
Cilk has extensions for both C and C++, so you may choose either if you want to write your solution from scratch.

Reversi AI
~~~~~~~~~~

You will write a shared-memory parallel program in Cilk Plus that enables a computer player to play the game.
Implement the computer player as a parallel function that plays Reversi by searching ``n`` moves ahead to select the best board position for its move.

For example, searching 1 move ahead for Player 1 means selecting the best legal move for Player 1, based only on comparing the board states that would result from any of the possible legal moves for Player 1.
Searching 2 moves ahead for Player 1 means selecting the move that would result in the best board position after Player 1's move, followed by Player 2's best move.
This process of considering alternating moves generalizes naturally to consider look-aheads of ``n`` moves.

Note that if one player cannot move, their opponent can move again if any legal moves remain.
Your search should account for this accordingly.

Constructing a sophisticated board evaluator to compute the best strategic move is beyond the scope of the assignment.
One easy method of evaluation the board computes the best move by maximizing the difference between the number of your disks and the number of the opponents disks on the board.
This will suffice for this assignment.
However, if you want to implement a more complicated evaluation function, feel free.

Minimax Algorithm
~~~~~~~~~~~~~~~~~

To implement an AI for Reversi, you will use the `Minimax algorithm`_.
The minimax algorithm is a recursive algorithm for choosing the next move in an ``n``-player game, usually a two-player game.
The minimax algorithm fits naturally into Cilk's task parallel programming model due to its recursive nature.

From Wikipedia, the pseudo-code for the algorithm is:

.. code-block:: none

    function minimax(node, depth, maximizingPlayer)
        if depth = 0 or node is a terminal node
            return the heuristic value of node

        if maximizingPlayer
            bestValue := −∞
            for each child of node
                v := minimax(child, depth − 1, FALSE)
                bestValue := max(bestValue, v)
            return bestValue

        else    (* minimizing player *)
            bestValue := +∞
            for each child of node
                v := minimax(child, depth − 1, TRUE)
                bestValue := min(bestValue, v)
            return bestValue

.. _Minimax algorithm: https://en.wikipedia.org/wiki/Minimax

Given Files
~~~~~~~~~~~

The repository you cloned includes a few folders.
The ``src`` directory contains the files that you will be modifying:

- the main program, ``reversi.c``.
- the file where you will put your optimized players, ``reversi-good-ai.c``.

Other files to note include:

- a human player, ``reversi-human.c``.
- a simple AI to play against, ``reversi-simple-ai.c``.

The simple AI chooses a random move from all of the available moves.
It is included to test your good AI against.
It can give a unique game every time.
However, *do not run experiments with a completely random AI*.
The seed chosen in the template gives an interesting game, and should be used when running experiments.
You can uncomment the random seed in ``main()`` of ``reversi.c`` while debugging to produce more than one game scenario.

A timing library is also included in ``src/hwtimer.c`` and ``include/hwtimer.h``.
It times the second player of the game, giving total runtime and average per-turn runtime.

1. Sequential Minimax
~~~~~~~~~~~~~~~~~~~~~

Implement the sequential version of your minimax algorithm ``GoodAITurnSequential()`` in ``reversi-good-ai.c``.
Your algorithm needs to consider up to a depth of ``DEPTH``, the depth parameter which is defined at the top of ``reversi-good-ai.c``.

Once you are done, modify ``main()`` in ``reversi.c`` to use your algorithm for the second player.

Prove to yourself that your minimax algorithm is working correctly.
Make sure that your algorithm makes the best decision within the working depth.

Answer the following question in your report.

#. How did you ensure that your minimax algorithm was implemented correctly?

2. Parallel Minimax
~~~~~~~~~~~~~~~~~~~

This time, implement the parallel version of your minimax algorithm ``GoodAITurnParallel()`` in ``reversi-good-ai.c``.
Again, modify ``main()`` in ``reversi.c`` to use your new algorithm.

`Reducer objects`_ can be useful for improving performance.
Unfortunately, they do not work with ``cilkscreen``, which we will be using for the next step.
Do not use them for this assignment.

If you're curious about reducers, check the `link above`__ for more details.
If you want to understand how reducers and other hyper-objects are implemented, you can look at `this paper by the Cilk creators`_.
This is optional, given that you won't be using them at all in this assignment.

Answer the following question in your report.

2. How did you feel about creating the parallelized version of your minimax algorithm? Was it difficult to turn your sequential code into parallel code?

.. _Reducer objects: https://www.cilkplus.org/docs/doxygen/include-dir/page_reducers_in_c.html
__ `Reducer objects`_
.. _this paper by the Cilk creators: http://www.fftw.org/~athena/papers/hyper.pdf

3. Check for Data Races
~~~~~~~~~~~~~~~~~~~~~~~

Your submitted program should be free of data races.
Cilk Plus' ``cilkscreen`` tool uses binary rewriting to instrument your executable.
It checks itself for data races as it runs.
Running your program with ``cilkscreen`` at the front of your execution command will check that execution for data races.

You can run ``cilkscreen`` using the shell script in the repository:

.. code-block:: sh

    ./cilkscreen.sh

``cilkscreen`` does takes a while to run, unfortunately.
You may want to leave it running and come back later.

If ``cilkscreen`` reports races, make sure that you compile your program with the ``-g`` flag.
Uncomment the ``DEBUG`` variable in the ``Makefile`` and compile your program again.
Executables compiled with ``-g`` have more detailed race reports, which will help you identify the references involved in the data races.

Answer the following question in your report:

3. Describe your experience with ``cilkscreen``. Did you find any data races? If so, how did you manage to fix them?

4. Measuring Parallelism
~~~~~~~~~~~~~~~~~~~~~~~~

Cilk Plus' ``cilkview`` tool uses binary rewriting to instrument your program to profile its parallelism.
``cilkview`` reports the following:

- the total amount of work in your program.
- the critical path length.
- the average parallelism.
- other measures, such as the total number of stack frames, spawns, and syncs.

Compile variants of ``reversi-parallel`` to have the good AI player use lookahead depths of 1, 2, 3, 4, and 5.
For each lookahead depth, use ``cilkview`` to profile your program.

You can run ``cilkview`` using the shell script in the repository:

.. code-block:: sh

    ./cilkview.sh

Unlike ``cilkscreen``, ``cilkview`` takes far less time to run.

The shell script will save the output to ``cilkoutput.txt``.
Make sure to rename the file appropriately, so that you don't overwrite it.
You will need to turn in the ``cilkview`` output file for each of your runs.

Answer the following question in your report.

4. Graph your measurements of the parallelism found by ``cilkview`` with respect to the lookahead depth. Explain why you think the graph looks like it does. Did it look like what you expected?
5. What is *burdened parallelism*, as reported by ``cilkview``? How does the burdened parallelism scale with the lookahead depth? How does it scale with the work and span reported by ``cilkview``?
6. Given the output from ``cilkview``, how do you think this program will perform on 16 cores at each lookahead depth? How about 64 cores at each lookahead depth?

5. Lookahead Depth and Workers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Run both your sequential and parallel versions of Reversi for lookahead depths of 1, 2, 3, 4, and 5.
For a depth of 5, run your parallel version with 1, 2, 4, and 8 threads.
For the other depths, run your parallel version with 4 threads.

You do not need to use ``cilkview`` for this problem.
Instead, make sure to record the time sets for each run.
You should have 5 sequential time sets and 8 parallel time sets.

You can specify the number of threads for Cilk to use by changing the ``CILK_NWORKERS`` environment variable.
The CSIF uses ``tcsh`` as its default shell.
You can set ``CILK_NWORKERS`` in ``tcsh`` using the following command:

.. code-block:: sh

    setenv CILK_NWORKERS N

where ``N`` is the number of workers you want to use.
You will need to redo this for every new session on the CSIF you start.
You can also add this environment variable to a login script, if you so choose.

If you want to verify that ``CILK_NWORKERS`` is set correctly, use the following command:

.. code-block:: sh

    echo $CILK_NWORKERS

Answer the following questions in your report.

7. How does the speedup of your parallel implementation of Reversi scale with the lookahead depth? Why do you think this is the case? Does it look like you expected?
8. How does your parallel version of Reversi scale with the number of threads? In this particular case, how well did ``cilkview`` predict the speedup? Explain why or why not ``cilkview`` predicted the performance.

Submission
----------

Archive the following into a .gz or .tgz file:

- ``reversi-good-ai.c``, with both the sequential and parallel versions of your minimax algorithm.
- The ``cilkviewer`` output for each of your runs, appropriately named.

Submit your archive, as well as the PDF of your report, on Canvas_.
*Do not include the PDF in the archive, submit it as a separate file.*

Late assignments receive an automatic 25% reduction per day they are late.
Assignments will not be accepted for late submission four days after the due date.

For your convenience, all the questions to be answered in the report are repeated below.

#. How did you ensure that your minimax algorithm was implemented correctly?
#. How did you feel about creating the parallelized version of your minimax algorithm? Was it difficult to turn your sequential code into parallel code?
#. Describe your experience with ``cilkscreen``. Did you find any data races? If so, how did you manage to fix them?
#. Graph your measurements of the parallelism found by ``cilkview`` with respect to the lookahead depth. Explain why you think the graph looks like it does. Did it look like what you expected?
#. What is *burdened parallelism*, as reported by ``cilkview``? How does the burdened parallelism scale with the lookahead depth? How does it scale with the work and span reported by ``cilkview``?
#. Given the output from ``cilkview``, how do you think this program will perform on 16 cores at each lookahead depth? How about 64 cores at each lookahead depth?
#. How does the speedup of your parallel implementation of Reversi scale with the lookahead depth? Why do you think this is the case? Does it look like you expected?
#. How does your parallel version of Reversi scale with the number of threads? In this particular case, how well did ``cilkview`` predict the speedup? Explain why or why not ``cilkview`` predicted the performance.
