
#include "bSplines.h"

using namespace Nc;
using namespace Nc::Graphic;

bSplines::bSplines(const Array<Vector2f> &controlPoints, const Array<int> &weight, const TMatrix &matrice)
  : Object(matrice), _controlPoints(controlPoints), _weight(weight)
{
    InitOption();
    ComputeKnots(_knots);
    Update();
}

bSplines::bSplines(const Array<Vector2f> &controlPoints, const Array<int> &weight, const Array<int> &knots, const TMatrix &matrice)
    : Object(matrice), _controlPoints(controlPoints), _weight(weight), _knots(knots)
{
    InitOption();
    Update();
}

bSplines::~bSplines()
{
}

void bSplines::InitOption()
{
    _resolution = 50;
    _degree = 3;

    _drawables.push_back(new Drawable(new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL::Enum::Points)));
    _drawables[0]->Config->RasterMode().SetPointSize(5);
    _drawables.push_back(new Drawable(new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL::Enum::LineStrip)));
    _drawables.push_back(new Drawable(new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL::Enum::LineStrip)));
    _drawables.push_back(new Drawable(new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL::Enum::LineStrip)));
    ChooseDefaultMaterial();
}

void bSplines::Draw(Graphic::SceneGraph *scene)
{
    _mutex.Lock();
    Object::Draw(scene);
    _mutex.Unlock();
}

void bSplines::ComputeKnots(Array<int> &knots)
{
    int n = _controlPoints.Size() - 1;

    knots.InitSize(n + _degree + 1);
    for (int j=0; j <= n + _degree; j++)
    {
        if (j < _degree)
            knots[j] = 0;
        else if (j <= n)
            knots[j] = j - _degree + 1;
        else if (j > n)
            knots[j] = n - _degree + 2;
    }
}

void bSplines::Update()
{
    _mutex.Lock();

    // update les geometry des drawables
    UpdateControlPoints(Color(255, 0, 0), Color(128, 128, 128));
    UpdatebSpline(Color(255, 255, 0));
    UpdateNurb(Color(0, 255, 255));

    _mutex.Unlock();
}

void bSplines::UpdateControlPoints(const Color &c1, const Color &c2)
{
  Array<DefaultVertexType::Colored2d>   vertices(_controlPoints.Size());

  for (unsigned int i = 0; i < _controlPoints.Size(); ++i)
      vertices[i].Fill(_controlPoints[i][0], _controlPoints[i][1], c1);
  static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d,false>*>(_drawables[0]->Geometry)->VBO().UpdateData(vertices, GL::Enum::DataBuffer::StaticDraw);

  for (unsigned int i = 0; i < _controlPoints.Size(); ++i)
      vertices[i].Fill(_controlPoints[i][0], _controlPoints[i][1], c2);
  static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d,false>*>(_drawables[1]->Geometry)->VBO().UpdateData(vertices, GL::Enum::DataBuffer::StaticDraw);
}

void bSplines::UpdatebSpline(const Color &c)
{
    int		n = _controlPoints.Size() - 1;
    float	interval = 0, increment = (n - _degree + 2) / (float)(_resolution - 1);
    Vector2f	p;

    Array<DefaultVertexType::Colored2d>   vertices(_resolution + 1);

    vertices[0].Fill(_controlPoints[0][0], _controlPoints[0][1], c);
    for (unsigned int i = 1; i < _resolution; ++i)
    {
        SplinePoint(n, interval, p);
        vertices[i].Fill(p[0], p[1], c);
        interval += increment;
    }
    vertices[_resolution].Fill(_controlPoints[n][0] , _controlPoints[n][1], c);
    static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d,false>*>(_drawables[2]->Geometry)->VBO().UpdateData(vertices, GL::Enum::DataBuffer::StaticDraw);
}

void bSplines::UpdateNurb(const Color &c)
{
    int n = _controlPoints.Size() - 1;
    float interval = 0;
    float increment = (n - _degree + 2) / (float)(_resolution - 1);

    Vector2f p;

    Array<DefaultVertexType::Colored2d>   vertices(_resolution + 1);

    vertices[0].Fill(_controlPoints[0][0], _controlPoints[0][1], c);
    for (unsigned int i = 1; i < _resolution; ++i)
    {
        NurbPoint(n, interval, p);
        vertices[i].Fill(p[0], p[1], c);
        interval += increment;
    }
    vertices[_resolution].Fill(_controlPoints[n][0] , _controlPoints[n][1], c);
    static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d,false>*>(_drawables[3]->Geometry)->VBO().UpdateData(vertices, GL::Enum::DataBuffer::StaticDraw);
}

void bSplines::SplinePoint(int n, float v, Vector2f &point)
{
    int     k;
    float   b;

    point.Init(0, 0);
    for (k=0; k <= n; ++k)
    {
        b = SplineBlend(k, _degree, v);
        point[0] += _controlPoints[k][0] * b;
        point[1] += _controlPoints[k][1] * b;
    }
}

void bSplines::NurbPoint(int n, float v, Vector2f &point)
{
    int         k;
    float       b;
    Vector2f    tmpPoint;

    point.Init(0, 0);
    for (k=0; k <= n; ++k)
    {
        b = SplineBlend(k, _degree, v);
        point[0] += _controlPoints[k][0] * b * _weight[k];
        point[1] += _controlPoints[k][1] * b * _weight[k];
    }
    for (k=0; k <= n; ++k)
    {
        b = SplineBlend(k, _degree, v);
        tmpPoint[0] += b * _weight[k];
        tmpPoint[1] += b * _weight[k];
    }
    point[0] /= tmpPoint[0];
    point[1] /= tmpPoint[1];
}

float bSplines::SplineBlend(int k, int t, float v)
{
    float value;

    if (t == 1)
    {
        if ((_knots[k] <= v) && (v < _knots[k+1]))
            value = 1;
        else
            value = 0;
    }
    else
    {
        if ((_knots[k+t-1] == _knots[k]) && (_knots[k+t] == _knots[k+1]))
            value = 0;
        else if (_knots[k+t-1] == _knots[k])
            value = (_knots[k+t] - v) / (_knots[k+t] - _knots[k+1]) * SplineBlend(k+1, t-1, v);
        else if (_knots[k+t] == _knots[k+1])
            value = (v - _knots[k]) / (_knots[k+t-1] - _knots[k]) * SplineBlend(k, t-1, v);
        else
            value = (v - _knots[k]) / (_knots[k+t-1] - _knots[k]) * SplineBlend(k, t-1, v) +
                    (_knots[k+t] - v) / (_knots[k+t] - _knots[k+1]) * SplineBlend(k+1, t-1, v);
    }
    return value;
}


