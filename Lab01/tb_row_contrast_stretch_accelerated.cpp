# include <stdio.h>
#include <stdint.h>

# define IH 8
# define HEIGHT (1 << IH)
# define IW 8
# define WIDTH (1 << IW)
# define ROW_GRADIENT 42
# define COLUMN_GRADIENT 17

void row_contrast_stretch_accelerated(uint8_t A[HEIGHT][WIDTH], uint8_t C[HEIGHT][WIDTH]);

void software_reference(uint8_t A[HEIGHT][WIDTH], uint8_t C[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        uint8_t row_min = 255;
        uint8_t row_max = 0;

        for (int j = 0; j < WIDTH; j++){
        	if (A[i][j] > row_max){
        		row_max = A[i][j];
        	}
        	if (A[i][j] < row_min){
        		row_min = A[i][j];
        	}
        }

        uint32_t R = row_max - row_min;
        uint32_t S = 0;

        if (R > 0) {
            S = (255 << 16) / R;
        }

        if (R == 0) {
            for (int j = 0; j < WIDTH; j++) {
                C[i][j] = A[i][j];
            }
        } else {
            for (int j = 0; j < WIDTH; j++) {
                C[i][j] = ((A[i][j] - row_min) * S + (1 << 15)) >> 16;
            }
        }
    }
}


int main(){

	uint8_t A[HEIGHT][WIDTH], C_hw[HEIGHT][WIDTH], C_ref[HEIGHT][WIDTH];

	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j ++){
			if (i == 0){
				A[i][j] = 142;
				continue;
			}
			A[i][j] = ROW_GRADIENT + (COLUMN_GRADIENT * j) / WIDTH;
		}
	}

	row_contrast_stretch_accelerated(A, C_hw);
	software_reference(A, C_ref);

	int errors = 0;

	for (int i =0; i < HEIGHT; i++){
		for (int j = 0; j <WIDTH; j++){
			if (C_hw[i][j] != C_ref[i][j]){
				errors += 1;
			}
		}
	}

	if (errors == 0){
		printf("Test Passed!");
	}
	else{
		printf("Test Failed with %d errors!", errors);
	}

	return 0;
}
