





#pragma once

#include <tagsql/query/select_query.h++>
#include <tagsql/query/insert_query.h++>
#include <tagsql/query/update_query.h++>
#include <tagsql/core/formatter.h++>
#include <tagsql/core/update_expression.h++>
#include <tagsql/clauses/all.h++>
#include <tagsql/anatomy/table.h++>


namespace tagsql 
{
	template<typename ...Columns>
	struct make_select_clause 
	{
		using type = select_clause<select_query<::std::tuple<typename get_tag<Columns>::type...>>>;
	};

	class data_context 
	{
		public:
			
			data_context(std::string dbname, std::string host, int port, std::string username, std::string password)
			{
				std::string connection_string = ::foam::strlib::format("dbname={0} host={1} port={2} user={3} password={4}", dbname, host, port, username, password);
				_connection.reset(new pqxx::connection(connection_string));
			}

			//select (DB style)
			template<typename ... Columns>
			auto select(Columns ... ) -> typename make_select_clause<Columns...>::type
			{
				return {_connection };
			}
    
			//insert (DB sytle)
			template<typename Table>
			auto insert_into(Table const &) -> insert_query<data_context, table_tag_t<Table>>
			{
				return {*this};
			}
			
			//insert (C++ container style)
			template<typename Table>
			auto insert(Table const & item) -> pqxx::result
			{
				formatting::sql_insert_formatter<Table> fmt { item };
				auto insert_command = ::foam::strlib::format("INSERT INTO {0} ({1}) VALUES ({2})",
																				metaspace::meta_table<Table>::name(), 
																				tagsql::join(",", std::get<0>(fmt.output())),
																				tagsql::join(",", std::get<1>(fmt.output())));
				return execute(insert_command);
			}
    
			//update (DB style)
			template<typename Table>
			auto update(Table const &) -> update_query<data_context, table_tag_t<Table>>
			{
				return { *this };
			}
	
			//execute raw query
			auto execute(std::string query) -> pqxx::result
			{
				pqxx::work transaction { *_connection } ;
				auto ret = transaction.exec(query);
				transaction.commit();
				return ret;
			}
		private:
			std::shared_ptr<pqxx::connection> _connection;
	};

} //tagsql 
