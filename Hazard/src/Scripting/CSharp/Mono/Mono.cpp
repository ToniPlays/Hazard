
#include <hzrpch.h>
#include "Mono.h"

#include "../ScriptUtils.h"
#include "Scripting/CSharp/ScriptBindings.h"
#include "../AttributeBuilder.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/mono-gc.h>

namespace Hazard::Scripting::CSharp {

	MonoData Mono::s_Data;

	static std::vector<Attribute*> CreateAttribs(MonoCustomAttrInfo* info) 
	{
		std::vector<Attribute*> result;
		result.reserve(info->num_attrs);

		for (int i = 0; i < info->num_attrs; i++) {
			MonoCustomAttrEntry entry = info->attrs[i];

			MonoClass* a = Mono::GetMethodClass(entry.ctor);
			MonoObject* obj = mono_custom_attrs_get_attr(info, a);
			MonoClass* attribClass = mono_object_get_class(obj);

			Attribute* attrib = AttributeBuilder::Create(Mono::ClassName(attribClass), obj);
			result.push_back(attrib);
		}
		return result;
	}


	void Mono::InitAssembly(ScriptEngineCreateInfo* info)
	{
		s_Data.monoCoreAssemblyPath = info->CoreAssemblyPath;
		mono_set_dirs(info->MonoDirectoryLib.c_str(), info->MonoDirectoryEtc.c_str());
	}
	void Mono::CreateDomain(const char* name)
	{
		std::stringstream ss;
		ss << name << "-runtime";
		MonoDomain* domain = mono_jit_init(ss.str().c_str());

		s_Data.mono_domain = mono_domain_create_appdomain((char*)ss.str().c_str(), nullptr);
	}
	void Mono::LoadRuntimeAssembly(const char* path)
	{
		if (!File::Exists(path)) 
		{
			HZR_THROW("Runtime assembly does not exist " + std::string(path));
		}
		LoadMonoAssebly(path);
		ScriptBindings::Init();
	}
	void Mono::Shutdown()
	{
		if (s_Data.mono_domain == nullptr) return;
	}
	uint32_t Mono::InstantiateHandle(MonoClass* monoClass)
	{
		MonoObject* obj = mono_object_new(s_Data.mono_domain, monoClass);
		mono_runtime_object_init(obj);
		return mono_gchandle_new(obj, false);
	}
	MonoObject* Mono::CallMethod(MonoObject* object, MonoMethod* method, void** params)
	{
		MonoObject* exception = nullptr;
		MonoObject* result = mono_runtime_invoke(method, object, params, &exception);
		if (exception) {
			MonoClass* exceptionClass = Mono::GetClassFromObject(exception);
			MonoType* type = mono_class_get_type(exceptionClass);
			const char* typeName = Mono::GetTypeName(type);
			std::string message = Mono::GetStringProperty("Message", exceptionClass, result);
			std::string stackTrace = Mono::GetStringProperty("StackTrace", exceptionClass, result);
			
			HZR_CORE_ERROR("[Mono]: {0} from {1}", message, stackTrace);
		}
		return result;
	}
	bool Mono::ModuleExists(const char* name)
	{
		if (strcmp(name, "") == 0) return false;

		std::string nameSpace, ClassName;
		ScriptUtils::GetNames(name, nameSpace, ClassName);
		return GetMonoClass(nameSpace.c_str(), ClassName.c_str()) != nullptr;
	}
	MonoObject* Mono::ObjectFromHandle(uint32_t handle)
	{
		MonoObject* obj = mono_gchandle_get_target(handle);
		HZR_CORE_ASSERT(obj, "Object null for handle {0}", handle);
		return obj;
	}
	void Mono::FreeHandle(uint32_t handle)
	{
		mono_gchandle_free(handle);
	}
	MonoMethod* Mono::GetCoreMethod(const std::string& name)
	{
		MonoMethodDesc* desc = mono_method_desc_new(name.c_str(), NULL);
		MonoMethod* method = mono_method_desc_search_in_image(desc, s_Data.core_image);
		HZR_CORE_ASSERT((method != nullptr), "Method {0} not found", name);
		return method;
	}
	MonoMethod* Mono::GetAppMethod(const std::string& name)
	{
		MonoMethodDesc* desc = mono_method_desc_new(name.c_str(), NULL);
		MonoMethod* method = mono_method_desc_search_in_image(desc, s_Data.app_image);

		return method;
	}
	const char* Mono::GetMethodFullName(MonoMethod* method)
	{
		return mono_method_full_name(method, 1);
	}
	MonoClass* Mono::GetMonoClass(const char* moduleName)
	{
		if (strcmp(moduleName, "") == 0) return nullptr;

		std::string nameSpace, name;
		ScriptUtils::GetNames(moduleName, nameSpace, name);
		return GetMonoClass(nameSpace.c_str(), name.c_str());
	}
	MonoClass* Mono::GetMonoClass(const char* nameSpace, const char* name)
	{
		return mono_class_from_name(s_Data.app_image, nameSpace, name);
	}
	MonoClass* Mono::GetClassFromObject(MonoObject* object)
	{
		return mono_object_get_class(object);
	}
	const char* Mono::ClassName(MonoClass* monoClass)
	{
		return mono_class_get_name(monoClass);
	}
	std::vector<Attribute*> Mono::GetClassAttributes(const std::string& moduleName)
	{
		MonoClass* monoClass = GetMonoClass(moduleName.c_str());
		MonoCustomAttrInfo* info = mono_custom_attrs_from_class(monoClass);

		if (info == nullptr) return std::vector<Attribute*>();

		return CreateAttribs(info);
	}
	MonoClass* Mono::GetMethodClass(MonoMethod* method)
	{
		return mono_method_get_class(method);
	}
	uint32_t Mono::ClassMethodCount(MonoClass* monoClass)
	{
		return mono_class_num_methods(monoClass);
	}
	MonoMethod* Mono::GetClassMethod(MonoClass* monoClass, void** iter)
	{
		return mono_class_get_methods(monoClass, iter);
	}
	std::vector<ScriptMethodMetadata> Mono::GetClassMethods(MonoClass* monoClass)
	{
		MonoMethod* monoMethod = nullptr;
		void* ptr = nullptr;

		std::vector<ScriptMethodMetadata> result;
		result.reserve(ClassMethodCount(monoClass));

		while ((monoMethod = GetClassMethod(monoClass, &ptr)))
		{
			ScriptMethodMetadata method;
			method.Name = GetMethodFullName(monoMethod);
			method.Visibility = GetVisibility(monoMethod);
			method.MethodAttributes = GetMethodAttributes(monoMethod);
			result.push_back(method);
		}
		return result;
	}
	std::vector<Attribute*> Mono::GetMethodAttributes(MonoMethod* monoMethod)
	{

		MonoCustomAttrInfo* info = mono_custom_attrs_from_method(monoMethod);
		if (info == nullptr) return std::vector<Attribute*>();

		return CreateAttribs(info);
	}
	std::vector<MonoClassField*> Mono::GetClassFields(MonoClass* monoClass)
	{
		std::vector<MonoClassField*> result;

		MonoClassField* field;
		void* ptr = 0;

		while ((field = GetMonoField(monoClass, &ptr))) 
			result.push_back(field);
		return result;
	}
	MonoClassField* Mono::GetField(MonoClass* monoClass, const std::string& name)
	{
		return mono_class_get_field_from_name(monoClass, name.c_str());
	}
	std::vector<ScriptFieldMetadata> Mono::GetFields(MonoClass* monoClass)
	{
		std::vector<MonoClassField*> fields = GetClassFields(monoClass);
		std::vector<ScriptFieldMetadata> result;

		for (auto* monoField : fields) {
			ScriptFieldMetadata field;
			field.Name = GetFieldName(monoField);
			field.Type = ScriptUtils::GetFieldType(monoField);
			field.Visibility = GetVisibility(monoField);
			field.FieldAttributes = GetFieldAttributes(monoField);

			result.push_back(field);
		}

		return result;
	}
	std::vector<Attribute*> Mono::GetFieldAttributes(MonoClassField* field)
	{
		MonoClass* monoClass = mono_field_get_parent(field);
		MonoCustomAttrInfo* info = mono_custom_attrs_from_field(monoClass, field);
		if (info == nullptr) return std::vector<Attribute*>();

		return CreateAttribs(info);
	}
	void Mono::GetFieldValue(MonoObject* object, MonoClassField* field, void* buffer)
	{
		mono_field_get_value(object, field, buffer);
	}
	void Mono::SetFieldValue(MonoObject* object, MonoClassField* field, void* buffer)
	{
		mono_field_set_value(object, field, buffer);
	}
	std::string Mono::GetFieldName(MonoClassField* field)
	{
		return mono_field_get_name(field);
	}
	MonoType* Mono::GetFieldType(MonoClassField* field)
	{
		return mono_field_get_type(field);
	}
	MonoClassField* Mono::GetMonoField(MonoClass* monoClass, void** iter)
	{
		return mono_class_get_fields(monoClass, iter);
	}
	void* Mono::GetArrayValuePointer(MonoArray* monoArray)
	{
		uint32_t size = mono_array_length(monoArray);
		return mono_array_addr_with_size(monoArray, size, 0);
	}
	int Mono::GetType(MonoType* type)
	{
		return mono_type_get_type(type);
	}
	char* Mono::GetTypeName(MonoType* type)
	{
		return mono_type_get_name(type);
	}
	std::string Mono::GetStringProperty(const char* name, MonoClass* monoClass, MonoObject* obj)
	{
		MonoProperty* property = mono_class_get_property_from_name(monoClass, name);
		MonoMethod* getter = mono_property_get_get_method(property);
		MonoString* result = (MonoString*)mono_runtime_invoke(getter, obj, NULL, NULL);
		return result != nullptr ? ScriptUtils::MonoStringToString(result) : "";
	}
	MonoType* Mono::ReflectionToType(void* type)
	{
		return mono_reflection_type_get_type((MonoReflectionType*)type);
	}
	MonoType* Mono::TypeFromReflectionName(const std::string& name)
	{
		return mono_reflection_type_from_name((char*)name.c_str(), s_Data.core_image);
	}
	FieldVisibility Mono::GetVisibility(MonoClassField* field)
	{
		uint32_t flags = mono_field_get_flags(field);

		if (flags & MONO_FIELD_ATTR_PUBLIC)
			return FieldVisibility::Public;
		if (flags & MONO_FIELD_ATTR_PRIVATE)
			return FieldVisibility::Private;

		return FieldVisibility::Protected;
	}
	FieldVisibility Mono::GetVisibility(MonoMethod* method)
	{
		uint32_t iFlags = 0;
		uint32_t flags = mono_method_get_flags(method, &iFlags);

		if (flags & MONO_FIELD_ATTR_PUBLIC)
			return FieldVisibility::Public;
		if (flags & MONO_FIELD_ATTR_PRIVATE)
			return FieldVisibility::Private;

		return FieldVisibility::Protected;
	}
	void Mono::RunGarbageCollection()
	{
		Timer timer;
		mono_gc_collect(mono_gc_max_generation());
		using namespace std::chrono_literals;
		while (mono_gc_pending_finalizers()) 
		{
			std::this_thread::sleep_for(500ns);
		}
		HZR_CORE_INFO("Garbage collection took: {0}ms", timer.ElapsedMillis());
	}
	void Mono::LoadMonoAssebly(const char* path)
	{
		MonoDomain* domain = nullptr;
		bool cleanUp = false;

		if (s_Data.mono_domain) {
			domain = mono_domain_create_appdomain("Hazard-runtime", nullptr);
			mono_domain_set(domain, false);
			cleanUp = true;
		}
		s_Data.core_assembly = LoadAssembly(s_Data.monoCoreAssemblyPath.c_str());
		s_Data.core_image = GetAssemblyImage(s_Data.core_assembly);

		HZR_CORE_ERROR("Loading App core from {}", path);

		auto appAssembly = LoadAssembly(path);
		auto appAssemblyImage = GetAssemblyImage(appAssembly);
		
		if (cleanUp) {
			s_Data.mono_domain = domain;
		}
		s_Data.app_assembly = appAssembly;
		s_Data.app_image = appAssemblyImage;
	}
	MonoAssembly* Mono::LoadAssembly(const char* path)
	{
		Buffer data = File::ReadBinaryFile(std::filesystem::path(path));

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full((char*)data.Data, (uint32_t)data.Size, 1, &status, 0);
		MonoAssembly* loaded = mono_assembly_load_from_full(image, path, &status, 0);
		mono_image_close(image);
		return loaded;
	}
	MonoImage* Mono::GetAssemblyImage(MonoAssembly* assembly)
	{
		HZR_CORE_ASSERT((assembly != nullptr), "Cannot read nullptr MonoAssembly image");
		MonoImage* image = mono_assembly_get_image(assembly);
		return image;
	}
}
