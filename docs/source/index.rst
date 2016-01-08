.. blnuhr documentation master file

.. include:: ../../README.rst

How it shows the time
---------------------

The time is calculated by adding the lit rectangles. 
The top rectangle blinks changes every second. 
In the next row, the each rectangle represents 5 hours.
In the third row, every rectangle represents 1 hour.
Together, these two rows show the hour of the day.
The fourth row rectangles represents 5 minute intervals. 
(Red rectangles show 15 minute intervals.)
In the last row, every rectangle represents 1 minute.
Like the hours, these two rows show the minutes after the hour.

Example 14:28 (2:28 pm)
=======================

In this example, the time is 14:28.

.. compound::

    .. _fig._1428:

    .. figure:: images/14-28.png
        :alt: fig._1428

Here's the explanation:

:row 1: seconds are an even number, LED is off
:row 2: first two LEDs are on, at least 10 AM
:row 3: all LEDs are on, hours = 10 AM + 4 = 14:00
:row 4: first 5 LEDs are on, at least 25 after the hour
:row 5: first 3 LEDs are on, minutes = 25 + 3 = 14:28


Compare 16:57 (4:57 pm)
=======================

Compare with a view of the Berlin Quantity Didactics Clock in 2004
(after it was moved to the Europa center).  The time on the clock
shown is 16:57 (4:57 pm).

.. compound::

    .. _fig.blnuhr-orig2004:

    .. figure:: images/blnuhr-orig2004.jpg
        :alt: fig.blnuhr-orig2004


--------

*blnuhr* Package: Source Code Documentation
===========================================

Source code documentation for *blnuhr*.

:mod:`main` Module
------------------

.. automodule:: blnuhr.main
    :members:
    :undoc-members:
    :show-inheritance:

:mod:`resources` Module
-----------------------

.. automodule:: blnuhr.resources
    :members:
    :undoc-members:
    :show-inheritance:


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

