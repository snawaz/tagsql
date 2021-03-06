

#pragma once

#include <tagsql/clauses/join_clause.h++>
#include <tagsql/core/join_type.h++>
#include <tagsql/deferred_range.h++>
#include <tagsql/core/meta_table_base.h++>
#include <tagsql/anatomy/table.h++>

#include <string>
#include <pqxx/pqxx>

namespace tagsql
{
	template<typename SelectQuery>
	class composite_table : public deferred_range<SelectQuery>
	{
			using base = deferred_range<SelectQuery>;

		public: 
    
			composite_table(std::shared_ptr<pqxx::connection> & connection, std::string query) : base(connection, query) 
			{ 
			}

			template<typename OtherTable>
			auto cross_join(OtherTable) -> composite_table<typename SelectQuery::template add_join<table_tag_t<OtherTable>, join_type::cross>::type>
			{
				return {this->_connection, this->_query_without_select + " CROSS JOIN " + metaspace::meta_table<table_tag_t<OtherTable>>::name() };
			}

			template<typename OtherTable>
			auto join(OtherTable) -> join_clause<typename SelectQuery::template add_join<table_tag_t<OtherTable>, join_type::inner>::type>
			{
				return { this->_connection, this->_query_without_select };
			}
			
			template<typename OtherTable>
			auto left_join(OtherTable) -> join_clause<typename SelectQuery::template add_join<table_tag_t<OtherTable>, join_type::left>::type>
			{
				return { this->_connection, this->_query_without_select };
			}
            
			template<typename OtherTable>
			auto right_join(OtherTable) -> join_clause<typename SelectQuery::template add_join<table_tag_t<OtherTable>, join_type::right>::type>
			{
				return { this->_connection, this->_query_without_select };
			}
	
			template<typename OtherTable>
			auto full_join(OtherTable) -> join_clause<typename SelectQuery::template add_join<table_tag_t<OtherTable>, join_type::full>::type>
			{
				return { this->_connection, this->_query_without_select };
			}
	};

} //tagsql
