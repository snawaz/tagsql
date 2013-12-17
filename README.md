TagSQL
======

TagSQL is not SQL &mdash; neither SQL server, nor SQL client.

TagSQL is **a code generator** which generates C++ client for a given PostgreSQL database. The generated C++ client is tag-based &mdash; what is meant by _tag-based_  is explained in detail in this document. 

###Features

- **Type-safety** &mdash; unlike most C++ SQL clients, the client generated by TagSQL is type-safe. That means, an integer column of a table in database becomes an `int` field of a struct representing the table in C++, not `char*`. No parsing is required by the programmers.

- **Semantic check at compile time** &mdash; the semantic of queries are checked at compile-time itself. That means, incorrect queries will fail to compile, providing good error messages for diagnostics.

- **SQL-like syntax** &mdash; the client is generated as embedded domain-specific language (EDSL) which makes C++ queries look like SQL query. So C++ programmers familiar with SQL will be able to write C++ queries effortlessly.

- **Tag-based** &mdash; there are tags all over, in the client code as well as in the framework. Tags are the building blocks of features like _type-safety_, _semantic-check_, _sql-like syntax_ and pretty much everything.

- **Deferred execution** &mdash; the queries are executed on demand. If you don't don't iterate over the results, the query is not executed.

- **Easy access** &mdash; the different columns of select queries can be accessed in many different ways.

###Getting started

Without further ado, lets write first a simple SQL query, and see how it gets translated into C++:

     SELECT name, age
     FROM person
     WHERE qualification = 'BA'
     LIMIT 100;

The same query is translated into C++ as:

     auto items = context.select(name, age)             //SELECT name, age
                         .from(person)                  //FROM person
                         .where(qualification == "BA")  //WHERE qualification = 'BA'
                         .limit(100);                   //LIMIT 100;

Now some quick questions:

- **What are `name`, `age` and `qualification` in the above query?** Well, they're tags! Tags are basically *objects* of specific types. `name` is an object of `name_t` type, `age` is an object of `age_t` type and so on. The programmers don't need to define them. They're defined in the library, ready for use.

- **Where did they come from?** They're generated, of course. All the tags of a particular table are defined in its own namespace &mdash; e.g tags for table `person` are defined in `person_tag` namespace.

- **What is `person`?** It is an object of type *person_t* representing a table in database. All the tables are defined in `schema` namespace.

So in the above query, `name`, `age` and `qualification` are tags corresponding to the respective columns of the `person` table. The tags know pretty much everything, as to _which column_ of _which table_ it corresponds to, what its _type_ is, whether it is a primary-key, foreign-key, nullable, has server-default or not, and so on.

Morever, if *name* and *age* columns of `person` is `string` and `int` respectively, then `items` (the result of the above query) is a container of tuples of `string` and `int`. So how do we access the values of the tuples? Well, there are many ways to access the values of each tuple. Here you go:

 - **Index-based Access** &mdash; you can use index to access the values:
      
          for(auto const & item : items)
          {
              std::string person_name = item.at<0>();
              int person_age = item.at<1>();
              //...
          }

    But index-based access is less preferable, because it is order-sensitive &mdash; if you change the order of tags in the `select` clause, you have to change the order all over the places where you access the values. That is problematic. So this is not recommended, unless it really helps you, such as in generic code.

 - **Tag-based Access** &mdash; you can use tags themselves to access the corresponding values:

          for(auto const & item : items)
          {
              std::string person_name = item[name];
              int person_age = item[age];
              //...
          }

     It is a bit better than index-based access.

 - **Member-based Access** &mdash; you can access them as member variable **(recommended)**.

          for(auto const & item : items)
          {
              std::string person_name = item.name;
              int person_age = item.age;
              //...
          }

###How to generate code?

Soon will be added. 
