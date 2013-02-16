#include "../includes/htmasciiparser.hpp"

bool ICoDF_HTM::HTMAsciiParser::Parse(std::string& filename)
{
	std::ifstream	file(filename);
	std::string	line(""), str1(""), str2("");
	size_t	pos1, pos2;
	nbObj = 0;
	
	if (file)
    {
		while (getline(file, line))
		{
			pos1 = line.find_first_of(" ", 0);
			str1 = line.substr(0, pos1);
			pos2 = line.find_first_of(" ", (pos1 + 1));
			str2 = line.substr(pos1+1, pos2-pos1-1);
			if ((str1 != " ") && (str1 != "\n") && (str1 != ""))
			{
				const double nb1 = strtod(str1.c_str(), NULL);
				const double nb2 = strtod(str2.c_str(), NULL);
				this->_objects.push_back(std::make_pair(nb1, nb2));
				nbObj++;
			}
		}
		file.close();
        return true;
    }
    return false;
}

unsigned int ICoDF_HTM::HTMAsciiParser::getNbObj(void) const
{
	return this->nbObj;
}

std::vector<std::pair<double, double>> const & ICoDF_HTM::HTMAsciiParser::getObjects(void) const
{
    return this->_objects;
}

ICoDF_HTM::HTMAsciiParser::HTMAsciiParser(HTM * parHtm)
: nbObj(0)
{
}

ICoDF_HTM::HTMAsciiParser::~HTMAsciiParser(void)
{
}
