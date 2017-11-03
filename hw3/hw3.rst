:Author: Jason Lowe-Power
:Editor: Justin Perona

=====================
ECS 201A - Homework 3
=====================

Due: Monday, 23 October 2017 at 3:10 PM
---------------------------------------

Submission of your code and your report will be done through Canvas_.
See the submission_ section below for more information.

.. _Canvas: https://canvas.ucdavis.edu/courses/146759

Please submit your report as a separate PDF!
Not only does this make things easier for me to grade, but I can give you feedback directly onto your report.


Overview
--------

The purpose of this assignment is to give you experience optimizing code to increase its instruction level parallelism (ILP) and to see the ILP limits of in-order processors.

We will again be using gem5.
However, for this assignment we will use the ARM ISA, not X86.
The main reason we are using ARM is because ARM (the company) has provided an implementation of a high-performance in-order processor that is similar to one of their products.

Using ARM adds an extra level of complexity in compiling your code.
Instead of just running ``gcc`` or what ever compiler you like, you must "cross-compile" the application.
Cross-compiling means compiling for a different ISA than your host machine (I assume no one in class is using an ARM processor as their main computer).

I have already downloaded and set up a cross compiler for everyone to use on the CSIF machines.
It is located at ``/home2/jlp/gcc-linaro-5.4.1-2017.05-x86_64_aarch64-linux-gnu``.

Setup
-----

Getting the code
~~~~~~~~~~~~~~~~

Like last time, you need to download the code from the jlpteaching github repo.
If you already added the repo as a remote, great!
Otherwise, go back to hw2 and follow the directions there.

You should be able to simply checkout the new branch:

.. code-block:: sh

    git fetch jlpteaching
    git checkout hw3

The test application
~~~~~~~~~~~~~~~~~~~~

In the new code, you'll find a test application: ``daxpy.cc``.
This file actaully has three many different test kernels: daxpy (double a\*x+b), daxsbxpxy (double a\*x^2+b\*x+x\*y (I made this up)), and a simple stencil.

There are two implementations of each function.
A naive version that is a simple for loop and an "unrolled" version.
You will fill in the unrolled version with a high-ILP implemenation.

The application runs all 6 functions one after another with ``m5_dump_reset_stats()`` between each function to isolate the performance.
Thus, you will have 8 different stat dumps when running the program.

Compiling
~~~~~~~~~

Once you checkout the ``hw3`` branch, you can compile the ``daxpy`` program for ARM by using ``make``.
If you insist on using your own device, you can follow the documentation on cross compiling from ARM: https://developer.arm.com/research/research-enablement/system-modeling

Importantly, you will have to recompile gem5 to use the ARM ISA.
To compile gem5 with the ARM ISA instead of x86, use the same command line, but substitute ARM for X86 as below.
There is no need to specify the CPUs on the command line this time since ARM defaults to using the Minor CPU.

.. code-block:: sh

    scons build/ARM/gem5.opt -j5

Running the test program
~~~~~~~~~~~~~~~~~~~~~~~~

To run the test program we are going to use the config files from the ARM research enablement package (see link above).
Thus, to run our application you can use the following

.. code-block:: sh

    build/ARM/gem5.opt configs/example/arm/starter_se.py daxpy --cpu=hpi

This will run the application using the "HPI" (high-performance in-order) core.
You can find the configuration for the HPI core in ``configs/common/cores/arm/HPI.py``.
You may want to look at this file now, you will need to modify it later.

Assignment
----------

Unroll the loops
~~~~~~~~~~~~~~~~

For each of the three experiemental loops, apply the loop unrolling technique to (try to) reduce the hazards.
You may want to read more about loop unrolling on wikipedia (https://en.wikipedia.org/wiki/Loop_unrolling) or watch these good youtube videos from Georgia Tech's computer architecture course (note: This was the first hit when I Google'd "loop unrolling").

Explination of loop unrolling
https://www.youtube.com/watch?v=hsQj7n-8Q3A

Once you have applied loop unrolling to the functions, run the application and measure the CPI, time, and instructions for each function.

Question 0: Prove to yourself your unrolled loops are still getting the right answer. How did you do this?

Question 1: Does unrolling the loops provide an improvement? (Answer for each function.) What kind of hazard does unrolling the loops decrease?


Increase the core resources
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Now, modify ``HPI.py`` to have more SIMD/floating point units so you can do more floating point operations in parallel and decrease the CPI.

Re-run the application.

Question 2: What effects does increasing the hardware have on both the unrolled and not unrolled functions? What hazard does adding more hardware decrease?

Optimize the loops
~~~~~~~~~~~~~~~~~~

How to get lower CPI with loop unrolling.
https://www.youtube.com/watch?v=2nx0ZCg5D9g

Now, further modify your unrolled loops to try to further decrease the CPI.

Question 3: Which versions are better (pick the right metric or multiple metrics). How did you further optimize? Did it work the way you expected?


Use the compiler to optimize
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Modify the Makefile to use the ``-O3`` option for GCC instead of ``-O1``.

Question 4: What performs better, your optimized loops or the GCC optimized? What about the instructions are different and why do you see the performance difference?
