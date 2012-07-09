
#include <Nc/Graphics/GL/Drawable.h>
#include "ParticleEffect.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace SimpleParticleEffect;

const GL::VertexDescriptor   &ParticleEffect::VertexType::GetDescriptor()
{
  static GL::VertexDescriptor desc("ParticleEffect::VertexType", sizeof(VertexType), 5, GetDescriptorPriority());
  desc[0].Init<float>("Coord",			3, GL::Enum::Float, 0);
  desc[1].Init<float>("Speed",			3, GL::Enum::Float, 3 * sizeof(float));
  desc[2].Init<float>("Direction",		3, GL::Enum::Float, 6 * sizeof(float));
  desc[3].Init<float>("Acceleration",		1, GL::Enum::Float, 9 * sizeof(float));
  desc[4].Init<float>("LifeTime",		1, GL::Enum::Float, 10 * sizeof(float));
  return desc;
}

ParticleEffect::ParticleEffect()
  : Animation(), _started(false)
{
    Matrix.Scale(0.1, 0.1, 0.1);

    // create the drawable
    unsigned int		nbParticles = 100000;
    Array<VertexType>	vertices(nbParticles);

    for (unsigned int i = 0; i < nbParticles; ++i)
    {
        RandomPointOnSphere(vertices[i].coord);
        RandomPointOnSphere(vertices[i].speed);
        RandomPointOnSphere(vertices[i].direction);

        // acceleration
        vertices[i].acceleration = Rand(0.f, 1.f);
        vertices[i].lifeTime = 5.0 * (Rand(0.f, 1.f) * 0.3 + 0.3);
    }

    _drawable = new GL::Drawable(vertices, GL::Enum::DataBuffer::StaticDraw, GL::Enum::Points);
    _drawable->Config->SetBlend(GL::Blend::Add);
    _drawable->Config->GetRasterMode().SetDepthTest(false);

    // config the material
    _material = new ProgrammableMaterial("simpleParticleEffect", "Nc:Shader:simpleParticleEffect.vs", "Nc:Shader:simpleParticleEffect.fs");
    _material->SetMVPMatrixUniform("MVPMatrix");
    _material->SetAttrib("Coord", "InCoord");
    _material->SetAttrib("Speed", "InSpeed");
    _material->SetAttrib("Direction", "InDirection");
    _material->SetAttrib("Acceleration", "InAcceleration");
    _material->SetAttrib("LifeTime", "InLifeTime");

    _material->Configure(*_drawable);
}

ParticleEffect::~ParticleEffect()
{
    delete _drawable;
    delete _material;
}

void ParticleEffect::RandomPointOnSphere(float point[3])
{
  float x1 = 0;
  float x2 = 0;
  float ds = 2;

  while (ds >= 1)
    {
      x1 = Rand(0.f, 2.f) - 1;
      x2 = Rand(0.f, 2.f) - 1;
      ds = (x1 * x1) + (x2 * x2);
    }
  float ds2 = sqrt(1.f - (x1 * x1) - (x2 * x2));

  point[0] = 2.f * x1 * ds2;
  point[1] = 2.f * x2 * ds2;
  point[2] = 1.f - (2.f * ds);
}

void            ParticleEffect::Render(SceneGraph *scene)
{
    if (!_activated || !_started)
        return;

    float t = _clock.ElapsedTime();// * 0.1;

    if (t > 3)
        _clock.Reset();

    _material->GetUniform<float,1>("Time")->value = t;
    _material->Render(scene, Matrix, *_drawable);
}
