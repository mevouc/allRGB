# allRGB

C++ implementation of an allRGB filter with OpenCV.

This program provides transformer and a checker.

## Transformer

The program targets an input image, checks that it has 2²⁴ colors, or scales it
to a picture with enough pixels. Once the given image has 2²⁴ colors, each color
is replaced with itself (if not already chosen), or the closest available.

The colors are stored in an octree, it allows a computation of the closest
availaible color within an O(log(N)) complexity.

### Example

![Kleinmond](http://i.imgur.com/MUVv3BM.jpg)
