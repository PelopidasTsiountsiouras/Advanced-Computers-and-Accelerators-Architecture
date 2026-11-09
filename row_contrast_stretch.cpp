#include <stdint.h>

#define IH 8
#define HEIGHT (1 << IH)
#define IW 8
#define WIDTH (1 << IW)

void row_contrast_stretch(uint8_t A[HEIGHT][WIDTH], uint8_t C[HEIGHT][WIDTH]) {
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
