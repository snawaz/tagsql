





#pragma once


#include <foam/meta/typelist.h++>

#include <tuple>


namespace tagsql { namespace development
{
    
    template<typename ... Tags>
    class named_tuple : public Tags...
    {
            struct sink
            {
                template<typename ...T>
                sink(T && ...) {}
            };
        public:
            using taglist = ::foam::meta::typelist<Tags...>;
    
            named_tuple(typename Tags::column_type ... values) : _tuple(values...)
            {
                sink { (Tags::set(values),0)... };
            }
    
            //tag-based access (non-const)
            template<typename Tag>
            auto operator[](Tag const &tag) -> typename Tag::column_type &
            {
                return const_cast<typename Tag::column_type&>(static_cast<named_tuple const&>(*this)[tag]);
            }
    
            //tag-based access (const)
            template<typename Tag>
            auto operator[](Tag const &) const -> typename Tag::column_type const&
            {
                constexpr auto index = taglist::template find<Tag>::value;
                static_assert( index >= 0, "Invalid tag passed to named_tuple::operator[].");
                return std::get<index>(_tuple);
            }
    
            //index-based access (non-const)
            template<std::size_t Index>
            auto & at() 
            {
                static_assert( Index < taglist::size, "Index is out of range for named_tuple::at<>.");
                return std::get<Index>(_tuple);
            }
    
            //index-based access (const)
            template<std::size_t Index>
            auto const & at() const
            {
                static_assert( Index < taglist::size, "Index is out of range for named_tuple::at<>.");
                return std::get<Index>(_tuple);
            }
        private:
            std::tuple<typename Tags::column_type...> _tuple;
    };
    

}} //tagsql # development
