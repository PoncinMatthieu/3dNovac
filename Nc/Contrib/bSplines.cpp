/*
	Nc-Graphics
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/
*/

#include "bSplines.h"

using namespace Nc;
using namespace Nc::Graphic;

bSplines::bSplines(const Array<Vector2f> &controlPoints, const Array<int> &weight, const TMatrix &matrice)
    : Object2D(matrice), _controlPoints(controlPoints), _weight(weight)
{
    InitOption();
    ComputeKnots(_knots);
    Update();
}

bSplines::bSplines(const Array<Vector2f> &controlPoints, const Array<int> &weight, const Array<int> &knots, const TMatrix &matrice)
    : Object2D(matrice), _controlPoints(controlPoints), _weight(weight), _knots(knots)
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
}

void bSplines::Display()
{
    _mutex.Lock();
    _displayList.Draw();
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
//        LOG << knots[j] << " " << std::endl;
    }
}

void bSplines::Update()
{
    _mutex.Lock();
    _displayList.New();
    _displayList.Enable();
    glPushMatrix();
        if (!Matrix.IsIdentity())
            EXT.MultTransposeMatrixf(Matrix.Element());
        DrawControlPoints(Color(255, 0, 0), Color(128, 128, 128));
        DrawbSpline(Color(255, 255, 0));
        DrawNurb(Color(0, 255, 255));
    glPopMatrix();
    _displayList.Disable();
    _mutex.Unlock();
}

void bSplines::DrawControlPoints(const Color &c1, const Color &c2)
{
    c1.Enable();
    glDisable(GL_TEXTURE);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(5);

    // dessine les points
    glBegin(GL_POINTS);
        for (unsigned int i = 0; i < _controlPoints.Size(); ++i)
            glVertex2f(_controlPoints[i][0], _controlPoints[i][1]);
    glEnd();
    glPointSize(1);

    // dessine les arretes
    c2.Enable();
    glBegin(GL_LINE_STRIP);
        for (unsigned int i = 0; i < _controlPoints.Size(); ++i)
            glVertex2f(_controlPoints[i][0], _controlPoints[i][1]);
    glEnd();
    glDisable(GL_POINT_SMOOTH);
}

void bSplines::DrawbSpline(const Color &c)
{
    int n = _controlPoints.Size() - 1;
    float interval = 0;
    float increment = (n - _degree + 2) / (float)(_resolution - 1);

    Vector2f p;
    c.Enable();
    glBegin(GL_LINE_STRIP);

    glVertex2f(_controlPoints[0][0] , _controlPoints[0][1]);
    for (unsigned int i = 0; i < _resolution - 1; ++i)
    {
        SplinePoint(n, interval, p);
        glVertex2f(p[0], p[1]);
        interval += increment;
    }
    glVertex2f(_controlPoints[n][0] , _controlPoints[n][1]);
    glEnd();
}

void bSplines::DrawNurb(const Color &c)
{
    int n = _controlPoints.Size() - 1;
    float interval = 0;
    float increment = (n - _degree + 2) / (float)(_resolution - 1);

    Vector2f p;
    c.Enable();
    glBegin(GL_LINE_STRIP);

    glVertex2f(_controlPoints[0][0] , _controlPoints[0][1]);
    for (unsigned int i = 0; i < _resolution - 1; ++i)
    {
        NurbPoint(n, interval, p);
        glVertex2f(p[0], p[1]);
        interval += increment;
    }
    glVertex2f(_controlPoints[n][0] , _controlPoints[n][1]);
    glEnd();
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


