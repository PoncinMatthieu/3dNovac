
#include "FrameBufferEffect.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace FBEffectsDemo;

FrameBufferEffect::FrameBufferEffect(Graphic::Window *window)
  : Effect(), _window(window), _postProcessActivated(true)
{
	_camera2d = new Camera2d(_window);

	_postProcessMaterial = new ProgrammableMaterial("postProcessEffect", "Nc:Shader:postProcessEffect.vs", "Nc:Shader:postProcessEffect.fs");

	_postProcessMaterial->SetAttrib(GL::DefaultVertexType::ComponentsName::Coord, "InCoord");
	_postProcessMaterial->SetAttrib(GL::DefaultVertexType::ComponentsName::TexCoord, "InTexCoord");

	_postProcessMaterial->SetMVPMatrixUniform("MVPMatrix");
	_postProcessMaterial->SetDrawableTextureUnitUniform(0, "Frame");
	_postProcessMaterial->GetUniform<GL::Texture, 1>("SketchSampler")->Init(1, GL::Texture("Nc:Image:Sketch.png"));
	_sketchJitter = _postProcessMaterial->GetUniform<float,2>("SketchJitter");

	_postProcessMaterial->GetUniform<bool,1>("SketchActivated")->value = true;
	_postProcessMaterial->GetUniform<bool,1>("SketchColored")->value = true;
	_postProcessMaterial->GetUniform<float,1>("SketchThreshold")->value = 1.f;
	_postProcessMaterial->GetUniform<float,1>("SketchBrightness")->value =	0.333f;
	_timeToNextJitter = 0;
	_sketchJitterSpeed = 10;

/*
	_postProcessMaterial->GetUniform<bool,1>("SketchActivated")->value = true;
	_postProcessMaterial->GetUniform<bool,1>("SketchColored")->value = false;
	_postProcessMaterial->GetUniform<float,1>("SketchThreshold")->value = 0.0f;
	_postProcessMaterial->GetUniform<float,1>("SketchBrightness")->value =	0.1333f;
	_timeToNextJitter = 0;
	_sketchJitterSpeed = 25;
*/

	Vector2ui size(_window->Width(), _window->Height());
	_sprite = new Sprite(size, GL::Texture(), Box2i(Vector2f(0,0), size), GL::Blend::Disabled);
	_sprite->UseSceneMaterial(true);

    _fboNeedInit = true;
}

FrameBufferEffect::~FrameBufferEffect()
{
	delete _camera2d;
	delete _sprite;
	delete _postProcessMaterial;
}

void	FrameBufferEffect::InitFbo1()
{
	Vector2ui size(_window->Width(), _window->Height());

	LOG << "create coloreTexture" << std::endl;
	GL::Texture colorTexture;
	colorTexture.Create(GL::Enum::Texture::Texture2d);
	colorTexture.Enable();
	colorTexture.Init2d(0, GL::Enum::Texture::RGBA8, size, GL::Enum::PixelFormat::RGBA, GL::Enum::PixelDataType::UnsignedByte, NULL);
	colorTexture.GenerateMipmaps();
	colorTexture.Disable();

	LOG << "create depthRenderBuffer" << std::endl;
	GL::RenderBuffer depthRenderBuffer;
	depthRenderBuffer.Create();
	depthRenderBuffer.Enable();
	depthRenderBuffer.Init(GL::Enum::RenderBuffer::DepthComponent, size);
	depthRenderBuffer.Disable();

	LOG << "Create fbo" << std::endl;
    _fboPass1.Create();
	_fboPass1.Enable();
	_fboPass1.Attach(GL::Enum::FrameBuffer::ColorAttachment0, colorTexture, 0);
	_fboPass1.Attach(GL::Enum::FrameBuffer::DepthAttachement, depthRenderBuffer);
	if(_fboPass1.CheckStatus() != GL::Enum::FrameBuffer::Complete)
		throw Utils::Exception("Can't initialize the fbo");
	_fboPass1.Disable();

    _sprite->SetTexture(colorTexture);
    _sprite->Size(size);
    _sprite->TextureBox(Box2i(Vector2f(0,0), size));
    _camera2d->Resized(size);
    LOG << "init done" << std::endl;

	_fboNeedInit = false;
}

void	FrameBufferEffect::Render(SceneGraph *scene)
{
	if (_fboNeedInit)
		InitFbo1();

	// pass1 : processing
	bool postProcessActivated = _postProcessActivated;
	if (postProcessActivated)
	{
		_fboPass1.Enable();
		scene->GLState()->Clear(GL::Enum::ColorBufferBit | GL::Enum::DepthBufferBit);
	}

	RenderChilds(scene);

	if (postProcessActivated)
	{
		_fboPass1.Disable();

		scene->PushMaterial();
		scene->Material() = _postProcessMaterial;

		// pass2 : post processing
		static_cast<Graphic::Object*>(_camera2d)->RenderNode(scene);
		_sprite->RenderNode(scene);

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
