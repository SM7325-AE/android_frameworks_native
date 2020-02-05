/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <ui/GraphicTypes.h>
#include "../GLESRenderEngine.h"
#include "../GLFramebuffer.h"
#include "GenericProgram.h"

using namespace std;

namespace android {
namespace renderengine {
namespace gl {

class BlurFilter {
public:
    // Downsample FBO to improve performance
    static constexpr float kFboScale = 0.25f;
    // To avoid downscaling artifacts, we interpolate the blurred fbo with the full composited
    // image, up to this radius.
    static constexpr float kMaxCrossFadeRadius = 15.0f;

    explicit BlurFilter(GLESRenderEngine& engine);
    virtual ~BlurFilter(){};

    // Set up render targets, redirecting output to offscreen texture.
    status_t setAsDrawTarget(const DisplaySettings&, uint32_t radius);
    // Allocate any textures needed for the filter.
    virtual void allocateTextures() = 0;
    // Execute blur passes, rendering to offscreen texture.
    virtual status_t prepare() = 0;
    // Render blur to the bound framebuffer (screen).
    status_t render();

protected:
    uint32_t mRadius;
    void drawMesh(GLuint uv, GLuint position);
    string getVertexShader() const;

    GLESRenderEngine& mEngine;
    // Frame buffer holding the composited background.
    GLFramebuffer mCompositionFbo;
    // Frame buffer holding the blur result.
    GLFramebuffer mBlurredFbo;
    uint32_t mDisplayWidth;
    uint32_t mDisplayHeight;

private:
    string getMixFragShader() const;
    bool mTexturesAllocated = false;

    GenericProgram mMixProgram;
    GLuint mMPosLoc;
    GLuint mMUvLoc;
    GLuint mMMixLoc;
    GLuint mMTextureLoc;
    GLuint mMCompositionTextureLoc;
};

} // namespace gl
} // namespace renderengine
} // namespace android
