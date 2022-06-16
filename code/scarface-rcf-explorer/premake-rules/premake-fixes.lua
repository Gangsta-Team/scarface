require('vstudio')
local p = premake
local api = p.api
local m = p.vstudio.vc2010

function myOutDir(cfg)
	m.element("OutDir", nil, "%s\\", p.vstudio.path(cfg, cfg.targetdir))
end

p.override(m.elements, "outputProperties", function(oldfn, cfg)
	return {
		myOutDir,
		m.intDir,
		m.targetName,
		m.targetExt
	}
end)

api.register {
	name = "librarydependencies",
	scope = "config",
	kind = "list:string",
	tokens = true,
	pathVars = true,
}

function libraryDependencies_fixed(cfg)
	if #cfg.librarydependencies > 0 then
		local libdeps = table.concat(cfg.librarydependencies, ";")
		m.element("LibraryDependencies", nil, "%s;$(StlLibraryName);%%(LibraryDependencies)", libdeps)
	end
end

p.override(m.elements, "link", function(oldfn, cfg, explicit)
	if cfg.kind == p.STATICLIB then
		return {
			m.subSystem,
			m.fullProgramDatabaseFile,
			m.generateDebugInformation,
			m.optimizeReferences,
		}
	else
		return {
			m.subSystem,
			m.fullProgramDatabaseFile,
			m.generateDebugInformation,
			m.optimizeReferences,
			m.additionalDependencies,
			m.additionalLibraryDirectories,
			libraryDependencies_fixed,
			m.entryPointSymbol,
			m.generateMapFile,
			m.moduleDefinitionFile,
			m.treatLinkerWarningAsErrors,
			m.ignoreDefaultLibraries,
			m.largeAddressAware,
			m.targetMachine,
			m.additionalLinkOptions,
			m.programDatabaseFile,
			m.assemblyDebug,
		}
	end
end)

api.register {
	name = "additionaldependencies",
	scope = "config",
	kind = "list:string",
	tokens = true,
	pathVars = true,
}

p.override(m, "additionalDependencies", function(oldfn, cfg)
	if #cfg.additionaldependencies > 0 then
		local deps = table.concat(cfg.additionaldependencies, ";")
		m.element("AdditionalDependencies", nil, "%s;%%(AdditionalDependencies)", deps)
	end
end)

m.categories.Library = {
	name = "Library",
	extensions = { ".so", ".a" },
	priority = 1,

	emitFiles = function(prj, group)
		m.emitFiles(prj, group, "Library", {m.generatedFile})
	end,

	emitFilter = function(prj, group)
		m.filterGroup(prj, group, "Library")
	end
}

p.override(m, "runtimeLibrary", function(oldfn, cfg)
	if cfg.system ~= premake.ANDROID then
		local runtimes = {
			StaticDebug = "MultiThreadedDebug",
			StaticRelease = "MultiThreaded",
			SharedDebug = "MultiThreadedDebugDLL",
			SharedRelease = "MultiThreadedDLL"
		}
		
		local runtime = p.config.getruntime(cfg)
		if runtime then
			m.element("RuntimeLibrary", nil, runtimes[runtime])
		end
	end
end)
