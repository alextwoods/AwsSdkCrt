#include <Rcpp.h>

class Point; // fwd declarations
class Shape;
class Circle;
class Rectangle;

RCPP_EXPOSED_CLASS(Point);
RCPP_EXPOSED_CLASS(Shape);
RCPP_EXPOSED_CLASS(Circle);
RCPP_EXPOSED_CLASS(Rectangle);

class Point {
public:
  Point( double x_, double y_) : x(x_), y(y_){}

  double x, y ;
} ;

double square( double x) {
  return x*x ;
}
double distance( const Point& p1, const Point& p2 ){
  return sqrt( square( p1.x - p2.x) + square( p1.y - p2.y ) ) ;
}


class Shape {
public:
  Shape( const Point& center_ ) : center(center_){}
  virtual ~Shape() {}

  Point center ;

  virtual double area() const { return 0.0 ;}
  virtual bool contains(const Point& point) const { return false ; }
} ;

class Circle : public Shape {
public:
  Circle( Point center_, double radius_ ): Shape(center_), radius(radius_){}

  double area() const {
    return 3.14159 * square( radius ) ;
  }
  bool contains( const Point& point ) const {
    return distance(point, center) < radius ;
  }

  double radius ;
} ;

class Rectangle : public Shape {
public:
  Rectangle( Point center_, double width_, double height_ ) :
  Shape(center_), width(width_), height(height_){}

  double area() const {
    return width * height ;
  }

  bool contains( const Point& point ){
    return (point.x >= ( center.x - width  / 2.0 )) &&
      (point.x <= ( center.x + width  / 2.0 )) &&
      (point.y >= ( center.y - height / 2.0 )) &&
      (point.y <= ( center.y + height / 2.0 ));
  }

  double width, height ;
} ;


RCPP_MODULE(play){
  using namespace Rcpp;

  class_<Point>("Point")
  .constructor<double,double>()
  .field( "x", &Point::x)
  .field( "y", &Point::y)
  ;
  class_<Shape>( "Shape" )
    .constructor<Point>()
    .method( "area", &Shape::area )
    .method( "contains", &Shape::contains )
  ;
  class_<Circle>( "Circle" )
    .derives<Shape>("Shape" )
    .constructor<Point,double>()
    .field( "r", &Circle::radius )
  ;
  class_<Rectangle>( "Rectangle" )
    .derives<Shape>("Shape" )
    .constructor<Point,double,double>()
    .field( "h", &Rectangle::height )
    .field( "w", &Rectangle::width )
  ;

};
