#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define BUFFER_SIZE 4096
#define MAX_NAMES 128

#define BASE64_CENCODE_H

typedef enum
{
	step_A, step_B, step_C
} base64_encodestep;

typedef struct
{
	base64_encodestep step;
	char result;
	int stepcount;
} base64_encodestate;


/*
cencoder.c - c source to a base64 encoding algorithm implementation

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/


const int CHARS_PER_LINE = 72;

void base64_init_encodestate(base64_encodestate* state_in)
{
	state_in->step = step_A;
	state_in->result = 0;
	state_in->stepcount = 0;
}

char base64_encode_value(char value_in)
{
	static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	if (value_in > 63) return '=';
	return encoding[(int)value_in];
}

int base64_encode_block(const char* plaintext_in, int length_in, char* code_out, base64_encodestate* state_in)
{
	const char* plainchar = plaintext_in;
	const char* const plaintextend = plaintext_in + length_in;
	char* codechar = code_out;
	char result;
	char fragment;
	
	result = state_in->result;
	
	switch (state_in->step)
	{
		while (1)
		{
	case step_A:
			if (plainchar == plaintextend)
			{
				state_in->result = result;
				state_in->step = step_A;
				return codechar - code_out;
			}
			fragment = *plainchar++;
			result = (fragment & 0x0fc) >> 2;
			*codechar++ = base64_encode_value(result);
			result = (fragment & 0x003) << 4;
	case step_B:
			if (plainchar == plaintextend)
			{
				state_in->result = result;
				state_in->step = step_B;
				return codechar - code_out;
			}
			fragment = *plainchar++;
			result |= (fragment & 0x0f0) >> 4;
			*codechar++ = base64_encode_value(result);
			result = (fragment & 0x00f) << 2;
	case step_C:
			if (plainchar == plaintextend)
			{
				state_in->result = result;
				state_in->step = step_C;
				return codechar - code_out;
			}
			fragment = *plainchar++;
			result |= (fragment & 0x0c0) >> 6;
			*codechar++ = base64_encode_value(result);
			result  = (fragment & 0x03f) >> 0;
			*codechar++ = base64_encode_value(result);
			
			++(state_in->stepcount);
			if (state_in->stepcount == CHARS_PER_LINE/4)
			{
				*codechar++ = '\n';
				state_in->stepcount = 0;
			}
		}
	}
	/* control should not reach here */
	return codechar - code_out;
}

int base64_encode_blockend(char* code_out, base64_encodestate* state_in)
{
	char* codechar = code_out;
	
	switch (state_in->step)
	{
	case step_B:
		*codechar++ = base64_encode_value(state_in->result);
		*codechar++ = '=';
		*codechar++ = '=';
		break;
	case step_C:
		*codechar++ = base64_encode_value(state_in->result);
		*codechar++ = '=';
		break;
	case step_A:
		break;
	}
	*codechar++ = '\n';
	
	return codechar - code_out;
}

char* encode(const char* input)
{
	/* set up a destination buffer large enough to hold the encoded data */
	char* output = (char*)malloc(BUFFER_SIZE);
	/* keep track of our encoded position */
	char* c = output;
	/* store the number of bytes encoded by a single call */
	int cnt = 0;
	/* we need an encoder state */
	base64_encodestate s;
	
	/*---------- START ENCODING ----------*/
	/* initialise the encoder state */
	base64_init_encodestate(&s);
	/* gather data from the input and send it to the output */
	cnt = base64_encode_block(input, strlen(input), c, &s);
	c += cnt;
	/* since we have encoded the entire input string, we know that 
	   there is no more input data; finalise the encoding */
	cnt = base64_encode_blockend(c, &s);
	c += cnt;
	/*---------- STOP ENCODING  ----------*/
	
	/* we want to print the encoded data, so null-terminate it: */
	*c = 0;
	
	return output;
}

void shuffle(int *array, int size, unsigned int seed) {
    srand(seed);
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];
    char *names[MAX_NAMES];
    int indices[MAX_NAMES];
    int name_count = 0;
    unsigned int seed;
    bool seed_provided = false;
    bool encode_partner = true;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            seed = (unsigned int)strtoul(argv[i + 1], NULL, 10);
            seed_provided = true;
        }
        if (strcmp(argv[i], "-d") == 0) {
            encode_partner = false;
        }
    }

    if (!seed_provided) {
        seed = (unsigned int)time(NULL);
    }

    printf("Seed: %u\n", seed);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        perror("Error reading input");
        return 1;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    char *token = strtok(buffer, " ");
    while (token != NULL) {
        if (name_count >= MAX_NAMES) {
            fprintf(stderr, "Too many names (max %d)\n", MAX_NAMES);
            return 1;
        }
        names[name_count] = strdup(token);
        indices[name_count] = name_count;
        name_count++;
        token = strtok(NULL, " ");
    }

    if (name_count < 2) {
        fprintf(stderr, "Need at least two names for Secret Santa\n");
        return 1;
    }

    shuffle(indices, name_count, seed);

    int offset = seed;
    if (offset % name_count == 0) {
        offset = 1;
    } 
    for (int i = 0; i < name_count; i++) {
        int giver = indices[i];
        int receiver = indices[(i + offset) % name_count];
        if (encode_partner) {
            printf("%s: %s\n", names[giver], encode(names[receiver]));
        } else {
            printf("%s: %s\n", names[giver], names[receiver]);
        }
    }

    for (int i = 0; i < name_count; i++) {
        free(names[i]);
    }

    return 0;
}
