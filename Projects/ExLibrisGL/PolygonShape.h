#ifndef _POLYGONSHAPE_H_
#define _POLYGONSHAPE_H_

#include "Polygon.h"
#include "TriangleList.h"

namespace ExLibris
{

	class PolygonShape
	{
	
	public:
	
		PolygonShape();
		~PolygonShape();
	
		void AddShape(const Polygon& a_Shape);

		TriangleList* Triangulate() const;

	private:

		std::vector<Polygon> m_Polygons;
	
	}; // class PolygonShape

}; // namespace ExLibris

#endif