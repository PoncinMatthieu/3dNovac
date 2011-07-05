
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        28/06/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_CORE_GL_GL_ENUM_H_
#define NC_GRAPHIC_CORE_GL_GL_ENUM_H_

#ifdef Always
    #undef Always
#endif

#include "../GL.h"

namespace Nc
{
	namespace Graphic
	{
		namespace GL
		{
			namespace Enum
			{
			    /// Define the gl capabilities
				/**
                    Sample of ogl functions used with this enum:
                        - glEnable()
                        - glDisable()
				*/
				enum Capability
				{
                    Blend                   = GL_BLEND,                     ///< If enabled, blend the computed fragment color values with the values in the color buffers. See glBlendFunc.
                    //ClipDistancei           = GL_CLIP_DISTANCEi,            ///< If enabled, clip geometry against user-defined half space i.
                    ColorLogicOp            = GL_COLOR_LOGIC_OP,            ///< If enabled, apply the currently selected logical operation to the computed fragment color and color buffer values. See glLogicOp.
                    CullFace                = GL_CULL_FACE,                 ///< If enabled, cull polygons based on their winding in window coordinates. See glCullFace.
                    DepthClamp              = GL_DEPTH_CLAMP,               ///< If enabled, the -wc ≤ zc ≤ wc plane equation is ignored by view volume clipping (effectively, there is no near or far plane clipping). See glDepthRange.
                    DepthTest               = GL_DEPTH_TEST,                ///< If enabled, do depth comparisons and update the depth buffer. Note that even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled. See glDepthFunc and glDepthRange.
                    Dither                  = GL_DITHER,                    ///< If enabled, dither color components or indices before they are written to the color buffer.
                    LineSmooth              = GL_LINE_SMOOTH,               ///< If enabled, draw lines with correct filtering. Otherwise, draw aliased lines. See glLineWidth.
                    Multisample             = GL_MULTISAMPLE,               ///< If enabled, use multiple fragment samples in computing the final color of a pixel. See glSampleCoverage.
                    PolygonOffsetFill       = GL_POLYGON_OFFSET_FILL,       ///< If enabled, and if the polygon is rendered in GL_FILL mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed. See glPolygonOffset.
                    PolygonOffsetLine       = GL_POLYGON_OFFSET_LINE,       ///< If enabled, and if the polygon is rendered in GL_LINE mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed. See glPolygonOffset.
                    PolygonOffsetPoint      = GL_POLYGON_OFFSET_POINT,      ///< If enabled, an offset is added to depth values of a polygon's fragments before the depth comparison is performed, if the polygon is rendered in GL_POINT mode. See glPolygonOffset.
                    PolygonSmooth           = GL_POLYGON_SMOOTH,            ///< If enabled, draw polygons with proper filtering. Otherwise, draw aliased polygons. For correct antialiased polygons, an alpha buffer is needed and the polygons must be sorted front to back.
                    PrimitiveRestart        = GL_PRIMITIVE_RESTART,         ///< Enables primitive restarting.  If enabled, any one of the draw commands which transfers a set of generic attribute array elements to the GL will restart the primitive when the index of the vertex is equal to the primitive restart index. See glPrimitiveRestartIndex.
                    SampleAlphaCoverage     = GL_SAMPLE_ALPHA_TO_COVERAGE,  ///< If enabled, compute a temporary coverage value where each bit is determined by the alpha value at the corresponding sample location.  The temporary coverage value is then ANDed with the fragment coverage value.
                    SampleAlphaToOne        = GL_SAMPLE_ALPHA_TO_ONE,       ///< If enabled, each sample alpha value is replaced by the maximum representable alpha value.
                    SampleCoverage          = GL_SAMPLE_COVERAGE,           ///< If enabled, the fragment's coverage is ANDed with the temporary coverage value. If GL_SAMPLE_COVERAGE_INVERT is set to GL_TRUE, invert the coverage value. See glSampleCoverage.
                    ScissorTest             = GL_SCISSOR_TEST,              ///< If enabled, discard fragments that are outside the scissor rectangle. See glScissor.
                    StencilTest             = GL_STENCIL_TEST,              ///< If enabled, do stencil testing and update the stencil buffer. See glStencilFunc and glStencilOp.
                    TextureCubeMapSeamLess  = GL_TEXTURE_CUBE_MAP_SEAMLESS, ///< If enabled, modifies the way sampling is performed on cube map textures.  See the spec for more information.
                    ProgramPointSize        = GL_PROGRAM_POINT_SIZE,        ///< If enabled, and a vertex or geometry shader is active, then the derived point size is taken from the (potentially clipped) shader builtin gl_PointSize and clamped to the implementation-dependent point size range.
                };

                /// Define ogl scalar types
				/**
                    Sample of ogl functions used with this enum:
                        - glDrawElements()
				*/
                enum Type
                {
                    UnsignedByte            = GL_UNSIGNED_BYTE,
                    UnsignedShort           = GL_UNSIGNED_SHORT,
                    UnsignedInt             = GL_UNSIGNED_INT,
                    Float                   = GL_FLOAT
                };

				/// Define the different types of primitive with can be draw
				/**
                    Sample of ogl functions used with this enum:
                        - glDrawArrays()
                        - glDrawElements()
				*/
				enum PrimitiveType
				{
					Points 					= GL_POINTS,
					LineStrip 				= GL_LINE_STRIP,
					LineLoop 				= GL_LINE_LOOP,
					Lines 					= GL_LINES,
					LineStripAdjacency		= GL_LINE_STRIP_ADJACENCY,
					LinesAdjacency			= GL_LINES_ADJACENCY,
					TriangleStrip			= GL_TRIANGLE_STRIP,
					TriangleFan				= GL_TRIANGLE_FAN,
					Triangles				= GL_TRIANGLES,
					TriangleStripAdjacency	= GL_TRIANGLE_STRIP_ADJACENCY,
					TrianglesAdjacency		= GL_TRIANGLES_ADJACENCY
				};

                /// Define the supported types of buffers
                /**
                    Sample of ogl functions used with this enum:
                        - glBindBuffer()
                        - glBufferData()
                        - glBufferSubData()
                        - glMapBuffer()
                        - glUnmapBuffer()

                    \sa
                        - GL::DataBuffer
                */
                enum BufferTarget
                {
                    ArrayBuffer             = GL_ARRAY_BUFFER,
                    CopyReadBuffer          = GL_COPY_READ_BUFFER,
                    CopyWriteBuffer         = GL_COPY_WRITE_BUFFER,
                    ElementArrayBuffer      = GL_ELEMENT_ARRAY_BUFFER,
                    PixelPackBuffer         = GL_PIXEL_PACK_BUFFER,
                    PixelUnpackBuffer       = GL_PIXEL_UNPACK_BUFFER,
                    TextureBuffer           = GL_TEXTURE_BUFFER,
                    TransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER,
                    UniformBuffer           = GL_UNIFORM_BUFFER
                };

                /// Define the supported types of buffers
                /**
                    Sample of ogl functions used with this enum:
                        - glBufferData()

                    \sa
                        - GL::DataBuffer
                */
                enum BufferUsage
                {
                    StreamDraw      = GL_STREAM_DRAW,
                    StreamRead      = GL_STREAM_READ,
                    StreamCopy      = GL_STREAM_COPY,
                    StaticDraw      = GL_STATIC_DRAW,
                    StaticRead      = GL_STATIC_READ,
                    StaticCopy      = GL_STATIC_COPY,
                    DynamicDraw     = GL_DYNAMIC_DRAW,
                    DynamicRead     = GL_DYNAMIC_READ,
                    DynamicCopy     = GL_DYNAMIC_COPY
                };

                /// Define an access rights for buffers
				/**
                    Sample of ogl functions used with this enum:
                        - glMapBuffer()
				*/
                enum BufferAccessType
                {
                    ReadOnly    = GL_READ_ONLY,
                    WriteOnly   = GL_WRITE_ONLY,
                    ReadWrite   = GL_READ_WRITE
                };

                /// Define a polygon face mode to be apply
				/**
                    Sample of ogl functions used with this enum:
                        - glPolygonMode()
				*/
                enum PolygonFace
                {
                    Front           = GL_FRONT,
                    Back            = GL_BACK,
                    FrontAndBack    = GL_FRONT_AND_BACK
                };

                /// Define a polygon mode to be apply
				/**
                    Sample of ogl functions used with this enum:
                        - glPolygonMode()
				*/
                enum PolygonMode
                {
                    Point = GL_POINT,
                    Line = GL_LINE,
                    Fill = GL_FILL
                };

                /// Define the blends factors
				/**
                    Sample of ogl functions used with this enum:
                        - glBlendFunc()
				*/
                enum BlendFactor
                {
                    Zero                    = GL_ZERO,
                    One                     = GL_ONE,
                    SrcColor                = GL_SRC_COLOR,
                    OneMinusSrcColor        = GL_ONE_MINUS_SRC_COLOR,
                    DstColor                = GL_DST_COLOR,
                    OneMinusDstSolor        = GL_ONE_MINUS_DST_COLOR,
                    SrcAlpha                = GL_SRC_ALPHA,
                    OneMinusSrcAlpha        = GL_ONE_MINUS_SRC_ALPHA,
                    DstAlpha                = GL_DST_ALPHA,
                    OneMinusDstAlpha        = GL_ONE_MINUS_DST_ALPHA,
                    ConstantColor           = GL_CONSTANT_COLOR,
                    OneMinusConstantColor   = GL_ONE_MINUS_CONSTANT_COLOR,
                    ConstantAlpha           = GL_CONSTANT_ALPHA,
                    OneMinusConstantAlpha   = GL_ONE_MINUS_CONSTANT_ALPHA
                };

                /// Define the different depth functions
				/**
                    Sample of ogl functions used with this enum:
                        - glDepthFunc()
				*/
                enum DepthFunc
                {
                    Never       = GL_NEVER,         ///< Never passes.
                    Less        = GL_LESS,          ///< Passes if the incoming depth value is less than the stored depth value.
                    Equal       = GL_EQUAL,         ///< Passes if the incoming depth value is equal to the stored depth value.
                    LEqual      = GL_LEQUAL,        ///< Passes if the incoming depth value is less than or equal to the stored depth value.
                    Greater     = GL_GREATER,       ///< Passes if the incoming depth value is greater than the stored depth value.
                    NotEqual    = GL_NOTEQUAL,      ///< Passes if the incoming depth value is not equal to the stored depth value.
                    GEqual      = GL_GEQUAL,        ///< Passes if the incoming depth value is greater than or equal to the stored depth value.
                    Always      = GL_ALWAYS         ///< Always passes.
                };

                /// Define ogl texture targets
				/**
                    Sample of ogl functions used with this enum:
                        - glBindTexture()
                        - glTexParameteri()
                        - glTexImage2D()
				*/
                enum TextureTarget
                {
                    Texture1D                   = GL_TEXTURE_1D,
                    Texture2D                   = GL_TEXTURE_2D,
                    Texture3D                   = GL_TEXTURE_3D,
                    Texture1DArray              = GL_TEXTURE_1D_ARRAY,
                    Texture2DArray              = GL_TEXTURE_2D_ARRAY,
                    TextureRectangle            = GL_TEXTURE_RECTANGLE,
                    TextureCubeMap              = GL_TEXTURE_CUBE_MAP,
                    Texture2DMultiSample        = GL_TEXTURE_2D_MULTISAMPLE,
                    Texture2DMultisampleArray   = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
                };

                /// Define ogl shader types
				/**
                    Sample of ogl functions used with this enum:
                        - glCreateShader()
				*/
                enum ShaderType
                {
                    VertexShader    = GL_VERTEX_SHADER,
                    GeometryShader  = GL_GEOMETRY_SHADER,
                    FragmentShader  = GL_FRAGMENT_SHADER
                };

                /// Define ogl buffer bit types
				/**
                    Sample of ogl functions used with this enum:
                        - glClear()
				*/
                enum BufferBitType
                {
                    ColorBufferBit      = GL_COLOR_BUFFER_BIT,      ///< Indicates the buffers currently enabled for color writing.
                    DepthBufferBit      = GL_DEPTH_BUFFER_BIT,      ///< Indicates the depth buffer.
                    StencilBufferBit    = GL_STENCIL_BUFFER_BIT     ///< Indicates the stencil buffer.
                };
			}
		}
	}
}

#endif

