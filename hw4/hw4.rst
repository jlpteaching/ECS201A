:Author: Jason Lowe-Power
:Editor: Justin Perona

=====================
ECS 201A - Homework 4
=====================

Due: Monday, 27 November 2017 at 3:10 PM
----------------------------------------

Submission of your code and your report will be done through Canvas_.
See the Submission_ section below for more information.

.. _Canvas: https://canvas.ucdavis.edu/courses/146759

*Please submit your report as a separate PDF, and not in an archive!*
Not only does this make things easier for me to grade, but I can give you feedback directly onto your report.

Overview
--------

In this assignment, you are going to experiment with different designs for an out of order core, looking at the trade-offs between complexity, performance, energy, area, and power.

For this assignment, we'll be again using gem5, but this time we will use slightly more "realistic" workloads.
I've included two workloads from the `PARSEC benchmark suite`_.
PARSEC is designed to model "desktop" workloads.
It was specifically created to work for multi-core processors, but we will look at a single core.
You can find more information about PARSEC from the `PARSEC paper`_.

.. _PARSEC benchmark suite: http://parsec.cs.princeton.edu/
.. _PARSEC paper: http://parsec.cs.princeton.edu/publications.htm

Setup
-----

Getting the Code
~~~~~~~~~~~~~~~~

You should be getting used to this part by now :).

Like last time, you need to download the code from the ``jlpteaching`` Github repository.
If you already added the repository as a remote, great!
Otherwise, go back to homework 2 and follow the directions there.

You should be able to simply checkout the new branch:

.. code-block:: sh

    git fetch jlpteaching
    git checkout hw4

Remember to recompile gem5 after checking out the ``hw4`` branch.
I have made a small change to gem5 to enable you to model a wider pipeline than normally reasonable.
The compilation command is the same as in homework 3:

.. code-block:: sh

    scons build/ARM/gem5.opt -jX

where ``X`` in ``-jX`` is the number of cores in your machine, plus one.

Be careful if you plan on using your own machine.
You will need to download a cross-compiler, as in the last assignment.

Additionally, if you recently upgraded to Ubuntu 17.10, or are using ``gcc`` 7.2, gem5 will not compile as is.
This is due to changes in ``gcc`` starting with 7.2.
I am currently working on updating gem5 for the new version, but it will take a while.
The easiest solution is to use the CSIF.
If you really want to use your own machine, you can download a previous version of ``gcc``.

Running the Test Applications
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I have included the code for two of the PARSEC workloads: ``blackscholes`` (a partial differential equation solver used in options trading), and ``streamcluster`` (a simple clustering kernel).
First, you need to build these workloads by running ``make``.
Like the last homework, you will be using an ARM cross compiler that is installed on the CSIF machines in my ``home2`` directory.

After building the workloads, you can use the following command line to run the workloads with the configuration for this assignment.
On a side note, I don't think using the ``se.py`` example config script is good practice.
If you go on to use gem5 at another point, I strongly encourage you to *write your own config script from scratch*.
However, this time we'll use ``se.py`` to quickly get results.

To run ``streamcluster``, use the following:

.. code-block:: sh

    build/ARM/gem5.opt --outdir=<your outdir here> configs/example/se.py \
    --cpu-type=hw4_core -c streamcluster \
    -o "5 12 8 256 256 150 none output.txt 1" --l2cache --caches

To run ``blackscholes``, use the following:

.. code-block:: sh

    build/ARM/gem5.opt --outdir=<your outdir here> configs/example/se.py \
    --cpu-type=hw4_core -c blackscholes -o "1 in_256.txt out" --l2cache --caches

You will definitely want to turn the above commands into shell scripts.
Also, I encourage you to explicitly set a different output directory for each workload and for each experiment you run with the workloads.
This will make the next step easier.

These command lines will use the ``se.py`` run script.
They pass a number of parameters.

- The ``cpu-type`` is the special reconfigurable CPU that you will be investigating for this assignment. You can find the code for this CPU in ``configs/common/cores/arm/hw4_core.py``.
- We pass an executable to the script with ``-c`` and the parameters for that executable with ``-o``.
- We specify that we want to use caches when simulating with ``--l2cache`` and ``--caches``.

Modeling Physical Parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There is one other new script included in the hw4 branch: ``hw4_calc.py``.
This script is similar in spirit to McPAT_, a power, area, and timing modeling framework.
Like McPAT, this script takes in the system configuration and statistics based on the execution of an application, and returns a set of physical parameters.
Namely, ``hw4_calc.py`` returns the area of your design, the energy, and the power of your core.

**Important:** The numbers generated by ``hw_calc.py`` *are not real numbers*.
I literally made them all up as an exercise.
**Do not use these numbers in any real research!**
It won't end well for you, or your research.

The ``hw4_calc.py`` script takes a single positional argument: the gem5 output directory.
For example, you can run the ``hw4_calc.py`` script like below.
You will likely want to substitute ``m5out`` with whatever directory you used for any particular experiment.

::

    $ ./hw4_calc.py m5out
    This is provided as an exercise only.
    These are not realistic numbers. I literally made all of this up.

    Your application took 9301007 ns
    Your core is 8.647255 mm^2
    Your core consumed 0.067016 J
    Your core dissipated 7.205240 Watts

.. _McPAT: http://www.hpl.hp.com/research/mcpat/

Assignment
----------

The goal of this assignment is to design the "best" out of order core.
We will look at different metrics for "best".

You can change the design of the ``hw4_core`` by modifying the Python dictionary at the top of the ``configs/common/cores/arm/hw4_core.py`` file.

.. code-block:: python

    # Change these parameters!
    config = {
        'fetchWidth': 4,
        'decodeWidth': 4,
        'renameWidth': 4,
        'issueWidth': 4,
        'dispatchWidth': 4,
        'wbWidth': 4, # Causes bug if 1, use min 2
        'commitWidth': 4,
        'squashWidth': 4,

        'fetchBufferSize': 4,

        'numIQEntries': 8,
        'numROBEntries': 8,

        'LQEntries': 8,
        'SQEntries': 8,

        'numSimpleIntUnits': 2, # only simple things like add/sub, etc.
        'numComplexIntUnits': 1, # things like mult/div
        'numFPUnits': 1,
        'numLoadUnits': 1,
        'numStoreUnits': 1,
    }

You can also investigate the source code for the out of order CPU to get a better understanding of what these parameters do.
The code can be found in ``src/cpu/o3``.
Understanding this code is not required for this assignment.

The initial design of the ``hw4_core`` is pretty random.
You can look at the file ``configs/common/cores/arm/O3_ARM_v7a.py`` for an example configuration that resembles a realistic ARM core design.

1. Maximize Performance
~~~~~~~~~~~~~~~~~~~~~~~

First, find the parameters that lead to the *best performance* for ``blackscholes`` and ``streamcluster``.
Choose *one* configuration, and run both applications with that configuration.
For this part, list the final parameters you chose, and the performance, area, energy for the computation, and power.

Answer the following questions in your report.

#. Why did you choose the parameters you changed to maximize performance?
#. Which parameters had the biggest effect on performance?
#. What is the power impact of getting high performance?
#. How do the two applications compare? Did you have to make compromises in the design to get higher performance for both workloads?

2. Minimize Energy
~~~~~~~~~~~~~~~~~~

Next, find the parameters that lead to the *minimum energy* for ``blackscholes`` and ``streamcluster``.
Choose *one* configuration, and run both applications with that configuration.
For this part, list the final parameters you chose, and the performance, area, energy for the computation, and power.

Answer the following questions in your report.

#. Why did you choose the parameters you changed to minimize energy?
#. Which parameters had the biggest effect on energy?
#. What is the relationship between energy and power?
#. How do the two applications compare? Did you have to make compromises in the design to get lower energy for both workloads?

3. Explore Other Metrics
~~~~~~~~~~~~~~~~~~~~~~~~

Sometimes, not just performance or just energy matters.
Therefore, often, the product of energy and delay (ED) is used as a single metric.
Similarly, if the performance is more important than the energy, you can use different powers of the delay (e.g., ED^2 or ED^3) to give more weight to the performance.

For this part, find the metric that minimizes ``energy * delay^2`` (energy delay squared) for ``blackscholes`` and ``streamcluster``.
Choose *one* configuration and run both applications with that configuration.
For this part, list the final parameters you chose, and the performance, area, energy for the computation, and power.

Answer the following questions in your report.

#. How do your parameters differ from maximizing performance or minimizing energy?
#. Which parameters had the largest effect? Put another way, which parameters make the biggest impact on performance while not increasing energy *too* much?

Submission
----------

For this assignment, you will only submit a PDF of your report.
Your report will contain the final set of parameters you used for each section, and the answers to the above questions.
You *are not required* to answer the above questions in a linear fashion.
You are *strongly encouraged* to use graphs and other data visualizations to communicate the answers to the above questions.

You also do not need to work on each part on its own.
Feel free to run many simulations, then go back and find the configuration with the maximum performance, minimum energy, and minimum energy delay squared.

I will be looking for the answers to all of the above questions in the report.
If you choose not to answer the questions linearly, ensure that when someone else is reading the report (e.g., me), they will be able to find the answers.

Submit the PDF of your report on Canvas_.
*Do not include the PDF in an archive, submit it by itself.*

Late assignments receive an automatic 25% reduction per day they are late.
Assignments will not be accepted for late submission four days after the due date.

For your convenience, all the questions to be answered in the report are repeated below.
As said above, you do not need to answer them in this order.
This is purely to make the questions easier to find.

Make sure you include your final parameters for each part.

**1. Maximize Performance**

#. Why did you choose the parameters you changed to maximize performance?
#. Which parameters had the biggest effect on performance?
#. What is the power impact of getting high performance?
#. How do the two applications compare? Did you have to make compromises in the design to get higher performance for both workloads?

**2. Minimize Energy**

#. Why did you choose the parameters you changed to minimize energy?
#. Which parameters had the biggest effect on energy?
#. What is the relationship between energy and power?
#. How do the two applications compare? Did you have to make compromises in the design to get lower energy for both workloads?

**3. Explore Other Metrics**

#. How do your parameters differ from maximizing performance or minimizing energy?
#. Which parameters had the largest effect? Put another way, which parameters make the biggest impact on performance while not increasing energy *too* much?
