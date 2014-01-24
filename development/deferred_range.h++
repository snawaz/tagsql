

#pragma once

#include <tagsql/development/row_type_helper.h++>
#include <tagsql/development/common_clauses.h++>
#include <tagsql/development/database_error.h++>
#include <tagsql/development/clause_picker.h++>
#include <tagsql/string_algo.h++>

#include <vector>
#include <memory>

#include <pqxx/pqxx>



namespace tagsql { namespace development
{
    template<typename Bucket>
    struct value_type 
	{
		using type  = typename detail::row_type<typename Bucket::all_tables, typename Bucket::select>::type;
	};

    template<typename Bucket>
    using value_type_t = typename value_type<Bucket>::type;

    template<typename Bucket>
    class deferred_range : public clause_picker<Bucket>
    {
		public:
			
			using _is_deferred_range = std::true_type;

			using bucket_type    = Bucket;
#if 0			
			using value_type     = typename detail::row_type<typename bucket_type::all_tables, typename bucket_type::select>::type;
			using iterator       = typename std::vector<value_type>::iterator;
			using const_iterator = typename std::vector<value_type>::const_iterator;
			
			deferred_range(std::shared_ptr<pqxx::connection> connection, std::string query_without_select)
				: _connection(connection), _query_without_select(query_without_select), _executed(false) { }
    
			//begin and end : const and non-const versions
			auto begin() -> iterator
			{
				return deferred_exec().begin();
			}
			auto end() -> iterator
			{
				return deferred_exec().end();
			}
			auto begin() const -> const_iterator
			{
				return const_cast<deferred_range&>(*this).begin();
			}
			auto end() const -> const_iterator
			{
				return const_cast<deferred_range&>(*this).end();
			}
    
			//index-based access : const and non-const versions
			auto operator[](int index) -> value_type &
			{
				return deferred_exec().at(index);
			}
			auto operator[](int index) const -> value_type const &
			{
				return const_cast<deferred_range&>(*this).deferred_exec().at(index);
			}
    
 			//friendly upcast
			auto defer() -> deferred_range&
			{
				return *this; 
			}
			auto defer() const -> deferred_range const &
			{
				return *this; 
			}
			
			//representations
			auto as_vector() const -> std::vector<value_type> const &
			{
				return const_cast<deferred_range&>(*this).deferred_exec();
			}
			auto move_as_vector() const -> std::vector<value_type> 
			{
				std::vector<value_type> & items = const_cast<deferred_range&>(*this).deferred_exec();
				_executed = false;
				return std::move(items);
        	}
			auto query_string() const -> std::string const &
			{
				typename value_type::taglist columnlist{};
				static_check(columnlist, typename bucket_type::all_tables());
				return make_query_string(columnlist);
			}
		private:
			std::vector<value_type>& deferred_exec()
			{
				return execute(typename value_type::indices());
			}
			template<typename ...Columns, typename Tables>
			void static_check(::foam::meta::typelist<Columns...> const &, Tables const &) const
			{
				using tables = typename ::foam::meta::typelist<typename Columns::table_type...>::template unique_cvr<>::type;
				static_assert(tables::template is_sublist_of_cvr<Tables>::value, 
								"Constraint Violation : at least one column does not belong to any table mentioned in the query.");
			}
			template<typename ...Columns>
			std::string const & make_query_string(::foam::meta::typelist<Columns...>) const 
			{
				static const std::vector<std::string> names { qualify(Columns()) ... };
				static const std::string select_clause = "SELECT " + ::tagsql::join(",", names) + " "; 
				if ( _query.empty() )
					_query = select_clause + _query_without_select;
				return _query;
			}
			template<int ... N>
			std::vector<value_type>& execute(foam::meta::seq<N...> const &)
			{
				if ( _executed ) 
					return _results;
				_executed = true;
				try
				{
					auto result =  pqxx::work(*_connection).exec(query_string());
					for(pqxx::tuple && item : result) 
					{
						_results.emplace_back(construct_from_field, std::move(item[N])...);
					}
				}
				catch(std::exception const & e)
				{
					throw database_error(query_string(),e); 
				}
				return _results;
			}
		public:
			std::shared_ptr<pqxx::connection> _connection;
			std::string                       _query_without_select;
			mutable std::string               _query;
		protected:
			std::vector<value_type>           _results;
			bool                              _executed;
#else
			deferred_range(std::shared_ptr<pqxx::connection> connection, std::string query_without_select)
				: _connection(connection), _query_without_select(query_without_select), _executed(false) { }
    
			//begin and end : const and non-const versions
			auto begin() //-> iterator
			{
				return deferred_exec().begin();
			}
			auto end() //-> iterator
			{
				return deferred_exec().end();
			}
			auto begin() const //-> const_iterator
			{
				return const_cast<deferred_range&>(*this).begin();
			}
			auto end() const //-> const_iterator
			{
				return const_cast<deferred_range&>(*this).end();
			}
    
			//index-based access : const and non-const versions
			auto operator[](int index) -> value_type_t<Bucket> &
			{
				return deferred_exec().at(index);
			}
			auto operator[](int index) const -> value_type_t<Bucket> const &
			{
				return const_cast<deferred_range&>(*this).deferred_exec().at(index);
			}
    
 			//friendly upcast
			auto defer() -> deferred_range&
			{
				return *this; 
			}
			auto defer() const -> deferred_range const &
			{
				return *this; 
			}
			
			//representations
			auto as_vector() const -> std::vector<value_type_t<Bucket>> const &
			{
				return const_cast<deferred_range&>(*this).deferred_exec();
			}
			auto move_as_vector() const -> std::vector<value_type_t<Bucket>> 
			{
				std::vector<value_type_t<Bucket>> & items = const_cast<deferred_range&>(*this).deferred_exec();
				_executed = false;
				return std::move(items);
        	}
			auto query_string() const -> std::string const &
			{
				typename value_type_t<Bucket>::taglist columnlist{};
				static_check(columnlist, typename bucket_type::all_tables());
				return make_query_string(columnlist);
			}
		private:
			std::vector<value_type_t<Bucket>>& deferred_exec()
			{
				return execute(typename value_type_t<Bucket>::indices());
			}
			template<typename ...Columns, typename Tables>
			void static_check(::foam::meta::typelist<Columns...> const &, Tables const &) const
			{
				using tables = typename ::foam::meta::typelist<typename Columns::table_type...>::template unique_cvr<>::type;
				static_assert(tables::template is_sublist_of_cvr<Tables>::value, 
								"Constraint Violation : at least one column does not belong to any table mentioned in the query.");
			}
			template<typename ...Columns>
			std::string const & make_query_string(::foam::meta::typelist<Columns...>) const 
			{
				static const std::vector<std::string> names { qualify(Columns()) ... };
				static const std::string select_clause = "SELECT " + ::tagsql::join(",", names) + " "; 
				if ( _query.empty() )
					_query = select_clause + _query_without_select;
				return _query;
			}
			template<int ... N>
			std::vector<value_type_t<Bucket>>& execute(foam::meta::seq<N...> const &)
			{
				static std::vector<value_type_t<Bucket>>           _results;
				if ( _executed ) 
					return _results;
				_executed = true;
				try
				{
					auto result =  pqxx::work(*_connection).exec(query_string());
					for(pqxx::tuple && item : result) 
					{
						_results.emplace_back(construct_from_field, std::move(item[N])...);
					}
				}
				catch(std::exception const & e)
				{
					throw database_error(query_string(),e); 
				}
				return _results;
			}
		public:
			std::shared_ptr<pqxx::connection> _connection;
			std::string                       _query_without_select;
			mutable std::string               _query;
		protected:
			bool                              _executed;
#endif			

    };

	template<typename Bucket>
	std::ostream& operator<<(std::ostream & out, deferred_range<Bucket> const & items)
	{
		out << items.query_string() << std::endl;
		for(auto const & item : items)
			out << item << "\n";
		return out;
	}

}} //tagsql # development
