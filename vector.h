typedef struct
{
  double x;
  double y;
  double z;
}vector;

vector operator+(vector a, vector b);
vector operator+(vector a, double b);
vector operator+(double a, vector b);
double operator*(vector a, vector b);
vector operator^(vector a, vector b);
vector operator*(double a, vector b);
vector operator*(vector a, double b);
vector operator-(vector a, vector b);
vector operator/(vector a, double b);

double Magnitude( vector vec );
double Magnitude_Sq( vector vec );
double radius( vector BD1, vector BD2 );

