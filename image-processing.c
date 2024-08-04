#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Function to check if a character is a space
int isspace(int argument);


// Structure to store PGM image data
typedef struct {
	char version[3]; // Version of the PGM image
	int width; // Width of the image
	int height; // Height of the image
	int maxGrayLevel; // Maximum gray level of the image
	int **imageData;  // Image data
	int **gx; // Gradient in the X direction
	int **gy; // Gradient in the Y direction
} pgm;

// Function to initialize the output image

void init_out_image( pgm* out, pgm image){
	int i, j;
	strcpy(out->version, image.version); // Copy the version of the input image to the output image

     // Copy the width and height of the input image to the output image
	out->width = image.width;
	out->height = image.height;
    // Copy the maximum gray level of the input image to the output image
	out->maxGrayLevel = image.maxGrayLevel;
	
    // Allocate memory for the image data of the output image
	out->imageData = (int**) calloc(out->height, sizeof(int*));
	for(i = 0; i < out->height; i++) {
		out->imageData[i] = calloc(out->width, sizeof(int));
	}
	
     // Allocate memory for the gradient in the X direction of the output image
	out->gx = (int**) calloc(out->height, sizeof(int*));
	for(i = 0; i < out->height; i++) {
		out->gx[i] = calloc(out->width, sizeof(int));
	}
	
    // Allocate memory for the gradient in the Y direction of the output image
	out->gy = (int**) calloc(out->height, sizeof(int*));
	for(i = 0; i < out->height; i++) {
		out->gy[i] = calloc(out->width, sizeof(int));
	}
	
    // Copy the image data of the input image to the output image
	for(i = 0; i < out->height; i++) {
		for(j = 0; j < out->width; j++) {
			out->imageData[i][j] = image.imageData[i][j];
			out->gx[i][j] = image.imageData[i][j];
			out->gy[i][j] = image.imageData[i][j];
		};
	}
}

// Function to read comments from a PGM file

void read_comments(FILE *input_image) {
    // Read characters from the file until a non-space character is encountered
	char ch;
	char line[100];

	while ((ch = fgetc(input_image)) != EOF && (isspace(ch)))  {
		;
    }

     // If the character is a '#', then read the comment line
	if (ch == '#') {
        fgets(line, sizeof(line), input_image);
    } 
     // Otherwise, rewind the file pointer by two characters
	else {
		fseek(input_image, -2L, SEEK_CUR);
	}
}


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


// This function pads the image with zeros
void padding(pgm* image) {
	int i;
    // Pad the top and bottom of the image with zeros
	for (i = 0; i < image->width; i++) {
		image->imageData[0][i] = 0;
		image->imageData[image->height - 1][i] = 0;
	}
	 // Pad the left and right sides of the image with zeros
	for (i = 0; i < image->height; i++) {
		image->imageData[i][0] = 0;
		image->imageData[i][image->width - 1] = 0;
	} 
}

// This function performs a convolution operation on the image
int convolution(pgm* image, int kernel[3][3], int row, int col) {
	int i, j, sum = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			sum += image->imageData[i + row][j + col] * kernel[i][j];
		}
	}
	return sum;
}


// This function performs a Sobel edge detection on the image

void sobel_edge_detector(pgm* image, pgm* out_image) {
	int i, j, gx, gy;
	int mx[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};
	int my[3][3] = {
		{-1, -2, -1},
		{0, 0, 0},
		{1, 2, 1}
	};
	
	for (i = 1; i < image->height - 2; i++) {
		for (j = 1; j < image->width - 2; j++) {
			gx = convolution(image, mx, i, j);
			gy = convolution(image, my, i, j);
			out_image->imageData[i][j] = sqrt(gx*gx + gy*gy);
			out_image->gx[i][j] = gx;
			out_image->gy[i][j] = gy;
		}
	}
	
}

// This function performs min-max normalization on the image


void min_max_normalization(pgm* image, int** matrix) {
	int min = 1000000, max = 0, i, j;
	
	for(i = 0; i < image->height; i++) {
		for(j = 0; j < image->width; j++) {
			if (matrix[i][j] < min) {
				min = matrix[i][j];
			}
			else if (matrix[i][j] > max) {
				max = matrix[i][j];
			}
		}
	}
	
	for(i = 0; i < image->height; i++) {
		for(j = 0; j < image->width; j++) {
			double ratio = (double) (matrix[i][j] - min) / (max - min);
			matrix[i][j] = ratio * 255;
		}
	} 
}


// This function writes a PGM file

void write_pgm_file(pgm* image, char dir[], int** matrix, char name[]) {
	FILE* out_image;
	int i, j, count = 0;
	
	char* token = strtok(dir, ".");
	if (token != NULL) {
		strcat(token, name);
		out_image = fopen(token, "wb");
	}
	
	out_image = fopen(dir, "wb");
	fprintf(out_image, "%s\n", image->version);
	fprintf(out_image, "%d %d\n", image->width, image->height);
	fprintf(out_image, "%d\n", image->maxGrayLevel);
	
	if (strcmp(image->version, "P2") == 0) {
		for(i = 0; i < image->height; i++) {
			for(j = 0; j < image->width; j++) {
				fprintf(out_image,"%d", matrix[i][j]);
				if (count % 17 == 0) 
					fprintf(out_image,"\n");
				else 
					fprintf(out_image," ");
				count ++;
			}
		} 
	}
	else if (strcmp(image->version, "P5") == 0) {
		for(i = 0; i < image->height; i++) {
			for(j = 0; j < image->width; j++) {
				char num = matrix[i][j];
				fprintf(out_image,"%c", num);
			}
		} 
	} 
	fclose(out_image);
}


// This is the main function

int main(int argc, char **argv)
{
	pgm image, out_image;
	char dir[200];
	printf("Enter the file name: ");
	scanf("%s", dir);
	
	read_pgm_file(dir, &image);
	padding(&image);
	init_out_image(&out_image, image);
	sobel_edge_detector(&image, &out_image);	
	
	min_max_normalization(&out_image, out_image.imageData);
	min_max_normalization(&out_image, out_image.gx);
	min_max_normalization(&out_image, out_image.gy);

	write_pgm_file(&out_image, dir, out_image.imageData, ".G.pgm");
	printf("\nGradient saved: %s \n", dir);
	write_pgm_file(&out_image, dir, out_image.gx, ".GX.pgm");
	printf("Gradient X saved: %s \n", dir);
	write_pgm_file(&out_image, dir, out_image.gy, ".GY.pgm");
	printf("Gradient Y saved: %s \n", dir);

	free(image.imageData);
	free(out_image.imageData);
	free(out_image.gx);
	free(out_image.gy);
	
	return 0;
}