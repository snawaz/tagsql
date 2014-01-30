
#pragma once

#include <memory>

#include <pqxx/pqxx>

namespace tagsql
{
	template<typename SelectQuery>
	class composite_table;

	template<typename SelectQuery>
	class select_clause 
    {
        public: 
			select_clause(std::shared_ptr<pqxx::connection> & connection) : _connection(connection) {}
   
			template<typename Table>
			auto from(Table) -> composite_table<typename SelectQuery::template add_from<Table>::type>
			{
				return { _connection, "FROM " + metaspace::meta_table<Table>::name() };
			}
		
		private:
			std::shared_ptr<pqxx::connection> _connection;
	};

} //tagsql
