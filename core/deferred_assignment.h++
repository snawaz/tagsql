

#pragma once

namespace tagsql { namespace core
{
	template<typename Column, typename ArgType>
	struct deferred_assignment
	{
	public:
		
		using _is_named_arg = std::true_type;
		using column_type = Column;
		using value_type  = typename Column::value_type;
		using argument_type = ArgType;

		template<typename T>
		deferred_assignment(Column * column, T && arg) 
			: _value(std::forward<T>(arg)), _column(column), _commit(true) 
		{}
		~deferred_assignment()
		{
			commit();
		}
		deferred_assignment(deferred_assignment const &) = delete;
		deferred_assignment& operator =(deferred_assignment const &) = delete;
		deferred_assignment& operator =(deferred_assignment &&) = delete;

		deferred_assignment(deferred_assignment && other) : _value(std::move(other._value)), _column(other._column), _commit(other._commit) 
		{
			other._commit = false;
		}
		operator column_type & ()
		{
			commit();
			return *_column;
		}
		operator value_type & ()
		{
			commit();
			return _column->value();
		}
	private:
		void commit()
		{
			if ( _commit )
			{
				_column->set(std::move(_value));
				_commit = false;
			}
		}
	private:

		template<typename DataContext, typename Table>
		friend struct insert_query;
		
		template<typename DataContext, typename Table>
		friend struct update_query;

		mutable argument_type  _value;  //mutable is used to enable move from this object (TO-DO : verify it! )
		column_type *          _column;
		mutable bool           _commit; 
	};

}} //tagsql # core
