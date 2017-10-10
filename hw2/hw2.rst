:Author: Jason Lowe-Power
:Editor: Justin Perona

=====================
ECS 201A - Homework 2
=====================

Due: Monday, 23 October 2017 at 3:10 PM
-----------------------------------------

Submission of your code and your report will be done through Canvas_.
See the submission_ section below for more information.

.. _Canvas: https://canvas.ucdavis.edu/courses/146759

Overview
--------

The purpose of this assignment is for you to get hands-on experience with the performance impact of cache designs and cache-conscious optimizations.

This assignment will build off of `Part II`_ of the `Learning gem5`_ book.
You are not required to go through `Part II`_ of the book.
However, it may help you to understand the design of the cache to go through the book.
I suggest you read through it, at a minimum.

I wrote the `Learning gem5`_ book that you will be going through.
If you have any feedback about errors, big or small, please let me know!
`Email me`_ with the subject "gem5-tutorial comments" with any comments or errors you find.

.. _email me: mailto:jlowepower@ucdavis.edu
.. _Learning gem5: http://learning.gem5.org/book/
.. _Part II: http://learning.gem5.org/book/part2

Getting the code
----------------

This homework requires some modifications to gem5.
However, I have already made them, so you don't have to.
Follow these steps to download my modifications.

Note: If there are any mistakes in my code (which is not unlikely), I will update this repository.
It's a good idea for you to use git so you can pull any updates easily.

In your gem5 repository, you should add a new remote for gem5, then checkout the hw2 branch.

.. code-block:: sh

    git remote add jlpteaching https://github.com/jlpteaching/gem5.git
    git fetch jlpteaching
    git checkout hw2

The assignment
--------------

Once you checkout the hw2 branch, there will be some new files.
First, in ``src/learning_gem5/part2`` there will be the SimpleCache files (SimpleCache.py, simple_cache.hh, and simple_cache.cc).
These implement a *very* simple cache model.
This cache is a *blocking* cache that is either fully associative or direct-mapped.
You will use this cache model for this assignment.

Next, there is ``run_mm.py`` which is a gem5 runscript for this homework.
This file creates a simple single-core system with an in-order CPU and a single level of cache.
The cache is by default 16 KB and the CPU is running at 4 GHz.
**Do not change anything in this file unless explicitly told to do so in this assingment.**

Finally, there is the application that you will be running for this assignment.
The file ``mm.cpp`` contains a matrix multiplication implementation.
This is a `blocked matrix multiply`_ (except for ``nbmm``).
You can build four variants of this algorithm with the included ``Makefile`` by running:

.. _blocked matrix multiply: https://en.wikipedia.org/wiki/Block_matrix#Block_matrix_multiplication

.. code-block:: sh

    make


Before you simulate
~~~~~~~~~~~~~~~~~~~

Before beginning to run your simulations, look at the implemtation for the blocked matrix multiply.

There are four different implementations of matrix multiply when you run ``make``.
There are three different blocked matrix multiple implemenations with block sizes of 16x16, 32x32, and 64x64.
Additionally, there is a normal (not blocked) matrix multiply implementation.

You will be running experiements for computing the matrix multiplications of A x B = C for matricies of 128x128 ``doubles``.
Note: a ``double`` is 8 bytes.

Answer these questions before continuing:

#. What is the memory footprint for this applications?
#. What is the cache footprint for each of the four implementations?
#. Assume you have a cache of 16KB. For each of the four implementations do you expect a high or low hit ratio?


Adding gem5 stats
~~~~~~~~~~~~~~~~~~

First, you will modify the simple cache implementation in gem5 to count the number of cold or compulsary misses.
For this, you will use the gem5 ``Stats`` package.
You can use the ``hits`` and ``misses`` statistics already part of the SimpleCache as examples for gem5 statistics.

You will need to make two changes in the ``simple_cache.cc`` file.
You will need to add statistics for the cold/compulsary misses to ``insertFA`` and ``insertDM`` functions.

Fully associative cache experiements
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For this part of assignment you will need to run four experiements.
For each implementation of matrix multiply, run gem5 using a fully associative 16KB cache with a latency for 4 cycles.
(This is the default in run_mm.py.)

Answer the following questions:

#. Which implemenation performs the best?
#. Which implementation has the highest hit ratio?
#. Does the hit ratio predict the performance? Why or why not? (I'm looking for a few sentences at least here.)


Other cache designs
~~~~~~~~~~~~~~~~~~~

Fully associative caches are expensive in terms of area, power, and latency.
If we were using a direct-mapped cache we could reduce the hit latency to a single cycle!

Run four more experiements.
This time use a direct-mapped cache (``assoc = "DirectMapped"``) and a single cycle (1) latency.

#. Which has a higher hit ratio, direct-mapped or fully-associative?
#. Now that you've run both fully associative and direct mapped, report the cold misses, conflict misses, and capacity misses for all four implementations.
#. For the direct-mapped cache, what is conflict miss to capacity miss ratio?
#. Why are most misses conflict (or capacity) misses for the direct-mapped cache?
#. Which cache performs better the fast direct-mapped cache or slower fully associative cache?
#. (No need to run any more experiments). Make an educated guess: What would the performance for a 2-way set associative cache be? 4-way?


Extra credit
~~~~~~~~~~~~

My simple cache implementation is not very good.
For example, it writes back data to main-memory whether or not it is dirty.
Modify the cache implementation to only write back dirty data by adding a dirty bit to the cache.
(You will have to do something different for the set-associative and direct-mapped case since I didn't unify the design.)

#. Why (or why not) does adding a dirty bit have a significant performance impact for this cache implementation running matrix multiply?


.. _submission:

Submission
----------

Archive the following into a GZ or TGZ file:

- Any code that you modified in gem5 (e.g., ``simple_cache.cc`` since you added the cold miss counters).

The answers to the following questions in pdf (copied from above).

#. What is the memory footprint for this applications?
#. What is the cache footprint for each of the four implementations?
#. Assume you have a cache of 16KB. For each of the four implementations do you expect a high or low hit ratio?

#. Which implemenation performs the best?
#. Which implementation has the highest hit ratio?
#. Does the hit ratio predict the performance? Why or why not? (I'm looking for a few sentences at least here.)

#. Which has a higher hit ratio, direct-mapped or fully-associative?
#. Now that you've run both fully associative and direct mapped, report the cold misses, conflict misses, and capacity misses for all four implementations.
#. For the direct-mapped cache, what is conflict miss to capacity miss ratio?
#. Why are most misses conflict (or capacity) misses for the direct-mapped cache?
#. Which cache performs better the fast direct-mapped cache or slower fully associative cache?
#. (No need to run any more experiments). Make an educated guess: What would the performance for a 2-way set associative cache be? 4-way?


Hints
-----

- The benchmark ROI has been surounded with dump and reset stat magic instructions. Thus, there are 3 different statistic dumps. *Use the second dump for all of your answers!* This is the stat dump for the ROI!
