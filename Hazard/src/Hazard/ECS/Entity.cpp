
#include <hzrpch.h>
#include "Entity.h"

namespace Hazard
{
	Entity::Entity(entt::entity handle, World* world) : m_Handle(handle), m_World(world) {}

	bool Entity::IsValid()
	{
		if (!m_World)
			return false;

		return m_World->GetWorldRegistry().valid(m_Handle);
	}

	Entity Entity::GetParent() const
	{
		auto& rsc = GetComponent<RelationshipComponent>();
		return m_World->TryGetEntityFromUID(rsc.ParentHandle);
	}

	void Entity::SetParent(Entity parent)
	{
		Entity currentParent = GetParent();
		if (parent == currentParent)
			return;

		if (currentParent)
			currentParent.RemoveChild(*this);

		auto& rsc = GetComponent<RelationshipComponent>();
		rsc.ParentHandle = parent.IsValid() ? parent.GetUID() : UID(0);

		if (!parent)
			return;

		UID id = GetUID();
		auto& parentChilds = parent.GetComponent<RelationshipComponent>().ChildHandles;

		if (std::find(parentChilds.begin(), parentChilds.end(), id) == parentChilds.end())
			parentChilds.push_back(GetUID());
	}

	bool Entity::RemoveChild(const Entity& child)
	{
		auto& rsc = GetComponent<RelationshipComponent>();
		auto& childHandles = rsc.ChildHandles;

		auto it = std::find(childHandles.begin(), childHandles.end(), child.GetUID());
		if (it != childHandles.end())
		{
			childHandles.erase(it);
			return true;
		}

		return false;
	}
	void Entity::UpdateWorldTransforms() const
	{
		HZR_PROFILE_FUNCTION();
		HZR_TIMED_FUNCTION();
		auto& tc = GetTransform();

		Entity parent = GetParent();
		glm::mat4 result = GetTransform().GetTransformMat4();

		while (parent)
		{
			result = parent.GetTransform().GetTransformMat4() * result;
			parent = parent.GetParent();
		}
		tc.WorldTransform = result;
	}
}
