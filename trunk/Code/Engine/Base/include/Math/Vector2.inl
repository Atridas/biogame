//-----------------------------------------------------------------------
// Vector2 inline
/// Definiciones de funciones inline de la clase 'Vector2'
/// Este fichero es realmente parte de la cabecera 'Vector2.h' 
//-----------------------------------------------------------------------

// NO PONER INCLUDES

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versi�n 1
/// Construcci�n sin inicializaci�n de par�metros  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T>::Vector2()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versi�n 2
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T>::Vector2 (const T tx, const T ty)
{
  x = tx;
  y = ty;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versi�n 3
/// Constructor de copia
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T>::Vector2 (const Vector2<T>& otro)
{
  x = otro.x; 
  y = otro.y;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor versi�n 4
/// A partir de un escalar, que se establecer� en las 2 componentes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T>::Vector2 (const T escalar)
{
  x = escalar;
  y = escalar;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: suma de vectores
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> Vector2<T>::operator + (const Vector2<T>& otro) const
{   
  return (Vector2<T>(x + otro.x,
                     y + otro.y));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: resta de vectores
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> Vector2<T>::operator - (const Vector2<T>& otro) const
{   
  return (Vector2<T>(x - otro.x,
                     y - otro.y));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: producto vector x escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T> Vector2<T>::operator * (const T escalar) const
{ 
  return (Vector2<T>(x * escalar,
                     y * escalar)); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n externa operador aritm�tico: producto escalar x vector 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T> operator * (const T escalar, const Vector2<T>& otro)
{ 
  return (Vector2<T>(otro.x * escalar,
                     otro.y * escalar));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: cociente vector / escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T> Vector2<T>::operator / (const T escalar) const
{ 
#ifdef CHECK_MATH_SINGULARITY
  //-----------------------------------------------<<<
  // Con chequeo de divisi�n por cero
  Vector2<T> retVect(*this);

  ASSERTMSG(escalar != Zero<T>(), "Divisi�n por cero en cociente vector-escalar");
  if(escalar != Zero<T>())
  {
    retVect.x /= escalar;
    retVect.y /= escalar;
  }
 
  return retVect;
  //----------------------------------------------->>>
#else
  //-----------------------------------------------<<<
  // Sin chequeo
  return (Vector2<T>(x / escalar,
                     y / escalar));
  //----------------------------------------------->>>
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n externa operador aritm�tico: cociente escalar / vector
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T> operator / (const T escalar, const Vector2<T>& otro)
{
#ifdef CHECK_MATH_SINGULARITY
  //-----------------------------------------------<<<
  // Con chequeo de divisi�n por cero
  Vector2<T> retVect(escalar);

  ASSERTMSG(otro.x != Zero<T>() &&
            otro.y != Zero<T>(),
            "Divisi�n por cero en cociente escalar-vector");
  if(otro.x != Zero<T>())
  {
    retVect.x /= otro.x; 
  }
  if(otro.y != Zero<T>())
  {
    retVect.y /= otro.y;
  }

  return retVect;
  //----------------------------------------------->>>
#else
  //-----------------------------------------------<<<
  // Sin chequeo
  return (Vector2<T>(escalar / otro.x,
                     escalar / otro.y));
  //----------------------------------------------->>>
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: suma de un escalar a las 2 componentes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> Vector2<T>::operator + (const T escalar) const
{
  return Vector2<T>(x + escalar, y + escalar);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n externa operador aritm�tico: suma de un escalar a las 2 componenetes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> operator + (const T escalar, Vector2<T> vector)
{
  return Vector2<T>(vector.x + escalar, vector.y + escalar);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: resta de un escalar a las 2 componentes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> Vector2<T>::operator - (const T escalar) const
{
  return Vector2<T>(x - escalar, y - escalar);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n externa operador aritm�tico: resta de las 2 componentes a un escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> operator - (const T escalar, const Vector2<T> vector)
{
  return Vector2<T>(escalar - vector.x, escalar - vector.y);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: cambio de signo
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T> Vector2<T>::operator - () const
{   
  return (Vector2<T>(-x, -y));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: signo positivo
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T>& Vector2<T>::operator + ()
{
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico: signo positivo (versi�n const)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline const Vector2<T>& Vector2<T>::operator + () const
{
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico de actualizaci�n: incremento
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T>& Vector2<T>::operator += (const Vector2<T>& otro)
{   
  x += otro.x;
  y += otro.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico de actualizaci�n: decremento
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T>& Vector2<T>::operator -= (const Vector2<T>& otro)
{
  x -= otro.x;
  y -= otro.y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico de actualizaci�n: vector x escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T>& Vector2<T>::operator *= (const T escalar)
{ 
  x *= escalar;
  y *= escalar;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico de actualizaci�n: vector / escalar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T>& Vector2<T>::operator /= (const T escalar)
{
#ifdef CHECK_MATH_SINGULARITY
  //-----------------------------------------------<<<
  // Con chequeo de divisi�n por cero
  ASSERTMSG(escalar != Zero<T>(), "Divisi�n por cero en cociente vector-escalar");
  if(escalar != Zero<T>())
  {
    x /= escalar;
    y /= escalar;
  }
  //----------------------------------------------->>>
#else
  //-----------------------------------------------<<<
  // Sin chequeo
  x /= escalar;
  y /= escalar;
  //----------------------------------------------->>>
#endif
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico de actualizaci�n: vector + escalar
/// Suma el escalar a las 2 componentes del vector
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T>& Vector2<T>::operator += (const T escalar)
{
  x += escalar;
  y += escalar;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador aritm�tico de actualizaci�n: vector - escalar
/// Resta el escalar a las 2 componentes del vector
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T>& Vector2<T>::operator -= (const T escalar)
{
  x -= escalar;
  y -= escalar;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de asignaci�n de componentes par�ntesis
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T>& Vector2<T>::operator () (const T tx, const T ty)
{   
  x = tx;
  y = ty;
  
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n de asignaci�n de componentes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline void Vector2<T>::Set (const T tx, const T ty)
{
  x = tx;
  y = ty;  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece el vector a [0, 0]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline void Vector2<T>::SetZero ()
{
  x = Zero<T>();
  y = Zero<T>();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Establece el valor del vector a partir de coordenadas polares
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Vector2<T>::SetFromPolar(const T ang, const T length)
{ 
  mathUtils::SinCos(ang, y, x);
  x *= length;
  y *= length;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obtiene los datos del vector en forma de coordenadas polares
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void Vector2<T>::GetPolar(T& ang, T& length) const
{
  length = Length();
  ang    = (T)atan2(y, x);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador vectorial: producto escalar (dot product)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline T Vector2<T>::operator * (const Vector2<T>& otro) const
{
  return (x * otro.x + y * otro.y);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de igualdad absoluto
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline bool Vector2<T>::operator == (const Vector2<T>& otro) const
{   
    return (x == otro.x &&
            y == otro.y);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de desigualdad absoluto
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline bool Vector2<T>::operator != (const Vector2<T>& otro) const
{   
    return (x != otro.x ||
            y != otro.y);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de igualdad con epsilon global
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline bool Vector2<T>::IsEqualEpsilon (const Vector2<T>& otro) const
{
  return (mathUtils::Compare(x, otro.x) == 0 &&
          mathUtils::Compare(y, otro.y) == 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de desigualdad con epsilon global
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline bool Vector2<T>::IsNotEqualEpsilon (const Vector2<T>& otro) const
{
  return (mathUtils::Compare(x, otro.x) != 0 ||
          mathUtils::Compare(y, otro.y) != 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Producto por componentes (escalado). Versi�n que modifica el vector (no const).
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T>& Vector2<T>::Scale (const Vector2<T>& otro)
{
  x *= otro.x;
  y *= otro.y;
  
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Producto por componentes (escalado). Versi�n que devuelve un vector modificado (const).
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> Vector2<T>::GetScaled (const Vector2<T>& otro) const
{
  return Vector2<T>(x * otro.x, y * otro.y);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Cambia cada componente del vector actual por la correspondiente del vector pasado en el par�metro si
/// esta es menor (su tipo devuelve 'true' al operador '<')
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const Vector2<T>& Vector2<T>::SetIfMinComponents (const Vector2<T>& otro)
{
  if(otro.x < x)
  {
    x = otro.x;
  }
  if(otro.y < y)
  {
    y = otro.y;
  }

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Cambia cada componente del vector actual por la correspondiente del vector pasado en el par�metro si
/// esta es mayor (su tipo devuelve 'true' al operador '>')
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const Vector2<T>& Vector2<T>::SetIfMaxComponents (const Vector2<T>& otro)
{
  if(otro.x > x)
  {
    x = otro.x;
  }
  if(otro.y > y)
  {
    y = otro.y;
  }

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de acceso []
/// (Versi�n const)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T Vector2<T>::operator [] (int i) const
{
  return ((T*)this)[i];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Operador de acceso []
/// (Versi�n no const)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T& Vector2<T>::operator [] (int i)
{
  return ((T*)this)[i];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n de normalizaci�n
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T>& Vector2<T>::Normalize (const T tk)
{
  T len = (T)sqrt(x * x + y * y);

#ifdef CHECK_MATH_SINGULARITY
  //---------------------------------------------------<<<
  ASSERTMSG(len != Zero<T>(), "Divisi�n por cero en normalizaci�n de vector");
  if(len != Zero<T>())
  {
    T aux = tk / len;
    x *= aux;
    y *= aux;
  }
  //--------------------------------------------------->>>
#else
  //---------------------------------------------------<<<
  T aux = tk / len;
  x *= aux;
  y *= aux;
  //--------------------------------------------------->>>
#endif 
    
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve un vector igual al actual pero normalizado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T> Vector2<T>::GetNormalized () const
{
  return (Vector2<T>(*this).Normalize());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Longitud
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline T Vector2<T>::Length () const
{
  return ((T)sqrt(x * x + y * y));
}

template<> 
inline int32 Vector2<int32>::Length () const
{
  return ((int32)sqrt((float)(x * x + y * y)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Longitud al cuadrado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline T Vector2<T>::SquaredLength () const
{
  return (x * x + y * y);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Rotaci�n del vector
/// Angulos positivos producen un giro antihorario
/// NOTA: Rotaci�n b�sica de la librer�a matem�tica. Matricialmente:
/// 
/// | cos a   -sen a |   | x |   | x*cos a - y*sen a |
/// |                | . |   | = |                   |
/// | sen a    cos a |   | y |   | s*sen a + y*cos a |
/// 
/// Define una rotaci�n con un �ngulo positivo como antihoraria, regla del 'tornillo saliente'.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T>& Vector2<T>::Rotate(const T angle)
{
  /*T sina = (T)sin(angle);
  T cosa = (T)cos(angle);*/
  T sina;
  T cosa;

  mathUtils::SinCos(angle, sina, cosa);

  T aux_x = x;
  T aux_y = y;

  x = cosa * aux_x - sina * aux_y;
  y = sina * aux_x + cosa * aux_y;

  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Devuelve un vector rotado 'angle' radianes respecto al actual
/// Sentido antihorarios para �ngulos positivos
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline Vector2<T> Vector2<T>::GetRotated(const T angle) const
{
  return (Vector2<T>(*this).Rotate(angle));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Angulo del vector
/// Origen en el eje X
/// NOTA (extraida de la ayuda de la librer�a run-time de microsoft):
///      'atan2' devuelve el arcotangente de y/x en el rango [�p, p] radianes.
///      Si los dos par�metros son 0 'atan2' devuelve 0.
///      'atan2' est� bien definida para todo punto menos el origen. Tambi�n si x==0 siendo y!=0.
///      'atan2' utiliza los signos de los dos par�metros para determinar el cuadrante del valor de retorno.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> 
inline T Vector2<T>::GetAngle () const
{
  return (T)atan2(y, x);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Interpolaci�n lineal entre el vector actual y el pasado en el par�metro
/// usando t como 'lambda', teniendo que si:
///    t=0 --> actual
///    t=1 --> otro
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T>& Vector2<T>::Lerp (const Vector2<T>& otro, const T t)
{
  x += (otro.x - x) * t;
  y += (otro.y - y) * t;
  
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Interpolaci�n lineal entre el vector actual y el pasado en el par�metro
/// usando t como 'lambda', teniendo que si:
///    t=0 --> actual
///    t=1 --> otro
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> Vector2<T>::GetLerp (const Vector2<T>& otro, const T t) const
{
  return Vector2<T>(*this).Lerp(otro, t);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n externa: Devuelve un vector con las componentes m�nimas de
/// los pasados en los par�metros
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> VectorMinComponents (const Vector2<T>& uno, const Vector2<T>& otro)
{
  return Vector2<T>(mathUtils::Min(uno.x, otro.x),
                    mathUtils::Min(uno.y, otro.y));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Funci�n externa: Devuelve un vector con las componentes m�ximas de
/// los pasados en los par�metros
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline Vector2<T> VectorMaxComponents (const Vector2<T>& uno, const Vector2<T>& otro)
{
  return Vector2<T>(mathUtils::Max(uno.x, otro.x),
                    mathUtils::Max(uno.y, otro.y));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
