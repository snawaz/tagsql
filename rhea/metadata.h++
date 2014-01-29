
#pragma once

#include <vector>
#include <string>
#include <cassert>
#include <algorithm>

#include <pqxx/pqxx>

#include "meta_column.h++"
#include "meta_table.h++"

namespace tagsql { namespace meta 
{
	class metadata
	{
	public:
		metadata(std::string dbname, std::string host, int port, std::string username, std::string password)
			: _connection("dbname=" + dbname + 
						  " host="  + host + 
						  " port="  + std::to_string(port) + 
						  " user="  + username + 
						  " password=" + password)
		{
		}
		auto meta_tables() -> std::vector<meta_table>
		{
			//reading names of all table.
			auto rtables = exec("SELECT table_name FROM information_schema.tables WHERE table_type ='BASE TABLE' AND table_schema='public'");

			//for each table, read the name and data_type of all columns
			std::vector<meta_table> tables;
			for(auto const & t : rtables)
			{
				std::string table_name = t[0].c_str();
				auto rcolumns = exec("select column_name,data_type,character_maximum_length,ordinal_position,is_nullable,column_default \
																from information_schema.columns where table_name = '" + table_name + "'");
				std::vector<meta_column> columns;
				//auto to_size = [](pqxx::field const item) { return item.is_null() ? 0 : item.as<std::size_t>(); };
				for(auto const & c : rcolumns)
				{
					meta_column column 
					{
						c[0].c_str(),              //column name
						c[1].c_str(),              //column type
						table_name,                //table name
						c[2].as<std::size_t>(0),   //character_max_length
						c[3].as<std::size_t>(),    //ordinal_position
						c[4].c_str()[0] == 'Y',    //is_nullable
						!c[5].is_null(),           //has_server_default
					};
					columns.push_back(std::move(column));
				}
				std::sort(columns.begin(), columns.end(), [](meta_column const & c1, meta_column const & c2){ return c1.ordinal_position < c2.ordinal_position; });
				meta_table table {table_name, std::move(columns)};
				tables.push_back( std::move(table) );
			}

			//read all the table and their primary key
			auto pkeys = exec("select tc.table_name, kc.column_name from information_schema.table_constraints as tc \
			    join information_schema.key_column_usage as kc on kc.constraint_name = tc.constraint_name \
			    where tc.table_schema = 'public' and tc.constraint_type = 'PRIMARY KEY'");
			for(auto const & key : pkeys)
			{
				auto it = std::find_if(tables.begin(), tables.end(),[&](meta_table const & t) { return t.name == key[0].c_str(); });
				assert(it != tables.end());
				auto ic = std::find_if(it->columns.begin(),it->columns.end(),[&](meta_column const & c) { return c.name == key[1].c_str(); });
				assert(ic != it->columns.end());
				ic->is_primary_key = true;
		    }
			
			//read all the table and their foreign key
			auto fkeyinfos = exec("select a.table_name, a.column_name, kc.table_name, kc.column_name from \
			    (select tc.table_name, kc.column_name, kc.constraint_name, rc.unique_constraint_name \
			     from information_schema.table_constraints as tc \
			     join information_schema.key_column_usage as kc on kc.constraint_name = tc.constraint_name \
			     join information_schema.referential_constraints as rc on rc.constraint_name = kc.constraint_name \
			     where tc.table_schema = 'public' and tc.constraint_type = 'FOREIGN KEY') as a \
			     join information_schema.key_column_usage as kc on kc.constraint_name = a.unique_constraint_name");
			for(auto const & fkeyinfo : fkeyinfos) //table, fkey, reftable
			{
				auto it = std::find_if(tables.begin(), tables.end(),[&](meta_table const & t) { return t.name == fkeyinfo[0].c_str(); });
				assert(it != tables.end());
			
				auto ic = std::find_if(it->columns.begin(),it->columns.end(), [&](meta_column const & c) { return c.name == fkeyinfo[1].c_str(); });
				assert(ic != it->columns.end());

				ic->is_foreign_key = true;
				ic->ref_table_name = fkeyinfo[2].c_str();
				ic->ref_column_name = fkeyinfo[3].c_str();
		    }
			return tables;
		}
	private:
		pqxx::result exec(std::string query)
		{
			return pqxx::work(_connection).exec(query);
		}
	private:
		pqxx::connection _connection;
	};

}} // tagsql # meta
