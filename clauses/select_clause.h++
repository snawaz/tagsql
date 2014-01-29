
#pragma once

#include <memory>

#include <tagsql/core/composite_table.h++>

#include <pqxx/pqxx>

namespace tagsql
{
#if 0	
	template<typename T>
	auto _is_deferred_range_impl(typename T::_is_deferred_range*)  -> std::true_type;
	
	template<typename T>
	auto _is_deferred_range_impl(...)  -> std::false_type;
	
	template<typename T>
	using _is_deferred_range = decltype(_is_deferred_range_impl<T>(0));

	template<typename T>
	using DisableIfDeferredRange = typename std::enable_if<not _is_deferred_range<T>::value>::type;
#endif 
	template<typename SelectQuery>
	class select_clause 
    {
        public: 
			select_clause(std::shared_ptr<pqxx::connection> & connection) : _connection(connection) {}
   
#if 1			
			template<typename Table>
			auto from(Table) -> composite_table<typename SelectQuery::template add_from<Table>::type>
			{
				return { _connection, "FROM " + metaspace::meta_table<Table>::name() };
			}
#else			
			template<typename Table, typename = DisableIfDeferredRange<Table> >
			auto from(Table) -> composite_table<typename SelectQuery::template add_from<Table>::type>
			{
				return { _connection, "FROM " + metaspace::meta_table<Table>::name() };
			}
			template<typename OtherSelectQuery>
			auto from(deferred_range<OtherSelectQuery> r) -> composite_table<typename SelectQuery::template add_from<typename OtherSelectQuery::from>::type>
			{
				return { _connection, "FROM (" + r.query_string() + ")"};
				//return "FROM (" + r.query_string() + ")";
			}
#endif			
		private:
			std::shared_ptr<pqxx::connection> _connection;
	};

} //tagsql
