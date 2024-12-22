#include <iostream>
#include <string>
#include "wavobj.h"
#include "converter.h"
#include "arg_parser.h"
#include "config_parser.h"
#include <sstream>
#include "exceptions.h"

const char* help_message[] = { "WAV converter\nUsage:\n\t[-h] help information\n\t[-c] config file\n\t<output file> <input file> [input file]" };

void run_program(ArgParser& parser)
{
	std::string output;
	std::string config;
	std::vector<WavFile*> inputs;
	WavFile* stream = nullptr;

	char c = parser.get_arg();
	while (c != '\0')
	{
		switch (c)
		{
		case '!':
		{
			std::vector < std::string >& param = parser.get_params(c);
			output = param[0];

			for (size_t i = 1; i < param.size(); i++)
			{
				if (i == 1) stream = new WavFile{ param[i].c_str() };

				inputs.push_back(new WavFile{param[i].c_str()});
			}
		}
		break;
		case 'h':
		{
			std::cout << *help_message << std::endl;
		}
		break;
		case 'c':
		{
			std::vector < std::string >& param = parser.get_params(c);
			config = param[0];
		}	
		break;
		default:
			break;
		}
	
		c = parser.get_arg();
	}

	if (stream == nullptr)
	{
		return;
	}

	Converter::set_stream(stream);

	ConfigParser conf_parser(config.c_str(), inputs);
	std::vector<Converter*>& converters = conf_parser.get_converters();
	
	for (auto c : converters)
	{
		c->change_stream();
	}

	stream->save_in_file(output.c_str());


	for (size_t i = 0; i < inputs.size(); i++)
	{
		delete inputs[i];
	}
}

int main(int argc, char** argv)
{
	try 
	{
		ArgParser parser{ argc-1, &argv[1], "h0c1"};
		run_program(parser);
	}
	catch (const FileFormatException& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const SyntaxException& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Undefined" << std::endl;
	}
	
	return 0;
}