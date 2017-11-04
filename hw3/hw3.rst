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
You may want to read more about loop unrolling on wikipedia_ or watch these `good youtube videos`_ from Georgia Tech's computer architecture course (note: This was the first hit when I Google'd "loop unrolling").

.. _good youtube videos: https://www.youtube.com/watch?v=hsQj7n-8Q3A

**Question 1**: Prove to yourself your unrolled loops are still getting the right answer. How did you do this?

Once you have applied loop unrolling to the functions, run the application and measure the CPI, time, and instructions for each function.

**Question 2**: Does unrolling the loops provide an improvement? (Answer for each function.) What kind of hazard does unrolling the loops decrease?

.. _wikipedia: https://en.wikipedia.org/wiki/Loop_unrolling

Increase the core resources
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Now, modify ``HPI.py`` to have more SIMD/floating point units so you can do more floating point operations in parallel and decrease the CPI.
You will want to modify ``configs/common/cores/arm/HPI.py``.
Specifically, you need to change the ``HPI_FUPool`` class to have more functional units.
You can duplicate the ``HPI_FloatSimdFU()`` an additional three times *at the end* of the ``funcUnits`` list.

Re-run the application and observe any differences in execution characteristics.

**Question 3**: What effects does increasing the hardware have on both the unrolled and not unrolled functions? What hazard does adding more hardware decrease?

Optimize the loops
~~~~~~~~~~~~~~~~~~

Now, further modify your unrolled loops to try to further decrease the CPI.
It may be helpful to look at the generated assembly code while you are making these optimizations.
For this, you can use ``objdump``.
However, you will have to use the version that comes with the cross compiler, not the version that is default on the system (which only supports the host platform, x86).

I have installed the cross compiler version at ``/home2/jlp/gcc-linaro-5.4.1-2017.05-x86_64_aarch64-linux-gnu/bin`` on the CSIF machines.
You will probably want to use ``objdump -dC daxpy``.
The ``-d`` option disassembles to convert from machine code to assembly, and the ``-C`` option "demangles" the names of functions so they are more readable.
You can find more information about ``objdump`` either by looking at the manpage (``man objdump``) or Googling.

`Another video from the Georgia Tech class`_ provides a good overview of how to get lower CPI by rearranging instructions.

.. _Another video from the Georgia Tech class: https://www.youtube.com/watch?v=2nx0ZCg5D9g

**Question 4**: How did you further optimize each function? Did your optimizations change the behavior as expected (e.g., improve performance, lower CPI)?

Use the compiler to optimize
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Modify the Makefile to use the ``-O3`` option for GCC instead of ``-O1``.
This tells the compiler to use the highest level of optimizations.

You now have four different versions of each of the functions.
 - Unoptimized simple loops
 - Hand-optimized unrolled loops
 - GCC-optimized simple loops
 - GCC-optimized hand-optimized unrolled loops

**Question 5**: Compare all four of these options on whatever metrics you deem appropriate. Also discuss the tradeoffs in your effort as a programmer.

**Question 6**: These were very simple loops. Most programs are much more complicated (even the inner-loop kernels). Do you think it's ever worth hand-optimizing your code, or do you think the compiler optimizations are good enough? (Note: If you use proprietary compilers (e.g., Intel's compiler for Intel machines), you will see better optimized code than what you saw in this assignment.)

Extra credit
~~~~~~~~~~~~

The MinorCPU, which is the model that the HPI core is based on, is a very flexible CPU model.
All of the parameters for the HPI CPU can be found in the ``HPI.py`` file.

Can you modify these parameters (specifically in the ``HPI`` class) to acheive even higher performance and lower CPI for the loops?
You can choose to concentrate on only one version of the loops out of the four versions above.

**Question 7**: How did you modify the hardware? Was it effective?

.. _submission:

Submission
----------

Archive the following into a .gz or .tgz file:

- The statistics files from your runs of ``daxpy``, appropriately named.
- The modified ``daxpy.cc`` file that has your unrolled loops.

Submit your archive, as well as the PDF of your report, on Canvas_.
**Do not include the PDF in the archive, submit it as a separate file.**

For your convenience, all the questions to be answered in the report are repeated below.

#. How did you prove that your unrolled loops are correct code?
#. Does unrolling the loops provide an improvement? (Answer for each function.) What kind of hazard does unrolling the loops decrease?
#. What effects does increasing the hardware have on both the unrolled and not unrolled functions? What hazard does adding more hardware decrease?
#. How did you further optimize each function? Did your optimizations change the behavior as expected (e.g., improve performance, lower CPI)?
#. Compare all four of these options on whatever metrics you deem appropriate. Also discuss the tradeoffs in your effort as a programmer.
#. Do you think it's ever worth hand-optimizing your code, or do you think the compiler optimizations are good enough?

Extra credit
~~~~~~~~~~~~

7. How did you modify the hardware? Was it effective?
