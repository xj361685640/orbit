// Copyright (c) 2020 The Orbit Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ORBIT_GL_TEXT_RENDERER_H_
#define ORBIT_GL_TEXT_RENDERER_H_

#include <GteVector.h>
#include <freetype-gl/mat4.h>
#include <freetype-gl/texture-atlas.h>
#include <freetype-gl/texture-font.h>
#include <freetype-gl/vec234.h>
#include <stddef.h>
#include <stdint.h>

#include <map>
#include <unordered_map>
#include <vector>

#include "Batcher.h"
#include "CoreMath.h"
#include "OpenGl.h"
#include "PickingManager.h"

namespace ftgl {
struct vertex_buffer_t;
struct texture_font_t;
}  // namespace ftgl

class GlCanvas;

class TextRenderer {
 public:
  explicit TextRenderer();
  ~TextRenderer();

  void Init();
  void Clear();
  void SetCanvas(GlCanvas* canvas) { canvas_ = canvas; }

  void RenderLayer(Batcher* batcher, float layer);
  void RenderDebug(Batcher* batcher);
  [[nodiscard]] std::vector<float> GetLayers() const;

  void AddText(const char* text, float x, float y, float z, const Color& color, uint32_t font_size,
               float max_size = -1.f, bool right_justified = false, Vec2* out_text_pos = nullptr,
               Vec2* out_text_size = nullptr);

  float AddTextTrailingCharsPrioritized(const char* text, float x, float y, float z,
                                        const Color& color, size_t trailing_chars_length,
                                        uint32_t font_size, float max_size);

  [[nodiscard]] float GetStringWidth(const char* text, uint32_t font_size);
  [[nodiscard]] float GetStringHeight(const char* text, uint32_t font_size);

  static void SetDrawOutline(bool value) { draw_outline_ = value; }

 protected:
  void AddTextInternal(texture_font_t* font, const char* text, const vec4& color, vec2* pen,
                       float max_size = -1.f, float z = -0.01f, vec2* out_text_pos = nullptr,
                       vec2* out_text_size = nullptr);

  void ToScreenSpace(float x, float y, float& o_x, float& o_y);
  [[nodiscard]] float ToScreenSpace(float width);
  [[nodiscard]] int GetStringWidthScreenSpace(const char* text, uint32_t font_size);
  [[nodiscard]] int GetStringHeightScreenSpace(const char* text, uint32_t font_size);
  [[nodiscard]] texture_font_t* GetFont(uint32_t size);
  [[nodiscard]] texture_glyph_t* MaybeLoadAndGetGlyph(texture_font_t* self, const char* character);

  void DrawOutline(Batcher* batcher, vertex_buffer_t* buffer);

 private:
  texture_atlas_t* texture_atlas_;
  // Indicates when a change to the texture atlas occurred so that we have to reupload the
  // texture data. Only freetype-gl's texture_font_load_glyph modifies the texture atlas,
  // so we need to set this to true when and only when we call that function.
  bool texture_atlas_changed_;
  std::unordered_map<float, vertex_buffer_t*> vertex_buffers_by_layer_;
  std::map<uint32_t, texture_font_t*> fonts_by_size_;
  GlCanvas* canvas_;
  GLuint shader_;
  mat4 model_;
  mat4 view_;
  mat4 projection_;
  vec2 pen_;
  bool initialized_;
  static bool draw_outline_;
};

inline vec4 ColorToVec4(const Color& color) {
  const float coeff = 1.f / 255.f;
  vec4 vec;
  vec.r = color[0] * coeff;
  vec.g = color[1] * coeff;
  vec.b = color[2] * coeff;
  vec.a = color[3] * coeff;
  return vec;
}

#endif  // ORBIT_GL_TEXT_RENDERER_H_
