#ifndef __BLACK_H__
#define __BLACK_H__

#include <iostream>
#include <stdexcept>

#include <SDL.h>
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define WARN printf
#define LOG printf

#include "HashedString.h"
#include "GraphicsContext.h"
#include "ObjectManager.h"
#include "GrowableString.h"
#include "Shader.h"
#include "Utils.h"
#include "IRenderable.h"
#include "Texture.h"
#include "FileImageTexture.h"
#include "GameObject.h"
#include "FrameBuffer.h"

#endif // __BLACK_H__
