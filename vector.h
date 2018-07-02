typedef struct
{
  double x;
  double y;
  double z;
}vector;

vector operator+(const vector& a, const vector& b);
vector operator+(const vector& a, const double& b);
vector operator+(const double& a, const vector& b);
double operator*(const vector& a, const vector& b);
vector operator^(const vector& a, const vector& b);
vector operator*(const double& a, const vector& b);
vector operator*(const vector& a, const double& b);
vector operator-(const vector& a, const vector& b);
vector operator-(const vector& a, const double& b);
vector operator/(const vector& a, const double& b);
void operator+=(vector& a, const vector& b);
void operator+=(vector& a, const double& b);
void operator-=(vector& a, const vector& b);
void operator-=(vector& a, const double& b);

double Magnitude(const vector& vec);
double Magnitude_Sq(const vector& vec);
double Magnitude_Cubed(const vector& vec);
double radius(const vector& BD1, const vector& BD2);

