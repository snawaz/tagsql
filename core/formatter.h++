

#pragma once


#include <tagsql/core/insert_adaptor.h++>
#include <tagsql/core/tiny_types.h++>

namespace tagsql { namespace formatting
{
	template<typename NamedTuple, typename Adaptor>
	class formatter
	{
		public:
			using value_type = NamedTuple;
			using output_type = typename Adaptor::value_type;

			formatter(value_type const & value)
			{
				write(value, typename value_type::taglist());
			}
			
			output_type const & output() const { return _output; }

		private:
			template<typename ...Tags>
			void write(value_type const & value, ::foam::meta::typelist<Tags...> const &)
			{
				unpack { (_adaptor.write(value.template get<Tags>(), _output),0) ... };
			}

		private:
			Adaptor        _adaptor;                
			output_type    _output;
	};

}} //tagsql # formatting
