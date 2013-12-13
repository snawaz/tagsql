





#pragma once


#include <foam/meta/typelist.h++>

#include <tagsql/development/column.h++>

#include <tuple>


namespace tagsql { namespace development
{
    
    template<typename ... Tags>
    class named_tuple : public Tags::template named_member<column<Tags>>...
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
                //sink { (Tags::set(values),0)... };
                
				sink { reinterpret_cast<column<Tags>*>(static_cast<typename Tags::template named_member<column<Tags>>*>(this))->set(values) ...  }; 
            }

			//truncate conversion to subset of OtherTags!
			template<typename ...OtherTags>
			named_tuple(named_tuple<OtherTags...> const & item)
			{
				using self_type = ::foam::meta::typelist<Tags...>;
				using other_type = ::foam::meta::typelist<OtherTags...>;
				static_assert(self_type::template is_sublist_of<other_type>::value, "Cannot be converted into the target named_tuple. Source must be superset or equal to target tags.");

				//sink { (Tags::set(item[Tags()]), 0)... };
				
				sink { reinterpret_cast<column<Tags>*>(static_cast<typename Tags::template named_member<column<Tags>>*>(this))->set(item[Tags()]) ...  }; 
				//auto & self = *this;
				//sink { (self[Tags()] = item[Tags()]) ... }; 
			}
   
#if 0			
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
#endif

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
                auto & member = *reinterpret_cast<column<Tag> const*>(static_cast<typename Tag::template named_member<column<Tag>> const*>(this));
				return member.value();
            }

            //index-based access (non-const)
            template<std::size_t Index>
            auto & at() 
            {
				auto & x = static_cast<named_tuple const&>(*this).at<Index>();
				return const_cast<typename std::remove_cv<decltype(x)>::type>(x);
            }
    
            //index-based access (const)
            template<std::size_t Index>
            auto const & at() const
            {
                static_assert( Index < taglist::size, "Index is out of range for named_tuple::at<>.");
                return get<Index>(std::integral_constant<bool, (Index < taglist::size)>());
            }
		private:	
			template<std::size_t Index>
			auto & get(std::true_type) const
            {
				return *this[typename taglist::template at<Index>::type()];
			}
			void get(std::false_type); //no need to define
        private:
            std::tuple<typename Tags::column_type...> _tuple;  //GET RID OF THIS MEMBER!
    };
    

}} //tagsql # development
