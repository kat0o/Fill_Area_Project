#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

struct pixel_array {
    struct pixel* pixels;
    int size;
};

int get_intensity(struct pixel_array array, int index) {
    int intensity = (array.pixels[index].R + array.pixels[index].G + array.pixels[index].B) / 3;
    return intensity;
}

void pixel_reset(struct pixel_array array, int target_intensity, int neighbor_index) {
    int neighbor_intensity = get_intensity(array, neighbor_index);

    if (abs(target_intensity - neighbor_intensity) == 1 || target_intensity == neighbor_intensity) {
        array.pixels[neighbor_index].R = 0;
        array.pixels[neighbor_index].G = 0;
        array.pixels[neighbor_index].B = 0;
    }
}

int main() {
    struct pixel_array array;
    int columns, rows;
    char file_name[100];
    printf("Enter name of file: ");
    scanf("%s", file_name);

    FILE* file_input = fopen(file_name, "r");
    if (file_input == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }
    if (fscanf(file_input, "%d %d", &columns, &rows) != 2)
    {
        printf("Failed to read columns and rows from file. \n");
        return 1;
    }

    array.size = columns*rows;

    array.pixels = (struct pixel*)malloc(array.size * sizeof(struct pixel));

    if (array.pixels == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }


    int r, g, b;

    for (int i = 0; i < array.size; i++) {
        if (fscanf(file_input, "%d:%d:%d", &r, &g, &b) != 3) {
            printf("Failed to read RGB values from the file.\n");
            break;
        }
        array.pixels[i].R = (char)r;
        array.pixels[i].G = (char)g;
        array.pixels[i].B = (char)b;
    }
    fclose(file_input);

    int row_fill, column_fill;
    printf("Enter target row and target column (target_row, target_column): ");
    scanf("%d, %d", &row_fill, &column_fill);

    int target_row = row_fill;
    int target_column = column_fill;
    int target;

    for (target = 0; target < array.size; target++)
    {
        int current_row = target / columns;
        int current_column = target % columns;
        if (current_row == target_row && current_column == target_column)break;
    }

    printf("Target Pixel: R = %d, G = %d, B = %d\n", array.pixels[target].R, array.pixels[target].G,array.pixels[target].B);
    int target_intensity = (array.pixels[target].R + array.pixels[target].G + array.pixels[target].B)/3;
    array.pixels[target].R = 0;
    array.pixels[target].G = 0;
    array.pixels[target].B = 0;

    if (target_row > 0) {
        int neighbor_index = target - columns;
        pixel_reset(array, target_intensity, neighbor_index);
    }

    if (target_row < rows - 1) {
        int neighbor_index = target + columns;
        pixel_reset(array, target_intensity, neighbor_index);
    }

    if (target_column > 0) {
        int neighbor_index = target - 1;
        pixel_reset(array, target_intensity, neighbor_index);
    }

    if (target_column < columns - 1) {
        int neighbor_index = target + 1;
        pixel_reset(array, target_intensity, neighbor_index);
    }

    int count_line = 0;
    printf("\nOutput RGB values:\n");
    for (int i = 0; i < array.size; i++) {
        printf("%d:%d:%d ", array.pixels[i].R, array.pixels[i].G, array.pixels[i].B);
        if (i < array.size - 1) {
            count_line++;
            if(count_line%columns == 0)printf("\n");
        }
    }


    FILE* file_output = fopen("output.txt", "w");
    if (file_output == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }
    fprintf(file_output, "%d %d\n", columns, rows);

    count_line = 0;
    for (int i = 0; i < array.size; i++) {
        fprintf(file_output, "%d:%d:%d ", array.pixels[i].R, array.pixels[i].G, array.pixels[i].B);
        if (i < array.size - 1) {
            count_line++;
            if(count_line%columns == 0)fprintf(file_output, "\n");
        }
    }

    fclose(file_output);

    free(array.pixels);

    return 0;
}
