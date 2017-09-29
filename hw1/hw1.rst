:Author: Jason Lowe-Power
:Editor: Justin Perona

=====================
ECS 201A - Homework 1
=====================

Due: Monday, 9 October 2017 at 3:10 PM
--------------------------------------

Submission of your code and your report will be done through Canvas_.
See the submission_ section below for more information.
Remember to bring a *paper* copy of your report to class.

.. _Canvas: https://canvas.ucdavis.edu/courses/146759

Overview
--------

The purpose of this assignment is dualfold.
First, to expose you to gem5, which we will use more heavily later in the semester.
Second, to give you experience measuring performance on different systems and comparing and contrasting those systems.

For this assignment, you will go through the first few parts of the gem5_ tutorial by yourself.
gem5 is a system simulator.
It is a modular platform for computer-system architecture research, encompassing system-level architecture as well as processor micro-architecture.

I wrote the tutorial_ that you will be going through.
If you have any feedback about errors, big or small, please let me know!
`Email me`_ with the subject "gem5-tutorial comments" with any comments or errors you find.

.. _email me: mailto:jlowepower@ucdavis.edu
.. _gem5: http://gem5.org/Main_Page
.. _tutorial: http://learning.gem5.org/book/

Instructions
------------

This assignment must be done alone.

Step 1: gem5 Tutorial, Part I
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Go through the Introduction and Part I of the `gem5 tutorial`__.
You will go through the following topics:

 - Downloading and building gem5.
 - Creating a simple configuration script.
 - Running gem5 for the first time.
 - Adding a cache to your original configuration script.
 - Understanding the statistics and output of gem5.
 - Looking at the default configuration scripts.


There are YouTube videos of me giving lectures on different parts of the tutorial.
The video for `Part I`_ will be helpful to watch.
You can find all of the videos on `my channel`_.

__ `tutorial`_
.. _`my channel`: https://www.youtube.com/channel/UCBDXDuN_5XcmntoE-dnQPbQ
.. _`Part I`: https://www.youtube.com/watch?v=5UT41VsGTsg

Platforms
"""""""""

gem5 runs on Linux and OS X.
There is limited support for Windows running the `Windows Subsystem for Linux`_.
If you use Windows, consider running a distribution of Linux in a VM or dual-booting.
(This will be a good idea for the rest of your time here as a student at Davis, as well.)

.. _`Windows Subsystem for Linux`: https://msdn.microsoft.com/commandline/wsl/about

I am currently looking into having gem5 run on the CSIF_ machines.
At the moment, gem5 will not run on them.
I will let everyone know if we can or cannot get gem5 to work in the CSIF through an announcement.

.. _CSIF: http://csifdocs.cs.ucdavis.edu/

.. _compilation instructions:

Compilation Instructions
""""""""""""""""""""""""

**Note: there is one modification to the tutorial that you will need to make.**
When you are compiling, you will need to pass the following option to SCons:

::

    CPU_MODELS=AtomicSimpleCPU,TimingSimpleCPU,O3CPU,MinorCPU

The entire compilation command should look like this:

::

    scons build/X86/gem5.opt -jX \
    CPU_MODELS=AtomicSimpleCPU,TimingSimpleCPU,O3CPU,MinorCPU

where ``X`` in ``-jX`` is the number of cores in your system, plus one.

As mentioned in the tutorial, it takes a while to compile gem5.
You should download and start the compilation process before doing anything else.


Step 2: Sieve of Eratosthenes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Write a simple C++ program to implement the `Sieve of Eratosthenes`_.
The sieve is an algorithm to calculate all prime numbers up to a certain limit.
If you are unfamiliar, have a look at the `linked Wikipedia page`__.

For this sieve, we want an output of one single integer at the end: the number of prime numbers <= the input argument, taken from the command line.
Compile your program as a static binary.
For the number of prime numbers <= 100 000 000, the output should be 5761455.

.. _Sieve of Eratosthenes: https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
__ `Sieve of Eratosthenes`_

Step 3: Use gem5
~~~~~~~~~~~~~~~~

Now, you will run your application in gem5 with the configuration script you made in the tutorial.
You will change the CPU model, frequency, and memory configuration while testing your sieve program.

1. Run gem5 with Your Sieve
"""""""""""""""""""""""""""

Run your sieve program in gem5 instead of the 'hello' example.
You will need to change the location in your configuration script to where your static binary is located.
*Do not use se.py or any other configuration script.
Use the one you made in the tutorial.*
Save the statistics file generated from this run, and for every run after this one.

**Choose an appropriate input size.**
You should use something large enough that the application is interesting, but not too large that gem5 takes more than 10 minutes to execute a simulation.
I found that somewhere around 1 000 000 takes about 5 minutes, which is a good compromise.

Note: the ``MinorCPU`` model (the next step) takes about 10x longer than ``TimingSimpleCPU`` takes.

2. Vary the CPU Model
"""""""""""""""""""""

Change the CPU model from ``TimingSimpleCPU`` to ``MinorCPU``.

Hint: you may want to add a command line parameter to control the CPU model.

3. Vary the CPU Frequency
"""""""""""""""""""""""""

Change the CPU clock from 1 GHz to 2 and 4 GHz with both CPU models.

Hint: you may want to add a command line parameter to change the frequency.

4. Vary the Memory Configuration
""""""""""""""""""""""""""""""""

Change the memory configuration from ``DDR3_1600_8x8`` to:

 - ``DDR3_2133_8x8``, which models DDR3 with a faster clock.
 - ``LPDDR2_S4_1066_1x32``, which models LPDDR2, low-power DRAM often found in mobile devices.
 - ``HBM_1000_4H_1x64``, which models High Bandwidth Memory, used in GPUs and network devices.

Use both CPU models.
Leave the frequency fixed at 4 GHz.

Hint: you may want to add a command line parameter to control the memory configuration.

5. Check Your Data
""""""""""""""""""

You should have nine statistic files for the following runs:

=============== =============== =======================
CPU Model       Frequency (GHz) Memory
=============== =============== =======================
TimingSimpleCPU 1               ``DDR3_1600_8x8``
TimingSimpleCPU 2               ``DDR3_1600_8x8``
TimingSimpleCPU 4               ``DDR3_1600_8x8``
MinorCPU        1               ``DDR3_1600_8x8``
MinorCPU        2               ``DDR3_1600_8x8``
MinorCPU        4               ``DDR3_1600_8x8``
MinorCPU        4               ``DDR3_2133_8x8``
MinorCPU        4               ``LPDDR2_S4_1066_1x32``
MinorCPU        4               ``HBM_1000_4H_1x64``
=============== =============== =======================

Step 4: Report
~~~~~~~~~~~~~~

Finally, you will describe the changes in performance between your tests.

Include a PDF named ``report.pdf`` in your submission.
This file will contain your observations and conclusions from the experiment.
It should contain answers to the following questions:

#. What metric should you use to compare the performance between different system configurations? Why?
#. Which CPU model is more sensitive to changing the CPU frequency? Why do you think so?
#. Is the sieve application more sensitive to the CPU model or the CPU frequency? Why?
#. Which CPU model is more sensitive to the memory technology? Why?
#. Is the sieve application more sensitive to the CPU model, the memory technology, or CPU frequency? Why?
#. If you were to use a different application, do you think your conclusions would change? Why?

Extra Credit (not required)
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Complete the following for extra credit.
This involves some digging into gem5 on your own.
There is (currently) no simple tutorial to walk through to accomplish this, thus it's extra credit.

gem5 has support for annotating your binary with special "region of interest" (ROI) magic instructions.
See the folders ``gem5/util/m5`` and ``gem5/include/gem5`` in the gem5 repository for more information.
Annotate your binary with ROI instructions and re-run the comparison between the MinorCPU at 1 GHz and 2 GHz.

Add answers to the following questions to your report.

7. Do you see a different result than before? If so, why?
8. Which result is more "correct" (i.e., if someone asked you which system you should use, which methodology gives you a more reliable answer)?

.. _submission:

Submission
----------

Archive the following into a GZ or TGZ file:

 - Your sieve .cpp file.
 - Your final gem5 confguration script from the tutorial.
 - Your statistics files (stats.txt) from your runs of your sieve.

Submit your archive as well as the PDF of your report to Canvas.

Late assignments receive an automatic 25% reduction per day they are late.
Assignments will not be accepted for late submission four days after the due date.

Common Errors
-------------

Building gem5
~~~~~~~~~~~~~

See the `Building gem5`_ page of the tutorial if you are having trouble getting gem5 to build.

.. _Building gem5: http://learning.gem5.org/book/part1/building.html

NameError: name 'MinorCPU' is not defined
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    $ ./build/X86/gem5.opt ./configs/tutorial/simple.py
    gem5 Simulator System.  http://gem5.org
    ...
    NameError: name 'MinorCPU' is not defined

You did not compile gem5 with the flag mentioned in the `compilation instructions`_.
Recompile gem5 with the flag and try again.

Cannot change the sieve program limit in gem5
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If your sieve program needs a command line argument to run, then you need to pass the options through the ``process.cmd`` parameter.
You can add another element to the list assigned to ``process.cmd``.
This parameter is like ``argv[]`` in a normal C program.

You can also add a command line option to your script to pass options through to the simulated process.
