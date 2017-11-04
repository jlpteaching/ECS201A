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
`Email me`_ with the subject "gem5-book comments" if you find any errors or have any comments.

.. _Part II: http://learning.gem5.org/book/part2
.. _Learning gem5: http://learning.gem5.org
.. _email me: mailto:jlowepower@ucdavis.edu

Setup
-----

Getting the Code
~~~~~~~~~~~~~~~~

This homework requires some modifications to gem5.
However, I have already made them, so you don't have to.
Follow these steps to download my modifications.

If there are any mistakes in my code (which is not unlikely), I will update this repository.
It's a good idea for you to use Git so you can pull any updates easily.
If you used Mercurial to install gem5 originally, now is a good time to switch over to Git.

In your gem5 repository, update gem5, add my repository as a new remote for gem5, then check out the ``hw2`` branch:

.. code-block:: sh

    git pull origin master
    git remote add jlpteaching https://github.com/jlpteaching/gem5.git
    git fetch jlpteaching
    git checkout hw2

.. _above:

gem5 Source Additions
~~~~~~~~~~~~~~~~~~~~~

Once you check out the hw2 branch, there will be some new files.

First, in ``src/learning_gem5/part2``, there will be the SimpleCache files: ``SimpleCache.py``, ``simple_cache.hh``, and ``simple_cache.cc``.
These implement a *very* simple cache model.
This cache is a *blocking* cache that is either fully associative or direct mapped.
You will use this cache model for this assignment.

Since we have these additional files, you'll want to recompile gem5 to be able to use them.
Recompile gem5 in the same way you compiled it in Homework 1:

.. code-block:: sh

    scons build/X86/gem5.opt -jX \
    CPU_MODELS=AtomicSimpleCPU,TimingSimpleCPU,O3CPU,MinorCPU

where ``X`` in ``-jX`` is the number of cores in your system, plus one.

You will need to recompile gem5 every time you make modifications to gem5 files.

gem5 Script
~~~~~~~~~~~

Next, there is ``run_mm.py``, which is a gem5 run script for this homework.
This file creates a simple single-core system with an in-order CPU and a single level of cache.
The cache is by default 16 KB, and the CPU is running at 4 GHz.
**Do not change anything in this file unless explicitly told to do so in this assignment.**

Application
~~~~~~~~~~~

Finally, there is the application that you will be running for this assignment.
The file ``mm.cpp`` contains a matrix multiplication implementation.
This is a `blocked matrix multiply`_ (except for ``nbmm``).
You can build four variants of this algorithm, each with different block sizes.

Compile all of them with the included ``Makefile`` by running

.. code-block:: sh

    make

.. _blocked matrix multiply: https://en.wikipedia.org/wiki/Block_matrix#Block_matrix_multiplication

Assignment
----------

Before You Simulate
~~~~~~~~~~~~~~~~~~~

Before beginning to run your simulations, look at the implementation for the blocked matrix multiply in ``mm.cpp``.

There are four different implementations of matrix multiply when you run ``make``.
There are three different blocked matrix multiple implementations: ``mm16``, ``mm32``, and ``mm64``, with block sizes of 16x16, 32x32, and 64x64, respectively.
Additionally, ``nbmm`` is a normal (not blocked) matrix multiply implementation.

You will be running experiments for computing the matrix multiplications of A x B = C for matrices of 128x128 ``doubles``.
A ``double`` is 8 bytes.

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

You will need to understand the gem5 Packet object.
Packets contain the memory command, address, data, etc.
The Learning gem5 book `describes the Packets in more detail`_, and you can also check out the code in ``src/mem/packet.hh``.

Additionally, in order to count the cold misses for *just the region of interest*, you will need to fill in the function ``resetColdMisses()``.
This function is called whenever the stats are reset (e.g., before the beginning of the ROI in the matrix multiply).
I have included the statistic in the code already: the variable ``coldMisses``.
You only have to add a structure to track the cold misses and the logic to increment the ``coldMisses`` statistic.

.. _std::set: http://en.cppreference.com/w/cpp/container/set

.. _describes the Packets in more detail: http://learning.gem5.org/book/part2/memoryobject.html#packets

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
This time, use a direct mapped cache (``assoc = "DirectMapped"``) and a single (1) cycle latency.

Answer the following questions in your report:

7. Which has a higher hit ratio, direct mapped or fully associative?
8. Now that you've tested both fully associative and direct mapped caches, report the compulsory, conflict, and capacity misses for all four implementations.
9. For the direct mapped cache, what is the conflict to capacity miss ratio?
10. Why are most misses conflict (or capacity) misses for the direct mapped cache?
11. Which cache performs better: the fast direct mapped cache, or the slower fully associative cache?
12. *Don't run any more experiments for this question, use only your intuition.* Make an educated guess: what would the performance for a 2-way set associative cache be? What about a 4-way set associative cache?

Extra Credit
~~~~~~~~~~~~

The simple cache implementation is not very good.
For example, it writes back data to main memory, whether or not it is dirty.

Modify the cache implementation to only write back dirty data, by adding a dirty bit to the cache.
You will have to do something different for the set associative and direct mapped caches, since the designs are not unified.

Add an answer to the following question to your report.

13. Why (or why not) does adding a dirty bit have a significant performance impact for this cache implementation, running matrix multiply?

.. _submission:

Submission
----------

Archive the following into a .gz or .tgz file:

- Any code that you modified in gem5, such as ``simple_cache.cc``, since you added the compulsory miss counters.
- The eight statistics files from your runs of ``run_mm.py``, appropriately named. You should have a file for each of the cache types run with each of the matrix multiply executables.

Submit your archive, as well as the PDF of your report, on Canvas_.
**Do not include the PDF in the archive, submit it as a separate file.**

Late assignments receive an automatic 25% reduction per day they are late.
Assignments will not be accepted for late submission four days after the due date.

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
#. *Don't run any more experiments for this question, use only your intuition.* Make an educated guess: what would the performance for a 2-way set associative cache be? What about a 4-way set associative cache?

**Extra Credit**

13. Why (or why not) does adding a dirty bit have a significant performance impact for this cache implementation, running matrix multiply?

Common Errors
-------------

error: unknown directive
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sh

    util/m5/m5op_x86.S:72:94: error: unknown directive
    .globl m5_init_param; .func m5_init_param; m5_init_param: ...

When attempting to compile the matrix multiply implementations, you may get this error.
The LLVM assembler has an issue with some of the directives in the assembly files in gem5.
Switch over to using ``gcc``, if you are able.

If you are using OS X, one method of moving to ``gcc`` is through MacPorts_.
If you don't want to go through the trouble, you can compile the matrix multiply on the CSIF, and copy the binaries back to your computer.

.. _MacPorts: https://www.macports.org/

Multiple statistics dump files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The benchmark region of interest (ROI) has been surrounded with dump and reset stat magic instructions.
Thus, there are three different statistic dumps.
*Use the second dump for all of your answers!*
This is the stat dump for the ROI!

NameError: name 'SimpleCache' is not defined
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

gem5 cannot find the definitions for ``SimpleCache`` and its associated functions.
The issue is that you didn't recompile gem5 as described above_.
You will need to recompile gem5 at the beginning, and after every time you make changes to gem5 source files.
