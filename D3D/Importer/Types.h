#pragma once

#include "stdafx.h"

struct asBone
{
	int Index;
	string Name;

	int Parent;
	Matrix Transform;
};

struct asMesh
{
	int BoneIndex;

	//Todo Material

	vector<SkeletalMesh::VertexSkeletalMesh > Vertices;
	vector<UINT> Indices;
};