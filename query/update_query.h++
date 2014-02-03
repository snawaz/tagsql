

#pragma once

#include <vector>
#include <tuple>

#include <tagsql/core/tiny_types.h++>
#include <tagsql/named_tuple.h++>
#include <tagsql/anatomy/column.h++>
#include <tagsql/anatomy/expression.h++>
#include <tagsql/common/string_algo.h++>
#include <tagsql/core/insert_adaptor.h++>
#include <tagsql/core/meta_table_base.h++>

namespace tagsql
{
	template<typename DataContext, typename Table>
	struct update_where
	{
	public:
		update_where(DataContext & dc, std::string query) : _dc(dc), _query(std::move(query))
		{
		}
		~update_where()
		{
			commit();
		}
		template<typename Condition>
		auto where(Condition const & expr) -> update_where<DataContext, Table> &
		{
			static_assert(is_condition_expression<Condition>::value, 
					"Constraint Violation : expression passed to where() is invalid. It must be a condition expression involving column(s).");
			static_assert(expression_traits<Condition>::tables::template exists<Table>::value,
					"Constraint Violation : table mismatch for the condition passed to from_expression::where().");
			
			_query += " WHERE " + expr.repr();
			return *this;
		}
		void commit()
		{
			if ( !_query.empty() )
			{
				_dc.execute(_query);
				_query.clear();
			}
		}
	private:
		DataContext & _dc;
		std::string _query;
	};

	template<typename DataContext, typename Table>
	struct update_query
	{
	public:
		update_query(DataContext & dc) : _dc(dc) {}

		template<typename ... NamedArgs>
		auto set(NamedArgs ... args) -> update_where<DataContext, Table>
		{
			std::string set_pairs[] { ::foam::strlib::format("{0} = {1}", NamedArgs::column_type::column_name(false), formatting::decorate(std::move(args._value))) ... };
			unpack { (args._commit = false) ... };
			return { _dc, ::foam::strlib::format("UPDATE {0} SET {1}",metaspace::meta_table<Table>::name(), join(",", set_pairs)) };
		}
	private:
		DataContext & _dc;
	};
}
