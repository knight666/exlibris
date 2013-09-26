#include "ExLibris.Test.PCH.h"

#include "MeshBuilder.h"
#include "Mock.MeshVisitor.h"

using namespace ExLibris;

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