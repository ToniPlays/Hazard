#pragma once

#include "UtilityCore.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace HazardRenderer {

	enum class MeshFlags {
		CalculateTangentSpace = BIT(0),
		JoinIdenticalVertices = BIT(1),
		MakeLeftHanded = BIT(2),
		Triangulate = BIT(3),
		RemoveComponent = BIT(4),
		GenerateNormals = BIT(5),
		GenerateSmoothNormals = BIT(6),
		SplitLargeMeshes = BIT(7),
		PreTransformVertices = BIT(8),
		LimitBoneWeights = BIT(9),
		ValidateDataStructure = BIT(10),
		ImproveCacheLocality = BIT(11),
		RemoveReduntantMaterials = BIT(12),
		FixInFacingNormals = BIT(13),
		PopulateArmatureData = BIT(14),
		SortByType = BIT(15),
		FindDegenerates = BIT(16),
		FindInvalidData = BIT(17),
		GenerateUVCoords = BIT(18),
		TransformUVCoords = BIT(19),
		FindInstances = BIT(20),
		OptimizeMeshes = BIT(21),
		OptimizeGraph = BIT(22),
		FlipUVs = BIT(23),
		FlipWindingOrderToCW = BIT(24),
		SplitByBoneCount = BIT(25),
		Debone = BIT(26),
		GlobalScale = BIT(27),
		EmbedTextures = BIT(28),
		ForceGenerateNormals = BIT(29),
		DropNormals = BIT(30),
		GenerateBoundingBoxes = BIT(31)
	};


	struct MeshCreateInfo
	{
		std::string FilePath;
	};
}