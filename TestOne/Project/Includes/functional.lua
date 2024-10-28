
premake.modules.lua = {}
local m = premake.modules.lua

local p = premake

print("Optioning")

newoption {
	trigger = "define",
	value = "def",
	description = "Define preprocessors"
}

newoption {
   trigger     = "gfxapi",
   value       = "API",
   description = "Choose a particular 3D API for rendering",
}

function get_keys(t)
  local keys={}
  for key,_ in pairs(t) do
    table.insert(keys, key)
  end
  return keys
end

return m