#version 330 core
in vec4 gl_FragCoord;

out vec4 frag_color;

#define MAX_ITERATIONS 500

int get_iterations()
{
  float real=(gl_FragCoord.x/1000.-.5)*4.;
  float imag=(gl_FragCoord.y/1000.-.375)*4.;
  
  int iterations=0;
  float const_real=real;
  float const_imag=imag;
  
  while(iterations<MAX_ITERATIONS)
  {
    float tmp_real=real;
    real=(real*real-imag*imag)+const_real;
    imag=(2.*tmp_real*imag)+const_imag;
    
    float dist=real*real+imag*imag;
    
    if(dist>4.)
    break;
    
    ++iterations;
  }
  return iterations;
}

vec3 gradient(float n){
  return vec3(3 * n / MAX_ITERATIONS, 3 * n / MAX_ITERATIONS - 1, 3 * n / MAX_ITERATIONS - 2)
}

vec4 return_color()
{
  int iter=get_iterations();
  if(iter==MAX_ITERATIONS)
  {
    gl_FragDepth=0.f;
    return vec4(0.f,0.f,0.f,1.f);
  }
  
  float iterations=float(iter)/MAX_ITERATIONS;
  return vec4(gradient(iterations),1.f);
}

void main()
{
  frag_color=return_color();
}