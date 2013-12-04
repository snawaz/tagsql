





#pragma once


#include <string>
#include <type_traits>

#include <tagsql/development/table.h++>
#include <tagsql/development/tags.h++>
#include <tagsql/development/named_tuple.h++>
#include <foam/meta/seq.h++>
#include <foam/meta/typelist.h++>

#include <tagsql/string_algo.h++>


namespace tagsql { namespace development { namespace metaspace
{
    
    template<typename Table> 
    struct meta_table 
    {
        static const bool is_table = false;
    };
    
    namespace detail
    {
        template<typename Table, typename ColumnsTuple>
        struct tuple_type;
    
        template<typename Table>
        struct tuple_type<Table, std::tuple<>>
        {
            using type = typename std::remove_cv<decltype(meta_table<Table>::columns())>::type;
        };
        template<typename Table, typename ... Columns>
        struct tuple_type<Table, std::tuple<Columns...>>
        {
            using type = std::tuple<Columns...>;
        };
    
        template<typename Table, typename ColumnsTuple>
        struct row_type;
    
        template<typename Table>
        struct row_type<Table, std::tuple<>>
        {
            using type = Table;
        };
    
        template<typename Table, typename SingleColumn>
        struct row_type<Table, std::tuple<SingleColumn>>
        {
            using type = typename SingleColumn::column_type;
        };
    
        template<typename Table, typename ... Columns>
        struct row_type<Table, std::tuple<Columns...>>
        {
            using list1 = foam::meta::typelist<Columns...>;
            using list2 = typename foam::meta::to_typelist<typename tuple_type<Table, std::tuple<>>::type>::type;
            using type  = typename std::conditional<list1::template is_sublist_of_cvr<list2>::value && list2::template is_sublist_of_cvr<list1>::value,
                                                      Table, named_tuple<Columns...>>::type;
        };
    }
    
    template<typename Table> 
    class meta_table_base
    {
        public:
    
            static const bool is_table = true;
    
            template<typename ColumnsTuple>
            struct row_type : detail::row_type<Table, ColumnsTuple> {};
    
            template<typename ColumnsTuple>
            struct tuple_type : detail::tuple_type<Table, ColumnsTuple> {};
    
            static std::vector<std::string> column_names(bool qualify) 
            {
                static std::vector<std::string> names[2];
                if (names[qualify].empty())
                {
                    names[qualify] = tuple_unpack(qualify, meta_table<Table>::index_sequence());
                }
                return names[qualify];
            }
            static std::string joined_names(bool qualify)
            {
                return tagsql::join(",", column_names(qualify));
            }
    
            template<typename ColumnsTuple>
            static auto columns_tuple(ColumnsTuple tuple)
            {
                return columns_tuple(tuple, std::is_same<typename row_type<ColumnsTuple>::type, Table>());
            }
        private:
            template<typename ColumnsTuple>
            static auto columns_tuple(ColumnsTuple tuple, std::true_type)
            {
                return meta_table<Table>::columns();
            }
            template<typename ColumnsTuple>
            static auto columns_tuple(ColumnsTuple tuple, std::false_type)
            {
                return tuple;
            }
            template<int ... N>
            static std::vector<std::string> tuple_unpack(bool qualify, foam::meta::seq<N...>)
            {
                const auto & columns = meta_table<Table>::columns();
                if ( qualify )
                    return {meta_table<Table>::name() + "." + std::get<N>(columns).column_name ... };
                else
                    return {std::get<N>(columns).column_name ... };
            }
    };
    

}}} //tagsql # development # metaspace
