





#pragma once


#include <tagsql/developement/formatter.h++>
#include <tagsql/developement/update_expression.h++>
#include <tagsql/developement/select_expression.h++>


namespace tagsql { namespace developement
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
            using tagsql::developement::formatting::decorate;
            static const std::vector<std::string> names { columns.column_name ... };
            static const std::vector<std::string> values { decorate(columns.value) ... };
            return ::foam::strlib::format("UPDATE {0} SET ({1}) = ({2})", metaspace::meta_table<Table>::name(), 
                                            ::tagsql::join(",", names), 
                                            ::tagsql::join(",", values));
        }
    }
    
    class data_context 
    {
        public:
    
        data_context(std::string dbname, std::string host, int port, std::string username, std::string password)
        {
            std::string connection_string = ::foam::strlib::format("dbname={0} host={1} port={2} user={3} password={4}", dbname, host, port, username, password);
            _connection.reset(new pqxx::connection(connection_string));
        }
    
        //select
        template<typename ... SelectedColumns>
        auto select(SelectedColumns ... columns) -> select_expression<std::tuple<SelectedColumns...>>
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
    

}} //tagsql # developement
