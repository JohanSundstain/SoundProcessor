#ifndef CONFIG_PARSER
#define	CONFIG_PARSER
#include <iostream>
#include <fstream>
#include <vector>
#include "wavobj.h"
#include "converter.h"

class ConfigParser
{
private:
	std::ifstream config_file;
	std::vector<Converter*> converters;
	std::vector<WavFile*> inputs;
	void parse();
	void ws_split(std::vector<std::string>& v, std::string s);
public:
	ConfigParser(const char* file_name,std::vector<WavFile*>& inputs);
	~ConfigParser();
	std::vector<Converter*>& get_converters();

};

#endif // !1

