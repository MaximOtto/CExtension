struct wavheader
{
    int8_t chunkId[4];
    uint32_t chunkSize;
    int8_t format[4];
    int8_t subChunkId[4];
    uint32_t subchunkSize;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    int8_t subChunk2Id[4];
    uint32_t subchunk2Size;
};

int readheader(FILE* file, struct wavheader* header)
{
	int result = 0;

    fread(&header->chunkId,	 	4 * (sizeof(int8_t)), 1, file);
	fread(&header->chunkSize,		sizeof(uint32_t), 1, file);
	fread(&header->format, 		4 * (sizeof(int8_t)), 1, file);
	fread(&header->subChunkId, 	4 * (sizeof(int8_t)), 1, file);
	fread(&header->subchunkSize, 	sizeof(uint32_t), 1, file);
	fread(&header->audioFormat, 	sizeof(uint16_t), 1, file);
	fread(&header->numChannels, 	sizeof(uint16_t), 1, file);
	fread(&header->sampleRate, 		sizeof(uint32_t), 1, file);
	fread(&header->byteRate, 		sizeof(uint32_t), 1, file);
	fread(&header->blockAlign, 		sizeof(uint16_t), 1, file);
	fread(&header->bitsPerSample, 	sizeof(uint16_t), 1, file);
	fread(&header->subChunk2Id,	4 * (sizeof(int8_t)), 1, file);
	fread(&header->subchunk2Size, 	sizeof(uint32_t), 1, file);

    // printf("chunkId=%.4s\n",header->chunkId);
    // printf("chunkSize=%u\n",header->chunkSize);
    // printf("format=%.4s\n",header->format);
    // printf("subChunkId=%.4s\n",header->subChunkId);
    // printf("numChannels=%u\n",header->numChannels);
    // printf("SampleRate=%u\n",header->sampleRate);
    // printf("byteRate=%u\n",header->byteRate);
    // printf("blockAlign=%u\n",header->blockAlign);
    // printf("bitsPerSample=%u\n",header->bitsPerSample);
    // printf("subChunk2Id=%.4s\n",header->subChunk2Id);
    // printf("subchunk2Size=%u\n",header->subchunk2Size);

	if (strncmp(&header->format, "WAVE", 4) || strncmp(&header->chunkId, "RIFF", 4))
		result = 1;

	return result;
}



int compare_headers(struct wavheader* headerA, struct wavheader* headerB) {
	if(0 != strncmp(headerA->format, "WAVE", 4) || 0 != strncmp(headerB->format, "WAVE", 4)) {
		printf("NOT WAVE\n");
		exit(0);
	}

	if(0 != strncmp(headerA->chunkId, headerB->chunkId, 4)) {
		printf("formats don't match1\n");
		exit(0);
	}
	if(headerA->chunkSize != headerB->chunkSize) {
		printf("formats don't match2\n");
		exit(0);
	}
	if(0 != strcmp(headerA->format, headerB->format)) {
		printf("formats don't match3\n");
		exit(0);
	}
	if(0 != strcmp(headerA->subChunkId, headerB->subChunkId)) {
		printf("formats don't match4\n");
		exit(0);
	}
	if(headerA->subchunkSize != headerB->subchunkSize) {
		printf("formats don't match5\n");
		exit(0);
	}
	if(headerA->audioFormat != headerB->audioFormat) {
		printf("formats don't match6\n");
		exit(0);
	}
	if(headerA->numChannels != headerB->numChannels) {
		printf("formats don't match7\n");
		exit(0);
	}
	if(headerA->sampleRate != headerB->sampleRate) {
		printf("formats don't match8\n");
		exit(0);
	}
	if(headerA->byteRate != headerB->byteRate) {
		printf("formats don't match9\n");
		exit(0);
	}
	if(headerA->blockAlign != headerB->blockAlign) {
		printf("formats don't match10\n");
		exit(0);
	}
	if(headerA->bitsPerSample != headerB->bitsPerSample) {
		printf("formats don't match11\n");
		exit(0);
	}
	if(0 != strcmp(headerA->subChunk2Id, headerB->subChunk2Id)) {
		printf("formats don't match12\n");
		exit(0);
	}

	return 0;
}

int compare_headers_(struct wavheader* headerA, struct wavheader* headerB) {

	if(	
		strncmp(headerA->chunkId,		headerB->chunkId, 4)	||
		strncmp(headerA->format,		headerB->format, 4)		||
		strncmp(headerA->subChunkId,	headerB->subChunkId, 4)	||
		// headerA->chunkSize			!=	headerB->chunkSize		||
		headerA->subchunkSize		!=	headerB->subchunkSize	||
		headerA->audioFormat		!=	headerB->audioFormat	||
		headerA->numChannels		!=	headerB->numChannels	||
		headerA->sampleRate			!=	headerB->sampleRate		||
		headerA->byteRate			!=	headerB->byteRate		||
		headerA->blockAlign			!=	headerB->blockAlign		||
		headerA->bitsPerSample		!=	headerB->bitsPerSample	||
		strncmp(headerA->subChunk2Id,	headerB->subChunk2Id, 4)||
		headerA->subchunk2Size  	!=  headerB->subchunk2Size
		) 

	{

		return 1;
	}
	else 
		return 0;
}