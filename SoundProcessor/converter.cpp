#include "converter.h"
#include "wavobj.h"
#include <iostream>
#include "exceptions.h"

WavFile* Converter::stream = nullptr;

void MuteConverter::change_stream()
{
	if (param1 > param2)
	{
		throw SyntaxException("Not correct range in mute converter\n");
	}

	if (param2 > stream->get_duration())
	{
		throw SyntaxException("Not correct range in mute converter\n");
	}

	std::vector<char>& str = stream->get_stream();
	size_t start = stream->get_pos(param1);
	size_t end = stream->get_pos(param2);

	if (str.size() < end)
	{
		throw FileFormatException("Something wrong with stream\n");
	}

	for (size_t i = start; i < end; i++)
	{
		str[i] = 0;
	}
}

void MixConverter::change_stream()
{
	if (param1 > stream->get_duration())
	{
		throw SyntaxException("Not correct time point in mix converter\n");
	}

	if (param1 > input->get_duration())
	{
		throw SyntaxException("Not correct time point in mix converter\n");
	}

	size_t start = stream->get_pos(param1);
	std::vector<char>& str = stream->get_stream();
	std::vector<char>& inp = input->get_stream();

	for (size_t i = start; i < str.size() && i < inp.size(); i++)
	{
		str[i] = (str[i] + inp[i]) / 2;
	}

	input->free_stream();
}

void NoiseConverter::change_stream()
{
	if (param1 > param2)
	{
		throw SyntaxException("Not correct range in mute converter\n");
	}

	if (param2 > stream->get_duration())
	{
		throw SyntaxException("Not correct range in mute converter\n");
	}

	std::vector<char>& str = stream->get_stream();
	size_t start = stream->get_pos(param1);
	size_t end = stream->get_pos(param2);

	if (str.size() < end)
	{
		throw FileFormatException("Something wrong with stream\n");
	}

	for (size_t i = start; i < end; i++)
	{
		str[i] = str[i] + (rand()%100) - 50;
	}
}

Converter* MuteCreator::create_converter(uint32_t param1, uint32_t param2)
{
	auto tmp = new MuteConverter();
	tmp->param1 = param1;
	tmp->param2 = param2;
	return tmp;
}

Converter* MixCreator::create_converter(WavFile* wav_file, uint32_t param1)
{
	auto tmp = new MixConverter();
	tmp->input = wav_file;
	tmp->param1 = param1;
	return tmp;
}

Converter* NoiseCreator::create_converter(uint32_t param1, uint32_t param2)
{
	auto tmp = new NoiseConverter();
	tmp->param1 = param1;
	tmp->param2 = param2;
	return tmp;
}
