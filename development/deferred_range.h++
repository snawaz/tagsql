





#pragma once


#include <vector>
#include <memory>

#include <tagsql/development/row_type_helper.h++>
#include <tagsql/string_algo.h++>
#include <pqxx/pqxx>



namespace tagsql { namespace development
{
    template<typename Bucket>
    class deferred_range 
    {
		public: 
			using bucket_type    = Bucket;
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
				return const_cast<deferred_range&>(*this).deferred_exec().begin();
			}
			auto end() const -> const_iterator
			{
				return const_cast<deferred_range&>(*this).deferred_exec().end();
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
    
 			//deferred execution
			//NOTE : if the caller makes a copy, then there will be slicing of the object
			//and the expression will be invalid as it will loss many valuable information
			//So the trick is to avoid slicing and instead use (const) reference.
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
		protected:
    
			std::vector<value_type>& deferred_exec()
			{
				using implied_select = typename detail::row_type<typename bucket_type::all_tables, typename bucket_type::select>::modified_tuple;
				return this->execute(implied_select(), _query_without_select, typename bucket_type::all_tables());
			}
    
			template<typename ImpliedSelectedColumns, typename Tables>
			auto execute(ImpliedSelectedColumns const & columns, std::string const & query_without_select, Tables const & tables) -> std::vector<value_type> &
			{
				constraint_check_for_selected_columns(columns, tables);
				return execute(query_without_select, columns, foam::meta::genseq_t<std::tuple_size<ImpliedSelectedColumns>::value>());
			}
		private:
			template<typename ...Columns, typename Tables>
			void constraint_check_for_selected_columns(std::tuple<Columns...> const &, Tables const &)
			{
				using tables = typename ::foam::meta::typelist<typename Columns::table_type...>::template unique_cvr<>::type;
				static_assert(tables::template is_sublist_of_cvr<Tables>::value, 
								"Constraint Violation : at least one column does not belong to any table mentioned in the query.");
			}
			template<typename ColumnsTuple, int ... N>
			std::vector<value_type>& execute(std::string query_without_select, ColumnsTuple const & columns, foam::meta::seq<N...> seq)
			{
				if ( _executed ) 
					return _results;
				_executed = true;
				static const std::vector<std::string> names { qualify(std::get<N>(columns)) ... };
				static const std::string select_clause = "SELECT " + ::tagsql::join(",", names) + " "; 
				auto query = select_clause + query_without_select;
				auto result =  pqxx::work(*_connection).exec(query);
				for(auto const & item : result)
					write(seq, item, columns, static_cast<value_type*>(0));
				return _results;
			}
			template<int ... N, typename ... Args, typename Columns>
			void write(foam::meta::seq<N...>, pqxx::tuple const & item, Columns const & columns, named_tuple<Args...> *)
			{
				value_type tuple {item[N].template as<typename std::tuple_element<N, Columns>::type::column_type>()...};
				_results.push_back(std::move(tuple));
			}
			template<int ... N, typename Columns>
			void write(foam::meta::seq<N...>, pqxx::tuple const & item, Columns const & columns, ... )
			{
				value_type v {item[N]...};
				_results.push_back(std::move(v));
			}
		public:
			std::shared_ptr<pqxx::connection> _connection;
			std::string                       _query_without_select;
		protected:
			std::vector<value_type>           _results;
			bool                              _executed;
    };

}} //tagsql # development
