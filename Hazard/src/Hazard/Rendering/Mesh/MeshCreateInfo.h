#pragma once

#include "UtilityCore.h"

#include "BoundingBox.h"
#include "Mesh.h"

#include <assimp/postprocess.h>

namespace Hazard {

	enum MeshLoaderFlags : uint64_t
	{
		MeshLoaderFlags_CalculateTangentSpace = aiProcess_CalcTangentSpace,
		MeshLoaderFlags_JoinIdenticalVertices = aiProcess_JoinIdenticalVertices,
		MeshLoaderFlags_MakeLeftHanded = aiProcess_MakeLeftHanded,
		MeshLoaderFlags_Triangulate = aiProcess_Triangulate,
		MeshLoaderFlags_RemoveComponent = aiProcess_RemoveComponent,
		MeshLoaderFlags_GenerateNormals = aiProcess_GenNormals,
		MeshLoaderFlags_GenerateSmoothNormals = aiProcess_GenSmoothNormals,
		MeshLoaderFlags_SplitLargeMeshes = aiProcess_SplitLargeMeshes,
		MeshLoaderFlags_PreTransformVertices = aiProcess_PreTransformVertices,
		MeshLoaderFlags_LimitBoneWeights = aiProcess_LimitBoneWeights,
		MeshLoaderFlags_ValidateDataStructure = aiProcess_ValidateDataStructure,
		MeshLoaderFlags_ImproveCacheLocality = aiProcess_ImproveCacheLocality,
		MeshLoaderFlags_RemoveReduntantMaterials = aiProcess_RemoveRedundantMaterials,
		MeshLoaderFlags_FixInFacingNormals = aiProcess_FixInfacingNormals,
		MeshLoaderFlags_PopulateArmatureData = aiProcess_PopulateArmatureData,
		MeshLoaderFlags_SortByType = aiProcess_SortByPType,
		MeshLoaderFlags_FindDegenerates = aiProcess_FindDegenerates,
		MeshLoaderFlags_FindInvalidData = aiProcess_FindInvalidData,
		MeshLoaderFlags_GenerateUVCoords = aiProcess_GenUVCoords,
		MeshLoaderFlags_TransformUVCoords = aiProcess_TransformUVCoords,
		MeshLoaderFlags_FindInstances = aiProcess_FindInstances,
		MeshLoaderFlags_OptimizeMeshes = aiProcess_OptimizeMeshes,
		MeshLoaderFlags_OptimizeGraph = aiProcess_OptimizeGraph,
		MeshLoaderFlags_FlipUVs = aiProcess_FlipUVs,
		MeshLoaderFlags_FlipWindingOrderToCW = aiProcess_FlipWindingOrder,
		MeshLoaderFlags_SplitByBoneCount = aiProcess_SplitByBoneCount,
		MeshLoaderFlags_Debone = aiProcess_Debone,
		MeshLoaderFlags_GlobalScale = aiProcess_GlobalScale,
		MeshLoaderFlags_EmbedTextures = aiProcess_EmbedTextures,
		MeshLoaderFlags_ForceGenerateNormals = aiProcess_ForceGenNormals,
		MeshLoaderFlags_DropNormals = aiProcess_DropNormals,
		MeshLoaderFlags_GenerateBoundingBoxes = aiProcess_GenBoundingBoxes,

		MeshLoaderFlags_DefaultFlags =
          MeshLoaderFlags_CalculateTangentSpace
		| MeshLoaderFlags_Triangulate 
		| MeshLoaderFlags_SortByType
		| MeshLoaderFlags_GenerateNormals
		| MeshLoaderFlags_GenerateUVCoords
		| MeshLoaderFlags_OptimizeMeshes
		| MeshLoaderFlags_JoinIdenticalVertices
		| MeshLoaderFlags_ValidateDataStructure
	};

	struct MeshData 
	{
		uint32_t Flags = 0;
		uint32_t VertexIndex = 0, BaseIndex = 0;
		BoundingBox BoundingBox = {};

		std::vector<SubMesh> SubMeshes;
		std::vector<Vertex3D> Vertices;
		std::vector<uint32_t> Indices;
	};
}
