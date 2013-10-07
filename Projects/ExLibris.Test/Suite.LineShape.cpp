#include "ExLibris.Test.PCH.h"

#include <LineShape.h>

#include "Mock.MeshVisitor.h"

using namespace ExLibris;

TEST(LineShape, Construct)
{
	LineShape shape;

	EXPECT_EQ(0, shape.GetPolygonCount());
}

TEST(LineShape, AddPolygon)
{
	LineShape shape;

	Polygon p;
	shape.AddPolygon(p);

	EXPECT_EQ(1, shape.GetPolygonCount());
}

TEST(LineShape, ClearPolygons)
{
	LineShape shape;

	Polygon p;
	shape.AddPolygon(p);

	shape.Clear();

	EXPECT_EQ(0, shape.GetPolygonCount());
}

TEST(LineShape, OutlineStraightLine)
{
	LineShape shape;

	Polygon p;
	p += glm::vec2(-50.0f, 25.0f);
	p += glm::vec2(150.0f, 25.0f);

	shape.AddPolygon(p);

	LineMeshOptions options;
	options.quality = LineMeshOptions::eQuality_Fast;
	options.thickness = 2.0f;

	MeshBuilder* builder = shape.BuildOutlineMesh(options);
	ASSERT_EQ(6, builder->GetVertexCount());

	MockMeshVisitor visitor;
	builder->Accept(visitor);

	ASSERT_EQ(2, visitor.triangles.size());

	EXPECT_VEC2_EQ(150.0f, 24.0f, visitor.triangles[0].a);
	EXPECT_VEC2_EQ(-50.0f, 24.0f, visitor.triangles[0].b);
	EXPECT_VEC2_EQ(-50.0f, 26.0f, visitor.triangles[0].c);

	EXPECT_VEC2_EQ(150.0f, 24.0f, visitor.triangles[1].a);
	EXPECT_VEC2_EQ(-50.0f, 26.0f, visitor.triangles[1].b);
	EXPECT_VEC2_EQ(150.0f, 26.0f, visitor.triangles[1].c);
}

TEST(LineShape, OutlineCornerUp)
{
	LineShape shape;

	Polygon p;
	p += glm::vec2(0.0f, 100.0f);
	p += glm::vec2(50.0f, 100.0f);
	p += glm::vec2(50.0f, 50.0f);

	shape.AddPolygon(p);

	LineMeshOptions options;
	options.quality = LineMeshOptions::eQuality_Fast;
	options.thickness = 2.0f;

	MeshBuilder* builder = shape.BuildOutlineMesh(options);
	ASSERT_EQ(12, builder->GetVertexCount());

	MockMeshVisitor visitor;
	builder->Accept(visitor);

	ASSERT_EQ(4, visitor.triangles.size());

	EXPECT_VEC2_EQ(50.0f, 99.0f, visitor.triangles[0].a);
	EXPECT_VEC2_EQ(0.0f, 99.0f, visitor.triangles[0].b);
	EXPECT_VEC2_EQ(0.0f, 101.0f, visitor.triangles[0].c);

	EXPECT_VEC2_EQ(50.0f, 99.0f, visitor.triangles[1].a);
	EXPECT_VEC2_EQ(0.0f, 101.0f, visitor.triangles[1].b);
	EXPECT_VEC2_EQ(50.0f, 101.0f, visitor.triangles[1].c);

	EXPECT_VEC2_EQ(49.0f, 50.0f, visitor.triangles[2].a);
	EXPECT_VEC2_EQ(49.0f, 100.0f, visitor.triangles[2].b);
	EXPECT_VEC2_EQ(51.0f, 100.0f, visitor.triangles[2].c);

	EXPECT_VEC2_EQ(49.0f, 50.0f, visitor.triangles[3].a);
	EXPECT_VEC2_EQ(51.0f, 100.0f, visitor.triangles[3].b);
	EXPECT_VEC2_EQ(51.0f, 50.0f, visitor.triangles[3].c);
}