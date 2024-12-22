#ifndef ARG_PARSER_H
#define ARG_PARSER_H
#include <string>
#include <vector>
#include <map>

class ArgParser
{
private:
	void parse(int argc,char* argv[]);
	std::string args_string;
	std::map<char, std::vector<std::string>> args_params; // '!' main param without letters
	std::map<char, std::vector<std::string>>::iterator iter;
public:
	ArgParser(int argc,char* argv[], std::string args);

	char get_arg();

	std::vector<std::string>& get_params(char c);


};

#endif // !ARG_PARSER_H


