#pragma once

#include "UtilityCore.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "BoundingBox.h"

#include "Mesh.h"

namespace Hazard {

	enum MeshLoaderFlags : uint32_t 
	{
		MeshLoaderFlags_CalculateTangentSpace = BIT(0),
		MeshLoaderFlags_JoinIdenticalVertices = BIT(1),
		MeshLoaderFlags_MakeLeftHanded = BIT(2),
		MeshLoaderFlags_Triangulate = BIT(3),
		MeshLoaderFlags_RemoveComponent = BIT(4),
		MeshLoaderFlags_GenerateNormals = BIT(5),
		MeshLoaderFlags_GenerateSmoothNormals = BIT(6),
		MeshLoaderFlags_SplitLargeMeshes = BIT(7),
		MeshLoaderFlags_PreTransformVertices = BIT(8),
		MeshLoaderFlags_LimitBoneWeights = BIT(9),
		MeshLoaderFlags_ValidateDataStructure = BIT(10),
		MeshLoaderFlags_ImproveCacheLocality = BIT(11),
		MeshLoaderFlags_RemoveReduntantMaterials = BIT(12),
		MeshLoaderFlags_FixInFacingNormals = BIT(13),
		MeshLoaderFlags_PopulateArmatureData = BIT(14),
		MeshLoaderFlags_SortByType = BIT(15),
		MeshLoaderFlags_FindDegenerates = BIT(16),
		MeshLoaderFlags_FindInvalidData = BIT(17),
		MeshLoaderFlags_GenerateUVCoords = BIT(18),
		MeshLoaderFlags_TransformUVCoords = BIT(19),
		MeshLoaderFlags_FindInstances = BIT(20),
		MeshLoaderFlags_OptimizeMeshes = BIT(21),
		MeshLoaderFlags_OptimizeGraph = BIT(22),
		MeshLoaderFlags_FlipUVs = BIT(23),
		MeshLoaderFlags_FlipWindingOrderToCW = BIT(24),
		MeshLoaderFlags_SplitByBoneCount = BIT(25),
		MeshLoaderFlags_Debone = BIT(26),
		MeshLoaderFlags_GlobalScale = BIT(27),
		MeshLoaderFlags_EmbedTextures = BIT(28),
		MeshLoaderFlags_ForceGenerateNormals = BIT(29),
		MeshLoaderFlags_DropNormals = BIT(30),
		MeshLoaderFlags_GenerateBoundingBoxes = BIT(31),

		MeshLoaderFlags_DefaultOptimization = MeshLoaderFlags_JoinIdenticalVertices |
		MeshLoaderFlags_GenerateSmoothNormals | MeshLoaderFlags_Triangulate |
		MeshLoaderFlags_GenerateUVCoords | MeshLoaderFlags_OptimizeMeshes |
		MeshLoaderFlags_GenerateBoundingBoxes
	};

	struct MeshData 
	{
		uint32_t Flags = 0;
		uint32_t VertexIndex = 0, BaseIndex = 0;
		BoundingBox BoundingBox;
		std::vector<SubMesh> SubMeshes;

		std::vector<Vertex3D> Vertices;
		std::vector<uint32_t> Indices;
	};


	struct MeshCreateInfo
	{
		std::string FilePath;
	};
}