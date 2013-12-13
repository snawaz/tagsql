





#pragma once


#include <type_traits>
#include <string>
#include <memory>
#include <array>

#include <foam/meta/seq.h++>
#include <foam/meta/typelist.h++>

#include <tagsql/development/table.h++>
#include <tagsql/development/meta_table.h++>
#include <tagsql/development/meta_column.h++>
#include <tagsql/development/tags.h++>
#include <tagsql/development/cross_expression.h++>
#include <tagsql/development/join_expression.h++>
#include <tagsql/development/deferred_range.h++>


#include <pqxx/pqxx>


namespace tagsql { namespace development
{
    template<typename Bucket>
    class where_expression : public deferred_range<Bucket>
    {
            using base = deferred_range<Bucket>;
        public: 
    
            where_expression(std::shared_ptr<pqxx::connection> & connection, std::string query)
            : base(connection, query) 
            { 
            }
            /*
			auto limit(std::size_t count) -> from_expression<Table, SelectedColumnsTuple> &
            {
                _query_without_select += " LIMIT " + std::to_string(count);
                return *this;
            }
			*/
    };
    

}} //tagsql # development
