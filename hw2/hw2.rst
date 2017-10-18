:Author: Jason Lowe-Power
:Editor: Justin Perona

=====================
ECS 201A - Homework 2
=====================

Due: Monday, 23 October 2017 at 3:10 PM
---------------------------------------

Submission of your code and your report will be done through Canvas_.
See the submission_ section below for more information.

.. _Canvas: https://canvas.ucdavis.edu/courses/146759

Overview
--------

The purpose of this assignment is for you to get hands-on experience with the performance impact of cache designs and cache-conscious optimizations.

This assignment will build off of `Part II`_ of the `Learning gem5`_ book.
You are not required to go through Part II.
However, going through it may help you understand the design of the cache.
I suggest you read through it, at a minimum.

I wrote the book that you will be going through.
If you have any feedback about errors, big or small, please let me know!
`Email me`_ with the subject "gem5-tutorial comments" if you find any errors or have any comments.

.. _Part II: http://learning.gem5.org/book/part2
.. _Learning gem5: http://learning.gem5.org
.. _email me: mailto:jlowepower@ucdavis.edu

Getting the Code
----------------

This homework requires some modifications to gem5.
However, I have already made them, so you don't have to.
Follow these steps to download my modifications.

Note: If there are any mistakes in my code (which is not unlikely), I will update this repository.
It's a good idea for you to use Git so you can pull any updates easily.
If you used Mercurial to install gem5 originally, now is a good time to switch over to Git.

In your gem5 repository, update gem5, add my repository as a new remote for gem5, then check out the ``hw2`` branch:

.. code-block:: sh

    git pull origin master
    git remote add jlpteaching https://github.com/jlpteaching/gem5.git
    git fetch jlpteaching
    git checkout hw2

After checking out the ``hw2`` branch, you'll also want to recompile gem5 to access the new files in ``src/learning_gem5/part2``.
Recompile gem5 in the same way you compiled it in Homework 1:

::

    scons build/X86/gem5.opt -jX \
    CPU_MODELS=AtomicSimpleCPU,TimingSimpleCPU,O3CPU,MinorCPU

where ``X`` in ``-jX`` is the number of cores in your system, plus one.
You will need to recompile gem5 every time you make modifications to gem5 files.

Assignment
----------

Once you check out the hw2 branch, there will be some new files.
First, in ``src/learning_gem5/part2``, there will be the SimpleCache files: ``SimpleCache.py``, ``simple_cache.hh``, and ``simple_cache.cc``.
These implement a *very* simple cache model.
This cache is a *blocking* cache that is either fully associative or direct mapped.
You will use this cache model for this assignment.

Next, there is ``run_mm.py``, which is a gem5 run script for this homework.
This file creates a simple single-core system with an in-order CPU and a single level of cache.
The cache is by default 16 KB, and the CPU is running at 4 GHz.
**Do not change anything in this file unless explicitly told to do so in this assignment.**

Finally, there is the application that you will be running for this assignment.
The file ``mm.cpp`` contains a matrix multiplication implementation.
This is a `blocked matrix multiply`_ (except for ``nbmm``).
You can build four variants of this algorithm.

Compile all of them with the included ``Makefile`` by running

.. code-block:: sh

    make

.. _blocked matrix multiply: https://en.wikipedia.org/wiki/Block_matrix#Block_matrix_multiplication

Before You Simulate
~~~~~~~~~~~~~~~~~~~

Before beginning to run your simulations, look at the implementation for the blocked matrix multiply.

There are four different implementations of matrix multiply when you run ``make``.
There are three different blocked matrix multiple implementations: ``mm16``, ``mm32``, and ``mm64``, with block sizes of 16x16, 32x32, and 64x64, respectively.
Additionally, ``nbmm`` is a normal (not blocked) matrix multiply implementation.

You will be running experiments for computing the matrix multiplications of A x B = C for matrices of 128x128 ``doubles``.
Note: a ``double`` is 8 bytes.

Answer these questions in your report before continuing:

#. What is the memory footprint of this application?
#. What is the cache footprint for each of the four implementations?
#. Assume you have a cache of 16 KB. For each of the four implementations, do you expect a low or high hit rate?

The cache footprint is how much of the data is used by the working set of the application.
If you need additional help understanding what it is, `this 1987 paper`_ from the University of Massachusetts, Amherst, has a description.

.. _this 1987 paper: https://dl.acm.org/citation.cfm?id=32979

Adding gem5 Statistics
~~~~~~~~~~~~~~~~~~~~~~

First, you will modify the simple cache implementation in gem5 to count the number of compulsory (also known as cold) misses.
For this, you will use the gem5 ``Stats`` package.
You can use the ``hits`` and ``misses`` statistics that are already part of the SimpleCache as an example of how statistics in gem5 work.

You will need to make a couple of changes in the ``simple_cache.cc`` and ``simple_cache.hh`` files.
You will need to add statistics for the compulsory misses to the ``insert()`` function.
You may find using a `std::set`_ useful.

Additionally, in order to count the cold misses for *just the region of interest*, you will need to fill in the function ``resetColdMisses()``.
This function is called whenever the stats are reset (e.g., before the beginning of the ROI in matrix multiply).

I have included the statistic in the code already: the variable ``coldMisses``.
You only have to add a structure to track the cold misses and the logic to increment the ``coldMisses`` statistic.

.. _std::set: http://en.cppreference.com/w/cpp/container/set

Fully Associative Cache Experiments
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For this part of assignment, you will need to run four experiments.
For each implementation of matrix multiply, run gem5 using a fully associative 16 KB cache with a latency of 4 cycles.
(This is the default in ``run_mm.py``.)

Answer the following questions in your report:

4. Which implementation performs the best?
5. Which implementation has the highest hit ratio?
6. Does the hit ratio predict the performance? Why or why not? (Your answer should be at least a few sentences long.)

Other Cache Designs
~~~~~~~~~~~~~~~~~~~

Fully associative caches are expensive in terms of area, power, and latency.
If we were using a direct mapped cache, we could reduce the hit latency to a single cycle!

Run four more experiments.
This time, use a direct mapped cache (``assoc = "DirectMapped"``) and a single cycle (1) latency.

Answer the following questions in your report:

7. Which has a higher hit ratio, direct mapped or fully associative?
8. Now that you've tested both fully associative and direct mapped caches, report the compulsory, conflict, and capacity misses for all four implementations.
9. For the direct mapped cache, what is the conflict to capacity miss ratio?
10. Why are most misses conflict (or capacity) misses for the direct mapped cache?
11. Which cache performs better: the fast direct mapped cache, or the slower fully associative cache?
12. *Don't run any more experiments for this question, use only your intuition.* Make an educated guess: what would the performance for a 2-way set associative cache be? 4-way?

Extra Credit
~~~~~~~~~~~~

My simple cache implementation is not very good.
For example, it writes back data to main memory, whether or not it is dirty.

Modify the cache implementation to only write back dirty data, by adding a dirty bit to the cache.
You will have to do something different for the set associative and direct mapped caches, since I didn't unify the design.

Add an answer to the following question to your report.

13. Why (or why not) does adding a dirty bit have a significant performance impact for this cache implementation running matrix multiply?

.. _submission:

Submission
----------

Archive the following into a .gz or .tgz file:

- Any code that you modified in gem5, such as ``simple_cache.cc``, since you added the compulsory miss counters.

Submit your archive, as well as the PDF of your report, on Canvas_.

For your convenience, all the questions to be answered in the report are repeated below.

#. What is the memory footprint of this application?
#. What is the cache footprint for each of the four implementations?
#. Assume you have a cache of 16 KB. For each of the four implementations, do you expect a low or high hit rate?
#. Which implementation performs the best?
#. Which implementation has the highest hit ratio?
#. Does the hit ratio predict the performance? Why or why not? (Your answer should be at least a few sentences long.)
#. Which has a higher hit ratio, direct mapped or fully associative?
#. Now that you've tested both fully associative and direct mapped caches, report the compulsory, conflict, and capacity misses for all four implementations.
#. For the direct mapped cache, what is the conflict to capacity miss ratio?
#. Why are most misses conflict (or capacity) misses for the direct mapped cache?
#. Which cache performs better: the fast direct mapped cache, or the slower fully associative cache?
#. *Don't run any more experiments for this question, use your intuition.* Make an educated guess: what would the performance for a 2-way set associative cache be? 4-way?

**Extra Credit**

13. Why (or why not) does adding a dirty bit have a significant performance impact for this cache implementation running matrix multiply?

Hints
-----

- The benchmark region of interest (ROI) has been surrounded with dump and reset stat magic instructions. Thus, there are three different statistic dumps. *Use the second dump for all of your answers!* This is the stat dump for the ROI!
