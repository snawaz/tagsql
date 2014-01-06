





#pragma once


#include <foam/meta/typelist.h++>
#include <foam/meta/seq.h++>

#include <tagsql/development/column.h++>

#include <tuple>


namespace tagsql { namespace development
{
	const struct construct_from_field_t {} construct_from_field {}; //tag to identify correct overload for pqxx::field variadic arguments!

    template<typename ... Tags>
    class named_tuple : public Tags::template named_member<column<Tags>>...
    {
			
			template<typename ...OtherTags>
			friend class named_tuple;

            struct sink
            {
                template<typename ...T>
                sink(T && ...) {}
            };

        public:
	    	using taglist  = ::foam::meta::typelist<Tags...>;
	    	using indices  = ::foam::meta::genseq_t<taglist::size>;
   
			constexpr named_tuple() = default;

			/*
            explicit named_tuple(typename Tags::column_type ... values)
            {
				sink { member<Tags>(values) ... }; 
            }
			*/

			template<typename ... PQXXFields>
            explicit named_tuple(construct_from_field_t, PQXXFields && ... fields)
            {
				static_assert(sizeof...(Tags) == sizeof...(PQXXFields), "Number of arguments to explicit named_tuple(...) is not same");
				sink { member<Tags>(std::forward<PQXXFields>(fields)) ... }; 
            }

			//truncate conversion to subset of OtherTags!
			template<typename ...OtherTags>
			named_tuple(named_tuple<OtherTags...> const & item)
			{
				using self_type = ::foam::meta::typelist<Tags...>;
				using other_type = ::foam::meta::typelist<OtherTags...>;
				static_assert(self_type::template is_sublist_of<other_type>::value, 
						"Cannot be converted into the target named_tuple. Source must be superset or equal to target tags.");

				sink { member<Tags>(item.template member<Tags>()) ...  }; 
			}
   
            //tag-based access (non-const)
            template<typename Tag>
            auto operator[](Tag const &) -> column<Tag> &
            {
				return get<Tag>();
            }
    
            //tag-based access (const)
            template<typename Tag>
            auto operator[](Tag const &) const -> column<Tag> const&
            {
				return get<Tag>();
            }

            //tag-based access (non-const)
            template<typename Tag>
            auto operator[](column<Tag> const &) -> column<Tag> &
            {
				return get<Tag>();
            }
    
            //tag-based access (const)
            template<typename Tag>
            auto operator[](column<Tag> const &) const -> column<Tag> const&
            {
				return get<Tag>();
            }

            //index-based access (non-const)
            template<std::size_t Index>
            auto at() -> column<typename taglist::template at<Index>::type> &
            {
				using is_valid = std::integral_constant<bool, (Index < taglist::size) >;
                static_assert(is_valid::value, "Index is out of range for named_tuple::at<>.");
                return member<Index>(is_valid());
            }
    
            //index-based access (const)
            template<std::size_t Index>
            auto at() const -> column<typename taglist::template at<Index>::type> &
            {
				using is_valid = std::integral_constant<bool, (Index < taglist::size) >;
                static_assert(is_valid::value, "Index is out of range for named_tuple::at<>.");
                return member<Index>(is_valid());
            }
		
			//tag-type based (non-const)
			template<typename Tag>
			auto get() -> column<Tag> &
			{
                constexpr auto index = taglist::template find<Tag>::value;
                static_assert( index >= 0, "Invalid Tag passed to named_tuple::get<>().");
				return member<Tag>();
			}
			
			//tag-type based (const)
			template<typename Tag>
			auto get() const -> column<Tag> const &
			{
                constexpr auto index = taglist::template find<Tag>::value;
                static_assert( index >= 0, "Invalid Tag passed to named_tuple::get<>().");
				return member<Tag>();
			}

		private:

			//inspect members
			template<typename Tag>
			auto member() -> column<Tag> & 
			{
                return const_cast<column<Tag>&>(static_cast<named_tuple const&>(*this).member<Tag>());
			}
			template<typename Tag>
			auto member() const -> column<Tag> const & 
			{
				using base_type = typename Tag::template named_member<column<Tag>>;
                auto const & item = reinterpret_cast<column<Tag> const&>(static_cast<base_type const&>(*this));
				return item;
			}
			template<std::size_t Index>
			auto member(std::true_type) -> column<typename taglist::template at<Index>::type> &
            {
				return member<typename taglist::template at<Index>::type>();
			}
			template<std::size_t Index>
			auto member(std::true_type) const -> column<typename taglist::template at<Index>::type> const &
            {
				return member<typename taglist::template at<Index>::type>();
			}

			//modify members!
			template<typename Tag, typename Other>
			auto member(Other && other) -> column<Tag>&
			{
				using base_type = typename Tag::template named_member<column<Tag>>;
                auto & self = reinterpret_cast<column<Tag>&>(static_cast<base_type&>(*this));
				return self.set(std::forward<Other>(other));
			}
    };
   
	template<> class named_tuple<> {}; //full specialization

	template<typename ... Tags>
	std::ostream & operator << (std::ostream & out, named_tuple<Tags...> const & t)
	{
		using taglist = ::foam::meta::typelist<Tags...>;
		using first = typename taglist::template at<0>::type;
		using unpack = int[];
    	unpack { (out << (std::is_same<Tags, first>::value? "{":", ") << t.template get<Tags>(), 0)... };
    	return out << "}"; 
	}
	
	std::ostream & operator << (std::ostream & out, named_tuple<> const &)
	{
		return out << "{}";
	}

}} //tagsql # development
