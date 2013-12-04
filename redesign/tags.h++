

#pragma once


namespace tagsql { namespace redesign 
{
	namespace author_tag
	{
		struct author_id_t
		{

		};
	}

	namespace tags
	{
		struct author_t
		{
			author_tag::author_id_t  author_id;
			author_tag::name_t       name;
			author_tag::age_t        age;
			author_tag::modified_t   modified;
			author_tag::created_t    created;
		};

		static const author_t author{};
	}
}}
