





#pragma once


#include <type_traits>
#include <string>
#include <array>
#include <tagsql/core/unproductive_tag.h++>


namespace tagsql { namespace metaspace
{
    
	template<typename Table>
	struct meta_table;
    
	template<typename Table, typename ColumnType>
	struct update_data
	{
		using column_type = ColumnType;
		
		std::string const column_name;
		column_type const value;
	};
    
	template<typename Table, typename ColumnType, bool Nullable, bool ServerDefault>
	struct meta_column : public unproductive_tag_t
	{
		using table_type  = Table;
		using column_type = ColumnType;
		using _is_column = std::true_type;
    
		static const bool is_nullable = Nullable;
		static const bool server_default = ServerDefault;
    
		std::string const column_name;
    
		meta_column(std::string column_name) : column_name(std::move(column_name)) {}
    
		auto operator()(column_type const & value) const -> update_data<Table, ColumnType>
		{
			return {column_name, value};
		}

		std::string repr() const 
		{
			return meta_table<Table>::name() + "." + column_name;
		}

	};
    
#if 0
	template<typename Table, typename ColumnType, bool Nullable, bool ServerDefault>
	struct meta_column_special: meta_column<Table, ColumnType, Nullable, ServerDefault>
	{
		using base = meta_column<Table, ColumnType, Nullable, ServerDefault>;
		using base::base;
	};
    
	template<typename Table, typename ColumnType, bool Nullable, bool ServerDefault>
	using meta_column_t = typename std::conditional<std::is_fundamental<ColumnType>::value, 
		  											meta_column<Table,ColumnType,Nullable,ServerDefault>,
		  											meta_column_special<Table,ColumnType,Nullable,ServerDefault>>::type;
#else
	template<typename Table, typename ColumnType, bool Nullable, bool ServerDefault>
	using meta_column_t = meta_column<Table,ColumnType,Nullable,ServerDefault>;
#endif

    
}} //tagsql # metaspace
