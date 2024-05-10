#pragma once

#define MAX_BONE_COUNT 250

class SkeletalMesh
{
public:
	struct VertexSkeletalMesh
	{
		Vector3 Position;
		Vector2 Uv;
		Vector3 Normal;
		Vector3 Tangent;
		Vector4 BlendIndices;
		Vector4 BlendWeights;

		VertexSkeletalMesh()
		{
			Position = Vector3(0, 0, 0);
			Uv = Vector2(0, 0);
			Normal = Vector3(0, 0, 0);
			Tangent = Vector3(0, 0, 0);
			BlendIndices = Vector4(0, 0, 0, 0);
			BlendWeights = Vector4(0, 0, 0, 0);
		}
	};
};