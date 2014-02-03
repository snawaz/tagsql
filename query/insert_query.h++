

#pragma once

#include <vector>
#include <tuple>

#include <tagsql/core/tiny_types.h++>
#include <tagsql/named_tuple.h++>
#include <tagsql/anatomy/column.h++>
#include <tagsql/common/string_algo.h++>
#include <tagsql/core/insert_adaptor.h++>
#include <tagsql/core/meta_table_base.h++>

namespace tagsql
{
	template<typename DataContext, typename Table, typename ... ColumnTags>
	struct insert_values
	{
	public:
		
		insert_values(DataContext & dc) : _dc(dc) {}

		template<typename ... Args>
		auto values(Args && ... args)  -> insert_values<DataContext, Table, ColumnTags ...> &
		{
			std::string tokens[] { formatting::decorate(std::forward<Args>(args)) ... };
			_values.push_back("(" + join(", ", tokens) + ")");
			return *this;
		}
		void commit()
		{
			if ( ! _values.empty() )
			{
				std::string columns[] { ColumnTags().column_name ... };
				auto command = ::foam::strlib::format("INSERT INTO {0} ({1}) VALUES {2};", metaspace::meta_table<Table>::name(), join(",", columns),join(",", _values) );
				std::cout << command << std::endl;
				_values.clear();
				_dc.execute(command);
			}
		}
		~insert_values()
		{
			commit();
		}
	private:
		DataContext _dc;
		std::vector<std::string> _values;
	};

	template<typename DataContext, typename Table>
	struct insert_query
	{
	public:
		insert_query(DataContext & dc) : _dc(dc) {}
		~insert_query()
		{
			commit();
		}

		template<typename ... Columns>
		auto columns(Columns ... ) -> insert_values<DataContext, Table, column_tag_t<Columns>...>
		{
			return { _dc };
		}
		template<typename ... Columns>
		auto operator()(Columns ... ) -> insert_values<DataContext, Table, column_tag_t<Columns>...>
		{
			return { _dc };
		}
		template<typename ... NamedArgs>
		auto set(NamedArgs ... args) -> insert_query<DataContext, Table> &
		{
			static std::string cols[] { std::move(NamedArgs::column_type::column_name(false)) ... };
			static auto joined_cols = join(",", cols);
			std::string vals[] { formatting::decorate(std::move(args._value)) ... };
			_insert_commands.push_back(::foam::strlib::format("INSERT INTO {0} ({1}) VALUES ({2});",metaspace::meta_table<Table>::name(),joined_cols, join(",", vals)));
			unpack { (args._commit = false) ... };
			return *this;
		}
		void commit()
		{
			if ( ! _insert_commands.empty() )
			{
				auto scoped_commands = std::move(_insert_commands);
				for(auto const & command : scoped_commands )
					_dc.execute(command);
			}
		}
	private:
		DataContext _dc;
		std::vector<std::string> _insert_commands;
	};

}
