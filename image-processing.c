#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// pgm.h
#ifndef PGM_H
#define PGM_H

typedef struct {
    char version[3];
    int width;
    int height;
    int maxGrayLevel;
    int **imageData;
    int **gx;
    int **gy;
} pgm;

#endif


// Function to read a PGM file

void read_pgm_file(char* dir, pgm* image) {
   
	FILE* input_image; 
	int i, j, num;
     // Open the file in binary mode 
	input_image = fopen(dir, "rb");

     // Check if the file is opened successfully
	if (input_image == NULL) {
		printf("File could not opened!");
		return;
	} 
	
    // Read the version of the image
	fgets(image->version, sizeof(image->version), input_image);
	read_comments(input_image);

    // Read the width and height of the image
	fscanf(input_image, "%d %d %d", &image->width, &image->height, &image->maxGrayLevel);
	
    // Allocate memory for the image data
	image->imageData = (int**) calloc(image->height, sizeof(int*));
	for(i = 0; i < image->height; i++) {
		image->imageData[i] = calloc(image->width, sizeof(int));

	}
	
    // Read the image data
	if (strcmp(image->version, "P2") == 0) {
		for (i = 0; i < image->height; i++) {
			for (j = 0; j < image->width; j++) {
				fscanf(input_image, "%d", &num);
				image->imageData[i][j] = num;
			}
		}	
	}
	else if (strcmp(image->version, "P5") == 0) {
		char *buffer;
		int buffer_size = image->height * image->width;
		buffer = (char*) malloc( ( buffer_size + 1) * sizeof(char));
		
		if(buffer == NULL) {
			printf("Can not allocate memory for buffer! \n");
			return;
		}
		fread(buffer, sizeof(char), image->width * image-> height, input_image);
		for (i = 0; i < image->height * image ->width; i++) {
			image->imageData[i / (image->width)][i % image->width] = buffer[i];
		}
		free(buffer);
	}
	fclose(input_image);
	printf("_______________IMAGE INFO__________________\n");
	printf("Version: %s \nWidth: %d \nHeight: %d \nMaximum Gray Level: %d \n", image->version, image->width, image->height, image->maxGrayLevel);
}


int main(int argc, char **argv)
{
    pgm image, out_image;
    char dir[200];
    
    // Prompt user for input file name
    printf("Enter the file name: ");
    scanf("%s", dir);
    
    // Read the PGM file
    read_pgm_file(dir, &image);
    
    // Apply image processing functions
    padding(&image);
    init_out_image(&out_image, image);
    sobel_edge_detector(&image, &out_image);    
    
    // Normalize and save gradient images
    min_max_normalization(&out_image, out_image.imageData);
    min_max_normalization(&out_image, out_image.gx);
    min_max_normalization(&out_image, out_image.gy);

    // Write results to files
    write_pgm_file(&out_image, dir, out_image.imageData, ".G.pgm");
    printf("Gradient saved: %s.G.pgm\n", dir);
    
    write_pgm_file(&out_image, dir, out_image.gx, ".GX.pgm");
    printf("Gradient X saved: %s.GX.pgm\n", dir);
    
    write_pgm_file(&out_image, dir, out_image.gy, ".GY.pgm");
    printf("Gradient Y saved: %s.GY.pgm\n", dir);

    // Free allocated memory
    free(image.imageData);
    free(out_image.imageData);
    free(out_image.gx);
    free(out_image.gy);
    
    return 0;
}