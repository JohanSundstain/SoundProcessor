#ifndef CONVERTER_H
#define CONVERTER_H

#include <iostream>
#include "wavobj.h"
#include <vector>

class Converter
{
public:
	static WavFile* stream;
	static void set_stream(WavFile* stream)
	{
		if (Converter::stream == nullptr)
		{
			Converter::stream = stream;
		}
	}
	uint32_t param1, param2;
	WavFile* input;
	Converter() = default;
	~Converter() = default;
	virtual void change_stream() = 0;
};


class MuteConverter : public Converter
{
public:
	void change_stream() override;

};

class MixConverter : public Converter
{
public:
	virtual void change_stream() override;
};


class NoiseConverter : public Converter
{
public:
	void change_stream() override;
};

class ConvertCreator
{
public:
	ConvertCreator() = default;
	~ConvertCreator() = default;
	virtual Converter* create_converter(uint32_t, uint32_t) = 0;
	virtual Converter* create_converter(WavFile*, uint32_t) = 0;
};


class PartImplMute : public ConvertCreator
{
public:
	Converter* create_converter(WavFile*, uint32_t) override { return nullptr; };
};

class PartImplMix : public ConvertCreator
{
public:
	Converter* create_converter(uint32_t, uint32_t) override { return nullptr; };
};

class PartImplNoise : public ConvertCreator
{
public:
	Converter* create_converter(WavFile*, uint32_t) override { return nullptr; };
};


class MuteCreator : public PartImplMute
{
public:
	Converter* create_converter(uint32_t, uint32_t) override;
};


class MixCreator : public PartImplMix
{
public:
	Converter* create_converter(WavFile*, uint32_t) override;
};

class NoiseCreator : public PartImplNoise
{
public:
	Converter* create_converter(uint32_t, uint32_t) override;
};

#endif 