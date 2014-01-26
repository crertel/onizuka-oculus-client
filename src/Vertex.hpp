
#pragma once


// Vertex type; same format is used for all shapes for simplicity.
// Shapes are built by adding vertices to Model.
struct Vertex
{
	float pos[3];
	float uv[2];
	float normal[3];
};