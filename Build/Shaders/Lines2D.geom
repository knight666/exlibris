#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

void main()
{
	for (int triangle_index = 0; triangle_index < gl_in.length() / 3; ++triangle_index)
	{
		vec4 position_a = gl_in[(triangle_index * 3)    ].gl_Position;
		vec4 position_b = gl_in[(triangle_index * 3) + 1].gl_Position;
		vec4 position_c = gl_in[(triangle_index * 3) + 2].gl_Position;
		
		gl_Position = position_a;
		EmitVertex();
		
		gl_Position = position_b;
		EmitVertex();
		
		gl_Position = position_b;
		EmitVertex();
		
		gl_Position = position_c;
		EmitVertex();
		
		gl_Position = position_c;
		EmitVertex();
		
		gl_Position = position_a;
		EmitVertex();
	}
	
	EndPrimitive();
}