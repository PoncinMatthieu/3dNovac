
#include "FrameBufferEffect.h"

using namespace Nc;
using namespace Nc::Graphic;

FrameBufferEffect::FrameBufferEffect()
  : Effect(ClassName()), _postProcessActivated(true)
{
  _camera2d = new Camera2d();

  _postProcessMaterial = new ProgrammableMaterial("postProcessEffect", "Nc:Shader:postProcessEffect.vs", "Nc:Shader:postProcessEffect.fs");
  _postProcessMaterial->SetMVPMatrixUniform("MVPMatrix");
  _postProcessMaterial->SetDrawableTextureUnitUniform(0, "Frame");
  GL::Uniform<GL::Texture, 1> *t1 = _postProcessMaterial->GetUniform<GL::Texture, 1>("SketchSampler");
  t1->unit = 1;
  t1->texture.LoadFromFile("Nc:Image:Sketch.png");

  _postProcessMaterial->GetUniform<bool,1>("SketchActivated")->value = true;
  _postProcessMaterial->GetUniform<bool,1>("SketchColored")->value = true;
  _postProcessMaterial->GetUniform<float,1>("SketchThreshold")->value = 1;
  _postProcessMaterial->GetUniform<float,1>("SketchBrightness")->value = 0.333f;
  _sketchJitter = _postProcessMaterial->GetUniform<float,2>("SketchJitter");
  _postProcessMaterial->SetAttrib(DefaultVertexType::ComponentsName::Coord, "InCoord");
  _postProcessMaterial->SetAttrib(DefaultVertexType::ComponentsName::TexCoord, "InTexCoord");

  _timeToNextJitter = 0;
  _sketchJitterSpeed = 10;
  /*
  _postProcessMaterial->GetUniform<bool,1>("SketchActivated")->value = true;
  _postProcessMaterial->GetUniform<bool,1>("SketchColored")->value = false;
  _postProcessMaterial->GetUniform<float,1>("SketchThreshold")->value = 0;//1;
  _postProcessMaterial->GetUniform<float,1>("SketchBrightness")->value = 0.133f;
  _sketchJitter = _postProcessMaterial->GetUniform<float,2>("SketchJitter");
  _postProcessMaterial->SetAttrib(DefaultVertexType::ComponentsName::Coord, "InCoord");
  _postProcessMaterial->SetAttrib(DefaultVertexType::ComponentsName::TexCoord, "InTexCoord");
  */
  _timeToNextJitter = 0;
  _sketchJitterSpeed = 10;
}

FrameBufferEffect::~FrameBufferEffect()
{
  delete _camera2d;
  delete _postProcessMaterial;
}

void	FrameBufferEffect::InitFbo1()
{
  Vector2ui size(Window::Width(), Window::Height());

  GL::Texture colorTexture;
  colorTexture.Create(GL::Enum::Texture::Texture2d);
  colorTexture.Enable();
  colorTexture.Init2d(0, GL::Enum::Texture::RGBA8, size, GL::Enum::PixelFormat::RGBA, GL::Enum::PixelDataType::UnsignedByte, NULL);
  colorTexture.GenerateMipmaps();
  colorTexture.Disable();

  GL::RenderBuffer depthRenderBuffer;
  depthRenderBuffer.Create();
  depthRenderBuffer.Enable();
  depthRenderBuffer.Init(GL::Enum::RenderBuffer::DepthComponent, size);
  depthRenderBuffer.Disable();

  _fboPass1.Create();
  _fboPass1.Enable();
  _fboPass1.Attach(GL::Enum::FrameBuffer::ColorAttachment0, colorTexture, 0);
  _fboPass1.Attach(GL::Enum::FrameBuffer::DepthAttachement, depthRenderBuffer);
  if(_fboPass1.CheckStatus() != GL::Enum::FrameBuffer::Complete)
    throw Utils::Exception("Can't initialize the fbo");
  _fboPass1.Disable();

  _sprite = new Sprite(size, colorTexture, Box2f(Vector2f(0,0), colorTexture.Size()), GL::Blend::Disabled);
}

void	FrameBufferEffect::Render(SceneGraph *scene)
{
  GL::State &st = GL::State::Current();

  if (!_fboPass1.IsAttached())
    InitFbo1();

  // pass1 : processing
  bool postProcessActivated = _postProcessActivated;
  if (postProcessActivated)
    {
      _fboPass1.Enable();
      st.Clear(GL::Enum::ColorBufferBit | GL::Enum::DepthBufferBit);
    }

  RenderChilds(scene);

  if (postProcessActivated)
    {
      _fboPass1.Disable();
      scene->PushMaterial();
      scene->Material() = _postProcessMaterial;

      // pass2 : post processing
      static_cast<Graphic::Object*>(_camera2d)->Render(scene);
      _sprite->Render(scene);

      scene->PopMaterial();
    }
}

void	FrameBufferEffect::Update(float elapsedTime)
{
  if (_sketchJitterSpeed > 0 && _postProcessActivated)
    {
      _timeToNextJitter += elapsedTime;
      if (_timeToNextJitter >= (1/_sketchJitterSpeed))
	{
	  _sketchJitter->value1 = Math::Rand(0.f, 1.f);
	  _sketchJitter->value2 = Math::Rand(0.f, 1.f);
	  _timeToNextJitter = 0;
	}
    }
  UpdateChilds(elapsedTime);
}
