# image-processing
Image Processing using Sobel Edge Detection Method

### Methodology
The provided code follows a methodology for reading an image into a matrix,
dividing it into patches, and determining important patches based on their
contents. The readImage function reads the image file, storing pixel values in a
dynamically allocated matrix. The processImage function calculates patch
scores by summing pixel values and identifies the patch with the highest score.
The main function drives the program, calling these functions and freeing
memory. The concept involves representing the image as a matrix, segmenting it
into patches, and evaluating patch importance based on scoring criteria. This
code exemplifies the process through the use of dynamic memory allocation
and nested loops for image processing. Implemented the Sobel edge detection
algorithm to find the edges in the image. This involves calculating the gradient
magnitude for each pixel using the Sobel operator



#### Input:
![261878299-03428aab-2c2a-4676-8d84-3fdfff88decb](https://github.com/user-attachments/assets/eb967f9b-3632-46e1-8f44-df5dc5c0aa29)


#### Output:

![261878283-2277f790-136d-4d88-bd3d-9139b09c3fb1](https://github.com/user-attachments/assets/2dda503c-9a0e-41ad-8b20-2953240cfdfb)
