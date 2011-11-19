================================================================================
Line Segment Detector
================================================================================


LSD is aimed at detecting segments in an image. The first step is to compute
the level-line field. All pixels having the same level-line angle are
clustered. Each cluster of point is then subjected to a validation procedure
called a contrario, ensuring that the regions are not likely to be obtained "by
change".

LSD performs really well on natural images, and quite well on most synthetised
images.


I thought the algorithm may have some problems dealing with colour gradients.
As expected, LSD detects a segment only on a very steep gradient (the
algorithm detects a segment only for the last image).

.. image:: images/degrades.png

.. image:: images/boxes_results.png

Considering problems with detecting gradients of colour, I imagined LSD would
perform less and less accurately on a blurred image. I tried detected segments
on an image more and more blurred.

.. image:: images/echiquiers.png
  :scale: 50 %

The algorithm works once again surprisingly well. When the image is blurred,
it detects less segments, but is still able to detect most of them.

Zooming on a 
.. image:: images/echiquier_zoom_results.png

