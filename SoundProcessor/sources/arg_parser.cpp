#include "arg_parser.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include "exceptions.h"

ArgParser::ArgParser(int argc, char* argv[], std::string args)
{
	this->args_string = args;
	parse(argc, argv);
	this->iter = this->args_params.begin();
}

void ArgParser::parse(int argc, char* argv[])
{
	for (size_t i = 0; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			auto it = std::find(this->args_string.begin(), this->args_string.end(), argv[i][1]);
			if (it == this->args_string.end())
			{
				throw SyntaxException("Not correct argument\n");
			}

			size_t place = it - this->args_string.begin();
			if (!isdigit(this->args_string[place + 1]))
			{
				throw SyntaxException("Not correct args format string: expected number\n");
			}

			size_t arg_num = std::stoull(&this->args_string[place + 1]);

			if (arg_num == 0)
			{
				this->args_params[argv[i][1]].push_back("");
			}
			else
			{
				for (size_t j = 0; j < arg_num; j++)
				{
					this->args_params[argv[i][1]].push_back(argv[i+j+1]);
				}
				i += arg_num;
			}

		}
		else
		{
			this->args_params['!'].push_back(argv[i]);
		}
	}
}

char ArgParser::get_arg()
{
	if (this->iter == this->args_params.end())
	{
		return '\0';
	}
	char c = this->iter->first;
	this->iter++;

	return c;
}

std::vector<std::string> &ArgParser::get_params(char c)
{
	return this->args_params[c];
}
