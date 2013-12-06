#include "ExLibris.Test.PCH.h"

#include "MeshBuilder.h"
#include "Mock.MeshVisitor.h"

using namespace ExLibris;

TEST(MeshBuilder, Construct)
{
	MeshBuilder builder;

	EXPECT_EQ(0, builder.GetVertexCount());
}

TEST(MeshBuilder, AddTriangle)
{
	MeshBuilder builder;
	builder.AddTriangle(
		glm::vec2(56.0f, 12.8f),
		glm::vec2(33.45f, 12.9f),
		glm::vec2(54.9f, 12.56f)
	);

	EXPECT_EQ(3, builder.GetVertexCount());
}

TEST(MeshBuilder, AddQuad)
{
	MeshBuilder builder;
	builder.AddQuad(
		glm::vec2(56.98f, 124.8f),
		glm::vec2(1.48f, 91.8f),
		glm::vec2(55.67f, 99.9f),
		glm::vec2(25.9f, -76.4f)
	);

	EXPECT_EQ(6, builder.GetVertexCount());
}

TEST(MeshBuilder, Clear)
{
	MeshBuilder builder;
	builder.AddTriangle(
		glm::vec2(-12.0f, 15.9f),
		glm::vec2(-12.9f, 19.8f),
		glm::vec2(-55.8f, 812.9f)
	);

	builder.Clear();

	EXPECT_EQ(0, builder.GetVertexCount());
}

TEST(MeshBuilder, BuildTriangle)
{
	MeshBuilder builder;
	builder.AddTriangle(
		glm::vec2(-25.0f, 45.0f),
		glm::vec2(33.9f, 16.7f),
		glm::vec2(125.3f, -76.9f)
	);

	MockMeshVisitor visitor;
	builder.Accept(visitor);

	EXPECT_EQ(3, visitor.vertex_count);
	ASSERT_EQ(1, visitor.triangles.size());
	EXPECT_VEC2_EQ(-25.0f, 45.0f, visitor.triangles[0].a);
	EXPECT_VEC2_EQ(33.9f, 16.7f, visitor.triangles[0].b);
	EXPECT_VEC2_EQ(125.3f, -76.9f, visitor.triangles[0].c);
}

TEST(MeshBuilder, BuildTwoTriangles)
{
	MeshBuilder builder;

	builder.AddTriangle(
		glm::vec2(-67.0f, 12.65f),
		glm::vec2(14.02f, 9.82f),
		glm::vec2(26.9f, 88.8f)
	);

	builder.AddTriangle(
		glm::vec2(25.9f, 76.9f),
		glm::vec2(56.8f, -76.9f),
		glm::vec2(29.8f, -71.3f)
	);

	MockMeshVisitor visitor;
	builder.Accept(visitor);

	EXPECT_EQ(6, visitor.vertex_count);
	ASSERT_EQ(2, visitor.triangles.size());

	EXPECT_VEC2_EQ(-67.0f, 12.65f, visitor.triangles[0].a);
	EXPECT_VEC2_EQ(14.02f, 9.82f, visitor.triangles[0].b);
	EXPECT_VEC2_EQ(26.9f, 88.8f, visitor.triangles[0].c);

	EXPECT_VEC2_EQ(25.9f, 76.9f, visitor.triangles[1].a);
	EXPECT_VEC2_EQ(56.8f, -76.9f, visitor.triangles[1].b);
	EXPECT_VEC2_EQ(29.8f, -71.3f, visitor.triangles[1].c);
}

TEST(MeshBuilder, BuildQuad)
{
	MeshBuilder builder;
	builder.AddQuad(
		glm::vec2(0.0f, 100.0f),
		glm::vec2(100.0f, 100.0f),
		glm::vec2(0.0f, 250.0f),
		glm::vec2(100.0f, 250.0f)
	);

	MockMeshVisitor visitor;
	builder.Accept(visitor);

	EXPECT_EQ(6, visitor.vertex_count);
	ASSERT_EQ(2, visitor.triangles.size());

	EXPECT_VEC2_EQ(100.0f, 100.0f, visitor.triangles[0].a);
	EXPECT_VEC2_EQ(0.0f, 100.0f, visitor.triangles[0].b);
	EXPECT_VEC2_EQ(0.0f, 250.0f, visitor.triangles[0].c);

	EXPECT_VEC2_EQ(100.0f, 100.0f, visitor.triangles[1].a);
	EXPECT_VEC2_EQ(0.0f, 250.0f, visitor.triangles[1].b);
	EXPECT_VEC2_EQ(100.0f, 250.0f, visitor.triangles[1].c);
}

TEST(MeshBuilder, BuildTwoQuads)
{
	MeshBuilder builder;

	builder.AddQuad(
		glm::vec2(55.0f, 88.0f),
		glm::vec2(345.0f, 76.0f),
		glm::vec2(76.0f, 214.0f),
		glm::vec2(415.0f, 345.0f)
	);

	builder.AddQuad(
		glm::vec2(345.0f, 76.0f),
		glm::vec2(516.0f, 76.0f),
		glm::vec2(865.0f, 345.0f),
		glm::vec2(912.0f, 345.0f)
	);

	MockMeshVisitor visitor;
	builder.Accept(visitor);

	EXPECT_EQ(12, visitor.vertex_count);
	ASSERT_EQ(4, visitor.triangles.size());

	EXPECT_VEC2_EQ(345.0f, 76.0f, visitor.triangles[0].a);
	EXPECT_VEC2_EQ(55.0f, 88.0f, visitor.triangles[0].b);
	EXPECT_VEC2_EQ(76.0f, 214.0f, visitor.triangles[0].c);

	EXPECT_VEC2_EQ(345.0f, 76.0f, visitor.triangles[1].a);
	EXPECT_VEC2_EQ(76.0f, 214.0f, visitor.triangles[1].b);
	EXPECT_VEC2_EQ(415.0f, 345.0f, visitor.triangles[1].c);

	EXPECT_VEC2_EQ(516.0f, 76.0f, visitor.triangles[2].a);
	EXPECT_VEC2_EQ(345.0f, 76.0f, visitor.triangles[2].b);
	EXPECT_VEC2_EQ(865.0f, 345.0f, visitor.triangles[2].c);

	EXPECT_VEC2_EQ(516.0f, 76.0f, visitor.triangles[3].a);
	EXPECT_VEC2_EQ(865.0f, 345.0f, visitor.triangles[3].b);
	EXPECT_VEC2_EQ(912.0f, 345.0f, visitor.triangles[3].c);
}

TEST(MeshBuilder, BuildEmpty)
{
	MeshBuilder builder;

	MockMeshVisitor visitor;
	builder.Accept(visitor);

	EXPECT_EQ(0, visitor.vertex_count);
	ASSERT_EQ(0, visitor.triangles.size());
}