#ifndef __COMMON_H__
#define __COMMON_H__

#ifndef WIN32
#include <windows.h>
#endif

#include <memory>
#include <vector>
#include <string>
#include <optional>
#include <glad/glad.h>  
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>

/*
#define CLASS_PTR(klassName) \                          // macro function
class klassName; \                                      // define class  
using klassName ## UPtr = std::unique_ptr<klassName>; \ // role of '\': connect other lines
using klassName ## Ptr = std::shared_ptr<klassName>; \  // what is ##? klassName+Uptr = klassNameUPtr
using klassName ## WPtr = std::weak_ptr<klassName>;

--- I dont know why this code is not working. Maybe some invisible characters or '\' make this code not working
*/
#define CLASS_PTR(klassName) \
class klassName; \
using klassName##UPtr = std::unique_ptr<klassName>; \
using klassName##Ptr = std::shared_ptr<klassName>; \
using klassName##WPtr = std::weak_ptr<klassName>;





/* for example,
CLASS_PTR(Shader);
class Shader;
using ShaderUptr = std::unique_ptr<Shader>;     // --> own memory block uniquely
using ShaderPtr = std::shared_ptr<Shader>;      // --> share memory block 
using ShaderWptr = std::weak_ptr<Shader>;       // --> don't own, but accessible       


*/

std::optional<std::string> LoadTextFile(const std::string& filename);

#endif      // __COMMON_H__