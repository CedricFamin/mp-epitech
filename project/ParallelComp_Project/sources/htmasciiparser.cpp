#include <limits>

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
                
                if (_minRa  > nb1) _minRa  = nb1;
                if (_maxRa  < nb1) _maxRa  = nb1;
                if (_minDec > nb2) _minDec = nb2;
                if (_maxDec < nb2) _maxDec = nb2;
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

ICoDF_HTM::HTMAsciiParser::HTMAsciiParser()
: nbObj(0)
, _minRa(std::numeric_limits<double>::max())
, _minDec(std::numeric_limits<double>::max())
, _maxRa(std::numeric_limits<double>::min())
, _maxDec(std::numeric_limits<double>::min())
{
}

ICoDF_HTM::HTMAsciiParser::~HTMAsciiParser(void)
{
}
