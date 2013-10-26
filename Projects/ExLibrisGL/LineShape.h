/*
 * This file is a part of the ExLibris project.
 *
 * Copyright (C) 2013 Quinten Lansu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */

#pragma once

#include <poly2tri/poly2tri.h>

#include "LineMeshOptions.h"
#include "MeshBuilder.h"
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

		MeshBuilder* BuildFilledMesh() const;
		MeshBuilder* BuildOutlineMesh(const LineMeshOptions& a_Options) const;

	private:

		bool _CheckPolylineOverlap(std::vector<p2t::Point*>& a_Polyline) const;
		void _TriangulateOutlineFast(MeshBuilder* a_Target, const Polygon& a_Polygon, float a_Thickness) const;
		void _TriangulateOutlineGapless(MeshBuilder* a_Target, const Polygon& a_Polygon, float a_Thickness) const;

	private:

		std::vector<Polygon> m_Polygons;

	}; // class LineShape

}; // namespace ExLibris