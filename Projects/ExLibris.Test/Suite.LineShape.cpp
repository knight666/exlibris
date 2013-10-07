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

TEST(LineShape, OutlineStraightLineGapless)
{
	LineShape shape;

	Polygon p;
	p += glm::vec2(10.0f, -60.0f);
	p += glm::vec2(10.0f, 20.0f);

	shape.AddPolygon(p);

	LineMeshOptions options;
	options.quality = LineMeshOptions::eQuality_Gapless;
	options.thickness = 10.0f;

	MeshBuilder* builder = shape.BuildOutlineMesh(options);
	ASSERT_EQ(6, builder->GetVertexCount());

	MockMeshVisitor visitor;
	builder->Accept(visitor);

	ASSERT_EQ(2, visitor.triangles.size());

	EXPECT_VEC2_EQ(15.0f, 20.0f, visitor.triangles[0].a);
	EXPECT_VEC2_EQ(15.0f, -60.0f, visitor.triangles[0].b);
	EXPECT_VEC2_EQ(5.0f, -60.0f, visitor.triangles[0].c);

	EXPECT_VEC2_EQ(15.0f, 20.0f, visitor.triangles[1].a);
	EXPECT_VEC2_EQ(5.0f, -60.0f, visitor.triangles[1].b);
	EXPECT_VEC2_EQ(5.0f, 20.0f, visitor.triangles[1].c);
}

TEST(LineShape, OutlineZigZag)
{
	LineShape shape;

	Polygon p;
	p += glm::vec2(0.0f, 100.0f);
	p += glm::vec2(100.0f, 100.0f);
	p += glm::vec2(100.0f, 200.0f);
	p += glm::vec2(200.0f, 200.0f);
	p += glm::vec2(200.0f, 100.0f);
	p += glm::vec2(300.0f, 100.0f);

	shape.AddPolygon(p);

	//                       
	// o ---- o      o ---- o
	//        |      |       
	//        |      |       
	//        o ---- o       
	//                        

	LineMeshOptions options;
	options.quality = LineMeshOptions::eQuality_Fast;
	options.thickness = 2.0f;

	MeshBuilder* builder = shape.BuildOutlineMesh(options);
	ASSERT_EQ(30, builder->GetVertexCount());

	MockMeshVisitor visitor;
	builder->Accept(visitor);

	ASSERT_EQ(10, visitor.triangles.size());

	EXPECT_VEC2_EQ(100.0f, 99.0f, visitor.triangles[0].a);
	EXPECT_VEC2_EQ(0.0f, 99.0f, visitor.triangles[0].b);
	EXPECT_VEC2_EQ(0.0f, 101.0f, visitor.triangles[0].c);
	EXPECT_VEC2_EQ(100.0f, 101.0f, visitor.triangles[1].c);

	EXPECT_VEC2_EQ(101.0f, 200.0f, visitor.triangles[2].a);
	EXPECT_VEC2_EQ(101.0f, 100.0f, visitor.triangles[2].b);
	EXPECT_VEC2_EQ(99.0f, 100.0f, visitor.triangles[2].c);
	EXPECT_VEC2_EQ(99.0f, 200.0f, visitor.triangles[3].c);

	EXPECT_VEC2_EQ(200.0f, 199.0f, visitor.triangles[4].a);
	EXPECT_VEC2_EQ(100.0f, 199.0f, visitor.triangles[4].b);
	EXPECT_VEC2_EQ(100.0f, 201.0f, visitor.triangles[4].c);
	EXPECT_VEC2_EQ(200.0f, 201.0f, visitor.triangles[5].c);

	EXPECT_VEC2_EQ(199.0f, 100.0f, visitor.triangles[6].a);
	EXPECT_VEC2_EQ(199.0f, 200.0f, visitor.triangles[6].b);
	EXPECT_VEC2_EQ(201.0f, 200.0f, visitor.triangles[6].c);
	EXPECT_VEC2_EQ(201.0f, 100.0f, visitor.triangles[7].c);

	EXPECT_VEC2_EQ(300.0f, 99.0f, visitor.triangles[8].a);
	EXPECT_VEC2_EQ(200.0f, 99.0f, visitor.triangles[8].b);
	EXPECT_VEC2_EQ(200.0f, 101.0f, visitor.triangles[8].c);
	EXPECT_VEC2_EQ(300.0f, 101.0f, visitor.triangles[9].c);
}

TEST(LineShape, OutlineZigZagGapless)
{
	LineShape shape;

	Polygon p;
	p += glm::vec2(0.0f, 100.0f);
	p += glm::vec2(100.0f, 100.0f);
	p += glm::vec2(100.0f, 200.0f);
	p += glm::vec2(200.0f, 200.0f);
	p += glm::vec2(200.0f, 100.0f);
	p += glm::vec2(300.0f, 100.0f);

	shape.AddPolygon(p);

	//                       
	// o ---- o      o ---- o
	//        |      |       
	//        |      |       
	//        o ---- o       
	//                        

	LineMeshOptions options;
	options.quality = LineMeshOptions::eQuality_Gapless;
	options.thickness = 14.0f;

	MeshBuilder* builder = shape.BuildOutlineMesh(options);
	ASSERT_EQ(42, builder->GetVertexCount());

	MockMeshVisitor visitor;
	builder->Accept(visitor);

	ASSERT_EQ(14, visitor.triangles.size());

	EXPECT_VEC2_EQ(100.0f, 93.0f, visitor.triangles[0].a);
	EXPECT_VEC2_EQ(0.0f, 93.0f, visitor.triangles[0].b);
	EXPECT_VEC2_EQ(0.0f, 107.0f, visitor.triangles[0].c);
	EXPECT_VEC2_EQ(93.0f, 107.0f, visitor.triangles[1].c);

	EXPECT_VEC2_EQ(100.0f, 93.0f, visitor.triangles[2].a);
	EXPECT_VEC2_EQ(93.0f, 107.0f, visitor.triangles[2].b);
	EXPECT_VEC2_EQ(107.0f, 100.0f, visitor.triangles[2].c);

	EXPECT_VEC2_EQ(107.0f, 193.0f, visitor.triangles[3].a);
	EXPECT_VEC2_EQ(107.0f, 100.0f, visitor.triangles[3].b);
	EXPECT_VEC2_EQ(93.0f, 107.0f, visitor.triangles[3].c);
	EXPECT_VEC2_EQ(93.0f, 200.0f, visitor.triangles[4].c);

	EXPECT_VEC2_EQ(93.0f, 200.0f, visitor.triangles[5].a);
	EXPECT_VEC2_EQ(100.0f, 207.0f, visitor.triangles[5].b);
	EXPECT_VEC2_EQ(107.0f, 193.0f, visitor.triangles[5].c);

	EXPECT_VEC2_EQ(193.0f, 193.0f, visitor.triangles[6].a);
	EXPECT_VEC2_EQ(107.0f, 193.0f, visitor.triangles[6].b);
	EXPECT_VEC2_EQ(100.0f, 207.0f, visitor.triangles[6].c);
	EXPECT_VEC2_EQ(200.0f, 207.0f, visitor.triangles[7].c);

	EXPECT_VEC2_EQ(200.0f, 207.0f, visitor.triangles[8].a);
	EXPECT_VEC2_EQ(207.0f, 200.0f, visitor.triangles[8].b);
	EXPECT_VEC2_EQ(193.0f, 193.0f, visitor.triangles[8].c);

	EXPECT_VEC2_EQ(193.0f, 100.0f, visitor.triangles[9].a);
	EXPECT_VEC2_EQ(193.0f, 193.0f, visitor.triangles[9].b);
	EXPECT_VEC2_EQ(207.0f, 200.0f, visitor.triangles[9].c);
	EXPECT_VEC2_EQ(207.0f, 107.0f, visitor.triangles[10].c);

	EXPECT_VEC2_EQ(193.0f, 100.0f, visitor.triangles[11].a);
	EXPECT_VEC2_EQ(207.0f, 107.0f, visitor.triangles[11].b);
	EXPECT_VEC2_EQ(200.0f, 93.0f, visitor.triangles[11].c);

	EXPECT_VEC2_EQ(300.0f, 93.0f, visitor.triangles[12].a);
	EXPECT_VEC2_EQ(200.0f, 93.0f, visitor.triangles[12].b);
	EXPECT_VEC2_EQ(207.0f, 107.0f, visitor.triangles[12].c);
	EXPECT_VEC2_EQ(300.0f, 107.0f, visitor.triangles[13].c);
}