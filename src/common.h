#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <vector>
#include <queue>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>

#include "../include/imgui/imgui.h"
#include "../include/imgui/backends/imgui_impl_glfw.h"
#include "../include/imgui/backends/imgui_impl_opengl3.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef float f64;

#include "math.h"
#include "model.h"
#include "node.h"
#include "scene.h"
#include "window.h"
#include "camera.h"
#include "editor.h"
#include "player.h"
#include "render.h"

extern Scene* cur_scene;

#endif
