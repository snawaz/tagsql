





#pragma once


#include <vector>
#include <memory>

#include <tagsql/string_algo.h++>
#include <pqxx/pqxx>



namespace tagsql { namespace developement
{
    
    template<typename ValueType>
    class deferred_range 
    {
        public: 
        using value_type     = ValueType;
        using iterator       = typename std::vector<value_type>::iterator;
        using const_iterator = typename std::vector<value_type>::const_iterator;
    
        deferred_range(std::shared_ptr<pqxx::connection> connection)
            : _connection(connection), _executed(false) { }
    
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
    
        virtual std::vector<value_type>& deferred_exec() = 0; 
    
        template<typename ImpliedSelectedColumns>
        auto execute(ImpliedSelectedColumns const & columns, std::string const & query_without_select) -> std::vector<ValueType> &
        {
            return execute(query_without_select, columns, foam::meta::genseq_t<std::tuple_size<ImpliedSelectedColumns>::value>());
        }
        private:
        template<typename ColumnsTuple, int ... N>
        std::vector<ValueType>& execute(std::string query_without_select, ColumnsTuple const & columns, foam::meta::seq<N...> seq)
        {
            if ( _executed ) 
                return _results;
            _executed = true;
            static const std::vector<std::string> names { qualify(std::get<N>(columns)) ... };
            static const std::string select_clause = "SELECT " + ::tagsql::join(",", names) + " "; 
            auto query = select_clause + query_without_select;
            auto result =  pqxx::work(*_connection).exec(query);
            for(auto const & item : result)
            write(seq, item, columns, static_cast<ValueType*>(0));
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
        protected:
        std::shared_ptr<pqxx::connection> _connection;
        std::vector<value_type>           _results;
        bool                              _executed;
    };
    

}} //tagsql # developement
