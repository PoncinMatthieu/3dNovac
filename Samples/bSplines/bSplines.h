
#ifndef BSPLINES_2D_H_
#define BSPLINES_2D_H_

#include <Nc/Graphics/Object/Object.h>

class bSplines : public Nc::Graphic::Object
{
 public:
  bSplines(const Array<Vector2f> &controlPoints, const Array<int> &weight, const TMatrix &matrice);
  bSplines(const Array<Vector2f> &controlPoints, const Array<int> &weight, const Array<int> &knots, const TMatrix &matrice);
  virtual ~bSplines();

  void            Update();

  Vector2f        &Vertex(unsigned int i)     {return _controlPoints[i];}
  unsigned int    NbPoint()                   {return _controlPoints.Size();}

 private:
  virtual void    Draw(Nc::Graphic::SceneGraph *scene);

  void	InitOption();

  void	UpdateControlPoints(const Color &c1, const Color &c2);
  void	UpdatebSpline(const Color &c);
  void	UpdateNurb(const Color &c);

  void	ComputeKnots(Array<int> &knots);
  void	SplinePoint(int n, float v, Vector2f &point);
  void	NurbPoint(int n, float v, Vector2f &point);
  float	SplineBlend(int k, int t, float v);

  Array<Vector2f>       _controlPoints;     // points de control des splines
  Array<int>            _weight;            // les poids de chaque point de control dans le cas de la representation des nurbs
  Array<int>            _knots;             // noeud
  int                   _degree;            // degree des splines
  unsigned int          _resolution;        // nombres de points de la courbe
  Nc::System::Mutex	_mutex;
};

#endif
