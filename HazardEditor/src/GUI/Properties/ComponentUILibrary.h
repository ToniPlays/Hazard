#pragma once

#include <imgui.h>
#include "Hazard.h"
#include "Hazard/Scripting/HScript.h"
#include "ScriptFieldUI.h"
#include "Hazard/ImGUI/UIElements/TextField.h"

#include "Editor/EditorUtils.h"

#include <vector>

namespace UI
{
	constexpr float colWidth = 125.0f;

	using namespace Hazard;
	template<typename T>
	bool ComponentMenu(std::vector<Entity>& entities)
	{
		STATIC_ASSERT(false, T);
	}

	template<typename T>
	bool ComponentMenuIfExists(std::vector<Entity>& entities)
	{
		if (AllEntitiesContain<T>(entities))
			return ComponentMenu<T>(entities);
		return false;
	}

	template<>
	bool ComponentMenu<TagComponent>(std::vector<Entity>& entities)
	{
		std::string& tag = entities[0].GetTag().Tag;
		bool mixed = false;

		for (auto& entity : entities)
		{
			if (entity.GetTag().Tag != tag)
			{
				mixed = true;
				break;
			}
		}

		ImUI::TextField tagField(tag);
		tagField.Render();

		if (tagField.DidChange())
		{
			for (auto& entity : entities)
				entity.GetTag().Tag = tagField.GetValue();
		}
		return false;
	}
	template<>
	bool ComponentMenu<TransformComponent>(std::vector<Entity>& entities)
	{
		ImUI::TreenodeWithOptions((const char*)(" " ICON_FK_MAP_MARKER " Transform"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {

			ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 6.0f));
			auto& firstTc = entities[0].GetComponent<TransformComponent>();

			const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

			uint32_t flags = 0;
			glm::vec3 translation = firstTc.GetTranslation();
			glm::vec3 rotation = firstTc.GetRotationEuler();
			glm::vec3 scale = firstTc.GetScale();

			for (uint32_t i = 1; i < entities.size(); i++)
			{
				auto& tc = entities[i].GetComponent<TransformComponent>();
				auto& t = tc.GetTranslation();
				auto r = glm::eulerAngles(tc.GetRotation());
				auto& s = tc.GetScale();

				flags |= (t.x != translation.x) ? BIT(0) : 0;
				flags |= (t.y != translation.y) ? BIT(1) : 0;
				flags |= (t.z != translation.z) ? BIT(2) : 0;

				flags |= (r.x != rotation.x) ? BIT(3) : 0;
				flags |= (r.y != rotation.y) ? BIT(4) : 0;
				flags |= (r.z != rotation.z) ? BIT(5) : 0;

				flags |= (s.x != scale.x) ? BIT(6) : 0;
				flags |= (s.y != scale.y) ? BIT(7) : 0;
				flags |= (s.z != scale.z) ? BIT(8) : 0;
			}

			rotation = glm::degrees(rotation);

			ImUI::InputFloat translationInput("Translation", 3);
			translationInput.ConfigureField(0, "X", style.Colors.AxisX, flags & BIT(0));
			translationInput.ConfigureField(1, "Y", style.Colors.AxisY, flags & BIT(1));
			translationInput.ConfigureField(2, "Z", style.Colors.AxisZ, flags & BIT(2));
			translationInput.SetFieldValue(0, translation.x, 0.0f);
			translationInput.SetFieldValue(1, translation.y, 0.0f);
			translationInput.SetFieldValue(2, translation.z, 0.0f);
			translationInput.Render();

			if (translationInput.DidAnyChange())
			{
				for (auto& entity : entities)
				{
					auto& tc = entity.GetComponent<TransformComponent>();
					glm::vec3 position = tc.GetTranslation();

					position.x = translationInput.DidChange(0) ? translationInput.GetValue(0) : position.x;
					position.y = translationInput.DidChange(1) ? translationInput.GetValue(1) : position.y;
					position.z = translationInput.DidChange(2) ? translationInput.GetValue(2) : position.z;

					tc.SetTranslation(position);
				}
			}

			ImGui::Separator();

			ImUI::InputFloat rotationInput("Rotation", 3);
			rotationInput.ConfigureField(0, "X", style.Colors.AxisX, flags & BIT(3));
			rotationInput.ConfigureField(1, "Y", style.Colors.AxisY, flags & BIT(4));
			rotationInput.ConfigureField(2, "Z", style.Colors.AxisZ, flags & BIT(5));
			rotationInput.SetFieldValue(0, rotation.x, 0.0f);
			rotationInput.SetFieldValue(1, rotation.y, 0.0f);
			rotationInput.SetFieldValue(2, rotation.z, 0.0f);
			rotationInput.Render();

			if (rotationInput.DidAnyChange())
			{
				for (auto& entity : entities)
				{
					auto& tc = entity.GetComponent<TransformComponent>();
					glm::vec3 rot = tc.GetRotationEuler();

					rot.x = rotationInput.DidChange(0) ? glm::radians(rotationInput.GetValue(0)) : rot.x;
					rot.y = rotationInput.DidChange(1) ? glm::radians(rotationInput.GetValue(1)) : rot.y;
					rot.z = rotationInput.DidChange(2) ? glm::radians(rotationInput.GetValue(2)) : rot.z;

					tc.SetRotation(rot);
				}
			}

			ImGui::Separator();

			ImUI::InputFloat scaleInput("Scale", 3);
			scaleInput.ConfigureField(0, "X", style.Colors.AxisX, flags & BIT(6));
			scaleInput.ConfigureField(1, "Y", style.Colors.AxisY, flags & BIT(7));
			scaleInput.ConfigureField(2, "Z", style.Colors.AxisZ, flags & BIT(8));
			scaleInput.SetFieldValue(0, scale.x, 1.0f);
			scaleInput.SetFieldValue(1, scale.y, 1.0f);
			scaleInput.SetFieldValue(2, scale.z, 1.0f);
			scaleInput.Render();

			if (scaleInput.DidAnyChange())
			{
				for (auto& entity : entities)
				{
					auto& tc = entity.GetComponent<TransformComponent>();
					glm::vec3 sc = tc.GetScale();

					sc.x = scaleInput.GetValue(0) ? scaleInput.GetValue(0) : sc.x;
					sc.y = scaleInput.GetValue(1) ? scaleInput.GetValue(1) : sc.y;
					sc.z = scaleInput.GetValue(2) ? scaleInput.GetValue(2) : sc.z;

					tc.SetScale(sc);
				}
			}
			ImGui::Separator();

		}, [&]() {
			ImUI::MenuItem("Reset", [&]() {
				for (auto& entity : entities)
				{
					auto& c = entity.GetComponent<TransformComponent>();
					c.SetTranslation({ 0, 0, 0 });
					c.SetRotation({ 0, 0, 0 });
					c.SetScale({ 1, 1, 1 });
				}
			});
		});
		return false;
	}
	template<>
	bool ComponentMenu<SpriteRendererComponent>(std::vector<Entity>& entities)
	{
		bool removed = false;

		ImUI::TreenodeWithOptions((const char*)(" " ICON_FK_PICTURE_O " Sprite renderer"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
								  ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {

			auto& firstSr = entities[0].GetComponent<SpriteRendererComponent>();

			uint32_t flags = 0;
			AssetHandle textureHandle = firstSr.TextureHandle;
			const Color& color = firstSr.Color;

			for (auto& entity : entities)
			{
				auto& sr = entity.GetComponent<SpriteRendererComponent>();
				if (sr.TextureHandle != textureHandle)
				{
					textureHandle = INVALID_ASSET_HANDLE;
					flags |= BIT(0);
				}

				flags |= (sr.Color.r != color.r) ? BIT(1) : 0;
			}

			std::string path = "None";
			if (textureHandle != INVALID_ASSET_HANDLE)
				path = File::GetNameNoExt(AssetManager::GetMetadata(textureHandle).Key);
			else if (flags & BIT(0))
				path = "---";


			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, colWidth);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4, 8 });

			ImUI::TextureSlot("Sprite", path, AssetManager::GetAsset<Texture2DAsset>(textureHandle), [&]() {
				ImUI::DropTarget<AssetHandle>(AssetType::Image, [&](AssetHandle handle) {
					Application::Get().SubmitMainThread([=]() mutable {
						for (auto& entity : entities)
						{
							auto& c = entity.GetComponent<SpriteRendererComponent>();
							c.TextureHandle = handle;
						}
					});
				});
			});

			ImGui::PopStyleVar();

			Color c = flags & BIT(1) ? Color::White : color;

			if (ImUI::ColorPicker("Tint", "##Tint", c, flags & BIT(1)))
			{
				for (auto& entity : entities)
				{
					auto& sr = entity.GetComponent<SpriteRendererComponent>();
					sr.Color = c;
				}
			}
			ImGui::Columns();
		}, [&]() {
			ImUI::MenuItem("Reset", [&]() {

				for (auto& entity : entities)
				{
					auto& c = entity.GetComponent<SpriteRendererComponent>();
					c.TextureHandle = INVALID_ASSET_HANDLE;
					c.Color = Color::White;
				}
			});
			ImUI::MenuItem("Remove component", [&]() {
				removed = true;
			});
		}, [&]() {
		});

		if (removed)
			RemoveComponentFromAll<SpriteRendererComponent>(entities);

		return false;
	}
	template<>
	bool ComponentMenu<CameraComponent>(std::vector<Entity>& entities)
	{
		bool removed = false;
		ImUI::TreenodeWithOptions((const char*)(" " ICON_FK_VIDEO_CAMERA " Camera"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
								  ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {

			ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, colWidth);

			const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

			auto& firstCamera = entities[0].GetComponent<CameraComponent>();

			uint32_t selected = (uint32_t)firstCamera.GetProjectionType();
			glm::vec2 clipping = firstCamera.GetClipping();
			float fov = firstCamera.GetFov();
			float size = firstCamera.GetSize();
			uint32_t flags = 0;

			for (auto& entity : entities)
			{
				auto& c = entity.GetComponent<CameraComponent>();
				flags |= ((uint32_t)c.GetProjectionType() != selected) ? BIT(0) : 0;
				flags |= (c.GetClipping().x != clipping.x) ? BIT(1) : 0;
				flags |= (c.GetClipping().y != clipping.y) ? BIT(2) : 0;

				flags |= (c.GetFov() != fov) ? BIT(3) : 0;
				flags |= (c.GetSize() != size) ? BIT(4) : 0;
			}

			std::string projectionTypes[] = { "Perspective", "Orthographic" };
			//Projection type here
			if (ImUI::Combo("Projection", "##projection", projectionTypes, 2, selected, flags & BIT(0)))
			{
				for (auto& entity : entities)
				{
					auto& c = entity.GetComponent<CameraComponent>();
					c.SetProjection((Projection)selected);
				}
			}
			if (!(flags & BIT(0)))
			{
				if ((Projection)selected == Projection::Perspective)
				{
					if (false)
					{
						for (auto& entity : entities)
						{
							auto& cc = entity.GetComponent<CameraComponent>();
							cc.SetFov(fov);
						}
					}
				}
				else
				{
					if (false)
					{
						for (auto& entity : entities)
						{
							auto& cc = entity.GetComponent<CameraComponent>();
							cc.SetSize(size);
						}
					}
				}
			}

			ImGui::Columns();

			ImUI::InputFloat clipInput("Clipping", 2);
			clipInput.ConfigureField(0, "N", style.Colors.AxisY);
			clipInput.ConfigureField(1, "F", style.Colors.Warning);
			clipInput.SetFieldValue(0, clipping.x, 0.03f);
			clipInput.SetFieldValue(1, clipping.y, 1000.0f);
			clipInput.Render();

			if (clipInput.DidAnyChange())
			{
				for (auto& entity : entities)
				{
					auto& cc = entity.GetComponent<CameraComponent>();
					glm::vec2 c = cc.GetClipping();

					c.x = clipInput.DidChange(0) ? clipInput.GetValue(0) : c.x;
					c.y = clipInput.DidChange(1) ? clipInput.GetValue(1) : c.y;
					cc.SetClipping(c);
				}
			}

			ImGui::Columns();
		}, [&]() {
			ImUI::MenuItem("Reset", [&]() {
				for (auto& entity : entities)
				{
					auto& cc = entity.GetComponent<CameraComponent>();
					cc.SetProjection(Projection::Perspective);
					cc.SetFov(60.0f);
					cc.SetClipping({ 0.03, 100.0f });
				}
			});
			ImUI::MenuItem("Remove component", [&]() {
				removed = true;
			});
		}, [&]() {
			/*ImUI::DragSource("Hazard.CameraComponent", &e.GetUID(), [&]() {
				ImGui::Text("Camera component");
				ImGui::Text(std::to_string(e.GetUID()).c_str());
				});
				*/
		});

		if (removed)
			RemoveComponentFromAll<CameraComponent>(entities);

		return false;
	}
	template<>
	bool ComponentMenu<ScriptComponent>(std::vector<Entity>& entities)
	{

		bool removed = false;

		ImUI::TreenodeWithOptions((const char*)(" " ICON_FK_CODE " Script"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
								  ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {
			using namespace HazardScript;
			ImUI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

			std::string oldModule = "";
			std::string field = "";

			ScriptComponent& c = entities[0].GetComponent<ScriptComponent>();
			ScriptEngine& scriptEngine = Application::Get().GetModule<ScriptEngine>();

			bool changed = ImUI::TextFieldWithHint(c.ModuleName, "Script class");

			ImUI::DropTarget<AssetHandle>(AssetType::Script, [&](AssetHandle handle) {
				Ref<HScript> script = AssetManager::GetAsset<HScript>(handle);

				if (script)
				{
					c.ModuleName = script->GetModuleName();
					changed = true;
				}
			});


			if (changed)
			{
				if (scriptEngine.FindModule(oldModule) && c.m_Handle)
					c.m_Handle = nullptr;

				if (scriptEngine.FindModule(c.ModuleName))
				{
					ScriptMetadata script = scriptEngine.GetScript(c.ModuleName);
					uint64_t uid = entities[0].GetUID();
					c.m_Handle = script.CreateObject(uid);
				}
			}
			if (c.m_Handle)
			{
				//ScriptMetadata& script = c.m_Handle->GetScript();

				ImGui::Columns(2, 0, false);
				ImGui::SetColumnWidth(0, colWidth);

				auto world = Application::Get().GetModule<WorldHandler>().GetCurrentWorld();
				/*
				for (auto& [name, field] : script.GetFields())
				{
					const char* label = name.c_str();
					Ref<FieldMetadata> f = field;

					ImUI::Group(name.c_str(), [&]() {
						UI::ScriptField(label, f, c.m_Handle, world);
						ImUI::ShiftY(3.0f);
						ImGui::Separator();
						ImUI::ShiftY(2.0f);
						});
				}
				*/
				ImGui::Columns();
			}

		}, [&]() {
			ImUI::MenuItem("Reset", [&]() {
			});
			ImUI::MenuItem("Remove component", [&]() {
				removed = true;
			});
		}, [&]() {
			/*ImUI::DragSource("Hazard.ScriptComponent", &e.GetUID(), [&]() {
				ImGui::Text("Script component");
				ImGui::Text(std::to_string(e.GetUID()).c_str());
				});
				*/
		});

		if (removed)
			RemoveComponentFromAll<ScriptComponent>(entities);

		return false;
	}
	template<>
	bool ComponentMenu<SkyLightComponent>(std::vector<Entity>& entities)
	{

		bool removed = false;

		ImUI::TreenodeWithOptions((const char*)(" " ICON_FK_GLOBE " Sky light"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
								  ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {

			auto& firstDir = entities[0].GetComponent<SkyLightComponent>();
			AssetHandle mapHandle = firstDir.EnvironmentMapHandle;
			float intensity = firstDir.Intensity;
			float lodLevel = firstDir.LodLevel;
			uint32_t flags = 0;

			for (auto& entity : entities)
			{
				auto& sc = entity.GetComponent<SkyLightComponent>();
				flags |= (sc.EnvironmentMapHandle != mapHandle) ? BIT(0) : 0;
				flags |= (sc.Intensity != intensity) ? BIT(1) : 0;
				flags |= (sc.LodLevel != lodLevel) ? BIT(2) : 0;
			}

			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, colWidth);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4, 8 });

			ImGui::Text("Map");
			ImGui::NextColumn();

			std::string path = "None";
			if (mapHandle != INVALID_ASSET_HANDLE)
				path = AssetManager::GetMetadata(mapHandle).Key;
			else if (flags & BIT(0))
				path = "---";


			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImUI::TextFieldWithHint(path, "Source image"))
			{

			}
			ImGui::PopStyleVar();

			ImUI::DropTarget<AssetHandle>(AssetType::EnvironmentMap, [&](AssetHandle handle) {
				Application::Get().SubmitMainThread([handle, entities]() mutable {

					for (auto& entity : entities)
						entity.GetComponent<SkyLightComponent>().EnvironmentMapHandle = handle;
				});
			});

			ImGui::NextColumn();
			/*
			if (ImUI::InputFloatX("Lod level", lodLevel, 0.0f, 0.025f, 0.0f, 1.0f, (flags & BIT(1)) != 0))
			{
				for (auto& entity : entities)
					entity.GetComponent<SkyLightComponent>().LodLevel = lodLevel;
			}
			if (ImUI::InputFloatX("Intensity", intensity, 1.0f, 0.025f, 0.0f, 0.0f, (flags & BIT(2)) != 0))
			{
				for (auto& entity : entities)
					entity.GetComponent<SkyLightComponent>().Intensity = intensity;
			}
			*/
		}, [&]() {
			ImUI::MenuItem("Reset", [&]() {
				for (auto& entity : entities)
				{
					auto& slc = entity.GetComponent<SkyLightComponent>();
					slc.Intensity = 1.0f;
					slc.EnvironmentMapHandle = 0;
					slc.LodLevel = 0.0f;
				}

			});
			ImUI::MenuItem("Remove component", [&]() {
				removed = true;
			});
		}, [&]() {
			/*ImUI::DragSource("Hazard.SkyLightComponent", &e.GetUID(), [&]() {
				ImGui::Text("Sky light");
				ImGui::Text(std::to_string(e.GetUID()).c_str());
				});
				*/
		});
		if (removed)
			RemoveComponentFromAll<SkyLightComponent>(entities);

		return false;
	}
	template<>
	bool ComponentMenu<DirectionalLightComponent>(std::vector<Entity>& entities)
	{
		bool removed = false;

		ImUI::TreenodeWithOptions("Directional light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, colWidth);

			auto& firstDir = entities[0].GetComponent<DirectionalLightComponent>();
			Color color = firstDir.LightColor;
			float intensity = firstDir.Intensity;
			uint32_t flags = 0;

			for (auto& entity : entities)
			{
				auto& dir = entity.GetComponent<DirectionalLightComponent>();
				flags |= (dir.LightColor.r != color.r) ? BIT(0) : 0;
				flags |= (dir.LightColor.g != color.g) ? BIT(0) : 0;
				flags |= (dir.LightColor.b != color.b) ? BIT(0) : 0;
				flags |= (dir.LightColor.a != color.a) ? BIT(0) : 0;

				flags |= (dir.Intensity != intensity) ? BIT(1) : 0;
			}

			color = flags & BIT(0) ? Color::White : color;

			if (ImUI::ColorPicker("Color", "##Color", color, flags & BIT(0)))
			{
				for (auto& entity : entities)
					entity.GetComponent<DirectionalLightComponent>().LightColor = color;
			}
			/*
			if (ImUI::InputFloatX("Intensity", intensity, 1.0f, 0.025f, 0.0f, 0.0f, flags & BIT(1)))
			{
				for (auto& entity : entities)
					entity.GetComponent<DirectionalLightComponent>().Intensity = intensity;
			}
			*/
			ImGui::Columns();
		}, [&]() {
			ImUI::MenuItem("Reset", [&]() {
				for (auto& entity : entities)
				{
					auto& dir = entity.GetComponent<DirectionalLightComponent>();
					dir.Intensity = 1.0f;
					dir.LightColor = Color::White;
				}
			});
			ImUI::MenuItem("Remove component", [&]() {
				removed = true;
			});
		}, [&]() {
			/*ImUI::DragSource("Hazard.DirectionalLightComponent", &e.GetUID(), [&]() {
				ImGui::Text("Directional light");
				ImGui::Text(std::to_string(e.GetUID()).c_str());
				});
				*/
		});
		if (removed)
			RemoveComponentFromAll<DirectionalLightComponent>(entities);

		return false;
	}
	template<>
	bool ComponentMenu<PointLightComponent>(std::vector<Entity>& entities)
	{

		bool removed = false;

		ImUI::TreenodeWithOptions("Point light", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, colWidth);

			auto& firstDir = entities[0].GetComponent<PointLightComponent>();
			Color color = firstDir.LightColor;
			float intensity = firstDir.Intensity;
			uint32_t flags = 0;

			for (auto& entity : entities)
			{
				auto& dir = entity.GetComponent<PointLightComponent>();
				flags |= (dir.LightColor.r != color.r) ? BIT(0) : 0;
				flags |= (dir.LightColor.g != color.g) ? BIT(0) : 0;
				flags |= (dir.LightColor.b != color.b) ? BIT(0) : 0;
				flags |= (dir.LightColor.a != color.a) ? BIT(0) : 0;

				flags |= (dir.Intensity != intensity) ? BIT(1) : 0;
			}

			color = flags & BIT(0) ? Color::White : color;

			if (ImUI::ColorPicker("Color", "##Color", color, flags & BIT(0)))
			{
				for (auto& entity : entities)
					entity.GetComponent<PointLightComponent>().LightColor = color;
			}
			/*
			if (ImUI::InputFloatX("Intensity", intensity, 1.0f, 0.025f, 0.0f, 0.0f, flags & BIT(1)))
			{
				for (auto& entity : entities)
					entity.GetComponent<PointLightComponent>().Intensity = intensity;
			}
			*/
			ImGui::Columns();
		}, [&]() {
			ImUI::MenuItem("Reset", [&]() {
				for (auto& entity : entities)
				{
					auto& dir = entity.GetComponent<PointLightComponent>();
					dir.Intensity = 1.0f;
					dir.LightColor = Color::White;
				}
			});
			ImUI::MenuItem("Remove component", [&]() {
				removed = true;
			});
		}, [&]() {
			/*ImUI::DragSource("Hazard.PointLightComponent", &e.GetUID(), [&]() {
				ImGui::Text("Directional light");
				ImGui::Text(std::to_string(e.GetUID()).c_str());
				});
				*/
		});
		if (removed)
			RemoveComponentFromAll<PointLightComponent>(entities);

		return false;
	}
	template<>
	bool ComponentMenu<MeshComponent>(std::vector<Entity>& entities)
	{
		bool removed = false;

		ImUI::TreenodeWithOptions((const char*)(" " ICON_FK_CUBE " Mesh"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
								  ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {

			auto& firstMc = entities[0].GetComponent<MeshComponent>();

			uint32_t flags = 0;
			AssetHandle mesh = firstMc.MeshHandle;
			AssetHandle material = firstMc.MaterialHandle;

			for (auto& entity : entities)
			{
				auto& mc = entity.GetComponent<MeshComponent>();
				if (mc.MeshHandle != mesh)
				{
					mesh = INVALID_ASSET_HANDLE;
					flags |= BIT(0);
				}
				if (mc.MaterialHandle != material)
				{
					material = INVALID_ASSET_HANDLE;
					flags |= BIT(1);
				}
			}

			std::string meshName = "";
			std::string materialName = "";
			if (mesh)
				meshName = File::GetNameNoExt(AssetManager::GetMetadata(mesh).Key);
			else if (flags & BIT(0))
				meshName = "---";

			if (material)
				materialName = File::GetNameNoExt(AssetManager::GetMetadata(material).Key);
			else if (flags & BIT(1))
				materialName = "---";


			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, colWidth);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4, 8 });

			ImGui::Text("Mesh");
			ImGui::NextColumn();

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImUI::TextFieldWithHint(meshName, "Mesh file"))
			{

			}
			ImUI::DropTarget<AssetHandle>(AssetType::Mesh, [&](AssetHandle handle) {
				Application::Get().SubmitMainThread([handle, entities]() {
					for (auto entity : entities)
						entity.GetComponent<MeshComponent>().MeshHandle = handle;
				});
			});
			ImGui::NextColumn();
			ImGui::Text("Material");
			ImGui::NextColumn();
			ImGui::PushID("##materialInput");

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImUI::TextFieldWithHint(materialName, "Material"))
			{

			}
			ImUI::DropTarget<AssetHandle>(AssetType::Material, [&](AssetHandle handle) {
				Application::Get().SubmitMainThread([handle, entities]() {
					for (auto entity : entities)
						entity.GetComponent<MeshComponent>().MaterialHandle = handle;
				});
			});

			ImGui::PopStyleVar();
			ImGui::PopID();
			ImGui::Columns();
		}, [&]() {
			ImUI::MenuItem("Reset", [&]() {
				for (auto& entity : entities)
				{
					auto& c = entity.GetComponent<MeshComponent>();
					c.MeshHandle = INVALID_ASSET_HANDLE;
					c.MaterialHandle = INVALID_ASSET_HANDLE;
				}
			});
			ImUI::MenuItem("Remove component", [&]() {
				removed = true;
			});
		}, [&]() {

		});

		if (removed)
			RemoveComponentFromAll<MeshComponent>(entities);

		return false;
	}
	template<>
	bool ComponentMenu<Rigidbody2DComponent>(std::vector<Entity>& entities)
	{
		bool removed = false;

		ImUI::TreenodeWithOptions("Rigidbody 2D", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
								  ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {

		}, [&]() {
			ImUI::MenuItem("Reset", [&]() {
			});
			ImUI::MenuItem("Remove component", [&]() {
				removed = true;
			});
		}, [&]() {
			/*ImUI::DragSource("Hazard.Rigidbody2DComponent", &e.GetUID(), [&]() {
				ImGui::Text("Rigidbody 2D");
				ImGui::Text(std::to_string(e.GetUID()).c_str());
				});
				*/
		});
		if (removed)
			RemoveComponentFromAll<Rigidbody2DComponent>(entities);

		return false;
	}
	template<>
	bool ComponentMenu<BoxCollider2DComponent>(std::vector<Entity>& entities)
	{

		bool removed = false;

		ImUI::TreenodeWithOptions((const char*)(" " ICON_FK_CODEPEN " Box collider 2D"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
								  ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {


		}, [&]() {
			ImUI::MenuItem("Reset", [&]() {
			});
			ImUI::MenuItem("Remove component", [&]() {
				removed = true;
			});
		}, [&]() {
			/*ImUI::DragSource("Hazard.BoxCollider2DComponent", &e.GetUID(), [&]() {
				ImGui::Text("Box collider 2D");
				ImGui::Text(std::to_string(e.GetUID()).c_str());
				});
				*/
		});
		if (removed)
			RemoveComponentFromAll<BoxCollider2DComponent>(entities);

		return false;
	}
	template<>
	bool ComponentMenu<CircleCollider2DComponent>(std::vector<Entity>& entities)
	{

		bool removed = false;

		ImUI::TreenodeWithOptions((const char*)(" " ICON_FK_CIRCLE_O " Circle collider 2D"), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
								  ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, [&]() {
		}, [&]() {
			ImUI::MenuItem("Reset", [&]() {
			});
			ImUI::MenuItem("Remove component", [&]() {
				removed = true;
			});
		}, [&]() {
			/*ImUI::DragSource("Hazard.CircleCollider2DComponent", &e.GetUID(), [&]() {
				ImGui::Text("Circle collider 2D");
				ImGui::Text(std::to_string(e.GetUID()).c_str());
				});
				*/
		});
		if (removed)
			RemoveComponentFromAll<CircleCollider2DComponent>(entities);

		return false;
	}
}
