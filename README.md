TagSQL
========

TagSQL enables C++ program to interact with PostGreSQL databases. It has two parts:

 - The headers which programmers include in their programs (implicitly). 
 - The code generator which generates schemas and other code, to interface with specific database. The generated code include the required headers from the other part of this module. The programmers just need to include **one** header in their programs to interface with the database.


###Objectives

- **Type-safety** &mdash; unlike most C++ SQL interfaces, this library is type-safe, which means an integer column of a table in database becomes an `int` field of a struct representing the table in C++, not `char*`; the programmers dont have to worry about parsings various strings, such as when reading results or when querying.

- **Semantic checks and constraint checks at compile time** &mdash; the semantic and constraints of queries are checked at compile-time itself. That means, incorrect queries will fail to compile, providing good error messages for diagnostics.

- **SQL-like syntax** &mdash; the library implements embedded domain-specific language (EDSL) which makes C++ queries look like SQL query. So C++ programmers familiar with SQL will be able to write C++ queries effortlessly.

- **Deferred execution** &mdash; the queries are executed on demand. If you don't don't iterate over the results, the query is never executed.

- **Easy programming because of its intuitive syntax and semantic.** &mdash; awesome programming experience.

###Getting started

Without further ado, lets write first a simple SQL query, and see how it gets translated into C++:

     SELECT name, age
     FROM person
     WHERE qualification = 'BA'
     LIMIT 100;

The same query is translated into C++ as:

     auto items = context.select(person.name, person.age)             //SELECT name, age
                         .from(person)                                //FROM person
                         .where(person.qualification == "BA")         //WHERE qualification = 'BA'
                         .limit(100);                                 //LIMIT 100;

and then the `items` can be iterated over in many different ways.

 - **Index-based Access** &mdash; you can use index to access the values:
      
          for(auto const & item : items)
          {
              std::string name = item.at<0>();
              int age = item.at<1>();
              //...
          }

    But index-based access is less preferable, because it is order-sensitive &mdash; if you change the order of tags in the `select` clause, you have to change the order all over the places where you access the values. That is problematic. So this is not recommended, unless it really helps you, such as in generic code.

 - **Tag-based Access** &mdash; you can use tags themselves to access the corresponding values:

          for(auto const & item : items)
          {
              std::string name = item[person.name];
              int age = item[persom.age];
              //...
          }

     It is a bit better than index-based access.

 - **Member-based Access** &mdash; you can access them as member variable **(recommended)**.

          for(auto const & item : items)
          {
              std::string name = item.name;
              int age = item.age;
              //...
          }
          
###Functionalities

    
Since the semantic of functionalities are already known (SQL is well-known), I would rather describe syntax with examples.

 - __select__
   
        auto items =  dc.select(author.name, author.age, book.title, review.comment)
                        .from(author)
                        .join(book).on(author.author_id == book.author_id)
                        .join(review).on(review.book_id == book.book_id)
                        .where(author.name.like("S%"))
                        .limit(100);

        for(auto const & item : items)
        {
               std::string name   = item.name;
               int         age    = item.age;
               std::string title  = item.title;
               std::string commnt = item.comment;
               //code
        }
        
 - __insert__
  
        //sql-like syntax
        dc.insert_into(author)(author.name,author.age).values("C++", 11).values("C#", 12).values("Java", 13)

        //sql-like syntax (inspired from update syntax)
        dc.insert_into(author).set(author.name = "C++", author.age = 10).set(author.age = 100, author.name = "Java")
        
        //c++ container syntax
        author_t new_author { "Sarfaraz Nawaz", 30 };
        dc.insert(new_author);
        
        
 - __update__
 
        dc.update(author).set(author.age = 110, author.name="S Nawaz").where(author.author_id == 1);



###Code Generation


The code generator is in `$/tagsql/codegen`. So first of all, switch to codegen directory, then do this:

     [sarfaraz@~/projects/tagsql/codegen]$ ./codegen
     Usage:
             ./codegen <config.cfg>
     [sarfaraz@~/projects/tagsql/codegen]$ ./codegen test.cfg
     ensuring '/home/sarfaraz/mycode/mysqldb/fest/' exists (create if it doesn't) ... ok
     Generating C++ keys for database table 'book' ... ok
     Generating C++ keys for database table 'author' ... ok
     Generating C++ keys for database table 'review' ... ok
     Generating C++ meta_table for database table 'book' ... ok
     Generating C++ meta_table for database table 'author' ... ok
     Generating C++ meta_table for database table 'review' ... ok
     Generating C++ table for database table 'book' ... ok
     Generating C++ table for database table 'author' ... ok
     Generating C++ table for database table 'review' ... ok
     Generating C++ tags for database table 'book' ... ok
     Generating C++ tags for database table 'author' ... ok
     Generating C++ tags for database table 'review' ... ok
     Generating C++ tags_impl for database table 'book' ... ok
     Generating C++ tags_impl for database table 'author' ... ok
     Generating C++ tags_impl for database table 'review' ... ok
     
     ==================== Summary ====================
     + {test,localhost,5432,snawaz,itsnotme}
     + triloq::postgres
     + {type=std::string, header=string}
     + {type=long int, header=None}
     + {type=std::string, header=string}
     + {type=short, header=None}
     
As you can see, `codegen` is the code generator which takes a config file as command line. So when you run it, it generates code and dump them in a directory as specified in the config file. On the standard output, it also prints the progress as to what is being generated and all. *(Currently, the summary is nonsense because we've not worked on that part, we'll surely work on that and print a nice summary.)*


####Config file

The config file looks like this:

    dbspace   = "fest"
    namespace = "mycompany::databases";

    source_path    = "/home/sarfaraz/mycode/mysqldb";
    include_path   = "mycode/mysqldb"; //include_path must be suffix of source_path

    connection =
    {
	       dbname = "test";
	       host = "localhost";
	       port = 5432;
	       user = "snawaz";
	       password = "itsnotme";
    };

    type_mapping = 
    {
	       character*   = { type = "std::string"; header = "string"; }
	       smallint     = { type = "short"; }
	       bigint       = { type = "long int"; }
	       integer      = { type = "int"; }
	       name         = { type = "std::string"; header = "string"; }
	       timestamp*   = { type = "std::string"; header = "string"; }
	       uuid         = { type = "std::string"; header = "string"; }
    }

As you can see, there are **only 6 variables** expected by the codegen:


- **dbspace** ... should be the name of database but I suspect a database name could be an invalid C++ identifier, so choose a valid C++ identifier for this variable. For example, if the database name is "google places", `dbspace` could be `google_places` (note that `google places` is an invalid C++ identifier). This variable gets appended to *namespace*, *sourch_path*, and *include_path* variables. 

- **namespace** ... is the top level namespace. 

- **sourch_path** ... is the directory where a new directory with name *dbspace* will be created wherein all the generated files will go.

- **include_path** ... is the suffix of *sourch_path* which is used by `#include` in the generated files.

- **connection** ... contains the database related information. It essentially answers "which database to connect to? and using what credentials?". Thats all.

- **type_mapping** ... contains all the sql-type to c++-type mapping. It also mentions the header. Note that for the built-in types, header doesn't make sense, so no need to mention.


Note that any other variable is ignored by the codegen.

**It is advised to experiment with these variables to fully understand how they refect in the generated code. Code generation is an easy step, so generating more than once, with different variables, wouldn't be an issue for understanding purpose.**

The rest can be understood from examples.

