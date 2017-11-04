:Author: Jason Lowe-Power
:Editor: Justin Perona

=====================
ECS 201A - Homework 1
=====================

Due: Monday, 9 October 2017 at 3:10 PM
--------------------------------------

Submission of your code and your report will be done through Canvas_.
See the submission_ section below for more information.

*Please submit your report as a separate PDF, and not in the archive!*
Not only does this make things easier for me to grade, but I can give you feedback directly onto your report.

.. _Canvas: https://canvas.ucdavis.edu/courses/146759

Overview
--------

The purpose of this assignment is dual-fold.
First, it will expose you to gem5_, which we will use more heavily later in the quarter.
Second, it will give you experience measuring performance on different systems, and comparing and contrasting those systems.

For this assignment, you will go through the first few parts of the `gem5 book`_ by yourself.
gem5 is a system simulator.
It is a modular platform for computer-system architecture research, encompassing system-level architecture as well as processor micro-architecture.

I wrote the book__ that you will be going through.
If you have any feedback about errors, big or small, please let me know!
`Email me`_ with the subject "gem5-book comments" if you find any errors or have any comments.

.. _gem5: http://gem5.org/Main_Page
.. _gem5 book: http://learning.gem5.org/book/
__ `gem5 book`_
.. _email me: mailto:jlowepower@ucdavis.edu

Assignment
----------

This assignment must be done alone.

Step 1: Compile gem5
~~~~~~~~~~~~~~~~~~~~

Go through the Introduction_ and `Building gem5`_ pages of the `gem5 book`_.
Make sure to get your gem5 install working before moving on.

.. _Introduction: http://learning.gem5.org/book/intro.html
.. _Building gem5: http://learning.gem5.org/book/part1/building.html

gem5 on Linux
"""""""""""""

gem5 will work the best on Linux.
The `Building gem5`_ page lists the dependency names on Ubuntu.
If you are using a different distribution, you should be able to find the corresponding packages through your package manager, if the names aren't the same.

gem5 on OS X
""""""""""""

gem5 will run on OS X.
However, in years past, we have had a lot of difficulty with certain tools used in the compilation process.
In particular, we have had trouble with Python versions, ``SCons``, the tool used to compile gem5, and ``LLVM``, which Xcode uses as its back-end for ``gcc``.
See the `Common Errors`_ section for more information.

gem5 on Windows
"""""""""""""""

There is limited support for Windows running the `Windows Subsystem for Linux`_.
If you use Windows, consider running a distribution of Linux in a VM or dual-booting.
This will be a good idea for the rest of your time here as a student at Davis, as well.

.. _`Windows Subsystem for Linux`: https://msdn.microsoft.com/commandline/wsl/about

gem5 on the CSIF
""""""""""""""""

gem5 will run on the `CSIF machines`_.
Your regular CSIF home directory does not have enough space to store gem5.
After you login, you will need to switch directories to ``/home2/<username>``.
This directory has enough space for you to work in gem5.

The contents of ``/home2`` will be deleted shortly after the quarter ends.
Make sure to back up anything you want to keep.

.. _CSIF machines: http://csifdocs.cs.ucdavis.edu/

.. _compilation instructions:

Compilation Instructions
""""""""""""""""""""""""

**Note: there is one modification to the compilation command in the book that you will need to make.**
When you are compiling, you will need to pass the following option to SCons:

.. code-block:: sh

    CPU_MODELS=AtomicSimpleCPU,TimingSimpleCPU,O3CPU,MinorCPU

The entire compilation command should look like this:

.. code-block::sh

    scons build/X86/gem5.opt -jX \
    CPU_MODELS=AtomicSimpleCPU,TimingSimpleCPU,O3CPU,MinorCPU

where ``X`` in ``-jX`` is the number of cores in your system, plus one.

As mentioned in the book, it takes a while to compile gem5.
You should download and start the compilation process before doing anything else.

Step 2: gem5 Book, Part I
~~~~~~~~~~~~~~~~~~~~~~~~~

Go through the rest of Part I of the `gem5 book`_.
You will go through the following topics:

 - Creating a simple configuration script.
 - Running gem5 for the first time.
 - Adding a cache to your original configuration script.
 - Understanding the statistics and output of gem5.
 - Looking at the default configuration scripts.

There are YouTube videos of me giving lectures on different parts of the book.
The video for `Part I`_ will be helpful to watch.
You can find all of the videos on `my channel`_.

.. _`my channel`: https://www.youtube.com/channel/UCBDXDuN_5XcmntoE-dnQPbQ
.. _`Part I`: https://www.youtube.com/watch?v=5UT41VsGTsg

Step 3: Sieve of Eratosthenes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Write a simple C++ program to implement the `Sieve of Eratosthenes`_.
The sieve is an algorithm to calculate all prime numbers up to a certain limit.
If you are unfamiliar, have a look at the `linked Wikipedia page`__.

For this sieve, we want an output of one single integer at the end: the number of prime numbers <= the input argument, taken from the command line.
Compile your program as a static binary.
For the number of prime numbers <= 100 000 000, the output should be 5761455.

.. _Sieve of Eratosthenes: https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
__ `Sieve of Eratosthenes`_

Step 4: Use gem5
~~~~~~~~~~~~~~~~

Now, you will run your application in gem5 with the configuration script you made in the book.
You will change the CPU model, frequency, and memory configuration while testing your sieve program.

1. Run gem5 with Your Sieve
"""""""""""""""""""""""""""

Run your sieve program in gem5 instead of the 'hello' example.
You will need to change the location in your configuration script to where your static binary is located.
*Do not use se.py or any other configuration script.
Use the one you made in the book.*
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

You should have twelve statistic files for the following runs:

=============== =============== =======================
CPU Model       Frequency (GHz) Memory
=============== =============== =======================
TimingSimpleCPU 1               ``DDR3_1600_8x8``
TimingSimpleCPU 2               ``DDR3_1600_8x8``
TimingSimpleCPU 4               ``DDR3_1600_8x8``
MinorCPU        1               ``DDR3_1600_8x8``
MinorCPU        2               ``DDR3_1600_8x8``
MinorCPU        4               ``DDR3_1600_8x8``
TimingSimpleCPU 4               ``DDR3_2133_8x8``
TimingSimpleCPU 4               ``LPDDR2_S4_1066_1x32``
TimingSimpleCPU 4               ``HBM_1000_4H_1x64``
MinorCPU        4               ``DDR3_2133_8x8``
MinorCPU        4               ``LPDDR2_S4_1066_1x32``
MinorCPU        4               ``HBM_1000_4H_1x64``
=============== =============== =======================

Step 5: Report
~~~~~~~~~~~~~~

Finally, you will describe the changes in performance between your tests.

Include a PDF named ``report.pdf`` in your submission.
This file will contain your observations and conclusions from the experiment.
It should contain answers to the following questions:

#. What metric should you use to compare the performance between different system configurations? Why?
#. Which CPU model is more sensitive to changing the CPU frequency? Why?
#. Which CPU model is more sensitive to changing the memory technology? Why?
#. Is the sieve application more sensitive to the CPU model, the memory technology, or CPU frequency? Why?
#. If you were to use a different application, do you think your conclusions would change? Why?

Extra Credit (not required)
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Complete the following for extra credit.
This involves some digging into gem5 on your own.
There isn't (currently) a simple tutorial in the book to walk through how to accomplish this, thus it's extra credit.

gem5 has support for annotating your binary with special "region of interest" (ROI) magic instructions.
See the folders ``gem5/util/m5`` and ``gem5/include/gem5`` in the gem5 repository for more information.

Annotate your binary with ROI instructions and re-run the comparison between ``MinorCPU`` at 1 and 2 GHz.
To compile your annotated .cpp file, you need to make two changes to your ``gcc`` compilation command.

#. Add the gem5 includes folder into your search path. You can accomplish this by adding ``-I<your gem5 path>/include`` to your compilation command.
#. Add the x86 assembly file to the list of files for ``gcc`` to compile. You can accomplish this by adding ``<your gem5 path>/util/m5/m5op_x86.S`` to your compilation command.

If you use ROI annotations, you'll need to tell gem5 to exit simulation when encountering ROI annotations.
You can find this option in the System SimObject.
See ``gem5/src/sim/System.py`` for more information.
Then, you will need to modify your Python run script.
A second, possibly easier, option is to use the ``dump_reset_stats`` magic instruction in your .cpp file.

Add answers to the following questions to your report.

6. Do you see a different result than before? If so, why?
7. Which result is more "correct"? If someone asked you which system you should use, which methodology gives you a more reliable answer?

.. _submission:

Submission
----------

Archive the following into a .gz or .tgz file:

 - Your sieve .cpp file.
 - Your final gem5 configuration script from the book.
 - Your statistics files (``stats.txt``) from your runs of your sieve, appropriately named.

Submit your archive, as well as the PDF of your report, to Canvas.
*Do not include the PDF in the archive, submit it as a separate file.*

Late assignments receive an automatic 25% reduction per day they are late.
Assignments will not be accepted for late submission four days after the due date.

For your convenience, all the questions to be answered in the report are repeated below.

#. What metric should you use to compare the performance between different system configurations? Why?
#. Which CPU model is more sensitive to changing the CPU frequency? Why?
#. Which CPU model is more sensitive to changing the memory technology? Why?
#. Is the sieve application more sensitive to the CPU model, the memory technology, or CPU frequency? Why?
#. If you were to use a different application, do you think your conclusions would change? Why?

**Extra Credit**

6. Do you see a different result than before? If so, why?
7. Which result is more "correct"? If someone asked you which system you should use, which methodology gives you a more reliable answer?

Common Errors
-------------

Difficulty building gem5
~~~~~~~~~~~~~~~~~~~~~~~~

See the `Building gem5`_ page of the book if you are having trouble getting gem5 to build.

For OS X, if you are having issues with SCons, you will need to downgrade to SCons 2.5.1.
SCons 3.0.0, which is default that Homebrew installs, does not work.

Additionally, on OS X, you will need to ensure that you are using the correct Python version.
As mentioned in the book, use the following command to specify the correct Python version:

.. code-block:: sh

    python `which scons` build/X86/gem5.opt -jX \
    CPU_MODELS=AtomicSimpleCPU,TimingSimpleCPU,O3CPU,MinorCPU

NameError: name 'MinorCPU' is not defined
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sh

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
