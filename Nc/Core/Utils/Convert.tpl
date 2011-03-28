
/*--------------------------------------------------------------------------------

	PonponUtils
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

--------------------------------------------------------------------------------*/

template <typename T>
bool StringTo(const std::string &s, T &dest)
{
    std::istringstream iss(s);
    return (iss >> dest != 0);
}

template <typename T>
std::string ToString(const T &data)
{
    std::ostringstream oss;
    oss << data;
    return oss.str();
}

template <typename T>
bool SplitStringTo(std::string &str, const std::string &delimitor, T &dest)
{
    size_t pos = str.find_first_of(delimitor);
    bool b = StringTo(str.substr(0, pos++), dest);
    str = str.substr(pos, std::string::npos);
    return b;
}

namespace Unicode
{
	template<typename In, typename Out>
	Out UTF32ToANSI(In begin, In end, Out output, char replacement, const std::locale& lc)
	{
        // Get the facet of the locale which deals with character conversion
        const std::ctype<wchar_t>& Facet = std::use_facet< std::ctype<wchar_t> >(lc);

        // Use the facet to convert each character of the input string
        while (begin < end)
            *output++ = Facet.narrow(static_cast<wchar_t>(*begin++), replacement);
		return output;
	}

	template<typename In, typename Out>
   	Out ANSIToUTF32(In begin, In end, Out output, const std::locale& lc)
	{
		// Get the facet of the locale which deals with character conversion
	    const std::ctype<wchar_t>& Facet = std::use_facet< std::ctype<wchar_t> >(lc);

        // Use the facet to convert each character of the input string
        while (begin < end)
        	*output++ = static_cast<Nc::UInt32>(Facet.widen(*begin++));
		return output;
	}

	template <typename In, typename Out>
    Out UTF16ToUTF32(In begin, In end, Out output, Nc::UInt32 replacement)
	{
		for (Nc::UInt16 c = *begin++; begin < end; )
		{
		    if ((c >= 0xD800) && (c <= 0xDBFF)) // We have a surrogate pair, ie. a character composed of two elements
		    {
		        if (begin < end)
		        {
		            Nc::UInt16 d = *begin++;
		            if ((d >= 0xDC00) && (d <= 0xDFFF))		// The second element is valid : convert the two elements to a UTF-32 character
		                *output++ = static_cast<Nc::UInt32>(((c - 0xD800) << 10) + (d - 0xDC00) + 0x0010000);
		            else if (replacement) // the second element is invalid
		                *output++ = replacement;
		        }
		    }
		    else if ((c >= 0xDC00) && (c <= 0xDFFF)) 	// Invalid character
		    {
		        if (replacement)
		            *output++ = replacement;
		    }
		    else // Valid character directly convertible to UTF-32
		        *output++ = static_cast<Nc::UInt32>(c);
		}
		return output;
	}

	template<typename In, typename Out>
	Out UTF32ToUTF16(In begin, In end, Out output, Nc::UInt16 replacement)
	{
		while (begin < end)
		{
		    Nc::UInt32 c = *begin++;
		    if (c < 0xFFFF)		// Character can be converted directly to 16 bits, just need to check it's in the valid range
		    {
		        if ((c >= 0xD800) && (c <= 0xDFFF))
		        {
		            if (replacement)	// Invalid character (this range is reserved)
		                *output++ = replacement;
		        }
		        else	// Valid character directly convertible to 16 bits
		            *output++ = static_cast<Nc::UInt16>(c);
		    }
		    else if (c > 0x0010FFFF)	// Invalid character (greater than the maximum unicode value)
		    {
		        if (replacement)
		            *output++ = replacement;
		    }
		    else	// Character will be converted to 2 UTF-16 elements
		    {
		        c -= 0x0010000;
		        *output++ = static_cast<Nc::UInt16>((c >> 10)     + 0xD800);
		        *output++ = static_cast<Nc::UInt16>((c & 0x3FFUL) + 0xDC00);
		    }
		}
		return output;
	}
}
