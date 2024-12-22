#ifndef WAVOBJ_H
#define WAVOBJ_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <vector>

struct ChunkDescriptor
{
	char riff[4];
	uint32_t chunk_size;
	char wave[4];
};


struct FmtSubChunk
{
	char fmt[4];
	uint32_t sub_chunk_size;
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_per_second;
};

struct DataSubChunk
{
	char data[4];
	uint32_t sub_chunk_size;

};

struct WavHeader
{
	ChunkDescriptor ch_descriptor;
	FmtSubChunk fs_chunk;
	DataSubChunk ds_chunk;
};

class WavFile
{
private:
	std::ifstream file_in;
	std::ofstream file_out;

	const char* file_name;
	std::vector<char> stream;

	WavHeader wav_header;
	void parse();
	void fill_stream();
public:
	WavFile();
	WavFile(const char*);
	~WavFile();
	
	uint32_t get_duration();

	std::vector<char>& get_stream();

	size_t get_pos(size_t seconds);

	void save_in_file(const char* fn);

	void free_stream()
	{
		stream.clear();
	}
};


#endif // !WAVOBJ_H

