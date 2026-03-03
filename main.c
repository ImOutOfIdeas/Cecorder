#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WAVE_MAX  32767
#define WAVE_MIN -32768

#define WAVE_FORMAT_PCM	1	// PCM Format Specifier
#define CHANNELS 	1	// Mono
#define SAMPLE_RATE 	44100	// Hz 
#define BIT_DEPTH 	16 	// Bit Depth 
				
#define SAMPLE_SIZE ((BIT_DEPTH + 7) / 8) // sample_size in bytes 

typedef struct {
	char 	 riff_id[4];  		// "RIFF"
	uint32_t riff_size;   		// 4 + 24 + (8 + sample_size(bytes) * samples_per_block * number_blocks)
	char 	 wave_id[4]; 		// "WAVE"
	char  	 format_id[4]; 		// "fmt "
	uint32_t format_size; 		// 16
	uint16_t format_tag;  		// WAVE_FORMAT_PCM (1)
	uint16_t channels;      	// samples_per_block
	uint32_t samples_per_second; 	// Sampling rate (44100 Hz)
	uint32_t bytes_per_second; 	// samples_per_second * sample_size(bytes) * samples_per_block
	uint16_t block_align; 		// sample_size(bytes) * samples_per_block
	uint16_t bits_per_sample;    	// 8 * sample_size(bytes)
	char	 data_id[4]; 		// "data"
	uint32_t data_size; 		// sample_size(bytes) * samples_per_block * number_block
} WaveHeader;

void wave_header_init(WaveHeader *wave);

int wave_header_write(FILE* fptr, WaveHeader *wave);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: WRONG\n");
		return 1;
	}
	
	FILE *fptr = fopen(argv[1], "wb");
	if (!fptr) {
		perror("Error opening file");
		return 1;
	}

	WaveHeader wave;

	wave_header_init(&wave);

	// Write placeholder header
	wave_header_write(fptr, &wave);

	srand((int) time(NULL));

	size_t blocks_written = 0;
	for (size_t i = 0; i < 100000; i++) {
		// Generate random 16 bit sample and scale to wave file amplitude range
		int16_t random_number = (rand() % (WAVE_MAX - WAVE_MIN + 1)) + WAVE_MIN;

		// TODO: Add error handling on fwrite
		blocks_written += fwrite(&random_number, sizeof(int16_t), 1, fptr);
	}
	
	uint32_t total_data_written = SAMPLE_SIZE * CHANNELS * blocks_written;

	if (total_data_written % 2 != 0) {
		uint8_t pad = 0;
		fwrite(&pad, sizeof(uint8_t), 1, fptr);
		total_data_written++;
	}

	// Update size for total data written
	wave.riff_size += total_data_written;
	wave.data_size = total_data_written;

	// Rewrite header to update size values
	fseek(fptr, 0, SEEK_SET);
	wave_header_write(fptr, &wave);

	if (fclose(fptr)) {
		perror("Error closing file");
		return 1;
	}

	return 0;
}

void wave_header_init(WaveHeader *wave) {
	(void) memcpy(wave->riff_id, "RIFF", 4 * sizeof(char));

	// The magic number 36 is the RIFF chunk size (in bytes) without the sound data
	// After the data is written this size will be updated
	wave->riff_size = 36;
	(void) memcpy(wave->wave_id, "WAVE", 4 * sizeof(char));

	(void) memcpy(wave->format_id, "fmt ", 4 * sizeof(char));
	wave->format_size = 16;
	wave->format_tag = WAVE_FORMAT_PCM;
	wave->channels = CHANNELS;
	wave->samples_per_second = SAMPLE_RATE;
	wave->bytes_per_second = SAMPLE_RATE * SAMPLE_SIZE * CHANNELS;
	wave->block_align = SAMPLE_SIZE * CHANNELS;
	wave->bits_per_sample = BIT_DEPTH;

	(void) memcpy(wave->data_id, "data", 4 * sizeof(char));
	wave->data_size = 0;
}

int wave_header_write(FILE* fptr, WaveHeader *wave) {
  	// TODO: Add error handling for each write 
    	fwrite(wave->riff_id, sizeof(char), 4, fptr);
    	fwrite(&wave->riff_size, sizeof(uint32_t), 1, fptr);
    	fwrite(wave->wave_id, sizeof(char), 4, fptr);

    	fwrite(wave->format_id, sizeof(char), 4, fptr);
    	fwrite(&wave->format_size, sizeof(uint32_t), 1, fptr);
    	fwrite(&wave->format_tag, sizeof(uint16_t), 1, fptr);
    	fwrite(&wave->channels, sizeof(uint16_t), 1, fptr);
    	fwrite(&wave->samples_per_second, sizeof(uint32_t), 1, fptr);
    	fwrite(&wave->bytes_per_second, sizeof(uint32_t), 1, fptr);
    	fwrite(&wave->block_align, sizeof(uint16_t), 1, fptr);
    	fwrite(&wave->bits_per_sample, sizeof(uint16_t), 1, fptr);

    	fwrite(wave->data_id, sizeof(char), 4, fptr);
    	fwrite(&wave->data_size, sizeof(uint32_t), 1, fptr);

    	return 0;
}
