#if 0
#include "glm/glm.hpp"

#include <string>
#include <unordered_map>

static std::unordered_map<std::string, void*> functions;


static void DoSomething(glm::vec4 color, float multiplier) {
	
}

static void RegisterCall(const char* name, const void* method) {
	functions[name] = &method;
}
static void CallMethod(const std::string& name, void** params) {
	void* function = functions[name];
	//function(params);

}

int main() {

	glm::vec4 color(1.0f);
	float val = 0.4f;

	void* params[] = {&color, &val};

	RegisterCall("DoSomething", (void*)DoSomething);
	CallMethod("DoSomething", params);
}
#endif