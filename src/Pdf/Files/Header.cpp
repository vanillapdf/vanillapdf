#include "Header.h"
#include "lexical/LexicalStream.h"
#include "Exception.h"

#include <cassert>
#include <vector>
#include <regex>

namespace Pdf
{
	namespace Files
	{
		using namespace std;
		using namespace Streams::Basic;

		Stream& operator>> (Stream& s, Header& o)
		{
			auto data = s.Readline();
			assert(data->Size() > 0);

			string str(data->begin(), data->end());

			std::smatch sm;
			std::regex header_regex("%PDF-([0-9])\\.([0-9])");
			if (std::regex_match(str, sm, header_regex))
			{
				assert(sm.size() == 3);
				o._majorVersion = stoi(sm[1]);
				o._minorVersion = stoi(sm[2]);
			}
			else
			{
				throw Exception("Could not find a valid PDF header");
			}

			assert(o._majorVersion == 1);
			assert(o._minorVersion >= 0 && o._minorVersion <= 7);

			return s;
		}
	}
}
