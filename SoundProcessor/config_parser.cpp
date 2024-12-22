#include "config_parser.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <sstream>
#include "exceptions.h"


void ConfigParser::parse()
{
	std::string buffer;
	std::vector<std::string> line_params;
	MuteCreator muc;
	MixCreator mic;
	NoiseCreator noc;

	while (getline(this->config_file, buffer))
	{
		line_params.clear();
		ws_split(line_params, buffer);

		if (line_params[0][0] == '#')
		{
			continue;
		}

		if (line_params[0] == "mute")
		{
			if (line_params.size() != 3)
			{
				throw SyntaxException("Mute converter requires 2 arguments\n");
			}

			uint32_t param1 = std::stoul(line_params[1]);
			uint32_t param2 = std::stoul(line_params[2]);

			auto tmp = new MuteCreator();
			this->converters.push_back(muc.create_converter(param1, param2));
		}

		if (line_params[0] == "mix")
		{
			if (line_params.size() < 2)
			{
				throw SyntaxException("Mix converter requires 2 or 3 arguments\n");
			}

			if (line_params.size() > 3)
			{
				throw SyntaxException("Mix converter requires 2 or 3 arguments\n");
			}

			if (line_params[1][0] != '$')
			{
				throw SyntaxException("Not correct link expected $n\n");
			}

			uint32_t param1 = line_params.size() == 2 ? 0 : std::stoul(line_params[2]);
			uint32_t link = std::stoul(&line_params[1][1]);
			if (link - 1 > this->inputs.size())
			{
				throw SyntaxException("Not enough input files for link\n");
			}

			this->converters.push_back(mic.create_converter(this->inputs[link - 1], param1));
		}

		if (line_params[0] == "noise")
		{
			if (line_params.size() != 3)
			{
				throw SyntaxException("Noise converter requires 2 arguments\n");
			}
			uint32_t param1 = std::stoul(line_params[1]);
			uint32_t param2 = std::stoul(line_params[2]);

			auto tmp = new MuteCreator();
			this->converters.push_back(noc.create_converter(param1, param2));
		}

	}

	if (converters.size() == 0)
	{
		throw FileFormatException("Empty or not correct config file\n");
	}
}

void ConfigParser::ws_split(std::vector<std::string>& v, std::string s)
{
	std::stringstream ss;
	std::string buffer;

	ss << s;
	
	while (ss >> buffer)
	{
		v.push_back(buffer);
	}
}

ConfigParser::ConfigParser(const char* file_name, std::vector<WavFile*>& inputs)
{
	this->inputs = inputs;
	this->config_file.open(file_name);
	if (!this->config_file.is_open())
	{
		throw FileFormatException("Can not open config file\n");
	}
	parse();
}

ConfigParser::~ConfigParser()
{
	this->config_file.close();
	for (size_t i = 0; i < this->converters.size(); i++)
	{
		delete this->converters[i];
	}
}

std::vector<Converter*>& ConfigParser::get_converters()
{
	return converters;
}

