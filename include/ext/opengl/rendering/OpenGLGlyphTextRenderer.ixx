/**
 * @file OpenGLGlyphTextRenderer.ixx
 * @brief OpenGL-based text renderer using FreeType glyph rendering.
 */
module;

#include <cassert>
#include <ft2build.h>
#include <glad/gl.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include FT_FREETYPE_H

export module helios.ext.opengl.rendering.OpenGLGlyphTextRenderer;

import helios.rendering.text.FontResourceProvider;
import helios.rendering.text.TextRenderer;
import helios.rendering.text.TextMesh;
import helios.rendering.text.Glyph;

import helios.math;
import helios.ext.opengl.rendering.shader.OpenGLShader;
import helios.ext.opengl.rendering.FreeTypeFontResourceManager;


import helios.rendering.shader.UniformSemantics;
import helios.engine.core.data.FontId;
import helios.rendering.text.TextRenderCommand;

import helios.rendering.shader.UniformValueMap;

export namespace helios::ext::opengl::rendering {

    /**
     * @brief Renders and manages text using OpenGL and FreeType.
     *
     * This class is responsible for rendering text using cached glyph textures and managing
     * OpenGL resources for efficient rendering. It leverages FreeType to rasterize font glyphs
     * and OpenGL to render them onto the screen. Text rendering is performed by positioning
     * quads (one per character) with texture-mapped glyphs.
     *
     * The class provides methods for initializing OpenGL resources, loading fonts into a cache,
     * and rendering text at specified positions and scales.
     *
     * Inherits from the TextRenderer class in the helios rendering system.
     */
    class OpenGLGlyphTextRenderer : public helios::rendering::text::TextRenderer {



        friend class OpenGLDevice;

        /**
         * @brief Initializes OpenGL resources (VAO and VBO) for text rendering.
         *
         * Creates a vertex array object and vertex buffer for rendering glyph quads.
         * The VBO is configured for dynamic updates (one quad per character).
         *
         * Must be called before any rendering operations.
         */
        void init() {
            glGenVertexArrays(1, &vao_);
            glGenBuffers(1, &vbo_);
            glBindVertexArray(vao_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            // 2D quad for a texture requires 6 vertices with 4 floats each => 6*4
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6*4, nullptr, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }




        /**
         * @brief OpenGL Vertex Array Object for glyph rendering.
         */
        unsigned int vao_{};

        /**
         * @brief OpenGL Vertex Buffer Object for glyph quad vertices.
         */
        unsigned int vbo_{};



        /**
         * @brief Renders text at a specified position using a specified font and scale.
         *
         * This method uses OpenGL to render text, character by character, by updating vertex buffer
         * objects (VBOs) and binding glyph textures. Each glyph is positioned relative to the given
         * starting position, scaled according to the provided scale factor, and rendered using stored
         * glyph information from a font cache.
         *
         * @param text The text string to render.
         * @param position The starting (x, y) position for the text, in screen space coordinates.
         * @param scale The scale factor to apply to the size of each glyph.
         * @param fontId The font identifier used to select the font from the font cache.
         */
        void renderText(
            const std::string_view text,
            const helios::engine::core::data::FontId fontId,
            std::span<helios::math::vec4f> vertices,
            helios::rendering::text::FontResourceProvider* fontResourceProvider
        )  noexcept {

            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(vao_);

            int offset = 0;
            int stride = 6;
            for (auto ch : text) {
                auto vertex = vertices.subspan(offset, stride);
                auto glyph = fontResourceProvider->glyph(ch, fontId);

                // update the VBO for _each_ character (multiple drawcalls!)

                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, glyph.textureId);
                // update content of vbo memory
                glBindBuffer(GL_ARRAY_BUFFER, vbo_);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(helios::math::vec4f) * stride, vertex.data());
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                //render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // advance cursors for next glyph (1/64 pixels)

                offset += stride;
            }

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }


    public:


        /**
         * @brief Destroys the OpenGLGlyphTextRenderer and releases all OpenGL resources.
         *
         * Cleans up the Vertex Array Object (VAO), Vertex Buffer Object (VBO),
         * and all cached glyph textures for every loaded font.
         *
         * @note Must be called in a valid OpenGL context.
         */
        ~OpenGLGlyphTextRenderer() override {

            if (vao_ != 0) {
                glDeleteVertexArrays(1, &vao_);
            }
            if (vbo_ != 0) {
                glDeleteBuffers(1, &vbo_);
            }

        }

        /**
         * @brief Returns the texture unit used for text rendering.
         *
         * @return The texture unit index (always 0).
         */
        static inline int textTextureUnit() {
            return 0;
        }

        /**
         * @brief Renders text using the specified render command.
         *
         * Processes the `TextRenderCommand`, activates the associated shader,
         * applies uniform values, and renders the text string.
         *
         * @param command The render command containing text and rendering properties.
         * @param frameUniformValues Frame-level uniforms (e.g., projection matrix).
         *
         * @note Requires a valid OpenGL context and initialized renderer.
         */
        void render(
            const helios::rendering::text::TextRenderCommand& command,
            const helios::rendering::shader::UniformValueMap& frameUniformValues)  noexcept override {

            if (auto* textPrototype = command.textRenderPrototype()) {
                const auto& baseShader = textPrototype->shader();

                const auto* shader = static_cast<const helios::ext::opengl::rendering::shader::OpenGLShader*>(&baseShader);
                assert(shader && "Unexpected failure when casting to OpenGLShader.");

                shader->use();

                shader->applyUniformValues(frameUniformValues);
                shader->applyUniformValues(command.objectUniformValues());;
                shader->applyUniformValues(command.materialUniformValues());;

                const auto* textMesh = command.textMesh();

                renderText(
                    textMesh->text(),
                    textMesh->fontId(),
                    textMesh->vertices(textPrototype->fontResourceProvider()),
                    &textPrototype->fontResourceProvider()
                );
            }

        };




    };
}