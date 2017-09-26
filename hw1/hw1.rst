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

Go through the introduction and part I of the `gem5 tutorial`__.
You will go through the following topics:

 - Downloading and building gem5.
 - Creating a simple configuration script.
 - Running gem5 for the first time.
 - Adding a cache to your original configuration script.
 - Understanding the statistics and output of gem5.
 - Looking at the default configuration scripts.

gem5 runs on Linux and Mac OS X.
There is no support for Windows.
If you use Windows, consider running a distribution of Linux in a VM or dual-booting.
(This will be a good idea for the rest of your time here as a student at Davis, as well.)

I am currently looking into having gem5 run on the CSIF_ machines.
At the moment, gem5 will not run on them.
I will let everyone know if we can or cannot get gem5 to work in the CSIF.

As mentioned in the tutorial, it takes a while to compile gem5.
You should download and start the compilation process before doing anything else.

__ `tutorial`_
.. _CSIF: http://csifdocs.cs.ucdavis.edu/

Step 2: Sieve of Eratosthenes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Write a simple C++ program to implement the `Sieve of Eratosthenes`_.
The sieve is an algorithm to calculate all prime numbers up to a certain limit.
If you are unfamiliar, have a look at the `linked Wikipedia page`__.

For this sieve, we want an output of one single integer at the end: the number of prime numbers <= the input argument, taken from the command line.
Compile your program as a static binary.
For the number of prime numbers <= 100 000 000, the output should be: 5761455.

.. _Sieve of Eratosthenes: https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
__ `Sieve of Eratosthenes`_

Step 3: Use gem5
~~~~~~~~~~~~~~~~

Now, you will run your application in gem5.
You will change the CPU model, frequency, and memory configuration while testing your sieve program.
Finally, you will describe the changes in performance between your tests.

1. Run your sieve program in gem5 instead of the 'hello' example.

**Choose an appropriate input size.**
You should use something large enough that the application is interesting, but not too large that gem5 takes more than 10 minutes to execute a simulation
I found that roughly 1 000 000 takes five minutes, and is a good compromise.
Note: the ``MinorCPU`` model (the next step) takes about 10x longer than ``TimingSimpleCPU`` takes.

2. Change the CPU model from ``TimingSimpleCPU`` to ``MinorCPU``.

Hint: you may want to add a command line parameter to control the CPU model.

3. Vary the CPU clock from 1 GHz to 3 GHz with both CPU models.

Hint: again, you may want to add a command line parameter for the frequency.

4. Change the memory configuration from ``DDR3_1600_x64`` to ``DDR3_2133_8x8`` and ``LPDDR2_S4_1066_1x32``, with both CPU models.

The first option models DDR3 with a faster clock.
The second option models LPDDR2, which is low-power DRAM often found in mobile devices.

Step 4: Report
~~~~~~~~~~~~~~

Include a PDF named ``report.pdf`` in your submission.
This file will contain your observations and conclusions from the experiment.
It should contain answers to the following questions:

#. Which CPU model is more sensitive to changing the CPU frequency? Why do you think so?
#. Is the sieve application more sensitive to the CPU model or the CPU frequency? Why?
#. Which CPU model is more sensitive to the memory technology? Why?
#. Is the sieve application more sensitive to the CPU model, the memory technology, or CPU frequency? Why?
#. If you were to use a different application, do you think your conclusions would change? Why?

.. _submission:

Submission
----------

Archive your sieve .cpp file and your gem5 configuration script into a .gz or .tgz.
Submit your archive as well as your report PDF to Canvas.

Late assignments receive an automatic 25% reduction per day they are late.
Assignments will not be accepted for late submission four days after the due date.
