
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

#ifndef NC_GRAPHIC_GL_ENUM_H_
#define NC_GRAPHIC_GL_ENUM_H_

#ifdef Always
    #undef Always
#endif

#include "GL.h"

namespace Nc
{
	namespace Graphic
	{
		namespace GL
		{
			namespace Enum
			{
			    /// Define opengl implementation descriptions, like opengl version and renderer.
				enum ImplementationDescription
				{
					Vendor					= GL_VENDOR,					///< Returns the company responsible for this GL implementation.
					Renderer				= GL_RENDERER,					///< Returns the name of the renderer. This name is typically specific to a particular configuration of a hardware platform.
                    Version					= GL_VERSION,					///< Returns a version or release number.
					ShadingLanguageVersion	= GL_SHADING_LANGUAGE_VERSION,	///< Returns a version or release number for the shading language.
                    Extensions				= GL_EXTENSIONS					///< Return the supported extensions.
				};

			    /// Define the gl capabilities.
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

                /// Define ogl scalar types.
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

				/// Define the different types of primitive with can be draw.
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

                namespace DataBuffer
                {
                    /// Define the supported types of buffers.
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
                    enum Target
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

                    /// Define the supported types of buffers.
                    /**
                        Sample of ogl functions used with this enum:
                            - glBufferData()

                        \sa
                            - GL::DataBuffer
                    */
                    enum Usage
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

                    /// Define an access rights for buffers.
                    /**
                        Sample of ogl functions used with this enum:
                            - glMapBuffer()
                    */
                    enum AccessType
                    {
                        ReadOnly    = GL_READ_ONLY,
                        WriteOnly   = GL_WRITE_ONLY,
                        ReadWrite   = GL_READ_WRITE
                    };
                }

                /// Define ogl buffer bit types.
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

                /// Define a polygon face mode to be apply.
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

                /// Define a polygon mode to be apply.
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

                /// Define the blends factors.
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

                /// Define the different depth functions.
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

				namespace Texture
				{
                    /// Define ogl texture targets.
                    /**
                        Sample of ogl functions used with this enum:
                            - glBindTexture()
                            - glTexParameteri()
                            - glTexImage2D()
                    */
                    enum Target
                    {
                        Texture1d                   = GL_TEXTURE_1D,
                        Texture2d                   = GL_TEXTURE_2D,
                        Texture3d                   = GL_TEXTURE_3D,
                        Texture1dArray              = GL_TEXTURE_1D_ARRAY,
                        Texture2dArray              = GL_TEXTURE_2D_ARRAY,
                        TextureRectangle            = GL_TEXTURE_RECTANGLE,
                        TextureCubeMap              = GL_TEXTURE_CUBE_MAP,
                        Texture2dMultiSample        = GL_TEXTURE_2D_MULTISAMPLE,
                        Texture2dMultisampleArray   = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
                    };

                    /// Define ogl texture internal format.
                    /**
                        Sample of ogl functions used with this enum:
                            - glTexImage*()
                    */
                    enum Format
                    {
                        CompressedRed               = GL_COMPRESSED_RED,
                        CompressedRG                = GL_COMPRESSED_RG,
                        CompressedRGB               = GL_COMPRESSED_RGB,
                        CompressedRGBA              = GL_COMPRESSED_RGBA,
                        CompressedSRGB              = GL_COMPRESSED_SRGB,
                        CompressedSRGBAlpha         = GL_COMPRESSED_SRGB_ALPHA,
                        DepthComponent              = GL_DEPTH_COMPONENT,
                        DepthComponent16            = GL_DEPTH_COMPONENT16,
                        DepthComponent24            = GL_DEPTH_COMPONENT24,
                        DepthComponent32            = GL_DEPTH_COMPONENT32,
                        R3_G3_B2                    = GL_R3_G3_B2,
                        Red                         = GL_RED,
                        RG                          = GL_RG,
                        RGB                         = GL_RGB,
                        RGB4                        = GL_RGB4,
                        RGB5                        = GL_RGB5,
                        RGB8                        = GL_RGB8,
                        RGB10                       = GL_RGB10,
                        RGB12                       = GL_RGB12,
                        RGB16                       = GL_RGB16,
                        RGBA                        = GL_RGBA,
                        RGBA2                       = GL_RGBA2,
                        RGBA4                       = GL_RGBA4,
                        RGB5_A1                     = GL_RGB5_A1,
                        RGBA8                       = GL_RGBA8,
                        RGB10_A2                    = GL_RGB10_A2,
                        RGBA12                      = GL_RGBA12,
                        RGBA16                      = GL_RGBA16,
                        SRGB                        = GL_SRGB,
                        SRGB8                       = GL_SRGB8,
                        SRGBAlpha                   = GL_SRGB_ALPHA,
                        SRGB8Alpha8                 = GL_SRGB8_ALPHA8
                    };

                    /// Define filtering methods.
                    enum Filter
                    {
                        Linear                      = GL_LINEAR,
                        Nearest                     = GL_NEAREST,
                        LinearMipmapLinear          = GL_LINEAR_MIPMAP_LINEAR,
                        LinearMipmapNearest         = GL_LINEAR_MIPMAP_NEAREST,
                        NearestMipmapNearest        = GL_NEAREST_MIPMAP_NEAREST,
                        NearestMipmapLinear         = GL_NEAREST_MIPMAP_LINEAR
                    };
				}

                /// Define ogl pixel format.
				/**
                    Sample of ogl functions used with this enum:
                        - glTexImage*()
				*/
				namespace PixelFormat
				{
                    enum Type
                    {
                        Red         = GL_RED,
                        RG          = GL_RG,
                        RGB         = GL_RGB,
                        BGR         = GL_BGR,
                        RGBA        = GL_RGBA,
                        BGRA        = GL_BGRA
                    };
				}

                /// Define ogl pixel data type.
				/**
                    Sample of ogl functions used with this enum:
                        - glTexImage*()
				*/
				namespace PixelDataType
				{
                    enum Type
                    {
                        UnsignedByte                    = GL_UNSIGNED_BYTE,
                        Byte                            = GL_BYTE,
                        UnsignedShort                   = GL_UNSIGNED_SHORT,
                        Short                           = GL_SHORT,
                        UnsignedInt                     = GL_UNSIGNED_INT,
                        Int                             = GL_INT,
                        Float                           = GL_FLOAT,
                        UnsignedByte332                 = GL_UNSIGNED_BYTE_3_3_2,
                        UnsignedByte233Rev              = GL_UNSIGNED_BYTE_2_3_3_REV,
                        UnsignedShort565                = GL_UNSIGNED_SHORT_5_6_5,
                        UnsignedShort565Rev             = GL_UNSIGNED_SHORT_5_6_5_REV,
                        UnsignedShort4444               = GL_UNSIGNED_SHORT_4_4_4_4,
                        UnsignedShort4444Rev            = GL_UNSIGNED_SHORT_4_4_4_4_REV,
                        UnsignedShort5551               = GL_UNSIGNED_SHORT_5_5_5_1,
                        UnsignedShort1555Rev            = GL_UNSIGNED_SHORT_1_5_5_5_REV,
                        UnsignedInt8888                 = GL_UNSIGNED_INT_8_8_8_8,
                        UnsignedInt8888Rev              = GL_UNSIGNED_INT_8_8_8_8_REV,
                        UnsignedInt1010102              = GL_UNSIGNED_INT_10_10_10_2,
                        UnsignedInt2101010Rev           = GL_UNSIGNED_INT_2_10_10_10_REV
                    };
				}

                /// Define ogl shader types.
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

                namespace FrameBuffer
                {
                    /// Define ogl Frame buffer types.
                    /**
                        Sample of ogl functions used with this enum:
                            - glBindFrameBuffer()
                            - glFrameBufferRenderBuffer()
                    */
                    enum Target
                    {
                        FrameBuffer         = GL_FRAMEBUFFER,
                        DrawFrameBuffer     = GL_DRAW_FRAMEBUFFER,
                        ReadFrameBuffer     = GL_READ_FRAMEBUFFER
                    };

                    /// Define ogl Frame buffer attachment points.
                    /**
                        Sample of ogl functions used with this enum:
                            - glFrameBufferRenderBuffer()
                            - glFraneBufferTexture()
                            - glFraneBufferTexture1D()
                            - glFraneBufferTexture2D()
                            - glFraneBufferTexture3D()
                    */
                    enum AttachementPoint
                    {
                        ColorAttachment0            = GL_COLOR_ATTACHMENT0,
                        ColorAttachment1            = GL_COLOR_ATTACHMENT1,
                        ColorAttachment2            = GL_COLOR_ATTACHMENT2,
                        ColorAttachment3            = GL_COLOR_ATTACHMENT3,
                        ColorAttachment4            = GL_COLOR_ATTACHMENT4,
                        ColorAttachment5            = GL_COLOR_ATTACHMENT5,
                        ColorAttachment6            = GL_COLOR_ATTACHMENT6,
                        ColorAttachment7            = GL_COLOR_ATTACHMENT7,
                        ColorAttachment8            = GL_COLOR_ATTACHMENT8,
                        ColorAttachment9            = GL_COLOR_ATTACHMENT9,
                        ColorAttachment10           = GL_COLOR_ATTACHMENT10,
                        ColorAttachment11           = GL_COLOR_ATTACHMENT11,
                        ColorAttachment12           = GL_COLOR_ATTACHMENT12,
                        ColorAttachment13           = GL_COLOR_ATTACHMENT13,
                        ColorAttachment14           = GL_COLOR_ATTACHMENT14,
                        ColorAttachment15           = GL_COLOR_ATTACHMENT15,
                        DepthAttachement            = GL_DEPTH_ATTACHMENT,
                        StencilAttachement          = GL_STENCIL_ATTACHMENT,
                        DepthStencilAttachment      = GL_DEPTH_STENCIL_ATTACHMENT
                    };

                    /// Define ogl Frame buffer status.
                    /**
                        Sample of ogl functions used with this enum:
                            - glCheckFramebufferStatus()
                    */
                    enum State
                    {
                        Complete                    = GL_FRAMEBUFFER_COMPLETE,                      ///< if the framebuffer bound to target is complete.
                        Undefined                   = GL_FRAMEBUFFER_UNDEFINED,                     ///< is returned if target is the default framebuffer, but the default framebuffer does not exist.
                        IncompleteAttachment        = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,         ///< is returned if any of the framebuffer attachment points are framebuffer incomplete.
                        IncompleteMissingAttachment = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, ///< is returned if the framebuffer does not have at least one image attached to it.
                        IncompleteDrawBuffer        = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,        ///< is returned if the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAWBUFFERi.
                        IncompleteReadBuffer        = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,        ///< is returned if GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER.
                        Unsuported                  = GL_FRAMEBUFFER_UNSUPPORTED,                   ///< is returned if the combination of internal formats of the attached images violates an implementation-dependent set of restrictions.
                        IncompleteMultisample       = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,        ///< is returned if the value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES. GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE is also returned if the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures.
                        IncompleteLayerTargets      = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,      ///< is returned if any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target.
                        Error                       = 0                                             ///< Additionally, if an error occurs, zero is returned.
                    };
                }

                namespace RenderBuffer
                {
                    /// Define ogl Render buffer types.
                    /**
                        Sample of ogl functions used with this enum:
                            - glBindRenderBuffer()
                            - glFrameBufferRenderBuffer()
                    */
                    enum Target
                    {
                        RenderBuffer         = GL_RENDERBUFFER
                    };

                    /// Define ogl Color buffer internal formats.
                    /**
                        Sample of ogl functions used with this enum:
                            - glRenderBufferStorage()
                            - glRenderBufferStorageMultisample()
                    */
                    enum ColorFormat
                    {
                        Red             = GL_RED,
                        RG              = GL_RG,
                        RGB             = GL_RGB,
                        RGBA            = GL_RGBA,
                        R8              = GL_R8,
                        R16             = GL_R16,
                        RG8             = GL_RG8,
                        RG16            = GL_RG16,
                        RGBA8           = GL_RGBA8,
                        RGB10_A2        = GL_RGB10_A2,
                        RGB10_A2UI      = GL_RGB10_A2UI,
                        RGBA16          = GL_RGBA16,
                        SRGB8_ALPHA8    = GL_SRGB8_ALPHA8,
                        R16F            = GL_R16F,
                        RG16F           = GL_RG16F,
                        RGBA16F         = GL_RGBA16F,
                        R32F            = GL_R32F,
                        RG32F           = GL_RG32F,
                        RGBA32F         = GL_RGBA32F,
                        R11F_G11F_B10F  = GL_R11F_G11F_B10F,
                        R8I             = GL_R8I,
                        R8UI            = GL_R8UI,
                        R16I            = GL_R16I,
                        R16UI           = GL_R16UI,
                        R32I            = GL_R32I,
                        R32UI           = GL_R32UI,
                        RG8I            = GL_RG8I,
                        RG8UI           = GL_RG8UI,
                        RG16I           = GL_RG16I,
                        RG16UI          = GL_RG16UI,
                        RG32I           = GL_RG32I,
                        RG32UI          = GL_RG32UI,
                        RGBA8I          = GL_RGBA8I,
                        RGBA8UI         = GL_RGBA8UI,
                        RGBA16I         = GL_RGBA16I,
                        RGBA16UI        = GL_RGBA16UI,
                        RGBA32I         = GL_RGBA32I,
                        RGBA32UI        = GL_RGBA32UI
                    };

                    /// Define ogl Depth buffer internal formats.
                    /**
                        Sample of ogl functions used with this enum:
                            - glRenderBufferStorage()
                            - glRenderBufferStorageMultisample()
                    */
                    enum DepthFormat
                    {
                        DepthComponent      = GL_DEPTH_COMPONENT,
                        DepthComponent16    = GL_DEPTH_COMPONENT16,
                        DepthComponent24    = GL_DEPTH_COMPONENT24,
                        DepthComponent32    = GL_DEPTH_COMPONENT32,
                        DepthComponent32f   = GL_DEPTH_COMPONENT32F
                    };

                    /// Define ogl Stencil buffer internal formats.
                    /**
                        Sample of ogl functions used with this enum:
                            - glRenderBufferStorage()
                            - glRenderBufferStorageMultisample()
                    */
                    enum StencilFormat
                    {
                            StencilIndex        = GL_STENCIL_INDEX,
                            StencilIndex4       = GL_STENCIL_INDEX4,
                            StencilIndex8       = GL_STENCIL_INDEX8,
                            StencilIndex16      = GL_STENCIL_INDEX16
                    };

                    /// Define ogl DepthStencil buffer internal formats.
                    /**
                        Sample of ogl functions used with this enum:
                            - glRenderBufferStorage()
                            - glRenderBufferStorageMultisample()
                    */
                    enum DepthStencilFormat
                    {
                        DepthStencil        = GL_DEPTH_STENCIL,
                        Depth24Stencil8     = GL_DEPTH24_STENCIL8,
                        Depth32fStencil8    = GL_DEPTH32F_STENCIL8
                    };
                }

                namespace Blit
                {
                    enum Filter
                    {
                        Linear      = GL_LINEAR,
                        Nearest     = GL_NEAREST
                    };
                }
			}
		}
	}
}

#endif

