





#pragma once

#include <tagsql/development/bucket.h++>
#include <tagsql/development/formatter.h++>
#include <tagsql/development/update_expression.h++>
#include <tagsql/development/select_expression.h++>


namespace tagsql { namespace development
{
    
    namespace query
    {
        //insert
        template<typename Table>
        auto for_insert(Table const & item) -> std::string
        {
            formatting::sql_insert_formatter<Table> fmt { item };
            return  ::foam::strlib::format("INSERT INTO {0} ({1}) VALUES ({2})", 
                                                metaspace::meta_table<Table>::name(),
                                                tagsql::join(",", std::get<0>(fmt.output())),
                                                tagsql::join(",", std::get<1>(fmt.output())));
        }
    
        //update
        template<typename Table, typename ... UpdateColumns>
        auto for_update(UpdateColumns && ... columns) -> std::string
        {
            using tagsql::development::formatting::decorate;
            static const std::vector<std::string> names { columns.column_name ... };
            static const std::vector<std::string> values { decorate(columns.value) ... };
            return ::foam::strlib::format("UPDATE {0} SET ({1}) = ({2})", metaspace::meta_table<Table>::name(), 
                                            ::tagsql::join(",", names), 
                                            ::tagsql::join(",", values));
        }
    }
   
	template<typename Column>
	auto _is_valid_select_arg_impl(typename Column::_is_tag *) -> std::true_type;
	
	template<typename Column>
	auto _is_valid_select_arg_impl(...) -> std::false_type;

	template<typename Column>
	using _is_valid_select_arg = decltype(_is_valid_select_arg_impl<Column>(0));

	template<typename Column>
	struct get_tag
	{
		static_assert(_is_valid_select_arg<Column>::value, "Invalid tag in select()");
	
		template<typename T>
		static auto tag_type(typename T::tag_type *) -> typename T::tag_type;
		
		template<typename T>
		static auto tag_type(...) -> T;

		using type = decltype(tag_type<Column>(0));
	};
	

	template<typename ...Columns>
	struct make_select_expression 
	{
		using type = select_expression<bucket<::std::tuple<typename get_tag<Columns>::type...>>>;
	};

    class data_context 
    {
        public:
    
			data_context(std::string dbname, std::string host, int port, std::string username, std::string password)
			{
				std::string connection_string = ::foam::strlib::format("dbname={0} host={1} port={2} user={3} password={4}", dbname, host, port, username, password);
				_connection.reset(new pqxx::connection(connection_string));
			}

        //select
        template<typename ... Columns>
        auto select(Columns ... ) -> typename make_select_expression<Columns...>::type
        //auto select(Columns ... ) -> select_expression<bucket<::foam::meta::typelist<Columns...>>>
        {
            return {_connection };
        }
    
        //insert
        template<typename Table>
        auto insert(Table const & item) -> std::string
        {
            return execute(query::for_insert(item));
        }
    
        //update
        template<typename Table, typename ... UpdateColumns>
        auto update(UpdateColumns && ... columns) -> update_expression<Table>
        {
            return { _connection, query::for_update<Table>(std::forward<UpdateColumns>(columns)...) };
        }

        private:
        std::string execute(std::string query)
        {
            pqxx::work transaction { *_connection } ;
            transaction.exec(query);
            transaction.commit();
            return query;
        }
        private:
        std::shared_ptr<pqxx::connection> _connection;
    };
    

}} //tagsql # development
