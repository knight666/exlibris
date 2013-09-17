#ifndef _TRIANGLELIST_H_
#define _TRIANGLELIST_H_

namespace ExLibris
{

	struct TriangleList
	{
	
		TriangleList()
			: vertex_count(0)
			, positions(nullptr)
		{
		}

		~TriangleList()
		{
			if (positions != nullptr)
			{
				delete [] positions;
				positions = nullptr;
			}
		}

		size_t vertex_count;
		glm::vec2* positions;
	
	}; // struct TriangleList

}; // namespace ExLibris

#endif