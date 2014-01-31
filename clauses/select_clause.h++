
#pragma once

#include <memory>
#include <pqxx/pqxx>

#include <tagsql/anatomy/table.h++>

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
			auto from(Table) -> composite_table<typename SelectQuery::template add_from<table_tag_t<Table>>::type>
			{
				return { _connection, "FROM " + metaspace::meta_table<table_tag_t<Table>>::name() };
			}
		
		private:
			std::shared_ptr<pqxx::connection> _connection;
	};

} //tagsql
