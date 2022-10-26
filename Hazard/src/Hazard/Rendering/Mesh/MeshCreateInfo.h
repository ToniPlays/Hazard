#pragma once

#include "UtilityCore.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

namespace Hazard {

	enum MeshFlags : uint32_t 
	{
		MeshFlags_CalculateTangentSpace = BIT(0),
		MeshFlags_JoinIdenticalVertices = BIT(1),
		MeshFlags_MakeLeftHanded = BIT(2),
		MeshFlags_Triangulate = BIT(3),
		MeshFlags_RemoveComponent = BIT(4),
		MeshFlags_GenerateNormals = BIT(5),
		MeshFlags_GenerateSmoothNormals = BIT(6),
		MeshFlags_SplitLargeMeshes = BIT(7),
		MeshFlags_PreTransformVertices = BIT(8),
		MeshFlags_LimitBoneWeights = BIT(9),
		MeshFlags_ValidateDataStructure = BIT(10),
		MeshFlags_ImproveCacheLocality = BIT(11),
		MeshFlags_RemoveReduntantMaterials = BIT(12),
		MeshFlags_FixInFacingNormals = BIT(13),
		MeshFlags_PopulateArmatureData = BIT(14),
		MeshFlags_SortByType = BIT(15),
		MeshFlags_FindDegenerates = BIT(16),
		MeshFlags_FindInvalidData = BIT(17),
		MeshFlags_GenerateUVCoords = BIT(18),
		MeshFlags_TransformUVCoords = BIT(19),
		MeshFlags_FindInstances = BIT(20),
		MeshFlags_OptimizeMeshes = BIT(21),
		MeshFlags_OptimizeGraph = BIT(22),
		MeshFlags_FlipUVs = BIT(23),
		MeshFlags_FlipWindingOrderToCW = BIT(24),
		MeshFlags_SplitByBoneCount = BIT(25),
		MeshFlags_Debone = BIT(26),
		MeshFlags_GlobalScale = BIT(27),
		MeshFlags_EmbedTextures = BIT(28),
		MeshFlags_ForceGenerateNormals = BIT(29),
		MeshFlags_DropNormals = BIT(30),
		MeshFlags_GenerateBoundingBoxes = BIT(31),

		MeshFlags_DefaultOptimization = MeshFlags_JoinIdenticalVertices | 
		MeshFlags_GenerateSmoothNormals | MeshFlags_Triangulate | 
		MeshFlags_GenerateUVCoords | MeshFlags_OptimizeMeshes | 
		MeshFlags_GenerateBoundingBoxes
	};

	struct MeshData {
		uint32_t vertexIndex = 0, baseIndex = 0;
		std::vector<SubMesh> subMeshes;

		std::vector<Vertex3D> vertices;
		std::vector<uint32_t> indices;
	};


	struct MeshCreateInfo
	{
		std::string FilePath;
	};
}