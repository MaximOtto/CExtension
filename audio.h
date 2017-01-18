

int readtobuf(FILE* fp, uint8_t* bits_buffer, int *samples, unsigned int framewidth, int buffersize)
{
	int i, j, shift, fread_return;

	fread_return = fread(bits_buffer, buffersize, 1, fp);

	if(1 != fread_return) {	
		printf("!!!!!!fread error!!!!!!!!!!!!! read %d elements\n", fread_return);		
		return -1;
	}

	for(i = 0; i < (buffersize / framewidth); ++i) {	
		samples[i] = 0;
		shift = 24;
		for(j = framewidth - 1; j >= 0; --j) {
		    samples[i] = samples[i] | (bits_buffer[i * framewidth + j] << shift);
			shift -= 8;
		}
    }
    return 0;
}

int uni_writesamples (FILE* outt, int *samples, uint8_t *bits, uint16_t bitsps, int buflength)
{
	int i, shift, j, bytespersample, length;
	bytespersample = bitsps / 8;
	length = buflength / bytespersample;

	for(i = 0; i < length; ++i) {
		shift = 24;
		for(j = bytespersample -1; j >= 0; --j) {
	        bits[i * bytespersample + j] = (samples[i] >> shift);
			shift -= 8;
		}
    }

	fwrite(bits, buflength, 1, outt);
	return 0;
}

double db_value(double sample)
{
	return 20 * log10(fabs(sample) / 0x7fffffff);
}

double cancellation(FILE* fileA, FILE* fileB, unsigned int samples_buf_length, unsigned int numchannels, unsigned int framewidth, unsigned int subchunk2Size) {

    uint8_t *bits_buffer;
    int32_t *samplesA, *samplesB;

	unsigned int buffersize, i, buffer_count = 0; 
	double diff, diff_max = -1.0;

	buffersize = samples_buf_length * framewidth * numchannels;

	samplesA = (int32_t*)malloc(samples_buf_length * sizeof(int32_t) * numchannels);
	if (samplesA == NULL) {
		printf("!!!malloc_samples!!!\n");
	}

	samplesB = (int32_t*)malloc(samples_buf_length * sizeof(int32_t) * numchannels);			
	if (samplesB == NULL) {
		printf("!!!malloc_samples!!!\n");
	}

	bits_buffer = (unsigned char*)malloc(buffersize);
	if (bits_buffer == NULL) {
		printf("!!!malloc_bits!!!\n");
	}
	
    while(0 != subchunk2Size - buffer_count) {
    	if ((0 != readtobuf(fileA, bits_buffer, samplesA, framewidth, buffersize)) || (0 != readtobuf(fileB, bits_buffer, samplesB, framewidth, buffersize))) {
    		printf("CANCELLATION READ ERROR");
    		break;
    	}

		for(i=0; i < buffersize / framewidth; ++i) {
			diff = fabs((float)samplesA[i] - samplesB[i]);
			if (diff > diff_max) {
				diff_max = diff;
			}
		}
		
		buffer_count += buffersize;
		if(buffersize > subchunk2Size - buffer_count)
			buffersize = subchunk2Size - buffer_count;
    }

	return diff_max;
}

