#ifndef _LINESHAPE_H_
#define _LINESHAPE_H_

#include "Polygon.h"
#include "TriangleList.h"

namespace ExLibris
{

	class LineShape
	{

	public:
	
		LineShape();
		~LineShape();

		size_t GetPolygonCount() const;

		void Clear();
		void AddPolygon(const Polygon& a_Polygon);

		TriangleList* Triangulate(float a_Thickness) const;

	private:

		std::vector<Polygon> m_Polygons;

	}; // class LineShape

}; // namespace ExLibris

#endif