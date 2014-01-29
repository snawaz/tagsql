

#pragma once

#include <type_traits>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <sstream>

namespace tagsql
{
    template<typename InputIterator>
    std::string join(std::string const & token, InputIterator begin, InputIterator end)
    {
		if ( begin == end ) 
	    	return {};
#if 1	
	std::stringstream result;
	result << *begin;
	for ( ++begin  ; begin != end ; ++begin)
	{
	    result << token  << *begin;
	}
	return result.str();
#else	
	std::string result = *begin;
	for ( ++begin  ; begin != end ; ++begin)
	{
	    result += token + *begin;
	}
	return result;
#endif	
    }

    template<typename Container>
    std::string join(std::string const & token, Container const & items)
    {
		using std::begin;
		using std::end;

		return join(token, begin(items), end(items));
    }

    template<typename Container, typename Transformer>
    auto transform(Container const & items, Transformer f) -> std::vector<decltype(f(*items.begin()))>
    {
		using result_type = typename std::result_of<Transformer(typename Container::value_type)>::type;
		std::vector<result_type> values;
		values.reserve(items.size());
		std::transform(items.begin(), items.end(), std::back_inserter(values), f);
		return values;
    }

    auto split(std::string const & s, std::string delim) -> std::vector<std::string>
    {
	std::size_t i = 0;
	std::vector<std::string> tokens;
	while(true)
	{
	    auto j = s.find(delim, i);
	    if ( j == std::string::npos) 
	    {
	    	tokens.push_back(s.substr(i));
		break;
	    }
	    tokens.push_back(s.substr(i, j - i));
	    i = j + delim.size();
	}
	return tokens;
    }
}
