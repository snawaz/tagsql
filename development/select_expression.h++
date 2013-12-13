





#pragma once


#include <memory>

#include <tagsql/development/from_expression.h++>

#include <pqxx/pqxx>

namespace tagsql { namespace development
{
	template<typename Bucket>
	class select_expression 
    {
        public: 
			select_expression(std::shared_ptr<pqxx::connection> & connection) : _connection(connection) {}
    
			template<typename Table>
			auto from(Table) -> from_expression<typename Bucket::template add_from<Table>::type>
			{
				return { _connection };
			}
		private:
			std::shared_ptr<pqxx::connection> _connection;
	};

}} //tagsql # development
