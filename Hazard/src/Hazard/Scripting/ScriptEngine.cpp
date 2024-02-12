
#include <hzrpch.h>
#include "ScriptEngine.h"
#include "InternalCalls.h"
#include "Hazard/Assets/AssetManager.h"
#include "Attributes/AttributeConstructor.h"

#define TYPEDEF(x, y) if (strcmp(##name, x) == 0) return y;

using namespace HazardScript;

namespace Hazard
{
	ScriptEngine::ScriptEngine(ScriptEngineCreateInfo* info) : Module("ScriptEngine")
	{
		m_Info = *info;
	}
	void ScriptEngine::PreInit()
	{
		HZR_PROFILE_FUNCTION();

		HazardScriptCreateInfo createInfo = {
			.CoreAssemblyPath = m_Info.CoreAssemblyPath,
			.AppAssemblyPath = m_Info.AppAssemblyPath,
			.CoralDirectory = m_Info.CoralDirectory,

			.DebugCallback = [&](ScriptMessage message) {
				if (m_MessageCallback.Count() == 0)
				{
					m_QueuedMessages.push_back(message);
					return;
				}

				for (auto& m : m_QueuedMessages)
					m_MessageCallback.Invoke<ScriptMessage&>(m);

				m_QueuedMessages.clear();
				m_MessageCallback.Invoke<ScriptMessage&>(message);
			},
			.BindingCallback = [&](Ref<ScriptAssembly> assembly) {
				if (!m_ScriptGlue.contains(assembly.Raw())) return;

				for (auto& glue : m_ScriptGlue[assembly.Raw()])
				{
					glue->OnAssemblyLoaded(assembly);
					glue->Register(assembly);
				}
				assembly->UploadInternalCalls();
			},
		};

		AttributeConstructor::Init();

		m_Engine = HazardScriptEngine::Create(&createInfo);
		Ref<ScriptAssembly> coreAssembly = m_Engine->GetLoadedAssembly("HazardScripting");

		RegisterScriptGlueFor<InternalCalls>(coreAssembly);
	}
	void ScriptEngine::Update()
	{
		HZR_PROFILE_FUNCTION();
	}

	bool ScriptEngine::FindModule(const std::string& moduleName)
	{
		for (auto& assembly : m_Engine->GetAssemblies())
		{
			if (assembly->HasScript(moduleName))
				return true;
		}
		return false;
	}

	ScriptMetadata ScriptEngine::GetScript(const std::string& moduleName)
	{
		for (auto& assembly : m_Engine->GetAssemblies())
		{
			if (assembly->HasScript(moduleName))
				return assembly->GetScript(moduleName);
		}
		return ScriptMetadata();
	}

	void ScriptEngine::SendDebugMessage(const ScriptMessage& message)
	{
		HZR_CORE_INFO(message.Message);
		m_Engine->SendDebugMessage(message);
	}
	void ScriptEngine::ReloadAssemblies()
	{
		Application::Get().SubmitMainThread([&]() {

			for (auto& [uid, ctx] : m_WorldContext)
			{
				//Destroy all instances
				auto view = ctx->GetEntitiesWith<ScriptComponent>();
				for (auto& e : view)
				{
					Entity entity(e, ctx.Raw());
					entity.GetComponent<ScriptComponent>().m_Handle = nullptr;
				}
			}

			m_Engine->Reload();

			for (auto& [uid, ctx] : m_WorldContext)
			{
				//Destroy all instances
				auto view = ctx->GetEntitiesWith<ScriptComponent>();
				for (auto& e : view)
				{
					Entity entity(e, ctx.Raw());
					InitializeComponent(ctx, entity);
				}
			}
		});
	}

	void ScriptEngine::AddDebugCallback(ScriptMessageCallback callback)
	{
		m_MessageCallback.Add(callback);

		for (auto& m : m_QueuedMessages)
			m_MessageCallback.Invoke<ScriptMessage&>(m);

		m_QueuedMessages.clear();
	}

	void ScriptEngine::InitializeComponent(Ref<World> targetWorld, const Entity& entity)
	{
		HZR_PROFILE_FUNCTION();
		auto& component = entity.GetComponent<ScriptComponent>();

		if (component.ModuleName == "") return;
		if (!FindModule(component.ModuleName)) return;

		ScriptMetadata script = GetScript(component.ModuleName);
		component.m_Handle = script.CreateObject<uint64_t>((uint64_t)entity.GetUID());

		m_Instances[entity.GetUID()] = targetWorld->GetHandle();
		m_WorldContext[targetWorld->GetHandle()] = targetWorld;
	}
	void ScriptEngine::RemoveComponent(Ref<World> targetWorld, const Entity& entity)
	{
		m_Instances.erase(entity.GetUID());
	}
	const std::vector<Ref<ScriptAssembly>>& ScriptEngine::GetAssemblies()
	{
		return m_Engine->GetAssemblies();
	}
}
