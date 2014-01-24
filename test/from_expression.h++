





#pragma once


#include <type_traits>
#include <string>
#include <memory>
#include <array>

#include <foam/meta/seq.h++>
#include <foam/meta/typelist.h++>

#include <tagsql/test/table.h++>
#include <tagsql/test/meta_table.h++>
#include <tagsql/test/meta_column.h++>
#include <tagsql/test/tags.h++>
#include <tagsql/test/cross_expression.h++>
#include <tagsql/test/join_expression.h++>
#include <tagsql/test/deferred_range.h++>
#include <tagsql/test/row_type_helper.h++>


#include <pqxx/pqxx>


namespace tagsql { namespace test
{
	
	template<typename Table, typename SelectedColumnsTuple>
	class from_expression : public deferred_range<typename metaspace::meta_table<Table>::template row_type<SelectedColumnsTuple>::type> 
	{
	    	using base = deferred_range<typename metaspace::meta_table<Table>::template row_type<SelectedColumnsTuple>::type>;
	    public: 
	    	using value_type = typename base::value_type; 
	    	using implied_select = typename std::conditional<metaspace::meta_table<value_type>::is_table, typename metaspace::meta_table<Table>::columns_tuple, SelectedColumnsTuple>::type;
	
	    	from_expression(std::shared_ptr<pqxx::connection> & connection)
			: base(connection) 
	    	{ 
		    	_query_without_select =  " FROM " + metaspace::meta_table<Table>::name();
	    	}
	    	template<typename OtherTable>
	    	auto cross_join() -> cross_expression<foam::meta::typelist<Table, OtherTable>, SelectedColumnsTuple>
	    	{
		    	return {this->_connection, _query_without_select};
	    	}
	    	template<typename OtherTable>
	    	auto join() -> join_expression<sql_join_type::inner, foam::meta::typelist<Table, OtherTable>, SelectedColumnsTuple>
	    	{
		    	return {this->_connection, _query_without_select};
	    	}
	    	auto where(condition clause) -> from_expression<Table, SelectedColumnsTuple> &
	    	{
		    	_query_without_select += " WHERE " + clause._value;
		    	return *this;
	    	}
	    	auto limit(std::size_t count) -> from_expression<Table, SelectedColumnsTuple> &
	    	{
		    	_query_without_select += " LIMIT " + std::to_string(count);
		    	return *this;
	    	}
	    	auto exec() -> std::vector<value_type>
	    	{
		    	return deferred_exec(); 
	    	}
	    protected:
	    	virtual std::vector<value_type>& deferred_exec()
	    	{
		    	return this->execute(implied_select(), _query_without_select);
	    	}
	    private:
	    	std::string  _query_without_select;
	};
	

}} //tagsql # test
