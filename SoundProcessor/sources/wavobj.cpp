#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <vector>
#include "wavobj.h"
#include "exceptions.h"

void WavFile::parse()
{
	this->file_in.open(file_name, std::ios::binary);
	if (!this->file_in.is_open())
	{
		throw FileFormatException("Can not open file for reading\n");
	}
	this->file_in.read(reinterpret_cast<char*>(&wav_header), sizeof(WavHeader));

	if (std::strncmp(wav_header.ch_descriptor.riff, "RIFF",4) != 0)
	{
		throw FileFormatException("Not correct file format\n");
	}

	if (std::strncmp(wav_header.fs_chunk.fmt, "fmt",3) != 0)
	{
		throw FileFormatException("Not correct file format\n");
	}

	if (std::strncmp(wav_header.ds_chunk.data, "data",4) != 0)
	{
		throw FileFormatException("Not correct file format\n");
	}
}

std::vector<char>& WavFile::get_stream()
{
	if (this->stream.empty())
	{
		fill_stream();
	}
	return this->stream;
}

size_t WavFile::get_pos(size_t seconds)
{
	return seconds * wav_header.fs_chunk.byte_rate;
}

void WavFile::fill_stream()
{
	this->file_in.seekg(sizeof(WavFile), std::ios::beg);
	char c;
	while (this->file_in.get(c))
	{
		this->stream.push_back(c);
	}
}

void WavFile::save_in_file(const char* fn)
{
	this->file_out.open(fn, std::ios::binary);
	if (!this->file_out.is_open())
	{
		throw FileFormatException("Can not open file for saving\n");
	}
	this->file_out.write(reinterpret_cast<char*>(&wav_header), sizeof(WavHeader));
	this->file_out.write(stream.data(), stream.size());
}

uint32_t WavFile::get_duration()
{
	return wav_header.ds_chunk.sub_chunk_size / wav_header.fs_chunk.byte_rate;
}

WavFile::WavFile() : wav_header{}, file_name(nullptr) {};

WavFile::WavFile(const char* file_name) 
{
	wav_header = {};
	this->file_name = file_name;
	parse();
}

WavFile::~WavFile()
{
	this->file_in.close();
	this->file_out.close();
}