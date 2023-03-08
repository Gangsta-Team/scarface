#ifndef _COMMON_HPP
#define _COMMON_HPP

#include <Windows.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <cstdarg>
#include <mutex>
#include <map>
#include <vector>
#include <d3d9.h>
#include <format>
#include <filesystem>

#include <imgui.h>
#include <MinHook.h>

#include <torque3d/CodeBlock.h>
#include <scripting/RegisteredMethods.hpp>
#include <radobjects.hpp>
#include <gameobject/render/todobject.hpp>
#include <radkey.hpp>
#include <streaming/chunkfile.hpp>
#include <gameobject/camera/cameramanager.hpp>
#include <gameobject/character/characterobject.hpp>
#include <gameobject/cvmanager.hpp>

inline std::string gUsingPackage = "";

#endif